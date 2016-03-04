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
	for (int i=0;i<KTPAINT_PENHEIRYOUIKI_MAX;i++) {
		hei[i].hen_id = i;
	}
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

// end_daen_index を返す
int KTPaintSheet::bunkatuDaenWithLine(int hei_index, int start_index, int end_index,int start_daen_index, int end_daen_index) {

	KTPAINT_penheiryouiki* pl = &this->hei[hei_index];
	int end_new_daen_index = end_daen_index;
	// 楕円の計算

	// 楕円のインデックス　が０でかつ線分と交差している楕円を見つける
	// 



	// 閉領域に反映

	pl->daen_many_start = start_daen_index;
	pl->daen_many_end = end_new_daen_index;

	return end_new_daen_index;

}

#define KTPAINT_SHEET_TEMPHEI_NOHEI 1
#define KTPAINT_SHEET_TEMPHEI_HEI 2
#define KTPAINT_SHEET_TEMPHEI_NOMOREHEI 3
KTPAINT_kouten* getDepthKouten(KTPAINT_kouten* siten, KTPAINT_kouten* koutens, int* keiro_indexs, int now_depth) {
	int nn_depth = 0;
	KTPAINT_kouten* ans = siten;
	set<pair<KTPAINT_kouten*, pair<KTPAINT_penkyokuline*, pair<int,int>>>>::iterator it;
	for (int i=0;i<now_depth;i++) {
		int index = keiro_indexs[i];
		it = ans->keiros.begin();
		for (int i=0;i<index;i++) {
			it++;
		}
		if (it != ans->keiros.end()) {
			ans = (*it).first;
		} else {
			return NULL;
		}
	}
	return ans;
}



void irekomiTempHei(KTPAINT_kouten* siten,
					KTPAINT_kouten* koutens,
					int* keiro_indexs, 
					KTPAINT_penheiryouiki* temp_hei, 
					int* temp_hei_count,
					KTPAINT_penheiryouikipart* temp_heipart,
					int* temp_heipart_count) {


	int nn_depth = 0;
	KTPAINT_kouten* ans = siten;
	set<pair<KTPAINT_kouten*, pair<KTPAINT_penkyokuline*, pair<int,int>>>>::iterator it;
	int temp_hei_c_start = *temp_hei_count;
	int temp_heipart_c_start = *temp_heipart_count;

	while((siten != ans) || (nn_depth < KTPAINT_PENHEIRYOUIKI_MAX)) {
		int index = keiro_indexs[nn_depth];
		it = ans->keiros.begin();
		for (int i=0;i<index;i++) {
			it++;
		}
		if (it != ans->keiros.end()) {

			KTPAINT_penheiryouikipart* tempp_heip = &temp_heipart[*temp_heipart_count];
			tempp_heip->kouten_x = ans->x;
			tempp_heip->kouten_y = ans->y;
			tempp_heip->kouten_index = ans->index;
			pair<KTPAINT_penkyokuline*,pair<int,int>> ppp = (*it).second;

			tempp_heip->keiro_first_index = ppp.second.first;
			tempp_heip->keiro_last_index = ppp.second.second;
			tempp_heip->keiro_index = index;
			*temp_heipart_count = *temp_heipart_count+1;
			if (*temp_heipart_count >= KTPAINT_PENHEIRYOUIKI_PART_MAX) return;

			ans = (*it).first;
		} else {
			return;
		}

		if (ans == siten) break;

		nn_depth++;
	}

	if (*temp_heipart_count - temp_heipart_c_start>0) {
		// 加える
		if (*temp_hei_count < KTPAINT_PENHEIRYOUIKI_MAX) {
			temp_hei[*temp_hei_count].daen_calced = 0;
			temp_hei[*temp_hei_count].daen_index = 0;
			temp_hei[*temp_hei_count].daen_many_end = 0;
			temp_hei[*temp_hei_count].daen_many_start = 0;
			temp_hei[*temp_hei_count].endheiryouiki = *temp_heipart_count-1;
			temp_hei[*temp_hei_count].startheiryouiki = temp_heipart_c_start;
			//temp_hei[*temp_hei_count].hen_id = *temp_hei_count;
			temp_hei[*temp_hei_count].offset = 0;
			*temp_hei_count = *temp_hei_count + 1;
		}
	}

	return;
}




int KTPaintSheet::getTempHeiFromSitenAndID(KTPAINT_kouten* siten, KTPAINT_kouten* koutens, int* keiro_indexs,int keiro_ID) {

	int now_ID=0;
	int now_depth =0;
	KTPAINT_kouten* kk = siten;
	set<pair<KTPAINT_kouten*, pair<KTPAINT_penkyokuline*, pair<int,int>>>>::iterator it;
	bool is_finish = false;
	int count_max = KTPAINT_PENHEIRYOUIKI_MAX*8;
	int count=0;
	while(count < count_max) {
		int now_depth_index = keiro_indexs[now_depth];
		kk = getDepthKouten(siten,koutens,keiro_indexs,now_depth);
		if (kk == NULL) return KTPAINT_SHEET_TEMPHEI_NOMOREHEI;

		it = kk->keiros.begin();
		if (it == kk->keiros.end()) return KTPAINT_SHEET_TEMPHEI_NOHEI; 
		
	
		for (int i=0;i<now_depth_index;i++) {
			it++;
			if (it == kk->keiros.end()) {
			// もう経路がない場合は
				break;
				

			}
		}
		if (it == kk->keiros.end()) {
			// もう経路がない場合は
			if (now_depth) {
				// ひとつ上にしてみる
				now_depth--;
				keiro_indexs[now_depth]++;
				continue;
				

			} else {
				// ほんとうにもう経路がない
				return KTPAINT_SHEET_TEMPHEI_NOMOREHEI; 
			}
		}

		KTPAINT_kouten* ikisaki = (*it).first;
		bool konomama_ikeru = true;
		if (ikisaki->is_tuuka && ikisaki != siten) {
			// 別の点でループした
			konomama_ikeru = false;
		//	now_ID++;
			if (now_ID == keiro_ID) return KTPAINT_SHEET_TEMPHEI_NOHEI;
			now_ID++;
		} else if(ikisaki->is_tuuka && ikisaki == siten) {
			konomama_ikeru = false;
			
			// 同じ経路を通る場合も含まれることに注意する
			if (now_ID == keiro_ID) return KTPAINT_SHEET_TEMPHEI_HEI;
			now_ID++;
		}


		if (konomama_ikeru) {
			kk->is_tuuka = true;
			kk = ikisaki;
			now_depth++;
			keiro_indexs[now_depth]=0;
			
		} else {
			// このままいけない場合は次の経路にいくかもしくは前の経路に戻る
			keiro_indexs[now_depth]++;
		}
		count++;
	}


	return KTPAINT_SHEET_TEMPHEI_NOHEI;
}

void KTPaintSheet::tryTourokuTempHeiToHei(KTPAINT_penheiryouiki* temp_hei, KTPAINT_penheiryouikipart* temp_heipart) {

	// 同じ経路を通っていないかどうかチェックする
	
	for (int i=temp_hei->startheiryouiki;i<=temp_hei->endheiryouiki;i++) {
		KTPAINT_penheiryouikipart* temp_part = &temp_heipart[i];
		for (int k=temp_hei->startheiryouiki;k<=i;k++) {
			if (k == i) continue;
			KTPAINT_penheiryouikipart* pp_part = &temp_heipart[k];
			if (/*(pp_part->kouten_index == temp_part->kouten_index)
				&&*/(pp_part->keiro_first_index == temp_part->keiro_first_index)
				&& (pp_part->keiro_last_index == temp_part->keiro_last_index)) {
					// はじく
					return;
			}
		}
	}
	
	
	
	// すでに登録されているループをはじく

	for (int i=0;i<hei_max;i++) {
		KTPAINT_penheiryouiki* heid = &hei[i];
		// まず長さがおなじか
		int heid_len = heid->endheiryouiki - heid->startheiryouiki;
		int temp_heid_len = temp_hei->endheiryouiki - temp_hei->startheiryouiki;
		if (temp_heid_len == heid_len) {
			// temp_heidは始点をとおるか
			int hei_kouten_index = hei_part[heid->startheiryouiki].kouten_index;
			int temp_hei_kouten_index = KTPAINT_SHEET_KOUTEN_MAX;
			for (int k = temp_hei->startheiryouiki;k<= temp_hei->endheiryouiki;k++) {
				if (temp_heipart[k].kouten_index == hei_kouten_index) {
					temp_hei_kouten_index = k;
					break;
				}
			}
			if (temp_hei_kouten_index != KTPAINT_SHEET_KOUTEN_MAX) {
				
				bool jyun_is_onaji = true;
				bool gyaku_is_onaji = true;
				for (int k = 0;k<temp_hei->endheiryouiki-temp_hei->startheiryouiki;k++) {
					int indee = temp_hei_kouten_index+k+temp_hei->startheiryouiki;
					if (indee > temp_hei->endheiryouiki) {
						indee = indee - (temp_hei->endheiryouiki - temp_hei_kouten_index) + temp_hei->startheiryouiki;
					}
					int hei_inde = k + heid->startheiryouiki;
					if ((hei_part[hei_inde].keiro_first_index == temp_heipart[indee].keiro_first_index) && 
						(hei_part[hei_inde].keiro_last_index == temp_heipart[indee].keiro_last_index) &&
						(hei_part[hei_inde].keiro_index == temp_heipart[indee].keiro_index) &&
						(hei_part[hei_inde].kouten_index == temp_heipart[indee].kouten_index)) {
					} else {
						jyun_is_onaji = false;
						break;
					}

					hei_inde = heid->endheiryouiki - k;
					if ((hei_part[hei_inde].keiro_first_index == temp_heipart[indee].keiro_first_index) && 
						(hei_part[hei_inde].keiro_last_index == temp_heipart[indee].keiro_last_index) &&
						(hei_part[hei_inde].keiro_index == temp_heipart[indee].keiro_index) &&
						(hei_part[hei_inde].kouten_index == temp_heipart[indee].kouten_index)) {
					} else {
						gyaku_is_onaji = false;
						break;
					}

				}
				//はじく
				if (gyaku_is_onaji || jyun_is_onaji) return;



			}
		}
	}

	// 登録する

	if ((hei_max < KTPAINT_PENHEIRYOUIKI_MAX) && hei_part_max + temp_hei->endheiryouiki - temp_hei->startheiryouiki+1 < KTPAINT_PENHEIRYOUIKI_PART_MAX) { 
		if (!hei[hei_max].daen_calced) {
			motomeruJyusin(&hei[hei_max], &hei_daen[hei_daen_max]);
			hei[hei_max].daen_calced = 1;
			hei[hei_max].daen_index = hei_daen_max;
			hei_daen_max++;
		} else {
			hei[hei_max].daen_calced = temp_hei->daen_calced;
			hei[hei_max].daen_index = temp_hei->daen_index;
		}
		hei[hei_max].daen_many_end = temp_hei->daen_many_end;
		hei[hei_max].daen_many_start = temp_hei->daen_many_start;
	
		hei[hei_max].hen_id = temp_hei->hen_id;
		hei[hei_max].offset = temp_hei->offset;
		hei[hei_max].startheiryouiki = hei_part_max;
		hei[hei_max].endheiryouiki = temp_hei->endheiryouiki - temp_hei->startheiryouiki + hei_part_max;

		for (int i=temp_hei->startheiryouiki; i<= temp_hei->endheiryouiki;i++) {
			hei_part[hei_part_max].keiro_first_index = temp_heipart[i].keiro_first_index;
			hei_part[hei_part_max].keiro_index = temp_heipart[i].keiro_index;
			hei_part[hei_part_max].keiro_last_index = temp_heipart[i].keiro_last_index;
			hei_part[hei_part_max].kouten_index = temp_heipart[i].kouten_index;
			hei_part[hei_part_max].kouten_x = temp_heipart[i].kouten_x;
			hei_part[hei_part_max].kouten_y = temp_heipart[i].kouten_y;
			hei_part_max++;


		}
		hei_max++;
	}

}





void KTPaintSheet::calcHeiryouiki(KTPaintNuri* nuri) {

	vector<KTPAINT_koutens*>* koutenss = nuri->getKoutenss();
	KTPAINT_kouten* koutens = nuri->getKoutens();
	int kouten_max = nuri->getKoutenMax();
	static KTPAINT_penheiryouiki temp_hei[KTPAINT_PENHEIRYOUIKI_MAX];
	static bool temp_hei_use[KTPAINT_PENHEIRYOUIKI_MAX];
	static int keiro_indexs[KTPAINT_PENHEIRYOUIKI_MAX];// max_depth
	int max_depth = KTPAINT_PENHEIRYOUIKI_MAX-1;

	static KTPAINT_penheiryouikipart temp_heipart[KTPAINT_PENHEIRYOUIKI_PART_MAX];
	memset(temp_hei,0,sizeof(KTPAINT_penheiryouiki)*KTPAINT_PENHEIRYOUIKI_MAX);
	for (int i=0;i<KTPAINT_PENHEIRYOUIKI_MAX*8;i++) {
		
		keiro_indexs[i] = 0;
	}

	memset(temp_heipart,0, sizeof(KTPAINT_penheiryouikipart)*KTPAINT_PENHEIRYOUIKI_PART_MAX);
	int temp_hei_count=0;
	int temp_heipart_count = 0;

	// すべての交点に対して走査する
	for (int k=0;k<kouten_max;k++) {

		if (hei_daen_max >= KTPAINT_PENHEIRYOUIKI_DAEN_MAX) continue;
		KTPAINT_kouten* siten = &koutens[k];
		temp_heipart_count = 0;
		temp_hei_count = 0;
		int temp_heipart_start_count = temp_heipart_count;
		int temp_heipart_end_count = temp_heipart_count;
		int keiro_ID = 0;
		bool is_has_more_keiro = true;
		for (int i=0;i<KTPAINT_PENHEIRYOUIKI_MAX;i++) {
			keiro_indexs[i] = 0;
			temp_hei_use[i] = true;
			temp_hei[i].hen_id = i;
			temp_hei[i].daen_calced = 0;
		}

		while(is_has_more_keiro && (keiro_ID < KTPAINT_PENHEIRYOUIKI_MAX)) {
			for (int l=0;l<kouten_max;l++) {
				koutens[l].is_tuuka = false;
			}
			for (int i=0;i<KTPAINT_PENHEIRYOUIKI_MAX;i++) {
				keiro_indexs[i] = 0;
		//	temp_hei_use[i] = true;
			//temp_hei[i].hen_id = i;
			}	
			siten->is_tuuka = true;
			// 最初はsitenから始まって　0,0,0,0,0,0,0,0,0... の経路をたどる(ID=1)
			// 次はsitenから始まって　0,0,0,0,0,0,....,0,1 の経路をたどる(ID=2) 1karaIDはじまることに注意
			// 個々の経路が閉集合をなしているかどうかを判定するのは行わなくてはならない
			// 経路がsiten以外でループを作っていたり、同じ経路をたどることになってしまっていたり
			// する場合もIDに数えるが　ループや同じ経路の場合は次の深さを走査しない
			int kekka  = getTempHeiFromSitenAndID(siten,koutens,keiro_indexs,keiro_ID);
			if (kekka == KTPAINT_SHEET_TEMPHEI_NOMOREHEI) {
				is_has_more_keiro = false;
			} else if(kekka == KTPAINT_SHEET_TEMPHEI_HEI) {
			// 閉集合をなしている
				// keiro_indexsに経路のインデックスの情報が入っているのでそれをもとにtemp_heipartに
				// 情報を入れ込む
				irekomiTempHei(siten,koutens,keiro_indexs, temp_hei, &temp_hei_count, temp_heipart, &temp_heipart_count);



			}
			keiro_ID++;
		}


		// 入れ込んだ同一点を始点とするきれいな親子関係がある場合は除く
		for (int i=0;i<temp_hei_count;i++) {
			for (int j=0;j<temp_hei_count;j++) {
				for (int p=0;p<temp_hei_count;p++) {

					if (i==j) continue;
					if (i==p) continue;
					if (j==p) continue;

					if ( i > j ) continue;
					if ( j > p ) continue;
					if ( i > p ) continue;

					KTPAINT_penheiryouiki* oyahei=NULL;
					if (this->oyakoKankeiHeiryouiki(&temp_hei[i],&temp_hei[j],&temp_hei[p], &oyahei, nuri)) {
						// きれいな親子関係があったので除く
						temp_hei_use[oyahei->hen_id] = false;
					}
				}
			}
		}

		// 実際に使用される領域に加える
		// 同じ経路を通っているループははじく
		// すでに登録されているループ（始点のみ異なる、または回り方が逆なだけ）もはじく

		for (int i=0;i<temp_hei_count;i++) {
			if (temp_hei_use[i]) {
				tryTourokuTempHeiToHei(&temp_hei[i], temp_heipart);
			}
		}
	}
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

void KTPaintSheet::insertOyakoKankei(int te12, KTPAINT_penheiryouiki* ryou1, KTPAINT_penheiryouiki* ryou2) {
	if (ryou1->hen_id < ryou2->hen_id) {
	oyakokankei.insert(pair<pair<int,int>,int>(
		pair<int,int>(ryou1->hen_id,ryou2->hen_id),te12));

	} else {
		if (te12 == 0) {
			te12 = 1;
		} else if(te12 == 1) {
			te12 = 0;
		}
			oyakokankei.insert(pair<pair<int,int>,int>(
		pair<int,int>(ryou2->hen_id,ryou1->hen_id),te12));
	}
}


bool KTPaintSheet::oyakoKankeiHeiryouiki(KTPAINT_penheiryouiki* ryou1, KTPAINT_penheiryouiki* ryou2, KTPAINT_penheiryouiki* ryou3, KTPAINT_penheiryouiki** out_oya_ryou, KTPaintNuri* nuri) {

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
	if (ryou1->hen_id < ryou2->hen_id) {
		if (oyakokankei.find(pair<int,int>(ryou1->hen_id,ryou2->hen_id)) != oyakokankei.end()) {
			te12 = oyakokankei[pair<int,int>(ryou1->hen_id,ryou2->hen_id)];
		}
	} else {
		if (oyakokankei.find(pair<int,int>(ryou2->hen_id,ryou1->hen_id)) != oyakokankei.end()) {
			te12 = oyakokankei[pair<int,int>(ryou2->hen_id,ryou1->hen_id)];
		}
	}

	int te13 =3;
	if (ryou1->startheiryouiki < ryou3->startheiryouiki) {
		if (oyakokankei.find(pair<int,int>(ryou1->hen_id,ryou3->hen_id)) != oyakokankei.end()) {
			te13 = oyakokankei[pair<int,int>(ryou1->hen_id,ryou3->hen_id)];
		}
	} else {
		if (oyakokankei.find(pair<int,int>(ryou3->hen_id,ryou1->hen_id)) != oyakokankei.end()) {
			te13 = oyakokankei[pair<int,int>(ryou3->hen_id,ryou1->hen_id)];
		}
	}

	int te23 =3;
	if (ryou2->startheiryouiki < ryou3->startheiryouiki) {
		if (oyakokankei.find(pair<int,int>(ryou2->hen_id,ryou3->hen_id)) != oyakokankei.end()) {
			te23 = oyakokankei[pair<int,int>(ryou2->hen_id,ryou3->hen_id)];
		}
	} else {
		if (oyakokankei.find(pair<int,int>(ryou3->hen_id,ryou2->hen_id)) != oyakokankei.end()) {
			te23 = oyakokankei[pair<int,int>(ryou3->hen_id,ryou2->hen_id)];
		}
	}

	if (te12 == 3) {
		// oyakohanteiを行って入れ込む
		
		te12 = karuiOyakoHantei(ryou1,ryou2,nuri);
		insertOyakoKankei(te12, ryou1,ryou2);
			
	}

	if (te23 == 3) {
		// oyakohanteiを行って入れ込む
		te23 = karuiOyakoHantei(ryou2,ryou3,nuri);
		insertOyakoKankei(te23,ryou2,ryou3);

	}

	if (te13 == 3) {
		// oyakohanteiを行って入れ込む
		te13 = karuiOyakoHantei(ryou1,ryou3,nuri);
		insertOyakoKankei(te13,ryou1,ryou3);

	}

	if ((te12 ==0) &&( te13 ==0)) {
		//1 oya
		*out_oya_ryou = ryou1;
		return true;
	}

	if ((te12 == 1) && (te23==0) ) {
		//2 oya
		*out_oya_ryou = ryou2;
	}

	if ((te13==1) && (te23==1)) {
		*out_oya_ryou = ryou3;
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