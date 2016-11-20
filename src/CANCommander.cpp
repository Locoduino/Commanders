/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <CAN Commander>
*************************************************************/

#include "Commanders.h"

#ifndef NO_CANCOMMANDER
#ifdef VISUALSTUDIO
#include "../VStudio/mcp_can.h"
#else
#include <mcp_can.h>
#include <SPI.h>
#endif

CANCommanderClass *CANCommanderClass::pCANCommander;

volatile uint8_t Flag_Recv = 0;   // variable d'�change avec l'interruption IRQ
							   
/*
*  ISR CAN (Routine de Service d'Interruption)
*  le flag IRQ monte quand au moins un message est re�u
*  le flag IRQ ne retombe QUE si tous les messages sont lus
*/

void MCP2515_ISR()
{
	Flag_Recv = 1;
}

void CANCommanderClass::begin(uint8_t inSPIpin, uint8_t inSpeed, uint8_t inInterrupt, uint16_t inId)
{
#ifdef COMMANDERS_PRINT_COMMANDERS
	uint8_t SPIpin = inSPIpin;
	uint8_t Speed = inSpeed;
	uint8_t Interrupt = inInterrupt;
	uint16_t Id = inId;
#endif

#ifdef VISUALSTUDIO
	pinMode(inSPIpin, OUTPUT_RESERVED);
	pinMode(inInterrupt, OUTPUT_RESERVED);
#endif
	this->pCan = new MCP_CAN(inSPIpin);
	while (CAN_OK != this->pCan->begin(inSpeed))              // init can bus with baudrate
	{
#ifdef COMMANDERS_DEBUG_MODE
		Serial.println(F("CAN BUS Shield init fail"));
		Serial.println(F("Init CAN BUS Shield again"));
#endif
		delay(100);
	}
#ifdef COMMANDERS_DEBUG_MODE
	Serial.println(F("CAN BUS Shield init ok!"));
#endif

	/* INTERRUPT values as macro argument :
	Board			int.0	int.1	int.2	int.3	int.4	int.5
	Uno, Ethernet	2		3
	Mega2560		2		3		21		20		19		18
	Leonardo		3		2		0		1		7
	*/
	attachInterrupt(inInterrupt, MCP2515_ISR, FALLING);

	/*
	* set mask & filter
	*/

	this->pCan->init_Mask(0, 0, 0xFFFF);	// All is tested
	this->pCan->init_Mask(1, 0, 0xFFFF);

	this->pCan->init_Filt(0, 0, inId);
	this->pCan->init_Filt(1, 0, 0);        // idem
	this->pCan->init_Filt(2, 0, 0);        // Reception possible : Id 40 � 4F (hex) 
	this->pCan->init_Filt(3, 0, 0);        // idem
	this->pCan->init_Filt(4, 0, 0);        // Reception possible : Id 00 � 0F
	this->pCan->init_Filt(5, 0, 0);        // Idem
}


/*
* Routine de r�cup�ration des messages CAN dans la m�moire circulaire _Circule
* appel�e par LOOP lorsque Flag_Recv = 1;
*/
void CANCommanderClass::CAN_recup()
{
	unsigned char len = 0;  // nombre d'octets du message
	unsigned char buf[8];   // message
	unsigned char Id;   // Id (on devrait plut�t utiliser un int car il y a 11 bits)

	while (CAN_MSGAVAIL == this->pCan->checkReceive())
	{
		this->pCan->readMsgBuf(&len, buf);        // read data, len: data length, buf: data buf
		Id = this->pCan->getCanId();
		if ((unsigned int) (this->Ncan + len + 2) < sizeof(this->Circule))
		{ // il reste de la place dans _Circule
			this->Circule[this->indexW] = Id;         // enregistrement de Id
			this->indexW++;
			this->Ncan++;
			if (this->indexW == sizeof(this->Circule))
				this->indexW = 0;
			this->Circule[this->indexW] = len;        // enregistrement de len
			this->indexW++;
			this->Ncan++;
			if (this->indexW == sizeof(this->Circule))
				this->indexW = 0;
			for (uint8_t z = 0; z<len; z++)
			{
				this->Circule[this->indexW] = buf[z];    // enregistrement du message
				this->indexW++;
				this->Ncan++;
				if (this->indexW == sizeof(this->Circule))
					this->indexW = 0;
			}
		}
		else 
		{
			this->CANoverflow = 1;  // d�passement de la capacite de Circule
							   // le message est perdu
		}
	}
}

unsigned long CANCommanderClass::loop()
{
	if (Flag_Recv)
	{
		Flag_Recv = 0;  // Flag MCP2515 ready for a new  IRQ
		this->CAN_recup();    // Get all the messages
	}

	// Handle the first message in the buffer _Circule...
	uint8_t RId;
	uint8_t Rlen;
	uint8_t Rbuf[8];

	while (this->Ncan > 2)
	{    // The minimal size of one message is 3 bytes long !
		this->Ncan--;
		RId = this->Circule[this->indexR];        // recup Id
		this->indexR++;
		if (this->indexR == sizeof(this->Circule))
			this->indexR = 0;
		this->Ncan--;
		Rlen = this->Circule[this->indexR];       // recup length
		this->indexR++;
		if (this->indexR == sizeof(this->Circule))
			this->indexR = 0;
#ifdef COMMANDERS_DEBUG_MODE
		Serial.print(F("CAN id "));
		Serial.print(RId);
		Serial.print(F(", data "));
#endif
		for (int k = 0; k < Rlen; k++) 
		{
			this->Ncan--;
			Rbuf[k] = this->Circule[this->indexR];  // recup octets message
			this->indexR++;
			if (this->indexR == sizeof(this->Circule))
				this->indexR = 0;
#ifdef COMMANDERS_DEBUG_MODE
			Serial.print(F("0x"));
			Serial.print(Rbuf[k], HEX);
#endif
		}
#ifdef COMMANDERS_DEBUG_MODE
		Serial.println("");
#endif
		// le message est maintenant dans les globales RId, Rlen et Rbuf[..]

		long four = Rbuf[0];
		long three = Rbuf[1];
		long two = Rbuf[2];
		long one = Rbuf[3];

		//Rebuild the recomposed long by using bitshift.
		unsigned long foundID = ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);

		COMMANDERS_EVENT_TYPE lastEventType = (COMMANDERS_EVENT_TYPE)Rbuf[4];

		int foundData = Rbuf[6];
		foundData = foundData << 8;
		foundData |= Rbuf[5];
		
		Commanders::RaiseEvent(foundID, lastEventType, foundData);

		Commanders::SetLastEventType(lastEventType);
		Commanders::SetLastEventData(foundData);
		return foundID;
	}

	return UNDEFINED_ID;
}

#ifdef COMMANDERS_PRINT_COMMANDERS
void CANCommanderClass::printCommander()
{
	Serial.print(F("Commander: CANCommander  - Pin SPI: "));
	Serial.print(this->SPIpin);
	Serial.print(F(" / Speed: "));
	switch (this->Speed)
	{
	case CAN_5KBPS:		Serial.print(F("5KBPS")); break;
	case CAN_10KBPS:	Serial.print(F("10KBPS")); break;
	case CAN_20KBPS:	Serial.print(F("20KBPS")); break;
	case CAN_25KBPS:	Serial.print(F("25KBPS")); break;
	case CAN_31K25BPS:	Serial.print(F("31K25BPS")); break;
	case CAN_33KBPS:	Serial.print(F("33KBPS")); break;
	case CAN_40KBPS:	Serial.print(F("40KBPS")); break;
	case CAN_50KBPS:	Serial.print(F("50KBPS")); break;
	case CAN_80KBPS:	Serial.print(F("80KBPS")); break;
	case CAN_83K3BPS:	Serial.print(F("83K3BPS")); break;
	case CAN_95KBPS:	Serial.print(F("95KBPS")); break;
	case CAN_100KBPS:	Serial.print(F("100KBPS")); break;
	case CAN_125KBPS:	Serial.print(F("125KBPS")); break;
	case CAN_200KBPS:	Serial.print(F("200KBPS")); break;
	case CAN_250KBPS:	Serial.print(F("250KBPS")); break;
	case CAN_500KBPS:	Serial.print(F("500KBPS")); break;
	case CAN_666KBPS:	Serial.print(F("666KBPS")); break;
	case CAN_1000KBPS:	Serial.print(F("1000KBPS")); break;
	}
	Serial.print(F(" / Interrupt: "));
	Serial.print(this->Interrupt);
	Serial.print(F(" / ID: "));
	Serial.println(this->Id);
}
#endif
#endif
