/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <Base Commander>
*************************************************************/

#include "Commanders.h"

CommandersEventHandlerFunction Commander::EventHandler = 0;
Commander *Commander::pFirstCommander = 0;
GPIO_pin_t Commander::StatusLedPin = DP_INVALID;

#ifdef DEBUG_MODE
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

void Commander::RaiseEvent(unsigned long inId, COMMANDERS_EVENT_TYPE inEvent, int inData)
{
	Commander::StatusBlink();
	if (Commander::EventHandler != 0)
		Commander::EventHandler(inId, inEvent, inData);
}

static unsigned long start_status_led = 0;
void Commander::StatusBlink()
{
	if (Commander::StatusLedPin != DP_INVALID && start_status_led == 0)
	{
		start_status_led = millis();
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

	if (Commander::StatusLedPin != DP_INVALID && start_status_led > 0 && millis() - start_status_led > 1000)
	{
		digitalWrite2f(Commander::StatusLedPin, LOW);
		start_status_led = 0;
	}

	return UNDEFINED_ID;
}

#ifdef DEBUG_MODE
void Commander::CheckIndex(byte inIndex, const __FlashStringHelper *inFunc)
{
}
#endif
