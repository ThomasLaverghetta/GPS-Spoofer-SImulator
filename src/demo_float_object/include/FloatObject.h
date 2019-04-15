#pragma once

#include "SerialObject.h"

class FloatObject : public SerialObject
{
	float _value;
	unsigned long _TIME; // COPY TIME TESTING!!!!!
public:
	FloatObject() { _value = -1.0f; }

	float GetValue() { return(_value); }
	void SetValue(float value) { _value = value; }

	unsigned long GetTime() {return(_TIME);}	// COPY TIME TESTING!!!!!
	void SetTime(unsigned long Time){_TIME = Time; } // New

	virtual void Serialize(char* outBuffer);
	virtual void Deserialize(const char* inBuffer);
	virtual int GetObjectSize();
};

