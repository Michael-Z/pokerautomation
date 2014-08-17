/*
$Revision: 35 $
$Date: 2008-09-10 01:49:58 -0700 (Wed, 10 Sep 2008) $
$Author: vananth $
*/
#pragma once
#include <iostream>
#include <math.h>
#include "globalDefines.h"
#include "pokerDefines.h"
#include "lookupTables.h"
#include "evaluate.h"
#include "codeTimer.h"
#include "randomizer.h"

//Defines for handPotential
#define USE_NORMALIZED_POTENTIAL
//#define REPORT_VARIANCE
//#define Z_ARRAY_SIZE 10 ///< Size of the normal distribution (z curve) array
/**
Performs various poker hand analysis functions
*/
class handAnalyzer {
public:
	/**
	A struct to store the various probabilities calculated through Monte Carlo simulations
	*/
	struct handStrength {
		double ppot; ///< Positive potential
		double npot; ///< Negative potential
		double rhs; ///< Raw hand strength
		double ehs; ///< Effective hand strength EHS = RHS * (1 - NPOT) + (1 - RHS) * PPOT
		double ehsopt; ///< Optimistic effective hand strength EHSOPT = RHS + (1 - RHS) * PPOT
	};

private :
	/*		
	http://www.measuringusability.com/zcalcp.php
	Confidence		Z-Value
	99.5			2.808
	99				2.577	
	98.5			2.433
	98				2.327	
	97.5			2.242
	97				2.171	
	96.5			2.109
	96				2.055	
	95.5			2.006
	95				1.961	
	*/
	static const int	Z_ARRAY_SIZE = 10; ///< Size of the normal distribution (z curve) array
	static const double Z_STEP_SIZE;  ///< Step size of confidence intervals
	static const double Z_CONF_START; ///< First value of the confidence interval
	static const double zValue[Z_ARRAY_SIZE]; 
	randomizer randomGen;

	inline uint32 getCardsOfSuit(uint64 mask, eCardSuit suit);
	inline uint64 indexToMask(int index);
	inline void swapInt(int &a, int &b);
	inline double indexToConfidence(int index);
	void reportEstimationError(double sampleMean, double varianceEstimate, double sampleSize);

public:
	handAnalyzer(void);
	~handAnalyzer(void);
	bool isSuited (uint64 handMask, int nCards);
	int getIncomeRatePreFlop(int holeCard1, int holeCard2, int expectedNumOpp);
	int getPreFlopHandRank(int holeCard1, int holeCard2, int expectedNumOpp);
	int getSklanskyGroup(int holeCard1, int holeCard2);
	double rawHandStrengthBasic(uint64 holeCardMask, uint64 boardCardMask, int numBoardCards);
	double rawHandStrengthMonte(uint64 holeCardMask, uint64 boardCardMask, int numBoardCards, int numOpponents, int maxIterations);
	handStrength handPotentialWithEHS(uint64 holeCardMask, uint64 boardCardMask, int numBoardCards, int numOpponents, int maxIterations);
	handStrength handPotentialWithEHS(int* holeCards, int* boardCards, int numBoardCards, int numOpponents, int maxIterations);
	double winProbabilityMonte(uint64 holeCardMask, uint64 boardCardMask, int numBoardCards, int numOpponents, int maxIterations);
};


/*Static member initialization
double handAnalyzer::Z_STEP_SIZE = 0.5; 
double handAnalyzer::Z_CONF_START = 95; 
double handAnalyzer::zValue[Z_ARRAY_SIZE] = {1.961,2.006,2.005,2.109,2.171,2.242,2.327,2.433,2.577,2.808};
*/