#pragma once

#include "SerialObject.h"

class IntObject : public SerialObject
{
	int _value;
public:
	IntObject(){ _value = -1; }
	
	int GetValue() { return(_value); }
	void SetValue(int value) { _value = value; }

	virtual void Serialize(char* outBuffer);
	virtual void Deserialize(const char* inBuffer);
	virtual int GetObjectSize();
};

