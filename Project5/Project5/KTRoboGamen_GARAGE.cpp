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
	clearpartsgamen =0 ;
	pressed_up_count = 0;
	pressed_down_count = 0;
	temp_focused_list = 0;
	t = 0;
	parts_head_list = 0;
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
	ggg3.TopLeftY = 350;
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
	parts_leg_category_list->byouga(g,gui,dsecond,stamp);
	parts_inside_category_list->byouga(g,gui,dsecond,stamp);
	parts_lkata_category_list->byouga(g,gui,dsecond,stamp);
	parts_lweapon_category_list->byouga(g,gui,dsecond,stamp);
	parts_rweapon_category_list->byouga(g,gui,dsecond,stamp);
	parts_rkata_category_list->byouga(g,gui,dsecond,stamp);
	if (parts_head_list) {
		parts_head_list->byouga(g,gui,dsecond,stamp);
	}
}

#define KTROBO_GAMEN_GARAGE_GUI_PNG "resrc/sample/gui.png"
#define KTROBO_GAMEN_GARAGE_KOUMOKU_NONE 0
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

#define KTROBO_GAMEN_GARAGE_KOUMOKU_LEG_K2_ID 112
#define KTROBO_GAMEN_GARAGE_KOUMOKU_LEG_T2_ID 113
#define KTROBO_GAMEN_GARAGE_KOUMOKU_LEG_J2_ID 114
#define KTROBO_GAMEN_GARAGE_KOUMOKU_LEG_TK_ID 115
#define KTROBO_GAMEN_GARAGE_KOUMOKU_LEG_R_ID 116
#define KTROBO_GAMEN_GARAGE_KOUMOKU_LEG_4K_ID 117


#define KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_ADDMISSILE_ID 118
#define KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_APKAIFUKU_ID 119
#define KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_BIT_ID 120
#define KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_DECOY_ID 121
#define KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_ENERGYZOUFUKU_ID 122
#define KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_SUBCOMPUTER_ID 123
#define KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_KIRAI_ID 124
#define KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_JAMMERROCKET_ID 125
#define KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_ROCKET_ID 126
#define KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_STEALTH_ID 127


#define KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_HANDGUN_ID 128
#define KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_RIFLE_ID 129
#define KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_SHOTGUN_ID 130
#define KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_BAZOOKA_ID 131
#define KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_GRENEDE_ID 132
#define KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_MACHINE_ID 133
#define KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_SNIPER_ID 134
#define KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_ROCKET_ID 135
#define KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_HANABI_ID 136
#define KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_ERIFLE_ID 137
#define KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_PLAZUMA_ID 138	
#define KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_PULSE_ID 139
#define KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_BLADE_ID 140
#define KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_EBLADE_ID 141
#define KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_PILE_ID 142
#define KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_MISSILE_ID 143


#define KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_HANDGUN_ID 144
#define KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_RIFLE_ID 145
#define KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_SHOTGUN_ID 146
#define KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_BAZOOKA_ID 147
#define KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_GRENEDE_ID 148
#define KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_MACHINE_ID 149
#define KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_SNIPER_ID 150
#define KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_ROCKET_ID 151
#define KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_HANABI_ID 152
#define KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_ERIFLE_ID 153
#define KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_PLAZUMA_ID 154	
#define KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_PULSE_ID 155
#define KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_BLADE_ID 156
#define KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_EBLADE_ID 157
#define KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_PILE_ID 158
#define KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_MISSILE_ID 159


#define KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_CHAIN_ID 160
#define KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_GRENEDE_ID 161
#define KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_HANABI_ID 162
#define KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_MISSILE_ID 163
#define KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_PLAZUMA_ID 164
#define KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_PULSE_ID 165
#define KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_LASER_ID 166
#define KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_ROCKET_ID 167


#define KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_CHAIN_ID 168
#define KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_GRENEDE_ID 169
#define KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_HANABI_ID 170
#define KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_MISSILE_ID 171
#define KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_PLAZUMA_ID 172
#define KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_PULSE_ID 173
#define KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_LASER_ID 174
#define KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_ROCKET_ID 175



#define KTROBO_GAMEN_GARAGE_KOUMOKU_PARTS_ID 176











void Koumoku_Parts::Init(Texture* t, MyTextureLoader* loader) {

	int tex_id = t->getTexture(KTROBO_GAMEN_GARAGE_GUI_PNG);
	gui_koumoku_name_id = t->getRenderText(parts->data->getData("name")->string_data, place.left,place.top,place.bottom - place.top, place.right - place.left,
		place.bottom - place.top);
	t->setRenderTextIsRender(gui_koumoku_name_id,true);

	gui_koumoku_name_bg_id = t->getRenderTex(tex_id,0xFFFFFFFF,place.left,place.top,place.right-place.left,place.bottom - place.top,
		0,0,100,100);
	t->setRenderTexIsRender(gui_koumoku_name_bg_id,true);
	this->t = t;





}


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

void Koumoku_Parts_Category::byouga(Graphics* g, GUI* gui, float dsecond, int stamp, bool has_clicked) {
	// focused_koumoku のkoumokuでもbyougaは呼ばれる
	if (!getEnabled() && !has_clicked) return;
	t->setRenderTexColor(gui_koumoku_name_bg_id, 0xFFFFFFFFF);
	
	
}
void Koumoku_Parts_Category::focusedByouga(Graphics* g, GUI* gui, float dsecond, int stamp, bool has_clicked) {
	if (!getEnabled() && !has_clicked) return;
	int stt = stamp % 130;
	unsigned int color = ((((0xFFFF0000 / 130 * stt) & 0x77770000) + 0x88880000) & 0xFFFF0000) + 0x0000FFFF;
	t->setRenderTexColor(gui_koumoku_name_bg_id,color);
}
void Gamen_GARAGE::clickedShoriWithData(int id, void* data) {
	Koumoku_Parts* parts_k = (Koumoku_Parts*)data;

	





}

void Koumoku_Parts::byouga(Graphics* g, GUI* gui, float dsecond, int stamp, bool has_clicked) {
	// focused_koumoku のkoumokuでもbyougaは呼ばれる
	if (!getEnabled() && !has_clicked) return;
	t->setRenderTexColor(gui_koumoku_name_bg_id, 0xFFFFFFFFF);
	
}// focused_koumoku のkoumokuでもbyougaは呼ばれる

void Koumoku_Parts::focusedByouga(Graphics* g, GUI* gui, float dsecond, int stamp, bool has_clicked) {

const D3D11_VIEWPORT* ggg = g->getViewPort();
	D3D11_VIEWPORT ggg2;
	D3D11_VIEWPORT ggg3;
	ggg2= *ggg;
	ggg3.TopLeftX = 550;
	ggg3.TopLeftY = 150;
	ggg3.Width = 330;
	ggg3.Height = 330;
	ggg3.MaxDepth = 1;
	ggg3.MinDepth = 0;
	g->getDeviceContext()->RSSetViewports(1,&ggg3);
	static float unko=0;
	unko += dsecond/3333;
	MYMATRIX view;

	float r = parts->getR();
	MYVECTOR3 lookat(0,0,r);
	MYVECTOR3 lookfrom(0,3*r,r*2);
	MYVECTOR3 up(0,0,1);
	MYMATRIX tes;
	MyMatrixRotationZ(tes,unko);
	MyVec3TransformNormal(lookfrom,lookfrom,tes);
	MyMatrixLookAtRH(view,lookfrom,lookat,up);
	float clearColor[4] = {
		0.6f,0.6f,0.8f,1.0f};
	
	//g->getDeviceContext()->ClearRenderTargetView(g->getRenderTargetView(), clearColor);
	if (this->hasLoad() && this->getVisible()) {
		this->parts->drawMesh(g,&view,g->getProj());
	}
	g->getDeviceContext()->RSSetViewports(1,ggg);


	if (!getEnabled() && !has_clicked) return;
	int stt = stamp % 130;
	unsigned int color = ((((0xFFFF0000 / 130 * stt) & 0x77770000) + 0x88880000) & 0xFFFF0000) + 0x0000FFFF;
	t->setRenderTexColor(gui_koumoku_name_bg_id,color);

}
//void clickedExe(Gamen* gamen, GamenPart* gp, KoumokuList* kl); // set_enable がfalse のときはリターンすること

void Koumoku_Parts::_exedayo(Gamen* gamen, GamenPart* gp, KoumokuList* kl) {




}
void KoumokuList_Parts::InitKoumokus(Texture* t, MyTextureLoader* loader) {
		int size = koumokus.size();
		for (int i=0;i<size;i++) {
			Koumoku_Parts* kp = (Koumoku_Parts*)koumokus[i];
			kp->Init(t,loader);
		}
	}


char* KoumokuList_Parts::getFilenameFromCID() {
	if (KTROBO_GAMEN_GARAGE_KOUMOKU_HEAD_ID == category_id) {
	return "resrc/ktrobo/info/metadata/ktroboheadpartsmetadata.txt";
	}
	
	throw new GameError(KTROBO::FATAL_ERROR, "no cid");
}

void Gamen_GARAGE::clickedShori(int id) {

	if (KTROBO_GAMEN_GARAGE_KOUMOKU_ARM_ID == id) {




	} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_BOOSTER_ID == id) {




	} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_CORE_ID == id) {



	} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_ENGINE_ID == id) {


	} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_FCS_ID == id) {






	} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_HEAD_ID == id) {
		if (!parts_head_list) {
			parts_head_list = new KoumokuList_Parts(KTROBO_GAMEN_GARAGE_KOUMOKU_HEAD_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_NONE,t);
			parts_head_list->setname("ヘッドパーツ");
			MyTokenAnalyzer ma;
			ma.load(parts_head_list->getFilenameFromCID());
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
			parts_head_list->setMetaData(head_md);

			ma.load("resrc/ktrobo/info/ktroboheadparts.txt");
			while(!ma.enddayo()) {

				if (strcmp(ma.Toke(), "{")==0) {
					RoboHead* head = new RoboHead();
					Koumoku_Parts* kp = new Koumoku_Parts(KTROBO_GAMEN_GARAGE_KOUMOKU_PARTS_ID,head);
					kp->setEnabled(false);
					
					parts_head_list->setKoumoku(kp);
					
					try {
						head->loadData(&ma,head_md);
						
					
					} catch (GameError* err) {
		
					//	MessageBoxA(g->getHWND(), err->getMessage(), err->getErrorCodeString(err->getErrorCode()), MB_OK);
						ma.deletedayo();
						throw err;
					}
				}
				ma.GetToken();
			}		
			ma.deletedayo();
		
			parts_head_list->setEnable(false);
			parts_head_list->setVisible(t,false);
			int si = parts_head_list->getKoumokuSize();
			parts_head_list->setSize(t,500,0,360,21*si);

			
			parts_head_list->InitKoumokus(t,loader);



			parts_head_list->setHyouji3Mode(false);
			parts_head_list->clickedDown();
			parts_head_list->clickedUp();
			parts_head_list->setEnable(false);
			parts_head_list->setVisible(t,false);
			
		}
		t->setRenderTexIsRender(clearpartsgamen, true);
		parts_head_list->setHyouji3Mode(true);
		
		parts_head_list->setVisible(parts_head_list->t, true);
		parts_head_list->setEnable(true);
		parts_head_list->clickedDown();
		parts_head_list->clickedUp();
		parts_category_list->setEnable(false);
		temp_focused_list = parts_head_list;


	} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_ID == id) {
		parts_inside_category_list->setVisible(parts_inside_category_list->t,true);
		parts_inside_category_list->setEnable(true);
		parts_category_list->setEnable(false);
		temp_focused_list = parts_inside_category_list;



	} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_LEG_ID == id) {
		parts_leg_category_list->setVisible(parts_leg_category_list->t,true);
		parts_leg_category_list->setEnable(true);
		parts_category_list->setEnable(false);
		temp_focused_list = parts_leg_category_list;


	} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_ID == id) {
		parts_lkata_category_list->setVisible(parts_lkata_category_list->t,true);
		parts_lkata_category_list->setEnable(true);
		parts_category_list->setEnable(false);
		temp_focused_list = parts_lkata_category_list;



	} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_ID == id) {

		parts_lweapon_category_list->setVisible(parts_lweapon_category_list->t,true);
		parts_lweapon_category_list->setEnable(true);
		parts_category_list->setEnable(false);
		temp_focused_list = parts_lweapon_category_list;


	} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_ID == id) {
		parts_rkata_category_list->setVisible(parts_rkata_category_list->t,true);
		parts_rkata_category_list->setEnable(true);
		parts_category_list->setEnable(false);
		temp_focused_list = parts_rkata_category_list;


	} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_ID == id) {
		parts_rweapon_category_list->setVisible(parts_rweapon_category_list->t,true);
		parts_rweapon_category_list->setEnable(true);
		parts_category_list->setEnable(false);
		temp_focused_list = parts_rweapon_category_list;
	}

}

void Koumoku_Parts_Category::_exedayo(Gamen* gamen, GamenPart* gp, KoumokuList* kl) {

	gamen->clickedShori(this->getID());

}

void Gamen_GARAGE::Init(Graphics* g, AtariHantei* hantei, Texture* t, MyTextureLoader* loader) {

	parts_rkata_category_list = new KoumokuList(t);
	parts_rkata_category_list->setname("右肩武器カテゴリ");
	Koumoku_Parts_Category* frk = new Koumoku_Parts_Category( KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_CHAIN_ID);
	Koumoku_Parts_Category* frk2 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_GRENEDE_ID);
	Koumoku_Parts_Category* frk3 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_HANABI_ID);
	Koumoku_Parts_Category* frk4 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_MISSILE_ID);
	Koumoku_Parts_Category* frk5 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_PLAZUMA_ID);
	Koumoku_Parts_Category* frk6 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_PULSE_ID);
	Koumoku_Parts_Category* frk7 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_LASER_ID);
	Koumoku_Parts_Category* frk8 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_ROCKET_ID);

	frk->setEnabled(true);
	frk2->setEnabled(true);
	frk3->setEnabled(true);
	frk4->setEnabled(true);
	frk5->setEnabled(true);
	frk6->setEnabled(true);
	frk7->setEnabled(true);
	frk8->setEnabled(true);
	
	
	parts_rkata_category_list->setKoumoku(frk);
	parts_rkata_category_list->setKoumoku(frk2);
	parts_rkata_category_list->setKoumoku(frk3);
	parts_rkata_category_list->setKoumoku(frk4);
	parts_rkata_category_list->setKoumoku(frk5);
	parts_rkata_category_list->setKoumoku(frk6);
	parts_rkata_category_list->setKoumoku(frk7);
	parts_rkata_category_list->setKoumoku(frk8);
		   
	parts_rkata_category_list->setEnable(true);
	parts_rkata_category_list->setVisible(t,true);
	parts_rkata_category_list->setSize(t,200,0,160,180);

	frk->Init(t,loader, "チェインガン");
	frk2->Init(t,loader, "グレネード");
	frk3->Init(t,loader, "ハナビガン");
	frk4->Init(t,loader,"ミサイル");
	frk5->Init(t,loader, "プラズマガン");
	frk6->Init(t,loader, "パルスガン");
	frk7->Init(t,loader, "レーザー");
	frk8->Init(t,loader, "ロケット");

	parts_rkata_category_list->setHyouji3Mode(false);
	parts_rkata_category_list->clickedDown();
	parts_rkata_category_list->clickedUp();
	parts_rkata_category_list->setEnable(false);
	parts_rkata_category_list->setVisible(t,false);





	parts_lkata_category_list = new KoumokuList(t);
	parts_lkata_category_list->setname("左肩武器カテゴリ");
	Koumoku_Parts_Category* flk = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_CHAIN_ID);
	Koumoku_Parts_Category* flk2 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_GRENEDE_ID);
	Koumoku_Parts_Category* flk3 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_HANABI_ID);
	Koumoku_Parts_Category* flk4 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_MISSILE_ID);
	Koumoku_Parts_Category* flk5 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_PLAZUMA_ID);
	Koumoku_Parts_Category* flk6 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_PULSE_ID);
	Koumoku_Parts_Category* flk7 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_LASER_ID);
	Koumoku_Parts_Category* flk8 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_ROCKET_ID);

	flk->setEnabled(true);
	flk2->setEnabled(true);
	flk3->setEnabled(true);
	flk4->setEnabled(true);
	flk5->setEnabled(true);
	flk6->setEnabled(true);
	flk7->setEnabled(true);
	flk8->setEnabled(true);
	
	
	parts_lkata_category_list->setKoumoku(flk);
	parts_lkata_category_list->setKoumoku(flk2);
	parts_lkata_category_list->setKoumoku(flk3);
	parts_lkata_category_list->setKoumoku(flk4);
	parts_lkata_category_list->setKoumoku(flk5);
	parts_lkata_category_list->setKoumoku(flk6);
	parts_lkata_category_list->setKoumoku(flk7);
	parts_lkata_category_list->setKoumoku(flk8);
		   
	parts_lkata_category_list->setEnable(true);
	parts_lkata_category_list->setVisible(t,true);
	parts_lkata_category_list->setSize(t,200,0,160,180);

	flk->Init(t,loader, "チェインガン");
	flk2->Init(t,loader, "グレネード");
	flk3->Init(t,loader, "ハナビガン");
	flk4->Init(t,loader,"ミサイル");
	flk5->Init(t,loader, "プラズマガン");
	flk6->Init(t,loader, "パルスガン");
	flk7->Init(t,loader, "レーザー");
	flk8->Init(t,loader, "ロケット");



	parts_lkata_category_list->setHyouji3Mode(false);
	parts_lkata_category_list->clickedDown();
	parts_lkata_category_list->clickedUp();
	parts_lkata_category_list->setEnable(false);
	parts_lkata_category_list->setVisible(t,false);




	parts_lweapon_category_list = new KoumokuList(t);
	parts_lweapon_category_list->setname("左手武器カテゴリ");
	Koumoku_Parts_Category* fl = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_HANDGUN_ID);
	Koumoku_Parts_Category* fl2 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_RIFLE_ID);
	Koumoku_Parts_Category* fl3 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_SHOTGUN_ID);
	Koumoku_Parts_Category* fl4 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_BAZOOKA_ID);
	Koumoku_Parts_Category* fl5 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_GRENEDE_ID);
	Koumoku_Parts_Category* fl6 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_MACHINE_ID);
	Koumoku_Parts_Category* fl7 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_SNIPER_ID);
	Koumoku_Parts_Category* fl8 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_ROCKET_ID);
	Koumoku_Parts_Category* fl9 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_HANABI_ID);
	Koumoku_Parts_Category* fl10 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_ERIFLE_ID);
	Koumoku_Parts_Category* fl11 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_PLAZUMA_ID);	
	Koumoku_Parts_Category* fl12 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_PULSE_ID);
	Koumoku_Parts_Category* fl13 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_BLADE_ID);
	Koumoku_Parts_Category* fl14 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_EBLADE_ID);
	Koumoku_Parts_Category* fl15 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_PILE_ID);
	Koumoku_Parts_Category* fl16 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_MISSILE_ID);

	fl->setEnabled(true);
	fl2->setEnabled(true);
	fl3->setEnabled(true);
	fl4->setEnabled(true);
	fl5->setEnabled(true);
	fl6->setEnabled(true);
	fl7->setEnabled(true);
	fl8->setEnabled(true);
	fl9->setEnabled(true);
	fl10->setEnabled(true);
	fl11->setEnabled(true);
	fl12->setEnabled(true);
	fl13->setEnabled(true);
	fl14->setEnabled(true);
	fl15->setEnabled(true);
	fl16->setEnabled(true);
	
	parts_lweapon_category_list->setKoumoku(fl);
	parts_lweapon_category_list->setKoumoku(fl2);
	parts_lweapon_category_list->setKoumoku(fl3);
	parts_lweapon_category_list->setKoumoku(fl4);
	parts_lweapon_category_list->setKoumoku(fl5);
	parts_lweapon_category_list->setKoumoku(fl6);
	parts_lweapon_category_list->setKoumoku(fl7);
	parts_lweapon_category_list->setKoumoku(fl8);
	parts_lweapon_category_list->setKoumoku(fl9);
	parts_lweapon_category_list->setKoumoku(fl10);
	parts_lweapon_category_list->setKoumoku(fl11);
	parts_lweapon_category_list->setKoumoku(fl12);
	parts_lweapon_category_list->setKoumoku(fl13);
	parts_lweapon_category_list->setKoumoku(fl14);
	parts_lweapon_category_list->setKoumoku(fl15);
	parts_lweapon_category_list->setKoumoku(fl16);
	parts_lweapon_category_list->setEnable(true);
	parts_lweapon_category_list->setVisible(t,true);
	parts_lweapon_category_list->setSize(t,200,0,200,350);

	fl->Init(t,loader, "ハンドガン");
	fl2->Init(t,loader, "ライフル");
	fl3->Init(t,loader, "ショットガン");
	fl4->Init(t,loader,"バズーカ");
	fl5->Init(t,loader, "グレネード");
	fl6->Init(t,loader, "マシンガン");
	fl7->Init(t,loader, "スナイパーライフル");
	fl8->Init(t,loader, "ロケット");
	fl9->Init(t,loader, "ハナビガン");
	fl10->Init(t,loader, "Eライフル");
	fl11->Init(t,loader, "プラズマガン");
	fl12->Init(t,loader, "パルスガン");
	fl13->Init(t,loader, "ブレード");
	fl14->Init(t,loader, "Eブレード");
	fl15->Init(t,loader, "パイル");
	fl16->Init(t,loader, "ミサイル");


	parts_lweapon_category_list->setHyouji3Mode(false);
	parts_lweapon_category_list->clickedDown();
	parts_lweapon_category_list->clickedUp();
	parts_lweapon_category_list->setEnable(false);
	parts_lweapon_category_list->setVisible(t,false);




	parts_rweapon_category_list = new KoumokuList(t);
	parts_rweapon_category_list->setname("右手武器カテゴリ");
	Koumoku_Parts_Category* fr = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_HANDGUN_ID);
	Koumoku_Parts_Category* fr2 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_RIFLE_ID);
	Koumoku_Parts_Category* fr3 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_SHOTGUN_ID);
	Koumoku_Parts_Category* fr4 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_BAZOOKA_ID);
	Koumoku_Parts_Category* fr5 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_GRENEDE_ID);
	Koumoku_Parts_Category* fr6 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_MACHINE_ID);
	Koumoku_Parts_Category* fr7 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_SNIPER_ID);
	Koumoku_Parts_Category* fr8 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_ROCKET_ID);
	Koumoku_Parts_Category* fr9 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_HANABI_ID);
	Koumoku_Parts_Category* fr10 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_ERIFLE_ID);
	Koumoku_Parts_Category* fr11 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_PLAZUMA_ID);	
	Koumoku_Parts_Category* fr12 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_PULSE_ID);
	Koumoku_Parts_Category* fr13 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_BLADE_ID);
	Koumoku_Parts_Category* fr14 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_EBLADE_ID);
	Koumoku_Parts_Category* fr15 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_PILE_ID);
	Koumoku_Parts_Category* fr16 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_MISSILE_ID);

	fr->setEnabled(true);
	fr2->setEnabled(true);
	fr3->setEnabled(true);
	fr4->setEnabled(true);
	fr5->setEnabled(true);
	fr6->setEnabled(true);
	fr7->setEnabled(true);
	fr8->setEnabled(true);
	fr9->setEnabled(true);
	fr10->setEnabled(true);
	fr11->setEnabled(true);
	fr12->setEnabled(true);
	fr13->setEnabled(true);
	fr14->setEnabled(true);
	fr15->setEnabled(true);
	fr16->setEnabled(true);
	
	parts_rweapon_category_list->setKoumoku(fr);
	parts_rweapon_category_list->setKoumoku(fr2);
	parts_rweapon_category_list->setKoumoku(fr3);
	parts_rweapon_category_list->setKoumoku(fr4);
	parts_rweapon_category_list->setKoumoku(fr5);
	parts_rweapon_category_list->setKoumoku(fr6);
	parts_rweapon_category_list->setKoumoku(fr7);
	parts_rweapon_category_list->setKoumoku(fr8);
	parts_rweapon_category_list->setKoumoku(fr9);
	parts_rweapon_category_list->setKoumoku(fr10);
	parts_rweapon_category_list->setKoumoku(fr11);
	parts_rweapon_category_list->setKoumoku(fr12);
	parts_rweapon_category_list->setKoumoku(fr13);
	parts_rweapon_category_list->setKoumoku(fr14);
	parts_rweapon_category_list->setKoumoku(fr15);
	parts_rweapon_category_list->setKoumoku(fr16);
	parts_rweapon_category_list->setEnable(true);
	parts_rweapon_category_list->setVisible(t,true);
	parts_rweapon_category_list->setSize(t,200,0,200,350);

	fr->Init(t,loader, "ハンドガン");
	fr2->Init(t,loader, "ライフル");
	fr3->Init(t,loader, "ショットガン");
	fr4->Init(t,loader,"バズーカ");
	fr5->Init(t,loader, "グレネード");
	fr6->Init(t,loader, "マシンガン");
	fr7->Init(t,loader, "スナイパーライフル");
	fr8->Init(t,loader, "ロケット");
	fr9->Init(t,loader, "ハナビガン");
	fr10->Init(t,loader, "Eライフル");
	fr11->Init(t,loader, "プラズマガン");
	fr12->Init(t,loader, "パルスガン");
	fr13->Init(t,loader, "ブレード");
	fr14->Init(t,loader, "Eブレード");
	fr15->Init(t,loader, "パイル");
	fr16->Init(t,loader, "ミサイル");


	parts_rweapon_category_list->setHyouji3Mode(false);
	parts_rweapon_category_list->clickedDown();
	parts_rweapon_category_list->clickedUp();
	parts_rweapon_category_list->setEnable(false);
	parts_rweapon_category_list->setVisible(t,false);






	parts_inside_category_list = new KoumokuList(t);
	parts_inside_category_list->setname("インサイドカテゴリ");
	Koumoku_Parts_Category* fi = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_ADDMISSILE_ID);
	Koumoku_Parts_Category* fi2 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_APKAIFUKU_ID);
	Koumoku_Parts_Category* fi3 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_BIT_ID);
	Koumoku_Parts_Category* fi4 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_DECOY_ID);
	Koumoku_Parts_Category* fi5 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_ENERGYZOUFUKU_ID);
	Koumoku_Parts_Category* fi6 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_JAMMERROCKET_ID);
	Koumoku_Parts_Category* fi7 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_KIRAI_ID);
	Koumoku_Parts_Category* fi8 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_ROCKET_ID);
	Koumoku_Parts_Category* fi9 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_STEALTH_ID);
	Koumoku_Parts_Category* fi10 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_SUBCOMPUTER_ID);




	fi->setEnabled(true);
	fi2->setEnabled(true);
	fi3->setEnabled(true);
	fi4->setEnabled(true);
	fi5->setEnabled(true);
	fi6->setEnabled(true);
	fi7->setEnabled(true);
	fi8->setEnabled(true);
	fi9->setEnabled(true);
	fi10->setEnabled(true);
	parts_inside_category_list->setKoumoku(fi);
	parts_inside_category_list->setKoumoku(fi2);
	parts_inside_category_list->setKoumoku(fi3);
	parts_inside_category_list->setKoumoku(fi4);
	parts_inside_category_list->setKoumoku(fi5);
	parts_inside_category_list->setKoumoku(fi6);
	parts_inside_category_list->setKoumoku(fi7);
	parts_inside_category_list->setKoumoku(fi8);
	parts_inside_category_list->setKoumoku(fi9);
	parts_inside_category_list->setKoumoku(fi10);
	parts_inside_category_list->setEnable(true);
	parts_inside_category_list->setVisible(t,true);
	parts_inside_category_list->setSize(t,200,0,240,250);

	fi->Init(t,loader,"増弾ミサイル");
	fi2->Init(t,loader, "AP回復装置");
	fi3->Init(t,loader, "ビット");
	fi4->Init(t,loader,"デコイ");
	fi5->Init(t,loader, "エネルギー増幅装置");
	fi6->Init(t,loader, "ジャマーロケット");
	fi7->Init(t,loader, "機雷");
	fi8->Init(t,loader, "ロケット");
	fi9->Init(t,loader, "ステルス");
	fi10->Init(t,loader, "サブコンピュータ");

	parts_inside_category_list->setHyouji3Mode(false);
	parts_inside_category_list->clickedDown();
	parts_inside_category_list->clickedUp();
	parts_inside_category_list->setEnable(false);
	parts_inside_category_list->setVisible(t,false);





	parts_leg_category_list = new KoumokuList(t);
	parts_leg_category_list->setname("レッグカテゴリ");
	Koumoku_Parts_Category* f = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_LEG_K2_ID);
	Koumoku_Parts_Category* f2 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_LEG_T2_ID);
	Koumoku_Parts_Category* f3 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_LEG_J2_ID);
	Koumoku_Parts_Category* f4 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_LEG_TK_ID);
	Koumoku_Parts_Category* f5 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_LEG_R_ID);
	Koumoku_Parts_Category* f6 = new Koumoku_Parts_Category(KTROBO_GAMEN_GARAGE_KOUMOKU_LEG_4K_ID);
	f->setEnabled(true);
	f2->setEnabled(true);
	f3->setEnabled(true);
	f4->setEnabled(true);
	f5->setEnabled(true);
	f6->setEnabled(true);
	parts_leg_category_list->setKoumoku(f);
	parts_leg_category_list->setKoumoku(f2);
	parts_leg_category_list->setKoumoku(f3);
	parts_leg_category_list->setKoumoku(f4);
	parts_leg_category_list->setKoumoku(f5);
	parts_leg_category_list->setKoumoku(f6);
	parts_leg_category_list->setEnable(true);
	parts_leg_category_list->setVisible(t,true);
	parts_leg_category_list->setSize(t,200,0,170,150);

	f->Init(t,loader,"軽量２脚");
	f2->Init(t,loader, "中量２脚");
	f3->Init(t,loader, "重量２脚");
	f4->Init(t,loader,"タンク");
	f5->Init(t,loader, "逆間接");
	f6->Init(t,loader, "４脚");

	parts_leg_category_list->setHyouji3Mode(false);
	parts_leg_category_list->clickedDown();
	parts_leg_category_list->clickedUp();
	parts_leg_category_list->setEnable(false);
	parts_leg_category_list->setVisible(t,false);




	parts_category_list = new KoumokuList(t);
	temp_focused_list = parts_category_list;

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
	clearrobogamen = t->getRenderTex(tex_id,0xDDEEFFDD, 50,350,400,400,0,0,128,128);
	t->setRenderTexIsRender(clearrobogamen,true);

	clearpartsgamen = t->getRenderTex(tex_id, 0xDDEEFFDD, 550,150,330,330,0,0,128,128);
	t->setRenderTexIsRender(clearpartsgamen,false);



	this->t = t;
	this->loader = loader;
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

	if (t) {
		t->lightdeleteAllRenderTex();
		t->lightdeleteAllBillBoard();
	}
	if (parts_head_list) {
		delete parts_head_list;
		parts_head_list = 0;
	}
}

void KoumokuList_Parts::load(Graphics* g, MyTextureLoader* loader) {

	int size = koumokus.size();
	for (int i=0;i<size;i++) {
		if (!koumokus[i]->hasLoad()) {
			koumokus[i]->load(g, loader);
			return; // 一度に全てをロードしないで抜ける
		}
	}
}


void Gamen_GARAGE::loadData(Graphics* g, float dsecond, int stamp) {
	
	if (temp_focused_list) {
		if (temp_focused_list->hasLoad()) {
		} else {
			temp_focused_list->load(g, g->getTexLoader());
		}
	}
}

void Gamen_GARAGE::clickedEscape() {
	temp_focused_list = parts_category_list;
	parts_leg_category_list->setEnable(false);
	parts_leg_category_list->setVisible(parts_leg_category_list->t,false);
	parts_lkata_category_list->setEnable(false);
	parts_lkata_category_list->setVisible(parts_lkata_category_list->t,false);
	parts_lweapon_category_list->setEnable(false);
	parts_lweapon_category_list->setVisible(parts_lweapon_category_list->t,false);
	parts_rkata_category_list->setEnable(false);
	parts_rkata_category_list->setVisible(parts_rkata_category_list->t,false);
	parts_rweapon_category_list->setEnable(false);
	parts_rweapon_category_list->setVisible(parts_rweapon_category_list->t,false);
	parts_inside_category_list->setEnable(false);
	parts_inside_category_list->setVisible(parts_inside_category_list->t,false);
	parts_category_list->setEnable(true);
	parts_category_list->setVisible(parts_category_list->t,true);

	if (parts_head_list) {
		parts_head_list->setEnable(false);
		parts_head_list->setVisible(parts_head_list->t, false);
	}

	t->setRenderTexIsRender(clearpartsgamen, false);

}


bool Gamen_GARAGE::handleMessage(int msg, void* data, DWORD time) {


	MYINPUTMESSAGESTRUCT* input = (MYINPUTMESSAGESTRUCT*) data;
	float x = input->getMOUSESTATE()->mouse_x;
	float y = input->getMOUSESTATE()->mouse_y;


	CS::instance()->enter(CS_MESSAGE_CS, "enter");
	if (msg == KTROBO_INPUT_MESSAGE_ID_MOUSERAWSTATE) {
		if (input->getMOUSESTATE()->mouse_l_button_pressed) {
			temp_focused_list->clicked(this,NULL,x,y);
		}
	}
	if (msg == KTROBO_INPUT_MESSAGE_ID_KEYDOWN) {
		
		
		if (input->getKEYSTATE()[VK_DOWN] & KTROBO_INPUT_BUTTON_DOWN) {
			pressed_down_count=1;
			temp_focused_list->clickedDown();
		}
		if (input->getKEYSTATE()[VK_UP] & KTROBO_INPUT_BUTTON_DOWN) {
			pressed_up_count=1;
			temp_focused_list->clickedUp();
		}
		if (input->getKEYSTATE()[VK_RETURN] & KTROBO_INPUT_BUTTON_DOWN) {
			temp_focused_list->clickedEnter(this,NULL);
		}
		if (input->getKEYSTATE()[VK_ESCAPE] & KTROBO_INPUT_BUTTON_DOWN) {
			clickedEscape();

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
				temp_focused_list->clickedUp();
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
				temp_focused_list->clickedDown();
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



bool Koumoku_Parts::hasLoad() {
	if (parts->hasMeshLoaded()) {
		return true;
	}
	return false;
}

void Koumoku_Parts::load(Graphics* g, MyTextureLoader* loader) {
	if (parts->hasMeshLoaded()) {

	} else {
		parts->loadMesh(g,loader);
	
	}
}

