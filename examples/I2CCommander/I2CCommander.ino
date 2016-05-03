/*************************************************************
project: <Carte 8 Servos Locoduino by UAD>
author: <Thierry PARIS>
*************************************************************/

#include <Commanders.h>

I2CCommander i2cCommander;

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

	i2cCommander.begin(0x10);

	Commanders::EndSetup();
}

void loop()
{
	Commanders::loop();
}
