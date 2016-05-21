/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <I2C Commander>
*************************************************************/

#include "Commanders.h"

#ifndef NO_I2CCOMMANDER

uint8_t I2CCommanderClass::I2CSlaveId;
unsigned long I2CCommanderClass::LastEventId;
I2CCommanderClass *I2CCommanderClass::pI2cCommander;

#define I2C_BUFFERLENGTH	(sizeof(unsigned long) + 1 + sizeof(int) + 5)

void OnReceiveHandler(int inNbBytes)
{
	I2CCommanderClass::LastEventId = UNDEFINED_ID;

	unsigned char buf[I2C_BUFFERLENGTH];
	byte count = 0;

	while (Wire.available() && count < I2C_BUFFERLENGTH)
		buf[count++] = Wire.read();

#ifdef COMMANDERS_DEBUG_MODE
	Serial.print("I2CCommander::ReceiveHandler: chars read : ");
	Serial.println(count, DEC);
#endif

	long four = buf[0];
	long three = buf[1];
	long two = buf[2];
	long one = buf[3];

	//Rebuild the recomposed long by using bitshift.
	unsigned long foundID = ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);

	COMMANDERS_EVENT_TYPE eventType = (COMMANDERS_EVENT_TYPE)buf[4];

	int foundData = buf[6];
	foundData = foundData << 8;
	foundData |= buf[5];

	Commander::RaiseEvent(foundID, eventType, foundData);

	Commanders::SetLastEventType(eventType);
	Commanders::SetLastEventData(foundData);
	I2CCommanderClass::LastEventId = foundID;
}

void I2CCommanderClass::begin(uint8_t inSlaveID)
{
	Wire.begin(inSlaveID);
	Wire.onReceive(OnReceiveHandler);
	I2CCommanderClass::I2CSlaveId = inSlaveID;
	I2CCommanderClass::LastEventId = UNDEFINED_ID;
}

unsigned long I2CCommanderClass::loop()
{
	if (I2CCommanderClass::LastEventId != UNDEFINED_ID)
	{
		unsigned long id = I2CCommanderClass::LastEventId;
		I2CCommanderClass::LastEventId = UNDEFINED_ID;
		return id;
	}

	return UNDEFINED_ID;
}
#endif
