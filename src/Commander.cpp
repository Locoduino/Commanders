/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <Base Commander>
*************************************************************/

#include "Commanders.h"

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
#ifdef COMMANDERS_DEBUG_MODE
		Commanders::beginSerial();

		Serial.println(F(""));
		Serial.println(F("Commanders V0.98"));
		Serial.println(F("Developed by Thierry Paris."));
		Serial.println(F("(c) Locoduino 2016"));
		Serial.println(F(""));

		Serial.println(F("*** Setup Commanders started."));
#endif
		return;
	}

	Commander *pCurr = Commander::pFirstCommander;

	while (pCurr->pNextCommander != NULL)
		pCurr = pCurr->pNextCommander;

	pCurr->pNextCommander = inCommander;
}

void Commander::CommanderPriorityLoop()
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
		unsigned long ret = pCurr->loop();
		if (ret != UNDEFINED_ID)
			return ret;
		pCurr = pCurr->pNextCommander;
	}

	return UNDEFINED_ID;
}

#ifdef COMMANDERS_DEBUG_MODE
#ifdef ARDUINO_ARCH_SAM
void Commander::CheckIndex(uint8_t inIndex, const char *inFunc)
{
}
#else
void Commander::CheckIndex(uint8_t inIndex, const __FlashStringHelper *inFunc)
{
}
#endif
#endif
