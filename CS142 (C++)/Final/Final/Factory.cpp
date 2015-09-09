#include "Factory.h"
#include "Bank.h"


Factory::Factory(void)
{
}


Factory::~Factory(void)
{
}


BankInterface* Factory::createBank()
{
	return new Bank();
}