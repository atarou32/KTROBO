#include "KTPaintSheet.h"
#include "math.h"
#include "KTPaintNuri.h"

KTPaintSheet::KTPaintSheet(void)
{
	eline_max = 0;
	pline_max = 0;
	memset(elines,0,sizeof(KTPAINT_enpituline)*KTPAINT_SHEET_LINE_MAX);
	memset(plines,0,sizeof(KTPAINT_penline)*KTPAINT_SHEET_LINE_MAX);
	memset(kyoku_plines, 0, sizeof(KTPAINT_penkyokuline)*KTPAINT_SHEET_KYOKULINE_MAX);
	kyokupline_max = 0;

	memset(hei_part,0,sizeof(KTPAINT_penheiryouikipart)*KTPAINT_PENHEIRYOUIKI_PART_MAX);
	memset(hei, 0, sizeof(KTPAINT_penheiryouiki)*KTPAINT_PENHEIRYOUIKI_MAX);
	hei_part_max = 0;
	hei_max = 0;

	memset(hei_daen, 0, sizeof(KTPAINT_penheiryouikidaen)* KTPAINT_PENHEIRYOUIKI_DAEN_MAX);
	hei_daen_max =0;

}


KTPaintSheet::~KTPaintSheet(void)
{
}

#define KTPAINT_SHEET_HEIKIN_LAST_NUM 10

void KTPaintSheet::heikinPline() {
	/*
	double x;
	double y;
	for (int i=pline_start_index+KTPAINT_SHEET_HEIKIN_LAST_NUM;i<pline_end_index;i++) {
		x = 0;
		y = 0;
		for (int k=0;k<KTPAINT_SHEET_HEIKIN_LAST_NUM;k++) {
			KTPAINT_penline* l = &this->plines[i-k];
			x += l->dx;
			y += l->dy;
		}
		x /= (KTPAINT_SHEET_HEIKIN_LAST_NUM+1);
		y /= (KTPAINT_SHEET_HEIKIN_LAST_NUM+1);
		KTPAINT_penline* ll = &this->plines[i];
		ll->x += floor(x);
		ll->y += floor(y);
	}

	for (int i=pline_start_index;i<pline_end_index-1;i++) {
		KTPAINT_penline* l = &this->plines[i];
		KTPAINT_penline* l2 = &this->plines[1+i];
		l->dx = l2->x - l->x;
		l->dy = l2->y - l->y;
	}

	*/
}


void KTPaintSheet::setEline(POINT mpo, POINT po, char alpha, unsigned char color_index) {
	short dx = po.x - mpo.x;
	short dy = po.y - mpo.y;
	float tx = abs(dx) /127.0f;
	float ty = abs(dy) /127.0f;
	int tans;

	if (eline_max >= KTPAINT_SHEET_LINE_MAX) {
		return;
	}


	if ((abs(dx) > 127) || (abs(dy) > 127)) {
		if (tx > ty) {
			tans = floor(tx) + 1;
		} else {
			tans = floor(ty) + 1;
		}

		unsigned short temp_x = mpo.x;
		unsigned short temp_y = mpo.y;

		for (int i=0;i<tans-1;i++) {
			elines[eline_max].x = temp_x + dx / tans * i;
			elines[eline_max].y = temp_y + dy / tans * i;
			elines[eline_max].dx = dx / tans * (1);
			elines[eline_max].dy = dy / tans * (1);
			elines[eline_max].color_index = color_index;
			elines[eline_max].alpha = alpha;
			eline_max++;
			if (eline_max >= KTPAINT_SHEET_LINE_MAX) {
				return;
			}
		}

		elines[eline_max].x = temp_x+dx/tans*(tans-1);
		elines[eline_max].y = temp_y + dy/tans*(tans-1);
		elines[eline_max].dx = mpo.x - (temp_x+dx/tans*(tans-1));
		elines[eline_max].dy = mpo.y - (temp_y+dy/tans*(tans-1));
		elines[eline_max].color_index = color_index;
		elines[eline_max].alpha = alpha;
		eline_max++;
		if (eline_max >= KTPAINT_SHEET_LINE_MAX) {
			return;
		}
		return;
	}


	elines[eline_max].x = mpo.x;
	elines[eline_max].y = mpo.y;
	elines[eline_max].dx = po.x - mpo.x;
	elines[eline_max].dy = po.y - mpo.y;
	elines[eline_max].color_index = color_index;
	elines[eline_max].alpha = alpha;
	eline_max++;
	if (eline_max >= KTPAINT_SHEET_LINE_MAX) {
		return;
	}
}
void KTPaintSheet::setPline(POINT mpo, POINT po, unsigned char width, unsigned char nwidth, unsigned char pen_index) {

	short dx = po.x - mpo.x;
	short dy = po.y - mpo.y;
	float tx = abs(dx) /127.0f;
	float ty = abs(dy) /127.0f;
	float dwidth = nwidth - width;
	int tans;

	if (pline_max >= KTPAINT_SHEET_LINE_MAX) {
		pline_max = 0;
		return;
	}


	if ((abs(dx) > 127) || (abs(dy) > 127)) {
		if (tx > ty) {
			tans = floor(tx) + 1;
		} else {
			tans = floor(ty) + 1;
		}

		unsigned short temp_x = mpo.x;
		unsigned short temp_y = mpo.y;

		for (int i=0;i<tans-1;i++) {
			plines[pline_max].x = temp_x + dx / tans * i;
			plines[pline_max].y = temp_y + dy / tans * i;
			plines[pline_max].dx = dx / tans * (1);
			plines[pline_max].dy = dy / tans * (1);
			plines[pline_max].width_and_nwidth = floor(width + dwidth / tans * i) + ((int)floor(width + dwidth / tans*(i+1)) << 4) ;
			plines[pline_max].pen_index = pen_index;
			pline_max++;
			if (pline_max >= KTPAINT_SHEET_LINE_MAX) {
				return;
			}
		}

		plines[pline_max].x = temp_x+dx/tans*(tans-1);
		plines[pline_max].y = temp_y + dy/tans*(tans-1);
		plines[pline_max].dx = po.x - (temp_x+dx/tans*(tans-1));
		plines[pline_max].dy = po.y - (temp_y+dy/tans*(tans-1));
		plines[pline_max].width_and_nwidth = floor(width + dwidth / tans * (tans-1)) + (nwidth << 4);
		plines[pline_max].pen_index = pen_index;
		pline_max++;
		if (pline_max >= KTPAINT_SHEET_LINE_MAX) {
			return;
		}
		return;
	}


	plines[pline_max].x = mpo.x;
	plines[pline_max].y = mpo.y;
	plines[pline_max].dx = po.x - mpo.x;
	plines[pline_max].dy = po.y - mpo.y;
	plines[pline_max].width_and_nwidth = nwidth + (width << 4);
	plines[pline_max].pen_index = pen_index;
	pline_max++;
	if (pline_max >= KTPAINT_SHEET_LINE_MAX) {
		return;
	}
}


void KTPaintSheet::calcHeiryouiki(KTPaintNuri* nuri) {

	vector<KTPAINT_koutens*>* koutenss = nuri->getKoutenss();
	KTPAINT_kouten* koutens = nuri->getKoutens();








}

void KTPaintSheet::motomeruJyusin(KTPAINT_penheiryouiki* ryou, KTPAINT_penheiryouikidaen* daen) {

	daen->height = 0;
	daen->index=0;
	daen->theta = 0;
	daen->width = 0;
	daen->x = 0;
	daen->y = 0;

	double x =0;
	double y =0;
	int t_count=0;
	for (int i=ryou->startheiryouiki; i<=ryou->endheiryouiki;i++) {
		x += this->hei_part[i].kouten_x;
		y += this->hei_part[i].kouten_y;
		t_count++;
		for (int k=hei_part[i].keiro_first_index;k<=hei_part[i].keiro_last_index;k++) {
			x += this->plines[k].x;
			y += this->plines[i].y;
			t_count++;
		}
	}
	if (t_count) {
		x /= t_count;
		y /= t_count;
	}
	
	// 次にradius を求める
	float radius2 = 0;

	for (int i=ryou->startheiryouiki; i<=ryou->endheiryouiki;i++) {
		float rad = (hei_part[i].kouten_x-x)*(hei_part[i].kouten_x-x)+
			(hei_part[i].kouten_y-y)*(hei_part[i].kouten_y-y);
		if (rad > radius2) {
			radius2 = rad;
		}
		for (int k=hei_part[i].keiro_first_index;k<=hei_part[i].keiro_last_index;k++) {
			float rad = (plines[k].x-x)*(plines[k].x-x)+(plines[i].y-y)*(plines[i].y-y);
			if (rad > radius2) {
				radius2 = rad;
			}
		}
	}

	daen->height = sqrt(radius2);
	daen->width = daen->height;
	daen->theta = 0;
	daen->x = x;
	daen->y = y;
}

bool KTPaintSheet::addTempPartsCount(KTPAINT_penheiryouikipart* temp_parts, int* temp_c, int maxx, KTPAINT_penheiryouiki* ryou1) {


	for (int i=ryou1->startheiryouiki; i<=ryou1->endheiryouiki;i++) {
		bool has_already = false;
		int tt=0;
		for (int k=0;k<*temp_c;k++) {

			if ((temp_parts[k].keiro_first_index == hei_part[i].keiro_first_index)
				&& (temp_parts[k].keiro_last_index == hei_part[i].keiro_last_index)) {
					has_already = true;
					tt =k;
					break;
			}
		}
		if (has_already) {
			// tukuru
			temp_parts[*temp_c].keiro_first_index = hei_part[i].keiro_first_index;
			temp_parts[*temp_c].keiro_last_index = hei_part[i].keiro_last_index;
			temp_parts[*temp_c].kouten_index = 1;
			*temp_c = *temp_c+1;
			if (*temp_c >= maxx) {
				return false;
			}
		} else {
			temp_parts[tt].kouten_index++;
		}
	}


	return true;

}



bool KTPaintSheet::oyakoKankeiHeiryouiki(KTPAINT_penheiryouiki* ryou1, KTPAINT_penheiryouiki* ryou2, KTPAINT_penheiryouiki* ryou3, KTPAINT_penheiryouiki* out_oya_ryou, KTPaintNuri* nuri) {

	// 各領域の重心を求める
	if (!ryou1->daen_calced) {
		motomeruJyusin(ryou1,&hei_daen[hei_daen_max]);
		ryou1->daen_index = hei_daen_max;
		ryou1->daen_calced = 1;
		hei_daen_max++;
	}
	if (!ryou2->daen_calced) {
		motomeruJyusin(ryou2,&hei_daen[hei_daen_max]);
		ryou2->daen_index = hei_daen_max;
		ryou2->daen_calced = 1;
		hei_daen_max++;
	}
	if (!ryou3->daen_calced) {
		motomeruJyusin(ryou3,&hei_daen[hei_daen_max]);
		ryou3->daen_index = hei_daen_max;
		ryou3->daen_calced = 1;
		hei_daen_max++;
	}

	// きれいな親子関係になっているかどうか判定する
	// 1024以下の点で構成される閉領域を対象とする
	static KTPAINT_penheiryouikipart temp_parts[1024];
	memset(temp_parts,0,sizeof(KTPAINT_penheiryouikipart)*1024);
	int temp_c = 0;

	// ryouiki1について
	addTempPartsCount(temp_parts,&temp_c,1024,ryou1);
	addTempPartsCount(temp_parts,&temp_c,1024,ryou2);
	addTempPartsCount(temp_parts,&temp_c,1024,ryou3);

	for (int i=0;i<temp_c;i++) {
		if (temp_parts[i].kouten_index < 2) {
			// きれいな親子関係ではない
			return false;
		}
	}
	

	// きれいな親子関係になっている場合は
	// 親となる閉領域が必ずほかの２領域の点も含むはず
	// いびつな形の場合は重心の中心点がどの領域にも含まれないことが起こりうる
	// その場合は親子関係にないとして３つ全て残す
	//map<pair<KTPAINT_penheiryouiki*, KTPAINT_penheiryouiki*>, int> oyakokankei; 
	// int 0 最初 がoya  1 最後がoya 2 不明 3が身計算
	int te12=3;
	if (ryou1->startheiryouiki < ryou2->startheiryouiki) {
		if (oyakokankei.find(pair<KTPAINT_penheiryouiki*,KTPAINT_penheiryouiki*>(ryou1,ryou2)) != oyakokankei.end()) {
			te12 = oyakokankei[pair<KTPAINT_penheiryouiki*,KTPAINT_penheiryouiki*>(ryou1,ryou2)];
		}
	} else {
		if (oyakokankei.find(pair<KTPAINT_penheiryouiki*,KTPAINT_penheiryouiki*>(ryou2,ryou1)) != oyakokankei.end()) {
			te12 = oyakokankei[pair<KTPAINT_penheiryouiki*,KTPAINT_penheiryouiki*>(ryou2,ryou1)];
		}
	}

	int te13 =3;
	if (ryou1->startheiryouiki < ryou3->startheiryouiki) {
		if (oyakokankei.find(pair<KTPAINT_penheiryouiki*,KTPAINT_penheiryouiki*>(ryou1,ryou3)) != oyakokankei.end()) {
			te13 = oyakokankei[pair<KTPAINT_penheiryouiki*,KTPAINT_penheiryouiki*>(ryou1,ryou3)];
		}
	} else {
		if (oyakokankei.find(pair<KTPAINT_penheiryouiki*,KTPAINT_penheiryouiki*>(ryou3,ryou1)) != oyakokankei.end()) {
			te13 = oyakokankei[pair<KTPAINT_penheiryouiki*,KTPAINT_penheiryouiki*>(ryou3,ryou1)];
		}
	}

	int te23 =3;
	if (ryou2->startheiryouiki < ryou3->startheiryouiki) {
		if (oyakokankei.find(pair<KTPAINT_penheiryouiki*,KTPAINT_penheiryouiki*>(ryou2,ryou3)) != oyakokankei.end()) {
			te23 = oyakokankei[pair<KTPAINT_penheiryouiki*,KTPAINT_penheiryouiki*>(ryou2,ryou3)];
		}
	} else {
		if (oyakokankei.find(pair<KTPAINT_penheiryouiki*,KTPAINT_penheiryouiki*>(ryou3,ryou2)) != oyakokankei.end()) {
			te23 = oyakokankei[pair<KTPAINT_penheiryouiki*,KTPAINT_penheiryouiki*>(ryou3,ryou2)];
		}
	}

	if (te12 == 3) {
		// oyakohanteiを行って入れ込む
		te12 = karuiOyakoHantei(ryou1,ryou2,nuri);
		oyakokankei.insert(pair<pair<KTPAINT_penheiryouiki*,KTPAINT_penheiryouiki*>,int>(
			pair<KTPAINT_penheiryouiki*,KTPAINT_penheiryouiki*>(ryou1,ryou2),te12));

	}

	if (te23 == 3) {
		// oyakohanteiを行って入れ込む
		te23 = karuiOyakoHantei(ryou2,ryou3,nuri);
		oyakokankei.insert(pair<pair<KTPAINT_penheiryouiki*,KTPAINT_penheiryouiki*>,int>(
			pair<KTPAINT_penheiryouiki*,KTPAINT_penheiryouiki*>(ryou2,ryou3),te23));

	}

	if (te13 == 3) {
		// oyakohanteiを行って入れ込む
		te13 = karuiOyakoHantei(ryou1,ryou3,nuri);
		oyakokankei.insert(pair<pair<KTPAINT_penheiryouiki*,KTPAINT_penheiryouiki*>,int>(
			pair<KTPAINT_penheiryouiki*,KTPAINT_penheiryouiki*>(ryou1,ryou3),te13));

	}

	if ((te12 ==0) &&( te13 ==0)) {
		//1 oya
		*out_oya_ryou = *ryou1;
		return true;
	}

	if ((te12 == 1) && (te23==0) ) {
		//2 oya
		*out_oya_ryou = *ryou2;
	}

	if ((te13==1) && (te23==1)) {
		*out_oya_ryou = *ryou3;
	}

	//きれいな親子関係だけど不明な部分があるので残す
	return false;


}


int KTPaintSheet::karuiOyakoHantei(KTPAINT_penheiryouiki* ryou1, KTPAINT_penheiryouiki* ryou2, KTPaintNuri* nuri) {
	KTPAINT_penheiryouikidaen* daen1 = &hei_daen[ryou1->daen_index];
	KTPAINT_penheiryouikidaen* daen2 = &hei_daen[ryou2->daen_index];
	bool d1r1 = isInHeiryouiki(daen1->x,daen1->y,ryou1,nuri);
	bool d1r2 = isInHeiryouiki(daen1->x, daen1->y, ryou2, nuri);
	bool d2r1 = isInHeiryouiki(daen2->x,daen2->y,ryou1,nuri);
	bool d2r2 = isInHeiryouiki(daen2->x,daen2->y, ryou2, nuri);
	if (d1r2 && d2r1) return 2;
	if (d1r2 && !d2r1 && d2r2) return 1;
	if (d1r1 && !d1r2 && d1r2) return 0;
	return 2;
}

bool KTPaintSheet::isInHeiryouiki(unsigned short x, unsigned short y, KTPAINT_penheiryouiki* heid, KTPaintNuri* nuri) {
	// crossing number algorithm を使用する
	KTPAINT_penline temp_penline_mae;
	KTPAINT_penline temp_penline_ato;
	temp_penline_mae.dx = 0;
	temp_penline_mae.dy = 0;
	temp_penline_mae.pen_index = 0;
	temp_penline_mae.width_and_nwidth = 0;
	temp_penline_mae.x = x;
	temp_penline_mae.y = y;

	temp_penline_ato.dx = 0;
	temp_penline_ato.dy = 0;
	temp_penline_ato.pen_index = 0;
	temp_penline_ato.width_and_nwidth = 0;
	temp_penline_ato.x = 60000;// 十分に大きな値
	temp_penline_ato.y = y;
	
	int kousa_kaisuu =0;
	for (int i=heid->startheiryouiki; i<=heid->endheiryouiki;i++) {
		for (int k=hei_part[i].keiro_first_index;k<=hei_part[i].keiro_last_index;k++) {
			KTPAINT_penline tempp;
			tempp.dx = 0;
			tempp.dy = 0;
			tempp.pen_index = 0;
			tempp.width_and_nwidth = 0;
			tempp.x = plines[k].x+plines[k].dx;// 十分に大きな値
			tempp.y = plines[k].y+plines[k].dy;
			if ((plines[k].dx !=0) && (plines[k].dy==0)) {
				continue;
			}
			if (nuri->isKousaLine2(&temp_penline_mae,&temp_penline_ato, &plines[k],&tempp)) {
				kousa_kaisuu++;
			}
		}
	}
	if ((kousa_kaisuu % 2) == 1) {
		// in
		return true;
	}
	return false;


}