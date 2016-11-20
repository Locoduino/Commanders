/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <Dcc Commander>
*************************************************************/

#include "Commanders.h"

#ifndef NO_DCCCOMMANDER

#ifdef VISUALSTUDIO
DCC_Decoder DCC_Decoder::DCCInstance;
#endif

DccAccDecoderPacket DccCommanderClass::func_AccPacket;
DccCommanderClass *DccCommanderClass::pDccCommander;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// DCC accessory packet handler 
//
void DccCommanderClass::DccAccessoryDecoderPacket(int address, boolean activate, uint8_t data)
{
	int realAddress = address;

#ifdef COMMANDERS_DEBUG_MODE
#ifdef COMMANDERS_DEBUG_VERBOSE_MODE
	Serial.print(F("Dcc packet found : real data : "));
	Serial.print(realAddress);
	Serial.print(F(" / "));
	Serial.print(data, DEC);
	Serial.print(F(" / "));
	Serial.print(activate, DEC);
#endif
#endif

	if (!DccCommander.UseRawDccAddresses)
	{
		realAddress -= 1;
		realAddress *= 4;
		realAddress += 1;
		realAddress += (data & 0x06) >> 1;
		data = data % 2;
	}

#ifdef COMMANDERS_DEBUG_MODE
#ifdef COMMANDERS_DEBUG_VERBOSE_MODE
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
	if (activate == false)
	{
#ifdef COMMANDERS_DEBUG_MODE
		Serial.print("DccCommander packet received :");
		Serial.print(realAddress);
		Serial.print(F(" / "));
		Serial.println(data, DEC);
#endif
		if (DccCommanderClass::func_AccPacket)
			(DccCommanderClass::func_AccPacket)(realAddress, activate, data);
		else
		{
			Commanders::RaiseEvent(DCCINT(realAddress, data), COMMANDERS_EVENT_MOVE, data ? COMMANDERS_MOVE_LEFT : COMMANDERS_MOVE_RIGHT);

			DccCommander.LastDccId = DCCINT(realAddress, data);
		}
	}
}

#ifdef COMMANDERS_DEBUG_MODE
#define CHECK(val, text)	CheckIndex(val, F(text))
#else
#define CHECK(val, text)
#endif

#ifdef COMMANDERS_DEBUG_MODE
void DccCommanderClass::CheckIndex(unsigned char inIndex, const __FlashStringHelper *inFunc)
{
}
#endif

bool status = false;
void StatusBlink_handler()
{
	digitalWrite2f(Commanders::StatusLedPin, status ? HIGH : LOW);
	status = !status;
}

void DccCommanderClass::begin(int i, int j, int k, boolean inInterruptMonitor, boolean inUseRawDccAddresses)
{
	DCC.beginDecoder(i, j, k);
	this->UseRawDccAddresses = inUseRawDccAddresses;
#ifdef VISUALSTUDIO
	// In VS, the exception address is also the pin number.
	// pinMode() is just here to declare the pin used for the emulator...
	pinMode(k, OUTPUT_INTERRUPT);
#endif
	this->LastDccId = UNDEFINED_ID;

	DCC.SetBasicAccessoryDecoderPacketHandler(DccAccessoryDecoderPacket, true);

	if (inInterruptMonitor)
		DCC.SetInterruptMonitor(StatusBlink_handler);

#ifdef COMMANDERS_PRINT_COMMANDERS
	this->Interrupt = k;
#endif
}

void DccCommanderClass::PriorityLoop()
{
	DccCommanderClass::loop();
}

#define MINTIME	2
#ifdef COMMANDERS_DEBUG_MODE
#ifdef COMMANDERS_DEBUG_VERBOSE_MODE
static int countLoop = 0;
#endif
#endif

static unsigned long start = 0;

#define ELAPSEDTIME	((unsigned long) -2)

unsigned long DccCommanderClass::loop()
{
	//if (start == 0)
		//start = millis();

	DCC.loop();
#ifdef COMMANDERS_DEBUG_MODE
#ifdef COMMANDERS_DEBUG_VERBOSE_MODE
	countLoop++;
#endif
#endif

	//if (millis() - start > MINTIME)
	{
		/*
#ifdef COMMANDERS_DEBUG_MODE
#ifdef COMMANDERS_DEBUG_VERBOSE_MODE
		Serial.print("DCC commander loop : ");
		Serial.println(countLoop, DEC);
		countLoop = 0;
#endif
#endif
*/
		start = 0;
		if (this->LastDccId != UNDEFINED_ID)
		{
			unsigned long last = this->LastDccId;
			this->LastDccId = UNDEFINED_ID;
			Commanders::SetLastEventType(COMMANDERS_EVENT_MOVEPOSITIONID);
			Commanders::SetLastEventData(0);
			return last;
		}
	}

	return UNDEFINED_ID;
}

//DccAccDecoderPacket DccCommanderClass::func_AccPacket = NULL;

void DccCommanderClass::SetAccessoryDecoderPacketHandler(DccAccDecoderPacket func)
{
	DccCommanderClass::func_AccPacket = func;
}

#ifdef COMMANDERS_DEBUG_MODE
void DccCommanderClass::printEvent(unsigned long inId, COMMANDERS_EVENT_TYPE inEventType, int inEventData)
{
	Serial.print(F("DCC Commander event : Address : "));
	Serial.print(DCCID(inId), DEC);
	Serial.print(F(" / "));
	Serial.print(DCCACTIVATION(inId), DEC);
	Serial.print(F(" / "));
	switch (inEventType)
	{
	case COMMANDERS_EVENT_NONE:			Serial.println(F("NONE"));		break;
	case COMMANDERS_EVENT_TOGGLE:		Serial.println(F("TOGGLE"));	break;
	case COMMANDERS_EVENT_MOVE:			
		Serial.print(F("MOVE "));		
		switch ((COMMANDERS_MOVE_TYPE)inEventData)
		{
		case COMMANDERS_MOVE_MORE:		Serial.println(F("MORE"));		break;
		case COMMANDERS_MOVE_LESS:		Serial.println(F("LESS"));		break;
		case COMMANDERS_MOVE_STOP:		Serial.println(F("STOP"));		break;
		case COMMANDERS_MOVE_LEFT:		Serial.println(F("LEFT"));		break;
		case COMMANDERS_MOVE_RIGHT:		Serial.println(F("RIGHT"));		break;
		case COMMANDERS_MOVE_CENTER:	Serial.println(F("CENTER"));	break;
		case COMMANDERS_MOVE_TOP:		Serial.println(F("TOP"));		break;
		case COMMANDERS_MOVE_BOTTOM:	Serial.println(F("BOTTOM"));	break;
		case COMMANDERS_MOVE_STRAIGHT:	Serial.println(F("STRAIGHT"));	break;
		case COMMANDERS_MOVE_DIVERGE:	Serial.println(F("DIVERGE"));	break;
		case COMMANDERS_MOVE_ON:		Serial.println(F("ON"));		break;
		case COMMANDERS_MOVE_OFF:		Serial.println(F("OFF"));		break;
		}
		break;
	case COMMANDERS_EVENT_MOVEPOSITION:
		Serial.print(F("MOVEPOSITION : "));
		Serial.println(inEventData, DEC);
		break;
	case COMMANDERS_EVENT_MOVEPOSITIONID:
		Serial.println(F("MOVEPOSITIONID"));
		break;
	case COMMANDERS_EVENT_MOVEPOSITIONINDEX:
		Serial.print(F("MOVEPOSITIONINDEX : "));
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

#ifdef COMMANDERS_PRINT_COMMANDERS
void DccCommanderClass::printCommander()
{
	Serial.print(F("Commander: DccCommander - Interrupt: "));
	Serial.println(this->Interrupt);
}
#endif

#endif
