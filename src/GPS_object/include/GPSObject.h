#pragma once

#include "SerialObject.h"
#include <string>
#include <ctime>

class GPSObject : public SerialObject
{
	// Latitude, longitude, and altitude of satilate
	float _lat, _long, _Alt;
	
	// format of NMEA
	std::string _N, _W;

	// Satellite number, Signal strength
	int _SatNum, _QualityIndicator;   
	
	// true = spoofer signal / False == true signal
	bool _spoofer;

	// UTC Time
	std::time_t _TIME; 

	// Transmission Time: From Sat to Reciever
	float _deltaT;

public:
	GPSObject() {_lat = 0.0f; _long = 0.0f; _N = "N"; _W = "W";
					_SatNum = 0; _QualityIndicator = 0; _spoofer = false;
					_TIME = 0; _Alt =0.0f; _deltaT = 0.0f;}

	float GetLat() { return(_lat); }
	void SetLat(float value) { _lat = value; }

	float GetLong() { return(_long); }
	void SetLong(float value) { _long = value; }
	
	bool GetSpoofer() { return(_spoofer); }
	void SetSpoofer(bool value) { _spoofer = value; }
	
	int GetGPSNum() { return(_SatNum); }
	void SetGPSNum(float value) { _SatNum = value; }

	int GetQualityInd() { return(_QualityIndicator); }
	void SetQualityInd(float value) { _QualityIndicator = value; }

	float GetAlt() { return(_Alt); }
	void SetAlt(float value) { _Alt = value; }

	float GetDeltaT() { return(_deltaT); } // seconds
	void SetDeltaT(float value) { _deltaT = value; }

	std::time_t GetTime() {return(_TIME);}	// COPY TIME TESTING!!!!!
	void SetTime(std::time_t Time){_TIME = Time; } // New

	virtual void Serialize(char* outBuffer);
	virtual void Deserialize(const char* inBuffer);
	virtual int GetObjectSize();
};

