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
		/*
		for (int i = 0; i < Accessories::AccessoriesFullList.AccessoriesAddCounter; i++)
			Accessories::AccessoriesFullList.pAccessoriesFullList[i]->DCCToggle(realAddress, data);
		for (int i = 0; i < AccessoryGroup::StaticData.AccessoryGroupAddCounter; i++)
			AccessoryGroup::StaticData.pAccessoryGroupFullList[i]->DCCToggle(realAddress, data);
			*/
	}
}
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Buttons accessory handler 
//
void AccessoryHandler(unsigned long inId, bool inAnalogData, int inData)
{	
	bool found = false;

	/*
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
	*/
}

void BasicsCommanders_StartSetup()
{
#ifdef DEBUG_MODE
	Serial.begin(115200);

	Serial.println(F(""));
	Serial.println(F("Basics Commanders V0.20."));
	Serial.println(F("Developed by Thierry Paris."));
	Serial.println(F(""));

	Serial.println(F("*** Setup started."));
#endif
	Commander::EventHandler = AccessoryHandler;
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
