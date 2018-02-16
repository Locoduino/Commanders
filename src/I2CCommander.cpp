/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <I2C Commander>
*************************************************************/

#include <Commanders.h>

#ifdef NO_I2CCOMMANDER
#pragma message ("Commanders : No I2C commander !")
#else
I2CCommanderClass *I2CCommanderClass::pI2cCommander;

#define I2C_BUFFERLENGTH	(sizeof(unsigned long) + 1 + sizeof(int) + 5)

// Just here and not in the class, because this value must be updated by a 'C' style function, and should be declared public in the class....
static unsigned long I2CLastEventId;

void OnReceiveHandler(int inNbBytes)
{
	I2CLastEventId = UNDEFINED_ID;

	unsigned char buf[I2C_BUFFERLENGTH];
	uint8_t count = 0;

	while (Wire.available() && count < I2C_BUFFERLENGTH)
		buf[count++] = Wire.read();

#ifdef COMMANDERS_DEBUG_MODE
	Serial.print(F("I2CCommander::ReceiveHandler: chars read : "));
	Serial.println(count, DEC);
#endif

	long four = buf[0];
	long three = buf[1];
	long two = buf[2];
	long one = buf[3];

	//Rebuild the recomposed long by using bit-shift.
	unsigned long foundID = ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);

	COMMANDERS_EVENT_TYPE lastEventType = (COMMANDERS_EVENT_TYPE)buf[4];

	int foundData = buf[6];
	foundData = foundData << 8;
	foundData |= buf[5];

	Commanders::RaiseEvent(foundID, lastEventType, foundData);

	Commanders::SetLastEventType(lastEventType);
	Commanders::SetLastEventData(foundData);
	I2CLastEventId = foundID;
}

void I2CCommanderClass::begin(uint8_t inSlaveID)
{
	Wire.begin(inSlaveID);
	Wire.onReceive(OnReceiveHandler);
	this->I2CSlaveId = inSlaveID;
	I2CLastEventId = UNDEFINED_ID;
}

unsigned long I2CCommanderClass::loop()
{
	if (I2CLastEventId != UNDEFINED_ID)
	{
		unsigned long id = I2CLastEventId;
		I2CLastEventId = UNDEFINED_ID;
		return id;
	}

	return UNDEFINED_ID;
}

#ifdef COMMANDERS_PRINT_COMMANDERS
void I2CCommanderClass::printCommander()
{
	Serial.print(F("Commander: I2CCommander  - slave ID: "));
	Serial.println(this->I2CSlaveId);
}
#endif

#endif


