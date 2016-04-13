/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <Base functions of the library>
*************************************************************/

#include "Commanders.h"

COMMANDERS_EVENT_TYPE Commanders::lastEventType;
int Commanders::lastEventData;

void Commanders::StartSetup(int inStatusLedPin)
{
#ifdef DEBUG_MODE
	Serial.begin(115200);

	Serial.println(F(""));
	Serial.println(F("Commanders V0.64."));
	Serial.println(F("Developed by Thierry Paris."));
	Serial.println(F(""));

	Serial.println(F("*** Setup started."));
#endif

	if (inStatusLedPin != 0)
	{
		Commander::StatusLedPin = Arduino_to_GPIO_pin(inStatusLedPin);
		pinMode2f(Commander::StatusLedPin, OUTPUT);
	}
		
#ifndef NO_DCCCOMMANDER
#ifndef VISUALSTUDIO
	DccCommander::SetAccessoryDecoderPacketHandler(DccCommander::DccAccessoryDecoderPacket);
#endif
#endif
}

void Commanders::StartSetup(CommandersEventHandlerFunction func, int inStatusLedPin)
{
	Commander::EventHandler = func;
	Commanders::StartSetup(inStatusLedPin);
}

void Commanders::EndSetup()
{
#ifdef DEBUG_MODE
	Serial.println(F("*** Setup Commanders Finished."));
#endif
}

unsigned long Commanders::loop()
{
	return Commander::loops();
}



