#include "MyButukari.h"
#include "math.h"


unsigned int getButukariStatus(MYRECT* rect,MYRECT* korenitaisite) {
	unsigned int ans=0;

	float yokomid = rect->left+(rect->right-rect->left)/2;
	float tatemid = rect->top + (rect->bottom - rect->top )/2;

	float dodaiyokomid = korenitaisite->left+(korenitaisite->right
		-korenitaisite->left)/2;
	float dodaitatemid = korenitaisite->top +(korenitaisite->bottom
		-korenitaisite->top)/2;

	if(yokomid >= dodaiyokomid) {
		ans |= BUTUKARI_RIGHT;
	}else{
		ans |= BUTUKARI_LEFT;
	}
	if(tatemid >= dodaitatemid) {
		ans |= BUTUKARI_BOTTOM;
	}else{
		ans |= BUTUKARI_TOP;
	}

	if(rect->left >= korenitaisite->left) {
		if(rect->right <= korenitaisite->right) {
			if(rect->top >= korenitaisite->top) {
				if(rect->bottom <= korenitaisite->bottom) {
					ans |= BUTUKARI_IN;
				}
			}
		}
	}

	if(rect->left > korenitaisite->right) {
		ans |= BUTUKARI_OUT;
		ans |= BUTUKARI_YOKOOUT;
	}
	if(rect->right < korenitaisite->left) {
		ans |= BUTUKARI_OUT;
		ans |= BUTUKARI_YOKOOUT;
	}
	if(rect->top > korenitaisite->bottom ) {
		ans |= BUTUKARI_OUT;
		ans |= BUTUKARI_TATEOUT;
	}
	if(rect->bottom < korenitaisite->top ) {
		ans |= BUTUKARI_OUT;
		ans |= BUTUKARI_TATEOUT;
	}

	if(rect->bottom <= korenitaisite->bottom) {
		if(rect->top >= korenitaisite->top ) {
			ans |= BUTUKARI_TATEMID;
		}
	}
	if(rect->left >= korenitaisite->left) {
		if(rect->right <= korenitaisite->right) {
			ans |= BUTUKARI_YOKOMID;
		}
	}

	if(rect->left <= korenitaisite->left) {
		if(rect->right >=korenitaisite->left) {
			ans |= BUTUKARI_YOKO;
		}
	}
	if(rect->left <= korenitaisite->right) {
		if(rect->right >= korenitaisite->right) {
			ans |= BUTUKARI_YOKO;
		}
	}
	if(rect->top <= korenitaisite->top ) {
		if(rect->bottom >= korenitaisite->top) {
			ans |= BUTUKARI_TATE;
		}
	}

	if(rect->top <= korenitaisite->bottom) {
		if(rect->bottom >= korenitaisite->bottom) {
			ans |= BUTUKARI_TATE;
		}
	}

	if(ans & BUTUKARI_OUT) {
		if(!(ans & BUTUKARI_TATEOUT) && ans & BUTUKARI_YOKO) {
			ans |= BUTUKARI_BUTUKARI;
		}
		if(!(ans & BUTUKARI_YOKOOUT) && ans & BUTUKARI_TATE) {
			ans |= BUTUKARI_BUTUKARI;
		}
		if(ans & BUTUKARI_TATE && ans & BUTUKARI_YOKO) {
			ans |= BUTUKARI_BUTUKARI;
		}
	}else{
		//OUT‚Å‚Í‚È‚¢
		if(ans & BUTUKARI_TATE || ans & BUTUKARI_YOKO) {
			ans |= BUTUKARI_BUTUKARI;
		}
		if(ans & BUTUKARI_IN) {
			ans |= BUTUKARI_BUTUKARI;
		}
	}

	return ans;
}

bool IS_MYRECT_ZERO(MYRECT* rect) {
	if (rect->left ==0 && rect->right ==0 && rect->top == 0 && rect->bottom == 0) {
		return true;
	}
	return false;
}



unsigned int getButukariStatusPoint(float x,float y,MYRECT* korenitaisite) {
	unsigned int ans=0;

	if(x < korenitaisite->left) {
		ans |= BUTUKARIPOINT_LEFT;
	}

	if(x > korenitaisite->right ) {
		ans |= BUTUKARIPOINT_RIGHT;
	}

	if(y < korenitaisite->top) {
		ans |= BUTUKARIPOINT_TOP;
	}
	if(y > korenitaisite->bottom) {
		ans |= BUTUKARIPOINT_BOTTOM;
	}

	if(x > korenitaisite->left && x < korenitaisite->right) {
		if(y> korenitaisite->top && y < korenitaisite->bottom) {
			ans |= BUTUKARIPOINT_IN;
		}
	}

	return ans;

}

double getKyori(float x,float y,float x2,float y2) {
	return sqrt((double)(x-x2)*(double)(x-x2)+(double)(y-y2)*(double)(y-y2));
}