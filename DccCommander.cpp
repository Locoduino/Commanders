/*************************************************************
project: <Universal Accessory Decoder>
author: <Thierry PARIS>
description: <Dcc Commander>
*************************************************************/

//#include "UniversalAccessoryDecoder.h"

#ifndef NO_DCCCOMMANDER
#include "DccCommander.hpp"

#ifdef VISUALC
DCC_Decoder DCC_Decoder::DCCInstance;
#endif

GPIO_pin_t DccCommander::dccStatusLedPin;
boolean DccCommander::UseRawDccAddresses;

void DccAccessoryDecoderPacket(int address, boolean activate, byte data);

#ifdef DEBUG_MODE
#define CHECK(val, text)	CheckIndex(val, F(text))
#else
#define CHECK(val, text)
#endif

#ifdef DEBUG_MODE
void DccCommander::CheckIndex(unsigned char inIndex, const __FlashStringHelper *inFunc)
{
}
#endif

void DccCommander::Setup(int i, int j, int k, boolean inUseRawDccAddresses)
{
	DCC.SetupDecoder(i, j, k);
	DccCommander::UseRawDccAddresses = inUseRawDccAddresses;
}

bool status = false;
void StatusBlink_handler()
{
	digitalWrite2f(DccCommander::dccStatusLedPin, status ? HIGH : LOW);
	status = !status;
}

void DccCommander::SetStatusLedPin(int inLedPin)
{
	DccCommander::dccStatusLedPin = Arduino_to_GPIO_pin(inLedPin);
	pinMode2f(DccCommander::dccStatusLedPin, OUTPUT);

	DCC.SetInterruptMonitor(StatusBlink_handler);
}

void DccCommander::PriorityLoop()
{
	DccCommander::Loop();
}

#define MINTIME	2
#ifdef DEBUG_MODE
#ifdef DEBUG_VERBOSE_MODE
static int countLoop = 0;
#endif
#endif

static unsigned long start = 0;

#define ELAPSEDTIME	((unsigned long) -2)

unsigned long DccCommander::Loop()
{
	if (start == 0)
		start = millis();

	DCC.loop();
#ifdef DEBUG_MODE
#ifdef DEBUG_VERBOSE_MODE
	countLoop++;
#endif
#endif

	if (millis() - start > MINTIME)
	{
#ifdef DEBUG_MODE
#ifdef DEBUG_VERBOSE_MODE
		Serial.print("DCC commander : ");
		Serial.println(countLoop, DEC);
		countLoop = 0;
#endif
#endif
		start = 0;
		return UNDEFINED_ID;
	}

	return ELAPSEDTIME;
}

DccBasicAccDecoderPacket DccCommander::func_BasicAccPacket = NULL;

void DccCommander::SetBasicAccessoryDecoderPacketHandler(DccBasicAccDecoderPacket func)
{
	DccCommander::func_BasicAccPacket = func;
}

#endif
