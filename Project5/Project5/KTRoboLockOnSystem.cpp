#include "KTRoboLockOnSystem.h"

using namespace KTROBO;

LockOnSystem::LockOnSystem(void)
{
}


LockOnSystem::~LockOnSystem(void)
{
}

void LockOnSystem::byougaStudyPoint(Graphics* g, MYMATRIX* world,MYMATRIX* view, 
		float rmin, float rmax, float mintate, float maxtate, 
		float minyoko, float maxyoko,float dtate, float dyoko, float dr) { 
	OBB ob;
	ob.e = MYVECTOR3(0.04f,0.04f,0.04f);
	float tempr;
	float temptate; // - ~ 1,57
	float tempyoko; // -1.57 ~ 1.57
	static int unko=0;
	float nowr; // tempr
	float nowtate; // -temptate ~ temptate
	float nowyoko; // -tempyoko ~ tempyoko
	unko=0;
	// 距離に関しては　rmin から　rmax ステップは　dr

	tempr = rmin;
	while (tempr <= rmax) {

		nowr = tempr;
		// temptate tempyoko の計算
		if (abs(nowr - rmin) < 0.00001f) {
			temptate = mintate;
			tempyoko = minyoko;
		} else if (abs(nowr - rmax) < 0.00001f) {
			temptate = maxtate;
			tempyoko = maxyoko;
		} else {
			temptate = mintate* (rmax- nowr)/(rmax-rmin) + maxtate * (nowr - rmin)/(rmax-rmin);
			tempyoko = minyoko* (rmax- nowr)/(rmax-rmin) + maxyoko * (nowr - rmin)/(rmax-rmin);
		}

		nowtate = -temptate;
		nowyoko = -tempyoko;

		while(nowtate <= temptate) {

			nowyoko = -tempyoko;
			while(nowyoko <= tempyoko) {

				ob.c = MYVECTOR3(nowyoko,-1*nowr,nowtate);
				g->drawOBB(g,0xFFFFFFFF,world,view,g->getProj(),&ob);
				unko++;

				if (abs(nowyoko - tempyoko) < 0.00001f) {
					break;
				}
				nowyoko += dyoko;
				if (nowyoko > tempyoko) {
					nowyoko = tempyoko;
				}
			}

			if (abs(nowtate- temptate) < 0.00001) {
				break;
			}

			nowtate += dtate;
			if (nowtate > temptate) {
				nowtate = temptate;
			}
		}




	
		if (abs(tempr- rmax) < 0.00001f) break;

		tempr += dr;
		if (tempr > rmax) {
			tempr = rmax;
		}
	}
}
int LockOnSystem::getStudyPointNum(float rmin, float rmax, float mintate, float maxtate, 
		float minyoko, float maxyoko,float dtate, float dyoko, float dr) {

	float tempr;
	float temptate; // - ~ 1,57
	float tempyoko; // -1.57 ~ 1.57
	static int unko=0;
	float nowr; // tempr
	float nowtate; // -temptate ~ temptate
	float nowyoko; // -tempyoko ~ tempyoko
	unko=0;
	// 距離に関しては　rmin から　rmax ステップは　dr

	tempr = rmin;
	while (tempr <= rmax) {

		nowr = tempr;
		// temptate tempyoko の計算
		if (abs(nowr - rmin) < 0.00001f) {
			temptate = mintate;
			tempyoko = minyoko;
		} else if (abs(nowr - rmax) < 0.00001f) {
			temptate = maxtate;
			tempyoko = maxyoko;
		} else {
			temptate = mintate* (rmax- nowr)/(rmax-rmin) + maxtate * (nowr - rmin)/(rmax-rmin);
			tempyoko = minyoko* (rmax- nowr)/(rmax-rmin) + maxyoko * (nowr - rmin)/(rmax-rmin);
		}

		nowtate = -temptate;
		nowyoko = -tempyoko;

		while(nowtate <= temptate) {

			nowyoko = -tempyoko;
			while(nowyoko <= tempyoko) {

				unko++;

				if (abs(nowyoko - tempyoko) < 0.00001f) {
					break;
				}
				nowyoko += dyoko;
				if (nowyoko > tempyoko) {
					nowyoko = tempyoko;
				}
			}

			if (abs(nowtate- temptate) < 0.00001) {
				break;
			}

			nowtate += dtate;
			if (nowtate > temptate) {
				nowtate = temptate;
			}
		}




	
		if (abs(tempr- rmax) < 0.00001f) break;

		tempr += dr;
		if (tempr > rmax) {
			tempr = rmax;
		}
	}

	return unko;


}
void LockOnSystem::byougaBigStudyPoint(int index, Graphics* g, MYMATRIX* world,MYMATRIX* view, 
	float rmin, float rmax, float mintate, float maxtate, 
	float minyoko, float maxyoko,float dtate, float dyoko, float dr) {




	OBB ob;
	ob.e = MYVECTOR3(1.4f,1.4f,1.4f);
	float tempr;
	float temptate; // - ~ 1,57
	float tempyoko; // -1.57 ~ 1.57
	static int unko=0;
	float nowr; // tempr
	float nowtate; // -temptate ~ temptate
	float nowyoko; // -tempyoko ~ tempyoko
	unko=0;
	// 距離に関しては　rmin から　rmax ステップは　dr

	tempr = rmin;
	while (tempr <= rmax) {

		nowr = tempr;
		// temptate tempyoko の計算
		if (abs(nowr - rmin) < 0.00001f) {
			temptate = mintate;
			tempyoko = minyoko;
		} else if (abs(nowr - rmax) < 0.00001f) {
			temptate = maxtate;
			tempyoko = maxyoko;
		} else {
			temptate = mintate* (rmax- nowr)/(rmax-rmin) + maxtate * (nowr - rmin)/(rmax-rmin);
			tempyoko = minyoko* (rmax- nowr)/(rmax-rmin) + maxyoko * (nowr - rmin)/(rmax-rmin);
		}

		nowtate = -temptate;
		nowyoko = -tempyoko;

		while(nowtate <= temptate) {

			nowyoko = -tempyoko;
			while(nowyoko <= tempyoko) {

				ob.c = MYVECTOR3(nowyoko,-1*nowr,nowtate);
			
				if (unko == index) {
						g->drawOBB(g,0xFFFFFFFF,world,view,g->getProj(),&ob);
				}
				unko++;

				if (abs(nowyoko - tempyoko) < 0.00001f) {
					break;
				}
				nowyoko += dyoko;
				if (nowyoko > tempyoko) {
					nowyoko = tempyoko;
				}
			}

			if (abs(nowtate- temptate) < 0.00001) {
				break;
			}

			nowtate += dtate;
			if (nowtate > temptate) {
				nowtate = temptate;
			}
		}




	
		if (abs(tempr- rmax) < 0.00001f) break;

		tempr += dr;
		if (tempr > rmax) {
			tempr = rmax;
		}
	}














}



MYVECTOR3 LockOnSystem::getPosOfStudyPoint(int index, float rmin, float rmax, float mintate, float maxtate, 
		float minyoko, float maxyoko,float dtate, float dyoko, float dr) {



				OBB ob;
	ob.e = MYVECTOR3(0.04f,0.04f,0.04f);
	float tempr;
	float temptate; // - ~ 1,57
	float tempyoko; // -1.57 ~ 1.57
	static int unko=0;
	float nowr; // tempr
	float nowtate; // -temptate ~ temptate
	float nowyoko; // -tempyoko ~ tempyoko
	unko=0;
	// 距離に関しては　rmin から　rmax ステップは　dr

	tempr = rmin;
	while (tempr <= rmax) {

		nowr = tempr;
		// temptate tempyoko の計算
		if (abs(nowr - rmin) < 0.00001f) {
			temptate = mintate;
			tempyoko = minyoko;
		} else if (abs(nowr - rmax) < 0.00001f) {
			temptate = maxtate;
			tempyoko = maxyoko;
		} else {
			temptate = mintate* (rmax- nowr)/(rmax-rmin) + maxtate * (nowr - rmin)/(rmax-rmin);
			tempyoko = minyoko* (rmax- nowr)/(rmax-rmin) + maxyoko * (nowr - rmin)/(rmax-rmin);
		}

		nowtate = -temptate;
		nowyoko = -tempyoko;

		while(nowtate <= temptate) {

			nowyoko = -tempyoko;
			while(nowyoko <= tempyoko) {

				ob.c = MYVECTOR3(nowyoko,-1*nowr,nowtate);
				//g->drawOBB(g,0xFFFFFFFF,world,view,g->getProj(),&ob);
				if (unko == index) {
					return ob.c;
				}

				unko++;

				if (abs(nowyoko - tempyoko) < 0.00001f) {
					break;
				}
				nowyoko += dyoko;
				if (nowyoko > tempyoko) {
					nowyoko = tempyoko;
				}
			}

			if (abs(nowtate- temptate) < 0.00001) {
				break;
			}

			nowtate += dtate;
			if (nowtate > temptate) {
				nowtate = temptate;
			}
		}




	
		if (abs(tempr- rmax) < 0.00001f) break;

		tempr += dr;
		if (tempr > rmax) {
			tempr = rmax;
		}
	}


	return MYVECTOR3(0,0,0);
}