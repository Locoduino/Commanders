/*************************************************************
project: <Universal Accessory Decoder>
author: <Thierry PARIS>
description: <Base functions of the library>
*************************************************************/

#include "BasicsCommanders.h"

#ifndef NO_DCCCOMMANDER
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// DCC Basic accessory packet handler 
//
void DccAccessoryDecoderPacket(int address, boolean activate, byte data)
{
	int realAddress = address;

	if (!DccCommander::UseRawDccAddresses)
	{
		realAddress -= 1;
		realAddress *= 4;
		realAddress += 1;
		realAddress += (data & 0x06) >> 1;
		data = data % 2;
	}

#ifdef DEBUG_VERBOSE_MODE
	Serial.print(F("Dcc packet found : Address : "));
	Serial.print(realAddress);
	Serial.print(F(" / "));
	Serial.print(data, DEC);
	Serial.print(F(" / "));
	Serial.println(activate, DEC);
#endif

	if (DccCommander::func_BasicAccPacket)
		(DccCommander::func_BasicAccPacket)(realAddress, activate, data);
	else
	{
		Commander::EventHandler(DCCINT(realAddress, data), EVENT_SELECTED, 0);
		/*
		for (int i = 0; i < Accessories::AccessoriesFullList.AccessoriesAddCounter; i++)
			Accessories::AccessoriesFullList.pAccessoriesFullList[i]->DCCToggle(realAddress, data);
		for (int i = 0; i < AccessoryGroup::StaticData.AccessoryGroupAddCounter; i++)
			AccessoryGroup::StaticData.pAccessoryGroupFullList[i]->DCCToggle(realAddress, data);
			*/
	}
}
#endif

void BasicsCommanders_StartSetup(EventHandlerFunction func)
{
#ifdef DEBUG_MODE
	Serial.begin(115200);

	Serial.println(F(""));
	Serial.println(F("Basics Commanders V0.30."));
	Serial.println(F("Developed by Thierry Paris."));
	Serial.println(F(""));

	Serial.println(F("*** Setup started."));
#endif

	Commander::EventHandler = func;

#ifndef NO_DCCCOMMANDER
	DccCommander::SetBasicAccessoryDecoderPacketHandler(DccAccessoryDecoderPacket);
#endif
}

void BasicsCommanders_EndSetup()
{
#ifdef DEBUG_MODE
	Serial.println(F("*** Setup Basics Commanders Finished."));
#endif
}

void BasicsCommanders_Loop()
{
	Commander::Loops();
}
