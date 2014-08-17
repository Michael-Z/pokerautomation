/*
$Revision: 35 $
$Date: 2008-09-10 01:49:58 -0700 (Wed, 10 Sep 2008) $
$Author: vananth $
*/
#pragma once
#include "globalDefines.h"
#include "pokerDefines.h"
#include "gameState.h"

/**
The base class for a poker table. Online, networked and gui based tables
are derived from this class
*/
class pokerTable
{
protected:
	gameState * gameStateP; ///< Pointer to the object where the game state should be saved
	
	/**
	Connect to the poker table
	\return int : 0 if the connection is successful
	*/
	virtual int connect() = 0;
	/**
	Disconnect from the poker table
	\return int 0 if the disconnection is successful
	*/
	virtual int disconnect() = 0;
	/**
	Perform a betting action
	\param eBettingAction : The action to perform
	\param raiseAmt : The amount to raise if eBettingAction = eBettingAction_Raise 
	\return int 0 if the action is successful
	*/
	virtual int performBettingAction(eBettingAction action,int raiseAmt = 0) = 0;
	/**
	Sitout but observe the game
	*/
	virtual int sitOut() = 0;
	/**
	Sit back in (should do nothing if already sitting in)
	*/
	virtual int sitIn() = 0;


public:
	pokerTable(gameState * gs);
	~pokerTable();
};
