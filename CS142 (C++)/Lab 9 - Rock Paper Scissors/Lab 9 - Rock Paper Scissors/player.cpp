#include "player.h"
using namespace std;

//---------------------------------------------------------------------------------------
Player::Player(string name_in)
{
	name = name_in;
	wins = 0;
	losses = 0;
	draws = 0;
	match_count = 0;
}
Player::~Player(){}
//---------------------------------------------------------------------------------------
string Player::getName()
{
	return name;
}
double Player::getWins()
{
	return wins;
}
double Player::getLosses()
{
	return losses;
}
double Player::getDraws()
{
	return draws;
}

void Player::Won(){
	wins++;
}
void Player::Lost(){
	losses++;
}
void Player::Tie(){
	draws++;
}
//---------------------------------------------------------------------------------------
double Player::getRPSThrow()
{
	double the_throw = rand() %3;
	match_count++;
	return the_throw;
}
//---------------------------------------------------------------------------------------
double Player::getWinRecord()
{
	double record;
	if (match_count > 0)
		record = (wins/match_count)*100;
	else
		record = 0;

	return record;
}
//---------------------------------------------------------------------------------------
string Player::toString()
{
	stringstream ss;
	ss << "Name: " << name << endl;
	ss << "Wins: " << wins << endl;
	ss << "Losses: " << losses << endl;
	ss << "Draws: " << draws << endl;
	ss << "Percentage: " << getWinRecord() << "%" << endl;
	return ss.str();
}
//---------------------------------------------------------------------------------------
