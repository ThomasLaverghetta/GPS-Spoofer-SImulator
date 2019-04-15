#include "GPSObject.h"

#include <cstdio>

void GPSObject::Serialize(char * outBuffer)
{
	int index = 0;
	char * dataRef;

	dataRef = (char*)&_lat;
	for (int i = 0; i < sizeof(_lat) / sizeof(char); i++)
		outBuffer[index++] = dataRef[i];

	dataRef = (char*)&_long;
	for (int i = 0; i < sizeof(_long) / sizeof(char); i++)
		outBuffer[index++] = dataRef[i];

	dataRef = (char*)&_N;
	for (int i = 0; i < sizeof(_N) / sizeof(char); i++)
		outBuffer[index++] = dataRef[i];

	dataRef = (char*)&_W;
	for (int i = 0; i < sizeof(_W) / sizeof(char); i++)
		outBuffer[index++] = dataRef[i];

	dataRef = (char*)&_SatNum;
	for (int i = 0; i < sizeof(_SatNum) / sizeof(char); i++)
		outBuffer[index++] = dataRef[i];

	dataRef = (char*)&_QualityIndicator;
	for (int i = 0; i < sizeof(_QualityIndicator) / sizeof(char); i++)
		outBuffer[index++] = dataRef[i];

	dataRef = (char*)&_spoofer;
	for (int i = 0; i < sizeof(_spoofer) / sizeof(char); i++)
		outBuffer[index++] = dataRef[i];
	
	dataRef = (char*)&_TIME;
	for (int i = 0; i < sizeof(_TIME) / sizeof(char); i++)
		outBuffer[index++] = dataRef[i];

	dataRef = (char*)&_Alt;
	for (int i = 0; i < sizeof(_Alt) / sizeof(char); i++)
		outBuffer[index++] = dataRef[i];

	dataRef = (char*)&_deltaT;
	for (int i = 0; i < sizeof(_Alt) / sizeof(char); i++)
		outBuffer[index++] = dataRef[i];
}

void GPSObject::Deserialize(const char * inBuffer)
{
	int index = 0;
	char *dataRef;
	dataRef = (char*)&_lat;
	for (int i = 0; i < sizeof(_lat) / sizeof(char); i++)
		dataRef[i] = inBuffer[index++];

	dataRef = (char*)&_long;
	for (int i = 0; i < sizeof(_long) / sizeof(char); i++)
		dataRef[i] = inBuffer[index++];

	dataRef = (char*)&_N;
	for (int i = 0; i < sizeof(_N) / sizeof(char); i++)
		dataRef[i] = inBuffer[index++];

	dataRef = (char*)&_W;
	for (int i = 0; i < sizeof(_W) / sizeof(char); i++)
		dataRef[i] = inBuffer[index++];

	dataRef = (char*)&_SatNum;
	for (int i = 0; i < sizeof(_SatNum) / sizeof(char); i++)
		dataRef[i] = inBuffer[index++];
	
	dataRef = (char*)&_QualityIndicator;
	for (int i = 0; i < sizeof(_QualityIndicator) / sizeof(char); i++)
		dataRef[i] = inBuffer[index++];

	dataRef = (char*)&_spoofer;
	for (int i = 0; i < sizeof(_spoofer) / sizeof(char); i++)
		dataRef[i] = inBuffer[index++];
	
	dataRef = (char*)&_TIME;
	for (int i = 0; i < sizeof(_TIME) / sizeof(char); i++)
		dataRef[i] = inBuffer[index++];

	dataRef = (char*)&_Alt;
	for (int i = 0; i < sizeof(_Alt) / sizeof(char); i++)
		dataRef[i] = inBuffer[index++];

	dataRef = (char*)&_deltaT;
	for (int i = 0; i < sizeof(_Alt) / sizeof(char); i++)
		dataRef[i] = inBuffer[index++];

}

int GPSObject::GetObjectSize()
{	
	return(
		sizeof(_lat)
		+ sizeof(_long)
		+ sizeof(_N)
		+ sizeof(_W)
		+ sizeof(_Alt)
		+ sizeof(_spoofer)
		+ sizeof(_SatNum)
		+ sizeof(_deltaT)
		+ sizeof(_TIME)
		);
}
