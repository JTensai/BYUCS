#include <iostream>
#include <string>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

class Player
{
	private:
		string name;
		double wins;
		double losses;
		double draws;
		double match_count;

	public:
		//---------------------------------------------------------------------------------------
		/* 
		 * Constructor/Destructor
		 * 
		 * Handles creation and deletion of Player objects.
		 * 
		 * Parameter: name_in
		 *		The name of a new player
		 * Parameter: wins_in
		 *		The players win count
		 * Parameter: losses_in
		 *		The players loss count
		 * Parameter: draws_in
		 *		The players draw count
		 */
		Player(string name_in);
		virtual ~Player();
		//---------------------------------------------------------------------------------------
		/* 
		 * getName
		 * Returns the name of the player.
		 */
		string getName();
		/* 
		 * getWins
		 * Returns the players wins.
		 */
		double getWins();
		/* 
		 * getLosses
		 * Returns the players losses.
		 */
		double getLosses();
		/* 
		 * getDraws
		 * Returns the players draws.
		 */	
		double getDraws();
		
		void Won();
		void Lost();
		void Tie();
		/* 
		 * getRPSThrow
		 * Returns 1-3 for the throw
		 */
		double getRPSThrow();

		/*
		 * getWinRecord()
		 * Returns players win record
		 */
		double getWinRecord();

		/* 
		 * toString
		 * Returns a single string containing useful information about the player.
		 */
		string toString();
		//---------------------------------------------------------------------------------------
};