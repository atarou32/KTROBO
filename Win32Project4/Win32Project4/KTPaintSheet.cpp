#include "KTPaintSheet.h"
#include "math.h"


KTPaintSheet::KTPaintSheet(void)
{
	eline_max = 0;
	pline_max = 0;
	memset(elines,0,sizeof(KTPAINT_enpituline)*KTPAINT_SHEET_LINE_MAX);
	memset(plines,0,sizeof(KTPAINT_penline)*KTPAINT_SHEET_LINE_MAX);
	memset(kyoku_plines, 0, sizeof(KTPAINT_penkyokuline)*KTPAINT_SHEET_KYOKULINE_MAX);
	kyokupline_max = 0;
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

