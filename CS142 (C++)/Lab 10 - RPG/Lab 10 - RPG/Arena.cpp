#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "Arena.h"
#include "Archer.h"
#include "Robot.h"
#include "Cleric.h"

using namespace std;

Arena::Arena(){}
Arena::~Arena(){}		

bool Arena::addFighter(string info)
{
	stringstream ss;
	string name, type;
	int mhp, strength, speed, magic;

	ss.str(info);

	ss >> name;
	ss >> type;
	ss >> mhp;
	ss >> strength;
	ss >> speed;
	ss >> magic;

	if (ss.fail() == true || ss.eof() == false){
		return false;
	}

	FighterInterface* fighter;

	if (type == "A"){
		fighter = new Archer(name, mhp, strength, speed, magic);
	}
	else if (type == "R"){
		fighter = new Robot(name, mhp, strength, speed, magic);
	}
	else if (type == "C"){
		fighter = new Cleric(name, mhp, strength, speed, magic);
	}
	else{
		// invalid fighter class, return false
		return false;
	}

	for (int i = 0; i < allFighters.size(); i++){
		if (allFighters[i]->getName() == name)
			// fighter exists, return false
			return false;		
	}

	// player does not exist, continue checking
	if (mhp <= 0 || strength <= 0 || speed <= 0 || magic <= 0){
		// invalid stats for fighter, return false
		return false;
	}

	allFighters.push_back(fighter);

	return true;
	/*
	*	addFighter(string)
	*
	*	Adds a new fighter to the collection of fighters in the arena. Do not allow 
	*	duplicate names.  Reject any string that does not adhere to the format 
	*	outlined in the lab specs.
	*
	*	Return true if a new fighter was added; false otherwise.
	*/
}

bool Arena::removeFighter(string name)
{
	for (int i = 0; i < allFighters.size(); i++){
		if (allFighters[i]->getName() == name){
			//fighter exists, remove from allFighters vector and return true
			allFighters.erase(allFighters.begin()+i);
			return true;
		}
		else{
			//fighter does not exists, do nothing
		}
	}
	
	return false;
	/*
	*	removeFighter(string)
	*
	*	Removes the fighter whose name is equal to the given name.  Does nothing if 
	*	no fighter is found with the given name.
	*
	*	Return true if a fighter is removed; false otherwise.
	*/
}

FighterInterface* Arena::getFighter(string name)
{
	for (int i = 0; i < allFighters.size(); i++){
		if (allFighters[i]->getName() == name){
			//fighter exists, return memory address of fighter
			return allFighters[i];
		}
		else{
			//fighter does not exists, do nothing
		}
	}

	return NULL;
	/*
	*	getFighter(string)
	*
	*	Returns the memory address of a fighter whose name is equal to the given 
	*	name.  Returns NULL if no fighter is found with the given name.
	*
	*	Return a memory address if a fighter is found; NULL otherwise.
	*/
}
		
int Arena::getSize()
{
	return allFighters.size();
	/*
	*	getSize()
	*
	*	Returns the number of fighters in the arena.
	*
	*	Return a non-negative integer.
	*/
}