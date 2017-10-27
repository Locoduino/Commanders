/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <Dcc commander demo>
*************************************************************/

#include "Commanders.h"

#ifdef NO_DCCCOMMANDER
#error To be able to compile this sample,the line #define NO_DCCCOMMANDER must be commented in Commanders.h
#endif

void ReceiveEvent(unsigned long inId, COMMANDERS_EVENT_TYPE inEvent, int inData)
{
	PRINT_EVENT_DCC(inId, inEvent, inData);
}

//////////////////////////////////
//
// Setup
//
void setup()
{
	Commanders::begin(ReceiveEvent, LED_BUILTIN);

	// Manufacturer ID, Product ID, interrupt, use led as Dcc monitor
	DccCommander.begin(0x0, 0x0, digitalPinToInterrupt(3), true);
}

void loop()
{
	Commanders::loop();
}
