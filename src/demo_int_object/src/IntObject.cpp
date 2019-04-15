#include "IntObject.h"

void IntObject::Serialize(char * outBuffer)
{
	int index = 0;
	char * dataRef;

	dataRef = (char*)&_value;
	for (int i = 0; i < sizeof(_value) / sizeof(char); i++)
		outBuffer[index++] = dataRef[i];
}

void IntObject::Deserialize(const char * inBuffer)
{
	int index = 0;
	char *dataRef;

	dataRef = (char*)&_value;
	for (int i = 0; i < sizeof(_value) / sizeof(char); i++)
		dataRef[i] = inBuffer[index++];
}

int IntObject::GetObjectSize()
{
	return(
		sizeof(_value)
		);
}
