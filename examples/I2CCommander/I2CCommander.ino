/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <I2C commander demo>
*************************************************************/

#include <Commanders.h>

#ifdef NO_I2CCOMMANDER
#error To be able to compile this sample,the line #define NO_I2CCOMMANDER must be commented in Commanders.h
#endif

#if defined(ARDUINO_ARCH_ESP32)
#define LED_BUILTIN 2
#endif

void ReceiveEvent(unsigned long inId, COMMANDERS_EVENT_TYPE inEventType, int inEventData)
{
	COMMANDERS_PRINT_EVENT(inId, inEventType, inEventData);
}

//////////////////////////////////
//
// Setup
//
void setup()
{
	Commanders::begin(ReceiveEvent, LED_BUILTIN);

	I2CCommander.begin(0x10);	// I2C Slave identifier.
}

void loop()
{
	Commanders::loop();
}
