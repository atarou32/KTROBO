#include "KTRoboLuaCMesh.h"

using namespace KTROBO;

int CMesh::getMesh(char* path_without_dot) {
	// �܂��͂��łɃ��[�h����Ă��邩�ǂ������肷��
	if (meshname_index.find(path_without_dot) != meshname_index.end()) {
		// ���������̂�
		// meshname_index��Ԃ�
		return meshname_index[path_without_dot];
	} else {
		execConstructOrDestruct();
		// ���[�h����Ă��Ȃ������烍�[�h�������K�v������ǂ�
		// ����̓��[�h�X���b�h�ł̂݉\
		// �X�V�X���b�h�Ń��b�V�����擾���邽�߂ɂ��̃��\�b�h�����[�h���ĂȂ����̂��Ă񂾏ꍇc++���ɗ�O�����
		// ����͎��O�ɕK�v�Ȃ��̂��킩���Ă���A���̂��̂��ω����Ȃ��ꍇ�ɂ̂ݑΉ��ł���
		// ���̂��ω�����ꍇ�� copy���Ƃ��� �����𔽉f���Ă��̃R�s�[�ƍX�V�����Ŏg���Ă�����̂����ւ���K�v������
		// ���Ԃ�finishcommandlist �����execute���ꂽ���ǂ������T�u�X���b�h���m�邱�Ƃ͂ł��Ȃ��H�Ǝv����
		// MeshInstanced�̃R�s�[�����ɂ̓e�N�X�`���̃R�s�[�����������Ă��邪�A�C���f�b�N�X���R�s�[���Ă���̂�
		// �������ςȂƂ�����㏑���͂��Ȃ��悤�ɂ͂Ȃ��Ă���E�E�E�@�܂�I�b�P�[�Ƃ������Ƃł���I
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

	// �R�s�[����Ƃ��͎��g��src�̍X�V�����j�����Ȃ��悤�ɂ��邱��
	// �R�s�[�����@�u�R�s�[�悪�ӔC�������b�V���v�������Ă��Ȃ��悤�ɂ��邱��
	// ���̃R�s�[�͌y���͂��i���\�[�X�Ȃǂ̐����Ȃǂ͂Ȃ��A�����������Ă�����̂̔j���͂Ȃ��͂��E�E�E�j
	// �j���Ɋւ��Ă͗�O�𓊂���̂ł͂Ȃ��@�j�����s�����Ƃɂ���
	// �܂�has_responsibility ��true�̂��̂������ꍇ�͒x�������ɂȂ�
#ifdef _DEBUG_DEBUGDAYO
	{
	// �R�s�[�悪�ӔC�������b�V�� 
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
			// mesh �̃f���[�g
			me->mesh->Release();
			delete me->mesh;
			me->mesh = 0;
		}
		delete me;
		me = 0;
	}
	meshs.clear();
	meshname_index.clear();

	// meshs �̃R�s�[
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
		// ���������̂�
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
		// ���̂͏����Ȃ�
	}

}


void CMesh::deleteALL() {
	
	// �S������
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