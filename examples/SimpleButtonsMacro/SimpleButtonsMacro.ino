/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <Demo sample by macros>
*************************************************************/

#include "Commanders.h"

DECLARE_BUTTONS_COMMANDER;

void CommandersEventHandler(unsigned long inId, COMMANDERS_EVENT_TYPE inEvent, int inData)
{
	Commanders::printEvent(inId, inEvent, inData);
}

void setup()
{
	START_BUTTONS_COMMANDER_SETUPEVENT(CommandersEventHandler);

	PUSH_BUTTON(pLeft, 20, 30);
	PUSH_BUTTON(pDc, 21, 34);
	PUSH_BUTTON(pRight, 22, 32);
	PUSH_BUTTON(pEPS, 23, 36);
	PUSH_BUTTON(pTJD, 24, 38);

	END_COMMANDERS_SETUP;
}

void loop()
{
	COMMANDERS_LOOP;
}
