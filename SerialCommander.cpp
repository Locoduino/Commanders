/*************************************************************
project: <Universal Accessory Decoder>
author: <Thierry PARIS>
description: <Serial Commander>
*************************************************************/

//#include "UniversalAccessoryDecoder.h"

#ifndef NO_SERIALCOMMANDER
#include "SerialCommander.hpp"
#include "TextCommunicationHelper.hpp"

#ifdef DEBUG_MODE
#endif

// The use of a pointer to a HardwareSerial object have not run...
// So this source must use very ugly switches everywhere...

void SerialCommander::Setup(unsigned long inSpeed, byte inSerialPort)
{
	if (inSerialPort <= 0 || inSerialPort > 4)
#ifdef DEBUG_MODE
		Serial.println(F("SerialCommander::Setup : Invalid serial port, must be 1 to 4 !"));
#else
		return;
#endif

	switch (inSerialPort)
	{
	case 1: Serial.begin(inSpeed); break;
#if defined(UBRR1H)
	case 2: Serial1.begin(inSpeed); break;
#endif
#if defined(UBRR2H)
	case 3: Serial2.begin(inSpeed); break;
#endif
#if defined(UBRR3H)
	case 4: Serial3.begin(inSpeed); break;
#endif
	}
	this->serialPort = inSerialPort;
	this->addCounter = 0;
}

unsigned long SerialCommander::Loop()
{
	unsigned long found;
	char buffer[9];
	char character;

	Commander::StaticData.CommanderPriorityLoop();

	int avail = 0;
	switch (this->serialPort)
	{
	case 1: avail = Serial.available(); break;
#if defined(UBRR1H)
	case 2: avail = Serial1.available(); break;
#endif
#if defined(UBRR2H)
	case 3: avail = Serial2.available(); break;
#endif
#if defined(UBRR3H)
	case 4: avail = Serial3.available(); break;
#endif
	}

	if (avail > 0)
	{
		while (avail > 0)
		{
			switch (this->serialPort)
			{
			case 1: character = Serial.read(); break;
#if defined(UBRR1H)
			case 2: character = Serial1.read(); break;
#endif
#if defined(UBRR2H)
			case 3: character = Serial2.read(); break;
#endif
#if defined(UBRR3H)
			case 4: character = Serial3.read(); break;
#endif
			}

			if ((int)character < 32 || (int)character > 60)
			{
				addCounter = 0;
				return UNDEFINED_ID;
			}

			Serial.print(F("read "));
			Serial.println(addCounter, DEC);
			buffer[addCounter++] = character;
			buffer[addCounter] = 0;
			Serial.print(F("buffer "));
			Serial.println(buffer);

			if (addCounter == 8)
			{
				Commander::StaticData.CommanderPriorityLoop();

				buffer[addCounter] = 0;
#ifdef DEBUG_MODE
				Serial.print(F("Serial string received: "));
				Serial.println(buffer);
#endif
				/*
				for (int i = 0; i < Accessories::AccessoriesFullList.AccessoriesAddCounter; i++)
					Accessories::AccessoriesFullList.pAccessoriesFullList[i]->DCCToggle(TextCommunicationHelper::DecodeID(buffer), TextCommunicationHelper::DecodeIDAccessory(buffer));
				for (int i = 0; i < AccessoryGroup::StaticData.AccessoryGroupAddCounter; i++)
					AccessoryGroup::StaticData.pAccessoryGroupFullList[i]->DCCToggle(TextCommunicationHelper::DecodeID(buffer), TextCommunicationHelper::DecodeIDAccessory(buffer));
				*/
				addCounter = 0;
				/*found.dccId = TextCommunicationHelper::DecodeID(buffer);
				found.dccAccessory = TextCommunicationHelper::DecodeIDAccessory(buffer);*/
			}

			switch (this->serialPort)
			{
			case 1: avail = Serial.available(); break;
#if defined(UBRR1H)
			case 2: avail = Serial1.available(); break;
#endif
#if defined(UBRR2H)
			case 3: avail = Serial2.available(); break;
#endif
#if defined(UBRR3H)
			case 4: avail = Serial3.available(); break;
#endif
			}
		}
	}
	else
		addCounter = 0;

	return found;
}

#endif
