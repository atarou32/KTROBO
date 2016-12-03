#ifndef KTROBOCLOCK_H
#define KTROBOCLOCK_H

#pragma once

namespace KTROBO {
class Clock
{
private:
	long bigtimecycle;
	long second;
	float dmsecond;
	bool is_paused;
public:
	Clock() {
		this->bigtimecycle = 0;
		this->second = 0;
		this->dmsecond = 0;
		is_paused = false;
	}

	Clock(long bigtimecycle, long second, float dmsecond) {
		this->bigtimecycle = bigtimecycle;
		this->second = second;
		this->dmsecond = dmsecond;
		is_paused = false;
	}

	void plus(float millisecond) {
		if (!is_paused) {
			dmsecond += millisecond;
			if (dmsecond > 1000) {
				dmsecond -=1000;
				second+=1;
				if (second > 0xFFFFFFFF) {
					bigtimecycle += 1;
					second -= 0xFFFFFFFF;
				}
			}
		}
	}

	void setPause(bool p) {
		is_paused = p;
	}

	bool getPause() {return is_paused;}
	long getTimeStamp() {
		return bigtimecycle * 0xFFFFFFFF + second;
	}

	long getBigTimeStamp() {
		return bigtimecycle;
	}

	long getSecond() {
		return second;
	}
	float getDMSecond() {return dmsecond;}


	double getBigTimeStampDT(Clock* c) {
		long t  = c->getBigTimeStamp();
		return (t - getBigTimeStamp());
	}

	double getDT(Clock* c) {
		long t = c->getTimeStamp();
		float dm = c->getDMSecond();
		return (t - getTimeStamp()) + (dm - getDMSecond());
	}



};
}

#endif
