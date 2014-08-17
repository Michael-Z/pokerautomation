/*
$Revision: 35 $
$Date: 2008-09-10 01:49:58 -0700 (Wed, 10 Sep 2008) $
$Author: vananth $
*/
#pragma once
#include "gameState.h"

class strategyBase
{
public:
	struct ProbabilityTriple {
		double raiseProb;
		double callProb;
		double foldProb;
	};
	enum eTablePosition {
		eTablePosition_Early = 0,
		eTablePosition_Middle = 1,
		eTablePosition_Late = 2
	};
protected:
	gameState *gameStateP;
	ProbabilityTriple prTrip;

public:
	strategyBase();
	strategyBase(gameState* gmStP);
	~strategyBase();
	/**
	Returns the action to be performed
	*/
	virtual eBettingAction getAction() = 0; 
	/**
	Start the evaluation. Should be called after the game state is stable.
	*/
	virtual void doEval() = 0;
	void initialize(gameState* gmStP);
protected:
	void initialize();
	void dispose();

	double getPotOdds();
};
