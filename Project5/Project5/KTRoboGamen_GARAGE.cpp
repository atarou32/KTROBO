#include "KTRoboGamen_GARAGE.h"
#include "KTRoboArmPositioner.h"

using namespace KTROBO;
Gamen_GARAGE::Gamen_GARAGE(void)
{
	parts_category_list=0;
	parts_leg_category_list=0;
	parts_rweapon_category_list=0;
	parts_lweapon_category_list=0;
	parts_rkata_category_list=0;
	parts_lkata_category_list=0;
	parts_inside_category_list=0;
	robo = 0;
	clearrobogamen = 0;
	pressed_up_count = 0;
	pressed_down_count = 0;
}


Gamen_GARAGE::~Gamen_GARAGE(void)
{
}


void Gamen_GARAGE::byouga(Graphics* g, GUI* gui, float dsecond, int stamp) {
	const D3D11_VIEWPORT* ggg = g->getViewPort();
	D3D11_VIEWPORT ggg2;
	D3D11_VIEWPORT ggg3;
	ggg2= *ggg;
	ggg3.TopLeftX = 50;
	ggg3.TopLeftY = 300;
	ggg3.Width = 400;
	ggg3.Height = 400;
	ggg3.MaxDepth = 1;
	ggg3.MinDepth = 0;
	g->getDeviceContext()->RSSetViewports(1,&ggg3);
	static float unko=0;
	unko += dsecond/3333;
	MYMATRIX view;
	MYVECTOR3 lookat(0,0,0);
	MYVECTOR3 lookfrom(0,7,2);
	MYVECTOR3 up(0,0,1);
	MYMATRIX tes;
	MyMatrixRotationZ(tes,unko);
	MyVec3TransformNormal(lookfrom,lookfrom,tes);
	MyMatrixLookAtRH(view,lookfrom,lookat,up);
	float clearColor[4] = {
		0.6f,0.6f,0.8f,1.0f};
	
	//g->getDeviceContext()->ClearRenderTargetView(g->getRenderTargetView(), clearColor);
	if (robo) {

		robo->aim(g,&view);
		robo->byouga(g,&view,g->getProj());
	}


	g->getDeviceContext()->RSSetViewports(1,&ggg2);
	
	parts_category_list->byouga(g,gui,dsecond,stamp);
}

#define KTROBO_GAMEN_GARAGE_GUI_PNG "resrc/sample/gui.png"
#define KTROBO_GAMEN_GARAGE_KOUMOKU_HEAD_ID 100
#define KTROBO_GAMEN_GARAGE_KOUMOKU_BOOSTER_ID 101
#define KTROBO_GAMEN_GARAGE_KOUMOKU_CORE_ID 102
#define KTROBO_GAMEN_GARAGE_KOUMOKU_ENGINE_ID 103
#define KTROBO_GAMEN_GARAGE_KOUMOKU_FCS_ID 104
#define KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_ID 105
#define KTROBO_GAMEN_GARAGE_KOUMOKU_ARM_ID 106
#define KTROBO_GAMEN_GARAGE_KOUMOKU_LEG_ID 107
#define KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_ID 108
#define KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_ID 109
#define KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_ID 110
#define KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_ID 111


void Koumoku_Parts_Category::Init(Texture* t, MyTextureLoader* loader, char* name) {
	int tex_id = t->getTexture(KTROBO_GAMEN_GARAGE_GUI_PNG);
	gui_koumoku_name_id = t->getRenderText(name, place.left,place.top,place.bottom - place.top, place.right - place.left,
		place.bottom - place.top);
	t->setRenderTextIsRender(gui_koumoku_name_id,true);

	gui_koumoku_name_bg_id = t->getRenderTex(tex_id,0xFFFFFFFF,place.left,place.top,place.right-place.left,place.bottom - place.top,
		0,0,100,100);
	t->setRenderTexIsRender(gui_koumoku_name_bg_id,true);
	this->t = t;
}

void Koumoku_Parts_Category::byouga(Graphics* g, GUI* gui, float dsecond, int stamp) {
	// focused_koumoku のkoumokuでもbyougaは呼ばれる
	t->setRenderTexColor(gui_koumoku_name_bg_id, 0xFFFFFFFFF);
	
	
}
void Koumoku_Parts_Category::focusedByouga(Graphics* g, GUI* gui, float dsecond, int stamp) {
	int stt = stamp % 130;
	unsigned int color = ((((0xFFFF0000 / 130 * stt) & 0x77770000) + 0x88880000) & 0xFFFF0000) + 0x0000FFFF;
	t->setRenderTexColor(gui_koumoku_name_bg_id,color);
}

void Gamen_GARAGE::clickedShori(int id) {

	if (KTROBO_GAMEN_GARAGE_KOUMOKU_ARM_ID == id) {




	} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_BOOSTER_ID == id) {




	} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_CORE_ID == id) {



	} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_ENGINE_ID == id) {


	} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_FCS_ID == id) {






	} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_HEAD_ID == id) {




	} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_ID == id) {




	} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_LEG_ID == id) {



	} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_ID == id) {




	} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_ID == id) {




	} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_ID == id) {



	} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_ID == id) {




	}

}

void Koumoku_Parts_Category::_exedayo(Gamen* gamen, GamenPart* gp, KoumokuList* kl) {

	gamen->clickedShori(this->getID());

}

void Gamen_GARAGE::Init(Graphics* g, AtariHantei* hantei, Texture* t, MyTextureLoader* loader) {
	parts_category_list = new KoumokuList(t);
	parts_category_list->setname("パーツカテゴリ");

	Koumoku_Parts_Category* c = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_HEAD_ID);
	Koumoku_Parts_Category* c2 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_BOOSTER_ID);
	Koumoku_Parts_Category* c3 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_CORE_ID);
	Koumoku_Parts_Category* c4 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_ENGINE_ID);
	Koumoku_Parts_Category* c5 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_FCS_ID);
	Koumoku_Parts_Category* c6 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_ID);
	Koumoku_Parts_Category* c7 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_ARM_ID);
	Koumoku_Parts_Category* c8 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_LEG_ID);
	Koumoku_Parts_Category* c9 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_ID);
	Koumoku_Parts_Category* c10 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_ID);
	Koumoku_Parts_Category* c11= new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_ID);
	Koumoku_Parts_Category* c12 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_ID);

	c->setEnabled(true);
	c2->setEnabled(true);
	c3->setEnabled(true);
	c4->setEnabled(true);
	c5->setEnabled(true);
	c6->setEnabled(true);
	c7->setEnabled(true);
	c8->setEnabled(true);
	c9->setEnabled(true);
	c10->setEnabled(true);
	c11->setEnabled(true);
	c12->setEnabled(true);
	
	
	parts_category_list->setKoumoku(c);
	parts_category_list->setKoumoku(c2);
	parts_category_list->setKoumoku(c3);
	parts_category_list->setKoumoku(c4);
	parts_category_list->setKoumoku(c5);
	parts_category_list->setKoumoku(c6);
	parts_category_list->setKoumoku(c7);
	parts_category_list->setKoumoku(c8);
	parts_category_list->setKoumoku(c9);
	parts_category_list->setKoumoku(c10);
	parts_category_list->setKoumoku(c11);
	parts_category_list->setKoumoku(c12);

	parts_category_list->setEnable(true);
	parts_category_list->setVisible(t,true);
	parts_category_list->setSize(t,0,0,170,300);

	c->Init(t,loader,"ヘッド");
	c2->Init(t,loader, "ブースタ");
	c3->Init(t,loader, "コア");
	c4->Init(t,loader,"エンジン");
	c5->Init(t,loader, "FCS");
	c6->Init(t,loader, "インサイド");
	c7->Init(t,loader,"アーム");
	c8->Init(t,loader, "レッグ");
	c9->Init(t,loader, "右手武器");
	c10->Init(t,loader,"左手武器");
	c11->Init(t,loader, "右肩武器");
	c12->Init(t,loader, "左肩武器");

	parts_category_list->setHyouji3Mode(false);
	parts_category_list->clickedDown();
	parts_category_list->clickedUp();


	robo = new Robo();
	robo->init(g, loader,NULL);
	robo->atarihan->setXYZ(0,0,0);
	
	ArmPoint* ap = robo->apinfo->getArmPoint(502);
	MYVECTOR3 pp = ap->pos;
	//pp.float3.y = - pp.float3.y;
	robo->setTarget(&pp);

	int tex_id = t->getTexture(KTROBO_GUI_PNG,4096);
	clearrobogamen = t->getRenderTex(tex_id,0xDDEEFFDD, 50,300,400,400,0,0,128,128);
	t->setRenderTexIsRender(clearrobogamen,true);
}

void Gamen_GARAGE::Release() {
	if (parts_category_list) {
		delete parts_category_list;
		parts_category_list = 0;
	}
	if (parts_leg_category_list) {
		delete parts_leg_category_list;
		parts_leg_category_list=0;
	}
	if (parts_inside_category_list) {
		delete parts_inside_category_list;
		parts_inside_category_list = 0;
	}

	if (parts_lkata_category_list) {
		delete parts_lkata_category_list;
		parts_lkata_category_list=0;
	}

	if (parts_rkata_category_list) {
		delete parts_rkata_category_list;
		parts_rkata_category_list=0;
	}
	if (parts_lweapon_category_list) {
		delete parts_lweapon_category_list;
		parts_lweapon_category_list=0;
	}

	if (parts_rweapon_category_list) {
		delete parts_rweapon_category_list;
		parts_rweapon_category_list=0;
	}


	if (robo) {
		robo->release();
		delete robo;
		robo = 0;
	}
}


bool Gamen_GARAGE::handleMessage(int msg, void* data, DWORD time) {


	MYINPUTMESSAGESTRUCT* input = (MYINPUTMESSAGESTRUCT*) data;
	float x = input->getMOUSESTATE()->mouse_x;
	float y = input->getMOUSESTATE()->mouse_y;


	CS::instance()->enter(CS_MESSAGE_CS, "enter");
	if (msg == KTROBO_INPUT_MESSAGE_ID_MOUSERAWSTATE) {
		if (input->getMOUSESTATE()->mouse_l_button_pressed) {
			parts_category_list->clicked(this,NULL,x,y);
		}
	}
	if (msg == KTROBO_INPUT_MESSAGE_ID_KEYDOWN) {
		
		
		if (input->getKEYSTATE()[VK_DOWN] & KTROBO_INPUT_BUTTON_DOWN) {
			pressed_down_count=1;
			parts_category_list->clickedDown();
		}
		if (input->getKEYSTATE()[VK_UP] & KTROBO_INPUT_BUTTON_DOWN) {
			pressed_up_count=1;
			parts_category_list->clickedUp();
		}
		if (input->getKEYSTATE()[VK_RETURN] & KTROBO_INPUT_BUTTON_DOWN) {
			parts_category_list->clickedEnter(this,NULL);
		}
	}

	if (msg == KTROBO_INPUT_MESSAGE_ID_KEYUP) {
		if (input->getKEYSTATE()[VK_DOWN] & KTROBO_INPUT_BUTTON_UP) {
			pressed_down_count = 0;
		}
		if (input->getKEYSTATE()[VK_UP] & KTROBO_INPUT_BUTTON_UP) {
			pressed_up_count = 0;
		}
	}
	if (pressed_up_count > 0 ) {
		if (pressed_up_count > 1) {
			if (input->getKEYSTATE()[VK_UP] & KTROBO_INPUT_BUTTON_PRESSED) {
				parts_category_list->clickedUp();
			}



		}else {
			if (input->getKEYSTATE()[VK_UP] & KTROBO_INPUT_BUTTON_PRESSED) {
				pressed_up_count++;
			}
		}
	}
		if (pressed_down_count>0) {
		if (pressed_down_count > 1) {

			if (input->getKEYSTATE()[VK_DOWN] & KTROBO_INPUT_BUTTON_PRESSED) {
				parts_category_list->clickedDown();
			}


		}else {
			if (input->getKEYSTATE()[VK_DOWN] & KTROBO_INPUT_BUTTON_PRESSED) {
				pressed_down_count++;
			}

		}
		}
	

	CS::instance()->leave(CS_MESSAGE_CS, "enter");
	return true;


}