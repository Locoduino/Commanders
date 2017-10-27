/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <Serial commander demo>
*************************************************************/

#include "Commanders.h"

#ifdef NO_SERIALCOMMANDER
#error To be able to compile this sample,the line #define NO_SERIALCOMMANDER must be commented in Commanders.h
#endif

SERIAL_COMMANDER(Serial);

void ReceiveEvent(unsigned long inId, COMMANDERS_EVENT_TYPE inEvent, int inData)
{
	COMMANDERS_PRINT_EVENT(inId, inEvent, inData);
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
