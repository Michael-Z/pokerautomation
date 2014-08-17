/*
$Revision: 48 $
$Date: 2008-10-03 00:35:49 -0700 (Fri, 03 Oct 2008) $
$Author: vananth $
*/
#include "byteBuffer.h"

byteBuffer::byteBuffer(int size)
{
	_writePosition = 0;
	_readPosition = 0;
	_bufferSize = size;
	_buffer = new char[_bufferSize];
}

byteBuffer::~byteBuffer()
{
	clear();
}

void byteBuffer::clear()
{
	delete [] _buffer;
	_writePosition = 0;
	_readPosition = 0;
}

void byteBuffer::writeBytes(const char *data, int size)
{
	memcpy((_buffer+_writePosition),data,size);
	_writePosition += size;
}
void byteBuffer::writeInt32(int data)
{
	int dt = htonl(data);
	writeBytes((const char*)&dt,4);
}
void byteBuffer::readBytes(char *retData, int size)
{
	memcpy(retData,_buffer+_readPosition,size);
	_readPosition += size;
}
int byteBuffer::readInt32()
{
	int ret = 0;
	memcpy(&ret,_buffer+_readPosition,4);
	_readPosition += 4;
	return ret;
}
std::string byteBuffer::readString()
{
	char * cstr = _buffer + _readPosition;
	std::string retStr(cstr);
	_readPosition += retStr.length() + 1;
	return retStr;
}
char* byteBuffer::getWritePtr()
{
	return (_buffer + _writePosition);
}
char* byteBuffer::getReadPtr()
{
	return (_buffer + _readPosition);
}
int byteBuffer::length()
{
	return _bufferSize;
}