/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <Dcc Commander>
*************************************************************/

#ifndef NO_DCCCOMMANDER
#include "DccCommander.hpp"

#ifdef VISUALSTUDIO
DCC_Decoder DCC_Decoder::DCCInstance;
#endif

unsigned long DccCommander::LastDccId = UNDEFINED_ID;
boolean DccCommander::UseRawDccAddresses;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// DCC accessory packet handler 
//
void DccCommander::DccAccessoryDecoderPacket(int address, boolean activate, byte data)
{
	int realAddress = address;

#ifdef COMMANDERS_DEBUG_MODE
#ifdef DEBUG_VERBOSE_MODE
	Serial.print(F("Dcc packet found : real data : "));
	Serial.print(realAddress);
	Serial.print(F(" / "));
	Serial.print(data, DEC);
	Serial.print(F(" / "));
	Serial.print(activate, DEC);
#endif
#endif

	if (!DccCommander::UseRawDccAddresses)
	{
		realAddress -= 1;
		realAddress *= 4;
		realAddress += 1;
		realAddress += (data & 0x06) >> 1;
		data = data % 2;
	}

#ifdef COMMANDERS_DEBUG_MODE
#ifdef DEBUG_VERBOSE_MODE
	Serial.print(F("  converted : "));
	Serial.print(realAddress);
	Serial.print(F(" / "));
	Serial.print(data, DEC);
	Serial.print(F(" / "));
	Serial.println(activate, DEC);
#endif
#endif

	// Sent packets are (on my MS2 !) :
	// id / data / 1
	// id / data / 1		 
	// id / data / 1		 
	// id / data / 0
	// The last byte is to activate for a while (three times at 1 !) and then desactivate the motor !
	// DccCommander will react only on the desactivate flag to avoid double events.
	if (activate == 0)
	{
		if (DccCommander::func_AccPacket)
			(DccCommander::func_AccPacket)(realAddress, activate, data);
		else
		{
			Commander::RaiseEvent(DCCID(realAddress), data ? COMMANDERS_EVENT_MOVELEFT : COMMANDERS_EVENT_MOVERIGHT, data);

			DccCommander::LastDccId = DCCID(realAddress);
		}
	}
}

#ifdef COMMANDERS_DEBUG_MODE
#define CHECK(val, text)	CheckIndex(val, F(text))
#else
#define CHECK(val, text)
#endif

#ifdef COMMANDERS_DEBUG_MODE
void DccCommander::CheckIndex(unsigned char inIndex, const __FlashStringHelper *inFunc)
{
}
#endif

bool status = false;
void StatusBlink_handler()
{
	digitalWrite2f(Commander::StatusLedPin, status ? HIGH : LOW);
	status = !status;
}

void DccCommander::begin(int i, int j, int k, boolean inInterruptMonitor, boolean inUseRawDccAddresses)
{
	DCC.beginDecoder(i, j, k);
	DccCommander::UseRawDccAddresses = inUseRawDccAddresses;

	DCC.SetBasicAccessoryDecoderPacketHandler(DccAccessoryDecoderPacket, true);

	if (inInterruptMonitor)
		DCC.SetInterruptMonitor(StatusBlink_handler);
}

void DccCommander::PriorityLoop()
{
	DccCommander::loop();
}

#define MINTIME	2
#ifdef COMMANDERS_DEBUG_MODE
#ifdef DEBUG_VERBOSE_MODE
static int countLoop = 0;
#endif
#endif

static unsigned long start = 0;

#define ELAPSEDTIME	((unsigned long) -2)

unsigned long DccCommander::loop()
{
	if (start == 0)
		start = millis();

	DCC.loop();
#ifdef COMMANDERS_DEBUG_MODE
#ifdef DEBUG_VERBOSE_MODE
	countLoop++;
#endif
#endif

	if (millis() - start > MINTIME)
	{
#ifdef COMMANDERS_DEBUG_MODE
#ifdef DEBUG_VERBOSE_MODE
		Serial.print("DCC commander loop : ");
		Serial.println(countLoop, DEC);
		countLoop = 0;
#endif
#endif
		start = 0;
		unsigned long last = this->LastDccId;
		this->LastDccId = UNDEFINED_ID;
		Commanders::SetLastEventType(COMMANDERS_EVENT_TOGGLE);
		Commanders::SetLastEventData(0);
		return last;
	}

	return UNDEFINED_ID;
}

DccAccDecoderPacket DccCommander::func_AccPacket = NULL;

void DccCommander::SetAccessoryDecoderPacketHandler(DccAccDecoderPacket func)
{
	DccCommander::func_AccPacket = func;
}

#ifdef COMMANDERS_DEBUG_MODE
void DccCommander::printEvent(unsigned long inId, COMMANDERS_EVENT_TYPE inEventType, int inEventData)
{
	Serial.print(F("DCC Commander event : Address : "));
	Serial.print(DCCID(inId), DEC);
	Serial.print(F(" / "));
	Serial.print(DCCACTIVATION(inId), DEC);
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
	case COMMANDERS_EVENT_CONFIG:
		Serial.print(F("CONFIG : "));
		Serial.print(COMMANDERSCONFIGADDRESS(inEventData), DEC);
		Serial.print(F(" / "));
		Serial.println(COMMANDERSCONFIGVALUE(inEventData), DEC);
		break;
	}
}
#endif

#endif
