/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <Base Commander>
*************************************************************/

#include <Commanders.h>

Commander *Commander::pFirstCommander = 0;

#ifdef COMMANDERS_DEBUG_MODE
#define CHECK(val, text)	CheckIndex(val, F(text))
#else
#define CHECK(val, text)
#endif

void Commander::AddCommander(Commander *inCommander)
{
	if (Commander::pFirstCommander == NULL)
	{
		Commander::pFirstCommander = inCommander;
		return;
	}

	Commander *pCurr = Commander::pFirstCommander;

	while (pCurr->pNextCommander != NULL)
		pCurr = pCurr->pNextCommander;

	pCurr->pNextCommander = inCommander;
}

void Commander::BeforeFirstLoops()
{
	Commander *pCurr = Commander::pFirstCommander;

	while (pCurr != NULL)
	{
		pCurr->beforeFirstLoop();
		pCurr = pCurr->pNextCommander;
	}
}

void Commander::CommanderPriorityLoops()
{
	Commander *pCurr = Commander::pFirstCommander;

	while (pCurr != NULL)
	{
		pCurr->PriorityLoop();
		pCurr = pCurr->pNextCommander;
	}
}

unsigned long Commander::loops()
{
	Commander *pCurr = Commander::pFirstCommander;

	while (pCurr != NULL)
	{
		Commander::CommanderPriorityLoops();
		unsigned long ret = pCurr->loop();
		if (ret != UNDEFINED_ID)
			return ret;
		pCurr = pCurr->pNextCommander;
	}

	return UNDEFINED_ID;
}

#ifdef COMMANDERS_PRINT_COMMANDERS
void Commander::printCommanders()
{
	Serial.println(F("********* Commanders List"));
	Commander *pCurr = Commander::pFirstCommander;

	while (pCurr != NULL)
	{
		pCurr->printCommander();
		pCurr = pCurr->pNextCommander;
	}
	Serial.println(F("********* End of List"));
}
#endif
