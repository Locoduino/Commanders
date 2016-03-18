/*************************************************************
project: <Universal Accessory Decoder>
author: <Thierry PARIS>
description: <Serial Commander>
*************************************************************/

#include "Commanders.h"

#ifndef NO_SERIALCOMMANDER
//#include "TextCommunicationHelper.hpp"

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
#ifndef NO_SERIALCOMMANDER_SERIAL
	case 1: Serial.begin(inSpeed); break;
#endif
#ifndef NO_SERIALCOMMANDER_SERIAL1
#if defined(UBRR1H)
	case 2: Serial1.begin(inSpeed); break;
#endif
#endif
#ifndef NO_SERIALCOMMANDER_SERIAL2
#if defined(UBRR2H)
	case 3: Serial2.begin(inSpeed); break;
#endif
#endif
#ifndef NO_SERIALCOMMANDER_SERIAL3
#if defined(UBRR3H)
	case 4: Serial3.begin(inSpeed); break;
#endif
#endif
	}
	this->serialPort = inSerialPort;
	this->addCounter = 0;
}

CommanderEvent SerialCommander::Loop()
{
	CommanderEvent found = EmptyEvent;
	char buffer[9];
	char character;

	Commander::CommanderPriorityLoop();

	int avail = 0;
	switch (this->serialPort)
	{
#ifndef NO_SERIALCOMMANDER_SERIAL
	case 1: avail = Serial.available(); break;
#endif
#ifndef NO_SERIALCOMMANDER_SERIAL1
#if defined(UBRR1H)
	case 2: avail = Serial1.available(); break;
#endif
#endif
#ifndef NO_SERIALCOMMANDER_SERIAL2
#if defined(UBRR2H)
	case 3: avail = Serial2.available(); break;
#endif
#endif
#ifndef NO_SERIALCOMMANDER_SERIAL3
#if defined(UBRR3H)
	case 4: avail = Serial3.available(); break;
#endif
#endif
	}

	if (avail > 0)
	{
		while (avail > 0)
		{
			switch (this->serialPort)
			{
#ifndef NO_SERIALCOMMANDER_SERIAL
			case 1: character = Serial.read(); break;
#endif
#if defined(UBRR1H)
#ifndef NO_SERIALCOMMANDER_SERIAL1
			case 2: character = Serial1.read(); break;
#endif
#endif
#if defined(UBRR2H)
#ifndef NO_SERIALCOMMANDER_SERIAL2
			case 3: character = Serial2.read(); break;
#endif
#endif
#if defined(UBRR3H)
#ifndef NO_SERIALCOMMANDER_SERIAL3
			case 4: character = Serial3.read(); break;
#endif
#endif		
			}

			if ((int)character < 32 || (int)character > 60)
			{
				addCounter = 0;
				return EmptyEvent;
			}

			Serial.print(F("read "));
			Serial.println(addCounter, DEC);
			buffer[addCounter++] = character;
			buffer[addCounter] = 0;
			Serial.print(F("buffer "));
			Serial.println(buffer);

			if (addCounter == 8)
			{
				Commander::CommanderPriorityLoop();

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
				/*found.ID = TextCommunicationHelper::DecodeID(buffer);
				found.dccAccessory = TextCommunicationHelper::DecodeIDAccessory(buffer);*/
			}

			switch (this->serialPort)
			{
#ifndef NO_SERIALCOMMANDER_SERIAL
			case 1: avail = Serial.available(); break;
#endif
#ifndef NO_SERIALCOMMANDER_SERIAL1
#if defined(UBRR1H)
			case 2: avail = Serial1.available(); break;
#endif
#endif
#ifndef NO_SERIALCOMMANDER_SERIAL2
#if defined(UBRR2H)
			case 3: avail = Serial2.available(); break;
#endif
#endif
#ifndef NO_SERIALCOMMANDER_SERIAL3
#if defined(UBRR3H)
			case 4: avail = Serial3.available(); break;
#endif
#endif
			}
		}
	}
	else
		addCounter = 0;

	Commander::RaiseEvent(found.ID, found.Event, found.Data);
	return found;
}

#endif
