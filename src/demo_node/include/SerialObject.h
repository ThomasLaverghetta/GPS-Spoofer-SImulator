#pragma once


class SerialObject
{
private:
	bool flagged;
public:
	virtual void Serialize(char* outBuffer) = 0;
	virtual void Deserialize(const char* inBuffer) = 0;
	virtual int GetObjectSize() = 0;

	bool GetFlagged() { return(flagged); }
	bool SetFlagged(bool flag) { flagged = flag; }
};