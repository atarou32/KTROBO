#include "KTRoboLuaCMesh.h"

using namespace KTROBO;

int CMesh::getMesh(char* path_without_dot) {
	// まずはすでにロードされているかどうか判定する
	if (meshname_index.find(path_without_dot) != meshname_index.end()) {
		// 見つかったので
		// meshname_indexを返す
		return meshname_index[path_without_dot];
	} else {
		execConstructOrDestruct();
		// ロードされていなかったらロード処理が必要だけれども
		// これはロードスレッドでのみ可能
		// 更新スレッドでメッシュを取得するためにこのメソッドをロードしてないものを呼んだ場合c++側に例外が飛ぶ
		// これは事前に必要なものがわかっており、そのものが変化しない場合にのみ対応できる
		// ものが変化する場合は copyをとって 差分を反映してそのコピーと更新処理で使っているものを入れ替える必要がある
		// たぶんfinishcommandlist されてexecuteされたかどうかをサブスレッドが知ることはできない？と思われる
		// MeshInstancedのコピー処理にはテクスチャのコピー処理が入ってくるが、インデックスをコピーしているので
		// 差分が変なところを上書きはしないようにはなっている・・・　つまりオッケーということである！
		Mesh* m = new Mesh();
		char meshname[512];
		char animename[512];
		char path[256];
		memset(meshname,0,512);
		memset(animename,0,512);
		memset(path,0,256);
		mystrcpy(path,255,0,path_without_dot);
		path[255] = '\0';
		sprintf_s(meshname, "%s.MESH", path);
		sprintf_s(animename, "%s.ANIME", path);
		m->readMesh(g, meshname, tex_loader);
		m->readAnime(animename);
		m->animate(0, true);
		Meshdayo* mm =  new Meshdayo();
		mm->has_responsibility = true;
		mm->mesh = m;
		int size = meshname_index.size();
		meshs.push_back(mm);
		meshname_index.insert(pair<string,int>((path_without_dot), size));
		return size;
	}
}
bool CMesh::getIsLoad() {
	return Loadable::getIsLoad();
}

void CMesh::setIsLoad(bool t) {
	Loadable::setIsLoad(t);
}

void CMesh::copyDAYO(CMesh* src) {

	// コピーするときは自身とsrcの更新生成破棄がないようにすること
	// コピー元が　「コピー先が責任を持つメッシュ」を持っていないようにすること
	// このコピーは軽いはず（リソースなどの生成などはなく、自分が持っているものの破棄はないはず・・・）
	// 破棄に関しては例外を投げるのではなく　破棄を行うことにした
	// つまりhas_responsibility がtrueのものが多い場合は遅い処理になる
#ifdef _DEBUG_DEBUGDAYO
	{
	// コピー先が責任を持つメッシュ 
	int size = meshs.size();
	for (int i=0;i<size;i++) {
		Meshdayo* me = meshs[i];
		if (me->has_responsibility) {
			int isize = src->meshs.size();
			for (int k=0;k<isize;k++) {
				Meshdayo* mee = src->meshs[k];
				if (mee->mesh == me->mesh) {
					throw new GameError(KTROBO::WARNING, "cmech copydayo dest has responsibility of src");
				}
			}
		}
	}

#endif
	int size = meshs.size();
	for (int i=0;i<size; i++) {
		Meshdayo* me = meshs[i];
		if (me->has_responsibility) {
			// mesh のデリート
			me->mesh->Release();
			delete me->mesh;
			me->mesh = 0;
		}
		delete me;
		me = 0;
	}
	meshs.clear();
	meshname_index.clear();

	// meshs のコピー
	int isize = src->meshs.size();
	for (int i=0;i<isize;i++) {
		Meshdayo* m = src->meshs[i];
		Meshdayo* me = new Meshdayo();
		me->has_responsibility = false;
		me->mesh = m->mesh;
		meshs.push_back(me);
	}
	meshname_index = src->meshname_index;
}

void CMesh::deleteMesh(char* path_without_dot) {
	if (meshname_index.find(path_without_dot) != meshname_index.end()) {
		// 見つかったので
		execConstructOrDestruct();
		int index = meshname_index[path_without_dot];
		if (meshs[index]->has_responsibility) {
			meshs[index]->mesh->Release();
			meshs[index]->has_responsibility = false;
			delete meshs[index]->mesh;
			meshs[index]->mesh =0;
			meshname_index.erase(meshname_index.find(path_without_dot));
			
		} else {
			meshname_index.erase(meshname_index.find(path_without_dot));
		}
		// 実体は消さない
	}

}


void CMesh::deleteALL() {
	
	// 全部消す
	execConstructOrDestruct();
	meshname_index.clear();
	vector<Meshdayo*>::iterator it = meshs.begin();
	while(it != meshs.end()) {
		Meshdayo* m = *it;
		if (m->has_responsibility) {
			m->mesh->Release();
			delete m->mesh;
			m->mesh = 0;
		}
		m->has_responsibility = false;
		delete m;
		m = 0;
		it = it + 1;
	}
	meshs.clear();
}

Mesh* CMesh::getCollectedMesh(int index) {

	int size = meshs.size();
	if (size > index && index >= 0) {
		return meshs[index]->mesh;
	}
	throw new GameError(KTROBO::WARNING, "out range vector of CMESH");
}

CMesh::CMesh(Graphics* g, MyTextureLoader* tex_loader) {
	this->g = g;
	this->tex_loader = tex_loader;
}

CMesh::~CMesh() {
	deleteALL();
}


CMeshs::CMeshs(Graphics* g, MyTextureLoader* tex_loader) {
	this->g = g;
	this->tex_loader = tex_loader;
}
CMeshs::~CMeshs() {
	vector<CMesh*>::iterator it = this->cmeshs.begin();
	while (it != cmeshs.end()) {
		CMesh* cm = *it;
		delete cm;
		cm = 0;
		it = it +1;
	}
	cmeshs.clear();
}

int CMeshs::makeInstance() {
	CMesh* cm = new CMesh(g, tex_loader);
	int size = cmeshs.size();
	cmeshs.push_back(cm);
	return size;
}
CMesh* CMeshs::getInstance(int index) {
	if (index >= 0 && cmeshs.size() > index) {
		return cmeshs[index];
	} else {
		throw new GameError(KTROBO::WARNING, "vector output range of cmesh get instance");
	}
}
ICMesh* CMeshs::getInterface(int index) {
	if (index >= 0 && cmeshs.size() > index) {
		return cmeshs[index];
	} else {
		throw new GameError(KTROBO::WARNING, "vector output range of cmesh get instance");
	}
}