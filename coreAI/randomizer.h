/*
$Revision: 35 $
$Date: 2008-09-10 01:49:58 -0700 (Wed, 10 Sep 2008) $
$Author: vananth $
*/
#pragma once
#include "globalDefines.h"

class randomizer
{
private:
	const int DOUBLE_PRECISION; ///< Controls the number of decimal places in random doubles (100 =2, 1000=3)
	static bool firstTime;
public:
	/**
	Default constructor
	*/
	randomizer();
	~randomizer();

	/**
	Initialize the random number generator with a specified seed
	\param seed : The seed to initialize the RNG
	*/
	void initSeed(int seed);
	/**
	Initializes RNG with current time
	*/
	void initTime();
	
	/**
	Generate a random integer within a certain range (inclusive of min and max)
	\param min : The minimum value that the random number can take
	\param max : The maximum value that the random number can take
	*/
	int randomIntRange(int min, int max);
	/**
	Generate a double between 0 and 1
	*/
	double randomZeroToOne();
	/**
	Generate a random number that has a fixed number of bits set and not within the excludedBitMask
	\param numSetBits		: The number of bits that should be set
	\param width			: The maximum bit width of the generated number
	\param excludedBitMask	: The bits that should not be a part of the randomization
	\return A 64 bit number with the required number of bits set
	*/
	uint64 randomBitMask(int numSetBits, int width, uint64 excludedBitMask);
};