/*************************************************************
project: <Commanders>
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
	if (inSerialPort < 0 || inSerialPort > 3)
#ifdef DEBUG_MODE
		Serial.println(F("SerialCommander::Setup : Invalid serial port, must be 0 to 3 !"));
#else
		return;
#endif

	switch (inSerialPort)
	{
#ifndef NO_SERIALCOMMANDER_SERIAL
	case 0: this->pSerial = &Serial; break;
#endif
#ifndef NO_SERIALCOMMANDER_SERIAL1
#if defined(UBRR1H)
	case 1: this->pSerial = &Serial1; break;
#endif
#endif
#ifndef NO_SERIALCOMMANDER_SERIAL2
#if defined(UBRR2H)
	case 2: this->pSerial = &Serial2; break;
#endif
#endif
#ifndef NO_SERIALCOMMANDER_SERIAL3
#if defined(UBRR3H)
	case 3: this->pSerial = &Serial3; break;
#endif
#endif
	}

	this->pSerial->begin(inSpeed);
	this->addCounter = 0;
}

CommanderEvent SerialCommander::Loop()
{
	CommanderEvent found = EmptyEvent;
	char buffer[9];
	char character;

	Commander::CommanderPriorityLoop();

	int avail = this->pSerial->available();

	if (avail > 0)
	{
		while (avail > 0)
		{
			character = this->pSerial->read();

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

			avail = this->pSerial->available();
		}
	}
	else
		addCounter = 0;

	Commander::RaiseEvent(found.ID, found.Event, found.Data);
	return found;
}

#endif
