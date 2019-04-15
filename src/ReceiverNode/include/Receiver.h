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

struct Cartesion
{
	float x,y,z;
	time_t YTime;
	GPSObject GPS;
};
struct LinearCalc
{
	float x,y,z,mag;
};
class Receiver : public Node 
{
private:
	Cartesion G[3];
	LinearCalc V[3], M[3], K[3];
	float D[3], Z[9][3], At[3][9], b[9], mat[3][3], determinant;
	float Inverse[3][3], mat2[3][9], m_[3];
	float Pos_P[3], Neg_P[3], lat, Long, alt;
protected:

	void Setup(int argc, char** argv);
	void SetNodeName(int argc, char** argv, std::string& nodeName);
private:
	void AppInit();
	void Process();
	void GPSPosition();
	void OnExit();
};