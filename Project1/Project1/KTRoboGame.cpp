#include "KTRoboGame.h"
#include "KTRoboGraphics.h"
#include "KTRoboTask.h"
#include "KTRoboClock.h"
#include "KTRoboCS.h"
#include "KTRoboGameError.h"
#include "stringconverter.h"

using namespace KTROBO;


Game::Game(void)
{
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
	}

	mesh = 0;
	mesh2 = 0;
	for (int i = 0 ; i < 11; i++) {
		mesh3[i] = 0;
	}

	mesh_instanceds = 0;
}


Game::~Game(void)
{
}

bool Game::Init(HWND hwnd) {
	// 順番をかえないこと cs とタスクの間に依存関係がある
	KTROBO::CS::instance()->Init();
	for (int i = 0 ; i <TASKTHREAD_NUM; i++) {
		task_threads[i] = Task::factory(hwnd);
	}
	g = new Graphics();
	if (!g->Init(hwnd)) {
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "graphics init error");
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
	telop_texts->readFile(g,"resrc/sample/terop.txt",30,14,&MYVECTOR4(1,1,1,1),0.1);

//	mesh = new Mesh();
//	mesh->readMesh(g, "resrc/model/ponko2-3/pk2skirt.MESH", demo->tex_loader);
//	mesh->readAnime("resrc/model/ponko2-3/pk2skirt.ANIME");
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
	//MyMatrixTranslation(worl,2,0,2);
	//mesh_i->setWorld(&worl);
	mesh_i = mesh_instanceds->makeInstanced(mesh2,mesh2,NULL,NULL,false,&kakeru);
	MeshInstanced* mesh_i2 = mesh_instanceds->makeInstanced(mesh3[10],mesh3[10],mesh_i,mesh2->BoneIndexes["migiArmTekubiBone"],true,&kakeru);

	//MYMATRIX worldforg;
	MyMatrixRotationZ(worldforg, 3.14/2);
	MyMatrixMultiply(kakeru, kakeru, worldforg);
	
	MyMatrixScaling(worldforg, 1/3.0,1/3.0,1/3.0);
	MyMatrixMultiply(kakeru, kakeru, worldforg);
	
	MyMatrixRotationY(worldforg, -3.14/2);
	MyMatrixMultiply(kakeru, kakeru, worldforg);


	mesh_instanceds->makeInstanced(mesh3[9],mesh3[9], mesh_i2, mesh3[10]->BoneIndexes["MigiHandMotiBone"],false, &kakeru);
	MYVECTOR4 colors[KTROBO_MESH_INSTANCED_COLOR_MAX];
	memset(colors,0,sizeof(colors));

	mesh_instanceds->loadAnimeMatrixBasisToTexture(g);
	mesh_instanceds->loadMatrixLocalToTexture(g);
	mesh_instanceds->calcCombinedMatrixToTexture(g);
	for (int i=0;i<KTROBO_MESH_INSTANCED_COLOR_MAX; i++) {
		colors[i].x = 1;
		colors[i].y = 1;
		colors[i].z = 1;
		colors[i].w = 1;
	}
	mesh_i->setColor(colors);






	mytest_for_vt = new MyTestForVertexTexture();
	mytest_for_vt->Init(g);
	mytest_for_vt->readVertexTexture(g,mesh_instanceds->combined_matrix_texture);//anime_matrix_basis_texture);//matrix_local_texture);


	mytest_for_vt->writeInfo(g);



	return true;
}
void Game::Del() {

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

	if (g) {
		g->Release();
		delete g;
		g = 0;
	}

	// 順番を変えないこと　cs と　タスクの間に依存関係がある
	for (int i = 0 ; i <TASKTHREAD_NUM; i++) {
		task_threads[i]->deleteTask();
		delete task_threads[i];
		task_threads[i] = 0;
	}

	KTROBO::CS::instance()->Del();
	
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


void Game::Run() {

	double millisecond = stopWatch();
		if (millisecond > RENDERTIME_IGNORETIME) {
		Sleep(1);
		millisecond = RENDERTIME_SETTIME;
	} else if ( millisecond < RENDERTIME_SETTIME ) {
		Sleep(DWORD(RENDERTIME_SETTIME - millisecond));
		millisecond = RENDERTIME_SETTIME;
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





	//demo->Render(g);
	float clearColor[4] = {
		0.3f,0.4f,0.8f,1.0f};
	ID3D11RenderTargetView* v = g->getRenderTargetView();
	g->getDeviceContext()->OMSetRenderTargets(1, &v, Mesh::pDepthStencilView);
	
	g->getDeviceContext()->ClearRenderTargetView(g->getRenderTargetView(),clearColor);
	g->getDeviceContext()->ClearDepthStencilView(Mesh::pDepthStencilView,  D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,1.0f, 0 );
	KTROBO::DebugTexts::instance()->render(g);
	telop_texts->plusTime(g,20);
	if (telop_texts->isRenderFinished()) {
		telop_texts->readFile(g,"resrc/sample/KTROBO.txt",30,14,&MYVECTOR4(1,1,1,1),0.1);
	}
	telop_texts->render(g);
	//demo->Render(g);

	static float frame = 0;
	static int frameint = 0;
	frame += 0.1f;
	




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
	mesh2->animate(frame, true);
	for (int i = 0 ; i <= 6 ; i++) {
		mesh3[i]->animate(frame, true);
	}
	
	mesh3[7]->animate(0,true);
	mesh3[8]->animate(0,true);
	mesh3[9]->animate(0,true);
	mesh3[10]->animate(0,true);
	}


	
	memset(animf,0,sizeof(animf));
	memset(animl,0,sizeof(animl));
	memset(animw,0,sizeof(animw));
	static int count = 0;
	count++;

	int bsize = mesh2->Bones.size();
	

	for (int i=0;i<bsize; i++) {

		MeshBone* bone = mesh2->Bones[i];
		mesh2->calculateOffsetMatrixToGetMinMaxAndWeight(bone, frame, &animl[i], &animf[i], &animw[i]);
	//	animf[i] = floor(frame);////floor(frame);//count * 0.3;
	//	animl[i] =ceil(frame);//count * 0.3;
	//	animw[i] = animl[i]- frame;
		//animw[i] = 1;//1- animw[i];
	}

	mesh_i->setBoneIndexInfo(animf, animl, animw);


	
	//frame = 30.001;

	MYMATRIX world;
	MYMATRIX view;
	MYMATRIX proj;
	MyMatrixIdentity(world);
	MYVECTOR3 a(8,-16,12);
	MYVECTOR3 b(0,0,0);
	MYVECTOR3 up(0,0,1);
	MyMatrixLookAtRH(view, a, b, up);
	MyMatrixPerspectiveFovRH(proj, 1, g->getScreenWidth() / (float)g->getScreenHeight(), 1, 1000);
	
	
	MYMATRIX worldforg;
	

	MyMatrixRotationZ(worldforg, frame);
	for (int i = 0 ; i <= 8; i ++) {
		
		mesh3[i]->draw(g, &world, &view, &proj);

	}
	
	mesh3[0]->draw(g, &world, &view, &proj);
//	mesh3[10]->draw(g, &world, &view, &proj);

	//mesh->draw(g, &world,&view,&proj);
	mesh2->draw(g, &world, &view, &proj);
	//float cc[] = {1.0f,1.0f,0.3f,1.0f};
	//mesh_instanceds->loadAnimeMatrixBasisToTexture(g);

	MYMATRIX temp_world;
	MyMatrixTranslation(temp_world, 0,0,0);
	mesh_i->setWorld(&temp_world);
	mesh_instanceds->setViewProj(g, &view,&proj, &MYVECTOR4(1,1,-10,1));
	mesh_instanceds->calcCombinedMatrixToTexture(g);
	mesh_instanceds->loadColorToTexture(g);
	mesh_instanceds->render(g);
	//g->getDeviceContext()->ClearRenderTargetView(mesh_instanceds->anime_matrix_basis_texture->target_view, cc);
	//demo->Render(g, mesh_instanceds->combined_matrix_texture);
	demo->Render(g, mesh_instanceds->anime_matrix_basis_texture);
//	demo->Render(g, mesh_instanceds->matrix_local_texture);

	g->getSwapChain()->Present(0,0);

	static int s = 0;
	//if (c->getSecond() != s) {
		s = c->getSecond();
		WCHAR test[512];
		char test2[1024];
		memset(test2,0,sizeof(1024));
		memset(test,0,sizeof(WCHAR)*512);
		sprintf_s(test2,512, "aw %f af %d al %d", animw[0], (int)animf[0], (int)animl[0]);
		stringconverter cs;
		cs.charToWCHAR(test2,test);
		KTROBO::DebugTexts::instance()->setText(g, wcslen(test), test);
	//}

	startWatch();
}