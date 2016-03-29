/*************************************************************
project: <Universal Accessory Decoder>
author: <Thierry PARIS>
description: <Base functions of the library>
*************************************************************/

#include "Commanders.h"

static COMMANDERS_EVENT_TYPE lastEventType;
static int lastEventData;

void Commanders_StartSetup()
{
#ifdef DEBUG_MODE
	Serial.begin(115200);

	Serial.println(F(""));
	Serial.println(F("Commanders V0.62."));
	Serial.println(F("Developed by Thierry Paris."));
	Serial.println(F(""));

	Serial.println(F("*** Setup started."));
#endif

#ifndef NO_DCCCOMMANDER
#ifndef VISUALSTUDIO
	DccCommander::SetAccessoryDecoderPacketHandler(DccCommander::DccAccessoryDecoderPacket);
#endif
#endif
}

void Commanders_StartSetup(CommandersEventHandlerFunction func)
{
	Commander::EventHandler = func;
	Commanders_StartSetup();
}

void Commanders_EndSetup()
{
#ifdef DEBUG_MODE
	Serial.println(F("*** Setup Commanders Finished."));
#endif
}

unsigned long Commanders_Loop()
{
	return Commander::Loops();
}

COMMANDERS_EVENT_TYPE Commanders_GetLastEventType()
{
	return lastEventType;
}

int Commanders_GetLastEventData()
{
	return lastEventData;
}

void Commanders_SetLastEventType(COMMANDERS_EVENT_TYPE inType)
{
	lastEventType = inType;
	lastEventData = 0;
}

void Commanders_SetLastEventData(int inData)
{
	lastEventData = inData;
}



