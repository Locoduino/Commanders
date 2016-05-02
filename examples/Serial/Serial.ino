/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <Demo Serial sample by macros>
*************************************************************/

#include "Commanders.h"

SERIAL_COMMANDER(Serial);

SerialCommander commander;

void CommandersEventHandler(unsigned long inId, COMMANDERS_EVENT_TYPE inEvent, int inData)
{
	Commanders::printEvent(inId, inEvent, inData);
}

void setup()
{
	Commanders::StartSetup(CommandersEventHandler);
	commander.begin(115200);

	Commanders::EndSetup();
}

void loop()
{
	Commanders::loop();
}
