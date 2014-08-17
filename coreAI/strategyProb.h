/*
$Revision: 35 $
$Date: 2008-09-10 01:49:58 -0700 (Wed, 10 Sep 2008) $
$Author: vananth $
*/
#pragma once
#include "strategyBase.h"
#include "handAnalyzer.h"

class strategyProb :
	public strategyBase
{
private:
	handAnalyzer handAnlyz;
	randomizer randGen;
public:
	strategyProb(gameState* gmStP);
	~strategyProb();
	/**
	Returns the action to be performed
	*/
	eBettingAction getAction(); 
	/**
	Start the evaluation. Should be called after the game state is stable.
	*/
	void doEval();
private:
	/**
	Do the preflop evaluation
	*/
	void doPreFlopEval();
	/**
	Do the post flop evaluation
	*/
	void doPostFlopEval();
	/**
	Find the table position (early, middle or late). Button is assumed to be at position 0
	\param myPosition	  : The bot's position after excluding people that have not been dealt
	\param numPlayers	  : The number of players dealt
	\return : eTablePostion
	*/
	strategyBase::eTablePosition findTablePosition(int myPostion, int numPlayers);
};
