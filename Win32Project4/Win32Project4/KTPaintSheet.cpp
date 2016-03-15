#include "KTPaintSheet.h"
#include "math.h"
#include "KTPaintNuri.h"

KTPaintSheet::KTPaintSheet(void)
{
	eline_max = 0;
	pline_max = 0;
	memset(elines,0,sizeof(KTPAINT_enpituline)*KTPAINT_SHEET_DUMMY_MAX);
	memset(plines,0,sizeof(KTPAINT_penline)*KTPAINT_SHEET_LINE_MAX);
	memset(kyoku_plines, 0, sizeof(KTPAINT_penkyokuline)*KTPAINT_SHEET_KYOKULINE_MAX);
	kyokupline_max = 0;

	memset(hei_plines,0,sizeof(KTPAINT_penline)*KTPAINT_SHEET_LINE_MAX);
	memset(hei_kyoku_plines, 0, sizeof(KTPAINT_penkyokuline)*KTPAINT_SHEET_KYOKULINE_MAX);
	hei_kyokupline_max = 0;
	hei_pline_max = 0;


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


void KTPaintSheet::setEline(POINT mpo, POINT po, unsigned char alpha, unsigned char color_index) {
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



void KTPaintSheet::setHeiPline(POINT mpo, POINT po, unsigned char width, unsigned char nwidth, unsigned char pen_index) {
	short dx = po.x - mpo.x;
	short dy = po.y - mpo.y;
	float tx = abs(dx) /127.0f;
	float ty = abs(dy) /127.0f;
	float dwidth = nwidth - width;
	int tans;

	if (hei_pline_max >= KTPAINT_SHEET_HEILINE_MAX) {
	//	hei_pline_max = 0;
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
			hei_plines[hei_pline_max].x = temp_x + dx / tans * i;
			hei_plines[hei_pline_max].y = temp_y + dy / tans * i;
			hei_plines[hei_pline_max].dx = dx / tans * (1);
			hei_plines[hei_pline_max].dy = dy / tans * (1);
			hei_plines[hei_pline_max].width_and_nwidth = floor(width + dwidth / tans * i) + ((int)floor(width + dwidth / tans*(i+1)) << 4) ;
			hei_plines[hei_pline_max].pen_index = pen_index;
			hei_pline_max++;
			if (hei_pline_max >= KTPAINT_SHEET_HEILINE_MAX) {
				return;
			}
		}

		hei_plines[hei_pline_max].x = temp_x+dx/tans*(tans-1);
		hei_plines[hei_pline_max].y = temp_y + dy/tans*(tans-1);
		hei_plines[hei_pline_max].dx = po.x - (temp_x+dx/tans*(tans-1));
		hei_plines[hei_pline_max].dy = po.y - (temp_y+dy/tans*(tans-1));
		hei_plines[hei_pline_max].width_and_nwidth = floor(width + dwidth / tans * (tans-1)) + (nwidth << 4);
		hei_plines[hei_pline_max].pen_index = pen_index;
		hei_pline_max++;
		if (pline_max >= KTPAINT_SHEET_HEILINE_MAX) {
			return;
		}
		return;
	}


	hei_plines[hei_pline_max].x = mpo.x;
	hei_plines[hei_pline_max].y = mpo.y;
	hei_plines[hei_pline_max].dx = po.x - mpo.x;
	hei_plines[hei_pline_max].dy = po.y - mpo.y;
	hei_plines[hei_pline_max].width_and_nwidth = nwidth + (width << 4);
	hei_plines[hei_pline_max].pen_index = pen_index;
	hei_pline_max++;
	if (hei_pline_max >= KTPAINT_SHEET_HEILINE_MAX) {
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
		//pline_max = 0;
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

//	pl->daen_many_start = start_daen_index;
//	pl->daen_many_end = end_new_daen_index;

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
	int temp_keiro_first_siten = 0;
	int temp_keiro_last_siten = 0;
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
			if (!nn_depth) {
				temp_keiro_first_siten = ppp.second.first;
				temp_keiro_last_siten = ppp.second.second;
			} else {
				if (temp_keiro_first_siten == ppp.second.first) {
					if (temp_keiro_last_siten == ppp.second.second) {
						// 同じ経路を通っている場合がある
						*temp_heipart_count = temp_heipart_c_start;
						return;
					}
				}
			}
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
			//temp_hei[*temp_hei_count].daen_calced = 0;
			temp_hei[*temp_hei_count].daen_index = 0;
			//temp_hei[*temp_hei_count].daen_many_end = 0;
			//temp_hei[*temp_hei_count].daen_many_start = 0;
			temp_hei[*temp_hei_count].endheiryouiki = *temp_heipart_count-1;
			temp_hei[*temp_hei_count].startheiryouiki = temp_heipart_c_start;
			//temp_hei[*temp_hei_count].hen_id = *temp_hei_count;
			//temp_hei[*temp_hei_count].offset = 0;
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

int KTPaintSheet::getTempHeiFromSitenAndID2(KTPAINT_kouten* siten, KTPAINT_kouten** mae_k, KTPAINT_kouten** kk, 
											KTPAINT_kouten* koutens, int* keiro_indexs,KTPAINT_kouten** temp_koutens, int* keiro_depth) {

	int now_ID=0;
	int now_depth =0;
//	KTPAINT_kouten* kk = siten;
	set<pair<KTPAINT_kouten*, pair<KTPAINT_penkyokuline*, pair<int,int>>>>::iterator it;
	bool is_finish = false;
	int count_max = KTPAINT_PENHEIRYOUIKI_MAX;
	int count=0;
	while((count < count_max) && (*keiro_depth<KTPAINT_PENHEIRYOUIKI_MAX)) {
		int now_depth_index = keiro_indexs[*keiro_depth];
	//	kk = getDepthKouten(siten,koutens,keiro_indexs,*keiro_depth);
		if (*kk) {
		for (int i=0;i<*keiro_depth;i++) {
			if (temp_koutens[i]) {
				if (temp_koutens[i] == *kk) {
					keiro_indexs[*keiro_depth]++;
					*kk = NULL;
					return KTPAINT_SHEET_TEMPHEI_NOHEI;
				}
			}
		}
		} 

		if (*kk == NULL) {
			if (*keiro_depth) {
				*keiro_depth = *keiro_depth - 1;
				keiro_indexs[*keiro_depth]++;
				count++;
				if (*mae_k&& *mae_k != siten) {
					(*mae_k)->is_tuuka = false;
				}
				*kk = *mae_k;
			
				if (*keiro_depth) {
					*mae_k = temp_koutens[*keiro_depth-1];//getDepthKouten(siten,koutens,keiro_indexs,*keiro_depth-1);
				} else {
					*mae_k = NULL;
				}
				
				continue;
			} else {
				return KTPAINT_SHEET_TEMPHEI_NOMOREHEI;
			}
		}
		it = (*kk)->keiros.begin();
		if (it == (*kk)->keiros.end()) return KTPAINT_SHEET_TEMPHEI_NOHEI; 
		
	
		for (int i=0;i<now_depth_index;i++) {
			it++;
			if (it == (*kk)->keiros.end()) {
			// もう経路がない場合は
				break;
				

			}
		}
		if (it == (*kk)->keiros.end()) {
			// もう経路がない場合は
			if (*keiro_depth) {
				// ひとつ上にしてみる
				*keiro_depth = *keiro_depth-1;
				keiro_indexs[*keiro_depth]++;

				if (*mae_k && *mae_k != siten) {
					(*mae_k)->is_tuuka = false;
				}

				*kk = *mae_k;
				

				if (*keiro_depth) {
					*mae_k =  temp_koutens[*keiro_depth-1];//getDepthKouten(siten,koutens,keiro_indexs,*keiro_depth-1);
				} else {
					*mae_k = NULL;
				}

				
				continue;
				

			} else {
				// ほんとうにもう経路がない
				return KTPAINT_SHEET_TEMPHEI_NOMOREHEI; 
			}
		}

		KTPAINT_kouten* ikisaki = (*it).first;
		bool konomama_ikeru = true;

		for (int i=0;i<*keiro_depth;i++) {
			if (temp_koutens[i]) {
				if ((temp_koutens[i] == *kk) && (ikisaki != siten)) {
					keiro_indexs[*keiro_depth]++;
					*kk = NULL;
					return KTPAINT_SHEET_TEMPHEI_NOHEI;
				}
			}
		}



		if (ikisaki->is_tuuka && ikisaki != siten) {
			// 別の点でループした
			konomama_ikeru = false;
		//	now_ID++;
		//	if (now_ID == keiro_ID) 
			keiro_indexs[*keiro_depth]++;
			return KTPAINT_SHEET_TEMPHEI_NOHEI;
		//	now_ID++;
		} else if(ikisaki->is_tuuka && ikisaki == siten) {
			konomama_ikeru = false;
			

		


			// 同じ経路を通る場合も含まれることに注意する
			//if (now_ID == keiro_ID) 

			return KTPAINT_SHEET_TEMPHEI_HEI;
			//now_ID++;
		}


		if (konomama_ikeru) {
			(*kk)->is_tuuka = true;
			temp_koutens[*keiro_depth] = *kk;
			*mae_k = *kk;
			*kk = ikisaki;
			*keiro_depth = *keiro_depth+1;
			keiro_indexs[*keiro_depth]=0;
			
		} else {
			// このままいけない場合は次の経路にいくかもしくは前の経路に戻る
			keiro_indexs[*keiro_depth]++;
		}
		count++;
	}


	return KTPAINT_SHEET_TEMPHEI_NOHEI;
}




int KTPaintSheet::getTempHeiFromSitenAndID3(KTPAINT_kouten* siten, KTPAINT_kouten** mae_k, KTPAINT_kouten** kk, 
											KTPAINT_kouten* koutens, int* keiro_indexs,
											KTPAINT_kouten** temp_koutens, int* keiro_depth,
											KTPAINT_bubblehei* heis) {

	int now_ID=0;
	int now_depth =0;
//	KTPAINT_kouten* kk = siten;
	set<pair<KTPAINT_kouten*, pair<KTPAINT_penkyokuline*, pair<int,int>>>>::iterator it;
	bool is_finish = false;
	int count_max = KTPAINT_PENHEIRYOUIKI_MAX;
	int count=0;
	while((count < count_max) && (*keiro_depth<KTPAINT_PENHEIRYOUIKI_MAX)) {
		int now_depth_index = keiro_indexs[*keiro_depth];
	//	kk = getDepthKouten(siten,koutens,keiro_indexs,*keiro_depth);
		if (*kk) {
		for (int i=0;i<*keiro_depth;i++) {
			if (temp_koutens[i]) {
				if (temp_koutens[i] == *kk) {
					keiro_indexs[*keiro_depth]++;
					*kk = NULL;
					return KTPAINT_SHEET_TEMPHEI_NOHEI;
				}
			}
		}
		} 

		if (*kk == NULL) {
			if (*keiro_depth) {
				*keiro_depth = *keiro_depth - 1;
				keiro_indexs[*keiro_depth]++;
				count++;
				if (*mae_k&& *mae_k != siten) {
					(*mae_k)->is_tuuka = false;
				}
				*kk = *mae_k;
			
				if (*keiro_depth) {
					*mae_k = temp_koutens[*keiro_depth-1];//getDepthKouten(siten,koutens,keiro_indexs,*keiro_depth-1);
				} else {
					*mae_k = NULL;
				}
				
				continue;
			} else {
				return KTPAINT_SHEET_TEMPHEI_NOMOREHEI;
			}
		}
		it = (*kk)->keiros.begin();
		if (it == (*kk)->keiros.end()) return KTPAINT_SHEET_TEMPHEI_NOHEI; 
		
	
		for (int i=0;i<now_depth_index;i++) {
			it++;
			if (it == (*kk)->keiros.end()) {
			// もう経路がない場合は
				break;
				

			}
		}
		if (it == (*kk)->keiros.end()) {
			// もう経路がない場合は
			if (*keiro_depth) {
				// ひとつ上にしてみる
				*keiro_depth = *keiro_depth-1;
				keiro_indexs[*keiro_depth]++;

				if (*mae_k && *mae_k != siten) {
					(*mae_k)->is_tuuka = false;
				}

				*kk = *mae_k;
				

				if (*keiro_depth) {
					*mae_k =  temp_koutens[*keiro_depth-1];//getDepthKouten(siten,koutens,keiro_indexs,*keiro_depth-1);
				} else {
					*mae_k = NULL;
				}

				
				continue;
				

			} else {
				// ほんとうにもう経路がない
				return KTPAINT_SHEET_TEMPHEI_NOMOREHEI; 
			}
		}

		KTPAINT_kouten* ikisaki = (*it).first;


		bool konomama_ikeru = true;





		for (int i=0;i<*keiro_depth;i++) {
			if (temp_koutens[i]) {
				if ((temp_koutens[i] == *kk) && (ikisaki != siten)) {
					keiro_indexs[*keiro_depth]++;
					*kk = NULL;
					return KTPAINT_SHEET_TEMPHEI_NOHEI;
				}
			}
		}



		if (ikisaki->is_tuuka && ikisaki != siten) {
			// 別の点でループした
			konomama_ikeru = false;
		//	now_ID++;
		//	if (now_ID == keiro_ID) 
			keiro_indexs[*keiro_depth]++;
			return KTPAINT_SHEET_TEMPHEI_NOHEI;
		//	now_ID++;
		} else if(ikisaki->is_tuuka && ikisaki == siten) {
			konomama_ikeru = false;
			

		


			// 同じ経路を通る場合も含まれることに注意する
			//if (now_ID == keiro_ID) 

			return KTPAINT_SHEET_TEMPHEI_HEI;
			//now_ID++;
		}


		if (heis->fukumu_kouten_indexs.find(ikisaki->index) == heis->fukumu_kouten_indexs.end()) {
			// 経路にないのでこのままいけない
			konomama_ikeru = false;
		}

		if (konomama_ikeru) {
			(*kk)->is_tuuka = true;
			temp_koutens[*keiro_depth] = *kk;
			*mae_k = *kk;
			*kk = ikisaki;
			*keiro_depth = *keiro_depth+1;
			keiro_indexs[*keiro_depth]=0;
			
		} else {
			// このままいけない場合は次の経路にいくかもしくは前の経路に戻る
			keiro_indexs[*keiro_depth]++;
		}
		count++;
	}


	return KTPAINT_SHEET_TEMPHEI_NOHEI;
}



bool KTPaintSheet::tryTourokuTempHeiToHei(KTPAINT_penheiryouiki* temp_hei, KTPAINT_penheiryouikipart* temp_heipart, DWORD color, bool onaji_check, unsigned short is_pline) {

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
					return false;
			}
		}
	}
	
	
	
	// すでに登録されているループをはじく
	if (onaji_check) {
	for (int i=0;i<hei_max;i++) {
		KTPAINT_penheiryouiki* heid = &hei[i];
		// まず長さがおなじか
		int heid_len = heid->endheiryouiki - heid->startheiryouiki;
		int temp_heid_len = temp_hei->endheiryouiki - temp_hei->startheiryouiki;
		if (temp_heid_len == heid_len) {
			// temp_heidは始点をとおるか
			int hei_kouten_index = hei_part[heid->startheiryouiki].kouten_index;
			int hei_kouten_first_index = hei_part[heid->startheiryouiki].keiro_first_index;
			int hei_kouten_last_index = hei_part[heid->startheiryouiki].keiro_last_index;
			int temp_hei_ryouiki_index = KTPAINT_SHEET_KOUTEN_MAX;
			for (int k = temp_hei->startheiryouiki;k<= temp_hei->endheiryouiki;k++) {
				if ((/*temp_heipart[k].kouten_index == hei_kouten_index*/
					(temp_heipart[k].keiro_first_index == hei_kouten_first_index)
				&&(temp_heipart[k].keiro_last_index == hei_kouten_last_index)
					
					
					
					)) {
					temp_hei_ryouiki_index = k-temp_hei->startheiryouiki;
					break;
				}
			}
			if (temp_hei_ryouiki_index != KTPAINT_SHEET_KOUTEN_MAX) {
				
				bool jyun_is_onaji = true;
				bool gyaku_is_onaji = true;
				for (int k = 0;k<=temp_hei->endheiryouiki-temp_hei->startheiryouiki;k++) {
					int indee = temp_hei_ryouiki_index+k+temp_hei->startheiryouiki;

					if (indee > temp_hei->endheiryouiki) {
						indee = indee - (temp_hei->endheiryouiki - temp_hei->startheiryouiki)-1;
					}
					int hei_inde = k + heid->startheiryouiki;
					if ((hei_part[hei_inde].keiro_first_index == temp_heipart[indee].keiro_first_index) && 
						(hei_part[hei_inde].keiro_last_index == temp_heipart[indee].keiro_last_index) 
						//(hei_part[hei_inde].keiro_index == temp_heipart[indee].keiro_index)/* &&
						/*(hei_part[hei_inde].kouten_index == temp_heipart[indee].kouten_index)*/) {
					} else {
						//if (indee = temp_hei->endheiryouiki) continue;
						jyun_is_onaji = false;
						
					}

					/*
					indee = temp_hei_ryouiki_index-k+temp_hei->startheiryouiki;

					if (indee < temp_hei->startheiryouiki) {
						indee = temp_hei->endheiryouiki - (temp_hei->startheiryouiki - indee);
					}
					*/

					hei_inde = heid->endheiryouiki - k+1;
					if (k==0) {
						hei_inde = heid->startheiryouiki;
					}

					if ((hei_part[hei_inde].keiro_first_index == temp_heipart[indee].keiro_first_index) && 
						(hei_part[hei_inde].keiro_last_index == temp_heipart[indee].keiro_last_index) 
						/*(hei_part[hei_inde].keiro_index == temp_heipart[indee].keiro_index) &&*/
						/*(hei_part[hei_inde].kouten_index == temp_heipart[indee].kouten_index)*/) {
					} else {
						//if (indee = temp_hei->endheiryouiki) continue;
						gyaku_is_onaji = false;
						
					}

					if (!jyun_is_onaji && !gyaku_is_onaji) break;

				}
				//はじく
				if (gyaku_is_onaji || jyun_is_onaji) return false;



			}
		}
	}
	}
	// 登録する

	if ((hei_max < KTPAINT_PENHEIRYOUIKI_MAX) && hei_part_max + temp_hei->endheiryouiki - temp_hei->startheiryouiki+1 < KTPAINT_PENHEIRYOUIKI_PART_MAX) { 
		
		//hei[hei_max].daen_many_end = temp_hei->daen_many_end;
		//hei[hei_max].daen_many_start = temp_hei->daen_many_start;
	
		hei[hei_max].hen_id = temp_hei->hen_id;
		//hei[hei_max].offset = temp_hei->offset;
		hei[hei_max].startheiryouiki = hei_part_max;
		hei[hei_max].endheiryouiki = temp_hei->endheiryouiki - temp_hei->startheiryouiki + hei_part_max;
		hei[hei_max].color = color;
		hei[hei_max].is_use_pline = is_pline;
		for (int i=temp_hei->startheiryouiki; i<= temp_hei->endheiryouiki;i++) {
			hei_part[hei_part_max].keiro_first_index = temp_heipart[i].keiro_first_index;
			hei_part[hei_part_max].keiro_index = temp_heipart[i].keiro_index;
			hei_part[hei_part_max].keiro_last_index = temp_heipart[i].keiro_last_index;
			hei_part[hei_part_max].kouten_index = temp_heipart[i].kouten_index;
			hei_part[hei_part_max].kouten_x = temp_heipart[i].kouten_x;
			hei_part[hei_part_max].kouten_y = temp_heipart[i].kouten_y;
			hei_part_max++;


		}
		if (is_pline) {
			temp_hei->daen_index = 0;
			// 再計算させる
		}
		if (!temp_hei->daen_index) {
			if (is_pline) {
				motomeruJyusinPline(&hei[hei_max], &hei_daen[hei_daen_max], hei_part);
			} else {
				motomeruJyusin(&hei[hei_max], &hei_daen[hei_daen_max], hei_part);
			}
		//	hei[hei_max].daen_calced = 1;
			hei[hei_max].daen_index = hei_daen_max;
			hei_daen_max++;
		} else {
		//	hei[hei_max].daen_calced = temp_hei->daen_calced;
			hei[hei_max].daen_index = temp_hei->daen_index;
		}

		hei_max++;
		return true;
	}
	return false;
}




void KTPaintSheet::simulationStepStart(unsigned short width, unsigned short height, KTPaintNuri* nuri) {

	KTPAINT_bubble* bubbles = nuri->getBubble();
	float radius = KTPAINT_PENHEIRYOUIKI_BUBBLE_RADIUS_SAISHO;
	int temp_count = 0;
	
	int width_count = width/ KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU / (int)(2*radius)+1;
	int height_count = height/ KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU / (int)(2*radius)+1;

	
	
	for (int j=0;j<height/KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU; j += (int)(2*radius)) {
		for (int i=0;i < width/KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU; i += (int)(2*radius)) {
		//	temp_count = i + j*width_count;
			bubbles[temp_count].radius = radius;
			bubbles[temp_count].status = 0;
			bubbles[temp_count].hei_index = 0;
			bubbles[temp_count].label_index_tate = 0;
			bubbles[temp_count].label_index_yoko = 0;
			bubbles[temp_count].ketugou[0] = NULL;
			for (int k=0;k<KTPAINT_PENHEIRYOUIKI_BUBBLE_FUKUMUTEN_MAX;k++) {
				bubbles[temp_count].fukumuten[k] = KTPAINT_PENHEIRYOUIKI_BUBBLE_FUKUMUTEN_FUKUMANAI;
			}
			bubbles[temp_count].x = i + (int)radius;
			bubbles[temp_count].y = j + (int)radius;
			temp_count++;
			
			if (temp_count >= KTPAINT_PENHEIRYOUIKI_BUBBLE_MAX) {
				nuri->setBubbleMax(temp_count);return;
			}
			
		}
	
	}


	// ketugou を設定する
	
		for (int j=0;j<height_count;j++) {
			for (int i=0;i<width_count;i++) {
			int index = i + j * width_count;
			if (index >= KTPAINT_PENHEIRYOUIKI_BUBBLE_MAX) continue;
			if (i >0) {
				int index3 = i-1 + j *width_count;
				bubbles[index].ketugou[1] = &bubbles[index3];
			}
			if (i < width_count-1) {
				int index4 = i + 1 + j*width_count;
				bubbles[index].ketugou[2] = &bubbles[index4];
			}
			if (j >0) {
				int index1 = i + (j-1)*width_count;
				bubbles[index].ketugou[0] = &bubbles[index1];
			}
			if (j < height_count-1) {
				int index6 = i + (j+1)*width_count;
				bubbles[index].ketugou[3] = &bubbles[index6];
			}
		}
	}

	nuri->setBubbleMax(temp_count);
	nuri->setBubbleHeiMax(0);

}

void KTPaintSheet::copyHeiTo(KTPaintSheet* dest) {

	int dest_hei_max = dest->getHeiMax();
	int dest_hei_daen_max = dest->getHeiDaenMax();
	int dest_hei_part_max = dest->getHeiPartMax();
	KTPAINT_penheiryouiki* dest_hei = dest->getHei();
	KTPAINT_penheiryouikipart* dest_hei_part =dest->getHeiPart();
	KTPAINT_penheiryouikidaen* dest_hei_daen = dest->getHeiDaen();

	if (dest_hei_max + hei_max >= KTPAINT_PENHEIRYOUIKI_MAX) return;
	if (dest_hei_daen_max + hei_daen_max >= KTPAINT_PENHEIRYOUIKI_DAEN_MAX) return;
	if (dest_hei_part_max + hei_part_max >= KTPAINT_PENHEIRYOUIKI_PART_MAX) return;
	
	int temp_heiryouiki;
	if (dest_hei_max) {
		temp_heiryouiki = dest_hei_part_max;
	} else {
		temp_heiryouiki = 0;
	}
	for (int i=0;i<hei_max;i++) {
		/*unsigned short startheiryouiki;
		unsigned short endheiryouiki;
		unsigned short daen_index;
		unsigned short hen_id;
		unsigned short is_use_pline; // 0 use_heipline, 1 use_pline
		unsigned short offset;
		DWORD color;*/
		dest_hei[i+dest_hei_max].startheiryouiki = temp_heiryouiki;
		dest_hei[i+dest_hei_max].endheiryouiki = temp_heiryouiki + hei[i].endheiryouiki - hei[i].startheiryouiki;
		temp_heiryouiki += hei[i].endheiryouiki - hei[i].startheiryouiki+1;
		dest_hei[i+dest_hei_max].daen_index = dest_hei_daen_max + i;
		dest_hei[i+dest_hei_max].is_use_pline = hei[i].is_use_pline;
		dest_hei[i+dest_hei_max].offset= 0;
		dest_hei[i+dest_hei_max].color = hei[i].color;
		// hei[i].daen_index の内容を登録する
		/*	unsigned short heiryouiki_index;
			unsigned short width;
			unsigned short height;
			unsigned short x;
			unsigned short y;
			unsigned short index;
			float theta;
			*/
		KTPAINT_penheiryouikidaen* da = &hei_daen[hei[i].daen_index];
		dest_hei_daen[dest_hei_daen_max+i].width = da->width;
		dest_hei_daen[dest_hei_daen_max+i].height = da->height;
		dest_hei_daen[dest_hei_daen_max+i].x = da->x;
		dest_hei_daen[dest_hei_daen_max+i].y = da->y;
		dest_hei_daen[dest_hei_daen_max+i].theta = da->theta;
		dest_hei_daen[dest_hei_daen_max+i].index =da->index;
		dest_hei_daen[dest_hei_daen_max+i].heiryouiki_index = i + dest_hei_max;
	}

	for (int i=0;i<hei_part_max;i++) {
		/*
		unsigned short kouten_x;
		unsigned short kouten_y;
		unsigned short keiro_first_index;
		unsigned short keiro_last_index;
		unsigned short kouten_index;
		unsigned short keiro_index;// kouten の何番目の経路かということ　kyokuline_indexは keiro_first_index, keiro_last_index から推測できるはず？
		*/
		dest_hei_part[dest_hei_part_max+i].kouten_x = hei_part[i].kouten_x;
		dest_hei_part[dest_hei_part_max+i].kouten_y = hei_part[i].kouten_y;
		dest_hei_part[dest_hei_part_max+i].keiro_first_index = hei_part[i].keiro_first_index;
		dest_hei_part[dest_hei_part_max+i].keiro_last_index = hei_part[i].keiro_last_index;
		dest_hei_part[dest_hei_part_max+i].kouten_index = hei_part[i].kouten_index;
		dest_hei_part[dest_hei_part_max+i].keiro_index = hei_part[i].keiro_index;
	}

	dest->hei_max = hei_max + dest_hei_max;
	dest->hei_part_max = hei_part_max + dest_hei_part_max;
	dest->hei_daen_max = dest_hei_daen_max + hei_max;

}


void KTPaintSheet::simulationBubbleStep(unsigned short width, unsigned short height, KTPaintNuri* nuri) {

	// 各bubbleと線分が交わっているかどうか判定して
	// 交わっている場合は交わっているしるしをつける
	KTPAINT_bubble* bubbles = nuri->getBubble();
	KTPAINT_kouten* koutens = nuri->getKoutens();
	
	int kouten_max = nuri->getKoutenMax();
	int bubble_max = nuri->getBubbleMax();
	// bubbleの半径が全て同じと仮定する
	int r2 = (int)(bubbles[0].radius*2);
	int bwidth = width /KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU/ r2+1;
	int bheight = bubble_max /bwidth;

	for (int k=0;k<kouten_max;k++) {

		int ll = koutens[k].x / KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU /r2;
		int ll2 = koutens[k].y / KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU /r2;
		bubbles[ll + ll2*bwidth].status = KTPAINT_PENHEIRYOUIKI_BUBBLE_STATUS_ONKOUTEN;


		set<pair<KTPAINT_kouten*, pair<KTPAINT_penkyokuline*, pair<int,int>>>>::iterator it = koutens[k].keiros.begin();
		while(it != koutens[k].keiros.end()) {
			int first_index = (*it).second.second.first;
			int last_index = (*it).second.second.second;
			
			for (int h=first_index; h<=last_index;h++) {
					float px = plines[h].x;
					float py = plines[h].y;
					float px2 = plines[h].x + plines[h].dx;
					float py2 = plines[h].y + plines[h].dy;
					int px_width = px / KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU/r2;
					int py_height = py /KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU/ r2;
					int px_width2 = px2 / KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU/r2;
					int py_height2 = py2 / KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU/r2;
					
					// 大きいほうを2にする
					if (px_width > px_width2) {
						int temp = px_width;
						px_width = px_width2;
						px_width2 = temp;
					}

					if (py_height > py_height2) {
						int temp = py_height;
						py_height = py_height2;
						py_height2 = temp;
					}
					py_height-=2;
					px_width-=2;
					py_height2+=2;
					px_width2+=2;

					if (px_width <0) {
						px_width = 0;
					}
					if (py_height <0) {
						py_height = 0;
					}
					if (px_width >= bwidth) {
						px_width = bwidth-1;
					}
					if(py_height >= bheight) {
						py_height = bheight-1;
					}

				// is_kousaLineAndBubbleの前に検討はずれのものは除外する
				
				
					for (int j=py_height;j <= py_height2;j++) {
							for (int i= px_width;i<=px_width2;i++) {
							int inde = i + j * bwidth;
							if (inde > bubble_max) continue;

							if (nuri->isKousaLineAndBubble(&plines[h],&bubbles[inde])) {
								int ind = KTPAINT_PENHEIRYOUIKI_BUBBLE_FUKUMUTEN_MAX;
								for (int g=0;g<KTPAINT_PENHEIRYOUIKI_BUBBLE_FUKUMUTEN_MAX;g++) {
									if (bubbles[inde].fukumuten[g] == KTPAINT_PENHEIRYOUIKI_BUBBLE_FUKUMUTEN_FUKUMANAI) {
										ind = g;
										break;
									}
								}
								if (ind == KTPAINT_PENHEIRYOUIKI_BUBBLE_FUKUMUTEN_MAX) continue; // 記録しない
								bubbles[inde].fukumuten[ind] = koutens[k].index;
								bubbles[inde].status = KTPAINT_PENHEIRYOUIKI_BUBBLE_STATUS_ONLINE;
							}
						}
					}
				
			}
			it++;
		}
	}

	for (int k=0;k<kouten_max;k++) {

		int ll = koutens[k].x / KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU /r2;
		int ll2 = koutens[k].y / KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU /r2;
		bubbles[ll + ll2*bwidth].status = KTPAINT_PENHEIRYOUIKI_BUBBLE_STATUS_ONKOUTEN;//onlineと同じ
		// ONKOUTENの場合はFUKUMUTENをクリアする
		for (int g=0;g<KTPAINT_PENHEIRYOUIKI_BUBBLE_FUKUMUTEN_MAX;g++) {
			bubbles[ll+ll2*bwidth].fukumuten[g] = KTPAINT_PENHEIRYOUIKI_BUBBLE_FUKUMUTEN_FUKUMANAI;
			if (ll > 0) {
				bubbles[ll-1+ll2*bwidth].fukumuten[g] = KTPAINT_PENHEIRYOUIKI_BUBBLE_FUKUMUTEN_FUKUMANAI;
			}
			if (ll2 >0) {
				bubbles[ll+(ll2-1)*bwidth].fukumuten[g] = KTPAINT_PENHEIRYOUIKI_BUBBLE_FUKUMUTEN_FUKUMANAI;
			}
			if (ll < bwidth-1 ) {
				bubbles[ll+1+ll2*bwidth].fukumuten[g] = KTPAINT_PENHEIRYOUIKI_BUBBLE_FUKUMUTEN_FUKUMANAI;
			}
			if(ll2 < bheight-1) {
				bubbles[ll+(ll2+1)*bwidth].fukumuten[g] = KTPAINT_PENHEIRYOUIKI_BUBBLE_FUKUMUTEN_FUKUMANAI;
			}

		}
	}



	// 次にバブルのラベル付けを行う
	unsigned short bhei_count=1;
	unsigned short label_index = 1;
	KTPAINT_bubblehei* bhei = nuri->getBubbleHeis();
	float radius = KTPAINT_PENHEIRYOUIKI_BUBBLE_RADIUS_SAISHO;
	int width_count = width/KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU/ (int)(2*radius)+1;
	int height_count = height/KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU/ (int)(2*radius)+1;

	for (int j=0;j<height_count; j += 1) {
		int label = 1;
		for (int i=0;i < width_count; i += 1) {
			int inde = i + j * width_count;
			
			if (bubbles[inde].status == KTPAINT_PENHEIRYOUIKI_BUBBLE_STATUS_ONLINE) {
				label++;
				continue;
			}
			bubbles[inde].label_index_yoko = label;
		}
	}

	
		
	for (int i=0;i < width_count; i += 1) {
		int label = 1;
		for (int j=0;j<height_count; j += 1) {
			int inde = i + j * width_count;
			
			if (bubbles[inde].status == KTPAINT_PENHEIRYOUIKI_BUBBLE_STATUS_ONLINE) {
				label++;
				continue;
			}
			bubbles[inde].label_index_tate = label;
		}
	}

	// tateと横のラベル付けは終わったのでheiのラベル付けを行う
	bool is_has=false;
	for (int i=0;i<bubble_max;i++) {
		if (bubbles[i].hei_index) continue;
		if (bubbles[i].x >= width/KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU) continue;
		if (labeldukeBubble(&bubbles[i],label_index,bhei_count,bhei)) {
			bhei[bhei_count].label = label_index;
			label_index++;
			bhei_count++;
			is_has =true;
		//	if (bhei_count > 5) break;
		}
	}
	nuri->setBubbleHeiMax(bhei_count+1);
}

bool KTPaintSheet::labeldukeBubble(KTPAINT_bubble* bubble, unsigned short label_index, unsigned short count, KTPAINT_bubblehei* gheis) {
	if ((bubble->status == KTPAINT_PENHEIRYOUIKI_BUBBLE_STATUS_LABELED) || (bubble->status == KTPAINT_PENHEIRYOUIKI_BUBBLE_STATUS_ONLINE)) return false;
	bubble->status = KTPAINT_PENHEIRYOUIKI_BUBBLE_STATUS_LABELED;
	KTPAINT_bubble* left = bubble;
	KTPAINT_bubble* right = bubble;
	bool is_true=false;
	bubble->hei_index = count;
	while (left->ketugou[1]) {
		left->status = KTPAINT_PENHEIRYOUIKI_BUBBLE_STATUS_LABELED;
		left->hei_index = count;
		if (left->ketugou[1]->status == KTPAINT_PENHEIRYOUIKI_BUBBLE_STATUS_ONLINE) {
			for (int i=0;i<KTPAINT_PENHEIRYOUIKI_BUBBLE_FUKUMUTEN_MAX;i++) {
				if (left->ketugou[1]->fukumuten[i] != KTPAINT_PENHEIRYOUIKI_BUBBLE_FUKUMUTEN_FUKUMANAI) {
					gheis[count].fukumu_kouten_indexs.insert(left->ketugou[1]->fukumuten[i]);
					is_true = true;
					
				}
			}
			break;
		}
		
		
		if (left->label_index_yoko != left->ketugou[1]->label_index_yoko) break;
		
		left = left->ketugou[1];
		
	}
	while (right->ketugou[2]) {
		right->status = KTPAINT_PENHEIRYOUIKI_BUBBLE_STATUS_LABELED;
		right->hei_index = count;
		if (right->ketugou[2]->status == KTPAINT_PENHEIRYOUIKI_BUBBLE_STATUS_ONLINE) {
			for (int i=0;i<KTPAINT_PENHEIRYOUIKI_BUBBLE_FUKUMUTEN_MAX;i++) {
				if (right->ketugou[2]->fukumuten[i] != KTPAINT_PENHEIRYOUIKI_BUBBLE_FUKUMUTEN_FUKUMANAI) {
					gheis[count].fukumu_kouten_indexs.insert(right->ketugou[2]->fukumuten[i]);
					is_true = true;
				}
			}
			break;
		}
	
		if (right->label_index_yoko != right->ketugou[2]->label_index_yoko) break;
		
		right = right->ketugou[2];
	}

	// left と　right には現在タンテンが入っている
	set<KTPAINT_bubble*> has_to_scan;
	KTPAINT_bubble* temp = left;
	KTPAINT_bubble* temp2 = right;
	// ひとつだけ上の方向を考えてhas_to_scanに入れていく
	bool tetu = true;
	while(tetu) {
	if (temp->ketugou[0]) {
		if (temp->ketugou[0]->label_index_tate == temp->label_index_tate) {
			has_to_scan.insert(temp->ketugou[0]);
			KTPAINT_bubble* bb = temp->ketugou[0];
			KTPAINT_bubble* bb_temp = bb;
			// 横方向に進んでいって
			// tempの結合0 があるのでtemp2もあると考えられる
			KTPAINT_bubble* bb_owari = temp2->ketugou[0];

			while((bb_owari != bb_temp) && bb_temp->ketugou[2]) {

				if (bb_temp->label_index_yoko != bb->label_index_yoko) {
					if (bb_temp->ketugou[3]->label_index_tate == bb_temp->label_index_tate) {
						bb = bb_temp;
						has_to_scan.insert(bb_temp);
					} else {


						if (bb_temp->status == KTPAINT_PENHEIRYOUIKI_BUBBLE_STATUS_ONLINE) {
						for (int i=0;i<KTPAINT_PENHEIRYOUIKI_BUBBLE_FUKUMUTEN_MAX;i++) {
						if (bb_temp->fukumuten[i] != KTPAINT_PENHEIRYOUIKI_BUBBLE_FUKUMUTEN_FUKUMANAI) {
							gheis[count].fukumu_kouten_indexs.insert(bb_temp->fukumuten[i]);
							is_true = true;
						}
						}
						
						}





					}
				}
				bb_temp= bb_temp->ketugou[2];
			}
			tetu = false;
		} else  {
			if (temp->ketugou[0]->status == KTPAINT_PENHEIRYOUIKI_BUBBLE_STATUS_ONLINE) {
			for (int i=0;i<KTPAINT_PENHEIRYOUIKI_BUBBLE_FUKUMUTEN_MAX;i++) {
				if (temp->ketugou[0]->fukumuten[i] != KTPAINT_PENHEIRYOUIKI_BUBBLE_FUKUMUTEN_FUKUMANAI) {
					gheis[count].fukumu_kouten_indexs.insert(temp->ketugou[0]->fukumuten[i]);
					is_true = true;
				}
			}
			}
		}
		
	}
	temp = temp->ketugou[2];
	if ((temp == right) || (temp->ketugou[2] == 0)) break;
	}
	temp = left;
	tetu = true;
	while(tetu) {
	if (temp->ketugou[3]) {
		if (temp->ketugou[3]->label_index_tate == temp->label_index_tate) {
			has_to_scan.insert(temp->ketugou[3]);
			KTPAINT_bubble* bb = temp->ketugou[3];
			KTPAINT_bubble* bb_temp = bb;
			// 横方向に進んでいって
			// tempの結合0 があるのでtemp2もあると考えられる
			KTPAINT_bubble* bb_owari = temp2->ketugou[3];

			while((bb_owari != bb_temp) && bb_temp->ketugou[2]) {

				if (bb_temp->label_index_yoko != bb->label_index_yoko) {
					if (bb_temp->ketugou[0]->label_index_tate == bb_temp->label_index_tate) {
						bb = bb_temp;
						has_to_scan.insert(bb_temp);
					} else {


						if (bb_temp->status == KTPAINT_PENHEIRYOUIKI_BUBBLE_STATUS_ONLINE) {
						for (int i=0;i<KTPAINT_PENHEIRYOUIKI_BUBBLE_FUKUMUTEN_MAX;i++) {
						if (bb_temp->fukumuten[i] != KTPAINT_PENHEIRYOUIKI_BUBBLE_FUKUMUTEN_FUKUMANAI) {
							gheis[count].fukumu_kouten_indexs.insert(bb_temp->fukumuten[i]);
							is_true = true;
						}
						}
						}

					}
				}
				bb_temp= bb_temp->ketugou[2];
			}
			tetu =false;
		} else  {
			if (temp->ketugou[3]->status == KTPAINT_PENHEIRYOUIKI_BUBBLE_STATUS_ONLINE) {
			for (int i=0;i<KTPAINT_PENHEIRYOUIKI_BUBBLE_FUKUMUTEN_MAX;i++) {
				if (temp->ketugou[3]->fukumuten[i] != KTPAINT_PENHEIRYOUIKI_BUBBLE_FUKUMUTEN_FUKUMANAI) {
					gheis[count].fukumu_kouten_indexs.insert(temp->ketugou[3]->fukumuten[i]);
					is_true = true;
				}
			}
			}
		}
		
	}
	temp =temp->ketugou[2];
	if ((temp == right) || (temp->ketugou[2] == 0)) break;
	}
	set<KTPAINT_bubble*>::iterator has_to_it = has_to_scan.begin();
	while(has_to_it != has_to_scan.end()) {
		KTPAINT_bubble* bs = *has_to_it;
		if (labeldukeBubble(bs,label_index,count,gheis)) {
			is_true = true;
		}
		has_to_it++;
	}

	
	return is_true;
}


bool KTPaintSheet::calcHeiryouikiPlus(KTPaintNuri* nuri, DWORD color) {
	// クリアしないでそのまま足す



	vector<KTPAINT_koutens*>* koutenss = nuri->getKoutenss();
	KTPAINT_kouten* koutens = nuri->getKoutens();
	static KTPAINT_kouten* temp_koutens[KTPAINT_PENHEIRYOUIKI_MAX];
	int kouten_max = nuri->getKoutenMax();
	static KTPAINT_penheiryouiki temp_hei[KTPAINT_PENHEIRYOUIKI_MAX];
	static bool temp_hei_use[KTPAINT_PENHEIRYOUIKI_MAX];
	static int keiro_indexs[KTPAINT_PENHEIRYOUIKI_MAX];// max_depth
	int max_depth = KTPAINT_PENHEIRYOUIKI_MAX-1;
	oyakokankei.clear();
//	hei_max = 0;
//	hei_part_max = 0;
	static KTPAINT_penheiryouikipart temp_heipart[KTPAINT_PENHEIRYOUIKI_PART_MAX];
	memset(temp_hei,0,sizeof(KTPAINT_penheiryouiki)*KTPAINT_PENHEIRYOUIKI_MAX);
	for (int i=0;i<KTPAINT_PENHEIRYOUIKI_MAX;i++) {
		
		keiro_indexs[i] = 0;
	}

	memset(temp_heipart,0, sizeof(KTPAINT_penheiryouikipart)*KTPAINT_PENHEIRYOUIKI_PART_MAX);
	int temp_hei_count=0;
	int temp_heipart_count = 0;
	bool ans = false;
	// すべての交点に対して走査する
	for (int k=0;k<kouten_max;k++) {

		if (hei_daen_max >= KTPAINT_PENHEIRYOUIKI_DAEN_MAX) continue;
		KTPAINT_kouten* siten = &koutens[k];
		KTPAINT_kouten* kk = siten;
		KTPAINT_kouten* mae_k =NULL;
		temp_heipart_count = 0;
		temp_hei_count = 0;
		int temp_heipart_start_count = temp_heipart_count;
		int temp_heipart_end_count = temp_heipart_count;
		int keiro_ID = 0;
		int keiro_depth=0;
		bool is_has_more_keiro = true;
		for (int i=0;i<KTPAINT_PENHEIRYOUIKI_MAX;i++) {
			keiro_indexs[i] = 0;
			temp_hei_use[i] = true;
			temp_hei[i].hen_id = i;
			//temp_hei[i].daen_calced = 0;
			temp_hei[i].daen_index = 0;
			temp_koutens[i] = 0;
		}
		for (int l=0;l<kouten_max;l++) {
				koutens[l].is_tuuka = false;
		}

		while(is_has_more_keiro && (keiro_ID < KTPAINT_PENHEIRYOUIKI_MAX)) {
			
			/*
			for (int i=0;i<KTPAINT_PENHEIRYOUIKI_MAX;i++) {
				keiro_indexs[i] = 0;
		//	temp_hei_use[i] = true;
			//temp_hei[i].hen_id = i;
			}*/	
			siten->is_tuuka = true;
			temp_koutens[0] = siten;
			// 最初はsitenから始まって　0,0,0,0,0,0,0,0,0... の経路をたどる(ID=1)
			// 次はsitenから始まって　0,0,0,0,0,0,....,0,1 の経路をたどる(ID=2) 1karaIDはじまることに注意
			// 個々の経路が閉集合をなしているかどうかを判定するのは行わなくてはならない
			// 経路がsiten以外でループを作っていたり、同じ経路をたどることになってしまっていたり
			// する場合もIDに数えるが　ループや同じ経路の場合は次の深さを走査しない
			
			int kekka  = getTempHeiFromSitenAndID2(siten,&mae_k,&kk,koutens,keiro_indexs,temp_koutens, &keiro_depth);
			if (kekka == KTPAINT_SHEET_TEMPHEI_NOMOREHEI) {
				is_has_more_keiro = false;
			} else if(kekka == KTPAINT_SHEET_TEMPHEI_HEI) {
			// 閉集合をなしている
				// keiro_indexsに経路のインデックスの情報が入っているのでそれをもとにtemp_heipartに
				// 情報を入れ込む
				irekomiTempHei(siten,koutens,keiro_indexs, temp_hei, &temp_hei_count, temp_heipart, &temp_heipart_count);
				keiro_indexs[keiro_depth]++;

			



			}
			
			keiro_ID++;
		}

		// 入れ込んだ同一点を始点とするきれいな親子関係がある場合は除く
		/*
		for (int i=0;i<temp_hei_count;i++) {
			for (int j=0;j<temp_hei_count;j++) {


				if (i == j) continue;
					if (temp_hei_use[i] || temp_hei_use[j]) {
						// 同じ経路をたどっているかどうか調べる
						// もし同じ経路をたどっていたら片方をオフにする
						static KTPAINT_penheiryouikipart temp_partsdayo[1024];
						memset(temp_partsdayo,0,sizeof(KTPAINT_penheiryouikipart)*1024);
						int temp_c = 0;
						KTPAINT_penheiryouiki* ryou1 = &temp_hei[i];
						KTPAINT_penheiryouiki* ryou2 = &temp_hei[j];

						// ryouiki1について
						addTempPartsCount(temp_partsdayo,&temp_c,1024,ryou1,temp_heipart);
						addTempPartsCount(temp_partsdayo,&temp_c,1024,ryou2,temp_heipart);
						bool test=true;
						for (int i=0;i<temp_c;i++) {
							if (temp_partsdayo[i].kouten_index == 2) {
							// きれいな親子関係ではない
							//test =false;
							}else{
								test = false;
								break;
							}
						}
					if (test) {
						// ryou1 と　ryou2 葉同じ
						if (ryou1->startheiryouiki > ryou2->startheiryouiki) {
							temp_hei_use[j] = false;
						} else {
							temp_hei_use[i] = false;
						}
					}
				}
			}
		}
		*/
		/*
		for (int i=0;i<temp_hei_count;i++) {
			for (int j=0;j<temp_hei_count;j++) {

				for (int p=0;p<temp_hei_count;p++) {

					if (i==j) continue;
					if (i==p) continue;
					if (j==p) continue;

					if ( i > j ) continue;
					if ( j > p ) continue;
					if ( i > p ) continue;
					if (!temp_hei_use[i]) continue;
					if (!temp_hei_use[j]) continue;
					if (!temp_hei_use[p]) continue;
					KTPAINT_penheiryouiki* oyahei=NULL;
					if (this->oyakoKankeiHeiryouiki(&temp_hei[i],&temp_hei[j],&temp_hei[p], &oyahei, nuri, temp_heipart)) {
						// きれいな親子関係があったので除く
						temp_hei_use[oyahei->hen_id] = false;
					}
				}
			}
		}
		*/

		// 実際に使用される領域に加える
		// 同じ経路を通っているループははじく
		// すでに登録されているループ（始点のみ異なる、または回り方が逆なだけ）もはじく

		for (int i=0;i<temp_hei_count;i++) {
			if (temp_hei_use[i]) {
				if (tryTourokuTempHeiToHei(&temp_hei[i], temp_heipart, color,true, 0)) {
					// 一個登録できたら十分
					ans = true;
				}
			}
		}
	}
	return ans;
}



void KTPaintSheet::copyto(KTPaintSheet* dest) {

	//KTPAINT_penheiryouikipart hei_part[KTPAINT_PENHEIRYOUIKI_PART_MAX];
	memset(dest->hei_part,0,sizeof(KTPAINT_penheiryouikipart)*KTPAINT_PENHEIRYOUIKI_PART_MAX);
	for (int i=0;i<hei_part_max;i++) {
		dest->hei_part[i].keiro_first_index = hei_part[i].keiro_first_index;
		dest->hei_part[i].keiro_index = hei_part[i].keiro_index;
		dest->hei_part[i].keiro_last_index = hei_part[i].keiro_last_index;
		dest->hei_part[i].kouten_index = hei_part[i].kouten_index;
		dest->hei_part[i].kouten_x = hei_part[i].kouten_x;
		dest->hei_part[i].kouten_y = hei_part[i].kouten_y;
	}

	//KTPAINT_penheiryouiki hei[KTPAINT_PENHEIRYOUIKI_MAX];
	memset(dest->hei, 0 , sizeof(KTPAINT_penheiryouiki)*KTPAINT_PENHEIRYOUIKI_MAX);
	for (int i=0;i<hei_max;i++) {
		dest->hei[i].color = hei[i].color;
		dest->hei[i].daen_index = hei[i].daen_index;
		dest->hei[i].endheiryouiki = hei[i].endheiryouiki;
		dest->hei[i].hen_id = hei[i].hen_id;
		dest->hei[i].startheiryouiki = hei[i].startheiryouiki;
	}
	//KTPAINT_penheiryouikidaen hei_daen[KTPAINT_PENHEIRYOUIKI_DAEN_MAX];
	memset(dest->hei_daen,0, sizeof(KTPAINT_penheiryouikidaen)*KTPAINT_PENHEIRYOUIKI_DAEN_MAX);
	for (int i=0;i<hei_daen_max;i++) {
		dest->hei_daen[i].height = hei_daen[i].height;
		dest->hei_daen[i].heiryouiki_index = hei_daen[i].heiryouiki_index;
		dest->hei_daen[i].index = hei_daen[i].index;
		dest->hei_daen[i].theta = hei_daen[i].theta;
		dest->hei_daen[i].width = hei_daen[i].width;
		dest->hei_daen[i].x = hei_daen[i].x;
		dest->hei_daen[i].y = hei_daen[i].y;
	}

	//map<pair<int, int>, int> oyakokankei;// KTPAINT_HEI* KTPAINT_HEI* int
	dest->oyakokankei.clear();
	dest->oyakokankei = oyakokankei;
	// int 0 最初 がoya  1 最後がoya 2 不明
	//KTPAINT_penline hei_plines[KTPAINT_SHEET_HEILINE_MAX];
	memset(dest->hei_plines, 0, sizeof(KTPAINT_penline)*KTPAINT_SHEET_HEILINE_MAX);
	for (int i=0;i<hei_pline_max;i++) {
		dest->hei_plines[i].dx = hei_plines[i].dx;
		dest->hei_plines[i].dy = hei_plines[i].dy;
		dest->hei_plines[i].pen_index = hei_plines[i].pen_index;
		dest->hei_plines[i].width_and_nwidth = hei_plines[i].width_and_nwidth;
		dest->hei_plines[i].x = hei_plines[i].x;
		dest->hei_plines[i].y = hei_plines[i].y;
	}

	//KTPAINT_penkyokuline hei_kyoku_plines[KTPAINT_SHEET_KYOKULINE_MAX];
	memset(dest->hei_kyoku_plines, 0,  sizeof(KTPAINT_penkyokuline)*KTPAINT_SHEET_KYOKULINE_MAX);
	for (int i=0;i<hei_kyokupline_max;i++) {
		dest->hei_kyoku_plines[i].end_index = hei_kyoku_plines[i].end_index;
		dest->hei_kyoku_plines[i].kyoku_id = hei_kyoku_plines[i].kyoku_id;
		dest->hei_kyoku_plines[i].kyoku_index = hei_kyoku_plines[i].kyoku_index;
		dest->hei_kyoku_plines[i].start_index = hei_kyoku_plines[i].start_index;
	}

	//KTPAINT_enpituline elines[KTPAINT_SHEET_DUMMY_MAX];
	memset(dest->elines,0, sizeof(KTPAINT_enpituline)*KTPAINT_SHEET_DUMMY_MAX);
	for (int i=0;i<eline_max;i++) {
		dest->elines[i].alpha = elines[i].alpha;
		dest->elines[i].color_index = elines[i].color_index;
		dest->elines[i].dx = elines[i].dx;
		dest->elines[i].dy = elines[i].dy;
		dest->elines[i].x = elines[i].x;
		dest->elines[i].y = elines[i].y;
	}

	// KTPAINT_penline plines[KTPAINT_SHEET_LINE_MAX];
	memset(dest->plines,0,sizeof(KTPAINT_penline)*KTPAINT_SHEET_LINE_MAX);
	for (int i=0;i<pline_max;i++) {
		dest->plines[i].dx = plines[i].dx;
		dest->plines[i].dy = plines[i].dy;
		dest->plines[i].pen_index = plines[i].pen_index;
		dest->plines[i].width_and_nwidth = plines[i].width_and_nwidth;
		dest->plines[i].x = plines[i].x;
		dest->plines[i].y = plines[i].y;
	}
	// KTPAINT_penkyokuline kyoku_plines[KTPAINT_SHEET_KYOKULINE_MAX];
	memset(dest->kyoku_plines, 0, sizeof(KTPAINT_penkyokuline)*KTPAINT_SHEET_KYOKULINE_MAX);
	for (int i=0;i<kyokupline_max;i++) {
		dest->kyoku_plines[i].end_index = kyoku_plines[i].end_index;
		dest->kyoku_plines[i].kyoku_id = kyoku_plines[i].kyoku_id;
		dest->kyoku_plines[i].kyoku_index = kyoku_plines[i].kyoku_index;
		dest->kyoku_plines[i].start_index = kyoku_plines[i].start_index;
	}

	dest->hei_daen_max =  hei_daen_max;
	dest->hei_part_max = hei_part_max;
	dest->hei_max =  hei_max;
	dest->hei_pline_max = hei_pline_max;
	dest->hei_kyokupline_max = hei_kyokupline_max;

	dest->eline_max =  eline_max;
	dest->pline_max = pline_max;
	dest->kyokupline_max = kyokupline_max;
	dest->pline_start_index = pline_start_index;
	dest->pline_end_index = pline_end_index;


}

void KTPaintSheet::calcHeiryouiki(KTPaintNuri* nuri, KTPAINT_bubblehei* ghei, DWORD color, POINT po) {

	vector<KTPAINT_koutens*>* koutenss = nuri->getKoutenss();
	KTPAINT_kouten* koutens = nuri->getKoutens();
	static KTPAINT_kouten* temp_koutens[KTPAINT_PENHEIRYOUIKI_MAX];
	int kouten_max = nuri->getKoutenMax();
	static KTPAINT_penheiryouiki temp_hei[KTPAINT_PENHEIRYOUIKI_MAX];
	static bool temp_hei_use[KTPAINT_PENHEIRYOUIKI_MAX];
	static int keiro_indexs[KTPAINT_PENHEIRYOUIKI_MAX];// max_depth
	int max_depth = KTPAINT_PENHEIRYOUIKI_MAX-1;
	oyakokankei.clear();
	hei_max = 0;
	memset(hei,0,sizeof(KTPAINT_penheiryouiki)*KTPAINT_PENHEIRYOUIKI_MAX);
	memset(hei_part,0,sizeof(KTPAINT_penheiryouikipart)*KTPAINT_PENHEIRYOUIKI_PART_MAX);

	hei_part_max = 0;
	static KTPAINT_penheiryouikipart temp_heipart[KTPAINT_PENHEIRYOUIKI_PART_MAX];
	memset(temp_hei,0,sizeof(KTPAINT_penheiryouiki)*KTPAINT_PENHEIRYOUIKI_MAX);
	for (int i=0;i<KTPAINT_PENHEIRYOUIKI_MAX;i++) {
		
		keiro_indexs[i] = 0;
	}

	memset(temp_heipart,0, sizeof(KTPAINT_penheiryouikipart)*KTPAINT_PENHEIRYOUIKI_PART_MAX);
	int temp_hei_count=0;
	int temp_heipart_count = 0;

	// すべての交点に対して走査する
	for (int k=0;k<kouten_max;k++) {
		if (ghei->fukumu_kouten_indexs.find(k) == ghei->fukumu_kouten_indexs.end()) continue;
		if (hei_daen_max >= KTPAINT_PENHEIRYOUIKI_DAEN_MAX) continue;
		KTPAINT_kouten* siten = &koutens[k];
		KTPAINT_kouten* kk = siten;
		KTPAINT_kouten* mae_k =NULL;
		temp_heipart_count = 0;
		temp_hei_count = 0;
		int temp_heipart_start_count = temp_heipart_count;
		int temp_heipart_end_count = temp_heipart_count;
		int keiro_ID = 0;
		int keiro_depth=0;
		bool is_has_more_keiro = true;
		for (int i=0;i<KTPAINT_PENHEIRYOUIKI_MAX;i++) {
			keiro_indexs[i] = 0;
			temp_hei_use[i] = true;
			temp_hei[i].hen_id = i;
			//temp_hei[i].daen_calced = 0;
			temp_hei[i].daen_index = 0;
			temp_koutens[i] = 0;
		}
		for (int l=0;l<kouten_max;l++) {
				koutens[l].is_tuuka = false;
		}

		while(is_has_more_keiro && (keiro_ID < KTPAINT_PENHEIRYOUIKI_MAX)) {
			
			/*
			for (int i=0;i<KTPAINT_PENHEIRYOUIKI_MAX;i++) {
				keiro_indexs[i] = 0;
		//	temp_hei_use[i] = true;
			//temp_hei[i].hen_id = i;
			}*/	
			siten->is_tuuka = true;
			temp_koutens[0] = siten;
			// 最初はsitenから始まって　0,0,0,0,0,0,0,0,0... の経路をたどる(ID=1)
			// 次はsitenから始まって　0,0,0,0,0,0,....,0,1 の経路をたどる(ID=2) 1karaIDはじまることに注意
			// 個々の経路が閉集合をなしているかどうかを判定するのは行わなくてはならない
			// 経路がsiten以外でループを作っていたり、同じ経路をたどることになってしまっていたり
			// する場合もIDに数えるが　ループや同じ経路の場合は次の深さを走査しない
			
			int kekka  = getTempHeiFromSitenAndID3(siten,&mae_k,&kk,koutens,keiro_indexs,temp_koutens, &keiro_depth, ghei);
			if (kekka == KTPAINT_SHEET_TEMPHEI_NOMOREHEI) {
				is_has_more_keiro = false;
			} else if(kekka == KTPAINT_SHEET_TEMPHEI_HEI) {
			// 閉集合をなしている
				// keiro_indexsに経路のインデックスの情報が入っているのでそれをもとにtemp_heipartに
				// 情報を入れ込む
				irekomiTempHei(siten,koutens,keiro_indexs, temp_hei, &temp_hei_count, temp_heipart, &temp_heipart_count);
				keiro_indexs[keiro_depth]++;

			



			}
			
			keiro_ID++;
		}

		// 入れ込んだ同一点を始点とするきれいな親子関係がある場合は除く
		
		for (int i=0;i<temp_hei_count;i++) {
			for (int j=0;j<temp_hei_count;j++) {


				if (i == j) continue;
					if (temp_hei_use[i] || temp_hei_use[j]) {
						// 同じ経路をたどっているかどうか調べる
						// もし同じ経路をたどっていたら片方をオフにする
						static KTPAINT_penheiryouikipart temp_partsdayo[1024];
						memset(temp_partsdayo,0,sizeof(KTPAINT_penheiryouikipart)*1024);
						int temp_c = 0;
						KTPAINT_penheiryouiki* ryou1 = &temp_hei[i];
						KTPAINT_penheiryouiki* ryou2 = &temp_hei[j];

						// ryouiki1について
						addTempPartsCount(temp_partsdayo,&temp_c,1024,ryou1,temp_heipart);
						addTempPartsCount(temp_partsdayo,&temp_c,1024,ryou2,temp_heipart);
						bool test=true;
						for (int i=0;i<temp_c;i++) {
							if (temp_partsdayo[i].kouten_index == 2) {
							// きれいな親子関係ではない
							//test =false;
							}else{
								test = false;
								break;
							}
						}
					if (test) {
						// ryou1 と　ryou2 葉同じ
						if (ryou1->startheiryouiki > ryou2->startheiryouiki) {
							temp_hei_use[j] = false;
						} else {
							temp_hei_use[i] = false;
						}
					}
				}
			}
		}

		
		for (int i=0;i<temp_hei_count;i++) {
			for (int j=0;j<temp_hei_count;j++) {

				for (int p=0;p<temp_hei_count;p++) {

					if (i==j) continue;
					if (i==p) continue;
					if (j==p) continue;

					if ( i > j ) continue;
					if ( j > p ) continue;
					if ( i > p ) continue;
					if (!temp_hei_use[i]) continue;
					if (!temp_hei_use[j]) continue;
					if (!temp_hei_use[p]) continue;
					KTPAINT_penheiryouiki* oyahei=NULL;
					if (this->oyakoKankeiHeiryouiki(&temp_hei[i],&temp_hei[j],&temp_hei[p], &oyahei, nuri, temp_heipart)) {
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
				if (isInHeiryouikiPline(po.x,po.y,&temp_hei[i],nuri,temp_heipart)) {
				if (tryTourokuTempHeiToHei(&temp_hei[i], temp_heipart, color,true,1)) {
					return;
				}
				}
			}
		}
	}
}

void KTPaintSheet::motomeruJyusin(KTPAINT_penheiryouiki* ryou, KTPAINT_penheiryouikidaen* daen, KTPAINT_penheiryouikipart* parts) {

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
		x += parts[i].kouten_x;
		y += parts[i].kouten_y;
		t_count++;
		for (int k=parts[i].keiro_first_index;k<=parts[i].keiro_last_index;k++) {
			x += this->hei_plines[k].x;
			y += this->hei_plines[k].y;
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
		float rad = (parts[i].kouten_x-x)*(parts[i].kouten_x-x)+
			(parts[i].kouten_y-y)*(parts[i].kouten_y-y);
		if (rad > radius2) {
			radius2 = rad;
		}
		for (int k=parts[i].keiro_first_index;k<=parts[i].keiro_last_index;k++) {
			float rad = (hei_plines[k].x-x)*(hei_plines[k].x-x)+(hei_plines[i].y-y)*(hei_plines[i].y-y);
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

bool KTPaintSheet::addTempPartsCount(KTPAINT_penheiryouikipart* temp_parts, int* temp_c, int maxx, KTPAINT_penheiryouiki* ryou1,
									 KTPAINT_penheiryouikipart* parts) {


	for (int i=ryou1->startheiryouiki; i<=ryou1->endheiryouiki;i++) {
		bool has_already = false;
		KTPAINT_penheiryouikipart* pp = &parts[i];
		int tt=0;
		for (int k=0;k<*temp_c;k++) {

			if ((temp_parts[k].keiro_first_index == pp->keiro_first_index)
				&& (temp_parts[k].keiro_last_index == pp->keiro_last_index)) {
					has_already = true;
					tt =k;
					break;
			}
		}
		if (!has_already) {
			// tukuru
			temp_parts[*temp_c].keiro_first_index = pp->keiro_first_index;
			temp_parts[*temp_c].keiro_last_index = pp->keiro_last_index;
			temp_parts[*temp_c].kouten_index = 1;
			temp_parts[*temp_c].keiro_index = pp->kouten_index;
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


bool KTPaintSheet::oyakoKankeiHeiryouiki(KTPAINT_penheiryouiki* ryou1, KTPAINT_penheiryouiki* ryou2, KTPAINT_penheiryouiki* ryou3,
										 KTPAINT_penheiryouiki** out_oya_ryou, KTPaintNuri* nuri, KTPAINT_penheiryouikipart* parts) {

	// 各領域の重心を求める
	if (!ryou1->daen_index) {
		motomeruJyusinPline(ryou1,&hei_daen[hei_daen_max], parts);
		ryou1->daen_index = hei_daen_max;
		//ryou1->daen_calced = 1;
		hei_daen_max++;
	}
	if (!ryou2->daen_index) {
		motomeruJyusinPline(ryou2,&hei_daen[hei_daen_max], parts);
		ryou2->daen_index = hei_daen_max;
		//ryou2->daen_calced = 1;
		hei_daen_max++;
	}
	if (!ryou3->daen_index) {
		motomeruJyusinPline(ryou3,&hei_daen[hei_daen_max], parts);
		ryou3->daen_index = hei_daen_max;
		//ryou3->daen_calced = 1;
		hei_daen_max++;
	}

	// きれいな親子関係になっているかどうか判定する
	// 1024以下の点で構成される閉領域を対象とする
	static KTPAINT_penheiryouikipart temp_parts[1024];
	memset(temp_parts,0,sizeof(KTPAINT_penheiryouikipart)*1024);
	int temp_c = 0;

	
	// ryouiki1について
	addTempPartsCount(temp_parts,&temp_c,1024,ryou1,parts);
	addTempPartsCount(temp_parts,&temp_c,1024,ryou2,parts);
	addTempPartsCount(temp_parts,&temp_c,1024,ryou3,parts);

	if (!temp_c) return false;
	for (int i=0;i<temp_c;i++) {
		if (temp_parts[i].kouten_index < 2) {
			// きれいな親子関係ではない
			return false;
		}
	}


	/*
	bool test=true;
	for (int i=0;i<temp_c;i++) {
		if (temp_parts[i].kouten_index == 2) {
			// きれいな親子関係ではない
			//test =false;
		}else{
			test = false;
			break;
		}
	}
	if (test) {
		// ryou1 と　ryou2 葉同じ
		if (ryou1->startheiryouiki > ryou2->startheiryouiki) {
		*out_oya_ryou = ryou2;
		} else {
			*out_oya_ryou = ryou1;
		}
		return true;
	}

	memset(temp_parts,0,sizeof(KTPAINT_penheiryouikipart)*1024);
	temp_c = 0;
	addTempPartsCount(temp_parts,&temp_c,1024,ryou1,parts);
	addTempPartsCount(temp_parts,&temp_c,1024,ryou3,parts);
	test=true;
	for (int i=0;i<temp_c;i++) {
		if (temp_parts[i].kouten_index == 2) {
			// きれいな親子関係ではない
			//test =false;
		}else{
			test = false;
			break;
		}
	}
	if (test) {
		// ryou1 と　ryou3 葉同じ
		if (ryou1->startheiryouiki > ryou3->startheiryouiki) {
			*out_oya_ryou = ryou3;
		} else {
			*out_oya_ryou = ryou1;
		}
		return true;
	}

	addTempPartsCount(temp_parts,&temp_c,1024,ryou2,parts);

	if (!temp_c) return false;
	for (int i=0;i<temp_c;i++) {
		if (temp_parts[i].kouten_index < 2) {
			// きれいな親子関係ではない
			return false;
		}
	}
	
	for (int i=0;i<temp_c;i++) {
		if (temp_parts[i].kouten_index > 2) {
			// きれいきれいな親子関係になっているので
			// どちらかひとつを残さないようにする
			if (ryou1->endheiryouiki-ryou1->startheiryouiki == ryou2->endheiryouiki-ryou2->startheiryouiki) {
				if (ryou1->startheiryouiki > ryou2->startheiryouiki) {
					*out_oya_ryou = ryou2;
				} else {
					*out_oya_ryou = ryou1;
				}
				return true;
			} else if (ryou1->endheiryouiki-ryou1->startheiryouiki == ryou3->endheiryouiki - ryou3->startheiryouiki) {
				if (ryou1->startheiryouiki > ryou3->startheiryouiki) {
					*out_oya_ryou = ryou3;
				} else {
					*out_oya_ryou = ryou1;
				}
				return true;
			} else if( ryou2->endheiryouiki - ryou2->startheiryouiki == ryou3->endheiryouiki - ryou3->startheiryouiki) {
				if (ryou2->startheiryouiki > ryou3->startheiryouiki) {
					*out_oya_ryou = ryou3;
				} else {
					*out_oya_ryou = ryou2;
				}
				return true;
			}

		}
	}
	*/

	// きれいな親子関係になっている場合は
	// 親となる閉領域が必ずほかの２領域の点も含むはず
	// いびつな形の場合は重心の中心点がどの領域にも含まれないことが起こりうる
	// その場合は親子関係にないとして３つ全て残す
	//map<pair<KTPAINT_penheiryouiki*, KTPAINT_penheiryouiki*>, int> oyakokankei; 
	// int 0 最初 がoya  1 最後がoya 2 不明 3が身計算
	int te12=3;
/*	if (ryou1->hen_id < ryou2->hen_id) {
		if (oyakokankei.find(pair<int,int>(ryou1->hen_id,ryou2->hen_id)) != oyakokankei.end()) {
			te12 = oyakokankei[pair<int,int>(ryou1->hen_id,ryou2->hen_id)];
		}
	} else {
		if (oyakokankei.find(pair<int,int>(ryou2->hen_id,ryou1->hen_id)) != oyakokankei.end()) {
			te12 = oyakokankei[pair<int,int>(ryou2->hen_id,ryou1->hen_id)];
		}
	}*/

	int te13 =3;
/*	if (ryou1->startheiryouiki < ryou3->startheiryouiki) {
		if (oyakokankei.find(pair<int,int>(ryou1->hen_id,ryou3->hen_id)) != oyakokankei.end()) {
			te13 = oyakokankei[pair<int,int>(ryou1->hen_id,ryou3->hen_id)];
		}
	} else {
		if (oyakokankei.find(pair<int,int>(ryou3->hen_id,ryou1->hen_id)) != oyakokankei.end()) {
			te13 = oyakokankei[pair<int,int>(ryou3->hen_id,ryou1->hen_id)];
		}
	}
*/
	int te23 =3;
/*	if (ryou2->startheiryouiki < ryou3->startheiryouiki) {
		if (oyakokankei.find(pair<int,int>(ryou2->hen_id,ryou3->hen_id)) != oyakokankei.end()) {
			te23 = oyakokankei[pair<int,int>(ryou2->hen_id,ryou3->hen_id)];
		}
	} else {
		if (oyakokankei.find(pair<int,int>(ryou3->hen_id,ryou2->hen_id)) != oyakokankei.end()) {
			te23 = oyakokankei[pair<int,int>(ryou3->hen_id,ryou2->hen_id)];
		}
	}
*/
	if (te12 == 3) {
		// oyakohanteiを行って入れ込む
		
		te12 = karuiOyakoHantei(ryou1,ryou2,nuri,parts);
//		insertOyakoKankei(te12, ryou1,ryou2);
			
	}

	if (te23 == 3) {
		// oyakohanteiを行って入れ込む
		te23 = karuiOyakoHantei(ryou2,ryou3,nuri,parts);
//		insertOyakoKankei(te23,ryou2,ryou3);
		
	}

	if (te13 == 3) {
		// oyakohanteiを行って入れ込む
		te13 = karuiOyakoHantei(ryou1,ryou3,nuri,parts);
//		insertOyakoKankei(te13,ryou1,ryou3);

	}


	if ((te12 == 1) && (te13==1) && (te23==2)) {
		return false;
	}

	if ((te23 == 0) && (te13==0) && (te12==2)) {
		return false;
	}

	if ((te12 == 0) && (te23==1) && (te13==2)) {
		return false;
	}


	if (((te12 ==0) || (te12 == 2)) &&( te13 ==0)) {
		//1 oya
		*out_oya_ryou = ryou1;
		return true;
	}

	if ((te12==0) && ((te13 ==0) || (te13==2))) {
		*out_oya_ryou = ryou1;
		return true;
	}



	if (((te12 == 1)|| te12 ==2) && (te23==0) ) {
		//2 oya
		*out_oya_ryou = ryou2;
		return true;
	}

	if ((te12 == 1) && ((te23==0)|| (te23 == 2)) ) {
		//2 oya
		*out_oya_ryou = ryou2;
		return true;
	}




	if ((te13==1) && ((te23==1) || (te23 ==2))) {
		*out_oya_ryou = ryou3;
		return true;
	}

	if (((te13==1)|| (te13==2)) && (te23==1)) {
		*out_oya_ryou = ryou3;
		return true;
	}





	//きれいな親子関係だけど不明な部分があるので残す
	return false;


}


int KTPaintSheet::karuiOyakoHantei(KTPAINT_penheiryouiki* ryou1, KTPAINT_penheiryouiki* ryou2, KTPaintNuri* nuri, KTPAINT_penheiryouikipart* parts) {
	KTPAINT_penheiryouikidaen* daen1 = &hei_daen[ryou1->daen_index];
	KTPAINT_penheiryouikidaen* daen2 = &hei_daen[ryou2->daen_index];
	bool d1r1 = isInHeiryouiki(daen1->x,daen1->y,ryou1,nuri,parts);
	bool d1r2 = isInHeiryouiki(daen1->x, daen1->y, ryou2, nuri,parts);
	bool d2r1 = isInHeiryouiki(daen2->x,daen2->y,ryou1,nuri,parts);
	bool d2r2 = isInHeiryouiki(daen2->x,daen2->y, ryou2, nuri,parts);
	if (d1r1 && d1r2 && d2r1 && d2r2) return 2;
	if (d2r2 && d1r2) return 1;
	if (d2r1 && d1r1) return 0;
	return 2;
}

bool KTPaintSheet::isInHeiryouiki(unsigned short x, unsigned short y, KTPAINT_penheiryouiki* heid, KTPaintNuri* nuri, KTPAINT_penheiryouikipart* parts) {
	// crossing number algorithm を使用する
	KTPAINT_penline temp_penline_mae;
	KTPAINT_penline temp_penline_ato;
	temp_penline_mae.dx = 0;
	temp_penline_mae.dy = 0;
	temp_penline_mae.pen_index = 0;
	temp_penline_mae.width_and_nwidth = 0;
	temp_penline_mae.x = x;///KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU;
	temp_penline_mae.y = y;///KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU;

	temp_penline_ato.dx = 0;
	temp_penline_ato.dy = 0;
	temp_penline_ato.pen_index = 0;
	temp_penline_ato.width_and_nwidth = 0;
	temp_penline_ato.x = 60000;// 十分に大きな値
	temp_penline_ato.y = y;///KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU;
	
	int kousa_kaisuu =0;
	for (int i=heid->startheiryouiki; i<=heid->endheiryouiki;i++) {
		for (int k=parts[i].keiro_first_index;k<=parts[i].keiro_last_index;k++) {
			KTPAINT_penline tempp;
			tempp.dx = 0;
			tempp.dy = 0;
			tempp.pen_index = 0;
			tempp.width_and_nwidth = 0;
			tempp.x = hei_plines[k].x+hei_plines[k].dx;// 十分に大きな値
			tempp.y = hei_plines[k].y+hei_plines[k].dy;
			if ((hei_plines[k].dx !=0) && (hei_plines[k].dy==0)) {
				continue;
			}
			if (nuri->isKousaLine2(&temp_penline_mae,&temp_penline_ato, &hei_plines[k],&tempp)) {
				kousa_kaisuu++;
			}/* else {
				if (nuri->isKousaLine2(&temp_penline_ato,&temp_penline_mae, &plines[k],&tempp)) {
					kousa_kaisuu++;
				} else {
					if (nuri->isKousaLine2(&temp_penline_mae,&temp_penline_ato, &tempp, &plines[k])) {
						kousa_kaisuu++;
					} else {
							if (nuri->isKousaLine2(&temp_penline_ato,&temp_penline_mae, &tempp, &plines[k])) {
								kousa_kaisuu++;
							}
					}
				}
			}*/
		}
	}
	if ((kousa_kaisuu % 2) == 1) {
		// in
		return true;
	}
	return false;


}

bool KTPaintSheet::isInHeiryouikiPline(unsigned short x, unsigned short y, KTPAINT_penheiryouiki* heid, KTPaintNuri* nuri, KTPAINT_penheiryouikipart* parts) {
	// crossing number algorithm を使用する
	KTPAINT_penline temp_penline_mae;
	KTPAINT_penline temp_penline_ato;
	temp_penline_mae.dx = 0;
	temp_penline_mae.dy = 0;
	temp_penline_mae.pen_index = 0;
	temp_penline_mae.width_and_nwidth = 0;
	temp_penline_mae.x = x;///KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU;
	temp_penline_mae.y = y;///KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU;

	temp_penline_ato.dx = 0;
	temp_penline_ato.dy = 0;
	temp_penline_ato.pen_index = 0;
	temp_penline_ato.width_and_nwidth = 0;
	temp_penline_ato.x = 60000;// 十分に大きな値
	temp_penline_ato.y = y;///KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU;
	
	int kousa_kaisuu =0;
	for (int i=heid->startheiryouiki; i<=heid->endheiryouiki;i++) {
		for (int k=parts[i].keiro_first_index;k<=parts[i].keiro_last_index;k++) {
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
			}/* else {
				if (nuri->isKousaLine2(&temp_penline_ato,&temp_penline_mae, &plines[k],&tempp)) {
					kousa_kaisuu++;
				} else {
					if (nuri->isKousaLine2(&temp_penline_mae,&temp_penline_ato, &tempp, &plines[k])) {
						kousa_kaisuu++;
					} else {
							if (nuri->isKousaLine2(&temp_penline_ato,&temp_penline_mae, &tempp, &plines[k])) {
								kousa_kaisuu++;
							}
					}
				}
			}*/
		}
	}
	if ((kousa_kaisuu % 2) == 1) {
		// in
		return true;
	}
	return false;


}





void KTPaintSheet::motomeruJyusinPline(KTPAINT_penheiryouiki* ryou, KTPAINT_penheiryouikidaen* daen, KTPAINT_penheiryouikipart* parts) {

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
		x += parts[i].kouten_x;
		y += parts[i].kouten_y;
		t_count++;
		for (int k=parts[i].keiro_first_index;k<=parts[i].keiro_last_index;k++) {
			x += this->plines[k].x;
			y += this->plines[k].y;
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
		float rad = (parts[i].kouten_x-x)*(parts[i].kouten_x-x)+
			(parts[i].kouten_y-y)*(parts[i].kouten_y-y);
		if (rad > radius2) {
			radius2 = rad;
		}
		for (int k=parts[i].keiro_first_index;k<=parts[i].keiro_last_index;k++) {
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