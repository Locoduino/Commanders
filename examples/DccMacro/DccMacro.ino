/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <Demo sample by macros>
*************************************************************/

#include "Commanders.h"

/* INTERRUPT values as macro argument :
Board			int.0	int.1	int.2	int.3	int.4	int.5
Uno, Ethernet	2		3
Mega2560		2		3		21		20		19		18
Leonardo		3		2		0		1		7
*/
DECLARE_DCC_COMMANDER(3);  // interrupt number

void CommandersEventHandler(unsigned long inId, COMMANDERS_EVENT_TYPE inEvent, int inData)
{
	Serial.print(F("Commander event : Address : "));
	Serial.print(DCCID(inId), DEC);
	Serial.print(F(" / "));
	Serial.print(DCCACCESSORY(inId), DEC);
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
	START_DCC_COMMANDER_SETUPEVENT(CommandersEventHandler);

	END_COMMANDER_SETUP;
}

void loop()
{
	COMMANDER_LOOP;
}
