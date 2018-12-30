/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <Dcc commander demo>
*************************************************************/

#include "Commanders.h"

#ifdef NO_DCCCOMMANDERNMRA
#error To be able to compile this sample, the line #define NO_DCCCOMMANDERNMRA must be commented in Commanders.h
#endif

#ifndef COMMANDERS_DEBUG_MODE
#error To be able to execute this sample, the line #define COMMANDERS_DEBUG_MODE must be uncommented in Commanders.h
#endif

#if defined(ARDUINO_ARCH_ESP32)
#define LED_BUILTIN 2
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
  Serial.begin(115200);
	Commanders::begin(ReceiveEvent, LED_BUILTIN);

	// Manufacturer ID, Product ID, pin for interrupt, use led as Dcc monitor, use Roco mode.
	DccCommanderNMRA.beginByPin(0x0, 0x0, 3, false, false);
}

void loop()
{
	Commanders::loop();
}
