#include "programInfo.h"

programInfo::programInfo():
		PORT(27019),
		IPADDR("127.0.0.1"),
		NAME("nemesis"),
		PASSWD("nemesis"),
		VERSION(1)
{
}

programInfo::~programInfo()
{
}
const int programInfo::getPort()
{
	return PORT;
}
const char* programInfo::getIpAddr()
{
	return IPADDR;
}
const char* programInfo::getName()
{
	return NAME;
}
const char* programInfo::getPasswd()
{
	return PASSWD;
}
const int programInfo::getVersion()
{
	return VERSION;
}
