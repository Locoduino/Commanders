/*************************************************************
project: <Universal Accessory Decoder>
author: <Thierry PARIS>
description: <Base functions of the library>
*************************************************************/

#include "UniversalAccessoryDecoder.h"

#ifndef UAD_VC
#include "MemoryFree.hpp"
#endif

static int freemem = 0;

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
		for (int i = 0; i < Accessories::AccessoriesFullList.AccessoriesAddCounter; i++)
			Accessories::AccessoriesFullList.pAccessoriesFullList[i]->DCCToggle(realAddress, data);
		for (int i = 0; i < AccessoryGroup::StaticData.AccessoryGroupAddCounter; i++)
			AccessoryGroup::StaticData.pAccessoryGroupFullList[i]->DCCToggle(realAddress, data);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Buttons accessory handler 
//
void AccessoryHandler(unsigned long inId, bool inAnalogData, int inData)
{	
	bool found = false;

	if (inAnalogData)
	{
		for (int i = 0; i < Accessories::AccessoriesFullList.AccessoriesAddCounter; i++)
			found |= Accessories::AccessoriesFullList.pAccessoriesFullList[i]->MovePosition(DCCID(inId), DCCACCESSORY(inId), inData);
	}
	else
	{
		for (int i = 0; i < Accessories::AccessoriesFullList.AccessoriesAddCounter; i++)
			found |= Accessories::AccessoriesFullList.pAccessoriesFullList[i]->DCCToggle(DCCID(inId), DCCACCESSORY(inId));
		for (int j = 0; j < AccessoryGroup::StaticData.AccessoryGroupAddCounter; j++)
			found |= AccessoryGroup::StaticData.pAccessoryGroupFullList[j]->DCCToggle(DCCID(inId), DCCACCESSORY(inId));
	}
}

void UAD_StartSetup()
{
#ifdef DEBUG_MODE
	// Done by the SerialCommander setup...
	Serial.begin(115200);
	// Just for let the time to the PIC to initialize internals...
	delay(500);

	Serial.println(F(""));
	Serial.println(F("Universal Accessories Decoder V4.10."));
	Serial.println(F("Developed by Thierry Paris."));
	Serial.println(F(""));

	Serial.println(F("*** Setup started."));
	freemem = freeMemory();
#endif
	Commander::EventHandler = AccessoryHandler;
	DccCommander::SetBasicAccessoryDecoderPacketHandler(DccAccessoryDecoderPacket);
}

void UAD_EndSetup()
{
#ifdef DEBUG_MODE
	Serial.print(F("*** Setup Finished."));
	Serial.print(F("   Memory used = "));
	Serial.print(freemem - freeMemory());
	Serial.println(F(" bytes"));
#endif
}

void UAD_Loop()
{
	Commander::Loops();
	Accessories::Loops();
}
