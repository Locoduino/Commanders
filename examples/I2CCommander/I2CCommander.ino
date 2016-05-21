/*************************************************************
project: <Carte 8 Servos Locoduino by UAD>
author: <Thierry PARIS>
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
	Commanders::StartSetup(ReceiveEvent, LED_BUILTIN);

	I2CCommander.begin(0x10);

	Commanders::EndSetup();
}

void loop()
{
	Commanders::loop();
}
