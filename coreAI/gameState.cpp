/*
$Revision: 52 $
$Date: 2008-10-03 00:42:35 -0700 (Fri, 03 Oct 2008) $
$Author: vananth $
*/
#include "gameState.h"

gameState::gameState(programInfo* pi): pInfo(pi)
{
}

gameState::~gameState()
{
}


void gameState::getHoleCards(int *holeCardArr)
{
	ourPlayer->getHoleCards(holeCardArr);
}
int gameState::getBoardCards(int *boardCardArr)
{
	switch (currentStreet)
	{
	case eStreet_PreFlop:
		return 0;
	case eStreet_Flop:
		boardCardArr[0] = flopCards[0];
		boardCardArr[1] = flopCards[1];
		boardCardArr[2] = flopCards[2];
		return 3;
	case eStreet_Turn:
		boardCardArr[0] = flopCards[0];
		boardCardArr[1] = flopCards[1];
		boardCardArr[2] = flopCards[2];
		boardCardArr[3] = turnCard;
		return 4;
	case eStreet_River:
		boardCardArr[0] = flopCards[0];
		boardCardArr[1] = flopCards[1];
		boardCardArr[2] = flopCards[2];
		boardCardArr[3] = turnCard;
		boardCardArr[4] = riverCard;
		return 5;
	}
	#ifdef DEBUGASSERT
		std::cout<<"Error getBoardCards: Unknown street \n";
	#endif
	return -1;
}
eStreet gameState::getCurrentStreet()
{
	return currentStreet;
}
int gameState::getPotSize()
{
	return potSize;
}
int gameState::getAmountToCall()
{
	return amountToCall;
}
int gameState::getPosition()
{
	#pragma message(Reminder "This does not take folded/sitting out players into consideration")
	int pos = getSeat() - buttonSeat;
	if (pos < 0)
	{
		pos = startingPlayers - buttonSeat + getSeat();
	}
	return pos;
}
int gameState::getSeat()
{
	return ourPlayer->getSeat();
}
int gameState::getNumPlayersDealt()
{
	return startingPlayers;
}
int gameState::getRemainingPlayers()
{
	int foldedPlayers(0);
	for (playerIter = gamePlayers.begin(); playerIter != gamePlayers.end();playerIter++)
	{
		if (playerIter->inGame() && playerIter->isFolded())
			foldedPlayers++;
	}
	return (startingPlayers - foldedPlayers);
}


player* gameState::getPlayer(int seat)
{
	for (playerIter = gamePlayers.begin(); playerIter != gamePlayers.end();playerIter++)
	{
		if (playerIter->getSeat() == seat)
			return &*playerIter;
	}
#ifdef DEBUGASSERT
	std::cout<<"Error getPlayer: Player not found\n";
#endif
	return NULL;
}

int gameState::getNumSeats()
{
	return numSeats;
}
void gameState::setBetSize(int size)
{
	betSize = size;
}

void gameState::setButtonSeat(int seat)
{
	#ifdef DEBUGASSERT
		if (seat > 8)
			std::cout<<"Error in setButtonSeat: Seat is > 8\n";
	#endif
	buttonSeat = seat;
}
void gameState::setSeat(int seat)
{
	ourPlayer->setSeat(seat);
}
void gameState::setGameId(int id)
{
	gameId = id;
	startingPlayers = 0; //No one in the game yet
	numSeats = 0;
	potSize = 0;
	ourPlayer = NULL;
	gamePlayers.clear();
	setCurrentStreet(eStreet_PreFlop);
}
void gameState::setHoleCards(int *holeCardArr)
{
	ourPlayer->setHoleCards(holeCardArr);
}
void gameState::setHoleCards(int* holeCardArr, int seat)
{
	for (playerIter = gamePlayers.begin(); playerIter != gamePlayers.end();playerIter++)
	{
		if (playerIter->getSeat() == seat)
			setHoleCards(holeCardArr);
	}
}
void gameState::setFlopCards(int *flopCardArr)
{
	flopCards[0] = flopCardArr[0];
	flopCards[1] = flopCardArr[1];
	flopCards[2] = flopCardArr[2];
	setCurrentStreet(eStreet_Flop);
}
void gameState::setTurnCard(int card)
{
	turnCard = card;
	setCurrentStreet(eStreet_Turn);
}
void gameState::setRiverCard(int card)
{
	riverCard = card;
	setCurrentStreet(eStreet_River);
}
void gameState::setCurrentStreet(eStreet street)
{
	currentStreet = street;
}
void gameState::setAmountToCall(int amount)
{
	amountToCall = amount;
}
void gameState::addPlayer(std::string name, int seat, bool wasDealt, int bankRoll)
{
	numSeats++;
	player pl;
	pl.setGameId(gameId);
	pl.setName(name);
	pl.setSeat(seat);
	if (wasDealt)
	{
		pl.setIsPlaying();
		startingPlayers++;
	}
	pl.setBankRoll(bankRoll);
	gamePlayers.push_back(pl);
	if (strcmp(name.c_str(),pInfo->getName()) == 0)
		ourPlayer = &gamePlayers.back(); //if it is our player, keep its reference
}
void gameState::recordAction(int seat, eBettingAction action, int amount)
{
	getPlayer(seat)->recordAction(action, amount, currentStreet);
	potSize += amount;
	logAction(seat, action, amount);
}
void gameState::logAction(int seat, eBettingAction action, int amount)
{
	std::cout<<getPlayer(seat)->getName()<<" : ";
	if (action == eBettingAction_Raise) {
		std::cout<<"Raises to ";
		std::cout<<amount;
	} else if (action == eBettingAction_Call) {
		if (amount != 0) {
			std::cout<<"Calls ";
			std::cout<<amount;
		} else {
			std::cout<<"Checks";
		}
	} else if (action == eBettingAction_Fold) {
		std::cout<<"Folds";
	} else if (action == eBettingAction_Blind) {
		std::cout<<"Blinds ";
		std::cout<<amount;
	}
	std::cout<<std::endl;
}