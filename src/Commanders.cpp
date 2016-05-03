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
#ifdef COMMANDERS_DEBUG_MODE
	Serial.begin(115200);

	Serial.println(F(""));
	Serial.println(F("Commanders V0.82"));
	Serial.println(F("Developed by Thierry Paris."));
	Serial.println(F("(c) Locoduino 2016"));
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
#ifdef COMMANDERS_DEBUG_MODE
	Serial.println(F("*** Setup Commanders Finished."));
#endif
}

#ifdef COMMANDERS_DEBUG_MODE
void Commanders::printEvent(unsigned long inId, COMMANDERS_EVENT_TYPE inEventType, int inEventData)
{
	Serial.print(F("Commander event : Address : "));
	Serial.print(inId, DEC);
	Serial.print(F(" / "));
	switch (inEventType)
	{
	case COMMANDERS_EVENT_NONE:			Serial.println(F("NONE"));		break;
	case COMMANDERS_EVENT_TOGGLE:		Serial.println(F("TOGGLE"));		break;
	case COMMANDERS_EVENT_MOVELEFT:		Serial.println(F("MOVELEFT"));		break;
	case COMMANDERS_EVENT_MOVERIGHT:	Serial.println(F("MOVERIGHT"));		break;
	case COMMANDERS_EVENT_MOVESTOP:		Serial.println(F("MOVESTOP"));		break;
	case COMMANDERS_EVENT_ABSOLUTEMOVE:
		Serial.print(F("ABSOLUTEMOVE : "));
		Serial.println(inEventData, DEC);
		break;
	case COMMANDERS_EVENT_RELATIVEMOVE:
		Serial.print(F("RELATIVEMOVE : "));
		Serial.println(inEventData, DEC);
		break;
	}
}
#endif

unsigned long Commanders::loop()
{
	return Commander::loops();
}



