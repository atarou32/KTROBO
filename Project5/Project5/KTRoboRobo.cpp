#include "KTRoboRobo.h"
#include "KTRoboGameError.h"

using namespace KTROBO;
Robo::Robo(void)
{
	head = 0;
	body = 0;
	leg = 0;
	arm = 0;
	raweapon = 0;
	laweapon = 0;
	rsweapon = 0;
	lsweapon = 0;
	iweapon = 0;
	fcs = 0;
	booster = 0;
	engine = 0;

}


Robo::~Robo(void)
{
}

void Robo::byouga(Graphics* g, MYMATRIX* world, MYMATRIX* view, MYMATRIX* proj) {



	/*if (head) {
		if (head->head) {
			head->head->drawWithObbs(g,world,view,proj);
		}
		if (head->head2) {
			head->head2->drawWithObbs(g,world,view,proj);
		}
		if (head->head3) {
			head->head3->drawWithObbs(g,world,view,proj);
		}

	}

	if (body) {
		if(body->body) {
			body->body->drawWithObbs(g,world,view,proj);
		}
	}*/

	if (leg) {
		if (leg->leg) {
			leg->leg->drawWithObbs(g,world,view,proj);
		}
	}

	if (arm) {
		if (arm->larm) {
			arm->larm->drawWithObbs(g,world,view,proj);
		}
	}
	







}


void Robo::init(Graphics* g, MyTextureLoader* tex_loader) {
/*	Mesh* head;
	Mesh* body;
	Mesh* leg;
	Mesh* rarm;
	Mesh* larm;
	RArmWeapon* raweapon;
	LArmWeapon* laweapon;
	RShoulderWeapon* rsweapon;
	LShoulderWeapon* lsweapon;
*/
	head = 0;
	body = 0;
	leg = 0;
	arm = 0;
	raweapon = 0;
	laweapon = 0;
	rsweapon = 0;
	lsweapon = 0;
	iweapon = 0;
	fcs = 0;
	booster = 0;
	engine = 0;
	// headparts のメタデータの読み込み
	
	MyTokenAnalyzer ma;
	{
	ma.load("resrc/ktrobo/info/metadata/ktroboheadpartsmetadata.txt");
	RoboDataMetaData* head_md = new RoboDataMetaData();
	RoboMetaDataPart rmdp;
	rmdp.clear();
	int dnum = ma.GetIntToken();
	for (int i=0;i<dnum;i++) {
		rmdp.clear();
		rmdp.readline(&ma);
		head_md->setData(rmdp.data_name,rmdp.data_name2,rmdp.data_type,rmdp.data_sentence,rmdp.data_compare);
	}

	ma.deletedayo();
	
	ma.load("resrc/ktrobo/info/ktroboheadparts.txt");

	head = new RoboHead();
	try {
	head->init(&ma,head_md,g,tex_loader);
		} catch (GameError* err) {
		
	//	MessageBoxA(g->getHWND(), err->getMessage(), err->getErrorCodeString(err->getErrorCode()), MB_OK);
		delete head_md;
		ma.deletedayo();
		throw err;
	}
	delete head_md;

	ma.deletedayo();
	}
	
	// 次はbody
	{
	ma.load("resrc/ktrobo/info/metadata/ktrobocorepartsmetadata.txt");
	RoboDataMetaData* body_md = new RoboDataMetaData();
	RoboMetaDataPart rmdp;
	rmdp.clear();
	int dnum = ma.GetIntToken();
	for (int i=0;i<dnum;i++) {
		rmdp.clear();
		rmdp.readline(&ma);
		body_md->setData(rmdp.data_name,rmdp.data_name2,rmdp.data_type,rmdp.data_sentence,rmdp.data_compare);
	}

	ma.deletedayo();
	ma.load("resrc/ktrobo/info/ktrobocoreparts.txt");


	body = new RoboBody();
	try {
	body->init(&ma,body_md,g,tex_loader);
	} catch (GameError* err) {
		ma.deletedayo();
	//	MessageBoxA(g->getHWND(), err->getMessage(), err->getErrorCodeString(err->getErrorCode()), MB_OK);
		delete body_md;
		throw err;
	}

	delete body_md;

	ma.deletedayo();
	}

	// 次はarm
	{
	ma.load("resrc/ktrobo/info/metadata/ktroboarmpartsmetadata.txt");
	RoboDataMetaData* arm_md = new RoboDataMetaData();
	RoboMetaDataPart rmdp;
	rmdp.clear();
	int dnum = ma.GetIntToken();
	for (int i=0;i<dnum;i++) {
		rmdp.clear();
		rmdp.readline(&ma);
		arm_md->setData(rmdp.data_name,rmdp.data_name2,rmdp.data_type,rmdp.data_sentence,rmdp.data_compare);
	}

	ma.deletedayo();
	ma.load("resrc/ktrobo/info/ktroboarmparts.txt");


	arm = new RoboArm();
	try {
	arm->init(&ma,arm_md,g,tex_loader);
		} catch (GameError* err) {
		
	//	MessageBoxA(g->getHWND(), err->getMessage(), err->getErrorCodeString(err->getErrorCode()), MB_OK);
		delete arm_md;
		ma.deletedayo();
		throw err;
	}
	delete arm_md;

	ma.deletedayo();
	}



	// 次はleg
	{
	ma.load("resrc/ktrobo/info/metadata/ktrobolegpartsmetadata.txt");
	RoboDataMetaData* leg_md = new RoboDataMetaData();
	RoboMetaDataPart rmdp;
	rmdp.clear();
	int dnum = ma.GetIntToken();
	for (int i=0;i<dnum;i++) {
		rmdp.clear();
		rmdp.readline(&ma);
		leg_md->setData(rmdp.data_name,rmdp.data_name2,rmdp.data_type,rmdp.data_sentence,rmdp.data_compare);
	}

	ma.deletedayo();

	ma.load("resrc/ktrobo/info/leg/ktrobolegt2.txt");

	leg = new RoboLeg();
	try { 
	leg->init(&ma,leg_md,g,tex_loader);
		} catch (GameError* err) {
		
	//	MessageBoxA(g->getHWND(), err->getMessage(), err->getErrorCodeString(err->getErrorCode()), MB_OK);
		delete leg_md;
		ma.deletedayo();
		throw err;
	}
	delete leg_md;

	ma.deletedayo();
	}
}

void Robo::release() {
	if (head) {
		head->Release();
		delete head;
		head = 0;
	}
	if (body) {
		body->Release();
		delete body;
		body = 0;
	}
	if (leg) {
		leg->Release();
		delete leg;
		leg = 0;
	}
	if (arm) {
		arm->Release();
		delete arm;
		arm = 0;
	}
	if (raweapon) {
		raweapon->Release();
		delete raweapon;
		raweapon = 0;
	}

	if (laweapon) {
		laweapon->Release();
		delete laweapon;
		laweapon = 0;
	}

	if (rsweapon) {
		rsweapon->Release();
		delete rsweapon;
		rsweapon = 0;
	}

	if (lsweapon) {
		lsweapon->Release();
		delete lsweapon;
		lsweapon = 0;
	}

	if (iweapon) {
		iweapon->Release();
		delete iweapon;
		iweapon = 0;
	}

	if (fcs) {
		fcs->Release();
		delete fcs;
		fcs = 0;
	}

	if (booster) {
		booster->Release();
		delete booster;
		booster = 0;
	}

	if (engine) {
		engine->Release();
		delete engine;
		engine = 0;
	}

}



void RoboHead::init(MyTokenAnalyzer* ma, RoboDataMetaData* meta_data, Graphics* g, MyTextureLoader* tex_loader) {

	data = new RoboData();
	ma->GetToken();
	if (strcmp(ma->Toke(),"{") != 0) {
		throw new GameError(KTROBO::FATAL_ERROR,"there is no { in init robo");
	}

	while(!ma->enddayo()) {
		ma->GetToken();
		if (strcmp(ma->Toke(), "}")==0) {
			break;
		}

		char* p = ma->Toke();
		RoboMetaDataPart* pp = meta_data->getData(p);
		if (pp) {
			if (strcmp(pp->data_type, "INT")==0) {
				this->data->setData(ma->GetIntToken(), pp->data_name,pp->data_name2,"",0);
			} else if(strcmp(pp->data_type, "STRING")==0) {
				ma->GetToken();
				this->data->setData(0,pp->data_name,pp->data_name2,ma->Toke(),0);
			} else if(strcmp(pp->data_type, "FLOAT")==0) {
				this->data->setData(0,pp->data_name,pp->data_name2,"",ma->GetFloatToken());
			} else {
				// UNKNOWN
				throw new GameError(KTROBO::FATAL_ERROR,"unknown token in init robo");
			}



		}
	}


	// data にデータが入っているので
	RoboDataPart* rdp = data->getData("mesh");
	if (rdp) {
		if (strcmp("@", rdp->string_data) != 0) {
			// 読み込む
			this->head = new Mesh();
			head->readMesh(g,rdp->string_data,tex_loader);
			// 次はアニメ
			rdp = data->getData("anime");
			if (!rdp) { 
				throw new GameError(KTROBO::FATAL_ERROR, "there is no anime in init robo");
			}
			head->readAnime(rdp->string_data);
			head->animate(0,true);
		}
	}

	rdp = data->getData("mesh2");
	if (rdp) {
		if (strcmp("@", rdp->string_data) != 0) {
			// 読み込む
			this->head2 = new Mesh();
			head2->readMesh(g,rdp->string_data,tex_loader);
			// 次はアニメ
			rdp = data->getData("anime2");
			if (!rdp) { 
				throw new GameError(KTROBO::FATAL_ERROR, "there is no anime in init robo");
			}
			head2->readAnime(rdp->string_data);
			head2->animate(0,true);
		}
	}

	rdp = data->getData("mesh3");
	if (rdp) {
		if (strcmp("@", rdp->string_data) != 0) {
			// 読み込む
			this->head3 = new Mesh();
			head3->readMesh(g,rdp->string_data,tex_loader);
			// 次はアニメ
			rdp = data->getData("anime3");
			if (!rdp) { 
				throw new GameError(KTROBO::FATAL_ERROR, "there is no anime in init robo");
			}
			head3->readAnime(rdp->string_data);
			head3->animate(0,true);
		}
	}


}

void RoboBody::init(MyTokenAnalyzer* ma, RoboDataMetaData* meta_data, Graphics* g, MyTextureLoader* tex_loader) {
//	throw new GameError(KTROBO::FATAL_ERROR,"there is no { in init robo");

	data = new RoboData();
	ma->GetToken();
	if (strcmp(ma->Toke(),"{") != 0) {
		throw new GameError(KTROBO::FATAL_ERROR,"there is no { in init robo");
	}

	while(!ma->enddayo()) {
		ma->GetToken();
		if (strcmp(ma->Toke(), "}")==0) {
			break;
		}

		char* p = ma->Toke();
		RoboMetaDataPart* pp = meta_data->getData(p);
		if (pp) {
			if (strcmp(pp->data_type, "INT")==0) {
				this->data->setData(ma->GetIntToken(), pp->data_name,pp->data_name2,"",0);
			} else if(strcmp(pp->data_type, "STRING")==0) {
				ma->GetToken();
				this->data->setData(0,pp->data_name,pp->data_name2,ma->Toke(),0);
			} else if(strcmp(pp->data_type, "FLOAT")==0) {
				this->data->setData(0,pp->data_name,pp->data_name2,"",ma->GetFloatToken());
			} else {
				// UNKNOWN
				throw new GameError(KTROBO::FATAL_ERROR,"unknown token in init robo");
			}



		}
	}

	// data にデータが入っているので
	RoboDataPart* rdp = data->getData("mesh");
	if (rdp) {
		if (strcmp("@", rdp->string_data) != 0) {
			// 読み込む
			this->body = new Mesh();
			body->readMesh(g,rdp->string_data,tex_loader);
			// 次はアニメ
			rdp = data->getData("anime");
			if (!rdp) { 
				throw new GameError(KTROBO::FATAL_ERROR, "there is no anime in init robo");
			}
			body->readAnime(rdp->string_data);
			body->animate(0,true);
		}
	}

}

void RoboArm::init(MyTokenAnalyzer* ma, RoboDataMetaData* meta_data, Graphics* g, MyTextureLoader* tex_loader) {

	data = new RoboData();
	ma->GetToken();
	if (strcmp(ma->Toke(),"{") != 0) {
		throw new GameError(KTROBO::FATAL_ERROR,"there is no { in init robo");
	}

	while(!ma->enddayo()) {
		ma->GetToken();
		if (strcmp(ma->Toke(), "}")==0) {
			break;
		}

		char* p = ma->Toke();
		RoboMetaDataPart* pp = meta_data->getData(p);
		if (pp) {
			if (strcmp(pp->data_type, "INT")==0) {
				this->data->setData(ma->GetIntToken(), pp->data_name,pp->data_name2,"",0);
			} else if(strcmp(pp->data_type, "STRING")==0) {
				ma->GetToken();
				this->data->setData(0,pp->data_name,pp->data_name2,ma->Toke(),0);
			} else if(strcmp(pp->data_type, "FLOAT")==0) {
				this->data->setData(0,pp->data_name,pp->data_name2,"",ma->GetFloatToken());
			} else {
				// UNKNOWN
				throw new GameError(KTROBO::FATAL_ERROR,"unknown token in init robo");
			}



		}
	}


	// data にデータが入っているので
	RoboDataPart* rdp = data->getData("mesh");
	if (rdp) {
		if (strcmp("@", rdp->string_data) != 0) {
			// 読み込む
			this->larm = new Mesh();
			this->rarm = new Mesh();
			larm->readMesh(g,rdp->string_data,tex_loader);
			rarm->readMesh(g,rdp->string_data,tex_loader);
			// 次はアニメ
			rdp = data->getData("anime");
			if (!rdp) { 
				throw new GameError(KTROBO::FATAL_ERROR, "there is no anime in init robo");
			}
			larm->readAnime(rdp->string_data);
			rarm->readAnime(rdp->string_data);
			larm->animate(0,true);
			rarm->animate(0,true);
		}
	}


}

void RoboLeg::init(MyTokenAnalyzer* ma, RoboDataMetaData* meta_data, Graphics* g, MyTextureLoader* tex_loader) {


	
	ma->GetToken();
	if (strcmp(ma->Toke(),"{") != 0) {
		throw new GameError(KTROBO::FATAL_ERROR,"there is no { in init robo");
	}

	data = new RoboData();

	while(!ma->enddayo()) {
		ma->GetToken();
		if (strcmp(ma->Toke(), "}")==0) {
			break;
		}

		char* p = ma->Toke();
		RoboMetaDataPart* pp = meta_data->getData(p);
		if (pp) {
			if (strcmp(pp->data_type, "INT")==0) {
				this->data->setData(ma->GetIntToken(), pp->data_name,pp->data_name2,"",0);
			} else if(strcmp(pp->data_type, "STRING")==0) {
				ma->GetToken();
				this->data->setData(0,pp->data_name,pp->data_name2,ma->Toke(),0);
			} else if(strcmp(pp->data_type, "FLOAT")==0) {
				this->data->setData(0,pp->data_name,pp->data_name2,"",ma->GetFloatToken());
			} else {
				// UNKNOWN
				
				throw new GameError(KTROBO::FATAL_ERROR,"unknown token in init robo");
			}



		}
	}

	// data にデータが入っているので
	RoboDataPart* rdp = data->getData("mesh");
	if (rdp) {
		if (strcmp("@", rdp->string_data) != 0) {
			// 読み込む
			this->leg = new Mesh();
			leg->readMesh(g,rdp->string_data,tex_loader);
			// 次はアニメ
			rdp = data->getData("anime");
			if (!rdp) { 
				throw new GameError(KTROBO::FATAL_ERROR, "there is no anime in init robo");
			}
			leg->readAnime(rdp->string_data);
			leg->animate(0,true);
		}
	}

}

void RArmWeapon::init(MyTokenAnalyzer* ma, RoboDataMetaData* meta_data, Graphics* g, MyTextureLoader* tex_loader) {

	data = new RoboData();
	ma->GetToken();
	if (strcmp(ma->Toke(),"{") != 0) {
		throw new GameError(KTROBO::FATAL_ERROR,"there is no { in init robo");
	}

	while(!ma->enddayo()) {
		ma->GetToken();
		if (strcmp(ma->Toke(), "}")==0) {
			break;
		}

		char* p = ma->Toke();
		RoboMetaDataPart* pp = meta_data->getData(p);
		if (pp) {
			if (strcmp(pp->data_type, "INT")==0) {
				this->data->setData(ma->GetIntToken(), pp->data_name,pp->data_name2,"",0);
			} else if(strcmp(pp->data_type, "STRING")==0) {
				ma->GetToken();
				this->data->setData(0,pp->data_name,pp->data_name2,ma->Toke(),0);
			} else if(strcmp(pp->data_type, "FLOAT")==0) {
				this->data->setData(0,pp->data_name,pp->data_name2,"",ma->GetFloatToken());
			} else {
				// UNKNOWN
				throw new GameError(KTROBO::FATAL_ERROR,"unknown token in init robo");
			}



		}
	}

	// data にデータが入っているので
	RoboDataPart* rdp = data->getData("mesh");
	if (rdp) {
		if (strcmp("@", rdp->string_data) != 0) {
			// 読み込む
			this->weapon = new Mesh();
			weapon->readMesh(g,rdp->string_data,tex_loader);
			// 次はアニメ
			rdp = data->getData("anime");
			if (!rdp) { 
				throw new GameError(KTROBO::FATAL_ERROR, "there is no anime in init robo");
			}
			weapon->readAnime(rdp->string_data);
			weapon->animate(0,true);
		}
	}


}

void LArmWeapon::init(MyTokenAnalyzer* ma, RoboDataMetaData* meta_data, Graphics* g, MyTextureLoader* tex_loader) {


	data = new RoboData();
	ma->GetToken();
	if (strcmp(ma->Toke(),"{") != 0) {
		throw new GameError(KTROBO::FATAL_ERROR,"there is no { in init robo");
	}

	while(!ma->enddayo()) {
		ma->GetToken();
		if (strcmp(ma->Toke(), "}")==0) {
			break;
		}

		char* p = ma->Toke();
		RoboMetaDataPart* pp = meta_data->getData(p);
		if (pp) {
			if (strcmp(pp->data_type, "INT")==0) {
				this->data->setData(ma->GetIntToken(), pp->data_name,pp->data_name2,"",0);
			} else if(strcmp(pp->data_type, "STRING")==0) {
				ma->GetToken();
				this->data->setData(0,pp->data_name,pp->data_name2,ma->Toke(),0);
			} else if(strcmp(pp->data_type, "FLOAT")==0) {
				this->data->setData(0,pp->data_name,pp->data_name2,"",ma->GetFloatToken());
			} else {
				// UNKNOWN
				throw new GameError(KTROBO::FATAL_ERROR,"unknown token in init robo");
			}



		}
	}

	// data にデータが入っているので
	RoboDataPart* rdp = data->getData("mesh");
	if (rdp) {
		if (strcmp("@", rdp->string_data) != 0) {
			// 読み込む
			this->weapon = new Mesh();
			weapon->readMesh(g,rdp->string_data,tex_loader);
			// 次はアニメ
			rdp = data->getData("anime");
			if (!rdp) { 
				throw new GameError(KTROBO::FATAL_ERROR, "there is no anime in init robo");
			}
			weapon->readAnime(rdp->string_data);
			weapon->animate(0,true);
		}
	}

}

void RShoulderWeapon::init(MyTokenAnalyzer* ma, RoboDataMetaData* meta_data, Graphics* g, MyTextureLoader* tex_loader) {


	data = new RoboData();
	ma->GetToken();
	if (strcmp(ma->Toke(),"{") != 0) {
		throw new GameError(KTROBO::FATAL_ERROR,"there is no { in init robo");
	}

	while(!ma->enddayo()) {
		ma->GetToken();
		if (strcmp(ma->Toke(), "}")==0) {
			break;
		}

		char* p = ma->Toke();
		RoboMetaDataPart* pp = meta_data->getData(p);
		if (pp) {
			if (strcmp(pp->data_type, "INT")==0) {
				this->data->setData(ma->GetIntToken(), pp->data_name,pp->data_name2,"",0);
			} else if(strcmp(pp->data_type, "STRING")==0) {
				ma->GetToken();
				this->data->setData(0,pp->data_name,pp->data_name2,ma->Toke(),0);
			} else if(strcmp(pp->data_type, "FLOAT")==0) {
				this->data->setData(0,pp->data_name,pp->data_name2,"",ma->GetFloatToken());
			} else {
				// UNKNOWN
				throw new GameError(KTROBO::FATAL_ERROR,"unknown token in init robo");
			}



		}
	}

	// data にデータが入っているので
	RoboDataPart* rdp = data->getData("mesh");
	if (rdp) {
		if (strcmp("@", rdp->string_data) != 0) {
			// 読み込む
			this->weapon = new Mesh();
			weapon->readMesh(g,rdp->string_data,tex_loader);
			// 次はアニメ
			rdp = data->getData("anime");
			if (!rdp) { 
				throw new GameError(KTROBO::FATAL_ERROR, "there is no anime in init robo");
			}
			weapon->readAnime(rdp->string_data);
			weapon->animate(0,true);
		}
	}


}

void LShoulderWeapon::init(MyTokenAnalyzer* ma, RoboDataMetaData* meta_data, Graphics* g, MyTextureLoader* tex_loader) {


	data = new RoboData();
	ma->GetToken();
	if (strcmp(ma->Toke(),"{") != 0) {
		throw new GameError(KTROBO::FATAL_ERROR,"there is no { in init robo");
	}

	while(!ma->enddayo()) {
		ma->GetToken();
		if (strcmp(ma->Toke(), "}")==0) {
			break;
		}

		char* p = ma->Toke();
		RoboMetaDataPart* pp = meta_data->getData(p);
		if (pp) {
			if (strcmp(pp->data_type, "INT")==0) {
				this->data->setData(ma->GetIntToken(), pp->data_name,pp->data_name2,"",0);
			} else if(strcmp(pp->data_type, "STRING")==0) {
				ma->GetToken();
				this->data->setData(0,pp->data_name,pp->data_name2,ma->Toke(),0);
			} else if(strcmp(pp->data_type, "FLOAT")==0) {
				this->data->setData(0,pp->data_name,pp->data_name2,"",ma->GetFloatToken());
			} else {
				// UNKNOWN
				throw new GameError(KTROBO::FATAL_ERROR,"unknown token in init robo");
			}



		}
	}

		// data にデータが入っているので
	RoboDataPart* rdp = data->getData("mesh");
	if (rdp) {
		if (strcmp("@", rdp->string_data) != 0) {
			// 読み込む
			this->weapon = new Mesh();
			weapon->readMesh(g,rdp->string_data,tex_loader);
			// 次はアニメ
			rdp = data->getData("anime");
			if (!rdp) { 
				throw new GameError(KTROBO::FATAL_ERROR, "there is no anime in init robo");
			}
			weapon->readAnime(rdp->string_data);
			weapon->animate(0,true);
		}
	}

}

void InsideWeapon::init(MyTokenAnalyzer* ma, RoboDataMetaData* meta_data, Graphics* g, MyTextureLoader* tex_loader) {


	data = new RoboData();
	ma->GetToken();
	if (strcmp(ma->Toke(),"{") != 0) {
		throw new GameError(KTROBO::FATAL_ERROR,"there is no { in init robo");
	}

	while(!ma->enddayo()) {
		ma->GetToken();
		if (strcmp(ma->Toke(), "}")==0) {
			break;
		}

		char* p = ma->Toke();
		RoboMetaDataPart* pp = meta_data->getData(p);
		if (pp) {
			if (strcmp(pp->data_type, "INT")==0) {
				this->data->setData(ma->GetIntToken(), pp->data_name,pp->data_name2,"",0);
			} else if(strcmp(pp->data_type, "STRING")==0) {
				ma->GetToken();
				this->data->setData(0,pp->data_name,pp->data_name2,ma->Toke(),0);
			} else if(strcmp(pp->data_type, "FLOAT")==0) {
				this->data->setData(0,pp->data_name,pp->data_name2,"",ma->GetFloatToken());
			} else {
				// UNKNOWN
				throw new GameError(KTROBO::FATAL_ERROR,"unknown token in init robo");
			}



		}
	}

	// data にデータが入っているので
	RoboDataPart* rdp = data->getData("mesh");
	if (rdp) {
		if (strcmp("@", rdp->string_data) != 0) {
			// 読み込む
			this->weapon = new Mesh();
			weapon->readMesh(g,rdp->string_data,tex_loader);
			// 次はアニメ
			rdp = data->getData("anime");
			if (!rdp) { 
				throw new GameError(KTROBO::FATAL_ERROR, "there is no anime in init robo");
			}
			weapon->readAnime(rdp->string_data);
			weapon->animate(0,true);
		}
	}


}




void RoboBooster::init(MyTokenAnalyzer* ma, RoboDataMetaData* meta_data, Graphics* g, MyTextureLoader* tex_loader) {


	data = new RoboData();
	ma->GetToken();
	if (strcmp(ma->Toke(),"{") != 0) {
		throw new GameError(KTROBO::FATAL_ERROR,"there is no { in init robo");
	}

	while(!ma->enddayo()) {
		ma->GetToken();
		if (strcmp(ma->Toke(), "}")==0) {
			break;
		}

		char* p = ma->Toke();
		RoboMetaDataPart* pp = meta_data->getData(p);
		if (pp) {
			if (strcmp(pp->data_type, "INT")==0) {
				this->data->setData(ma->GetIntToken(), pp->data_name,pp->data_name2,"",0);
			} else if(strcmp(pp->data_type, "STRING")==0) {
				ma->GetToken();
				this->data->setData(0,pp->data_name,pp->data_name2,ma->Toke(),0);
			} else if(strcmp(pp->data_type, "FLOAT")==0) {
				this->data->setData(0,pp->data_name,pp->data_name2,"",ma->GetFloatToken());
			} else {
				// UNKNOWN
				throw new GameError(KTROBO::FATAL_ERROR,"unknown token in init robo");
			}



		}
	}

	// data にデータが入っているので
	RoboDataPart* rdp = data->getData("mesh");
	if (rdp) {
		if (strcmp("@", rdp->string_data) != 0) {
			// 読み込む
			this->mesh = new Mesh();
			mesh->readMesh(g,rdp->string_data,tex_loader);
			// 次はアニメ
			rdp = data->getData("anime");
			if (!rdp) { 
				throw new GameError(KTROBO::FATAL_ERROR, "there is no anime in init robo");
			}
			mesh->readAnime(rdp->string_data);
			mesh->animate(0,true);
		}
	}


}



void RoboEngine::init(MyTokenAnalyzer* ma, RoboDataMetaData* meta_data, Graphics* g, MyTextureLoader* tex_loader) {


	data = new RoboData();
	ma->GetToken();
	if (strcmp(ma->Toke(),"{") != 0) {
		throw new GameError(KTROBO::FATAL_ERROR,"there is no { in init robo");
	}

	while(!ma->enddayo()) {
		ma->GetToken();
		if (strcmp(ma->Toke(), "}")==0) {
			break;
		}

		char* p = ma->Toke();
		RoboMetaDataPart* pp = meta_data->getData(p);
		if (pp) {
			if (strcmp(pp->data_type, "INT")==0) {
				this->data->setData(ma->GetIntToken(), pp->data_name,pp->data_name2,"",0);
			} else if(strcmp(pp->data_type, "STRING")==0) {
				ma->GetToken();
				this->data->setData(0,pp->data_name,pp->data_name2,ma->Toke(),0);
			} else if(strcmp(pp->data_type, "FLOAT")==0) {
				this->data->setData(0,pp->data_name,pp->data_name2,"",ma->GetFloatToken());
			} else {
				// UNKNOWN
				throw new GameError(KTROBO::FATAL_ERROR,"unknown token in init robo");
			}



		}
	}

	// data にデータが入っているので
	RoboDataPart* rdp = data->getData("mesh");
	if (rdp) {
		if (strcmp("@", rdp->string_data) != 0) {
			// 読み込む
			this->mesh = new Mesh();
			mesh->readMesh(g,rdp->string_data,tex_loader);
			// 次はアニメ
			rdp = data->getData("anime");
			if (!rdp) { 
				throw new GameError(KTROBO::FATAL_ERROR, "there is no anime in init robo");
			}
			mesh->readAnime(rdp->string_data);
			mesh->animate(0,true);
		}
	}


}



void RoboFCS::init(MyTokenAnalyzer* ma, RoboDataMetaData* meta_data, Graphics* g, MyTextureLoader* tex_loader) {


	data = new RoboData();
	ma->GetToken();
	if (strcmp(ma->Toke(),"{") != 0) {
		throw new GameError(KTROBO::FATAL_ERROR,"there is no { in init robo");
	}

	while(!ma->enddayo()) {
		ma->GetToken();
		if (strcmp(ma->Toke(), "}")==0) {
			break;
		}

		char* p = ma->Toke();
		RoboMetaDataPart* pp = meta_data->getData(p);
		if (pp) {
			if (strcmp(pp->data_type, "INT")==0) {
				this->data->setData(ma->GetIntToken(), pp->data_name,pp->data_name2,"",0);
			} else if(strcmp(pp->data_type, "STRING")==0) {
				ma->GetToken();
				this->data->setData(0,pp->data_name,pp->data_name2,ma->Toke(),0);
			} else if(strcmp(pp->data_type, "FLOAT")==0) {
				this->data->setData(0,pp->data_name,pp->data_name2,"",ma->GetFloatToken());
			} else {
				// UNKNOWN
				throw new GameError(KTROBO::FATAL_ERROR,"unknown token in init robo");
			}



		}
	}

	// data にデータが入っているので
	RoboDataPart* rdp = data->getData("mesh");
	if (rdp) {
		if (strcmp("@", rdp->string_data) != 0) {
			// 読み込む
			this->mesh = new Mesh();
			mesh->readMesh(g,rdp->string_data,tex_loader);
			// 次はアニメ
			rdp = data->getData("anime");
			if (!rdp) { 
				throw new GameError(KTROBO::FATAL_ERROR, "there is no anime in init robo");
			}
			mesh->readAnime(rdp->string_data);
			mesh->animate(0,true);
		}
	}


}