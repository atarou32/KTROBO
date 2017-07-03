#include "KTRoboLockOnSystem.h"
#include "KTRoboArmPositioner.h"

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

#define KTROBO_GETINDEXOFPOINT8_DEFAULT 100000

bool updatepointdayo(int pointindex, int nowindex, float nowyoko, float nowtate, float nowr, MYVECTOR3* pos,
					 MYVECTOR3* temppos, float* ddyoko, float* ddtate, float* ddr, int *inde) {
	bool isyokobig=false;
	bool istatebig = false;
	bool isrbig = false;

	if (nowyoko >= pos->float3.x) {
		isyokobig = true;
	} else {
		isyokobig = false;
	}

	if (nowtate >= pos->float3.z) {
		istatebig = true;
	} else {
		istatebig = false;
	}
	if (abs(nowr) >= abs(pos->float3.y)) {
		isrbig = true;
	} else {
		isrbig = false;
	}

	if ((pointindex == KTROBO_ARMPOINT8_MHS) ||
		(pointindex == KTROBO_ARMPOINT8_MHU) ||
		(pointindex == KTROBO_ARMPOINT8_MMS) ||
		(pointindex == KTROBO_ARMPOINT8_MMU)) {
			if (isrbig) {
		//		return true;
			}
	}

	if ((pointindex == KTROBO_ARMPOINT8_UHS) ||
		(pointindex == KTROBO_ARMPOINT8_UHU) ||
		(pointindex == KTROBO_ARMPOINT8_UMS) ||
		(pointindex == KTROBO_ARMPOINT8_UMU)) {
			if (abs(*ddr - KTROBO_GETINDEXOFPOINT8_DEFAULT) < 0.00001f) {
				 // すでにあるのでtrue
			} else {
		//		return true;
			}
	}

	if (pointindex == KTROBO_ARMPOINT8_MHS) {
		if (!isyokobig && !istatebig && !isrbig) {
			// ddr ddyoko ddtateの計算
			float tempddr = abs(abs(temppos->float3.y) - abs(pos->float3.y));
			float tempddyoko = abs(temppos->float3.x - pos->float3.x);
			float tempddtate = abs(temppos->float3.z - pos->float3.z);

			

			if (tempddr*tempddr + tempddyoko*tempddyoko + tempddtate*tempddtate < *ddr**ddr + *ddyoko * *ddyoko + *ddtate * * ddtate) {
				*ddr = tempddr;
				*ddyoko = tempddyoko;
				*ddtate = tempddtate;
				*inde = nowindex;
			}
			/*
			if ((tempddr <= *ddr+0.0001f) && (tempddyoko <= *ddyoko+0.0001f) && (tempddtate <= *ddtate+0.0001f)) {
				*ddr = tempddr;
				*ddyoko = tempddyoko;
				*ddtate = tempddtate;
				*inde = nowindex;
			}
			*/
		}
	}

	if (pointindex == KTROBO_ARMPOINT8_MHU) {
		if (!isyokobig && istatebig && !isrbig) {
			// ddr ddyoko ddtateの計算
			float tempddr =  abs(abs(temppos->float3.y) - abs(pos->float3.y));//abs(temppos->float3.y - pos->float3.y);
			float tempddyoko = abs(temppos->float3.x - pos->float3.x);
			float tempddtate = abs(temppos->float3.z - pos->float3.z);
			if (tempddr*tempddr + tempddyoko*tempddyoko + tempddtate*tempddtate < *ddr**ddr + *ddyoko * *ddyoko + *ddtate * * ddtate) {
				*ddr = tempddr;
				*ddyoko = tempddyoko;
				*ddtate = tempddtate;
				*inde = nowindex;
			}
			/*
			if ((tempddr <= *ddr+0.0001f) && (tempddyoko <= *ddyoko+0.0001f) && (tempddtate <= *ddtate+0.0001f)) {
				*ddr = tempddr;
				*ddyoko = tempddyoko;
				*ddtate = tempddtate;
				*inde = nowindex;
			}
			*/
		}
	}

	if (pointindex == KTROBO_ARMPOINT8_MMS) {
		if (isyokobig && !istatebig && !isrbig) {
			// ddr ddyoko ddtateの計算
			float tempddr =  abs(abs(temppos->float3.y) - abs(pos->float3.y));//abs(temppos->float3.y - pos->float3.y);
			float tempddyoko = abs(temppos->float3.x - pos->float3.x);
			float tempddtate = abs(temppos->float3.z - pos->float3.z);
			if (tempddr*tempddr + tempddyoko*tempddyoko + tempddtate*tempddtate < *ddr**ddr + *ddyoko * *ddyoko + *ddtate * * ddtate) {
				*ddr = tempddr;
				*ddyoko = tempddyoko;
				*ddtate = tempddtate;
				*inde = nowindex;
			}
			/*
			if ((tempddr <= *ddr+0.0001f) && (tempddyoko <= *ddyoko+0.0001f) && (tempddtate <= *ddtate+0.0001f)) {
				*ddr = tempddr;
				*ddyoko = tempddyoko;
				*ddtate = tempddtate;
				*inde = nowindex;
			}*/
		}
	}

	if (pointindex == KTROBO_ARMPOINT8_MMU) {
		if (isyokobig && istatebig && !isrbig) {
			// ddr ddyoko ddtateの計算
			float tempddr =  abs(abs(temppos->float3.y) - abs(pos->float3.y));//abs(temppos->float3.y - pos->float3.y);
			float tempddyoko = abs(temppos->float3.x - pos->float3.x);
			float tempddtate = abs(temppos->float3.z - pos->float3.z);
			if (tempddr*tempddr + tempddyoko*tempddyoko + tempddtate*tempddtate < *ddr**ddr + *ddyoko * *ddyoko + *ddtate * * ddtate) {
				*ddr = tempddr;
				*ddyoko = tempddyoko;
				*ddtate = tempddtate;
				*inde = nowindex;
			}
			/*
			if ((tempddr <= *ddr+0.0001f) && (tempddyoko <= *ddyoko+0.0001f) && (tempddtate <= *ddtate+0.0001f)) {
				*ddr = tempddr;
				*ddyoko = tempddyoko;
				*ddtate = tempddtate;
				*inde = nowindex;
			}
			*/
		}
	}


	if (pointindex == KTROBO_ARMPOINT8_UHS) {
		if (!isyokobig && !istatebig && isrbig) {
			// ddr ddyoko ddtateの計算
			float tempddr =  abs(abs(temppos->float3.y) - abs(pos->float3.y));//abs(temppos->float3.y - pos->float3.y);
			float tempddyoko = abs(temppos->float3.x - pos->float3.x);
			float tempddtate = abs(temppos->float3.z - pos->float3.z);
			if (tempddr*tempddr + tempddyoko*tempddyoko + tempddtate*tempddtate < *ddr**ddr + *ddyoko * *ddyoko + *ddtate * * ddtate) {
				*ddr = tempddr;
				*ddyoko = tempddyoko;
				*ddtate = tempddtate;
				*inde = nowindex;
			}
			/*
			if ((tempddr <= *ddr+0.0001f) && (tempddyoko <= *ddyoko+0.0001f) && (tempddtate <= *ddtate+0.0001f)) {
				*ddr = tempddr;
				*ddyoko = tempddyoko;
				*ddtate = tempddtate;
				*inde = nowindex;
			}*/
		}
	}

	if (pointindex == KTROBO_ARMPOINT8_UHU) {
		if (!isyokobig && istatebig && isrbig) {
			// ddr ddyoko ddtateの計算
			float tempddr =  abs(abs(temppos->float3.y) - abs(pos->float3.y));//abs(temppos->float3.y - pos->float3.y);
			float tempddyoko = abs(temppos->float3.x - pos->float3.x);
			float tempddtate = abs(temppos->float3.z - pos->float3.z);
			if (tempddr*tempddr + tempddyoko*tempddyoko + tempddtate*tempddtate < *ddr**ddr + *ddyoko * *ddyoko + *ddtate * * ddtate) {
				*ddr = tempddr;
				*ddyoko = tempddyoko;
				*ddtate = tempddtate;
				*inde = nowindex;
			}
			/*
			if ((tempddr <= *ddr+0.0001f) && (tempddyoko <= *ddyoko+0.0001f) && (tempddtate <= *ddtate+0.0001f)) {
				*ddr = tempddr;
				*ddyoko = tempddyoko;
				*ddtate = tempddtate;
				*inde = nowindex;
			}*/
		}
	}

	if (pointindex == KTROBO_ARMPOINT8_UMS) {
		if (isyokobig && !istatebig && isrbig) {
			// ddr ddyoko ddtateの計算
			float tempddr =  abs(abs(temppos->float3.y) - abs(pos->float3.y));//abs(temppos->float3.y - pos->float3.y);
			float tempddyoko = abs(temppos->float3.x - pos->float3.x);
			float tempddtate = abs(temppos->float3.z - pos->float3.z);
			if (tempddr*tempddr + tempddyoko*tempddyoko + tempddtate*tempddtate < *ddr**ddr + *ddyoko * *ddyoko + *ddtate * * ddtate) {
				*ddr = tempddr;
				*ddyoko = tempddyoko;
				*ddtate = tempddtate;
				*inde = nowindex;
			}
			/*
			if ((tempddr <= *ddr+0.0001f) && (tempddyoko <= *ddyoko+0.0001f) && (tempddtate <= *ddtate+0.0001f)) {
				*ddr = tempddr;
				*ddyoko = tempddyoko;
				*ddtate = tempddtate;
				*inde = nowindex;
			}
			*/
		}
	}

	if (pointindex == KTROBO_ARMPOINT8_UMU) {
		if (isyokobig && istatebig && isrbig) {
			// ddr ddyoko ddtateの計算
			float tempddr =  abs(abs(temppos->float3.y) - abs(pos->float3.y));//abs(temppos->float3.y - pos->float3.y);
			float tempddyoko = abs(temppos->float3.x - pos->float3.x);
			float tempddtate = abs(temppos->float3.z - pos->float3.z);

			if (tempddr*tempddr + tempddyoko*tempddyoko + tempddtate*tempddtate < *ddr**ddr + *ddyoko * *ddyoko + *ddtate * * ddtate) {
				*ddr = tempddr;
				*ddyoko = tempddyoko;
   				*ddtate = tempddtate;
				*inde = nowindex;
			}
		}
	}

	return false; // まだ続く
}

bool LockOnSystem::isIn(MYVECTOR3* pos, float rmin, float rmax, float mintate, float maxtate, 
		float minyoko, float maxyoko,float dtate, float dyoko, float dr) {

			ArmPoint8Positioner ap8;
			ArmPoint ap;
			ap.pos = MYVECTOR3(-minyoko,-rmin,-mintate);
			ap8.setPoint(KTROBO_ARMPOINT8_MHS,&ap);
			ap.pos = MYVECTOR3(-minyoko,-rmin,mintate);
			ap8.setPoint(KTROBO_ARMPOINT8_MHU, &ap);
			ap.pos = MYVECTOR3(minyoko, -rmin, -mintate);
			ap8.setPoint(KTROBO_ARMPOINT8_MMS, &ap);
			ap.pos = MYVECTOR3(minyoko, -rmin, mintate);
			ap8.setPoint(KTROBO_ARMPOINT8_MMU, &ap);

			ap.pos = MYVECTOR3(-minyoko,-rmax,-mintate);
			ap8.setPoint(KTROBO_ARMPOINT8_UHS,&ap);
			ap.pos = MYVECTOR3(-minyoko,-rmax,mintate);
			ap8.setPoint(KTROBO_ARMPOINT8_UHU, &ap);
			ap.pos = MYVECTOR3(minyoko, -rmax, -mintate);
			ap8.setPoint(KTROBO_ARMPOINT8_UMS, &ap);
			ap.pos = MYVECTOR3(minyoko, -rmax, mintate);
			ap8.setPoint(KTROBO_ARMPOINT8_UMU, &ap);

			return ap8.isInPoint(pos);

}


int LockOnSystem::getIndexOfPoint8(int pointindex, MYVECTOR3* pos, float rmin, float rmax, float mintate, float maxtate, 
		float minyoko, float maxyoko,float dtate, float dyoko, float dr) {

	OBB ob;
	ob.e = MYVECTOR3(0.04f,0.04f,0.04f);
	MYVECTOR3 temppos(0,0,0);
	
	float ddyoko= KTROBO_GETINDEXOFPOINT8_DEFAULT;
	float ddtate= KTROBO_GETINDEXOFPOINT8_DEFAULT;
	float ddr =   KTROBO_GETINDEXOFPOINT8_DEFAULT;

	float tempr;
	float temptate; // - ~ 1,57
	float tempyoko; // -1.57 ~ 1.57
	static int unko=0;
	int ans=-1;
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
				temppos = ob.c;

				if (updatepointdayo(pointindex, unko, nowyoko,nowtate, nowr, pos, &temppos, &ddyoko, &ddtate, &ddr, &ans)) {
					return ans;
				}



				//g->drawOBB(g,0xFFFFFFFF,world,view,g->getProj(),&ob);
			

				unko++;

				// もしunkoがindexなら返す














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


	return ans;

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