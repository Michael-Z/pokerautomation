/*
$Revision: 46 $
$Date: 2008-09-25 02:47:24 -0700 (Thu, 25 Sep 2008) $
$Author: vananth $
*/
#pragma once
#include "globalDefines.h"
/**
The suit of a card
*/
enum eCardSuit {
	eCardSuit_Spades = 0,
	eCardSuit_Hearts = 1,
	eCardSuit_Dice   = 2,
	eCardSuit_Clubs  = 3
};

/**
The current game stage
*/
enum eStreet {
	eStreet_PreFlop = 0,
	eStreet_Flop	= 1,
	eStreet_Turn	= 2,
	eStreet_River	= 3,
	eStreet_Min     = eStreet_PreFlop,
	eStreet_Max		= eStreet_River
};
enumTricks(eStreet)

/**
Betting Actions
*/	
enum eBettingAction {
	eBettingAction_NoAction	= 0, ///< Undefined action or not yet acted
	eBettingAction_Check	= 1, ///< Action of checking
	eBettingAction_Call		= 2, ///< Action of calling
	eBettingAction_Raise	= 3, ///< Action of raising/betting
	eBettingAction_Fold		= 4, ///< Action of folding
	eBettingAction_SmBlind  = 5, ///< Action of posting the small blind
	eBettingAction_BgBlind  = 6, ///< Action of posting the big blind
	eBettingAction_Blind    = 7  ///< Action of either posting the big/small blind
};


