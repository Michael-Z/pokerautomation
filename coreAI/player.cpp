/*
$Revision: 37 $
$Date: 2008-09-11 02:27:14 -0700 (Thu, 11 Sep 2008) $
$Author: vananth $
*/
#include "player.h"

player::player()
{
}

player::~player()
{
}

int player::getSeat()
{
	return seatNumber;
}
std::string player::getName()
{
	return playerName;
}
bool player::getHoleCards(int *holeCardArr)
{
	if (holeCardsRevealed)
	{
		holeCardArr[0] = holeCards[0];
		holeCardArr[1] = holeCards[1];
	}
	return holeCardsRevealed;
}
int player::getTotalAmountContributedToPot()
{
	int total = 0;
	for (eStreet st=eEnumMin(st);st<=eEnumMax(st);st++)
	{
		total += amountContributedInStreet[(int)st];
	}
	return total;
}
bool player::isFolded()
{
	return (lastAction == eBettingAction_Fold);
}
bool player::inGame()
{
	return wasDealt;
}
void player::setName(std::string name)
{
	playerName = name;
}
void player::setSeat(int seat)
{
	seatNumber = seat;
}
void player::recordAction(eBettingAction act, int amount, eStreet street)
{
	lastAction = act;
	if ((act == eBettingAction_Call) || (act == eBettingAction_Raise))
	{
		putMoneyInPot = true;
	}
	amountContributedInStreet[(int)street] += amount;
}
void player::setIsPlaying()
{
	wasDealt = true;
}
void player::setGameId(int id)
{
	wasDealt = false;
	putMoneyInPot = false;
	holeCardsRevealed = false;
	lastAction = eBettingAction_NoAction;
	for (eStreet st=eStreet_Min;st<=eStreet_Max;st++)
	{
		amountContributedInStreet[(int)st] = 0;
	}
	bankRoll = 0;
}
void player::setBankRoll(int money)
{
	bankRoll = money;
}
void player::setHoleCards(int* holeCardArr)
{
	holeCards[0] = holeCardArr[0];
	holeCards[1] = holeCardArr[1];
	holeCardsRevealed = true;
}