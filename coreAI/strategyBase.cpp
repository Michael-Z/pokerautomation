/*
$Revision: 35 $
$Date: 2008-09-10 01:49:58 -0700 (Wed, 10 Sep 2008) $
$Author: vananth $
*/
#include "strategyBase.h"

strategyBase::strategyBase()
{
	initialize();
}
strategyBase::strategyBase(gameState *gmStP)
{
	initialize(gmStP);
}

strategyBase::~strategyBase()
{
	dispose();
}

void strategyBase::initialize()
{
	gameStateP = 0;
}
void strategyBase::initialize(gameState *gmStP)
{
	gameStateP = gmStP;
}
void strategyBase::dispose()
{
	gameStateP = 0;
}
double strategyBase::getPotOdds()
{
	double amountToCall = (double)gameStateP->getAmountToCall();
	return amountToCall/(gameStateP->getPotSize() + amountToCall);
}