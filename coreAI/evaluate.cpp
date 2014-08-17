/*
$Revision: 51 $
$Date: 2008-10-03 00:41:53 -0700 (Fri, 03 Oct 2008) $
$Author: vananth $
*/
#include <iostream>
#include <string.h>
#include "evaluate.h"
#include "lookupTables.h"

namespace evaluate {
	/**
	This returns the rank of a hand. Higher ranked hands are better than lower ranked ones.
	\param handMask : 64 bit number representing the hand
	\param nCards : Number of cards in the hand
	\return rank : Returns a 32 bit rank
	*/
	uint32 evaluateHand(uint64 handMask, uint32 nCards)
	{
		/*
		Hand : 52 bit number with bits set for the appropriate card
			 cccc...dddd...hhhh...ssss  (lowest bit is 2 followed by 3,4...K,A)
		This is the packed format for a hand.

		The unpacked format is used for the return value. In the unpacked format, each card's rank is 
		represented by 4 bits. Therefore we need 20 bits for a 5-card hand + 4 bits for the hand type. 
		This is an ordered set.
		Mapping : 2=0, 3=1, .....K=11, A=12
		Hand looks like : HAND_TYPE-TOP_CARD-SECOND_CARD-THIRD_CARD-FOURTH_CARD-FIFTH_CARD  where each element is a 
		4 bit number
		*/
		uint32 retval = VALUE_ERROR;
		uint32 pairCardMask;
		uint32 tripsCardMask;
		uint32 quadsCardMask;
		uint32 remainingCards;
		/*
		Split the 52 bit hand into 4 hands based on the suit
		*/
		uint32 cardSpades = (uint32)((handMask >> SPADES_OFFSET) & 0x00001fffUL); 
		uint32 cardHearts = (uint32)((handMask >> HEARTS_OFFSET) & 0x00001fffUL);
		uint32 cardDice   = (uint32)((handMask >> DICE_OFFSET) & 0x00001fffUL);
		uint32 cardClubs  = (uint32)((handMask >> CLUBS_OFFSET) & 0x00001fffUL);

		uint32 ranks = cardSpades | cardHearts | cardDice | cardClubs;
		uint32 nRanks = nBitsTable[ranks]; //number of different ranks 
		uint32 nDuplicates = ((uint32)(nCards - nRanks)); //number of duplicate cards

	#ifdef DEBUGASSERT
		if ((nBitsTable[cardSpades]+nBitsTable[cardHearts]+nBitsTable[cardDice]+nBitsTable[cardClubs]) != nCards)
		{
			std::cout<<"ERROR in evaluateHand : nCards does not match bits set in handMask\n";
		}
	#endif
		//Step 1
		if (nRanks >=5) //atleast 5 different cards. If this is true, the hand can be a flush, 
						//straight or straight flush
		{
			if (nBitsTable[cardSpades] >= 5) //5 or more spades
			{
				/*FIXME : Why the TOP_CARD_SHIFT ??
				Straight table returns the unpacked format top card of the straight.
				*/
				if (straightTable[cardSpades] != 0)
				{
	                
	#ifdef NO_TOPCARD_SHIFT_STFLUSH
					return VALUE_STRAIGHTFLUSH + (uint32)(straightTable[cardSpades]);
	#else
					return VALUE_STRAIGHTFLUSH + (uint32)(straightTable[cardSpades] << TOP_CARD_SHIFT);
	#endif
				}
				else
					retval = VALUE_FLUSH + topFiveCardsTable[cardSpades];
			}
			else if (nBitsTable[cardHearts] >= 5)
			{
				if (straightTable[cardHearts] != 0)
				{
	                
	#ifdef NO_TOPCARD_SHIFT_STFLUSH
					return VALUE_STRAIGHTFLUSH + (uint32)(straightTable[cardHearts]);
	#else
					return VALUE_STRAIGHTFLUSH + (uint32)(straightTable[cardHearts] << TOP_CARD_SHIFT);
	#endif
				}
				else
					retval = VALUE_FLUSH + topFiveCardsTable[cardHearts];
			}
			else if (nBitsTable[cardDice] >= 5)
			{
				if (straightTable[cardDice] != 0)
				{
	                
	#ifdef NO_TOPCARD_SHIFT_STFLUSH
					return VALUE_STRAIGHTFLUSH + (uint32)(straightTable[cardDice]);
	#else
					return VALUE_STRAIGHTFLUSH + (uint32)(straightTable[cardDice] << TOP_CARD_SHIFT);
	#endif
				}
				else
					retval = VALUE_FLUSH + topFiveCardsTable[cardDice];
			}
			else if (nBitsTable[cardClubs] >= 5)
			{
				if (straightTable[cardClubs] != 0)
				{
	                
	#ifdef NO_TOPCARD_SHIFT_STFLUSH
					return VALUE_STRAIGHTFLUSH + (uint32)(straightTable[cardClubs]);
	#else
					return VALUE_STRAIGHTFLUSH + (uint32)(straightTable[cardClubs] << TOP_CARD_SHIFT);
	#endif
				}
				else
					retval = VALUE_FLUSH + topFiveCardsTable[cardClubs];
			}
			else
			{
				uint32 topCard = straightTable[ranks];
				if (topCard != 0)
					retval = VALUE_STRAIGHT + (topCard << TOP_CARD_SHIFT);
			}
			
			//If we found a flush or straight, but there are not enough duplicates for a full house
			//or quads, return because this is the best hand
			if (retval != VALUE_ERROR && nDuplicates < 3)
				return retval;
		} // if nRanks >=5

		/*Step 2 :
			Either we have no flushes or straights OR
			There are flushes or straights, but we may have a better hand (full house or quads)
		*/
		switch (nDuplicates)
		{
			case 0:
				//No duplicates, so just a high card
				return VALUE_HIGHCARD + topFiveCardsTable[ranks];
			case 1:
				//One pair

				pairCardMask = ranks ^ (cardSpades ^ cardHearts ^ cardClubs ^ cardDice); //inner xor sets the paired card
																					 //to 0. last xor inverts it

				remainingCards = ranks ^ pairCardMask;      // Discard the paired card.

				/*Set the top card value to the value of the paired card (because higher pair is the deciding factor)
				Get the top five cards. ShiftR by 4 to avoid colliding with the pairCard (which is at bits 19-16)
				and retain the 4 kickers
				Zero out the fourth kicker because we need only 3 kickers (the other two cards are the pair)
				*/
				retval = (uint32) (VALUE_PAIR + 
								(topCardTable[pairCardMask] << TOP_CARD_SHIFT) +
								((topFiveCardsTable[remainingCards] >> 4) & 0xFFFFFFF0));
				return retval;

			case 2:
				// Either two pair or trips
				pairCardMask = ranks ^ (cardSpades ^ cardHearts ^ cardClubs ^ cardDice); //inner xor sets the paired card
																					 //to 0. last xor inverts it
				if (pairCardMask != 0) //Two pair
				{
					//pairCardMask has two bits set
					remainingCards = ranks ^ pairCardMask;
					retval = (uint32) (VALUE_TWOPAIR + 
						(topFiveCardsTable[pairCardMask] & (0x000FF000))
						+ (topCardTable[remainingCards] << THIRD_CARD_SHIFT));

					return retval;
				}
				else //pairCardMask == 0 because of three xors on the same bit, which means it is a trips
				{
					tripsCardMask = ((cardClubs & cardDice) | (cardHearts & cardSpades)) & ((cardClubs & cardHearts) | (cardDice & cardSpades));
					remainingCards = ranks ^ tripsCardMask; 
					
					retval = (uint32) (VALUE_TRIPS + (topCardTable[tripsCardMask] << TOP_CARD_SHIFT));
					//FIXME: Instead of two lookups, why not just one into the five card table
					//Maybe better to use topCardTable because it has been cached in the previous step
	#ifdef TRIPS_SINGLE_LOOKUP
					retval += (topFiveCardsTable[remainingCards]>>4) & 0xFFFFFF00;
	#else
		
					uint32 secondCard = topCardTable[remainingCards];
					retval += (secondCard << SECOND_CARD_SHIFT);
					remainingCards ^= (0x1 << (int)secondCard);
					retval += (uint32) (topCardTable[remainingCards] << THIRD_CARD_SHIFT);
	#endif
					
					

					return retval;
				}

			default:
				//Quads, FullHouse, Two Pair, Straight or Flush
				//All of them are lumped under default, because three or more duplicates can be any one of them.
				
				//Check for quads first
				quadsCardMask = cardSpades & cardHearts & cardDice & cardClubs ;
				if (quadsCardMask != 0)
				{
					uint32 topCard = topCardTable[quadsCardMask];
					uint32 secondCard = ranks ^ (0x1 << (int)topCard);
					retval = (uint32) (VALUE_QUADS + 
						(topCard << TOP_CARD_SHIFT) + 
						((topCardTable[secondCard]) << SECOND_CARD_SHIFT));
					return retval;
				}

				pairCardMask = ranks ^ (cardSpades ^ cardHearts ^ cardClubs ^ cardDice);
				if (nBitsTable[pairCardMask] != nDuplicates)
				{
					/*
						This means there were an odd number of xors somewhere, which implies a 
						full house (since duplicates >=3)
					*/
					uint32 topCard;
					tripsCardMask = ((cardClubs & cardDice) | (cardHearts & cardSpades)) & ((cardClubs & cardHearts) | (cardDice & cardSpades));
					topCard = topCardTable[tripsCardMask];
					remainingCards = (pairCardMask | tripsCardMask) ^ (0x1 << (int)topCard);
					retval = (VALUE_FULLHOUSE + 
								(topCard << TOP_CARD_SHIFT) + 
								(topCardTable[remainingCards] << SECOND_CARD_SHIFT));
					return retval;
				}

				if (retval != VALUE_ERROR) //retval is non error if it got set while checking for a flush or straight
				{
					//If no quads or fullhouse, then the flush or straight discovered before is the best hand
					//so return with that value
					return retval;
				}
				else
				{
					//Three pairs. Therefore choose the best two pair
					uint32 topCard, secondCard, thirdCard;

					topCard = topCardTable[pairCardMask];
					secondCard = topCardTable[pairCardMask ^ (0x1 << (int)topCard)]; 
					thirdCard = topCardTable[ranks ^ (0x1 << (int)topCard) ^ (0x1 << (int)secondCard)];
					retval = (VALUE_TWOPAIR + 
						(topCard << TOP_CARD_SHIFT)+
						(secondCard << SECOND_CARD_SHIFT) +
						(thirdCard << THIRD_CARD_SHIFT));
					return retval;
				}
		}
		
	}

	uint32 cardStringToInt(char* card)
	{
		uint32 cardOffset;
		switch (card[0])
		{
		case 'A':
		case 'a':
			cardOffset = 12;
			break;
		case 'K':
		case 'k':
			cardOffset = 11;
			break;
		case 'Q':
		case 'q':
			cardOffset = 10;
			break;
		case 'J':
		case 'j':
			cardOffset = 9;
			break;
		case 'T':
		case 't':
			cardOffset = 8;
			break;
		case '9':
			cardOffset = 7;
			break;
		case '8':
			cardOffset = 6;
			break;
		case '7':
			cardOffset = 5;
			break;
		case '6':
			cardOffset = 4;
			break;
		case '5':
			cardOffset = 3;
			break;
		case '4':
			cardOffset = 2;
			break;
		case '3':
			cardOffset = 1;
			break;
		case '2':
			cardOffset = 0;
			break;
		}
		switch (card[1])
		{
		case 'S':
		case 's':
			cardOffset += SPADES_OFFSET;
			break;
		case 'H':
		case 'h':
			cardOffset += HEARTS_OFFSET;
			break;
		case 'D':
		case 'd':
			cardOffset += DICE_OFFSET;
			break;
		case 'C':
		case 'c':
			cardOffset += CLUBS_OFFSET;
			break;
		}
		return cardOffset;
	}
	void handStringToInt(char* handString, int* cardArr)
	{
		char* card;
		int cardCount(0);
		
		card = strtok(handString," ");
		while (card != NULL)
		{
			cardArr[cardCount++] = cardStringToInt(card);
			card = strtok(NULL," ");
		}
	}
	void handStringToInt(std::string handString, int* cardArr)
	{
		char* hand = new char[handString.length()+1];
		strcpy(hand,handString.c_str());
		handStringToInt(hand,cardArr);
		delete [] hand;
	}
} //namespace