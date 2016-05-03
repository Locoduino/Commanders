/*************************************************************
project: <Carte 8 Servos Locoduino by UAD>
author: <Thierry PARIS>
*************************************************************/

#ifdef VISUALSTUDIO
#include "../../Commanders/src/Commanders.h"
#else
#include <Commanders.h>
#endif

CANCommander canCommander;

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

	canCommander.begin(10, CAN_125KBPS, digitalPinToInterrupt(3), 0x10);

	Commanders::EndSetup();
}

void loop()
{
	Commanders::loop();
}
