#include "FloatObject.h"

#include <cstdio>

void FloatObject::Serialize(char * outBuffer)
{
	int index = 0;
	char * dataRef;

	dataRef = (char*)&_value;
	for (int i = 0; i < sizeof(_value) / sizeof(char); i++)
		outBuffer[index++] = dataRef[i];

	// COPY TIME TESTING!!!!!
	dataRef = (char*)&_TIME;
	for (int i = 0; i < sizeof(_TIME) / sizeof(char); i++)
		outBuffer[index++] = dataRef[i];	
	// COPY TIME TESTING!!!!!
}

void FloatObject::Deserialize(const char * inBuffer)
{
	int index = 0;
	char *dataRef;

	dataRef = (char*)&_value;
	for (int i = 0; i < sizeof(_value) / sizeof(char); i++)
		dataRef[i] = inBuffer[index++];

	// COPY TIME TESTING!!!!!
	dataRef = (char*)&_TIME;
	for (int i = 0; i < sizeof(_TIME) / sizeof(char); i++)
		dataRef[i] = inBuffer[index++];	
	// COPY TIME TESTING!!!!!
}

int FloatObject::GetObjectSize()
{	
	return(
		sizeof(_value)
		+ sizeof(_TIME)		// COPY TIME TESTING!!!!!
		);
}
