#include "KTPaintNuri.h"


KTPaintNuri::KTPaintNuri(void)
{
	kouten_max = 0;
}


KTPaintNuri::~KTPaintNuri(void)
{
}

bool isKousaKyokusen(KTPAINT_penkyokuline* line1, KTPAINT_penkyokuline* line2, KTPAINT_penline* lines, OUT_ int* line1_index, OUT_ int* line2_index) {








}

bool isKousaLine2(KTPAINT_penline* line1_mae,KTPAINT_penline* line1_ato, KTPAINT_penline* line2_mae, KTPAINT_penline* line2_ato) {

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