/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <Demo independent CAN sender for Commanders messages>
*************************************************************/

// demo: CAN-BUS Shield, send data
#include <mcp_can.h>
#include <SPI.h>

#include "Events.h"

// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 10;

MCP_CAN CAN(SPI_CS_PIN);                                    // Set CS pin

// Comment this line in production !
#define DEBUGMODE

bool Commanders_CAN_SendEvent(uint8_t inID, unsigned long inEventID, COMMANDERS_EVENT_TYPE inEventType, int inEventData)
{
	unsigned char stmp[8];

#ifdef DEBUGMODE
	Serial.print(F("Send Commanders event on CAN bus: "));
	Serial.print(inEventID, DEC);
	Serial.print(F("/"));
	Serial.print((char) ('0' + (char)inEventType));
	Serial.print(F("/"));
	Serial.println(inEventData, DEC);
#endif
	
	byte *pEvent;

	pEvent = (byte *)&inEventID;
	stmp[0] = *pEvent;
	stmp[1] = *(pEvent+1);
	stmp[2] = *(pEvent+2);
	stmp[3] = *(pEvent+3);
	stmp[4] = (byte)inEventType;
	pEvent = (byte *)&inEventData;
	stmp[5] = *pEvent;
	stmp[6] = *(pEvent + 1);

	// send data:  id = inID, standard frame, data len = 7, stmp: data buf
	byte nb = CAN.sendMsgBuf(inID, 0, 7, stmp);
	delay(100);                       // send data per 100ms

#ifdef DEBUGMODE
	Serial.print(F("CAN sender : nb byte transmitted : "));
	Serial.println(nb, DEC);
#endif

	return true;
}

void setup()
{
    Serial.begin(115200);

    while (CAN_OK != CAN.begin(CAN_125KBPS))              // init can bus : baudrate = 125k
    {
        Serial.println(F(" CAN BUS Shield failed again"));
        delay(100);
    }
    Serial.println(F("CAN BUS Shield Emitter init 125K ok!"));
}

unsigned long time = 0;

// send an event each EVENT_DELAY !
#define EVENT_DELAY	2000

void loop()
{
	if (millis() - time > EVENT_DELAY)
	{
		Commanders_CAN_SendEvent(0x10, 12131415, COMMANDERS_EVENT_TYPE::COMMANDERS_EVENT_MOVEPOSITION, 6869);
		time = millis();
	}
}
