/*
$Revision: 52 $
$Date: 2008-10-03 00:42:35 -0700 (Fri, 03 Oct 2008) $
$Author: vananth $
*/
#include "pokerAcadTable.h"
#include <exception>

pokerAcadTable::pokerAcadTable(programInfo* pInfo, gameState* gs, win32Event* turnEvent):
	pokerTable(gs), HEADERSIZE(8), ourTurnEvent(turnEvent), progInfo(pInfo)
{
}

pokerAcadTable::~pokerAcadTable()
{
	disconnect();
}

int pokerAcadTable::connect()
{
	int iReturn = iSocket.connectToServer(progInfo->getPort(),progInfo->getIpAddr());
	if (iReturn)
		return iReturn;

	int totalDataLength = (int)strlen(progInfo->getName()) + (int)strlen(progInfo->getPasswd()) + sizeof(progInfo->getVersion()) + (int)strlen(progInfo->getName()) + 3;
	
	byteBuffer msgBuffer(HEADERSIZE + totalDataLength);
	
	msgBuffer.writeInt32((int)ePaMessage_JoinGame);
	msgBuffer.writeInt32(totalDataLength);
	msgBuffer.writeBytes(progInfo->getName(),(int)strlen(progInfo->getName())+1);
	msgBuffer.writeBytes(progInfo->getPasswd(),(int)strlen(progInfo->getPasswd())+1);
	msgBuffer.writeInt32(progInfo->getVersion());
	msgBuffer.writeBytes(progInfo->getName(),(int)strlen(progInfo->getName())+1);
	
	iReturn = iSocket.sendData(msgBuffer.getReadPtr(),msgBuffer.length());
	
	if (iReturn) //socket problem
	{
		return iReturn;
	}
	char response[4];
	iReturn = iSocket.recvData(response,4);
	if (iReturn) //socket problem
	{
		return iReturn;
	}
	int * responseInt = reinterpret_cast<int*> (response);
	if ((*responseInt) == (int)ePaMessage_GoodPass)
	{
		isDying = 0;
		thread.resume();
		return 0;
	}
	else
		return 1;
}

int pokerAcadTable::disconnect()
{
	kill();
	#pragma message(Reminder "Need to make sure that run thread has finished before returning")
	return 0;
}
void pokerAcadTable::initThread()
{
}
void pokerAcadTable::flushThread()
{
}
void pokerAcadTable::run()
{
	ePaMessage curMsg = ePaMessage_Undefined;
	char msgHeader[8];
	int retVal;
	while (1)
	{
		if (isDying)
			break;
		retVal = iSocket.recvData(msgHeader,HEADERSIZE);
		if (retVal)
		{
			break;
		}
		int* msgTypeP = reinterpret_cast<int*> (msgHeader);
		int* msgLengthP = msgTypeP + 1;
		byteBuffer msgBuffer(*msgLengthP);
		
		retVal = iSocket.recvData(msgBuffer.getWritePtr(),*msgLengthP);
		if (retVal)
		{
			break;
		}
		parseMessage((ePaMessage)(*msgTypeP),msgBuffer);
	}
	#pragma message(Reminder "Need to send a quit game message to the server")
	iSocket.closeConnection();
}
void pokerAcadTable::parseMessage(pokerAcadTable::ePaMessage msgType, byteBuffer& msgBuffer)
{
	switch (msgType)
	{
	case ePaMessage_NewGame: 
	{
		gameStateP->setBetSize(msgBuffer.readInt32());
		int numPlayers = msgBuffer.readInt32();
		gameStateP->setButtonSeat(msgBuffer.readInt32());
		int ourSeat = msgBuffer.readInt32();
		gameStateP->setGameId(msgBuffer.readInt32());
		std::string name;
		int bankRoll;
		for (int i(0) ; i< numPlayers; i++)
		{
			name = msgBuffer.readString();
			bankRoll = msgBuffer.readInt32();
			int plseat = msgBuffer.readInt32();
			int pldealt = msgBuffer.readInt32();
			gameStateP->addPlayer(name,plseat,(bool)pldealt,bankRoll);
		}
		break;
	}
	case ePaMessage_HoleCards: 
	{
		int seat = msgBuffer.readInt32();;
		std::string holecardS = msgBuffer.readString();
		int holecardI[2]; 
		evaluate::handStringToInt(holecardS,holecardI);
		gameStateP->setHoleCards(holecardI, seat);
		break;
	}
	case ePaMessage_NewStage: 
	{
		int stage = msgBuffer.readInt32();
		std::string boardCards = msgBuffer.readString();
		switch (stage)
		{
		case 1: //Flop
			int flopCards[3];
			evaluate::handStringToInt(boardCards,flopCards);
			gameStateP->setFlopCards(flopCards);
			break;
		case 2: //Turn
			int turnCard;
			evaluate::handStringToInt(boardCards,&turnCard);
			gameStateP->setTurnCard(turnCard);
			break;
		case 3:
			int riverCard;
			evaluate::handStringToInt(boardCards,&riverCard);
			gameStateP->setRiverCard(riverCard);
			break;
		}
		break;
	}
	case ePaMessage_NextToAct: 
	{
		if (msgBuffer.readInt32() == gameStateP->getSeat())
		{
			gameStateP->setAmountToCall(msgBuffer.readInt32());
			#pragma message(Reminder "Need to add min and max bet size here")
			ourTurnEvent->signal();
		}
		break;
	}
	case ePaMessage_Blind:
	{
		int plseat = msgBuffer.readInt32();
		int amnt = msgBuffer.readInt32();
		gameStateP->recordAction(plseat,eBettingAction_Blind,amnt);
		break;
	}
	case ePaMessage_Fold:
	{
		int plseat = msgBuffer.readInt32();
		int amnt = msgBuffer.readInt32();
		gameStateP->recordAction(plseat,eBettingAction_Fold,amnt);
		break;
	}
	case ePaMessage_Call:
	{
		int plseat = msgBuffer.readInt32();
		int amnt = msgBuffer.readInt32();
		gameStateP->recordAction(plseat,eBettingAction_Call,amnt);
		break;
	}
	case ePaMessage_Raise:
	{
		int plseat = msgBuffer.readInt32();
		int amnt = msgBuffer.readInt32();
		gameStateP->recordAction(plseat,eBettingAction_Raise,amnt);
		break;
	}
	case ePaMessage_Winners:
		break;
	case ePaMessage_Ping:
		break;
	case ePaMessage_Information:
		break;
	}
}
int pokerAcadTable::performBettingAction(eBettingAction action,int raiseAmt)
{
	byteBuffer msgBuffer(HEADERSIZE + 4);
	msgBuffer.writeInt32((int)ePaMessage_Action);
	msgBuffer.writeInt32(4);
	switch (action)
	{
	case eBettingAction_Call:
	case eBettingAction_Check:
		msgBuffer.writeInt32(1);
		break;
	case eBettingAction_Fold:
		msgBuffer.writeInt32(0);
		break;
	case eBettingAction_Raise:
		msgBuffer.writeInt32(2);
		break;
	}
	int iReturn = iSocket.sendData(msgBuffer.getReadPtr(),msgBuffer.length());
	return iReturn;
}
int pokerAcadTable::sitOut()
{
	return 1;
}
int pokerAcadTable::sitIn() 
{
	return 1;
}
/*
#pragma region NetworkBuffer
inline void pokerAcadTable::resetMessage()
{
	messageLen = 0;
}
inline void pokerAcadTable::addToBuffer(const char* data, int size)
{
	memcpy((message+messageLen),data,size);
	messageLen += size;
}

inline void pokerAcadTable::addToBuffer(int data)
{
	int dt = htonl(data);
	addToBuffer((const char*)&dt,4);
}
void pokerAcadTable::readFromBuffer(char* data, int size)
{
	memcpy(data,message+messageLen,size);
	messageLen += size;
}
int pokerAcadTable::readIntFromBuffer()
{
	int ret = 0;
	memcpy(&ret,message+messageLen,4);
	messageLen += 4;
	return ret;
}

std::string pokerAcadTable::readStringFromBuffer()
{
	char * cstr = message + messageLen;
	std::string retStr(cstr);
	messageLen += retStr.length() + 1;
	return retStr;
}
#pragma endregion
*/