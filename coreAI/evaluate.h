/*
$Revision: 35 $
$Date: 2008-09-10 01:49:58 -0700 (Wed, 10 Sep 2008) $
$Author: vananth $
*/
#pragma once
#include "globalDefines.h"
#include "pokerDefines.h"

namespace evaluate {
//Defines for optimization in evaluateHand
//#define NO_TOPCARD_SHIFT_STFLUSH
//#define TRIPS_SINGLE_LOOKUP

#define SPADES_OFFSET (eCardSuit_Spades*13)
#define HEARTS_OFFSET (eCardSuit_Hearts*13)
#define DICE_OFFSET (eCardSuit_Dice*13)
#define CLUBS_OFFSET (eCardSuit_Clubs*13)

#define HIGHCARD		0
#define PAIR			1
#define TWOPAIR			2
#define TRIPS			3
#define STRAIGHT		4
#define FLUSH			5
#define FULLHOUSE		6
#define QUADS			7
#define STRAIGHTFLUSH	8

#define HANDTYPE_SHIFT		24 ///< Bottom 20 bits are for five cards (each of 4 bits). Next four bits are for hand type
#define TOP_CARD_SHIFT		16 ///< Shift by this amount to get the top card
#define SECOND_CARD_SHIFT	12
#define THIRD_CARD_SHIFT	8
#define FOURTH_CARD_SHIFT	4
#define FIFTH_CARD_SHIFT	0


#define VALUE_HIGHCARD      (HIGHCARD << HANDTYPE_SHIFT)
#define VALUE_PAIR          (PAIR << HANDTYPE_SHIFT)
#define VALUE_TWOPAIR       (TWOPAIR << HANDTYPE_SHIFT)
#define VALUE_TRIPS         (TRIPS << HANDTYPE_SHIFT)
#define VALUE_STRAIGHT      (STRAIGHT << HANDTYPE_SHIFT)
#define VALUE_FLUSH         (FLUSH << HANDTYPE_SHIFT)
#define VALUE_FULLHOUSE     (FULLHOUSE << HANDTYPE_SHIFT)
#define VALUE_QUADS			(QUADS << HANDTYPE_SHIFT)
#define VALUE_STRAIGHTFLUSH (STRAIGHTFLUSH << HANDTYPE_SHIFT)
#define VALUE_ERROR			(0x0)

uint32 evaluateHand(uint64 handMask, uint32 nCards);
void handStringToInt(char* handString, int* cardArr);
void handStringToInt(std::string handString, int* cardArr);
uint32 cardStringToInt(char* card);

} //namespace