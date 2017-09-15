#include "KTROBOMission.h"
#include "KTRoboGame.h"

using namespace KTROBO;
Mission::Mission(void)
{
	nokori_second = 654;
}


Mission::~Mission(void)
{
}

void Mission::start() {
}
void Mission::end(){
	
}

void Mission::success() {


}

void Mission::failure() {

}

void Mission::gift() {


}

int Mission::get_now_state() {

	return 0;
}


void Gamen_MISSION::setView(MYMATRIX* world, float R, float dsecond) {
	MYVECTOR3 up(0,0,1);
	MYVECTOR3 pos(0,0,0);
	MYVECTOR3 vec(0,-1,0);
	MyVec3TransformCoord(pos,pos,*world);
	MyVec3TransformNormal(vec,vec,*world);
	MYVECTOR3 temp_lookfrom = up * R *3+ pos - vec * R*5;
	if (vec.float3.z > 0.5) {
		temp_lookfrom = temp_lookfrom + up * R*2 + vec * R * 7;
	}
	float offset = 1;
	float maxspeed = 1.57 / 2.0f*3; // Å‘å‘¬“x
	if (vec.float3.z > 0.5f) {
		maxspeed *= 1000.0f;
		lookfromspeed = maxspeed;
		offset = 6;
	}

	float speed = lookfromspeed;
	float kyori = dsecond * speed;

	MYVECTOR3 dlookfrom  = (temp_lookfrom - lookfrom);
	float dkyori = MyVec3Length(dlookfrom);
	if (abs(dkyori) > 0.1f) {

		if (kyori < dsecond * maxspeed / 200) {
		
			lookfromspeed += maxspeed /10.0f;
			if (lookfromspeed > maxspeed) {
				lookfromspeed = maxspeed;
			}
		}else 
		 if (dkyori < 0.0f) {
			// ‰½‚à‚µ‚È‚¢
		} else if (dkyori > 300000 * kyori) {
			lookfrom = temp_lookfrom;
			lookfromspeed = maxspeed;
		} else if(dkyori > kyori) {
			lookfromspeed += maxspeed * kyori/dkyori;
			if (lookfromspeed > maxspeed) {
				lookfromspeed = maxspeed;
			}
		lookfrom = lookfrom + dlookfrom * kyori/dkyori;
		} else {
			lookfromspeed += maxspeed /10.0f;
			if (lookfromspeed > maxspeed) {
				lookfromspeed = maxspeed;
			}
			lookfrom = lookfrom + dlookfrom/ 10.0f*offset;// temp_lookfrom;
		}
	} else {
		lookfromspeed /= 1.1f;
	}


	maxspeed = 1.57/1.0f*100.0f;
	if (vec.float3.z > 0.5f) {
		maxspeed *= 100.0f;
		lookatspeed = maxspeed;
	}
	speed = lookatspeed;
	kyori = dsecond * speed;
	MYVECTOR3 temp_lookat = pos + vec * 10 * R;

	MYVECTOR3 dk = (temp_lookat - lookat);
	float dkyor = MyVec3Length(dk);
	if (abs(dkyor) > 0.1f) {
		if (kyori < dsecond * maxspeed/200) {
		lookatspeed += maxspeed / 10.0f;
			if (lookatspeed > maxspeed) {
				lookatspeed = maxspeed;
			}
		
		}else
		if(dkyor < 0.0f) {
		    // ‰½‚à‚µ‚È‚¢
		} else if (dkyor > 300000* kyori) {
			lookat = temp_lookat;
			lookatspeed = maxspeed;
		}else if (dkyor > kyori) {
			lookatspeed += maxspeed * kyori/dkyori;
			if (lookatspeed > maxspeed) {
				lookatspeed = maxspeed;
			}
			lookat = lookat + dk * kyori/dkyor;
		}else  {
			lookatspeed += maxspeed / 10.0f;
			if (lookatspeed > maxspeed) {
				lookatspeed = maxspeed;
			}
			lookat = lookat + dk / 100.0f*offset*10;

		}
	} else {
		lookatspeed /= 1.1f;
	}
	MyMatrixLookAtRH(view,lookfrom,lookat,up);


}

void Gamen_MISSION::byouga(Graphics* g, GUI* gui, float dsecond, int stamp) {

	static int stam =0;
	if (stam) {
		if (stam != stamp) {

			mis.decSecond();
			stam  = stamp;
			int s_ue = mis.getSecondUe();
			int s_sita = mis.getSecondSita();
			this->setTimerTex(s_sita,s_ue);
		}
	}else {
		stam = stamp;
	}
	
	if (robo) {
		setHoui(robo->atarihan->rotz);
		setView(&robo->atarihan->world,robo->atarihan->r,dsecond);
	}
}

void Gamen_MISSION::setTexSuuji(int suuji,int tex_id) {
	if (suuji == 0) {
		t->setRenderTexTexPos(tex_id, 1,69,65-1,132-69);
	} else if(suuji == 1) {
		t->setRenderTexTexPos(tex_id, 69,69,131-69,132-69);
	} else if(suuji == 2) {
		t->setRenderTexTexPos(tex_id, 137,69,199-137,132-69);
	} else if(suuji == 3) {
		t->setRenderTexTexPos(tex_id, 203,69,266-203,132-69);
	} else if(suuji == 4) {
		t->setRenderTexTexPos(tex_id, 1,136,63-1,199-136);
	} else if(suuji == 5) {
		t->setRenderTexTexPos(tex_id, 69,136,132-69,199-136);
	} else if(suuji == 6) {
		t->setRenderTexTexPos(tex_id, 137,136,200-137,199-136);
	} else if(suuji == 7) {
		t->setRenderTexTexPos(tex_id, 202,136,266-202,199-136);
	} else if(suuji == 8) {
		t->setRenderTexTexPos(tex_id, 1,203,64-1,265-203);
	} else if(suuji == 9) {
		t->setRenderTexTexPos(tex_id, 69,203,132-69,265-203);
	}

}

void Gamen_MISSION::setTimerTex(int second_sita, int second_ue) {
	if (second_ue >= 100) {
		second_ue = 99;
	}
	setTexSuuji(second_ue % 10, timer_suuji_tex[1]);
	setTexSuuji(second_ue / 10, timer_suuji_tex[0]);
	setTexSuuji(second_sita % 10, timer_suuji_tex[3]);
	setTexSuuji(second_sita / 10, timer_suuji_tex[2]);
}	

void Gamen_MISSION::Init() {
	int tex_id = t->getTexture(KTROBO_MISSION_GAMEN_PILOT_PNG);
	/*
	int timer_suuji_tex[4];
	int ap_suuji_tex[5];
	int houi_tex;
	int a_tex;
	int p_tex;
	int timer_tex;
	int energy_tex;
	int n_tex;
	int e_tex;
	int s_tex;
	int w_tex;
	*/
	timer_suuji_tex[0] = t->getRenderTex(tex_id,0xFFFFFFFF,720,30,50,50,0,64,63,63);
	timer_suuji_tex[1] = t->getRenderTex(tex_id,0xFFFFFFFF,770,30,50,50,0,64,63,63);
	timer_suuji_tex[2] = t->getRenderTex(tex_id,0xFFFFFFFF,860,30,50,50,0,64,63,63);
	timer_suuji_tex[3] = t->getRenderTex(tex_id,0xFFFFFFFF,910,30,50,50,0,64,63,63);

	a_tex = t->getRenderTex(tex_id, 0xFFFFFFFF,0,0,50,50,136,203,63,63);
	p_tex = t->getRenderTex(tex_id, 0xFFFFFFFF,50,0,50,50,203,202,63,63);
	for (int i=0;i<5;i++) {
	ap_suuji_tex[i] = t->getRenderTex(tex_id,0xFFFFFFFF,100+i*50,0,50,50,0,64,63,63);
	}
	timer_tex = t->getRenderTex(tex_id,0xFFFFFFFF,706,6,250,100,274,236,495-274,352-236+2);
	n_tex = t->getRenderTex(tex_id,0xFFFFFFFF,500-20/2,110,20,20,0,0,68,64);
	e_tex = t->getRenderTex(tex_id,0xFFFFFFFF,250-20/2,110,20,20,69,0,66,64);
	s_tex = t->getRenderTex(tex_id,0xFFFFFFFF,500-20/2,110,20,20,136,0,65,64);
	w_tex = t->getRenderTex(tex_id,0xFFFFFFFF,750-20/2,110,20,20,202,0,266-202-1,64);

	houi_tex = t->getRenderTex(tex_id,0xFFFFFFFF,250,100,500,20,0,278,273,22*2);
	houi_tex2 = t->getRenderTex(tex_id,0xFFFFFFFF,750,100,1,20,273,278,0,22*2);

	energy_tex = t->getRenderTex(tex_id,0xFFFFFFFF,0,100,125,700,445,4,511-445,238-4);

	for (int i=0;i<4;i++) {
		t->setRenderTexIsRender(timer_suuji_tex[i],true);
	}
	for (int i=0;i<5;i++) {
		t->setRenderTexIsRender(ap_suuji_tex[i],true);
	}

	t->setRenderTexIsRender(a_tex,true);
	t->setRenderTexIsRender(p_tex,true);
	t->setRenderTexIsRender(timer_tex,true);
	t->setRenderTexIsRender(houi_tex,true);
	t->setRenderTexIsRender(houi_tex2,true);
	t->setRenderTexIsRender(n_tex,true);
	t->setRenderTexIsRender(e_tex,true);
	t->setRenderTexIsRender(s_tex,true);
	t->setRenderTexIsRender(w_tex,true);
	t->setRenderTexIsRender(energy_tex,true);

}
void Gamen_MISSION::clickedShori(int id) {


}

void Gamen_MISSION::clickedShoriWithData(int id, void* data) {


}




Game_SCENE::Game_SCENE(Graphics* g, AtariHantei* hantei, Texture* tex, Texture* tex2, MyTextureLoader* loader) : Scene("gamescene",9) {
	this->g = g;
	this->hantei = hantei;
	this->tex = tex;
	this->tex2 = tex2;
	this->loader = loader;
	gg = 0;
	gm = 0;
	if (!gm) {
		gm = new Gamen_MISSION(tex,tex2);
		gm->Init();
	}
}
Game_SCENE::~Game_SCENE(void) {
	if (gm) {
		delete gm;
		gm = 0;
	}

}

void Gamen_MISSION::setHoui(float rotz) {
	int sdayo = 500;
	int ndayo = 1000;// ‚Ü‚½‚Í0
	int edayo = 250;
	int wdayo = 750;

	while(rotz <-3.141592) {
		rotz += 6.283184;
	}
	while(rotz > 3.141592) {
		rotz -=6.283184;
	}

	// rotz ‚ª2pai 6.28‚Åˆê‰ñ“](1000i‚Þ)
	sdayo += rotz / 3.141592*500;
	ndayo += rotz / 3.141592*500;
	edayo += rotz / 3.141592*500;
	wdayo += rotz / 3.141592*500;

	if (sdayo > 1000) {
		sdayo -=1000;
	}
	if (ndayo > 1000) {
		ndayo -=1000;
	}
	if (edayo > 1000) {
		edayo -=1000;
	}
	if (wdayo > 1000) {
		wdayo -=1000;
	}

	if (sdayo < 0) {
		sdayo += 1000;
	}
	if (ndayo < 0) {
		ndayo += 1000;
	}
	if (wdayo <0) {
		wdayo += 1000;
	}
	if (edayo < 0) {
		edayo +=1000;
	}
	
	if ((250 <= sdayo) && (750 >= sdayo)) {
		t->setRenderTexIsRender(s_tex,true);
		t->setRenderTexPos(s_tex,sdayo-10,110);
	} else {
		t->setRenderTexIsRender(s_tex,false);
	}

	if ((250 <= wdayo) && (750 >= wdayo)) {
		t->setRenderTexIsRender(w_tex,true);
		t->setRenderTexPos(w_tex,wdayo-10,110);
	} else {
		t->setRenderTexIsRender(w_tex,false);
	}

	if ((250 <= edayo) && (750 >= edayo)) {
		t->setRenderTexIsRender(e_tex,true);
		t->setRenderTexPos(e_tex,edayo-10,110);
	} else {
		t->setRenderTexIsRender(e_tex,false);
	}

	if ((250 <= ndayo) && (750 >= ndayo)) {
		t->setRenderTexIsRender(n_tex,true);
		t->setRenderTexPos(n_tex,ndayo-10,110);
	} else {
		t->setRenderTexIsRender(n_tex,false);
	}

	while (rotz <0) {
		rotz += 1.570796;
	}

	while (rotz > 1.570796) {
		rotz -= 1.570796;
	}
	


	float ritu = rotz / 1.570796/2;
	ritu = 1-ritu;
	t->setRenderTexTexPos(houi_tex, 0+273*ritu/2,278,273/2,44);

}
void Game_SCENE::mainrenderIMPL(bool is_focused, Graphics* g, Game* game) {
	if (gm) {
		gm->robo = game->robodayo;
		gm->byouga(g,NULL,game->getDMSecond(), game->getTimeStamp());
		game->view = gm->view;
		tex->setViewProj(g,&gm->view,g->getProj(),&gm->lookfrom,&gm->lookat);
		tex2->setViewProj(g,&gm->view, g->getProj(),&gm->lookfrom,&gm->lookat);
	}
}

void Game_SCENE::renderhojyoIMPL(Task* task, TCB* thisTCB, Graphics* g, lua_State* l, Game* game) {

}

void Game_SCENE::aiIMPL(Task* task, TCB* thisTCB, Graphics* g, lua_State* l, Game* game) {


}

void Game_SCENE::posbutukariIMPL(Task* task, TCB* thisTCB, Graphics* g, lua_State* l, Game* game) {

	double millisecond = game->stopWatchForButukari();//stopWatch();
	game->startWatchForButukari();
	if (millisecond > RENDERTIME_IGNORETIME) {
		CS::instance()->leave(CS_TASK_CS, "leave main", TASKTHREADS_UPDATEPOSBUTUKARI);
		//Sleep(1);
		millisecond = RENDERTIME_SETTIME*2/3;
		butukari_clock.plus((float)millisecond);
		CS::instance()->enter(CS_TASK_CS, "enter main", TASKTHREADS_UPDATEPOSBUTUKARI);
	}/* else if ( millisecond < RENDERTIME_SETTIME*2/3 ) {
		CS::instance()->leave(CS_TASK_CS, "leave main", TASKTHREADS_UPDATEPOSBUTUKARI);
		butukari_clock.plus((float)millisecond);
		Sleep(DWORD(RENDERTIME_SETTIME*2/3 - millisecond));
		millisecond = RENDERTIME_SETTIME*2/3;
		CS::instance()->enter(CS_TASK_CS, "enter main", TASKTHREADS_UPDATEPOSBUTUKARI);
	}*/ else {
		
		CS::instance()->leave(CS_TASK_CS, "leave main", TASKTHREADS_UPDATEPOSBUTUKARI);
		butukari_clock.plus((float)millisecond);
		//Sleep(1);
		CS::instance()->enter(CS_TASK_CS, "enter main", TASKTHREADS_UPDATEPOSBUTUKARI);
	}

	float frameTime = millisecond;
	int frame = butukari_clock.getSecond();
	/*
	CS::instance()->enter(CS_DEVICECON_CS,"unko");
	CS::instance()->enter(CS_RENDERDATA_CS,"unko");
	*/
	
	/*
	hantei->ataristart();
	hantei->maecalcdayo(g);
	hantei->calcAuInfo(g,true);
	hantei->calcKumi(g);
	hantei->calcObb(g);
	hantei->clearKekkaOfBuffer(g);
	hantei->runComputeShader(g);

	hantei->copyKekkaToBufferForCopy(g,true);
	
	hantei->calcKumiKuwasiku(g);
	hantei->runComputeShaderKuwasiku(g);
	hantei->copyKekkaToBufferForCopy(g,false);
	*/

//	g->getDeviceContext()->RSSetViewports(1, g->getViewPort());
	/*
	if (gm) {
	if (hantei->canGetAns()) {
		if (game->robodayo->atarihan) {
			game->robodayo->atarishori(g, &gm->view, hantei, frameTime, (int)frame);
		}

		if (game->roboaitedayo->atarihan) {
			game->roboaitedayo->atarishori(g, &gm->view, hantei, frameTime, (int)frame);
		}

		
		//hantei->drawKekka(g,&view,&proj);
		hantei->setIsCalcKuwasikuGetted();
	}
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "unko");
	CS::instance()->leave(CS_DEVICECON_CS, "unko");
	*/
}

void Game_SCENE::loaddestructIMPL(Task* task, TCB* thisTCB, Graphics* g, lua_State* l, Game* game) {



}

void Game_SCENE::enter() {
	INPUTGETBYMESSAGESTRUCT* ss  = InputMessageDispatcher::getRootInputGetStruct();
	while (ss->getParent()) {
		ss = ss->getParent();
	}
	InputMessageDispatcher::registerImpl(this, NULL, ss->impl);

	Scene::enter();
}
void Game_SCENE::leave() {
	Scene::leave();

	
	InputMessageDispatcher::unregisterImpl(this);
	//	gg->Release();
	//	delete gg;
	//	gg = 0;
	//}

}
bool Game_SCENE::handleMessage(int msg, void* data, DWORD time) {


	return true;
}
