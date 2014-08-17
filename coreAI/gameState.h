/*
$Revision: 52 $
$Date: 2008-10-03 00:42:35 -0700 (Fri, 03 Oct 2008) $
$Author: vananth $
*/
#pragma once
#include <iostream>
#include <list>
#include "globalDefines.h"
#include "pokerDefines.h"
#include "programInfo.h"
#include "player.h"


class gameState
{
private:
	programInfo* pInfo;
	/*
	Note: We can't use vectors here, because we need the reference to an element of the container
	Vectors have dynamic resizing and will move the elements if there is not enough space, while lists
	are just linked lists. Random access is not possible though (and not needed in this case)
	*/
	std::list<player> gamePlayers; ///< List of players
	std::list<player>::iterator playerIter; ///< Iterator for list of players
	player* ourPlayer; ///< Pointer to our player
	int flopCards[3]; ///< The flop cards
	int turnCard; ///< The turn card
	int riverCard; ///< The river card
	
	eStreet currentStreet; ///< The current street (pre-flop, flop, turn or river)
	int potSize; ///< Total pot size 
	
	int amountToCall; ///< Amount needed for a call on the current street

	int startingPlayers; ///< Number of players dealt
	int remainingPlayers; ///< Number of players who haven't folded
	
	int numSeats;
	int buttonSeat; ///< Seat number of the button (0 - 8)
	int betSize; ///< The minimum bet size for the game
	int gameId; ///< Unique Id of the game

public:
	gameState(programInfo* pi);
	/**
	Get the two hole cards
	\return Array (size 2) of cards
	\sa getBoardCards
	*/
	void getHoleCards(int *holeCardArr);
	/**
	Get the board cards that are visible
	\return int specifying the number of visible cards
	\return array of cards of size mentioned above
	*/
	int getBoardCards(int *boardCardArr);

	/**
	Return the current Street
	\return eStreet
	*/
	eStreet getCurrentStreet();
	
	/**
	The number of bets/raises in the current round.
	*/
	int getNumRaises(); 
	/**
	The number of players that have not folded (includes players that haven't acted and ourself)
	*/
	int getRemainingPlayers(); 
	/**
	The number of players to act after current player.
	*/
	int getNumPlayersToAct(); 
	/**
	The number of players that were dealt into this hand.
	*/
	int getNumPlayersDealt(); 
	/**
	Get the current pot size
	*/
	int getPotSize ();
	/**
	Get the amount needed for a call
	*/
	int getAmountToCall();
	
	/**
	Get the player at a particular seat
	\param seat : The seat number
	\return Player pointer at the seat
	*/
	player* getPlayer(int seat);
	/**
	Get the seat number of a player
	\param pl : The Player
	\return Seat number
	*/
	int getSeat(player pl);
	/**
	Get our seat number
	\return Seat number
	*/
	int getSeat();
	/**
	Get the button's seat
	\return Seat number
	*/
	int getButtonSeat();
	/**
	Get the number of seats in the game
	*/
	int getNumSeats();
	/**
	Get our position, assuming the button is at 0 and excluding players that have
	not been dealt cards.
	\return int position
	*/
	int getPosition();
	/**
	Set the bet size. 
	\param betSize as an integer
	*/
	void setBetSize(int size);
	/**
	Set the number of players that have been dealt into this hand
	*/
	void setNumPlayersDealt(int numPlayers);
	/**
	Set the seat number of the button
	*/
	void setButtonSeat(int seat);
	/**
	Set the game ID
	*/
	void setGameId(int id);
	/**
	Set our hole cards
	\param holeCardArr : Pointer to the hole cards
	*/
	void setHoleCards(int* holeCardArr);
	/**
	Set hole cards of player in a seat
	\param holeCardArr : Pointer to the hole cards
	\param seat : Seat number of the player whose hole cards are being shown
	*/
	void setHoleCards(int* holeCardArr, int seat);
	/**
	Set the flop cards
	\param flopCardArr : Pointer to the flop cards
	*/
	void setFlopCards(int* flopCardArr);
	/**
	Set the turn card
	\param card : The turn card
	*/
	void setTurnCard(int card);
	/**
	Set the river card
	\param card : The river	card
	*/
	void setRiverCard(int card);
	/**
	Set the amount to call
	\param amount : The amount to call
	*/
	void setAmountToCall(int amount);
	/**
	Add a player to the game. 
	\param name : The player's name
	\param seat : The player's seat number
	\param wasDealt: True if the player was dealt cards
	\param bankRoll : The starting bankroll 
	*/
	void addPlayer(std::string name, int seat, bool wasDealt, int bankRoll);
	/**
	Record an action in the game
	\param seat : The seat number that performed this action
	\param action : The action performed
	\param amount : Amount (or other parameter if required)
	*/
	void recordAction(int seat, eBettingAction action, int amount);
	#pragma message(Reminder "Functions for a showdown event need to be added")
	~gameState();
private:
	/**
	Set the current street
	\param street : eStreet enum of the street
	*/
	void setCurrentStreet(eStreet street);
	/**
	Set our seat number
	*/
	void setSeat(int seat);
	/**
	Log the action
	*/
	void logAction(int seat, eBettingAction action, int amount);
};
