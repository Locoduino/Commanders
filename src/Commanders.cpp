/*************************************************************
project: <Universal Accessory Decoder>
author: <Thierry PARIS>
description: <Base functions of the library>
*************************************************************/

#include "Commanders.h"

const CommanderEvent EmptyEvent = CommanderEvent(UNDEFINED_ID, COMMANDERS_EVENT_NONE, 0);

void Commanders_StartSetup()
{
#ifdef DEBUG_MODE
	Serial.begin(115200);

	Serial.println(F(""));
	Serial.println(F("Commanders V0.50."));
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

CommanderEvent Commanders_Loop()
{
	return Commander::Loops();
}

