/*************************************************************
project: <Universal Accessory Decoder>
author: <Thierry PARIS>
description: <Base functions of the library>
*************************************************************/

#include "BasicsCommanders.h"

const BasicsCommanderEvent EmptyEvent = BasicsCommanderEvent(UNDEFINED_ID, COMMANDERS_EVENT_NONE, 0);

void BasicsCommanders_StartSetup()
{
#ifdef DEBUG_MODE
	Serial.begin(115200);

	Serial.println(F(""));
	Serial.println(F("Basics Commanders V0.40."));
	Serial.println(F("Developed by Thierry Paris."));
	Serial.println(F(""));

	Serial.println(F("*** Setup started."));
#endif

#ifndef NO_DCCCOMMANDER
	DccCommander::SetBasicAccessoryDecoderPacketHandler(DccCommander::DccAccessoryDecoderPacket);
#endif
}

void BasicsCommanders_StartSetup(CommandersEventHandlerFunction func)
{
	Commander::EventHandler = func;
	BasicsCommanders_StartSetup();
}

void BasicsCommanders_EndSetup()
{
#ifdef DEBUG_MODE
	Serial.println(F("*** Setup Basics Commanders Finished."));
#endif
}

BasicsCommanderEvent BasicsCommanders_Loop()
{
	return Commander::Loops();
}

