#include "KTRoboGame.h"
#include "KTRoboGraphics.h"
#include "KTRoboTask.h"
#include "KTRoboClock.h"
#include "KTRoboCS.h"
#include "KTRoboGameError.h"
#include "stringconverter.h"
#include "tolua_glue/tolua_glue.h"


using namespace KTROBO;


// タスクスレッドの分割
// メインスレッド：描画（毎回処理が走る）
// TASK0:  描画のための更新処理（アニメの計算処理）など
// TASK1:  オブジェクトの生成・破棄処理(ロード）　必要なときだけ走る 
// 生成処理破棄処理（コンストラクタデストラクタ呼び出し）をする場合は
    // 他のスレッドが使わないことを確認または使えない状態にしてから
    // 処理を行うこと
    // 方法その１　スレッドのロックをかけて他のスレッドの処理が走らないようなタイミングを作る
    //             これは読み込みロード処理が長くなりそうであることを考えると有効な方法とはいえないような気がする
    // 方法その２　ロードしたオブジェクトをもっているコレクションクラスを他のスレッドが使用しないような仕組みを作る
    //             これはロード専用コレクションクラスを作ることのような気がする
    // 方法その２をより具体的にどう実装するか　
    // いつかはロードしたオブジェクトをもつコレクションクラスも利用されることを考えると
    // 渡すさいのタイミングに気をつけなければならない→ロックする（ロードしている間ロックしているよりは普通のような気がする）
    // 

// TASK2:  オブジェクトの更新処理・AI？毎回処理が走る メイン進行処理
// TASK3:  衝突・位置更新処理：毎回処理が走る
// TASK4:  入力取得および入力の解釈記憶処理

Game::Game(void)
{
	L =0;
	g = 0;
	this->timeEnd.QuadPart = 0;
	this->timeStart.QuadPart = 0;
	this->timeFreq.QuadPart = 1;
	fps = 0;
	c = 0;



	timestamp = 0;
	big_timestamp = 0;
	second = 0;
	dmsecond = 0;
	dt = 0;

	demo = 0;
	telop_texts = 0;

	for (int i = 0 ; i <TASKTHREAD_NUM; i++) {
		task_threads[i] = 0;
		g_for_task_threads[i] = 0;
	}

	mesh = 0;
	mesh2 = 0;
	for (int i = 0 ; i < 11; i++) {
		mesh3[i] = 0;
	}

	mesh_instanceds = 0;
	cltf=0;
	texdayo = 0;
	mytest_for_vt = 0;
	cmeshs = 0;
}


Game::~Game(void)
{
}

void RENDERTCB(TCB* thisTCB) {
	
	Game* g = (Game*)(thisTCB->data);

	g->Run();
}


void CALCCOMBINEDTCB(TCB* thisTCB) {
	
	MeshInstanceds* mis = (MeshInstanceds*)thisTCB->data;
	Graphics* g = (Graphics*)thisTCB->Work[0];

	//if (!mis->getIsLoad()) {
		
		mis->loadAnimeMatrixBasisToTexture(g);
		mis->loadMatrixLocalToTexture(g);
		mis->calcCombinedMatrixToTexture(g);
		mis->loadColorToTexture(g);

	//}
}



void LOADMESHTCB(TCB* thisTCB) {
	

	
	lua_State* L = (lua_State*)thisTCB->data;
	Task* t = (Task*)thisTCB->Work[0];
	Graphics* g = (Graphics*)thisTCB->Work[1];
	MeshInstanceds* m = (MeshInstanceds*)thisTCB->Work[2];
	

	char buff[] = "resrc/script/sample.lua.txt";
	int error;
	try {
		error = luaL_loadfile(L, buff) || lua_pcall(L, 0, 0, 0);
	//	m->setIsLoad(false);
	}catch (GameError* err) {
		
	//	MessageBoxA(g->getHWND(), err->getMessage(), err->getErrorCodeString(err->getErrorCode()), MB_OK);
		delete err;
	}
    if (error) {
		mylog::writelog("errtxt.txt", "%s", lua_tostring(L, -1));
		OutputDebugStringA(lua_tostring(L,-1));
        lua_pop(L, 1);
    } else {
	
		t->kill(thisTCB);
	}
	Sleep(1);
}

bool Game::Init(HWND hwnd) {
	// 順番をかえないこと cs とタスクの間に依存関係がある
	
	g = new Graphics();
	if (!g->Init(hwnd)) {
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "graphics init error");
	}

	KTROBO::CS::instance()->Init();
	for (int i = 0 ; i <TASKTHREAD_NUM; i++) {
		task_threads[i] = Task::factory(hwnd);
		g_for_task_threads[i] = g->makeGraphicsOfNewDeviceContext();
	}

	Mesh::Init(g);
	MyTokenAnalyzer::Init();

	demo = new KTRoboDemoRender();
	demo->Init(g);
	c = new Clock(0,0,0);

	KTROBO::DebugTexts::instance()->Init(g, demo->font);
	KTROBO::DebugTexts::instance()->setText(g, 14 , L"少なくとも頑張れる余地はある");

	telop_texts = new TelopTexts();
	telop_texts->Init(g,demo->font);
	telop_texts->readFile(g,"resrc/sample/terop.txt",30,14,&MYVECTOR4(1,1,1,1),0.03);

	mesh = new Mesh();
	mesh->readMesh(g, "resrc/model/ponko2-3/pk2skirt.MESH", demo->tex_loader);
	mesh->readAnime("resrc/model/ponko2-3/pk2skirt.ANIME");
	mesh2 = new Mesh();
	mesh2->readMesh(g, "resrc/model/ponko2-4/pk2sailordayo.MESH", demo->tex_loader);
	mesh2->readAnime("resrc/model/ponko2-4/pk2sailordayo.ANIME");
	mesh3[0] = new Mesh();
	mesh3[0]->readMesh(g, "resrc/model/ponko2-3/pk2face.MESH", demo->tex_loader);
	mesh3[0]->readAnime("resrc/model/ponko2-3/pk2face.ANIME");

	mesh3[1] = new Mesh();
	mesh3[1]->readMesh(g, "resrc/model/ponko2-3/pk2hidarime.MESH", demo->tex_loader);
	mesh3[1]->readAnime("resrc/model/ponko2-3/pk2hidarime.ANIME");

	mesh3[2] = new Mesh();
	mesh3[2]->readMesh(g, "resrc/model/ponko2-3/pk2hidarimedama.MESH", demo->tex_loader);
	mesh3[2]->readAnime("resrc/model/ponko2-3/pk2hidarimedama.ANIME");

	mesh3[3] = new Mesh();
	mesh3[3]->readMesh(g, "resrc/model/ponko2-3/pk2kami.MESH", demo->tex_loader);
	mesh3[3]->readAnime("resrc/model/ponko2-3/pk2kami.ANIME");

	mesh3[4] = new Mesh();
	mesh3[4]->readMesh(g, "resrc/model/ponko2-3/pk2kuti.MESH", demo->tex_loader);
	mesh3[4]->readAnime("resrc/model/ponko2-3/pk2kuti.ANIME");

	mesh3[5] = new Mesh();
	mesh3[5]->readMesh(g, "resrc/model/ponko2-3/pk2kubi.MESH", demo->tex_loader);
	mesh3[5]->readAnime("resrc/model/ponko2-3/pk2kubi.ANIME");

	mesh3[6] = new Mesh();
	mesh3[6]->readMesh(g, "resrc/model/ponko2-3/pk2mayuge.MESH", demo->tex_loader);
	mesh3[6]->readAnime("resrc/model/ponko2-3/pk2mayuge.ANIME");
/*
	mesh3[7] = new Mesh();
	mesh3[7]->readMesh(g, "resrc/model/ponko2-3/pk2midarime.MESH", demo->tex_loader);
	mesh3[7]->readAnime("resrc/model/ponko2-3/pk2midarime.ANIME");
*/
/*
	mesh3[8] = new Mesh();
	mesh3[8]->readMesh(g, "resrc/model/ponko2-3/pk2migimedama.MESH", demo->tex_loader);
	mesh3[8]->readAnime("resrc/model/ponko2-3/pk2migimedama.ANIME");
	*/
	mesh3[7] = new Mesh();
	mesh3[7]->readMesh(g, "resrc/model/ponko2-4/pk2migihanddayo.MESH", demo->tex_loader);
	mesh3[7]->readAnime("resrc/model/ponko2-4/pk2migihanddayo.ANIME");
	mesh3[7]->RootBone->parent_bone = mesh2->Bones[mesh2->BoneIndexes["migiArmTekubiBone"]];
	mesh3[7]->RootBone->parent_bone_index = mesh2->BoneIndexes["migiArmTekubiBone"];
	mesh3[7]->RootBone_connect_without_material_local = true;

	mesh3[10] = new Mesh();
	mesh3[10]->readMesh(g, "resrc/model/ponko2-4/pk2migihanddayo.MESH", demo->tex_loader);
	mesh3[10]->readAnime("resrc/model/ponko2-4/pk2migihanddayo.ANIME");
	mesh3[10]->RootBone->parent_bone;// = mesh2->Bones[mesh2->BoneIndexes["migiArmTekubiBone"]];
	mesh3[10]->RootBone->parent_bone_index;// = mesh3[9]->RootBone->parent_bone->bone_index;
	//mesh3[10]->RootBone_connect_without_material_local = true;

	
	mesh3[9] = new Mesh();
	mesh3[9]->readMesh(g, "resrc/model/bazooka/rweaponbazookaguna.MESH", demo->tex_loader);
	mesh3[9]->readAnime("resrc/model/bazooka/rweaponbazookaguna.ANIME");
//	mesh3[9]->RootBone->parent_bone = /*mesh2->Bones[mesh2->BoneIndexes["migiArmTekubiBone"]];*/mesh3[9]->Bones[mesh3[9]->BoneIndexes["MigiHandMotiBone"]];
//	mesh3[9]->RootBone->parent_bone_index = mesh3[10]->RootBone->parent_bone->bone_index;
//	mesh3[9]->RootBone_connect_without_material_local = false;

	mesh3[8] = new Mesh();
	mesh3[8]->readMesh(g, "resrc/model/bazooka/rweaponbazookaguna.MESH", demo->tex_loader);
	mesh3[8]->readAnime("resrc/model/bazooka/rweaponbazookaguna.ANIME");
	mesh3[8]->RootBone->parent_bone = mesh3[7]->Bones[mesh3[7]->BoneIndexes["MigiHandMotiBone"]];
	mesh3[8]->RootBone->parent_bone_index = mesh3[7]->BoneIndexes["MigiHandMotiBone"];
	mesh3[8]->RootBone_connect_without_material_local = false;
	
	
	MYMATRIX worldforg;
	MyMatrixRotationZ(worldforg, 3.14/2);
	MyMatrixMultiply(mesh3[8]->rootbone_matrix_local_kakeru, mesh3[8]->rootbone_matrix_local_kakeru, worldforg);
	
	MyMatrixScaling(worldforg, 1/3.0,1/3.0,1/3.0);
	MyMatrixMultiply(mesh3[8]->rootbone_matrix_local_kakeru, mesh3[8]->rootbone_matrix_local_kakeru, worldforg);
	
	MyMatrixRotationY(worldforg, -3.14/2);
	MyMatrixMultiply(mesh3[8]->rootbone_matrix_local_kakeru, mesh3[8]->rootbone_matrix_local_kakeru, worldforg);
	
	
	/*
	MyMatrixRotationX(worldforg, -3.14/4*2);
	MyMatrixMultiply(mesh3[10]->RootBone->matrix_local, mesh3[10]->RootBone->matrix_local, worldforg);
	MyMatrixRotationZ(worldforg, 3.14/2);
	MyMatrixMultiply(mesh3[10]->RootBone->matrix_local, mesh3[10]->RootBone->matrix_local, worldforg);

	MyMatrixRotationY(worldforg, 3.14/2);
	MyMatrixMultiply(mesh3[10]->RootBone->matrix_local, mesh3[10]->RootBone->matrix_local, worldforg);
	MyMatrixRotationZ(worldforg, 3.14);
	MyMatrixMultiply(mesh3[10]->RootBone->matrix_local, mesh3[10]->RootBone->matrix_local, worldforg);
	MyMatrixRotationY(worldforg, 3.14/2);
	MyMatrixMultiply(mesh3[10]->RootBone->matrix_local, mesh3[10]->RootBone->matrix_local, worldforg);
	*/


	float frame = 0;

	//mesh->animate(frame, true);
	mesh2->animate(frame, true);
	for (int i = 0 ; i <= 10 ; i++) {
		mesh3[i]->animate(frame, true);
		if (i ==9) {
			mesh3[i]->animate(0,true);
		}
	}


	MeshInstanceds::Init(g);
	mesh_instanceds = new MeshInstanceds(g, this->demo->tex_loader);
	//mesh_instanceds->setSkeleton(mesh);
	
	MYMATRIX kakeru;
	MyMatrixIdentity(kakeru);

	

	MYMATRIX worl;

	for (int i = 0 ; i < 30;i++) {
	
		mesh_is[i] = mesh_instanceds->makeInstanced(mesh2,mesh2,NULL,NULL,false,&kakeru);
		mesh_is2[i] = mesh_instanceds->makeInstanced(mesh,mesh,NULL,NULL,false,&kakeru);
		MyMatrixTranslation(worl,2, i*3,2);
		mesh_is[i]->setWorld(&worl);
		mesh_is2[i]->setWorld(&worl);
	}
	mesh_i = mesh_instanceds->makeInstanced(mesh2,mesh2,NULL,NULL,false,&kakeru);
	mesh_i2 = mesh_instanceds->makeInstanced(mesh,mesh,NULL,NULL,false, &kakeru);
	
	MeshInstanced* mesh_i3 = mesh_instanceds->makeInstanced(mesh3[10],mesh3[10],mesh_i,mesh2->BoneIndexes["migiArmTekubiBone"],true,&kakeru);

	//MYMATRIX worldforg;
	MyMatrixRotationZ(worldforg, 3.14/2);
	MyMatrixMultiply(kakeru, kakeru, worldforg);
	
	MyMatrixScaling(worldforg, 1/3.0,1/3.0,1/3.0);
	MyMatrixMultiply(kakeru, kakeru, worldforg);
	
	MyMatrixRotationY(worldforg, -3.14/2);
	MyMatrixMultiply(kakeru, kakeru, worldforg);


	mesh_instanceds->makeInstanced(mesh3[9],mesh3[9], mesh_i3, mesh3[10]->BoneIndexes["MigiHandMotiBone"],false, &kakeru);
	MYVECTOR4 colors[KTROBO_MESH_INSTANCED_COLOR_MAX];
	memset(colors,0,sizeof(colors));



/*	for (int i=0;i<KTROBO_MESH_INSTANCED_COLOR_MAX; i++) {
		colors[i].x = 1;
		colors[i].y = 1;
		colors[i].z = 1;
		colors[i].w = 1;
	}

	mesh_i->setColor(colors);
	*/





//	mytest_for_vt = new MyTestForVertexTexture();
//	mytest_for_vt->Init(g);
//	mytest_for_vt->readVertexTexture(g,mesh_instanceds->combined_matrix_texture);//anime_matrix_basis_texture);//matrix_local_texture);


//	mytest_for_vt->writeInfo(g);

	

	long work[TASK_WORK_SIZE];
	memset(work,0, sizeof(work));
	work[0] = (long)g_for_task_threads[TASKTHREADS_UPDATEANIMEFRAMENADO];

	task_threads[TASKTHREADS_UPDATEANIMEFRAMENADO]->make(CALCCOMBINEDTCB,mesh_instanceds,work,0x0000FFFF);
	
	

    int error;
	//Sleep(1000);
    L = luaL_newstate();
    luaL_openlibs(L);
	cltf = new TextFromLuas(g);
	cmeshs = new CMeshs(g, demo->tex_loader);
	MyLuaGlueSingleton::getInstance()->setColCMeshs(cmeshs);
	MyLuaGlueSingleton::getInstance()->setColTextFromLuas(cltf);
	MyLuaGlueSingleton::getInstance()->setColMeshInstanceds(mesh_instanceds);
	MyLuaGlueSingleton::getInstance()->registerdayo(L);
	
	

	Texture::Init(g);
	
	texdayo = new Texture(demo->tex_loader);
	
	int i = texdayo->getTexture("resrc/model/ponko-niyake.png");
	
/*	int j = texdayo->getRenderTex(i,0xFFFFFFFF,50,0,200,200,0,0,512,512);
	
	texdayo->setRenderTexIsRender(j,true);
	*/
	MYMATRIX jiken;
	MeshBone* bon = mesh2->Bones[mesh2->BoneIndexes["migiArmBone"]];
	MyMatrixMultiply(jiken,bon->combined_matrix,bon->matrix_local);
	MYVECTOR3 ppos(0,0,0);
	MyVec3TransformCoord(ppos,ppos,jiken);
	MyMatrixTranslation(jiken,ppos.float3.x,ppos.float3.y,ppos.float3.z);
//	j = texdayo->getRenderTex(i,0xFFFFFFFF,250,200,312,312,0,0,512,512);
//	texdayo->setRenderTexIsRender(j,true);
//		j = texdayo->getRenderTex(i,0xFFFFFFFF,150,200,512,512,0,0,512,512);
//	texdayo->setRenderTexColor(i,0xFFFF0022);
//	texdayo->setRenderTexIsRender(j,true);
//	j = texdayo->getRenderTex(i,0xFFFFFFFF,350,700,200,200,50,50,100,100);
//	texdayo->setRenderTexIsRender(j,true);
//	j = texdayo->getRenderTex(i,0xFFFFFFFF,450,700,200,200,50,50,100,100);
//	texdayo->setRenderTexIsRender(j,true);
//	j = texdayo->getRenderTex(i,0xFFFFFFFF,550,700,200,200,50,50,100,100);
//	texdayo->setRenderTexIsRender(j,true);
//	j = texdayo->getRenderTex(i,0xFFFFFFFF,650,700,200,200,50,50,100,100);
//	texdayo->setRenderTexIsRender(j,true);
//	j = texdayo->getRenderTex(i,0xFFFFFFFF,750,700,200,200,50,50,100,100);
//	texdayo->setRenderTexIsRender(j,true);
	MYMATRIX world;
	MYMATRIX view;
	MYMATRIX proj;
//	MYVECTOR3 from(0,-1,0);
	MYVECTOR3 from(5,-5,12);
	MYVECTOR3 at(0,0,0);
	MYVECTOR3 up(0,0,1);
	MyMatrixIdentity(world);
//	MyMatrixTranslation(world,0,0.05,0.0);

	
//	MyMatrixRotationZ(world, 0.5f);
	MyMatrixLookAtRH(view,from,at,up);
	MyMatrixPerspectiveFovRH(proj, 1, g->getScreenWidth() / (float)g->getScreenHeight(), 1, 1000);

	int j = texdayo->getRenderBillBoard(i,0xFFFFFFFF,&world,0.021,0.021,0,0,500,500);
	texdayo->setRenderBillBoardIsRender(j,true);
	MyMatrixTranslation(world,0,0,0);
	texdayo->getRenderBillBoard(i,0xFFFF00FF,&world,0.30,0.30,100,100,250,250);
	texdayo->setRenderBillBoardIsRender(1,true);


	MyMatrixTranslation(world,4,0,0);
	texdayo->getRenderBillBoard(i,0xFF0000FF,&world,3,3,100,100,250,250);
	texdayo->setRenderBillBoardIsRender(1,true);

	MyMatrixTranslation(world,0,0,4);
	texdayo->getRenderBillBoard(i,0xFFFFFFFF,&world,13,13,100,100,250,250);
	texdayo->setRenderBillBoardIsRender(1,true);

	MyMatrixTranslation(world,0,4,0);
	texdayo->getRenderBillBoard(i,0x0000FFFF,&world,3,3,100,100,250,250);
	texdayo->setRenderBillBoardIsRender(1,true);

//	texdayo->setViewProj(g,&view,&proj,&from, &at);
	
	//long work[TASK_WORK_SIZE];
	memset(work,0, sizeof(work));
	work[1] = (long)g_for_task_threads[TASKTHREADS_LOADDESTRUCT];
	work[0] = (long)task_threads[TASKTHREADS_LOADDESTRUCT];
	work[2] = (long)mesh_instanceds;

	task_threads[TASKTHREADS_LOADDESTRUCT]->make(LOADMESHTCB,L,work,0x0000FFFF);


	memset(work,0,sizeof(work));
	task_threads[TASKTHREADS_UPDATEMAINRENDER]->make(RENDERTCB,this,work,0x0000FFFF);

	InputMessageDispatcher::registerImpl(&k,NULL,NULL);
	return true;
}
void Game::Del() {

	


	


	if (task_threads[TASKTHREADS_UPDATEMAINRENDER]) {
		task_threads[TASKTHREADS_UPDATEMAINRENDER]->deleteTask();
		delete task_threads[TASKTHREADS_UPDATEMAINRENDER];
		task_threads[TASKTHREADS_UPDATEMAINRENDER] = 0;
	}
	if(g_for_task_threads[TASKTHREADS_UPDATEMAINRENDER]) {
		g_for_task_threads[TASKTHREADS_UPDATEMAINRENDER]->Release();
		delete g_for_task_threads[TASKTHREADS_UPDATEMAINRENDER];
		g_for_task_threads[TASKTHREADS_UPDATEMAINRENDER] = 0;
	}

	// 順番を変えないこと　cs と　タスクの間に依存関係がある
	for (int i = 0 ; i <TASKTHREAD_NUM; i++) {
		if (i != TASKTHREADS_UPDATEMAINRENDER) {
		if (task_threads[i]) {
			task_threads[i]->deleteTask();
			delete task_threads[i];
			task_threads[i] = 0;
		}
		if(g_for_task_threads[i]) {
			g_for_task_threads[i]->Release();
			delete g_for_task_threads[i];
			g_for_task_threads[i] = 0;
		}
		}
	}

	

if (L) {
	lua_close(L);
}

	 if (cmeshs) {
		 delete cmeshs;
		 cmeshs = 0;
	 }

	 
	 if(cltf) {
		 delete cltf;
		 cltf= 0;
	 }

	 if (texdayo) {
		delete texdayo;
		texdayo = 0;
	}

	KTROBO::CS::instance()->Del();

	KTROBO::DebugTexts::instance()->Del();

	if (mytest_for_vt) {
		delete mytest_for_vt;
		mytest_for_vt = 0;
	}

	if (telop_texts) {
		telop_texts->Del();
		delete telop_texts;
		telop_texts = 0;
	}

	if (demo) {
		demo->Release();
		delete demo;
		demo = 0;
	}
	

	if (mesh_instanceds) {
		mesh_instanceds->Release();
		delete mesh_instanceds;
		mesh_instanceds = 0;
	}

	MeshInstanceds::Del();

	MyTokenAnalyzer::Del();
	Mesh::Del();


	

	Texture::Del();
	


	if (g) {
		g->Release();
		delete g;
		g = 0;
	}

	
	
	if (c) {
		delete c;
		c = 0;
	}

	if (mesh) {
		delete mesh;
		mesh = 0;
	}

	if (mesh2) {
		delete mesh2;
		mesh2 = 0;
	}
	for (int i = 0 ; i <= 10 ; i++) {
		if (mesh3[i]) {
		delete mesh3[i];
		mesh3[i] = 0;
		}
	}


}



void Game::startWatch() {
	QueryPerformanceCounter((LARGE_INTEGER*)&this->timeStart);
}

double Game::stopWatch() {
	QueryPerformanceCounter((LARGE_INTEGER*)&this->timeEnd);
	QueryPerformanceFrequency((LARGE_INTEGER*)&this->timeFreq);
	return (timeEnd.QuadPart - timeStart.QuadPart)/(double)timeFreq.QuadPart*1000;
}

 bool kurukuru::handleMessage(int msg, void* data, DWORD time) {

	 a = 0.01f;


	 MYMATRIX s;
	 MyMatrixRotationZ(s,a);

	 MYVECTOR3 fromat = from - at;
	 MyVec3TransformNormal(fromat,fromat,s);
	 from = at+fromat;

	 MyMatrixLookAtRH(view,from,at,up);

	 return true;
 }
void Game::Run() {

	double millisecond = stopWatch();
	startWatch();
	WCHAR test[512];
	char test2[1024];
	memset(test2,0,sizeof(1024));
	memset(test,0,sizeof(WCHAR)*512);


	if (millisecond > RENDERTIME_IGNORETIME) {
		CS::instance()->leave(CS_MAINTHREAD_CS, "leave main");
		Sleep(1);
		millisecond = RENDERTIME_SETTIME;
		CS::instance()->enter(CS_MAINTHREAD_CS, "enter main");
	} else if ( millisecond < RENDERTIME_SETTIME ) {
		CS::instance()->leave(CS_MAINTHREAD_CS, "leave main");
		Sleep(DWORD(RENDERTIME_SETTIME - millisecond));
		millisecond = RENDERTIME_SETTIME;
		CS::instance()->enter(CS_MAINTHREAD_CS, "enter main");
	} else {
		CS::instance()->leave(CS_MAINTHREAD_CS, "leave main");
		Sleep(1);
		CS::instance()->enter(CS_MAINTHREAD_CS, "enter main");
	}

	
	c->plus((float)millisecond);

	big_timestamp =	c->getBigTimeStamp();
	timestamp = c->getTimeStamp();
	dt = (float)millisecond;
	dmsecond = c->getDMSecond();
	second = c->getSecond();

	frameTime = (float)millisecond;
	if (millisecond > 0) {
		fps = (int)(1000 / (float)millisecond);
	}

	static int byouga_count = 0;
	static int byouga_count_second = 0;
		
	if (c->getSecond() != byouga_count_second) {
			byouga_count_second = c->getSecond();
			byouga_count = 0;
	}
	byouga_count++;
	if (byouga_count >= (int)(1000 / (float)RENDERTIME_SETTIME)) {
		// 描画しないでリターンする
		return;
	}



	//demo->Render(g);
	float clearColor[4] = {
		0.3f,0.4f,0.8f,1.0f};
	telop_texts->plusTime(g,millisecond);
	if (telop_texts->isRenderFinished()) {
		telop_texts->readFile(g,"resrc/sample/KTROBO.txt",30,14,&MYVECTOR4(1,1,1,1),0.1);
	}
	
	//demo->Render(g);

	static float frame = 0;
	static int frameint = 0;
	frame += millisecond/ (float)RENDERTIME_SETTIME * 0.1f;
	




	unsigned short animf[KTROBO_MESH_INSTANCED_BONE_MAX];
	unsigned short animl[KTROBO_MESH_INSTANCED_BONE_MAX];
	float animw[KTROBO_MESH_INSTANCED_BONE_MAX];


	if (frameint != (int)(frame*5)) {
		frameint = (int)(frame*5);
		if (frame >40) {
			frame = 0;
		}
	//frame = 30.001;
//	mesh->animate(frame, true);
	mesh2->animate(0, true);
	for (int i = 0 ; i <= 6 ; i++) {
//		mesh3[i]->animate(frame, true);
	}
	
//	mesh3[7]->animate(0,true);
//	mesh3[8]->animate(0,true);
//	mesh3[9]->animate(0,true);
//	mesh3[10]->animate(0,true);
	}


	
	
	//}
	
	//frame = 30.001;

	MYMATRIX world;
	MYMATRIX view;
	MYMATRIX proj;
	MyMatrixIdentity(world);
	MYVECTOR3 a(5,-5,12);
//	MYVECTOR3 a(0,10,40);
	MYVECTOR3 b(0,0,0);
	MYVECTOR3 up(0,0,1);
	MyMatrixLookAtRH(view, a, b, up);
	MyMatrixPerspectiveFovRH(proj, 1, g->getScreenWidth() / (float)g->getScreenHeight(), 1, 1000);
	
	
	MYMATRIX worldforg;
	









	MYMATRIX temp_world;
	MyMatrixTranslation(temp_world, 0,0,0);
	mesh_i->setWorld(&temp_world);
	mesh_instanceds->setViewProj(g, &view,&proj, &MYVECTOR4(1,1,-10,1));








	CS::instance()->enter(CS_MESSAGE_CS, "enter");
	InputMessageDispatcher::messageDispatch();
	view = k.view;
	b = k.at;
	a = k.from;
	CS::instance()->leave(CS_MESSAGE_CS, "leave");


	CS::instance()->enter(CS_DEVICECON_CS, "render game");

	

	

	memset(animf,0,sizeof(animf));
	memset(animl,0,sizeof(animl));
	memset(animw,0,sizeof(animw));
	static int count = 0;
	count++;

	int bsize = mesh2->Bones.size();
	
	//if (count % 2 ==0) {
	for (int i=0;i<bsize; i++) {

		MeshBone* bone = mesh2->Bones[i];
	//	mesh2->calculateOffsetMatrixToGetMinMaxAndWeight(bone, frame, &animl[i], &animf[i], &animw[i]);
		animf[i] = floor(frame);////floor(frame);//count * 0.3;
		animl[i] =ceil(frame);//count * 0.3;
		animw[i] = animl[i]- frame;
		//animw[i] = 1;//1- animw[i];
	}

	mesh_i->setBoneIndexInfo(animf, animl, animw);
	mesh_i2->setBoneIndexInfo(animf,animl,animw);
	for (int i=0;i<13;i++) {
		mesh_is[i]->setBoneIndexInfo(animf,animl,animw);
		mesh_is2[i]->setBoneIndexInfo(animf,animl,animw);
	}





	ID3D11RenderTargetView* v = g->getRenderTargetView();
	g->getDeviceContext()->OMSetRenderTargets(1, &v, Mesh::pDepthStencilView);
	g->getDeviceContext()->RSSetViewports(1, g->getViewPort());
	g->getDeviceContext()->ClearRenderTargetView(g->getRenderTargetView(),clearColor);
	g->getDeviceContext()->ClearDepthStencilView(Mesh::pDepthStencilView,  D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,1.0f, 0 );
	KTROBO::DebugTexts::instance()->render(g);
	telop_texts->render(g);


	

	for (int i = 0 ; i < 4; i++) {
		ID3D11CommandList* pd3dCommandList=0;
		HRESULT hr = g_for_task_threads[i]->getDeviceContext()->FinishCommandList(FALSE, &pd3dCommandList);
		if (FAILED(hr)) {
			throw new GameError(KTROBO::FATAL_ERROR, "failed in finishing list");
		}
		if (pd3dCommandList) {
			g->getDeviceContext()->ExecuteCommandList(pd3dCommandList,false);
			pd3dCommandList->Release();
			pd3dCommandList = 0;
		}
	}

	v = g->getRenderTargetView();
	g->getDeviceContext()->OMSetRenderTargets(1, &v, Mesh::pDepthStencilView);
	g->getDeviceContext()->RSSetViewports(1, g->getViewPort());


	MyMatrixRotationZ(worldforg, frame);
	for (int i = 0 ; i <= 6; i ++) {
		
		mesh3[i]->draw(g, &world, &view, &proj);

	}
	
	mesh3[0]->draw(g, &world, &view, &proj);
//	mesh3[10]->draw(g, &world, &view, &proj);

	//mesh->draw(g, &world,&view,&proj);
	static float ccc;
	ccc += 0.01f;
	char bbf[512];
	sprintf_s(bbf,512,"%f,",ccc);
	WCHAR bbc[512];
	bbc[511] = 0;
	stringconverter ssf;
	ssf.charToWCHAR(bbf,bbc);
	DebugTexts::instance()->setText(g,wcslen(bbc), bbc);
	MyMatrixTranslation(world,ccc,0,0);
	mesh2->draw(g, &world, &view, &proj);

	texdayo->setViewProj(g,&view,&proj,&a,&b);
	texdayo->setRenderBillBoardPos(0,&world);


	MyMatrixTranslation(world,4,0,0);
	mesh2->draw(g,&world,&view,&proj);

	MyMatrixTranslation(world,0,0,4);
	mesh2->draw(g,&world,&view,&proj);


	//float cc[] = {1.0f,1.0f,0.3f,1.0f};
	//mesh_instanceds->loadAnimeMatrixBasisToTexture(g);


//	mesh_instanceds->calcCombinedMatrixToTexture(g);
//	mesh_instanceds->loadColorToTexture(g);
//	mesh_instanceds->render(g);
	//g->getDeviceContext()->ClearRenderTargetView(mesh_instanceds->anime_matrix_basis_texture->target_view, cc);
	//demo->Render(g, mesh_instanceds->combined_matrix_texture);
	demo->Render(g, mesh_instanceds->anime_matrix_basis_texture);
//	demo->Render(g, mesh_instanceds->matrix_local_texture);

	static float h = 0.0f;
	h += 0.001f;
	MYMATRIX wor;
	MyMatrixIdentity(wor);
//	MyMatrixRotationZ(wor,h);
	MYMATRIX r1;
	MyMatrixTranslation(r1,0,0,h/100.0f);
	if ( h > 100) {
		 h=  1;
	}

	static int coun = 0;
	coun++;
	

	unsigned int color = 0xFF00FF00 + ((int)(coun)%256);
/*	texdayo->setRenderTexColor(0,0xFFFFFF00+(coun)%256);
	texdayo->setRenderBillBoardColor(0,color);*/
	texdayo->createIndexBuffer(g);
	texdayo->updateIndexBuffer(g);
	texdayo->sendinfoToVertexTexture(g);
	texdayo->render(g);




	g->getSwapChain()->Present(0,0);

	static int s = 0;
	if (c->getSecond() != s) {
	}
	CS::instance()->leave(CS_DEVICECON_CS, "render game");
	

}