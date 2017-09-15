#include "KTRoboClock.h"


using namespace KTROBO;
void ClockWatch::startWatch() {

	QueryPerformanceCounter((LARGE_INTEGER*)&this->time_start);


}
double ClockWatch::stopWatch() {


	QueryPerformanceCounter((LARGE_INTEGER*)&this->time_end);
	QueryPerformanceFrequency((LARGE_INTEGER*)&this->timeFreq);
	return (time_end.QuadPart - time_start.QuadPart)/(double)timeFreq.QuadPart*1000;





}