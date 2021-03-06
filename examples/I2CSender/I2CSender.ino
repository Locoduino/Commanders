/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <Demo independent I2C sender for UAD messages>
*************************************************************/

#include <Wire.h>

#include "Events.h"

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
	
    Serial.println("Setup finished.");
}

unsigned long time = 0;

// send an event each EVENT_DELAY !
#define EVENT_DELAY	2000

void loop()
{
	if (millis() - time > EVENT_DELAY)
	{
		Commanders_I2C_SendEvent(0x10, 12131415, COMMANDERS_EVENT_MOVE, COMMANDERS_MOVE_LEFT);
		time = millis();
	}
}
