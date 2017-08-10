/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <CAN commander demo>
*************************************************************/

#include <Commanders.h>

#ifdef NO_CANCOMMANDER
#error To be able to compile this sample,the line #define NO_CANCOMMANDER must be commented in Commanders.h
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

	CANCommander.begin(10, CAN_500KBPS, digitalPinToInterrupt(3), 0x10); // SPI pin, CAN speed, interrupt number, CAN message prefix.
}

void loop()
{
	Commanders::loop();
}
