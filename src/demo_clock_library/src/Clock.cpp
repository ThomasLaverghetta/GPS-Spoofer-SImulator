#include "Clock.h"


struct timeval requestStart, requestEnd;

void ResetClock()
{
    gettimeofday(&requestStart, NULL);
} 

float ElapsedTime()
{
    gettimeofday(&requestEnd, NULL);

    long seconds  = requestEnd.tv_sec  - requestStart.tv_sec;
    long useconds = requestEnd.tv_usec - requestStart.tv_usec;

    float elapsed = static_cast<float>(((seconds) * 1000 + useconds/1000.0) + 0.5)/1000.0f;
  
	return(elapsed);
}

time_t UTC_Time(){
	return std::time(nullptr);
}

