/*
$Revision: 35 $
$Date: 2008-09-10 01:49:58 -0700 (Wed, 10 Sep 2008) $
$Author: vananth $
*/
#pragma once
#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

class clientSocket
{
private:
	WSADATA wsaData;
	SOCKET iSocket;
	SOCKADDR_IN iSockAddr;
	bool connectionAlive;
	bool winSockInit;
public:
	clientSocket();
	~clientSocket();
	/**
	Connect via tcp ip to the server
	\param port : Connect to this port
	\param ipaddr : Connect to this ip address
	\return : Status (0 = success)
	*/
	int connectToServer(int port, const char* ipaddr);
	/**
	Close the active connection and unload the winsock2 dll
	*/
	void closeConnection();
	/**
	Check if the connection is still alive
	*/
	bool isConnected();
	/**
	Send data
	*/
	int sendData(char * data, int size);
	/**
	Receive data
	*/
	int recvData(char * data, int size);

private:
	int initWinsockDll();
};
