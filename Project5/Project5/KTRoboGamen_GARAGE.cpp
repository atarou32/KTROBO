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
	clearcommentgamen = 0;
	pressed_up_count = 0;
	pressed_down_count = 0;
	temp_focused_list = 0;
	t = 0;
	parts_head_list = 0;
	parts_booster_list = 0;
	parts_core_list = 0;
	parts_engine_list = 0;
	parts_fcs_list = 0;
	parts_arm_list = 0;
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
	if (parts_booster_list) {
		parts_booster_list->byouga(g, gui , dsecond, stamp);
	}
	if (parts_core_list) {
		parts_core_list->byouga(g,gui,dsecond, stamp);
	}
	if (parts_engine_list) {
		parts_engine_list->byouga(g,gui,dsecond,stamp);
	}
	if (parts_fcs_list) {
		parts_fcs_list->byouga(g, gui,dsecond, stamp);
	}
	if (parts_arm_list) {
		parts_arm_list->byouga(g,gui,dsecond,stamp);
	}

	vector<KoumokuList_Parts*>::iterator it;

	it = parts_leg_lists.begin();
	while(it != parts_leg_lists.end()) {
		KoumokuList_Parts* kk = *it;
		kk->byouga(g,gui,dsecond,stamp);
		it++;
	}

	it = parts_inside_lists.begin();
	while(it != parts_inside_lists.end()) {
		KoumokuList_Parts* kk = *it;
		kk->byouga(g,gui,dsecond,stamp);
		it++;
	}

	it = parts_rarmweapon_lists.begin();
	while(it != parts_rarmweapon_lists.end()) {
		KoumokuList_Parts* kk = *it;
		kk->byouga(g,gui,dsecond,stamp);
		it++;
	}

	it = parts_larmweapon_lists.begin();
	while(it != parts_larmweapon_lists.end()) {
		KoumokuList_Parts* kk = *it;
		kk->byouga(g,gui,dsecond,stamp);
		it++;
	}

	it = parts_rkataweapon_lists.begin();
	while(it != parts_rkataweapon_lists.end()) {
		KoumokuList_Parts* kk = *it;
		kk->byouga(g,gui,dsecond,stamp);
		it++;
	}

	it = parts_lkataweapon_lists.begin();
	while(it != parts_lkataweapon_lists.end()) {
		KoumokuList_Parts* kk = *it;
		kk->byouga(g,gui,dsecond,stamp);
		it++;
	}
	gamenpart_yesnoequip.byouga(g,gui,dsecond,stamp);

}

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
	static float unko=0;
	unko += dsecond;
	if (unko >= 1000) {
		unko =0;
	}

	int stt = (int) (unko) % 130;
	unsigned int color = ((((0xFFFF0000 / 130 * stt) & 0x77770000) + 0x88880000) & 0xFFFF0000) + 0x0000FFFF;
	t->setRenderTexColor(gui_koumoku_name_bg_id,color);
}
void Gamen_GARAGE::clickedShoriWithData(int id, void* data) {

	if (KTROBO_GAMEN_GARAGE_KOUMOKU_PARTS_ID == id) { 
		KoumokuList_Parts* parts_k = (KoumokuList_Parts*)data;

			// パーツのYESNOEQUIPにいく
		gamenpart_yesnoequip.setMaeFocusedList(temp_focused_list);
		gamenpart_yesnoequip.setRoboParts(gamenpart_roboparam.getRoboParts(),parts_k->getCategoryID());
		gamenpart_yesnoequip.setFocused(this, true);
		this->temp_focused_list = gamenpart_yesnoequip.kl;
	}
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
	ggg3.TopLeftX = 500;
	ggg3.TopLeftY = 100;
	ggg3.Width = 330;
	ggg3.Height = 330;
	ggg3.MaxDepth = 1;
	ggg3.MinDepth = 0;
	g->getDeviceContext()->RSSetViewports(1,&ggg3);
	static float unko=0;
	unko += dsecond/3333;
	
	
	//g->getDeviceContext()->ClearRenderTargetView(g->getRenderTargetView(), clearColor);
	if (this->hasLoad() && this->getVisible()) {
		MYMATRIX view;

		float r = parts->getR();
		MYVECTOR3 c = parts->getC();
		MYVECTOR3 lookat(c.float3.x,c.float3.y,c.float3.z +r);
		MYVECTOR3 lookfrom(0,3*r,r*2);
		MYVECTOR3 up(0,0,1);
		MYMATRIX tes;
		MyMatrixRotationZ(tes,unko);
		MyVec3TransformNormal(lookfrom,lookfrom,tes);
		MyMatrixLookAtRH(view,lookfrom,lookat,up);
		float clearColor[4] = {
			0.6f,0.6f,0.8f,1.0f};
			this->parts->drawMesh(g,&view,g->getProj());
	}
	g->getDeviceContext()->RSSetViewports(1,ggg);


	if (!getEnabled() && !has_clicked) return;
	//int stt = stamp % 130;
	static float unko2=0;
	unko2 += dsecond;
	if (unko2 >= 1000) {
		unko2 =0;
	}

	int stt = (int) (unko2) % 130;
	unsigned int color = ((((0xFFFF0000 / 130 * stt) & 0x77770000) + 0x88880000) & 0xFFFF0000) + 0x0000FFFF;
	t->setRenderTexColor(gui_koumoku_name_bg_id,color);

}
//void clickedExe(Gamen* gamen, GamenPart* gp, KoumokuList* kl); // set_enable がfalse のときはリターンすること

void Koumoku_Parts::_exedayo(Gamen* gamen, GamenPart* gp, KoumokuList* kl) {

	gamen->clickedShoriWithData(KTROBO_GAMEN_GARAGE_KOUMOKU_PARTS_ID, (void*)kl);

}
void KoumokuList_Parts::InitKoumokus(Texture* t, MyTextureLoader* loader) {
		int size = koumokus.size();
		for (int i=0;i<size;i++) {
			Koumoku_Parts* kp = (Koumoku_Parts*)koumokus[i];
			kp->Init(t,loader);
		}

		parts_setumei_text = t->getRenderText("ここにテキストがきます",0,770,20,880,30);
		t->setRenderTextIsRender(parts_setumei_text, false);


	}

void Koumoku_Parts::setFocused(KoumokuList* kl) {
	KoumokuList_Parts* klp = (KoumokuList_Parts*)kl;
	klp->setFocusedParts(this->parts);
	klp->t->setRenderTextChangeText(klp->parts_setumei_text, parts->data->getData("COMMENT")->string_data);
	klp->t->setRenderTextIsRender(klp->parts_setumei_text,true);
}


char* KoumokuList_Parts::getMetaPartsFilenameFromCID() {
	if (KTROBO_GAMEN_GARAGE_KOUMOKU_HEAD_ID == category_id) {
	return "resrc/ktrobo/info/metadata/ktroboheadpartsmetadata.txt";
	}
	if (KTROBO_GAMEN_GARAGE_KOUMOKU_BOOSTER_ID == category_id) {
		return "resrc/ktrobo/info/metadata/ktroboboosterpartsmetadata.txt";
	}
	if (KTROBO_GAMEN_GARAGE_KOUMOKU_CORE_ID == category_id) {
		return "resrc/ktrobo/info/metadata/ktrobocorepartsmetadata.txt";
	}

	if (KTROBO_GAMEN_GARAGE_KOUMOKU_ENGINE_ID == category_id) {
		return "resrc/ktrobo/info/metadata/ktroboenginepartsmetadata.txt";
	}

	if (KTROBO_GAMEN_GARAGE_KOUMOKU_FCS_ID == category_id) {
		return "resrc/ktrobo/info/metadata/ktrobofcspartsmetadata.txt";
	}

	if (KTROBO_GAMEN_GARAGE_KOUMOKU_ARM_ID == category_id) {
		return "resrc/ktrobo/info/metadata/ktroboarmpartsmetadata.txt";
	}

	if (KTROBO_GAMEN_GARAGE_KOUMOKU_LEG_ID == category_id) {
			return "resrc/ktrobo/info/metadata/ktrobolegpartsmetadata.txt";
	}
	if (KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_ID == category_id) {
				
		if (category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_DECOY_ID) {
			return "resrc/ktrobo/info/metadata/inside/ktroboinsidedecoy.txt";
		} else 	if (category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_ADDMISSILE_ID) {
			return "resrc/ktrobo/info/metadata/inside/ktroboinsidepartsaddmissile.txt";
		} else 	if (category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_APKAIFUKU_ID) {
			return "resrc/ktrobo/info/metadata/inside/ktroboinsidepartsapkaifuku.txt";
		} else 	if (category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_SUBCOMPUTER_ID) {
			return "resrc/ktrobo/info/metadata/inside/ktroboinsidesubcomputer.txt";
		} else 	if (category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_ENERGYZOUFUKU_ID) {
			return "resrc/ktrobo/info/metadata/inside/ktroboinsideenergyzoufuku.txt";
		} else 	if (category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_ROCKET_ID) {
			return "resrc/ktrobo/info/metadata/inside/ktroboinsiderocket.txt";
		} else 	if (category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_JAMMERROCKET_ID) {
			return "resrc/ktrobo/info/metadata/inside/ktroboinsidejyamarocket.txt";
		} else 	if (category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_KIRAI_ID) {
			return "resrc/ktrobo/info/metadata/inside/ktroboinsidekirai.txt";
		} else 	if (category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_BIT_ID) {
			return "resrc/ktrobo/info/metadata/inside/ktroboinsidepartsbit.txt";
		} else 	if (category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_STEALTH_ID) {
			return "resrc/ktrobo/info/metadata/inside/ktroboinsidestealth.txt";
		}

	}

	if (KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_ID == category_id) {
		if (category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_BAZOOKA_ID) {
			return "resrc/ktrobo/info/metadata/weapon/ktrobopartsweaponmetadata.txt";
		} else if(category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_BLADE_ID) {
			return "resrc/ktrobo/info/metadata/weapon/ktrobopartsweaponblade.txt";
		} else if(category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_EBLADE_ID) {
			return "resrc/ktrobo/info/metadata/weapon/ktrobopartsweaponblade.txt";
		} else if(category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_ERIFLE_ID) {
			return "resrc/ktrobo/info/metadata/weapon/ktrobopartsweaponmetadata.txt";
		} else if(category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_GRENEDE_ID) {
			return "resrc/ktrobo/info/metadata/weapon/ktrobopartsweaponbakuhatumetadata.txt";
		} else if(category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_HANABI_ID) {
			return "resrc/ktrobo/info/metadata/weapon/ktrobopartsweaponmetadata.txt";
		} else if(category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_HANDGUN_ID) {
			return "resrc/ktrobo/info/metadata/weapon/ktrobopartsweaponmetadata.txt";
		} else if(category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_MACHINE_ID) {
			return "resrc/ktrobo/info/metadata/weapon/ktrobopartsweaponmetadata.txt";
		} else if(category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_MISSILE_ID) {
			return "resrc/ktrobo/info/metadata/weapon/ktrobopartsweaponyuudoumetadata.txt";
		} else if(category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_PILE_ID) {
			return "resrc/ktrobo/info/metadata/weapon/ktrobopartsweaponpile.txt";
		} else if(category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_PLAZUMA_ID) {
			return "resrc/ktrobo/info/metadata/weapon/ktrobopartsweaponmetadata.txt";
		} else if(category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_PULSE_ID) {
			return "resrc/ktrobo/info/metadata/weapon/ktrobopartsweaponmetadata.txt";
		} else if(category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_RIFLE_ID) {
			return "resrc/ktrobo/info/metadata/weapon/ktrobopartsweaponmetadata.txt";
		} else if(category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_ROCKET_ID) {
			return "resrc/ktrobo/info/metadata/weapon/ktrobopartsweaponmetadata.txt";
		} else if(category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_SHOTGUN_ID) {
			return "resrc/ktrobo/info/metadata/weapon/ktrobopartsweaponmetadata.txt";
		} else if(category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_SNIPER_ID) {
			return "resrc/ktrobo/info/metadata/weapon/ktrobopartsweaponmetadata.txt";
		}
	}

	if (KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_ID == category_id) {
		if (category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_BAZOOKA_ID) {
			return "resrc/ktrobo/info/metadata/weapon/ktrobopartsweaponmetadata.txt";
		} else if(category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_BLADE_ID) {
			return "resrc/ktrobo/info/metadata/weapon/ktrobopartsweaponblade.txt";
		} else if(category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_EBLADE_ID) {
			return "resrc/ktrobo/info/metadata/weapon/ktrobopartsweaponblade.txt";
		} else if(category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_ERIFLE_ID) {
			return "resrc/ktrobo/info/metadata/weapon/ktrobopartsweaponmetadata.txt";
		} else if(category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_GRENEDE_ID) {
			return "resrc/ktrobo/info/metadata/weapon/ktrobopartsweaponbakuhatumetadata.txt";
		} else if(category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_HANABI_ID) {
			return "resrc/ktrobo/info/metadata/weapon/ktrobopartsweaponmetadata.txt";
		} else if(category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_HANDGUN_ID) {
			return "resrc/ktrobo/info/metadata/weapon/ktrobopartsweaponmetadata.txt";
		} else if(category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_MACHINE_ID) {
			return "resrc/ktrobo/info/metadata/weapon/ktrobopartsweaponmetadata.txt";
		} else if(category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_MISSILE_ID) {
			return "resrc/ktrobo/info/metadata/weapon/ktrobopartsweaponyuudoumetadata.txt";
		} else if(category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_PILE_ID) {
			return "resrc/ktrobo/info/metadata/weapon/ktrobopartsweaponpile.txt";
		} else if(category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_PLAZUMA_ID) {
			return "resrc/ktrobo/info/metadata/weapon/ktrobopartsweaponmetadata.txt";
		} else if(category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_PULSE_ID) {
			return "resrc/ktrobo/info/metadata/weapon/ktrobopartsweaponmetadata.txt";
		} else if(category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_RIFLE_ID) {
			return "resrc/ktrobo/info/metadata/weapon/ktrobopartsweaponmetadata.txt";
		} else if(category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_ROCKET_ID) {
			return "resrc/ktrobo/info/metadata/weapon/ktrobopartsweaponmetadata.txt";
		} else if(category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_SHOTGUN_ID) {
			return "resrc/ktrobo/info/metadata/weapon/ktrobopartsweaponmetadata.txt";
		} else if(category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_SNIPER_ID) {
			return "resrc/ktrobo/info/metadata/weapon/ktrobopartsweaponmetadata.txt";
		}
	}

	if (KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_ID == category_id) {
		if (KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_CHAIN_ID == category2_id) {
			return "resrc/ktrobo/info/metadata/kata/ktrobopartskatametadata.txt";
		} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_GRENEDE_ID == category2_id) {
			return "resrc/ktrobo/info/metadata/kata/ktrobopartskatabakuhatumetadata.txt";
		} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_HANABI_ID == category2_id) {
			return "resrc/ktrobo/info/metadata/kata/ktrobopartskatametadata.txt";
		} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_LASER_ID == category2_id ) {
			return "resrc/ktrobo/info/metadata/kata/ktrobopartskatametadata.txt";
		} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_MISSILE_ID == category2_id) {
			return "resrc/ktrobo/info/metadata/kata/ktrobopartskatamissilemetadata.txt";
		} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_PLAZUMA_ID == category2_id) {
			return "resrc/ktrobo/info/metadata/kata/ktrobopartskatabakuhatumetadata.txt";
		} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_PULSE_ID == category2_id) {
			return "resrc/ktrobo/info/metadata/kata/ktrobopartskatabakuhatumetadata.txt";
		} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_ROCKET_ID == category2_id) {
			return "resrc/ktrobo/info/metadata/kata/ktrobopartskatametadata.txt";
		}
	}

	if (KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_ID == category_id) {
		if (KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_CHAIN_ID == category2_id) {
			return "resrc/ktrobo/info/metadata/kata/ktrobopartskatametadata.txt";
		} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_GRENEDE_ID == category2_id) {
			return "resrc/ktrobo/info/metadata/kata/ktrobopartskatabakuhatumetadata.txt";
		} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_HANABI_ID == category2_id) {
			return "resrc/ktrobo/info/metadata/kata/ktrobopartskatametadata.txt";
		} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_LASER_ID == category2_id ) {
			return "resrc/ktrobo/info/metadata/kata/ktrobopartskatametadata.txt";
		} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_MISSILE_ID == category2_id) {
			return "resrc/ktrobo/info/metadata/kata/ktrobopartskatamissilemetadata.txt";
		} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_PLAZUMA_ID == category2_id) {
			return "resrc/ktrobo/info/metadata/kata/ktrobopartskatabakuhatumetadata.txt";
		} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_PULSE_ID == category2_id) {
			return "resrc/ktrobo/info/metadata/kata/ktrobopartskatabakuhatumetadata.txt";
		} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_ROCKET_ID == category2_id) {
			return "resrc/ktrobo/info/metadata/kata/ktrobopartskatametadata.txt";
		}
	}


	throw new GameError(KTROBO::FATAL_ERROR, "no cid");
}

KoumokuList_Parts* Gamen_GARAGE::getPartsList(int cid, int cid2) {
	if (KTROBO_GAMEN_GARAGE_KOUMOKU_HEAD_ID == cid ) {
		return parts_head_list;
	}
	if (KTROBO_GAMEN_GARAGE_KOUMOKU_BOOSTER_ID == cid) {
		return parts_booster_list;
	}
	if (KTROBO_GAMEN_GARAGE_KOUMOKU_CORE_ID == cid) {
		return parts_core_list;
	}
	if (KTROBO_GAMEN_GARAGE_KOUMOKU_ENGINE_ID == cid) {
		return parts_engine_list;
	}

	if (KTROBO_GAMEN_GARAGE_KOUMOKU_FCS_ID == cid) {
		return parts_fcs_list;
	}
	if (KTROBO_GAMEN_GARAGE_KOUMOKU_HEAD_ID == cid) {
		return parts_fcs_list;
	}
	if (KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_ID == cid) {
		return getPartsList(&parts_inside_lists, cid,cid2);
	}
	if (KTROBO_GAMEN_GARAGE_KOUMOKU_LEG_ID == cid) {
		return getPartsList(&parts_leg_lists,cid,cid2);
	}
	if (KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_ID == cid) {
		return getPartsList(&parts_lkataweapon_lists, cid, cid2);
	}
	if(KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_ID == cid) {
		return getPartsList(&parts_rkataweapon_lists, cid, cid2);
	}
	if (KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_ID == cid) {
		return getPartsList(&parts_larmweapon_lists, cid, cid2);
	}
	if (KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_ID == cid) {
		return getPartsList(&parts_rarmweapon_lists, cid, cid2);
	}
	throw new GameError(KTROBO::FATAL_ERROR, "no parts list");
	return 0;
}

KoumokuList_Parts* Gamen_GARAGE::getPartsList(vector<KoumokuList_Parts*>* lists, int category_id, int category_id2) {
	vector<KoumokuList_Parts*>::iterator it;
	it = lists->begin();
	while(it != lists->end()) {
		KoumokuList_Parts* kk = *it;
		if (kk->isCID(category_id,category_id2)) {
			return kk;
		}

		it++;
	}
	return 0;
}


char* KoumokuList_Parts::getPartsFilenameFromCID() {
	if (KTROBO_GAMEN_GARAGE_KOUMOKU_HEAD_ID == category_id) {
	return "resrc/ktrobo/info/ktroboheadparts.txt";
	}
	if (KTROBO_GAMEN_GARAGE_KOUMOKU_BOOSTER_ID == category_id) {
		return "resrc/ktrobo/info/ktroboboosterparts.txt";
	}
	if (KTROBO_GAMEN_GARAGE_KOUMOKU_CORE_ID == category_id) {
		return "resrc/ktrobo/info/ktrobocoreparts.txt";
	}

	if (KTROBO_GAMEN_GARAGE_KOUMOKU_ENGINE_ID == category_id) {
		return "resrc/ktrobo/info/ktroboengineparts.txt";
	}

	if (KTROBO_GAMEN_GARAGE_KOUMOKU_FCS_ID == category_id) {
		return "resrc/ktrobo/info/ktrobofcsparts.txt";
	}

	if (KTROBO_GAMEN_GARAGE_KOUMOKU_ARM_ID == category_id) {
		return "resrc/ktrobo/info/ktroboarmparts.txt";
	}

	if (KTROBO_GAMEN_GARAGE_KOUMOKU_LEG_ID == category_id) {
		if (KTROBO_GAMEN_GARAGE_KOUMOKU_LEG_4K_ID == category2_id) {
			return "resrc/ktrobo/info/leg/ktroboleg4k.txt";
		} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_LEG_J2_ID == category2_id) {
			return "resrc/ktrobo/info/leg/ktrobolegj2.txt";
		} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_LEG_K2_ID == category2_id) {
			return "resrc/ktrobo/info/leg/ktrobolegk2.txt";
		} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_LEG_R_ID == category2_id) {
			return "resrc/ktrobo/info/leg/ktrobolegr.txt";
		} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_LEG_T2_ID == category2_id) {
			return "resrc/ktrobo/info/leg/ktrobolegt2.txt";
		} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_LEG_TK_ID == category2_id) {
			return "resrc/ktrobo/info/leg/ktrobolegtk.txt";
		}
	}


if (KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_ID == category_id) {
				
		if (category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_DECOY_ID) {
			return "resrc/ktrobo/info/inside/ktroboinsidedecoy.txt";
		} else 	if (category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_ADDMISSILE_ID) {
			return "resrc/ktrobo/info/inside/ktroboinsideaddmissile.txt";
		} else 	if (category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_APKAIFUKU_ID) {
			return "resrc/ktrobo/info/inside/ktroboinsideapkaifuku.txt";
		} else 	if (category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_SUBCOMPUTER_ID) {
			return "resrc/ktrobo/info/inside/ktroboinsidesubcomputer.txt";
		} else 	if (category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_ENERGYZOUFUKU_ID) {
			return "resrc/ktrobo/info/inside/ktroboinsideenergyzoufuku.txt";
		} else 	if (category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_ROCKET_ID) {
			return "resrc/ktrobo/info/inside/ktroboinsiderocket.txt";
		} else 	if (category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_JAMMERROCKET_ID) {
			return "resrc/ktrobo/info/inside/ktroboinsidejyamarocket.txt";
		} else 	if (category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_KIRAI_ID) {
			return "resrc/ktrobo/info/inside/ktroboinsidekirai.txt";
		} else 	if (category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_BIT_ID) {
			return "resrc/ktrobo/info/inside/ktroboinsidebit.txt";
		} else 	if (category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_STEALTH_ID) {
			return "resrc/ktrobo/info/inside/ktroboinsidestealth.txt";
		}










	}





if (KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_ID == category_id) {
		if (category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_BAZOOKA_ID) {
			return "resrc/ktrobo/info/lweapon/ktrobolarmweaponbazooka.txt";
		} else if(category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_BLADE_ID) {
			return "resrc/ktrobo/info/lweapon/ktrobolarmweaponblade.txt";
		} else if(category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_EBLADE_ID) {
			return "resrc/ktrobo/info/lweapon/ktrobolarmweaponenergyblade.txt";
		} else if(category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_ERIFLE_ID) {
			return "resrc/ktrobo/info/lweapon/ktrobolarmweaponenergyrifle.txt";
		} else if(category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_GRENEDE_ID) {
			return "resrc/ktrobo/info/lweapon/ktrobolarmweapongrenede.txt";
		} else if(category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_HANABI_ID) {
			return "resrc/ktrobo/info/lweapon/ktrobolarmweaponhanabigun.txt";
		} else if(category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_HANDGUN_ID) {
			return "resrc/ktrobo/info/lweapon/ktrobolarmweaponhandgun.txt";
		} else if(category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_MACHINE_ID) {
			return "resrc/ktrobo/info/lweapon/ktrobolarmweaponmachinegun.txt";
		} else if(category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_MISSILE_ID) {
			return "resrc/ktrobo/info/lweapon/ktrobolarmweaponmissile.txt";
		} else if(category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_PILE_ID) {
			return "resrc/ktrobo/info/lweapon/ktrobolarmweaponpile.txt";
		} else if(category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_PLAZUMA_ID) {
			return "resrc/ktrobo/info/lweapon/ktrobolarmweaponplazumagun.txt";
		} else if(category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_PULSE_ID) {
			return "resrc/ktrobo/info/lweapon/ktrobolarmweaponpulsegun.txt";
		} else if(category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_RIFLE_ID) {
			return "resrc/ktrobo/info/lweapon/ktrobolarmweaponrifle.txt";
		} else if(category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_ROCKET_ID) {
			return "resrc/ktrobo/info/lweapon/ktrobolarmweaponrocket.txt";
		} else if(category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_SHOTGUN_ID) {
			return "resrc/ktrobo/info/lweapon/ktrobolarmweaponshotgun.txt";
		} else if(category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_SNIPER_ID) {
			return "resrc/ktrobo/info/lweapon/ktrobolarmweaponsniperrifle.txt";
		}
	}




if (KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_ID == category_id) {
		if (category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_BAZOOKA_ID) {
			return "resrc/ktrobo/info/rweapon/ktroborarmweaponbazooka.txt";
		} else if(category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_BLADE_ID) {
			return "resrc/ktrobo/info/rweapon/ktroborarmweaponblade.txt";
		} else if(category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_EBLADE_ID) {
			return "resrc/ktrobo/info/rweapon/ktroborarmweaponenergyblade.txt";
		} else if(category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_ERIFLE_ID) {
			return "resrc/ktrobo/info/rweapon/ktroborarmweaponenergyrifle.txt";
		} else if(category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_GRENEDE_ID) {
			return "resrc/ktrobo/info/rweapon/ktroborarmweapongrenede.txt";
		} else if(category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_HANABI_ID) {
			return "resrc/ktrobo/info/rweapon/ktroborarmweaponhanabigun.txt";
		} else if(category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_HANDGUN_ID) {
			return "resrc/ktrobo/info/rweapon/ktroborarmweaponhandgun.txt";
		} else if(category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_MACHINE_ID) {
			return "resrc/ktrobo/info/rweapon/ktroborarmweaponmachinegun.txt";
		} else if(category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_MISSILE_ID) {
			return "resrc/ktrobo/info/rweapon/ktroborarmweaponmissile.txt";
		} else if(category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_PILE_ID) {
			return "resrc/ktrobo/info/rweapon/ktroborarmweaponpile.txt";
		} else if(category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_PLAZUMA_ID) {
			return "resrc/ktrobo/info/rweapon/ktroborarmweaponplazumagun.txt";
		} else if(category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_PULSE_ID) {
			return "resrc/ktrobo/info/rweapon/ktroborarmweaponpulsegun.txt";
		} else if(category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_RIFLE_ID) {
			return "resrc/ktrobo/info/rweapon/ktroborarmweaponrifle.txt";
		} else if(category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_ROCKET_ID) {
			return "resrc/ktrobo/info/rweapon/ktroborarmweaponrocket.txt";
		} else if(category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_SHOTGUN_ID) {
			return "resrc/ktrobo/info/rweapon/ktroborarmweaponshotgun.txt";
		} else if(category2_id == KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_SNIPER_ID) {
			return "resrc/ktrobo/info/rweapon/ktroborarmweaponsniperrifle.txt";
		}
	}




	if (KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_ID == category_id) {
		if (KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_CHAIN_ID == category2_id) {
			return "resrc/ktrobo/info/rkata/ktroborkatachaingun.txt";
		} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_GRENEDE_ID == category2_id) {
			return "resrc/ktrobo/info/rkata/ktroborkatagrenede.txt";
		} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_HANABI_ID == category2_id) {
			return "resrc/ktrobo/info/rkata/ktroborkatahanabi.txt";
		} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_LASER_ID == category2_id ) {
			return "resrc/ktrobo/info/rkata/ktroborkataraser.txt";
		} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_MISSILE_ID == category2_id) {
			return "resrc/ktrobo/info/rkata/ktroborkatamissile.txt";
		} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_PLAZUMA_ID == category2_id) {
			return "resrc/ktrobo/info/rkata/ktroborkataplazuma.txt";
		} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_PULSE_ID == category2_id) {
			return "resrc/ktrobo/info/rkata/ktroborkatapulse.txt";
		} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_ROCKET_ID == category2_id) {
			return "resrc/ktrobo/info/rkata/ktroborkatarocket.txt";
		}
	}

	
	if (KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_ID == category_id) {
		if (KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_CHAIN_ID == category2_id) {
			return "resrc/ktrobo/info/lkata/ktrobolkatachaingun.txt";
		} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_GRENEDE_ID == category2_id) {
			return "resrc/ktrobo/info/lkata/ktrobolkatagrenede.txt";
		} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_HANABI_ID == category2_id) {
			return "resrc/ktrobo/info/lkata/ktrobolkatahanabi.txt";
		} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_LASER_ID == category2_id ) {
			return "resrc/ktrobo/info/lkata/ktrobolkataraser.txt";
		} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_MISSILE_ID == category2_id) {
			return "resrc/ktrobo/info/lkata/ktrobolkatamissile.txt";
		} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_PLAZUMA_ID == category2_id) {
			return "resrc/ktrobo/info/lkata/ktrobolkataplazuma.txt";
		} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_PULSE_ID == category2_id) {
			return "resrc/ktrobo/info/lkata/ktrobolkatapulse.txt";
		} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_ROCKET_ID == category2_id) {
			return "resrc/ktrobo/info/lkata/ktrobolkatarocket.txt";
		}
	}



	throw new GameError(KTROBO::FATAL_ERROR, "no cid");
}
void Gamen_GARAGE::clickedKoumokuListPartsdayo(KoumokuList_Parts* kp, RoboParts* kk) {


	
	MyTokenAnalyzer ma;
	ma.load(kp->getMetaPartsFilenameFromCID());
	RoboDataMetaData* booster_md = new RoboDataMetaData();
	RoboMetaDataPart rmdp;
	rmdp.clear();
	int dnum = ma.GetIntToken();
	for (int i=0;i<dnum;i++) {
		rmdp.clear();
		rmdp.readline(&ma);
		booster_md->setData(rmdp.data_name,rmdp.data_name2,rmdp.data_type,rmdp.data_sentence,rmdp.data_compare);
	}

	ma.deletedayo();
	kp->setMetaData(booster_md);

	if (this->canBeEmpty(kp->getCategoryID())) {
		Koumoku_Parts* emptyk = new Koumoku_Parts(KTROBO_GAMEN_GARAGE_KOUMOKU_PARTS_ID, new RoboPartsEmpty());
		emptyk->setEnabled(false);
		kp->setKoumoku(emptyk);
	}

	ma.load(kp->getPartsFilenameFromCID());//"resrc/ktrobo/info/ktroboboosterparts.txt");
	while(!ma.enddayo()) {

		if (strcmp(ma.Toke(), "{")==0) {
			RoboParts* booster = kk->myNew();//new RoboBooster();
			Koumoku_Parts* kpp = new Koumoku_Parts(KTROBO_GAMEN_GARAGE_KOUMOKU_PARTS_ID,booster);
			kpp->setEnabled(false);
					
			kp->setKoumoku(kpp);
					
			try {
				booster->loadData(&ma,booster_md);
						
					
			} catch (GameError* err) {
		
				//	MessageBoxA(g->getHWND(), err->getMessage(), err->getErrorCodeString(err->getErrorCode()), MB_OK);
				ma.deletedayo();
				throw err;
			}
		}
		ma.GetToken();
	}		
	ma.deletedayo();
		
	kp->setEnable(false);
	kp->setVisible(t,false);
	int si = kp->getKoumokuSize();
	kp->setSize(t,500,0,460,21*si);

			
	kp->InitKoumokus(t,loader);



	kp->setHyouji3Mode(false);
	kp->clickedDown();
	kp->clickedUp();
	kp->setEnable(false);
	kp->setVisible(t,false);
}

KoumokuList* Gamen_GARAGE::getCategoryList(int category_id) {
	if (category_id == KTROBO_GAMEN_GARAGE_KOUMOKU_LEG_ID) {
		return parts_leg_category_list;
	} else if(category_id == KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_ID) {
		return parts_inside_category_list;
	} else if(category_id == KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_ID) {
		return parts_rweapon_category_list;
	} else if(category_id == KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_ID) {
		return parts_lweapon_category_list;
	} else if(category_id == KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_ID) {
		return parts_rkata_category_list;
	} else if(category_id == KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_ID) {
		return parts_lkata_category_list;
	}
	throw new GameError(KTROBO::FATAL_ERROR, "no category");
}


RoboParts* Gamen_GARAGE::getRoboPartsFromCID(int category_id) {
	if (category_id == KTROBO_GAMEN_GARAGE_KOUMOKU_ARM_ID) {
			return robo->arm;
	}
	if (category_id == KTROBO_GAMEN_GARAGE_KOUMOKU_BOOSTER_ID) {
		return robo->booster;
	}
	if (category_id == KTROBO_GAMEN_GARAGE_KOUMOKU_CORE_ID) {
		return robo->body;
	}
	if (category_id == KTROBO_GAMEN_GARAGE_KOUMOKU_ENGINE_ID) {
		return robo->engine;
	}
	if (category_id == KTROBO_GAMEN_GARAGE_KOUMOKU_FCS_ID) {
		return robo->fcs;
	}
	if (category_id == KTROBO_GAMEN_GARAGE_KOUMOKU_HEAD_ID) {
		return robo->head;
	}
	if (category_id == KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_ID) {
		return robo->iweapon;
	}
	if (category_id == KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_ID) {
		return robo->raweapon;
	}
	if (category_id == KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_ID) {
		return robo->laweapon;
	}
	if (category_id == KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_ID) {
		return robo->rsweapon;
	}
	if (category_id == KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_ID) {
		return robo->lsweapon;
	}
	if (category_id == KTROBO_GAMEN_GARAGE_KOUMOKU_LEG_ID) {
		return robo->leg;
	}
	return 0;
}

bool Gamen_GARAGE::canBeEmpty(int category_id) {
	if (category_id == KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_ID) {
		return true;
	}
	if( category_id == KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_ID) {
		return true;
	}
	if (category_id == KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_ID) {
		return true;
	}
	if (category_id == KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_ID) {
		return true;
	}
	if (category_id == KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_ID) {
		return true;
	}
	return false;
}

void Gamen_GARAGE::clickedShoriCParts(vector<KoumokuList_Parts*>* kp, RoboParts* kkp, int category_id, int category_id2, char* namedayo) {

	KoumokuList_Parts* kk = this->getPartsList(kp, category_id, category_id2);
		if (!kk) {
			kk = new KoumokuList_Parts(this,category_id, category_id2,t);
			kk->setname(namedayo);
			this->clickedKoumokuListPartsdayo(kk, kkp);
			kp->push_back(kk);
		}
		t->setRenderTexIsRender(clearpartsgamen, true);
		kk->setHyouji3Mode(true);
		t->setRenderTexIsRender(clearcommentgamen, true);
		kk->setVisible(kk->t, true);
		kk->setEnable(true);
		kk->clickedDown();
		kk->clickedUp();
		KoumokuList* kkkk = getCategoryList(category_id);
		kkkk->setEnable(false);
		
		temp_focused_list = kk;
		RoboParts* temp_parts=0;
		temp_parts = this->getRoboPartsFromCID(category_id);
		gamenpart_roboparam.setFocused(this,true);
		gamenpart_roboparam.setRoboParts(kk->getFocusedParts(), kk->getMetaData(),temp_parts, category_id); 
	
}

void Gamen_GARAGE::clickedShori(int id) {

	if (KTROBO_GAMEN_GARAGE_KOUMOKU_ARM_ID == id) {

		if (!parts_arm_list) {

			parts_arm_list = new KoumokuList_Parts(this,KTROBO_GAMEN_GARAGE_KOUMOKU_ARM_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_NONE,t);
			parts_arm_list->setname("アームパーツ");
			RoboArm booster = RoboArm();
			this->clickedKoumokuListPartsdayo(parts_arm_list, &booster);
		}

		t->setRenderTexIsRender(clearpartsgamen, true);
		parts_arm_list->setHyouji3Mode(true);
		t->setRenderTexIsRender(clearcommentgamen, true);



		parts_arm_list->setVisible(parts_arm_list->t, true);
		parts_arm_list->setEnable(true);
		parts_arm_list->clickedDown();
		parts_arm_list->clickedUp();
		parts_category_list->setEnable(false);
		temp_focused_list = parts_arm_list;
			gamenpart_roboparam.setFocused(this,true);
		gamenpart_roboparam.setRoboParts(parts_arm_list->getFocusedParts(), parts_arm_list->getMetaData(), robo->arm, id); 
	

	} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_BOOSTER_ID == id) {
		if (!parts_booster_list) {

			parts_booster_list = new KoumokuList_Parts(this,KTROBO_GAMEN_GARAGE_KOUMOKU_BOOSTER_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_NONE,t);
			parts_booster_list->setname("ブースタパーツ");
			RoboBooster booster = RoboBooster();
			this->clickedKoumokuListPartsdayo(parts_booster_list, &booster);
		}

		t->setRenderTexIsRender(clearpartsgamen, true);
		parts_booster_list->setHyouji3Mode(true);
		t->setRenderTexIsRender(clearcommentgamen, true);



		parts_booster_list->setVisible(parts_booster_list->t, true);
		parts_booster_list->setEnable(true);
		parts_booster_list->clickedDown();
		parts_booster_list->clickedUp();
		parts_category_list->setEnable(false);
		temp_focused_list = parts_booster_list;

			gamenpart_roboparam.setFocused(this,true);
		gamenpart_roboparam.setRoboParts(parts_booster_list->getFocusedParts(), parts_booster_list->getMetaData(), robo->booster,id); 
		//gamenpart_roboparam.setFocused(this,true);


	} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_CORE_ID == id) {
		if (!parts_core_list) {

			parts_core_list = new KoumokuList_Parts(this,KTROBO_GAMEN_GARAGE_KOUMOKU_CORE_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_NONE,t);
			parts_core_list->setname("コアパーツ");
			RoboBody booster = RoboBody();
			this->clickedKoumokuListPartsdayo(parts_core_list, &booster);
		}

		t->setRenderTexIsRender(clearpartsgamen, true);
		parts_core_list->setHyouji3Mode(true);
		t->setRenderTexIsRender(clearcommentgamen, true);



		parts_core_list->setVisible(parts_core_list->t, true);
		parts_core_list->setEnable(true);
		parts_core_list->clickedDown();
		parts_core_list->clickedUp();
		parts_category_list->setEnable(false);
		temp_focused_list = parts_core_list;
			gamenpart_roboparam.setFocused(this,true);
		gamenpart_roboparam.setRoboParts(parts_core_list->getFocusedParts(), parts_core_list->getMetaData(), robo->body,id); 
//		gamenpart_roboparam.setFocused(this,true);


	} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_ENGINE_ID == id) {
		if (!parts_engine_list) {

			parts_engine_list = new KoumokuList_Parts(this,KTROBO_GAMEN_GARAGE_KOUMOKU_ENGINE_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_NONE,t);
			parts_engine_list->setname("エンジンパーツ");
			RoboEngine booster = RoboEngine();
			this->clickedKoumokuListPartsdayo(parts_engine_list, &booster);
		}

		t->setRenderTexIsRender(clearpartsgamen, true);
		parts_engine_list->setHyouji3Mode(true);
		t->setRenderTexIsRender(clearcommentgamen, true);



		parts_engine_list->setVisible(parts_engine_list->t, true);
		parts_engine_list->setEnable(true);
		parts_engine_list->clickedDown();
		parts_engine_list->clickedUp();
		parts_category_list->setEnable(false);
		temp_focused_list = parts_engine_list;
		gamenpart_roboparam.setFocused(this,true);
		gamenpart_roboparam.setRoboParts(parts_engine_list->getFocusedParts(), parts_engine_list->getMetaData(), robo->engine,id); 
//		gamenpart_roboparam.setFocused(this,true);


	} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_FCS_ID == id) {

		if (!parts_fcs_list) {

			parts_fcs_list = new KoumokuList_Parts(this,KTROBO_GAMEN_GARAGE_KOUMOKU_FCS_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_NONE,t);
			parts_fcs_list->setname("fcsパーツ");
			RoboFCS booster = RoboFCS();
			this->clickedKoumokuListPartsdayo(parts_fcs_list, &booster);
		}

		t->setRenderTexIsRender(clearpartsgamen, true);
		parts_fcs_list->setHyouji3Mode(true);
		t->setRenderTexIsRender(clearcommentgamen, true);



		parts_fcs_list->setVisible(parts_fcs_list->t, true);
		parts_fcs_list->setEnable(true);
		parts_fcs_list->clickedDown();
		parts_fcs_list->clickedUp();
		parts_category_list->setEnable(false);
		temp_focused_list = parts_fcs_list;
	gamenpart_roboparam.setFocused(this,true);
		gamenpart_roboparam.setRoboParts(parts_fcs_list->getFocusedParts(), parts_fcs_list->getMetaData(), robo->fcs,id); 
//		gamenpart_roboparam.setFocused(this,true);




	} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_HEAD_ID == id) {
		if (!parts_head_list) {
			parts_head_list = new KoumokuList_Parts(this,KTROBO_GAMEN_GARAGE_KOUMOKU_HEAD_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_NONE,t);
			parts_head_list->setname("ヘッドパーツ");
			RoboHead head = RoboHead();
			this->clickedKoumokuListPartsdayo(parts_head_list, &head);
			
		}
		t->setRenderTexIsRender(clearpartsgamen, true);
		parts_head_list->setHyouji3Mode(true);
		t->setRenderTexIsRender(clearcommentgamen, true);



		parts_head_list->setVisible(parts_head_list->t, true);
		parts_head_list->setEnable(true);
		parts_head_list->clickedDown();
		parts_head_list->clickedUp();
		parts_category_list->setEnable(false);
		temp_focused_list = parts_head_list;
	
			gamenpart_roboparam.setFocused(this,true);
		gamenpart_roboparam.setRoboParts(parts_head_list->getFocusedParts(), parts_head_list->getMetaData(), robo->head,id); 
//		gamenpart_roboparam.setFocused(this,true);


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
	} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_LEG_4K_ID == id) {
		RoboLeg rk = RoboLeg();
		clickedShoriCParts(&parts_leg_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_LEG_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_LEG_4K_ID,"レッグ_4脚パーツ");	
	} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_LEG_J2_ID == id) {
		RoboLeg rk = RoboLeg();
		clickedShoriCParts(&parts_leg_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_LEG_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_LEG_J2_ID,"レッグ_重量2脚パーツ");
	} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_LEG_K2_ID == id) {
		RoboLeg rk = RoboLeg();
		clickedShoriCParts(&parts_leg_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_LEG_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_LEG_K2_ID,"レッグ_軽量2脚パーツ");
	} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_LEG_T2_ID == id) {
		RoboLeg rk = RoboLeg();
		clickedShoriCParts(&parts_leg_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_LEG_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_LEG_T2_ID,"レッグ_中量2脚パーツ");
	} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_LEG_TK_ID == id) {
		RoboLeg rk = RoboLeg();
		clickedShoriCParts(&parts_leg_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_LEG_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_LEG_TK_ID,"レッグ_タンクパーツ");
	} else if(KTROBO_GAMEN_GARAGE_KOUMOKU_LEG_R_ID == id) {
		RoboLeg rk = RoboLeg();
		clickedShoriCParts(&parts_leg_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_LEG_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_LEG_R_ID,"レッグ_逆間接パーツ");
	} else if (id == KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_DECOY_ID) {
		InsideWeapon rk = InsideWeapon();
		clickedShoriCParts(&parts_inside_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_DECOY_ID,"インサイド_デコイパーツ");
	} else 	if (id == KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_ADDMISSILE_ID) {
		InsideWeapon rk = InsideWeapon();
		clickedShoriCParts(&parts_inside_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_ADDMISSILE_ID,"インサイド_増弾ミサイルパーツ");
	} else 	if (id == KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_APKAIFUKU_ID) {
		InsideWeapon rk = InsideWeapon();
		clickedShoriCParts(&parts_inside_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_APKAIFUKU_ID,"インサイド_AP回復パーツ");
	} else 	if (id == KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_SUBCOMPUTER_ID) {
		InsideWeapon rk = InsideWeapon();
		clickedShoriCParts(&parts_inside_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_SUBCOMPUTER_ID,"インサイド_サブコンピュータパーツ");

	} else 	if (id == KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_ENERGYZOUFUKU_ID) {
		InsideWeapon rk = InsideWeapon();
		clickedShoriCParts(&parts_inside_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_ENERGYZOUFUKU_ID,"インサイド_エネルギー増幅パーツ");

	} else 	if (id == KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_ROCKET_ID) {
		InsideWeapon rk = InsideWeapon();
		clickedShoriCParts(&parts_inside_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_ROCKET_ID,"インサイド_ロケットパーツ");

	} else 	if (id == KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_JAMMERROCKET_ID) {
		InsideWeapon rk = InsideWeapon();
		clickedShoriCParts(&parts_inside_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_JAMMERROCKET_ID,"インサイド_ジャマーロケットパーツ");

	} else 	if (id == KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_KIRAI_ID) {
		InsideWeapon rk = InsideWeapon();
		clickedShoriCParts(&parts_inside_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_KIRAI_ID,"インサイド_機雷パーツ");

	} else 	if (id == KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_BIT_ID) {
		InsideWeapon rk = InsideWeapon();
		clickedShoriCParts(&parts_inside_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_BIT_ID,"インサイド_ビットパーツ");

	} else 	if (id == KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_STEALTH_ID) {
		InsideWeapon rk = InsideWeapon();
		clickedShoriCParts(&parts_inside_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_STEALTH_ID,"インサイド_ステルスパーツ");

	} else if( id == KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_BAZOOKA_ID) {
		LArmWeapon rk = LArmWeapon();
		clickedShoriCParts(&parts_larmweapon_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_BAZOOKA_ID,"左手_バズーカパーツ");
	} else if( id == KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_BLADE_ID) {
			LArmWeapon rk = LArmWeapon();
		clickedShoriCParts(&parts_larmweapon_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_BLADE_ID,"左手_ブレードパーツ");

	} else if( id == KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_EBLADE_ID) {
			LArmWeapon rk = LArmWeapon();
		clickedShoriCParts(&parts_larmweapon_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_EBLADE_ID,"左手_Eブレードパーツ");

	} else if( id == KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_ERIFLE_ID) {
			LArmWeapon rk = LArmWeapon();
		clickedShoriCParts(&parts_larmweapon_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_ERIFLE_ID,"左手_Eライフルパーツ");

	} else if( id == KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_GRENEDE_ID) {
			LArmWeapon rk = LArmWeapon();
		clickedShoriCParts(&parts_larmweapon_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_GRENEDE_ID,"左手_グレネードパーツ");

	} else if( id == KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_HANABI_ID) {
			LArmWeapon rk = LArmWeapon();
		clickedShoriCParts(&parts_larmweapon_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_HANABI_ID,"左手_ハナビパーツ");

	} else if( id == KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_HANDGUN_ID) {
			LArmWeapon rk = LArmWeapon();
		clickedShoriCParts(&parts_larmweapon_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_HANDGUN_ID,"左手_ハンドガンパーツ");

	} else if( id == KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_MACHINE_ID) {
			LArmWeapon rk = LArmWeapon();
		clickedShoriCParts(&parts_larmweapon_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_MACHINE_ID,"左手_マシンガンパーツ");

	} else if( id == KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_MISSILE_ID) {
			LArmWeapon rk = LArmWeapon();
		clickedShoriCParts(&parts_larmweapon_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_MISSILE_ID,"左手_ミサイルパーツ");

	} else if( id == KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_PILE_ID) {
			LArmWeapon rk = LArmWeapon();
		clickedShoriCParts(&parts_larmweapon_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_PILE_ID,"左手_パイルパーツ");

	} else if( id == KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_PLAZUMA_ID) {
			LArmWeapon rk = LArmWeapon();
		clickedShoriCParts(&parts_larmweapon_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_PLAZUMA_ID,"左手_プラズマガンパーツ");

	} else if( id == KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_PULSE_ID) {
			LArmWeapon rk = LArmWeapon();
		clickedShoriCParts(&parts_larmweapon_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_PULSE_ID,"左手_パルスガンパーツ");

	} else if( id == KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_RIFLE_ID) {
			LArmWeapon rk = LArmWeapon();
		clickedShoriCParts(&parts_larmweapon_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_RIFLE_ID,"左手_ライフルパーツ");

	} else if( id == KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_ROCKET_ID) {
			LArmWeapon rk = LArmWeapon();
		clickedShoriCParts(&parts_larmweapon_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_ROCKET_ID,"左手_ロケットパーツ");

	} else if( id == KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_SHOTGUN_ID) {
			LArmWeapon rk = LArmWeapon();
		clickedShoriCParts(&parts_larmweapon_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_SHOTGUN_ID,"左手_ショットガンパーツ");

	} else if( id == KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_SNIPER_ID) {
			LArmWeapon rk = LArmWeapon();
		clickedShoriCParts(&parts_larmweapon_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_SNIPER_ID,"左手_スナイパーライフルパーツ");

	} else if( id == KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_BAZOOKA_ID) {
		RArmWeapon rk = RArmWeapon();
		clickedShoriCParts(&parts_rarmweapon_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_BAZOOKA_ID,"右手_バズーカパーツ");
	} else if( id == KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_BLADE_ID) {
			RArmWeapon rk = RArmWeapon();
		clickedShoriCParts(&parts_rarmweapon_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_BLADE_ID,"右手_ブレードパーツ");

	} else if( id == KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_EBLADE_ID) {
			RArmWeapon rk = RArmWeapon();
		clickedShoriCParts(&parts_rarmweapon_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_EBLADE_ID,"右手_Eブレードパーツ");

	} else if( id == KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_ERIFLE_ID) {
			RArmWeapon rk = RArmWeapon();
		clickedShoriCParts(&parts_rarmweapon_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_ERIFLE_ID,"右手_Eライフルパーツ");

	} else if( id == KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_GRENEDE_ID) {
			RArmWeapon rk = RArmWeapon();
		clickedShoriCParts(&parts_rarmweapon_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_GRENEDE_ID,"右手_グレネードパーツ");

	} else if( id == KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_HANABI_ID) {
			RArmWeapon rk = RArmWeapon();
		clickedShoriCParts(&parts_rarmweapon_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_HANABI_ID,"右手_ハナビパーツ");

	} else if( id == KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_HANDGUN_ID) {
			RArmWeapon rk = RArmWeapon();
		clickedShoriCParts(&parts_rarmweapon_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_HANDGUN_ID,"右手_ハンドガンパーツ");

	} else if( id == KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_MACHINE_ID) {
			RArmWeapon rk = RArmWeapon();
		clickedShoriCParts(&parts_rarmweapon_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_MACHINE_ID,"右手_マシンガンパーツ");

	} else if( id == KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_MISSILE_ID) {
			RArmWeapon rk = RArmWeapon();
		clickedShoriCParts(&parts_rarmweapon_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_MISSILE_ID,"右手_ミサイルパーツ");

	} else if( id == KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_PILE_ID) {
			RArmWeapon rk = RArmWeapon();
		clickedShoriCParts(&parts_rarmweapon_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_PILE_ID,"右手_パイルパーツ");

	} else if( id == KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_PLAZUMA_ID) {
			RArmWeapon rk = RArmWeapon();
		clickedShoriCParts(&parts_rarmweapon_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_PLAZUMA_ID,"右手_プラズマガンパーツ");

	} else if( id == KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_PULSE_ID) {
			RArmWeapon rk = RArmWeapon();
		clickedShoriCParts(&parts_rarmweapon_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_PULSE_ID,"右手_パルスガンパーツ");

	} else if( id == KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_RIFLE_ID) {
			RArmWeapon rk = RArmWeapon();
		clickedShoriCParts(&parts_rarmweapon_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_RIFLE_ID,"右手_ライフルパーツ");

	} else if( id == KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_ROCKET_ID) {
			RArmWeapon rk = RArmWeapon();
		clickedShoriCParts(&parts_rarmweapon_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_ROCKET_ID,"右手_ロケットパーツ");

	} else if( id == KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_SHOTGUN_ID) {
			RArmWeapon rk = RArmWeapon();
		clickedShoriCParts(&parts_rarmweapon_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_SHOTGUN_ID,"右手_ショットガンパーツ");

	} else if( id == KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_SNIPER_ID) {
			RArmWeapon rk = RArmWeapon();
		clickedShoriCParts(&parts_rarmweapon_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_SNIPER_ID,"右手_スナイパーライフルパーツ");

	} else if(id == KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_CHAIN_ID) {
		RShoulderWeapon rk = RShoulderWeapon();
		clickedShoriCParts(&parts_rkataweapon_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_CHAIN_ID, "右肩_チェインパーツ");
	} else if(id == KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_GRENEDE_ID) {
		RShoulderWeapon rk = RShoulderWeapon();
		clickedShoriCParts(&parts_rkataweapon_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_GRENEDE_ID, "右肩_グレネードパーツ");
	} else if(id == KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_HANABI_ID) {
		RShoulderWeapon rk = RShoulderWeapon();
		clickedShoriCParts(&parts_rkataweapon_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_HANABI_ID, "右肩_ハナビパーツ");
	} else if(id == KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_PULSE_ID) {
		RShoulderWeapon rk = RShoulderWeapon();
		clickedShoriCParts(&parts_rkataweapon_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_PULSE_ID, "右肩_パルスパーツ");
	} else if(id == KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_LASER_ID) {
		RShoulderWeapon rk = RShoulderWeapon();
		clickedShoriCParts(&parts_rkataweapon_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_LASER_ID, "右肩_レーザーパーツ");
	} else if(id == KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_ROCKET_ID) {
		RShoulderWeapon rk = RShoulderWeapon();
		clickedShoriCParts(&parts_rkataweapon_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_ROCKET_ID, "右肩_ロケットパーツ");
	} else if(id == KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_PLAZUMA_ID) {
		RShoulderWeapon rk = RShoulderWeapon();
		clickedShoriCParts(&parts_rkataweapon_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_PLAZUMA_ID, "右肩_プラズマパーツ");
	} else if(id == KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_MISSILE_ID) {
		RShoulderWeapon rk = RShoulderWeapon();
		clickedShoriCParts(&parts_rkataweapon_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_MISSILE_ID, "右肩_ミサイルパーツ");
	}
	 else if(id == KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_CHAIN_ID) {
		LShoulderWeapon rk = LShoulderWeapon();
		clickedShoriCParts(&parts_lkataweapon_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_CHAIN_ID, "左肩_チェインパーツ");
	} else if(id == KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_GRENEDE_ID) {
		LShoulderWeapon rk = LShoulderWeapon();
		clickedShoriCParts(&parts_lkataweapon_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_GRENEDE_ID, "左肩_グレネードパーツ");
	} else if(id == KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_HANABI_ID) {
		LShoulderWeapon rk = LShoulderWeapon();
		clickedShoriCParts(&parts_lkataweapon_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_HANABI_ID, "左肩_ハナビパーツ");
	} else if(id == KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_PULSE_ID) {
		LShoulderWeapon rk = LShoulderWeapon();
		clickedShoriCParts(&parts_lkataweapon_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_PULSE_ID, "左肩_パルスパーツ");
	} else if(id == KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_LASER_ID) {
		LShoulderWeapon rk = LShoulderWeapon();
		clickedShoriCParts(&parts_lkataweapon_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_LASER_ID, "左肩_レーザーパーツ");
	} else if(id == KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_ROCKET_ID) {
		LShoulderWeapon rk = LShoulderWeapon();
		clickedShoriCParts(&parts_lkataweapon_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_ROCKET_ID, "左肩_ロケットパーツ");
	} else if(id == KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_PLAZUMA_ID) {
		LShoulderWeapon rk = LShoulderWeapon();
		clickedShoriCParts(&parts_lkataweapon_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_PLAZUMA_ID, "左肩_プラズマパーツ");
	} else if(id == KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_MISSILE_ID) {
		LShoulderWeapon rk = LShoulderWeapon();
		clickedShoriCParts(&parts_lkataweapon_lists, &rk, KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_ID, KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_MISSILE_ID, "左肩_ミサイルパーツ");
	}
	
	
	/*
	else if(id == KTROBO_GAMEN_GARAGE_KOUMOKU_PARTS_ID) {
		// パーツのYESNOEQUIPにいく
		gamenpart_yesnoequip.setMaeFocusedList(temp_focused_list);
		gamenpart_yesnoequip.setRoboParts(gamenpart_roboparam.getRoboParts());
		gamenpart_yesnoequip.setFocused(this, true);
		this->temp_focused_list = gamenpart_yesnoequip.kl;
	}
	*/
}

void Koumoku_Parts_Category::_exedayo(Gamen* gamen, GamenPart* gp, KoumokuList* kl) {

	gamen->clickedShori(this->getID());

}

void Gamen_GARAGE::Init(Graphics* g, AtariHantei* hantei, Texture* t, Texture* t2, MyTextureLoader* loader) {

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

	clearpartsgamen = t->getRenderTex(tex_id, 0xDDEEFFDD, 500,100,330,330,0,0,128,128);
	t->setRenderTexIsRender(clearpartsgamen,false);

	clearcommentgamen = t->getRenderTex(tex_id, 0xDDEEFFDD, 0,770,880,20,0,0,128,128);
	t->setRenderTexIsRender(clearcommentgamen,false);


	this->t = t;
	this->t2 = t2;
	this->loader = loader;
	robo->roboparam.Init(robo);

	gamenpart_roboparam.Init(robo,g,t,loader);

	gamenpart_yesnoequip.setGG(this);
	gamenpart_yesnoequip.setGraphics(g);
	gamenpart_yesnoequip.setTexture(t2);
	gamenpart_yesnoequip.setRobo(robo);
	gamenpart_yesnoequip.setTextureLoader(loader);
	gamenpart_yesnoequip.makeKL();
	gamenpart_yesnoequip.setFocused(this, false);
	

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
		t->lightdeleteAllRenderText();
	}

	if (t2) {
		t2->lightdeleteAllRenderTex();
		t2->lightdeleteAllBillBoard();
		t2->lightdeleteAllRenderText();
	}

	if (parts_head_list) {
		delete parts_head_list;
		parts_head_list = 0;
	}
	if (parts_booster_list) {
		delete parts_booster_list;
		parts_booster_list = 0;
	}
	if (parts_core_list) {
		delete parts_core_list;
		parts_core_list = 0;
	}
	if (parts_engine_list) {
		delete parts_engine_list;
		parts_engine_list = 0;
	}
	if (parts_arm_list) {
		delete parts_arm_list;
		parts_arm_list = 0;
	}
	if (parts_fcs_list) {
		delete parts_fcs_list;
		parts_fcs_list = 0;
	}

	vector<KoumokuList_Parts*>::iterator it;

	it = parts_leg_lists.begin();
	while(it != parts_leg_lists.end()) {
		KoumokuList_Parts* kk = *it;
		delete kk;
		kk = 0;
		it++;
	}
	parts_leg_lists.clear();

	it = parts_inside_lists.begin();
	while(it != parts_inside_lists.end()) {
		KoumokuList_Parts* kk = *it;
		delete kk;
		kk = 0;
		it++;
	}
	parts_inside_lists.clear();

	it = parts_larmweapon_lists.begin();
	while(it != parts_larmweapon_lists.end()) {
		KoumokuList_Parts* kk = *it;
		delete kk;
		kk = 0;
		it++;
	}
	parts_larmweapon_lists.clear();

	it = parts_rarmweapon_lists.begin();
	while(it != parts_rarmweapon_lists.end()) {
		KoumokuList_Parts* kk = *it;
		delete kk;
		kk = 0;
		it++;
	}
	parts_rarmweapon_lists.clear();

	it = parts_lkataweapon_lists.begin();
	while(it != parts_lkataweapon_lists.end()) {
		KoumokuList_Parts* kk = *it;
		delete kk;
		kk = 0;
		it++;
	}
	parts_lkataweapon_lists.clear();

	it = parts_rkataweapon_lists.begin();
	while(it != parts_rkataweapon_lists.end()) {
		KoumokuList_Parts* kk = *it;
		delete kk;
		kk = 0;
		it++;
	}
	parts_rkataweapon_lists.clear();

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
		parts_head_list->t->setRenderTextIsRender(parts_head_list->parts_setumei_text,false);
	}

	if (parts_booster_list) {
		parts_booster_list->setEnable(false);
		parts_booster_list->setVisible(parts_booster_list->t, false);
		parts_booster_list->t->setRenderTextIsRender(parts_booster_list->parts_setumei_text,false);
	}

	if (parts_core_list) {
		parts_core_list->setEnable(false);
		parts_core_list->setVisible(parts_core_list->t, false);
		parts_core_list->t->setRenderTextIsRender(parts_core_list->parts_setumei_text,false);
	}

	if (parts_engine_list) {
		parts_engine_list->setEnable(false);
		parts_engine_list->setVisible(parts_engine_list->t, false);
		parts_engine_list->t->setRenderTextIsRender(parts_engine_list->parts_setumei_text,false);
	}


	if (parts_booster_list) {
		parts_booster_list->setEnable(false);
		parts_booster_list->setVisible(parts_booster_list->t, false);
		parts_booster_list->t->setRenderTextIsRender(parts_booster_list->parts_setumei_text,false);
	}

	if (parts_fcs_list) {
		parts_fcs_list->setEnable(false);
		parts_fcs_list->setVisible(parts_fcs_list->t,false);
		parts_fcs_list->t->setRenderTextIsRender(parts_fcs_list->parts_setumei_text,false);
	}

	if (parts_arm_list) {
		parts_arm_list->setEnable(false);
		parts_arm_list->setVisible(parts_arm_list->t,false);
		parts_arm_list->t->setRenderTextIsRender(parts_arm_list->parts_setumei_text,false);
	}

	vector<KoumokuList_Parts*>::iterator it;

	it = parts_leg_lists.begin();
	while(it != parts_leg_lists.end()) {
		KoumokuList_Parts* kk = *it;
		kk->setEnable(false);
		kk->setVisible(kk->t,false);
		kk->t->setRenderTextIsRender(kk->parts_setumei_text,false);
		it++;
	}

	it = parts_inside_lists.begin();
	while(it != parts_inside_lists.end()) {
		KoumokuList_Parts* kk = *it;
		kk->setEnable(false);
		kk->setVisible(kk->t,false);
		kk->t->setRenderTextIsRender(kk->parts_setumei_text,false);
		it++;
	}

	it = parts_rkataweapon_lists.begin();
	while(it != parts_rkataweapon_lists.end()) {
		KoumokuList_Parts* kk = *it;
		kk->setEnable(false);
		kk->setVisible(kk->t,false);
		kk->t->setRenderTextIsRender(kk->parts_setumei_text,false);
		it++;
	}

	it = parts_lkataweapon_lists.begin();
	while(it != parts_lkataweapon_lists.end()) {
		KoumokuList_Parts* kk = *it;
		kk->setEnable(false);
		kk->setVisible(kk->t,false);
		kk->t->setRenderTextIsRender(kk->parts_setumei_text,false);
		it++;
	}

	it = parts_rarmweapon_lists.begin();
	while(it != parts_rarmweapon_lists.end()) {
		KoumokuList_Parts* kk = *it;
		kk->setEnable(false);
		kk->setVisible(kk->t,false);
		kk->t->setRenderTextIsRender(kk->parts_setumei_text,false);
		it++;
	}

	it = parts_larmweapon_lists.begin();
	while(it != parts_larmweapon_lists.end()) {
		KoumokuList_Parts* kk = *it;
		kk->setEnable(false);
		kk->setVisible(kk->t,false);
		kk->t->setRenderTextIsRender(kk->parts_setumei_text,false);
		it++;
	}

	this->gamenpart_roboparam.setFocused(this,false);
	this->gamenpart_yesnoequip.setFocused(this, false);

	t->setRenderTexIsRender(clearpartsgamen, false);
	t->setRenderTexIsRender(clearcommentgamen,false);

}


bool Gamen_GARAGE::handleMessage(int msg, void* data, DWORD time) {


	MYINPUTMESSAGESTRUCT* input = (MYINPUTMESSAGESTRUCT*) data;
	float x = input->getMOUSESTATE()->mouse_x;
	float y = input->getMOUSESTATE()->mouse_y;


	CS::instance()->enter(CS_MESSAGE_CS, "enter");
	if (msg == KTROBO_INPUT_MESSAGE_ID_MOUSERAWSTATE) {
		if (input->getMOUSESTATE()->mouse_l_button_pressed) {

			try {
			temp_focused_list->clicked(this,NULL,x,y);
			} catch(GameError* err) {
				CS::instance()->leave(CS_MESSAGE_CS, "leave");
				delete err;
				return true;
			}
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
			

			
			try {
			temp_focused_list->clickedEnter(this,NULL);
			} catch(GameError* err) {
				CS::instance()->leave(CS_MESSAGE_CS, "leave");
				delete err;
				return true;
			}


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
	return false;


}

void KoumokuList_Parts::clickedDown() {


	KoumokuList::clickedDown();
	g->gamenpart_roboparam.setRoboParts(this->focused_parts, metadata, g->getRoboPartsFromCID(this->category_id),category_id);
	//g->gamenpart_roboparam.setFocused(g,true);
}

void KoumokuList_Parts::clickedUp() {

	KoumokuList::clickedUp();
	g->gamenpart_roboparam.setRoboParts(this->focused_parts, metadata, g->getRoboPartsFromCID(this->category_id),category_id);
	//g->gamenpart_roboparam.setRoboParts(this->focused_parts, metadata, g->getRoboPartsFromCID(this->category_id));

}

void GamenGARAGE_partRoboParam::calcRoboParam(RoboParts* new_parts, RoboParts* parts_compare, int category_id) {
	if (!this->gamen_robo_params.size()) {
		int test = robo->roboparam.getKoumokuSize();
		
		while(test > 0) {
				GamenGARAGE_partsParam * pp = new GamenGARAGE_partsParam(this->t);
				gamen_robo_params.push_back(pp);
				test--;
		}
	}

	// まずは全てノーユーズにする
	int gsize = gamen_robo_params.size();
	for (int i=0;i<gsize;i++) {
		gamen_robo_params[i]->setNoUse();
	}

	int temp = 0;
	MYRECT pla;
	pla.left = 550;
	pla.right = 850;
	pla.top = 550;
	pla.bottom = 550 + GamenGARAGE_partsParam::string_haba_place_height;

	for (int i=0;i<gsize;i++) {
		if (gamen_robo_params[temp]->setChara2(&pla,robo,new_parts,category_id, i,parts_compare)) {
			temp++;
			pla.top += GamenGARAGE_partsParam::string_haba_place_height;
			pla.bottom += GamenGARAGE_partsParam::string_haba_place_height;
		}
	}

}
void GamenGARAGE_partsParam::setChara2dayo(Robo* robo, char* dataname, char* dataname_for_disp, int suuti, RoboParts* new_parts, RoboParts* compare_parts) {

	// AP
		int parts_ap = 0;
		int parts_ap_new = 0;
		if (new_parts) {
			parts_ap_new = new_parts->data->getData(dataname)->int_data;
		}
		if (compare_parts) {
			parts_ap = compare_parts->data->getData(dataname)->int_data;
		}
		int sa = parts_ap_new - parts_ap;
		int suuti_chara_haba = 4;
		char test[128];
		char test2[128];
		for(int i=0;i<128;i++) {
			test[i] = 0;
			test2[i] = 0;
		}
		this->getSuutiHikakuChara(sa,test);
		this->getSuutiChara(suuti+sa, test2);
		suuti_chara_haba = strlen(test2) * string_haba;

		t->setRenderTextChangeText(this->robo_parts_param_name, dataname_for_disp);
		t->setRenderTextChangeText(this->robo_parts_param_suuti, test2);
		t->setRenderTextChangeText(this->robo_parts_param_suuti_hikaku, test);

		// place にあわせて場所を移動させる
		t->setRenderTextPos(this->robo_parts_param_name, place.left,place.top);
		t->setRenderTextPos(this->robo_parts_param_suuti, place.right - suuti_chara_haba,place.top);
		t->setRenderTextPos(this->robo_parts_param_suuti_hikaku, place.right + string_haba, place.top);
		this->is_hikaku = true;
		this->is_use = true;
}

bool GamenGARAGE_partsParam::setChara2(MYRECT* placedayo, Robo* robo, RoboParts* new_parts, int category_id, int index,RoboParts* compare_parts) {

	if ((robo->roboparam.getKoumokuSize() <= index) || index < 0) {
		this->is_use = false;
		return false;
	}
	place = *placedayo;

	if (index ==0) {
		this->setChara2dayo(robo,"AP","AP",robo->roboparam.getMaxAP(),new_parts, compare_parts);
	} else if(index ==1) {
		this->setChara2dayo(robo,"LOADMAX","積載量",robo->roboparam.getCanWeight(),new_parts, compare_parts);
	} else if(index==2) {
		this->setChara2dayo(robo, "WEIGHT", "総重量", robo->roboparam.getAllWeight(), new_parts,compare_parts);
	} else if(index==3) {
		this->setChara2dayo(robo, "DEF", "実弾防御力", robo->roboparam.getDef(), new_parts,compare_parts);
	} else if(index==4) {
		this->setChara2dayo(robo, "EDEF", "エネルギー防御力", robo->roboparam.getEDef(), new_parts,compare_parts);
	} else if(index==5) {
		this->setChara2dayo(robo, "EPOWER", "エネルギー出力", robo->roboparam.getEnergyShuturyoku(), new_parts, compare_parts);
	} else if(index == 6) {
		this->setChara2dayo(robo,"EPOOL", "エネルギー容量", robo->roboparam.getEnergyPool(),new_parts, compare_parts);
	} else if(index==7) {
		this->setChara2dayo(robo, "EDRAIN", "エネルギー余剰", robo->roboparam.getAmariEnergy(), compare_parts, new_parts);
	}
	this->is_use = true;
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
void GamenGARAGE_partRoboParam::setRoboParts(RoboParts* robo_parts, RoboDataMetaData* metadata, RoboParts* robo_parts_compare, int category_id) {
	this->robo_parts = robo_parts;
	this->metadata = metadata;
	this->robo_parts_compare = robo_parts_compare;
	if (metadata) {
		vector<RoboMetaDataPart*>* vec = metadata->getMetaDatas();
		int size = vec->size();
		int gsize = gamen_parts_params.size();
		if (size > gsize) {
			int test = size- gsize;
			while(test > 0) {
				GamenGARAGE_partsParam * pp = new GamenGARAGE_partsParam(this->t);
				gamen_parts_params.push_back(pp);
				test--;
			}
		}
		// まずは全てノーユーズにする
		gsize = gamen_parts_params.size();
		for (int i=0;i<gsize;i++) {
			gamen_parts_params[i]->setNoUse();
		}

		int temp = 0;
		MYRECT pla;
		pla.left = 550;
		pla.right = 800;
		pla.top = 100;
		pla.bottom = 100 + GamenGARAGE_partsParam::string_haba_place_height;

		for (int i=0;i<size;i++) {
			if (gamen_parts_params[temp]->setChara(&pla,metadata,robo_parts,i,robo_parts_compare)) {
				temp++;
				pla.top += GamenGARAGE_partsParam::string_haba_place_height;
				pla.bottom += GamenGARAGE_partsParam::string_haba_place_height;
			}
		}
	}

	calcRoboParam(robo_parts,robo_parts_compare, category_id);
}
bool GamenGARAGE_partsParam::setChara(MYRECT* placedayo, RoboDataMetaData* metadata, RoboParts* parts, int index, RoboParts* compare_parts) {
	if (!parts || !metadata) return false;
	vector<RoboMetaDataPart*>* vec = metadata->getMetaDatas();
	if ((vec->size() <= index) || index < 0) return false;
	if (strcmp(((*vec)[index])->data_sentence, "YES_SENTENCE")==0) {
		place = *placedayo;
		int suuti_chara_haba=4;

		t->setRenderTextChangeText(this->robo_parts_param_name,((*vec)[index])->data_name2);
		// 次に数値もしくは文字のセット
		if (strcmp(((*vec)[index])->data_type, "INT")==0) {
			if (strcmp(((*vec)[index])->data_compare, "YES_SENTENCE")==0) {
				char test[128];
				char test2[128];
				for (int i=0;i<128;i++) {
					test[i] = 0;
					test2[i] = 0;
				}
				int suuti = parts->data->getData(((*vec)[index])->data_name)->int_data;
				
				int suuti_hikaku = 0;
				if (compare_parts) {
					suuti_hikaku = compare_parts->data->getData(((*vec)[index])->data_name)->int_data;
				}
				suuti_hikaku = suuti - suuti_hikaku;
				this->getSuutiChara(suuti, test);
				this->getSuutiHikakuChara(suuti_hikaku, test2);
				suuti_chara_haba = strlen(test)*GamenGARAGE_partsParam::string_haba;
				t->setRenderTextChangeText(this->robo_parts_param_suuti, test);
				t->setRenderTextChangeText(this->robo_parts_param_suuti_hikaku, test2);
				is_hikaku = true;
			} else {
				char test[128];
				for (int i=0;i<128;i++) {
					test[i] = 0;
				}
				int suuti = parts->data->getData(((*vec)[index])->data_name)->int_data;
				this->getSuutiChara(suuti, test);
				suuti_chara_haba = strlen(test)*GamenGARAGE_partsParam::string_haba;
				t->setRenderTextChangeText(this->robo_parts_param_suuti,test);
				is_hikaku = false;
			}

		} else if(strcmp(((*vec)[index])->data_type, "STRING")==0) {

			t->setRenderTextChangeText(this->robo_parts_param_suuti, parts->data->getData(((*vec)[index]->data_name))->string_data);
			suuti_chara_haba =  strlen(parts->data->getData(((*vec)[index]->data_name))->string_data) * GamenGARAGE_partsParam::string_haba;
			is_hikaku = false;

		}
		// place にあわせて場所を移動させる
		t->setRenderTextPos(this->robo_parts_param_name, place.left,place.top);
		t->setRenderTextPos(this->robo_parts_param_suuti, place.right - suuti_chara_haba,place.top);
		t->setRenderTextPos(this->robo_parts_param_suuti_hikaku, place.right + string_haba, place.top);
		this->is_use = true;
		return true;

	} else {
		this->is_use = false;
		return false;
	}
}

void GamenGARAGE_partsParam::getSuutiChara(int suuti, char* chara) {
	int temp = 0;
	if (suuti < 0) {
		chara[temp] = '-';
		temp++;
		suuti *= -1;
	}
	int suut = suuti;
	int keta = 0;
	while(suut > 0) {
		suut = suut / 10;
		keta++;
	}
	if (keta ==0) {
		chara[temp] = '0';
		temp++;
		chara[temp] = '\0';
		return;
	}
	suut = suuti;
	int tt = keta;
	while(keta >0) {
		int amari = suut % 10;
		if (amari == 0) {
			chara[temp+keta-1] = '0';
		} else if(amari == 1) {
			chara[temp+keta-1] = '1';
		} else if(amari == 2) {
			chara[temp+keta-1] = '2';
		} else if(amari == 3) {
			chara[temp+keta-1] = '3';
		} else if(amari == 4) {
			chara[temp+keta-1] = '4';
		} else if(amari == 5) {
			chara[temp+keta-1] = '5';
		} else if(amari == 6) {
			chara[temp+keta-1] = '6';
		} else if(amari == 7) {
			chara[temp+keta-1] = '7';
		} else if(amari == 8) {
			chara[temp+keta-1] = '8';
		} else if(amari == 9) {
			chara[temp+keta-1] = '9';
		}
		suut = suut /10;
		keta--;
	}
	chara[temp+tt] = 0;
	return;


}


void GamenGARAGE_partsParam::getSuutiHikakuChara(int suuti, char* chara) {

	if (suuti >0) {
		chara[0] = '+';
		getSuutiChara(suuti, &chara[1]);
	} else {
		getSuutiChara(suuti,chara);
	}
}





void GamenGARAGE_partRoboParam::Init(Robo* robo, Graphics* g, Texture* t, MyTextureLoader* loader) {

	this->robo = robo;
	this->t = t;
	int offset = 80;
	int head_ = t->getRenderText("ヘッド",0,300+offset,20,80,22);
	int body_ = t->getRenderText("コア", 0,322+offset,20,80,22);
	int arm_ = t->getRenderText("アーム", 0,344+offset,20,80,22);
	int leg_ = t->getRenderText("レッグ", 0,366+offset,20,80,22);
	int engine_ = t->getRenderText("エンジン", 0,388+offset, 20,120,22);
	int booster_ = t->getRenderText("ブースタ", 0,410+offset, 20, 120, 22);
	int fcs_ = t->getRenderText("FCS", 0,432+offset,20,120,22);
	int inside_ = t->getRenderText("インサイド", 0,454+offset,20,150,22);
	int raweapon_ = t->getRenderText("右手武器", 0,476+offset,20,150,22);
	int laweapon_ = t->getRenderText("左手武器", 0, 498+offset, 20, 150, 22);
	int rsweapon_ = t->getRenderText("右肩武器", 0, 520+offset,20,150,22);
	int lsweapon_ = t->getRenderText("左肩武器", 0, 542+offset,20,150,22);


	robo_parts_name_category.push_back(head_);
	robo_parts_name_category.push_back(body_);
	robo_parts_name_category.push_back(arm_);
	robo_parts_name_category.push_back(leg_);
	robo_parts_name_category.push_back(engine_);
	robo_parts_name_category.push_back(booster_);
	robo_parts_name_category.push_back(fcs_);
	robo_parts_name_category.push_back(inside_);
	robo_parts_name_category.push_back(raweapon_);
	robo_parts_name_category.push_back(laweapon_);
	robo_parts_name_category.push_back(rsweapon_);
	robo_parts_name_category.push_back(lsweapon_);
	t->setRenderTextIsRender(head_,true);
	t->setRenderTextIsRender(body_,true);
	t->setRenderTextIsRender(arm_,true);
	t->setRenderTextIsRender(leg_,true);
	t->setRenderTextIsRender(engine_,true);
	t->setRenderTextIsRender(booster_,true);
	t->setRenderTextIsRender(fcs_,true);
	t->setRenderTextIsRender(inside_,true);
	t->setRenderTextIsRender(raweapon_,true);
	t->setRenderTextIsRender(laweapon_,true);
	t->setRenderTextIsRender(rsweapon_,true);
	t->setRenderTextIsRender(lsweapon_,true);
	hanneiPartsName();
	
}

void GamenGARAGE_partRoboParam::hanneiPartsName() {
	int offset = 80;
	if (robo && (robo_parts_name.size() == 0)) {
		char* headd = robo->roboparam.getNameOfHead();
		int len = strlen(headd);
		int headdd = t->getRenderText(headd,500-len*20,300+offset,20,20*len+20,22);
		robo_parts_name.push_back(headdd);
		t->setRenderTextIsRender(headdd,true);
		char* bodyd = robo->roboparam.getNameOfBody();
		len = strlen(bodyd);
		int bodydd = t->getRenderText(bodyd, 500-len*20,322+offset,20,20*len+20,22);
		t->setRenderTextIsRender(bodydd,true);
		robo_parts_name.push_back(bodydd);
		char* named = robo->roboparam.getNameOfArm();
		len = strlen(named);
		int namedd = t->getRenderText(named, 500-len*20,344+offset,20,20*len+20,22);
		t->setRenderTextIsRender(namedd,true);
		robo_parts_name.push_back(namedd);
		named = robo->roboparam.getNameOfLeg();
		len = strlen(named);
		namedd = t->getRenderText(named, 500-len*20,366+offset,20,20*len+20,22);
		t->setRenderTextIsRender(namedd,true);
			robo_parts_name.push_back(namedd);
		named = robo->roboparam.getNameOfEngine();
		len = strlen(named);
		namedd = t->getRenderText(named, 500-len*20,388+offset,20,20*len+20,22);
		t->setRenderTextIsRender(namedd,true);
			robo_parts_name.push_back(namedd);
		named = robo->roboparam.getNameOfBooster();
		len = strlen(named);
		namedd = t->getRenderText(named, 500-len*20,410+offset,20,20*len+20,22);
		t->setRenderTextIsRender(namedd,true);
			robo_parts_name.push_back(namedd);
		named = robo->roboparam.getNameOfFCS();
		len = strlen(named);
		namedd = t->getRenderText(named, 500-len*20,432+offset,20,20*len+20,22);
		t->setRenderTextIsRender(namedd,true);
			robo_parts_name.push_back(namedd);
		named = robo->roboparam.getNameOfInside();
		len = strlen(named);
		namedd = t->getRenderText(named, 500-len*20,454+offset,20,20*len+20,22);
		t->setRenderTextIsRender(namedd,true);
			robo_parts_name.push_back(namedd);
		named = robo->roboparam.getNameOfRArmWeapon();
		len = strlen(named);
		namedd = t->getRenderText(named, 500-len*20,476+offset,20,20*len+20,22);
		t->setRenderTextIsRender(namedd,true);
			robo_parts_name.push_back(namedd);
		named = robo->roboparam.getNameOfLArmWeapon();
		len = strlen(named);
		namedd = t->getRenderText(named, 500-len*20,498+offset,20,20*len+20,22);
		t->setRenderTextIsRender(namedd,true);
			robo_parts_name.push_back(namedd);
		named = robo->roboparam.getNameOfRShoulderWeapon();
		len = strlen(named);
		namedd = t->getRenderText(named, 500-len*20,520+offset,20,20*len+20,22);
		t->setRenderTextIsRender(namedd,true);
			robo_parts_name.push_back(namedd);
		named = robo->roboparam.getNameOfLShoulderWeapon();
		len = strlen(named);
		namedd = t->getRenderText(named, 500-len*20,544+offset,20,20*len+20,22);
		t->setRenderTextIsRender(namedd,true);
			robo_parts_name.push_back(namedd);




	} else if(robo && (robo_parts_name.size()  >=12)) {



		char* headd = robo->roboparam.getNameOfHead();
		int len = strlen(headd);
		int headdd = robo_parts_name[0];
		t->setRenderTextPos(headdd,500-len*20,300+offset);
		//robo_parts_name.push_back(headdd);
		//t->setRenderTextIsRender(headdd,true);
		t->setRenderTextChangeText(headdd,headd);

		char* bodyd = robo->roboparam.getNameOfBody();
		len = strlen(bodyd);
		int bodydd = robo_parts_name[1];
			
		t->setRenderTextPos(bodydd, 500-len*20,322+offset);
		t->setRenderTextChangeText(bodydd,bodyd);

		char* named = robo->roboparam.getNameOfArm();
		len = strlen(named);
		int namedd = robo_parts_name[2];
		t->setRenderTextPos(namedd, 500-len*20,344+offset);
		t->setRenderTextChangeText(namedd,named);

		named = robo->roboparam.getNameOfLeg();
		len = strlen(named);
		namedd = robo_parts_name[3];
		t->setRenderTextPos(namedd, 500-len*20,366+offset);
		t->setRenderTextChangeText(namedd,named);

		named = robo->roboparam.getNameOfEngine();
		len = strlen(named);
		namedd = robo_parts_name[4];
		t->setRenderTextPos(namedd, 500-len*20,388+offset);
		t->setRenderTextChangeText(namedd,named);

		named = robo->roboparam.getNameOfBooster();
		len = strlen(named);
		namedd = robo_parts_name[5];
		t->setRenderTextPos(namedd, 500-len*20,410+offset);
		t->setRenderTextChangeText(namedd,named);

		named = robo->roboparam.getNameOfFCS();
		len = strlen(named);
		namedd = robo_parts_name[6];
		t->setRenderTextPos(namedd, 500-len*20,432+offset);
		t->setRenderTextChangeText(namedd,named);

		named = robo->roboparam.getNameOfInside();
		len = strlen(named);
		namedd = robo_parts_name[7];
		t->setRenderTextPos(namedd, 500-len*20,454+offset);
		t->setRenderTextChangeText(namedd,named);

		named = robo->roboparam.getNameOfRArmWeapon();
		len = strlen(named);
		namedd = robo_parts_name[8];
		t->setRenderTextPos(namedd, 500-len*20,476+offset);
		t->setRenderTextChangeText(namedd,named);

		named = robo->roboparam.getNameOfLArmWeapon();
		len = strlen(named);
		namedd = robo_parts_name[9];
		t->setRenderTextPos(namedd, 500-len*20,498+offset);
		t->setRenderTextChangeText(namedd,named);

		named = robo->roboparam.getNameOfRShoulderWeapon();
		len = strlen(named);
		namedd = robo_parts_name[10];
		t->setRenderTextPos(namedd, 500-len*20,520+offset);
		t->setRenderTextChangeText(namedd,named);

		named = robo->roboparam.getNameOfLShoulderWeapon();
		len = strlen(named);
		namedd = robo_parts_name[11];
		t->setRenderTextPos(namedd, 500-len*20,544+offset);
		t->setRenderTextChangeText(namedd,named);









	}
}
void GamenGARAGE_partsParam::setVisible(bool te) {

	if (!is_use) {
		t->setRenderTextIsRender(robo_parts_param_name,false);
		t->setRenderTextIsRender(this->robo_parts_param_suuti,false);
		t->setRenderTextIsRender(this->robo_parts_param_suuti_hikaku,false);
	} else {

		t->setRenderTextIsRender(this->robo_parts_param_name,te);
		t->setRenderTextIsRender(this->robo_parts_param_suuti,te);
		t->setRenderTextIsRender(this->robo_parts_param_suuti_hikaku,te);
		if (!is_hikaku) {
			t->setRenderTextIsRender(this->robo_parts_param_suuti_hikaku, false);
		}
	}
}


void GamenGARAGE_partRoboParam::setFocused(Gamen* g, bool te) {

	if (te) {
		robo->roboparam.calcParam();
		int gsize = gamen_parts_params.size();
		for (int i=0;i<gsize;i++) {
			gamen_parts_params[i]->setVisible(te);
		}
		gsize = gamen_robo_params.size();
		for (int i=0;i<gsize;i++) {
			gamen_robo_params[i]->setVisible(te);
		}

	} else {
		int gsize = gamen_parts_params.size();
		for (int i=0;i<gsize;i++) {
			gamen_parts_params[i]->setVisible(te);
		}
			gsize = gamen_robo_params.size();
		for (int i=0;i<gsize;i++) {
			gamen_robo_params[i]->setVisible(te);
		}
	}


}
void GamenGARAGE_partRoboParam::byouga(Graphics* g, GUI* gui, float dsecond, int stamp) {





}


void GamenGARAGE_partYESNOEQUIP::setTexture(Texture* tex) {
	this->t = tex;
	question_str = t->getRenderText("sample", 200,400,20,400,22);
	
	
}

void GamenGARAGE_partYESNOEQUIP::setTextureLoader(MyTextureLoader* loader) {
	this->tex_loader = loader;
}

void GamenGARAGE_partYESNOEQUIP::makeKL() {

	this->kl = new KoumokuList(t);
	kl->setname("YESNOEQUIP");
	KoumokuYESEQUIP* y = new KoumokuYESEQUIP(t, this->gg,this);
	y->setEnabled(true);
	kl->setKoumoku(y);
	KoumokuNOEQUIP* n = new KoumokuNOEQUIP(t, gg, this);
	n->setEnabled(true);
	kl->setKoumoku(n);

	kl->setEnable(true);
	kl->setVisible(t, true);
	kl->setSize(t,870,450,120,60);
	y->init();
	n->init();
	kl->setHyouji3Mode(false);
	kl->clickedDown();
	kl->clickedUp();
	kl->setEnable(false);
	kl->setVisible(t,false);

	int gamen_id = t->getTexture(KTROBO_GUI_PNG);
	clear_gamen = t->getRenderTex(gamen_id, 0xFFFFFFFF, 150,360,700,100,0,0,128,128); 
	t->setRenderTexIsRender(clear_gamen,true);

}

void KoumokuYESEQUIP::byouga(Graphics* g, GUI* gui, float dsecond, int stamp,bool has_clicked) {


if (!getEnabled() && !has_clicked) return;
	t->setRenderTexColor(gui_koumoku_name_bg_id, 0xFFFFFFFFF);
	

}// focused_koumoku のkoumokuでもbyougaは呼ばれる

void KoumokuYESEQUIP::focusedByouga(Graphics* g, GUI* gui, float dsecond, int stamp, bool has_clicked) {
if (!getEnabled() && !has_clicked) return;
	static float unko=0;
	unko += dsecond;
	if (unko >= 1000) {
		unko =0;
	}

	int stt = (int) (unko) % 130;
	//int stt = stamp % 13;
	unsigned int color = ((((0xFFFF0000 / 130 * stt) & 0x77770000) + 0x88880000) & 0xFFFF0000) + 0x0000FFFF;
	t->setRenderTexColor(gui_koumoku_name_bg_id,color);


}

void KoumokuYESEQUIP::_exedayo(Gamen* gamen, GamenPart* gp, KoumokuList* kl) {
	KoumokuList_Parts* kkl = (KoumokuList_Parts*)g_part->mae_focused_list;

	this->equipParts();
	g->gamenpart_roboparam.setRoboParts(g_part->getRoboParts(), kkl->getMetaData(),g_part->getRoboParts(),g_part->getCategoryID());
	g->gamenpart_roboparam.hanneiPartsName();
	// 前の画面に戻る
	this->g_part->setFocused(gamen,false);
	this->g->temp_focused_list = g_part->mae_focused_list;
	g->temp_focused_list->setEnable(true);
	g->temp_focused_list->clickedUp();
	g->temp_focused_list->clickedDown();
}


void KoumokuYESEQUIP::equipParts() {
	if (this->g_part) {
		g_part->equip();
	
	}
}

void GamenGARAGE_partYESNOEQUIP::equip() {

	CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	try {
	if (!is_empty_mode) {
	if (robo) {
		if (robo_parts) {
			robo_parts->equipRobo(robo,g,tex_loader);
			//robo->roboparam.calcParam();
		}
	}
	} else {
		RoboPartsEmpty emp = RoboPartsEmpty();
		if (category_id == KTROBO_GAMEN_GARAGE_KOUMOKU_INSIDE_ID) {
			emp.emptyInsideWeapon(robo,true);
		} else if(category_id == KTROBO_GAMEN_GARAGE_KOUMOKU_RWEAPON_ID) {
			emp.emptyRArmWeapon(robo,true);
		} else if(category_id == KTROBO_GAMEN_GARAGE_KOUMOKU_LWEAPON_ID) {
			emp.emptyLArmWeapon(robo,true);
		} else if(category_id == KTROBO_GAMEN_GARAGE_KOUMOKU_RKATA_ID) {
			emp.emptyRShoulderWeapon(robo,true);
		} else if(category_id == KTROBO_GAMEN_GARAGE_KOUMOKU_LKATA_ID) {
			emp.emptyLShoulderWeapon(robo,true);
		}
		emp.Release();
	}
	if (robo) {
		robo->roboparam.calcParam();
		//gg->gamenpart_roboparam.hanneiPartsName();
		//gg->gamenpart_roboparam.calcRoboParam(robo_parts,robo_parts, category_id);

	}
	} catch (GameError* error) {

		delete error;
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "leave");
}

void GamenGARAGE_partYESNOEQUIP::setGraphics(Graphics* g) {
	this->g = g;
}

void GamenGARAGE_partYESNOEQUIP::setRobo(Robo* robo) {
	this->robo = robo;
}

void GamenGARAGE_partYESNOEQUIP::setRoboParts(RoboParts* robo_parts, int category_id) {
	this->robo_parts = robo_parts;
	this->category_id = category_id;
	if (robo_parts && !robo_parts->isEmpty()) {
		char str[256];
		for (int i=0;i<256;i++) {
			str[i] = 0;
		}	
		strcpy_s(str,256,robo_parts->data->getData("name")->string_data);
		strcat_s(str,256-32," を装備しますか?");
		t->setRenderTextChangeText(question_str, str);
		setEmptyMode(false);
	}

	if (robo_parts && robo_parts->isEmpty()) {
		t->setRenderTextChangeText(question_str, "パーツをはずしますか?");
		setEmptyMode(true);
	}


}

void GamenGARAGE_partYESNOEQUIP::setFocused(Gamen* g, bool te) {
	if (te) {
		t->setRenderTexIsRender(clear_gamen,true);
		t->setRenderTextIsRender(question_str,true);
		kl->setEnable(true);
		kl->setVisible(t,true);
		
	} else {
		t->setRenderTexIsRender(clear_gamen,false);
		t->setRenderTextIsRender(question_str,false);
		kl->setVisible(t,false);
		kl->setEnable(false);
	}
}

void GamenGARAGE_partYESNOEQUIP::byouga(Graphics* g, GUI* gui, float dsecond, int stamp) {
	if (this->kl) {
		kl->byouga(g, gui, dsecond, stamp);
	}
}

void GamenGARAGE_partYESNOEQUIP::setMaeFocusedList(KoumokuList* mae) {
	this->mae_focused_list = mae;
}




void KoumokuNOEQUIP::_exedayo(Gamen* gamen, GamenPart* gp, KoumokuList* kl) {

	//this->equipParts();
	// 前の画面に戻る
	this->g_part->setFocused(gamen,false);
	this->g->temp_focused_list = g_part->mae_focused_list;
	g->temp_focused_list->setEnable(true);
}


void KoumokuNOEQUIP::byouga(Graphics* g, GUI* gui, float dsecond, int stamp,bool has_clicked) {


if (!getEnabled() && !has_clicked) return;
	t->setRenderTexColor(gui_koumoku_name_bg_id, 0xFFFFFFFFF);
}// focused_koumoku のkoumokuでもbyougaは呼ばれる

void KoumokuNOEQUIP::focusedByouga(Graphics* g, GUI* gui, float dsecond, int stamp, bool has_clicked) {
if (!getEnabled() && !has_clicked) return;
	//int stt = stamp % 13;
	static float unko=0;
	unko += dsecond;
	if (unko >= 1000) {
		unko =0;
	}

	int stt = (int) (unko) % 130;
	unsigned int color = ((((0xFFFF0000 / 130 * stt) & 0x77770000) + 0x88880000) & 0xFFFF0000) + 0x0000FFFF;
	t->setRenderTexColor(gui_koumoku_name_bg_id,color);
}