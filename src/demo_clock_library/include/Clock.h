#include <ctime>
#include <time.h>
#include <cstdio>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>


// --- ResetClock
// Behavior: Initializes a starting point for the clock timer to obtain elapsed time differences
void ResetClock();

// --- ElapsedTime
// Behavior:  Computes the time difference in seconds from the previous call to elapsed time
// Return:  Time difference from last call in seconds
float ElapsedTime();


time_t UTC_Time();