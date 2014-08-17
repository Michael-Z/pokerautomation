/*
$Revision: 35 $
$Date: 2008-09-10 01:49:58 -0700 (Wed, 10 Sep 2008) $
$Author: vananth $
*/
#include <ctime>
#include <cstdlib>

#include "randomizer.h"
bool randomizer::firstTime = true;
randomizer::randomizer():DOUBLE_PRECISION(1000)
{
	if (firstTime)
	{
		initTime();
		firstTime = false;
	}
}
randomizer::~randomizer()
{
}
void randomizer::initSeed(int seed)
{
	srand(seed);
}
void randomizer::initTime()
{
	srand((unsigned int)time(0));
}

int randomizer::randomIntRange(int min, int max)
{
	if (min == max)
		return min;
	int rNum = rand();
	return ((rNum%(max-min+1))+min);
}

uint64 randomizer::randomBitMask(int numSetBits, int width, uint64 excludedBitMask)
{
	int setBitCount = 0;
	int shiftPosition;
	uint64 retVal = 0;
	while (setBitCount != numSetBits)
	{
		shiftPosition = randomIntRange(0,(width-1));
		if (((retVal | excludedBitMask) >> shiftPosition)&(uint64)0x1) // if any used bits are set, dont increment the count
			continue;
		retVal = retVal | ((uint64)0x1 << shiftPosition);
		setBitCount++;
	}
	return retVal;
}
double randomizer::randomZeroToOne()
{
	return (randomIntRange(0,DOUBLE_PRECISION)/(double)DOUBLE_PRECISION);
}