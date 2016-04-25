/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <I2C Commander>
*************************************************************/

#ifndef NO_I2CCOMMANDER
#include "I2CCommander.hpp"

uint8_t I2CCommander::I2CSlaveId;
unsigned long I2CCommander::LastEventId;

#define I2C_BUFFERLENGTH	(sizeof(unsigned long) + 1 + sizeof(int) + 5)

void OnReceiveHandler(int inNbBytes)
{
	I2CCommander::LastEventId = UNDEFINED_ID;

	Commander *pCurr = Commander::pFirstCommander;

	while (pCurr != 0)
	{
		if (Wire.requestFrom(I2CCommander::I2CSlaveId, sizeof(unsigned long) + 1 + sizeof(int)) > 0)
			break;
		pCurr = pCurr->pNextCommander;
	}

	char buf[I2C_BUFFERLENGTH];
	byte count = 0;

	Serial.println("I2CCommander::ReceiveHandler chars available");

	while (Wire.available() && count < I2C_BUFFERLENGTH)
	{
		buf[count++] = Wire.read();
	}

	Serial.print("I2CCommander::ReceiveHandler: chars read : ");
	Serial.println(count);

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
	I2CCommander::LastEventId = foundID;
}

void I2CCommander::begin(uint8_t inSlaveID)
{
	Wire.onReceive(OnReceiveHandler);
	I2CCommander::I2CSlaveId = inSlaveID;
	I2CCommander::LastEventId = UNDEFINED_ID;
}

unsigned long I2CCommander::loop()
{
	if (I2CCommander::LastEventId != UNDEFINED_ID)
	{
		unsigned long id = I2CCommander::LastEventId;
		I2CCommander::LastEventId = UNDEFINED_ID;
		return id;
	}

	return UNDEFINED_ID;
}
#endif
