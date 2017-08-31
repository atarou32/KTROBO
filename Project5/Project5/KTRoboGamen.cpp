#include "KTRoboGamen.h"
using namespace KTROBO;

Gamen::Gamen(void)
{
}


Gamen::~Gamen(void)
{
}


void KoumokuList::clicked(Gamen* gamen, GamenPart* gp, int mouse_x, int mouse_y) {
	if (!is_enable) return;
	vector<Koumoku*>::iterator it = koumokus.begin();
	while(it != koumokus.end()) {
		Koumoku* k = *it;
		if (k->clicked(mouse_x, mouse_y)) {
			k->clickedExe(gamen, gp, this);
			return;
		}
		it++;
	}
}

void KoumokuList::clickedEnter(Gamen* gamen, GamenPart* gp) {
	if (!is_enable) return;
	if (focused_koumoku) {
		focused_koumoku->clickedExe(gamen, gp, this);
	}
}
void KoumokuList::clickedUp() {
	if (!is_enable) return;
	this->cursor--;

	int size = koumokus.size();
	if (size && (cursor < 0 )) {
		cursor = size-1;
	}

	focused_koumoku = koumokus[cursor];
	if (!focused_koumoku->getEnabled() && this->hasEnabledKoumoku()) {
		clickedUp();
	}
	

}
void KoumokuList::clickedDown() {
	if (!is_enable) return;
	int size = koumokus.size();
	if (size) {
		cursor++;
		if (size <= cursor) {
			cursor = 0;
		}

		focused_koumoku = koumokus[cursor];
		if (!focused_koumoku->getEnabled() && this->hasEnabledKoumoku()) {
			clickedDown();
		}
		
	}


}
void KoumokuList::moved(int mouse_x, int mouse_y) {
	if (!is_enable) return;
	int size = koumokus.size();
	for (int i =0;i<size;i++) {
		Koumoku* k = koumokus[i];
		if (k->moved(mouse_x, mouse_y)) {
			cursor = i;
			focused_koumoku = k;
			return;
		}
	}
}

void KoumokuList::setVisible(bool t) {
	is_visible = t;

}

void KoumokuList::setEnable(bool t) {
	is_enable = t;

	int size = koumokus.size();
	for (int i=0;i<size;i++) {
		koumokus[i]->setEnabled(t);
	}

}

void KoumokuList::setEnableKoumoku(int index) {
	if (!is_enable) return;
	int size = koumokus.size();
	if ((index > 0) && (size > index)) {
		koumokus[index]->setEnabled(true);
	}
}
void KoumokuList::setDisableKoumoku(int index) {
	if (!is_enable) return;
	int size = koumokus.size();
	if ((index > 0) && (size > index)) {
		koumokus[index]->setEnabled(false);
	}

}

void KoumokuList::setSize(int x, int y, int width, int height) {
	place.left = x;
	place.top = y;
	place.right = x + width;
	place.bottom = y + height;
	// çÄñ⁄Ç™Ç†ÇÈèÍçáÇÕÇªÇÃÉTÉCÉYÇ‡ê›íËÇ∑ÇÈ
	int size = koumokus.size();
	if (size) {
		float dheight = height / (float)size;

		for (int i = 0 ; i<size;i++) {
			koumokus[i]->setSize(x,y + dheight * i , width, y + dheight*(i+1));
		}
	}
}

void KoumokuList::byouga(Graphics* g, GUI* gui, float dsecond, int stamp) {
	if (!is_visible) return;

	int size = koumokus.size();
	for (int i=0;i<size;i++) {
		Koumoku* k  = koumokus[i];
		k->byouga(g,gui,dsecond,stamp);
	}

	if (focused_koumoku) {
		focused_koumoku->focusedByouga(g,gui,dsecond, stamp);
	}
}





bool Koumoku::clicked(int mouse_x, int mouse_y) {
	if (!is_enable) return false;
	if (getButukariStatusPoint(mouse_x,mouse_y, &place) & BUTUKARIPOINT_IN) {
		return true;
	}
	return false;
}

bool Koumoku::moved(int mouse_x, int mouse_y) {
	if (!is_enable) return false;
	if (getButukariStatusPoint(mouse_x,mouse_y, &place) & BUTUKARIPOINT_IN) {
		return true;
	}
	return false;
}

void Koumoku::setEnabled(bool t) {
	this->is_enable = t;
}

void Koumoku::setSize(int x, int y, int width, int height) {
	place.left = x;
	place.right = x + width;
	place.top = y;
	place.bottom = y + height;

}

int Koumoku::getID() {
	return clicked_id;
}