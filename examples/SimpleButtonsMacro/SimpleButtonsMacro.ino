/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <Demo sample by macros>
*************************************************************/

#include "Commanders.h"

DECLARE_BUTTONS_COMMANDER

void CommandersEventHandler(unsigned long inId, COMMANDERS_EVENT_TYPE inEvent, int inData)
{
	Serial.print(F("Commander event : Id : "));
	Serial.print(inId, DEC);
	Serial.print(F(" / "));
	switch (inEvent)
	{
	case COMMANDERS_EVENT_NONE:			Serial.println(F("NONE"));			break;
	case COMMANDERS_EVENT_SELECTED:		Serial.println(F("SELECTED"));		break;
	case COMMANDERS_EVENT_ABSOLUTEMOVE:	
		Serial.print(F("ABSOLUTEMOVE : "));	
		Serial.println(inData, DEC);
		break;
	case COMMANDERS_EVENT_RELATIVEMOVE:	
		Serial.print(F("RELATIVEMOVE : "));	
		Serial.println(inData, DEC);
		break;
	}
}

void setup()
{
	START_BUTTONS_COMMANDER_SETUPEVENT(CommandersEventHandler);

	PUSH_BUTTON(pLeft, 20UL, 30);
	PUSH_BUTTON(pDc, 21UL, 34);
	PUSH_BUTTON(pRight, 22UL, 32);
	PUSH_BUTTON(pEPS, 23UL, 36);
	PUSH_BUTTON(pTJD, 24UL, 38);

	END_COMMANDER_SETUP;
}

void loop()
{
	COMMANDER_LOOP;
}
