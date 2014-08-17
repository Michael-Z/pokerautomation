/*
$Revision: 52 $
$Date: 2008-10-03 00:42:35 -0700 (Fri, 03 Oct 2008) $
$Author: vananth $
*/
#include "clientSocket.h"
#include <iostream>

clientSocket::clientSocket()
{
	connectionAlive = false;
	winSockInit = false;
	initWinsockDll();
}

clientSocket::~clientSocket()
{
	closeConnection();
}

int clientSocket::initWinsockDll()
{
	int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult == 0)
		winSockInit = true;
	return iResult;
}

int clientSocket::connectToServer(int port, const char* ipaddr)
{
	int iResult;
	if (!winSockInit)
	{
		iResult = initWinsockDll();
		if (iResult)
			return iResult;
	}
	iSocket = INVALID_SOCKET;
	iSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (iSocket == INVALID_SOCKET)
		return 1;
	memset(&iSockAddr,0, sizeof(iSockAddr));
	iSockAddr.sin_family = AF_INET;
    iSockAddr.sin_port = htons(port);
    iSockAddr.sin_addr.s_addr = inet_addr(ipaddr);
	iResult = connect(iSocket,(SOCKADDR *)&iSockAddr,sizeof(SOCKADDR_IN));
	if (iResult == 0)
		connectionAlive = true;
	return iResult;
}

int clientSocket::sendData(char *data, int size)
{
	int totalBytesSent = 0;
	int curBytesSent = 0;
	while (totalBytesSent < size)
	{
		curBytesSent = send(iSocket,data,size,0);
		if (curBytesSent == SOCKET_ERROR)
		{
			closeConnection();
			return 1;
		}
		totalBytesSent += curBytesSent;
	}
	return 0;
}
int clientSocket::recvData(char *data, int size)
{
	int totalReceivedBytes = 0, curRecvBytes = 0;
	char * curDataPtr = data;
	while (totalReceivedBytes < size)
	{
		curRecvBytes = recv(iSocket,curDataPtr,size,0);
		if (curRecvBytes == SOCKET_ERROR)
		{
			std::cout<<"Error. Socket error!!";
			if (WSAGetLastError() == WSAEMSGSIZE) //buffer space is not enough. return because the buffer has
												  //been filled
			{
				return 0;
			}
			else // some other socket error
			{
				closeConnection();
				return 1;
			}
		}
		else if (curRecvBytes == 0) //Server terminated connection
		{
			closeConnection();
			return 2;
		} 	
		totalReceivedBytes += curRecvBytes;
		curDataPtr += curRecvBytes;
	}
	return 0;
}
bool clientSocket::isConnected()
{
	return connectionAlive;
}
void clientSocket::closeConnection()
{
	closesocket(iSocket);
	WSACleanup();
	connectionAlive = false;
	winSockInit = false;
}
