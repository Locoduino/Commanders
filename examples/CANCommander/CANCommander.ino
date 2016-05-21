/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
*************************************************************/

#ifdef VISUALSTUDIO
#include "../../Commanders/src/Commanders.h"
#else
#include <Commanders.h>
#endif

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
	Commanders::StartSetup(ReceiveEvent, LED_BUILTIN);

	CANCommander.begin(10, CAN_500KBPS, digitalPinToInterrupt(3), 0x10);

	Commanders::EndSetup();
}

void loop()
{
	Commanders::loop();
}
