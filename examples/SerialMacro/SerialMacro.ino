/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <Demo Serial sample by macros>
*************************************************************/

#include "Commanders.h"

DECLARE_SERIAL_COMMANDER(Serial, 115200);

void CommandersEventHandler(unsigned long inId, COMMANDERS_EVENT_TYPE inEvent, int inData)
{
	Commanders::printEvent(inId, inEvent, inData);
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
