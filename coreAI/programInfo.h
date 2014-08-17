#pragma once

class programInfo
{
private:
	const int PORT;
	const char* IPADDR;
	const char* NAME;
	const char* PASSWD;
	const int VERSION;
public:
	programInfo();
	~programInfo();
	const int getPort();
	const char* getIpAddr();
	const char* getName();
	const char* getPasswd();
	const int getVersion();
};
