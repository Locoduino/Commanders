/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <Demo independent I2C sender for UAD messages>
*************************************************************/

#include <Wire.h>

// Copy of the COMMANDERS_EVENT_TYPE and MOVE_TYPE from Commanders library.
enum COMMANDERS_MOVE_TYPE
{
	COMMANDERS_MOVE_MORE = +1,
	COMMANDERS_MOVE_LESS = -1,
	COMMANDERS_MOVE_STOP = 0,
	COMMANDERS_MOVE_LEFT = -2,
	COMMANDERS_MOVE_RIGHT = -3,
	COMMANDERS_MOVE_CENTER = -4,
	COMMANDERS_MOVE_TOP = -5,
	COMMANDERS_MOVE_BOTTOM = -6,
	COMMANDERS_MOVE_STRAIGHT = -7,
	COMMANDERS_MOVE_DIVERGE = -8,
	COMMANDERS_MOVE_ON = -9,
	COMMANDERS_MOVE_OFF = -10
};

enum COMMANDERS_EVENT_TYPE
{
	COMMANDERS_EVENT_NONE = 0,			// Should never appear
	COMMANDERS_EVENT_TOGGLE = 1,		// If a push button or similar is pressed, invert the state/position
	COMMANDERS_EVENT_MOVE = 2,			// If a push 'left' button or similar is pressed, data is a COMMANDERS_MOVE_TYPE
	COMMANDERS_EVENT_MOVEPOSITION = 3,	// If a potentiometer or similar is moved, data is the position to reach
	COMMANDERS_EVENT_CONFIG = 4			// Data is the configuration address and value
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
