/*
$Revision: 48 $
$Date: 2008-10-03 00:35:49 -0700 (Fri, 03 Oct 2008) $
$Author: vananth $
*/
#pragma once
#include <string>
#include <winsock2.h>
class byteBuffer
{
private:
	int _writePosition;
	int _readPosition;
	char* _buffer;
	int _bufferSize;
public:
	byteBuffer(int size);
	~byteBuffer();
	void clear();
	void writeInt32(int data);
	void writeBytes(const char* data, int size);
	int readInt32();
	std::string readString();
	void readBytes(char* retData, int size);
	char* getWritePtr();
	char* getReadPtr();
	int length();
};
