/*
$Revision: 43 $
$Date: 2008-09-25 02:43:58 -0700 (Thu, 25 Sep 2008) $
$Author: vananth $
*/
#include "handAnalyzer.h"

//Static member initialization
//const int handAnalyzer::Z_ARRAY_SIZE = 10;
const double handAnalyzer::Z_STEP_SIZE = 0.5; 
const double handAnalyzer::Z_CONF_START = 95; 
const double handAnalyzer::zValue[Z_ARRAY_SIZE] = {1.961,2.006,2.005,2.109,2.171,2.242,2.327,2.433,2.577,2.808};

handAnalyzer::handAnalyzer(void) : randomGen()
{
}
handAnalyzer::~handAnalyzer(void)
{
}
/**
Returns a bitmask of cards in a specific suit
\param mask : The full hand mask
\param suit : The suit for which the bitmask is needed
\return The bitmask of the suit
*/
inline uint32 handAnalyzer::getCardsOfSuit(uint64 mask, eCardSuit suit)
{
	return (uint32)((mask >> (int)suit)&0x1fff);
}
/**
Convert a card index (0-51) to a 64 bit mask
*/
inline uint64 handAnalyzer::indexToMask(int index)
{
	return (uint64)0x1<<index;
}
/**
Swap two integers (using xor)
*/
inline void handAnalyzer::swapInt(int &a, int &b)
{
	a = a^b;
	b = a^b;
	a = a^b;
}

/**
Returns the confidence percentage of an index in the normal distribution array (z-table)
\param index : The index into the z-table
\return The confidence interval
*/
inline double handAnalyzer::indexToConfidence(int index)
{
	return (index*Z_STEP_SIZE + Z_CONF_START);
}
/**
Reports the estimation error of the mean of the population distribution given the sample distribution
within various confidence intervals for a Monte Carlo simulation.
\param sampleMean		: The mean of the sample distribution
\param varianceEstimate : The variance of the sample distribution
\param sampleSize		: The number of samples used
*/
void handAnalyzer::reportEstimationError(double sampleMean, double varianceEstimate, double sampleSize)
{
	double standardDeviationEstimate = sqrt(varianceEstimate);
	double standardErrorOfMean = standardDeviationEstimate / (sqrt((double)sampleSize));

	std::cout <<  "------ Estimation Error ------\n";
	std::cout <<  "Standard Deviation : "<<standardDeviationEstimate<<"\n";
	std::cout <<  "Variance : "<<varianceEstimate<<"\n";
	std::cout <<  "Sample Size : "<<sampleSize<<"\n";
	for (int i=0;i<Z_ARRAY_SIZE;i++)
	{
		std::cout<<sampleMean<<"+-"<<zValue[i]*standardErrorOfMean<<" --- "<<indexToConfidence(i)<<"%\n";
	}
	std::cout <<  "------------------------------\n";
}

/**
Returns true if the mask has all suited cards
*/
bool handAnalyzer::isSuited (uint64 handMask, int nCards)
{
	uint32 sp = getCardsOfSuit(handMask,eCardSuit_Spades);
	uint32 he = getCardsOfSuit(handMask,eCardSuit_Hearts);
	uint32 di = getCardsOfSuit(handMask,eCardSuit_Dice);
	uint32 cl = getCardsOfSuit(handMask,eCardSuit_Clubs);

	return ((nBitsTable[sp] == nCards) || (nBitsTable[he] == nCards) || (nBitsTable[di] == nCards) || (nBitsTable[cl] == nCards));
}
/**
Find the income rate of two pre-flop cards
\param card1 : The first hole card
\param card2 : The second hole card
\param expectedNumOpp : The number of opponents expected to play this hand
*/
int handAnalyzer::getIncomeRatePreFlop(int holeCard1, int holeCard2, int expectedNumOpp)
{
	uint64 pocketMask = indexToMask(holeCard1) | indexToMask(holeCard2);
	int rank1 = holeCard1%13;
	int rank2 = holeCard2%13;

	//Rank 1 should be the higher one
	if (rank2 > rank1)
		swapInt(rank1, rank2);

	int IR;
	if (expectedNumOpp >= 5)
		IR = isSuited(pocketMask,2)?preFlopIR6[rank1][rank2]:preFlopIR6[rank2][rank1];
	else
		IR = isSuited(pocketMask,2)?preFlopIR3[rank1][rank2]:preFlopIR3[rank2][rank1];
	return IR;
}
/**
Find the hand rank of two pre-flop cards
\param card1 : The first hole card
\param card2 : The second hole card
\param expectedNumOpp : The number of opponents expected to play this hand
\return The hand rank from 0 to 168 with 0 being the best and 168 the worst
*/
int handAnalyzer::getPreFlopHandRank(int holeCard1, int holeCard2, int expectedNumOpp)
{
	uint64 pocketMask = indexToMask(holeCard1) | indexToMask(holeCard2);
	int rank1 = holeCard1%13;
	int rank2 = holeCard2%13;

	//Rank 1 should be the higher one
	if (rank2 > rank1)
		swapInt(rank1, rank2);

	int rank;
	if (expectedNumOpp >= 5)
	{
		rank = isSuited(pocketMask,2)?preFlopRank6[rank1][rank2]:preFlopRank6[rank2][rank1];
	}
	else
	{
		rank = isSuited(pocketMask,2)?preFlopRank3[rank1][rank2]:preFlopRank3[rank2][rank1];
	}
	return rank;
}
/**
Get the Sklansky group for a pocket hand
*/
int handAnalyzer::getSklanskyGroup(int holeCard1, int holeCard2)
{
	uint64 pocketMask = indexToMask(holeCard1) | indexToMask(holeCard2);
	int rank1 = holeCard1%13;
	int rank2 = holeCard2%13;

	//Rank 1 should be the higher one
	if (rank2 > rank1)
		swapInt(rank1, rank2);

	return isSuited(pocketMask,2)?sklanskyGroupTable[rank1][rank2]:sklanskyGroupTable[rank2][rank1];
}
/**
	This returns the probability of winning a hand (betwn 0 to 1) given the hole cards and the board cards.
	It assumes one opponent who has a completely random hand. The number of board cards can be anywhere from 3 to 5.
	*/
double handAnalyzer::rawHandStrengthBasic(uint64 holeCardMask, uint64 boardCardMask, int numBoardCards)
{
#ifdef CODETIMER
	codeTimer myTimer;
	myTimer.Start();
#endif
	uint64 usedCardMask = holeCardMask | boardCardMask;
	uint32 myRank = evaluate::evaluateHand(usedCardMask, numBoardCards+2);
	uint64 oppCardMask1, oppCardMask2;
	uint32 oppRank;
	int totalIterations = 0;
	double win = 0.0;

#ifdef DEBUG
	int numWins = 0;
	int numTies = 0;
#endif

	for (int i=0;i<52;i++)
	{
		oppCardMask1 = cardMaskTable[i];
		if ((oppCardMask1 & usedCardMask) != 0) 
			continue;
		for (int j=i+1;j<52;j++)
		{
			oppCardMask2 =  cardMaskTable[j];
			if ((oppCardMask2 & usedCardMask) != 0) 
				continue;
			oppRank = evaluate::evaluateHand(oppCardMask1 | oppCardMask2 | boardCardMask,numBoardCards+2);
			if (myRank > oppRank) 
			{
				win += 1.0;
#ifdef DEBUG
				numWins++;
#endif
			}
			else if (myRank == oppRank)
			{
#ifdef DEBUG
				numTies++;
#endif
				win += 0.5;
			}
			totalIterations++;
		}
	}

	DBGPR(myRank);
	DBGPR(numWins);
	DBGPR(numTies);

#ifdef CODETIMER
	myTimer.Stop();
	myTimer.Stop();
	std::cout <<"rawHandStrengthBasic :-\n\tNumber of Iterations = "<<totalIterations<<
		"\n\tNumber of Opponents = 1"<<
		"\n\tNumber of Community Cards = "<<numBoardCards<<
		"\n\tHand Strength = "<<win/totalIterations<<
		"\n\tWins = "<<win<<
		"\n\tTime taken = "<<myTimer.Elapsedus()<<" us\n";
#endif
	return win/totalIterations;
}


/**
This returns the probability of having the best hand (betwn 0 to 1) given the hole cards and the board cards using
a Monte Carlo Simulation against a variable number of opponents. It does not randomize any future community cards
\param holeCardMask		: Our hole cards
\param boardCardMask	: The board cards
\param numBoardCards    : The number of board cards
\param numOpponents     : The number of opponents that have not folded
\param maxIterations    : The numer of iterations for the Monte Carlo simulation
\return	The probability of having the best hand currently
*/
double handAnalyzer::rawHandStrengthMonte(uint64 holeCardMask, uint64 boardCardMask, int numBoardCards, int numOpponents, int maxIterations)
{
#ifdef CODETIMER
	codeTimer myTimer;
	myTimer.Start();
#endif
	uint32 oppHandRank;
	uint64 oppCardMask;
	uint64 usedCardMask;
	bool lost, tied;
	double win = 0;
	uint32 myRank = evaluate::evaluateHand((holeCardMask | boardCardMask), numBoardCards+2);
	int iterationCount = 0;

	while (iterationCount < maxIterations)
	{
		usedCardMask = holeCardMask | boardCardMask;
		lost = false;
		tied = false;
		//Randomize the opponent cards
		for (int i=0 ; i<numOpponents; i++)
		{
			oppCardMask = randomGen.randomBitMask(2,52,usedCardMask);
			usedCardMask |= oppCardMask;
			oppHandRank = evaluate::evaluateHand((oppCardMask | boardCardMask),(numBoardCards + 2));
			if (oppHandRank > myRank)
			{
				lost = true;
				break; //If we lost to even one opponent, we can end the current iteration
			}
			else if (oppHandRank == myRank)
			{
				tied = true;
			}
		}
		if (!lost)
		{
			if (!tied)
				win += 1.0; // Our hand beat all opponent hands
			else
				win += 0.5; // Our hand tied with atleast one opponent hand
		}
		iterationCount++;
	}
#ifdef CODETIMER
	myTimer.Stop();
	std::cout <<"rawHandStrengthMonte :-\n\tNumber of Iterations = "<<iterationCount<<
		"\n\tNumber of Opponents = "<<numOpponents<<
		"\n\tNumber of Community Cards = "<<numBoardCards<<
		"\n\tHand Strength = "<<win/iterationCount<<
		"\n\tWins = "<<win<<
		"\n\tTime taken = "<<myTimer.Elapsedus()<<" us\n";
#endif
	return win/iterationCount;
}
/**
This returns the potential of the current hand. Potential is defined as the probability that our hand will
improve to the best hand after new community cards have been dealt.
*/
handAnalyzer::handStrength handAnalyzer::handPotentialWithEHS(uint64 holeCardMask, uint64 boardCardMask, int numBoardCards, int numOpponents, int maxIterations)
{
#ifdef CODETIMER
	codeTimer myTimer;
	myTimer.Start();
#endif
	const int ahead = 2, behind = 0, tied = 1;
	int index;
	uint32 oppHandRank;
	uint64 oppCardMask[9];
	uint64 usedCardMask;
	bool lostHand, tiedHand;
	int iterationCount = 0;
	int handPot[3][3] = {0};
	double win = 0.0;

#ifdef USE_NORMALIZED_POTENTIAL
	int handPotTotal[3] = {0};
#endif

	uint32 myRank = evaluate::evaluateHand((holeCardMask | boardCardMask), numBoardCards+2);

	while (iterationCount < maxIterations)
	{
		lostHand = false;
		tiedHand = false;
		usedCardMask = holeCardMask | boardCardMask;

		//Randomize the opponent cards and find out whether we are ahead, behind or tied with the current 
		//board cards
		for (int i=0 ; i<numOpponents; i++)
		{
			oppCardMask[i] = randomGen.randomBitMask(2,52,usedCardMask);
			usedCardMask |= oppCardMask[i];
			oppHandRank = evaluate::evaluateHand((oppCardMask[i] | boardCardMask),(numBoardCards + 2));
			if (oppHandRank > myRank)
			{
				lostHand = true;
			}
			else if (oppHandRank == myRank)
			{
				tiedHand = true;
			}
		}
		if (!lostHand)
		{
			if (!tiedHand)
			{
				win += 1.0;
				index = ahead; // Our hand beat all opponent hands
			}
			else
			{
				win += 0.5;
				index = tied; // Our hand tied with atleast one opponent hand
			}
		}
		else
		{
			index = behind; // Our hand lost to atleast one opponent
		}
#ifdef USE_NORMALIZED_POTENTIAL
		handPotTotal[index]++;
#endif
		//Randomize the next board cards and calculate whether we are ahead, behind or tied
		lostHand = false;
		tiedHand = false;
		uint64 newBoardMask = boardCardMask | randomGen.randomBitMask(5-numBoardCards,52,usedCardMask);
		uint32 myNewRank = evaluate::evaluateHand((holeCardMask | newBoardMask), 7);
		for (int i=0 ; i<numOpponents; i++)
		{
			oppHandRank = evaluate::evaluateHand((oppCardMask[i] | newBoardMask),7);
			if (oppHandRank > myNewRank)
			{
				lostHand = true;
				break; //If we lost to even one opponent, we can end the current iteration
			}
			else if (oppHandRank == myNewRank)
			{
				tiedHand = true;
			}
		}
		if (!lostHand)
		{
			if (!tiedHand)
				handPot[index][ahead]++; // Our hand beat all opponent hands
			else
				handPot[index][tied]++; // Our hand tied with atleast one opponent hand
		}
		else
		{
			handPot[index][behind]++; // Our hand lost to atleast one opponent
		}
		iterationCount++;
	} //while(iterationCount < maxIterations)

	handStrength hpStr;
	hpStr.rhs = win/(double)iterationCount;

#ifdef USE_NORMALIZED_POTENTIAL
	if ((handPotTotal[behind]==0) && (handPotTotal[tied] == 0))
		hpStr.ppot = 0.0;
	else
		hpStr.ppot= (handPot[behind][ahead] + (handPot[behind][tied]/2.0) + (handPot[tied][ahead]/2.0)) / ((double)handPotTotal[behind] + (double)(handPotTotal[tied]/2.0));

	if ((handPotTotal[ahead]==0) && (handPotTotal[tied] == 0))
		hpStr.npot = 0.0;
	else
		hpStr.npot= (handPot[ahead][behind] + (handPot[tied][behind]/2.0) + (handPot[ahead][tied]/2.0)) / ((double)handPotTotal[ahead] + (double)(handPotTotal[tied]/2.0));
#else
	hpStr.ppot= (handPot[behind][ahead] + (handPot[behind][tied]/2.0) + (handPot[tied][ahead]/2.0)) / ((double)iterationCount);
	hpStr.npot= (handPot[ahead][behind] + (handPot[tied][behind]/2.0) + (handPot[ahead][tied]/2.0)) / ((double)iterationCount);
#endif
	hpStr.ehsopt = hpStr.rhs + (1 - hpStr.rhs) * hpStr.ppot;
	hpStr.ehs = hpStr.ehsopt - hpStr.rhs*hpStr.npot;
#ifdef CODETIMER
	myTimer.Stop();
	std::cout <<"handPotential :-\n\tNumber of Iterations = "<<iterationCount<<
		"\n\tNumber of Opponents = "<<numOpponents<<
		"\n\tNumber of Community Cards = "<<numBoardCards<<
		"\n\tPPOT = "<<hpStr.ppot<<
		"\n\tNPOT = "<<hpStr.npot<<
		"\n\tRHS  = "<<hpStr.rhs<<
		"\n\tEHS  = "<<hpStr.ehs<<
		"\n\tEHSOPT  = "<<hpStr.ehsopt<<
		"\n\tBehind->Ahead = "<<handPot[behind][ahead]<<
		"\n\tTime taken = "<<myTimer.Elapsedus()<<" us\n";
#endif
	return hpStr;
}

/**
This returns the potential of the current hand. Potential is defined as the probability that our hand will
improve to the best hand after new community cards have been dealt.
*/
handAnalyzer::handStrength handAnalyzer::handPotentialWithEHS(int* holeCards, int* boardCards, int numBoardCards, int numOpponents, int maxIterations)
{
	uint64 boardCardMask(0);
	for (int i=0;i<numBoardCards;i++)
	{
		boardCardMask |= indexToMask(boardCards[i]);
	}
	return handPotentialWithEHS(indexToMask(holeCards[0])|indexToMask(holeCards[1]),
		boardCardMask,numBoardCards,numOpponents,maxIterations);
}
/**
This function returns the probability of winning by doing a Monte Carlo simulation where the community cards and the opponent
cards are randomized
The value returned by this = RHS * (1 - NPOT) + (1 - RHS) * PPOT = EHS
*/
double handAnalyzer::winProbabilityMonte(uint64 holeCardMask, uint64 boardCardMask, int numBoardCards, int numOpponents, int maxIterations)
{
#ifdef CODETIMER
	codeTimer myTimer;
	myTimer.Start();
#endif
#ifdef REPORT_VARIANCE
	int numWins = 0;
	int numTies = 0;
#endif
	uint32 oppHandRank;
	uint64 oppCardMask;
	uint64 usedCardMask;
	uint64 newBoardMask;
	bool lost, tied;
	double win = 0.0;
	int iterationCount = 0;
	uint32 myRank;

	while (iterationCount < maxIterations)
	{
		lost = false;
		tied = false;
		usedCardMask = holeCardMask | boardCardMask;

		//Randomize the board cards
#pragma message(Reminder "Randomize board cards AFTER opponent cards")
		newBoardMask = boardCardMask | randomGen.randomBitMask(5-numBoardCards,52,usedCardMask);
		usedCardMask |= newBoardMask;

		myRank = evaluate::evaluateHand((holeCardMask | newBoardMask), 7);
		//Randomize the opponent cards
		for (int i=0 ; i<numOpponents; i++)
		{
			oppCardMask = randomGen.randomBitMask(2,52,usedCardMask);
			usedCardMask |= oppCardMask;
			oppHandRank = evaluate::evaluateHand((oppCardMask | newBoardMask),7);
			if (oppHandRank > myRank)
			{
				lost = true;
				break; //If we lost to even one opponent, we can end the current iteration
			}
			else if (oppHandRank == myRank)
			{
				tied = true;
			}
		}
		if (!lost)
		{
			if (!tied)
			{
#ifdef REPORT_VARIANCE
				numWins++;
#endif
				win += 1.0; // Our hand beat all opponent hands
			}
			else
			{
#ifdef REPORT_VARIANCE
				numTies++;
#endif
				win += 0.5; // Our hand tied with atleast one opponent hand
			}
		}
		iterationCount++;
	}
#ifdef REPORT_VARIANCE
	double sampleMean = (win/(double)iterationCount)*100;
	double varianceEstimate = (
		pow((100-sampleMean),2)*numWins + 
		pow((50-sampleMean),2)*numTies + 
		pow((0-sampleMean),2)*(iterationCount - numWins - numTies)
		) /(iterationCount - 1);
	reportEstimationError(sampleMean,varianceEstimate,iterationCount);
#endif
#ifdef CODETIMER
	myTimer.Stop();
	std::cout <<"winProbabilityMonte :-\n\tNumber of Iterations = "<<iterationCount<<
		"\n\tNumber of Opponents = "<<numOpponents<<
		"\n\tNumber of Community Cards = "<<numBoardCards<<
		"\n\tWin Probability = "<<win/iterationCount<<
		"\n\tWins = "<<win<<
		"\n\tTime taken = "<<myTimer.Elapsedus()<<" us\n";
#endif
	return win/iterationCount;
}
