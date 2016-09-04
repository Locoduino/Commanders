/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <Serial commander demo>
*************************************************************/

#include "Commanders.h"

SERIAL_COMMANDER(Serial);

void ReceiveEvent(unsigned long inId, COMMANDERS_EVENT_TYPE inEvent, int inData)
{
	Commanders::printEvent(inId, inEvent, inData);
}

void setup()
{
	Serial.begin(115200);	// Baud rate.
	Commanders::begin(ReceiveEvent, LED_BUILTIN);

	SerialCommander.begin();
}

void loop()
{
	Commanders::loop();
}
