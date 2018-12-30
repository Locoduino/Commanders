/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <Dcc Commander>
*************************************************************/

#include <Commanders.h>

#ifdef NO_DCCCOMMANDERNMRA
#pragma message ("Commanders : No DCC NMRA commander !")
#else

DccCommanderNMRAClass *DccCommanderNMRAClass::pDccCommander;
NmraDcc DccCommanderNMRAClass::Dcc;

struct CVPair
{
	uint16_t  CV;
	uint8_t   Value;
};

CVPair FactoryDefaultCVs[] =
{
	{CV_ACCESSORY_DECODER_ADDRESS_LSB, 1},
	{CV_ACCESSORY_DECODER_ADDRESS_MSB, 0},
};

uint8_t FactoryDefaultCVIndex = 0;

void notifyCVResetFactoryDefault()
{
	// Make FactoryDefaultCVIndex non-zero and equal to num CV's to be reset 
	// to flag to the loop() function that a reset to Factory Defaults needs to be done
	FactoryDefaultCVIndex = sizeof(FactoryDefaultCVs) / sizeof(CVPair);
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// DCC accessory packet handler 
//
void notifyDccAccTurnoutBoard(uint16_t inBoardAddr, uint8_t inOutputPair, uint8_t inDirection, uint8_t inOutputPower)
{												
	DccCommanderNMRAClass::CheckLastReception(inBoardAddr, inOutputPair, inDirection, inOutputPower);
}

void DccCommanderNMRAClass::CheckLastReception(uint16_t inBoardAddr, uint8_t inOutputPair, uint8_t inDirection, uint8_t inOutputPower)
{
	int realAddress = (inBoardAddr - (DccCommanderNMRA.GetRocoMode() ?0:1)) * 4 + inOutputPair + 1;

#ifdef COMMANDERS_DEBUG_MODE
//#ifdef COMMANDERS_DEBUG_VERBOSE_MODE
	Serial.print(F("Dcc packet found : real data : "));
	Serial.print(inBoardAddr);
	Serial.print(F(" / "));
	Serial.print(inOutputPair);
	Serial.print(F(" / "));
	Serial.print(inDirection, DEC);
	Serial.print(F(" / "));
	Serial.print(inOutputPower, DEC);

	Serial.print(F("  converted : "));
	Serial.print(realAddress);
	Serial.print(F(" / "));
	Serial.print(inDirection, DEC);
	Serial.print(F(" / "));
	Serial.println(inOutputPower, DEC);
//#endif
#endif

	// Sent packets are (on my MS2 !) :
	// id / data / 1
	// id / data / 1		 
	// id / data / 1		 
	// id / data / 0
	// The last byte is to activate for a while (three times at 1 !) and then deactivate the motor !
	// DccCommanderNMRA will react only on the deactivate flag to avoid double events.
	if (inOutputPower == DccCommanderNMRA.raiseEventOnActivation)
	{
		if (DCCINT(realAddress, inDirection) == DccCommanderNMRA.lastDccId)
			if (millis() - DccCommanderNMRA.lastEventDate < DccCommanderNMRA.raiseEventDebounceDelay)
			{
#ifdef COMMANDERS_DEBUG_MODE
				Serial.print(F("DccCommander packet IGNORED :"));
				Serial.print(realAddress);
				Serial.print(F(" / "));
				Serial.println(inDirection, DEC);
#endif
				return;
			}

		DccCommanderNMRA.lastEventDate = millis();

#ifdef COMMANDERS_DEBUG_MODE
		Serial.print(F("DccCommander packet received :"));
		Serial.print(realAddress);
		Serial.print(F(" / "));
		Serial.println(inDirection, DEC);
#endif
/*		if (DccCommanderNMRAClass::func_AccPacket)
			(DccCommanderNMRAClass::func_AccPacket)(realAddress, inOutputPower, inDirection);
		else*/
		{
			Commanders::RaiseEvent(DCCINT(realAddress, inDirection), COMMANDERS_EVENT_MOVE, inDirection ? COMMANDERS_MOVE_LEFT : COMMANDERS_MOVE_RIGHT);

			DccCommanderNMRA.lastDccId = DCCINT(realAddress, inDirection);
		}
	}
}

bool status = false;
void StatusBlink_handler()
{
	digitalWrite2f(Commanders::StatusLedPin, status ? HIGH : LOW);
	status = !status;
}

void DccCommanderNMRAClass::beginByPin(int inManufacturerId, int inVersionId, int interruptPinNumber, boolean inInterruptMonitor, boolean inRocoMode)
{
	Serial.println("Start beginByPin");
	DccCommanderNMRAClass::Dcc.pin(interruptPinNumber, true);
	Serial.println("pin ok");
	DccCommanderNMRAClass::Dcc.initAccessoryDecoder(inManufacturerId, inVersionId, FLAGS_DCC_ACCESSORY_DECODER, 0);
	Serial.println("init ok");

	this->rocoMode = inRocoMode;
#ifdef VISUALSTUDIO
	// In VS, the interrupt address is also the pin number.
	// pinMode() is just here to declare the pin used for the emulator...
	pinMode(interruptPinNumber, OUTPUT_INTERRUPT);
#endif
	this->lastDccId = UNDEFINED_ID;
	this->lastEventDate = 0;
	this->raiseEventOnActivation = false;
	this->raiseEventDebounceDelay = 200;

	//if (inInterruptMonitor)
	//	DCC.SetInterruptMonitor(StatusBlink_handler);

#ifdef COMMANDERS_PRINT_COMMANDERS
	this->Interrupt = digitalPinToInterrupt(interruptPinNumber);
#endif
	Serial.println("End beginByPin");
}

void DccCommanderNMRAClass::RaiseEventWhen(boolean inRaiseEventOnActivation, int inRaiseEventDebounceDelay)
{
	this->raiseEventOnActivation = inRaiseEventOnActivation;
	this->raiseEventDebounceDelay = inRaiseEventDebounceDelay;
}

void DccCommanderNMRAClass::PriorityLoop()
{
	DccCommanderNMRAClass::loop();
}

unsigned long DccCommanderNMRAClass::loop()
{
	Dcc.process();

	if (FactoryDefaultCVIndex && Dcc.isSetCVReady())
	{
		FactoryDefaultCVIndex--; // Decrement first as initially it is the size of the array 
		Dcc.setCV(FactoryDefaultCVs[FactoryDefaultCVIndex].CV, FactoryDefaultCVs[FactoryDefaultCVIndex].Value);
	}

	if (this->lastDccId != UNDEFINED_ID)
	{
		unsigned long last = this->lastDccId;
		this->lastDccId = UNDEFINED_ID;
		Commanders::SetLastEventType(COMMANDERS_EVENT_MOVEPOSITIONID);
		Commanders::SetLastEventData(0);
		return last;
	}

	return UNDEFINED_ID;
}

#ifdef COMMANDERS_DEBUG_MODE
void DccCommanderNMRAClass::printEvent(unsigned long inId, COMMANDERS_EVENT_TYPE inEventType, int inEventData)
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
void DccCommanderNMRAClass::printCommander()
{
	Serial.print(F("Commander: DccCommander - Interrupt: "));
	Serial.println(this->Interrupt);
}
#endif

#endif
