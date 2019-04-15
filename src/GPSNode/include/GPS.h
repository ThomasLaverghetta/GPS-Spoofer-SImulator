#pragma once


#include "Node.h"
#include "FloatObject.h"
#include "GPSObject.h"

#include "Clock.h"
#include "Keyboard.h"
#include <ros/ros.h> // For access to parameter server

#include <cmath>
#include <cstdio>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <ctime>

class GPS : public Node
{
private:
	int SatNum, Quality;
	float Lat, Long, sLat, D;
	bool spoofer;
	GPSObject GPSData, GPSInputData;
	int AttackMode;

protected:
	void Setup(int argc, char** argv);
	void SetNodeName(int argc, char** argv, std::string& nodeName);
private:
	void AppInit();
	void Process();
	void falseSignal();
	void trueSignal();
	void sendSignal();
	void OnExit();
	void SetParamGPS();
	// Getting coordinated universal time
	time_t UTC_Time(){return std::time(nullptr);}
};