#pragma once


#include "Node.h"
#include "FloatObject.h"
#include "GPSObject.h"

#include "tinyxml2.h"
#include "Clock.h"
#include "Keyboard.h"
#include <ros/ros.h> // For access to parameter server

#include <cmath>
#include <cstdio>
#include <signal.h>
#include <unistd.h>
#include <time.h>

class Filter : public Node
{
private:
	GPSObject G[5];

protected:
	void Setup(int argc, char** argv);
	void SetNodeName(int argc, char** argv, std::string& nodeName);
private:
	void AppInit();
	void Process();
	void falseSignal();
	void OnExit();
};