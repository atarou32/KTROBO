#include "KTRoboClock.h"
#include <WinSock2.h>

using namespace KTROBO;
void ClockWatch::startWatch() {

	QueryPerformanceCounter((LARGE_INTEGER*)&this->time_start);


}
double ClockWatch::stopWatch() {


	QueryPerformanceCounter((LARGE_INTEGER*)&this->time_end);
	QueryPerformanceFrequency((LARGE_INTEGER*)&this->timeFreq);
	return (time_end.QuadPart - time_start.QuadPart)/(double)timeFreq.QuadPart*1000;





}



void ClockWatch::wait(unsigned int microsecond) {
	
	timeval tt;
	tt.tv_sec = 0;
	tt.tv_usec = microsecond*1000;
	select(0,NULL,NULL,NULL,&tt);
}

void ClockWatches::startWatch(int index) {
	if (index >=0 && index < 10) {
		watches[index].startWatch();
	}
}

void ClockWatches::stopWatch(int index) {
	if (index >= 0 && index < 10) {
		times[index] = watches[index].stopWatch();
	}
}