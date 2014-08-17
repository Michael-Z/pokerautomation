/*
$Revision: 37 $
$Date: 2008-09-11 02:27:14 -0700 (Thu, 11 Sep 2008) $
$Author: vananth $
*/
#pragma once
#include <string>
#include "globalDefines.h"
#include "pokerDefines.h"

class player
{
private:
	std::string playerName; ///< Player's name
	int seatNumber; ///< Player's seat number
	eBettingAction lastAction; ///< The last action performed by the player
	bool wasDealt; ///< Was the player dealt cards in this hand
	bool putMoneyInPot; ///< Whether the player put any money into the pot
	bool holeCardsRevealed; ///< Set to true when hole cards are known. Set to false when gameId is set
	int bankRoll; ///< Bankroll for the player in the current hand
	int holeCards[2]; ///< The hole cards of the player (if revealed)
	int amountContributedInStreet[4]; ///< Amount put into pot in the four streets (including blinds if preflop)
public:
	player();
	
	/**
	Get the seat number of the player
	*/
	int getSeat();
	/**
	Get the name of the player
	\return String player name
	*/
	std::string getName();
	/**
	Get the two cards that were revealed by the player
	\param holeCardArr : Pointer to the location where the hole cards should be stored
	\return boolean which is true if the hole cards are known, and false if they aren't
	*/
	bool getHoleCards(int *holeCardArr);
	/**
	Check whether the player has folded
	*/
	bool isFolded();
	/**
	Check whether the player put money in. (i.e, either a bet or a call or a raise, but no check)
	*/
	bool isCommitted();
	/**
	Check whether the player was dealt any cards or is sitting out
	*/
	bool inGame();
	/**
	Get the amount of money with the player
	*/
	int getBankRoll();
	/**
	Get the amount of money this player has put into the pot
	*/
	int getTotalAmountContributedToPot();
	/**
	Get the last action made by the player
	\return One of eBettingAction
	*/
	eBettingAction getLastAction();

	/**
	Set the player's name
	\param name : The player's name
	*/
	void setName(std::string name);
	/**
	Set the player's seat
	*/
	void setSeat(int seat);
	/**
	Set the last action that the player performed
	*/
	void recordAction(eBettingAction act, int amount, eStreet street);
	/**
	Call this function if the player was dealt cards this game
	*/
	void setIsPlaying();
	/**
	Call this function every new game.
	All previous history is cleared and player is reinitialized
	This function should be called before any other player initialization, for every new game
	*/
	void setGameId(int id);
	/**
	Set the bankroll of the player
	*/
	void setBankRoll(int money);
	/**
	Set the hole cards of the player
	*/
	void setHoleCards(int* holeCardArr);
public:
	~player(void);
};
