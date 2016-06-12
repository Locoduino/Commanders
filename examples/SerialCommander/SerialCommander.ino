/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <Serial commander demo>
*************************************************************/

#include "Commanders.h"

SERIAL_COMMANDER(Serial);

void CommandersEventHandler(unsigned long inId, COMMANDERS_EVENT_TYPE inEvent, int inData)
{
	Commanders::printEvent(inId, inEvent, inData);
}

void setup()
{
	Commanders::SetStatusLedPin(LED_BUILTIN);
	Commanders::SetEventHandler(CommandersEventHandler);

	SerialCommander.begin(115200);	// Baud rate.
}

void loop()
{
	Commanders::loop();
}
