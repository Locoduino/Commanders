/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <Demo independent I2C sender for UAD messages>
*************************************************************/

#include <Wire.h>

// Copy of the COMMANDERS_EVENT_TYPE from Commanders library.
enum COMMANDERS_EVENT_TYPE
{
	NONE = 0,			// Should never appear
	TOGGLE = 1,			// If a push button or similar is pressed
	MLEFT = 2,			// If a switch in on its first pos
	MRIGHT = 3,			// If a switch is on its second pos
	MSTOP = 4,			// If a motor or light have to stop
	ABSOLUTEMOVE = 5,	// If a potentiometer or similar is moved
	RELATIVEMOVE = 6	// If an encoder or similar is moved
};

// Comment this line in production !
#define DEBUGMODE

bool Commanders_I2C_SendEvent(uint8_t inID, unsigned long inEventID, COMMANDERS_EVENT_TYPE inEventType, int inEventData)
{
#ifdef DEBUGMODE
	Serial.print(F("I2C send Commanders event: "));
	Serial.print(inEventID, DEC);
	Serial.print(F("/"));
	Serial.print((char) ('0' + (char)inEventType));
	Serial.print(F("/"));
	Serial.println(inEventData, DEC);
#endif
	
	Wire.beginTransmission(inID);

	int nb = Wire.write((const uint8_t *)&inEventID, sizeof(unsigned long));
	nb += Wire.write((char )inEventType);
	nb += Wire.write((const uint8_t *)&inEventData, sizeof(int));

#ifdef DEBUGMODE
	Serial.print(F("I2C sender : nb byte transmitted : "));
	Serial.println(nb, DEC);
#endif

	Wire.endTransmission(true);

	return true;
}

void setup()
{
    Serial.begin(115200);

	Wire.begin(); // join i2c bus
}

unsigned long time = 0;

// send an event each EVENT_DELAY !
#define EVENT_DELAY	2000

void loop()
{
	if (millis() - time > EVENT_DELAY)
	{
		Commanders_I2C_SendEvent(0x10, 100, COMMANDERS_EVENT_TYPE::TOGGLE, 200);
		time = millis();
	}
}
