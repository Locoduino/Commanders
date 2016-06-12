/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <I2C commander demo>
*************************************************************/

#include <Commanders.h>

void ReceiveEvent(unsigned long inId, COMMANDERS_EVENT_TYPE inEventType, int inEventData)
{
    Commanders::printEvent(inId, inEventType, inEventData);
}

//////////////////////////////////
//
// Setup
//
void setup()
{
	Commanders::SetStatusLedPin(LED_BUILTIN);
	Commanders::SetEventHandler(ReceiveEvent);

	I2CCommander.begin(0x10);	// I2C Slave identifier.
}

void loop()
{
	Commanders::loop();
}
