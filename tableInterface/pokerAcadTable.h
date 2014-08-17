/*
$Revision: 52 $
$Date: 2008-10-03 00:42:35 -0700 (Fri, 03 Oct 2008) $
$Author: vananth $
*/
#pragma once
#include <string>
#include "gameState.h"
#include "pokerTable.h"
#include "clientSocket.h"
#include "win32activeObject.h"
#include "win32Event.h"
#include "evaluate.h"
#include "programInfo.h"
#include "byteBuffer.h"

#define MAX_STRING_SIZE 50
class pokerAcadTable : public pokerTable, public win32ActiveObject
{
private:
	clientSocket iSocket;
	//char* message;
	//int messageLen;
	win32Event * ourTurnEvent;
	programInfo* progInfo;

	const int HEADERSIZE;
public:
	/**
	Enumeration of all messages that come on the network
	*/
	enum ePaMessage {
		ePaMessage_Fold = 0,
		ePaMessage_Call = 1,
		ePaMessage_Raise = 2,
		ePaMessage_Blind = 3,
		ePaMessage_JoinGame = 20,
		ePaMessage_GoodPass = 21,
		ePaMessage_BadPass = 22,
		ePaMessage_BadNick = 24,
		ePaMessage_Action = 30,
		ePaMessage_Chat = 32,
		ePaMessage_QuitGame = 33,
		ePaMessage_Information = 43,
		ePaMessage_NewGame = 50,
		ePaMessage_HoleCards = 51,
		ePaMessage_NewStage = 52,
		ePaMessage_Winners = 53,
		ePaMessage_Chatter = 54,
		ePaMessage_NextToAct = 57,
		ePaMessage_Ping = 60,
		ePaMessage_Pong = 61,
		ePaMessage_Undefined = 62

	};
	pokerAcadTable(programInfo* pInfo, gameState* gs, win32Event* turnEvent);
	~pokerAcadTable();
	/**
	Connects to the game server. If the connection is successful, it spawns a thread that receives
	messages from the server on the socket and updates the gamestate object.
	*/
	int connect();
	/**
	Disconnect from the server. The receive thread is shutdown
	*/
	int disconnect();
	int performBettingAction(eBettingAction action,int raiseAmt = 0);
	int sitOut();
	int sitIn();
private:
	/**
	Add data to the message buffer. message buffer is a private data member of the class
	\param data : Pointer to the source char array of data
	\param size : Size of data to add
	*/
	inline void addToBuffer(const char* data, int size);
	/**
	Add data to the message buffer. message buffer is a private data member of the class
	This function adds 4 bytes that make up the integer data to the message buffer.
	\param data : Integer to add
	*/	
	inline void addToBuffer(int data);
	/**
	Read data from the message buffer. message buffer is a private data member of the class
	\param data : Pointer to the destination char array of data
	\param size : Size of data to read
	*/
	void readFromBuffer(char* data, int size);
	/**
	Read data from the message buffer. message buffer is a private data member of the class
	Reads 4 bytes.
	\return integer representation of four bytes of data
	*/
	int readIntFromBuffer();
	/**
	Read data from the message buffer. message buffer is a private data member of the class
	Reads a string (upto the c-string null character \0)
	\return string representation of the data
	*/
	std::string pokerAcadTable::readStringFromBuffer();
	/**
	Reset the internal message data structure and the counter
	*/
	inline void resetMessage();
	/**
	Initialize the thread. Empty function
	*/
	void initThread();
	/**
	Flush the thread. Empty function
	*/
	void flushThread();
	/**
	The threadable function. Should be started by calling thread.resume().
	*/
	void run();
	/**
	Parse a network message and populate the gamestate object
	\param msgType : The ePaMessage type of the message
	\param msgData : The data payload of the message
	*/
	void parseMessage(ePaMessage msgType, byteBuffer& msgData);
};
