/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <Demo Serial sample by macros>
*************************************************************/

#include "Commanders.h"

DECLARE_SERIAL_COMMANDER(Serial, 115200);

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
	START_SERIAL_COMMANDER_SETUPEVENT(CommandersEventHandler);

	END_COMMANDERS_SETUP;
}

void loop()
{
	COMMANDERS_LOOP;
}
