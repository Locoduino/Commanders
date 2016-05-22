/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <Base Commander>
*************************************************************/

#include "Commanders.h"

Commander *Commander::pFirstCommander = 0;
GPIO_pin_t Commander::StatusLedPin = DP_INVALID;
unsigned int Commander::BlinkDelay = 1000;
unsigned long Commander::StartStatusLed = 0;

#ifdef COMMANDERS_DEBUG_MODE
#define CHECK(val, text)	CheckIndex(val, F(text))
#else
#define CHECK(val, text)
#endif

void Commander::AddCommander(Commander *inCommander)
{
	if (Commander::pFirstCommander == 0)
	{
		Commander::pFirstCommander = inCommander;
		return;
	}

	Commander *pCurr = Commander::pFirstCommander;

	while (pCurr->pNextCommander != 0)
		pCurr = pCurr->pNextCommander;

	pCurr->pNextCommander = inCommander;
}

void Commander::CommanderPriorityLoop()
{
	Commander *pCurr = Commander::pFirstCommander;

	while (pCurr != 0)
	{
		pCurr->PriorityLoop();
		pCurr = pCurr->pNextCommander;
	}
}

unsigned long Commander::RaiseEvent(unsigned long inId, COMMANDERS_EVENT_TYPE inEvent, int inData)
{
	Commander::StatusBlink();
	Commanders::SetLastEventType(inEvent);
	Commanders::SetLastEventData(inData);
	if (Commanders::EventHandler != 0)
		Commanders::EventHandler(inId, inEvent, inData);

	return inId;
}

void Commander::StatusBlink()
{
	if (Commander::StatusLedPin != DP_INVALID && StartStatusLed == 0)
	{
		StartStatusLed = millis();
		digitalWrite2f(Commander::StatusLedPin, HIGH);
	}
}

unsigned long Commander::loops()
{
	Commander *pCurr = Commander::pFirstCommander;

	while (pCurr != 0)
	{
		unsigned long ret = pCurr->loop();
		if (ret != UNDEFINED_ID)
			return ret;
		pCurr = pCurr->pNextCommander;
	}

	if (Commander::StatusLedPin != DP_INVALID && StartStatusLed > 0 && millis() - StartStatusLed > BlinkDelay)
	{
		digitalWrite2f(Commander::StatusLedPin, LOW);
		StartStatusLed = 0;
	}

	return UNDEFINED_ID;
}

#ifdef COMMANDERS_DEBUG_MODE
void Commander::CheckIndex(byte inIndex, const __FlashStringHelper *inFunc)
{
}
#endif
