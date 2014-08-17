/*
$Revision: 35 $
$Date: 2008-09-10 01:49:58 -0700 (Wed, 10 Sep 2008) $
$Author: vananth $
*/
#include "strategyProb.h"

strategyProb::strategyProb(gameState * gs) : strategyBase(gs),handAnlyz(), randGen()
{
}

strategyProb::~strategyProb()
{
}
strategyBase::eTablePosition strategyProb::findTablePosition(int myPosition, int numPlayers)
{
	double relPostion = (double)(myPosition+1) / numPlayers;
	if (relPostion > 0.7)
		return eTablePosition_Late;
	else if (relPostion >= 0.33)
		return eTablePosition_Middle;
	else
		return eTablePosition_Early;
}
void strategyProb::doEval()
{
	eStreet curSt = gameStateP->getCurrentStreet();
	if ( curSt == eStreet_PreFlop)
	{
		doPreFlopEval();		
	}
	else 
	{
		doPostFlopEval();
	}
}

void strategyProb::doPreFlopEval()
{
	int holeCards[2];
	int sklGrp;
	eTablePosition position;

	gameStateP->getHoleCards(holeCards);
	sklGrp = handAnlyz.getSklanskyGroup(holeCards[0],holeCards[1]);
	position = findTablePosition(gameStateP->getPosition(),gameStateP->getNumPlayersDealt());
	switch (sklGrp)
	{
	case 9:
		prTrip.raiseProb = 0.01;
		prTrip.foldProb = 0.94;
		prTrip.callProb = 0.05;
		break;
	case 8:
	case 7:
		switch (position)
		{
		case eTablePosition_Early:
			prTrip.raiseProb = 0.01;
			prTrip.foldProb = 0.94;
			prTrip.callProb = 0.05;
			break;
		case eTablePosition_Middle:
			prTrip.raiseProb = 0.20;
			prTrip.foldProb = 0.70;
			prTrip.callProb = 0.10;
			break;
		case eTablePosition_Late:
			prTrip.raiseProb = 0.05;
			prTrip.foldProb = 0.01;
			prTrip.callProb = 0.94;
			break;
		}
		break;
	case 6:
		switch (position)
		{
		case eTablePosition_Early:
			prTrip.raiseProb = 0.20;
			prTrip.foldProb = 0.70;
			prTrip.callProb = 0.10;
			break;
		case eTablePosition_Middle:
			prTrip.raiseProb = 0.20;
			prTrip.foldProb = 0.10;
			prTrip.callProb = 0.70;
			break;
		case eTablePosition_Late:
			prTrip.raiseProb = 0.25;
			prTrip.foldProb = 0.05;
			prTrip.callProb = 0.70;
			break;
		}
		break;
	case 5:
	case 4:
		switch (position)
		{
		case eTablePosition_Early:
			prTrip.raiseProb = 0.20;
			prTrip.foldProb = 0.10;
			prTrip.callProb = 0.70;
			break;
		case eTablePosition_Middle:
			prTrip.raiseProb = 0.20;
			prTrip.foldProb = 0.10;
			prTrip.callProb = 0.70;
			break;
		case eTablePosition_Late:
			prTrip.raiseProb = 0.25;
			prTrip.foldProb = 0.05;
			prTrip.callProb = 0.70;
			break;
		}
		break;
	case 3:
	case 2:
		switch (position)
		{
		case eTablePosition_Early:
			prTrip.raiseProb = 0.40;
			prTrip.foldProb = 0.0;
			prTrip.callProb = 0.60;
			break;
		case eTablePosition_Middle:
			prTrip.raiseProb = 0.50;
			prTrip.foldProb = 0.0;
			prTrip.callProb = 0.50;
			break;
		case eTablePosition_Late:
			prTrip.raiseProb = 0.70;
			prTrip.foldProb = 0.0;
			prTrip.callProb = 0.30;
			break;
		}
		break;
	case 1:
		switch (position)
		{
		case eTablePosition_Early:
			prTrip.raiseProb = 0.80;
			prTrip.foldProb = 0.0;
			prTrip.callProb = 0.20;
			break;
		case eTablePosition_Middle:
			prTrip.raiseProb = 0.88;
			prTrip.foldProb = 0.0;
			prTrip.callProb = 0.12;
			break;
		case eTablePosition_Late:
			prTrip.raiseProb = 0.95;
			prTrip.foldProb = 0.0;
			prTrip.callProb = 0.05;
			break;
		}
		break;
	}
	
}

void strategyProb::doPostFlopEval()
{
	int boardCards[5];
	int holeCards[2];
	int numBoardCards = gameStateP->getBoardCards(boardCards);
	gameStateP->getHoleCards(holeCards);
	int numPlayers = gameStateP->getRemainingPlayers();
	int numOpponents =  numPlayers - 1;
	
	handAnalyzer::handStrength hsStruct;
	hsStruct = handAnlyz.handPotentialWithEHS(holeCards,boardCards,numBoardCards,numOpponents,50000);
	if ((hsStruct.ehs >= (1/(double)numPlayers)) || (hsStruct.rhs >= (1/(double)numPlayers)))
	{
		prTrip.raiseProb = 0.70;
		prTrip.foldProb = 0.0;
		prTrip.callProb = 0.30;
	}
	else if (hsStruct.ppot >= getPotOdds())
	{
		prTrip.raiseProb = 0.25;
		prTrip.foldProb = 0.0;
		prTrip.callProb = 0.75;
	}
	else
	{
		prTrip.raiseProb = 0.05;
		prTrip.foldProb = 0.95;
		prTrip.callProb = 0.0;
	}
}
eBettingAction strategyProb::getAction()
{
	double act = randGen.randomZeroToOne();
	if (act <= prTrip.foldProb) //check/fold
	{
		if (gameStateP->getAmountToCall() == 0)
			return eBettingAction_Check;
		else
			return eBettingAction_Fold;
	}
	else if (act <= (prTrip.foldProb + prTrip.callProb)) //check/call
	{
		if (gameStateP->getAmountToCall() == 0)
			return eBettingAction_Check;
		else
			return eBettingAction_Call;
	}
	else //bet/raise
	{
		return eBettingAction_Raise;
	}
}
