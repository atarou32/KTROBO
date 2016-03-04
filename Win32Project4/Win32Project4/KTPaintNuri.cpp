#include "KTPaintNuri.h"


KTPaintNuri::KTPaintNuri(void)
{
	kouten_max = 0;
	for (int i=0;i<KTPAINT_SHEET_KOUTEN_MAX;i++) {
		koutens[i].index = i;
	}
}


KTPaintNuri::~KTPaintNuri(void)
{
}

void KTPaintNuri::koutenShoriLinePlus(KTPAINT_penkyokuline* new_line, KTPAINT_penkyokuline* lines, int penkyoku_line_max, KTPAINT_penline* line_infos) {

	int tarinai_size = penkyoku_line_max - koutenss.size();
	int tt = koutenss.size();
	if (tarinai_size) {
		for (int i=0;i<tarinai_size;i++) {
			KTPAINT_koutens* kk = new KTPAINT_koutens();
			kk->bigline_id = i+tt;
			koutenss.push_back(kk);
		}
	}
	
	// ���ȃ��[�v������ꍇ���l���Ĕ��肷��
	kousaKyokusenAndMakeKouten(new_line,new_line,line_infos);

	for (int i=0;i<penkyoku_line_max;i++) {
		
		if (&lines[i] == new_line) continue;
		kousaKyokusenAndMakeKouten(new_line, &lines[i], line_infos);

	}
}

void KTPaintNuri::koutenShori(KTPAINT_penkyokuline* lines, int penkyoku_line_max, KTPAINT_penline* line_infos) {

	vector<KTPAINT_koutens*>::iterator it = koutenss.begin();
	while(it != koutenss.end()) {
		KTPAINT_koutens* kk = *it;
		kk->ks.clear();
		it++;
	}
	for (int i=0;i<KTPAINT_SHEET_KOUTEN_MAX;i++) {
		koutens[i].iti.clear();
		koutens[i].keiros.clear();
		koutens[i].x = 0;
		koutens[i].y = 0;
		koutens[i].index = i;
	}

	int tarinai_size = penkyoku_line_max - koutenss.size();
	int tt = koutenss.size();
	if (tarinai_size) {
		for (int i=0;i<tarinai_size;i++) {
			KTPAINT_koutens* kk = new KTPAINT_koutens();
			kk->bigline_id = i+tt;
			koutenss.push_back(kk);
		}
	}

	kouten_max = 0;
	// ���ȃ��[�v������ꍇ���l���Ĕ��肷��
	for (int i=0;i<penkyoku_line_max;i++) {
		kousaKyokusenAndMakeKouten(&lines[i],&lines[i],line_infos);
	}
	for (int i=0;i<penkyoku_line_max;i++) {
		for (int j=0;j<penkyoku_line_max;j++) {
			if (i == j) continue;
			kousaKyokusenAndMakeKouten(&lines[i], &lines[j], line_infos);
		}
	}
}

void KTPaintNuri::addNewKoutenOfKeiro(int kouten_index, int line1_index, KTPAINT_penkyokuline* bigline1) {


	if (koutenss[bigline1->kyoku_id]->ks.size()) {
		// ���łɂ���Ƃ������Ƃ͂R�_�ȏ゠��Ƃ������ƂȂ̂łQ�ߖT�̓_���擾����
		KTPAINT_kouten* littlek=0;
		KTPAINT_kouten* bigk=0;
		int littlek_index = -1;
		int bigk_index = KTPAINT_SHEET_LINE_MAX;

		vector<KTPAINT_kouten*>::iterator it = koutenss[bigline1->kyoku_id]->ks.begin();
		while(it != koutenss[bigline1->kyoku_id]->ks.end()) {
			KTPAINT_kouten* kl = *it;
			set<pair<KTPAINT_penkyokuline*,int>>::iterator itt = kl->iti.begin();
			while (kl->iti.end() != itt) {
				pair<KTPAINT_penkyokuline*,int> tt = *itt;
				if (tt.first != bigline1) {
					itt++;
					continue;
				}

				int inde = tt.second;
				if ((inde >= littlek_index) && (inde <= line1_index)) {
					littlek_index = inde;
					littlek = kl;
				}
				if ((inde <= bigk_index) && (inde >= line1_index)) {
					bigk_index = inde;
					bigk = kl;
				}
				itt++;
			}
			it++;
		}

		if (littlek_index == line1_index) {
			if (littlek == &koutens[kouten_index]) {
				// ���Ȃ����[�Ɋ܂܂��ꍇ�͌�_���̍X�V���s��Ȃ�
				return;
			}
		}
		if (bigk_index == line1_index) {
			if (bigk == &koutens[kouten_index]) {
				return;
			}
		}


		// littlek �� bigk �̌o�H���̕ύX��
		// k�̌o�H�����쐬����
		// littlek �̌o�H���̕ύX
		// littlek �̏ꍇ�́@bigk�ւ̌o�H(�Ή������ŃN�X�̑@�ہj�����ւ̌o�H�ɕύX�����
		// bigk�̏ꍇ�� littlek�ւ̌o�H�i�Ή�����C���f�b�N�X�̑J�ځj�����ւ̌o�H�ɕύX�����
		{
			set<pair<KTPAINT_kouten*, pair<KTPAINT_penkyokuline*, pair<int,int>>>>::iterator it = littlek->keiros.begin();
			bool is_roop=true;
			while(is_roop) {
				is_roop = false;
				it = littlek->keiros.begin();
				while(it != littlek->keiros.end()) {
					KTPAINT_kouten* kk = (*it).first;
					if ((kk == bigk)) {
						KTPAINT_penkyokuline* pen_line = (*it).second.first;
						int start_inde = (*it).second.second.first;
						int end_inde = (*it).second.second.second;
						if ((start_inde != littlek_index) || ((end_inde) != bigk_index)) {
							it++;
							// �֌W�Ȃ��ʂ̌o�H�Ȃ̂ŃX�L�b�v����
							continue;
						}
						// end_inde������������
						end_inde = line1_index;
						littlek->keiros.erase(it);
						littlek->keiros.insert(pair<KTPAINT_kouten*,pair<KTPAINT_penkyokuline*,pair<int,int>>>(&koutens[kouten_index],
						pair<KTPAINT_penkyokuline*,pair<int,int>>(bigline1,pair<int,int>(start_inde,end_inde))));
						// k �ɂ��o�^����
						koutens[kouten_index].keiros.insert(pair<KTPAINT_kouten*,pair<KTPAINT_penkyokuline*,pair<int,int>>>(littlek,
						pair<KTPAINT_penkyokuline*,pair<int,int>>(bigline1,pair<int,int>(start_inde,end_inde))));
						
						is_roop = true;
						break;
					}	
					it++;
				}
			}
		}

		// ���x��bigk�ɂ��čs��
		{
			set<pair<KTPAINT_kouten*, pair<KTPAINT_penkyokuline*, pair<int,int>>>>::iterator it = bigk->keiros.begin();
			bool is_roop=true;
			while(is_roop) {
				is_roop = false;
				it = bigk->keiros.begin();
				while(it != bigk->keiros.end()) {
					KTPAINT_kouten* kk = (*it).first;
					if (kk == littlek) {
						KTPAINT_penkyokuline* pen_line = (*it).second.first;
						int start_inde = (*it).second.second.first;
						int end_inde = (*it).second.second.second;
						if ((start_inde != littlek_index) || ((end_inde) != bigk_index)) {
							it++;
							// �֌W�Ȃ��ʂ̌o�H�Ȃ̂ŃX�L�b�v����
							continue;
						}
						// statr_inde������������
						start_inde = line1_index;
						bigk->keiros.erase(it);
						bigk->keiros.insert(pair<KTPAINT_kouten*,pair<KTPAINT_penkyokuline*,pair<int,int>>>(&koutens[kouten_index],
						pair<KTPAINT_penkyokuline*,pair<int,int>>(bigline1,pair<int,int>(start_inde,end_inde))));
						// k �ɂ��o�^����
						koutens[kouten_index].keiros.insert(pair<KTPAINT_kouten*,pair<KTPAINT_penkyokuline*,pair<int,int>>>(bigk,
						pair<KTPAINT_penkyokuline*,pair<int,int>>(bigline1,pair<int,int>(start_inde,end_inde))));
						
						is_roop = true;
						break;
					}
					it++;
				}
			}
		}

	} else {
		// �ߖT�̓_���Ȃ��i�͂��߂Č�_�����ꍇ�j��
		// �R�_���ꍞ�� ���̊֐��ł͂Q�_����Ă����i�^���e���j
		koutens[kouten_max].iti.insert(pair<KTPAINT_penkyokuline*,int>(bigline1,bigline1->start_index));
		koutens[kouten_max].keiros.insert(pair<KTPAINT_kouten*,pair<KTPAINT_penkyokuline*,pair<int,int>>>(&koutens[kouten_index],
			pair<KTPAINT_penkyokuline*,pair<int,int>>(bigline1,pair<int,int>(bigline1->start_index,line1_index))));
		koutens[kouten_index].iti.insert(pair<KTPAINT_penkyokuline*,int>(bigline1,line1_index));
		koutens[kouten_index].keiros.insert(pair<KTPAINT_kouten*,pair<KTPAINT_penkyokuline*,pair<int,int>>>(&koutens[kouten_max],
			pair<KTPAINT_penkyokuline*,pair<int,int>>(bigline1,pair<int,int>(bigline1->start_index,line1_index))));
		koutens[kouten_index].keiros.insert(pair<KTPAINT_kouten*,pair<KTPAINT_penkyokuline*,pair<int,int>>>(&koutens[kouten_max+1],
			pair<KTPAINT_penkyokuline*,pair<int,int>>(bigline1,pair<int,int>(line1_index, bigline1->end_index))));
		koutens[kouten_max+1].iti.insert(pair<KTPAINT_penkyokuline*,int>(bigline1,bigline1->end_index));
		koutens[kouten_max+1].keiros.insert(pair<KTPAINT_kouten*,pair<KTPAINT_penkyokuline*,pair<int,int>>>(&koutens[kouten_index],
			pair<KTPAINT_penkyokuline*,pair<int,int>>(bigline1,pair<int,int>(line1_index, bigline1->end_index))));
		koutenss[bigline1->kyoku_id]->ks.push_back(&koutens[kouten_max]);
		koutenss[bigline1->kyoku_id]->ks.push_back(&koutens[kouten_max+1]);
		kouten_max++;
		kouten_max++;
	}






}


void KTPaintNuri::makeKouTen(int line1_index, int line2_index, KTPAINT_penkyokuline* bigline1, KTPAINT_penkyokuline* bigline2, KTPAINT_penline* linesdayo) {


	// ��_������Ƃ�������
	// 2���ɂ�����Q�ߖT�̂S�_���܂��擾����
	// ���łɂ����_���g���邩�ǂ������肷��
	int temp_max = kouten_max;
	int kouten_index = kouten_max;
	if (koutenss[bigline1->kyoku_id]->ks.size()) {
		vector<KTPAINT_kouten*>::iterator it = koutenss[bigline1->kyoku_id]->ks.begin();
		while(it != koutenss[bigline1->kyoku_id]->ks.end()) {
			KTPAINT_kouten* kouten = *it;
			set<pair<KTPAINT_penkyokuline*,int>>::iterator itt = kouten->iti.begin();
			while(itt != kouten->iti.end()) {
				KTPAINT_penkyokuline* p = (*itt).first;
				int inde = (*itt).second;
				if ((p == bigline1) && (inde == line1_index)) {
					kouten_index = kouten->index;
				}
				itt++;
			}
			it++;
		}
	}

	if (koutenss[bigline2->kyoku_id]->ks.size()) {
		vector<KTPAINT_kouten*>::iterator it = koutenss[bigline2->kyoku_id]->ks.begin();
		while(it != koutenss[bigline2->kyoku_id]->ks.end()) {
			KTPAINT_kouten* kouten = *it;
			set<pair<KTPAINT_penkyokuline*,int>>::iterator itt = kouten->iti.begin();
			while(itt != kouten->iti.end()) {
				KTPAINT_penkyokuline* p = (*itt).first;
				int inde = (*itt).second;
				if ((p == bigline2) && (inde == line2_index)) {
					kouten_index = kouten->index;
				}
				itt++;
			}
			it++;
		}
	}



	if (kouten_max+4 > KTPAINT_SHEET_KOUTEN_MAX) return;
	
	if (kouten_index == temp_max) {
		kouten_max++;
	}

	if (bigline1 == bigline2) {
		// �������g�Ō�����Ă���ꍇ�͈������قȂ�(�قȂ�Ȃ��Ă��ނ�������Ȃ�)
		addNewKoutenOfKeiro(kouten_index, line1_index, bigline1);
		koutenss[bigline1->kyoku_id]->ks.push_back(&koutens[kouten_index]);
		koutens[kouten_index].iti.insert(pair<KTPAINT_penkyokuline*,int>(bigline1,line1_index));
		addNewKoutenOfKeiro(kouten_index, line2_index, bigline2);

		/*
		if (kouten_index == temp_max) {
			kouten_index = kouten_max;
			kouten_max++;
		}
		*/

	
	//	koutens[kouten_index].iti.insert(pair<KTPAINT_penkyokuline*,int>(bigline2,line2_index));
	
	//	koutenss[bigline2->kyoku_id]->ks.push_back(&koutens[kouten_index]);
		goto SUPER_POINTDAYO;
	}

	

	addNewKoutenOfKeiro(kouten_index, line1_index, bigline1);
	addNewKoutenOfKeiro(kouten_index, line2_index, bigline2);
	// �Ō�ɏꏊ�̈ʒu������ꍞ��
	koutens[kouten_index].iti.insert(pair<KTPAINT_penkyokuline*,int>(bigline1,line1_index));
	koutens[kouten_index].iti.insert(pair<KTPAINT_penkyokuline*,int>(bigline2,line2_index));
	koutenss[bigline1->kyoku_id]->ks.push_back(&koutens[kouten_index]);
	koutenss[bigline2->kyoku_id]->ks.push_back(&koutens[kouten_index]);
SUPER_POINTDAYO:


	

	KTPAINT_penline* pointA = &linesdayo[line1_index];
	KTPAINT_penline* pointB = &linesdayo[bigline1->end_index];
	if (line1_index < bigline1->end_index) {
		pointB = &linesdayo[line1_index+1];
	}
	KTPAINT_penline* pointC = &linesdayo[line2_index];
	KTPAINT_penline* pointD = &linesdayo[bigline2->end_index];
	if (line2_index < bigline2->end_index) {
		pointD = &linesdayo[line2_index+1];
	}



	double dBunbo	= ( pointB->x - pointA->x )
					* ( pointD->y - pointC->y )
					- ( pointB->y - pointA->y )
					* ( pointD->x - pointC->x );
	if( 0 == dBunbo )
	{	// ���s
		return;
	}

	MYVECTOR2 vectorAC;
	vectorAC.x = pointC->x - pointA->x;
	vectorAC.y = pointC->y - pointA->y;

	double dR = ( ( pointD->y - pointC->y ) * vectorAC.x
		 - ( pointD->x - pointC->x ) * vectorAC.y ) / dBunbo;
	double dS = ( ( pointB->y - pointA->y ) * vectorAC.x
		 - ( pointB->x - pointA->x ) * vectorAC.y ) / dBunbo;

	if (koutens[kouten_index].x) {
		// ���_�łȂ��ꍇ�͂��łɂ���_���g���Ă���̂Œ��V���Ƃ�
		koutens[kouten_index].x = (koutens[kouten_index].x + pointA->x + dR * ( pointB->x - pointA->x ))/2;
		
	} else {
		koutens[kouten_index].x =pointA->x + dR * ( pointB->x - pointA->x );
	}
	if (koutens[kouten_index].y) {
		koutens[kouten_index].y = (koutens[kouten_index].y + pointA->y + dR * ( pointB->y - pointA->y ))/2;
	} else {
		koutens[kouten_index].y =pointA->y + dR * ( pointB->y - pointA->y );
	}
	return;

}


void KTPaintNuri::kousaKyokusenAndMakeKouten(KTPAINT_penkyokuline* line1, KTPAINT_penkyokuline* line2, KTPAINT_penline* lines) {

	int line1_inde_length = line1->end_index - line1->start_index;
	int line2_inde_length = line2->end_index - line2->start_index;

//	int line1_step = line1_inde_length/10 +1;// ������10�������Ȃ� ������������l����
//	int line2_step = line2_inde_length/10 +1;

	for (int i=line1->start_index;i<line1->end_index;i+=1) {
		for (int j=line2->start_index;j<line2->end_index;j+=1) {
			if ((i != j) && (i!= j+1) && (j != i+1) &&( abs(i-j) > 4)) {
				if (isKousaLine(&lines[i],&lines[j])) {
					if (line1->kyoku_id == line2->kyoku_id) {
					//	if ((lines[line1->start_index].x ==0) && (lines[line1->end_index].x ==0)
					//		&&(lines[line1->start_index].y==0) && (lines[line1->end_index].y ==0)) {
					//	} else {
							makeKouTen(i,j,line1,line2,lines);
					//	}
					}else {
							makeKouTen(i,j,line1,line2,lines);
					}
				}
			}
		}
	}
}

bool KTPaintNuri::isKousaLine2(KTPAINT_penline* line1_mae,KTPAINT_penline* line1_ato, KTPAINT_penline* line2_mae, KTPAINT_penline* line2_ato) {

	unsigned short x1 = line1_mae->x;
	unsigned short x2 = line1_ato->x;// + line1->dx;
	unsigned short x3 = line2_mae->x;
	unsigned short x4 = line2_ato->x;// + line2->dx;

	unsigned short y1 = line1_mae->y;
	unsigned short y2 = line1_ato->y;// + line1->dy;
	unsigned short y3 = line2_mae->y;
	unsigned short y4 = line2_ato->y;//+ line2->dy;

	// x check
	if (x1 >= x2 ) {
		if (((x1 < x3) && (x1 < x4)) || ((x2 > x3) && (x2 > x4))) {
			return false;
		}
	} else {
		if (((x2 < x3) && (x2 < x4)) || ((x1 > x3) && (x1 > x4))) {
			return false;
		}
	}

	// ycheck
	if (y1 >= y2 ) {
		if (((y1 < y3) && (y1 < y4)) || ((y2 > y3) && (y2 > y4))) {
			return false;
		}
	} else {
		if (((y2 < y3) && (y2 < y4)) || ((y1 > y3) && (y1 > y4))) {
			return false;
		}
	}

	if (((x1 - x2)*(y3 - y1) + (y1 - y2)* (x1- x3)) * 
		((x1 - x2)*(y4 - y1) + (y1 - y2)* (x1 - x4)) > 0) {
			return false;
	}
	if (((x3 - x4)*(y1- y3) + (y3 - y4) *(x3 - x1)) *
		((x3 - x4)*(y2 - y3) + (y3- y4)*(x3 - x2)) > 0) {
			return false;
	}
	return true;







}


bool KTPaintNuri::isKousaLine(KTPAINT_penline* line1, KTPAINT_penline* line2) {

	unsigned short x1 = line1->x;
	unsigned short x2 = line1->x + line1->dx;
	unsigned short x3 = line2->x;
	unsigned short x4 = line2->x + line2->dx;

	unsigned short y1 = line1->y;
	unsigned short y2 = line1->y + line1->dy;
	unsigned short y3 = line2->y;
	unsigned short y4 = line2->y+ line2->dy;

	// x check
	if (x1 >= x2 ) {
		if (((x1 < x3) && (x1 < x4)) || ((x2 > x3) && (x2 > x4))) {
			return false;
		}
	} else {
		if (((x2 < x3) && (x2 < x4)) || ((x1 > x3) && (x1 > x4))) {
			return false;
		}
	}

	// ycheck
	if (y1 >= y2 ) {
		if (((y1 < y3) && (y1 < y4)) || ((y2 > y3) && (y2 > y4))) {
			return false;
		}
	} else {
		if (((y2 < y3) && (y2 < y4)) || ((y1 > y3) && (y1 > y4))) {
			return false;
		}
	}

	if (((x1 - x2)*(y3 - y1) + (y1 - y2)* (x1- x3)) * 
		((x1 - x2)*(y4 - y1) + (y1 - y2)* (x1 - x4)) > 0) {
			return false;
	}
	if (((x3 - x4)*(y1- y3) + (y3 - y4) *(x3 - x1)) *
		((x3 - x4)*(y2 - y3) + (y3- y4)*(x3 - x2)) > 0) {
			return false;
	}
	return true;

}


void KTPaintNuri::printKouten(KTROBO::Graphics* g,KTPAINT_penline* line_infos) {

	for (int i=0;i<kouten_max;i++) {
		KTPAINT_kouten* kk = &koutens[i];
		if ((kk->x ==0) && (kk->y ==0)) continue;
		KTROBO::Graphics::drawDaen(g,0xFF000000, MYVECTOR3(kk->x/KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU,
				kk->y/KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU,0), 16,16,0);

		set<pair<KTPAINT_penkyokuline*,int>>::iterator it = kk->iti.begin();
		while (it != kk->iti.end()) {
			pair<KTPAINT_penkyokuline*,int> tt = *it;
			KTPAINT_penkyokuline* l = tt.first;
			int inde = tt.second;
			KTPAINT_penline* pp = &line_infos[inde];
			
			KTROBO::Graphics::drawDaen(g,0xFF0000FF, MYVECTOR3(pp->x/KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU,
				pp->y/KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU,0), 5,5,0);
			KTPAINT_penline* pp2 = &line_infos[inde+1];
			KTROBO::Graphics::drawDaen(g,0xFFFF0000, MYVECTOR3(pp2->x/KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU,
				pp2->y/KTROBO_GRAPHICS_RENDER_PEN_SPECIAL_BAIRITU,0), 5,5,0);
		


			it++;
		}
	
	}

}