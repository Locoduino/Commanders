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

unsigned long CANCommanderClass::lastEventId;
CANCommanderClass *CANCommanderClass::pCANCommander;

volatile byte Flag_Recv = 0;   // variable d'échange avec l'interruption IRQ
							   
/*
*  ISR CAN (Routine de Service d'Interruption)
*  le flag IRQ monte quand au moins un message est reçu
*  le flag IRQ ne retombe QUE si tous les messages sont lus
*/

void MCP2515_ISR()
{
	Flag_Recv = 1;
}

void CANCommanderClass::begin(byte inSPIpin, byte inSpeed, byte inInterrupt, uint16_t inId)
{
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
	this->pCan->init_Filt(2, 0, 0);        // Reception possible : Id 40 à 4F (hex) 
	this->pCan->init_Filt(3, 0, 0);        // idem
	this->pCan->init_Filt(4, 0, 0);        // Reception possible : Id 00 à 0F
	this->pCan->init_Filt(5, 0, 0);        // Idem
}

// Variables globales pour la gestion des Messages reçus et émis
byte IdR;                       // Id pour la routine CAN_recup()
unsigned char lenR = 0;         // Longueur "    "       "
unsigned char bufR[8];          // tampon de reception      "
unsigned char bufS[8];          // tampon d'emission

								// Variable globale Mémoire circulaire pour le stockage des messages reçus
unsigned char _Circule[256];    // récepteur circulaire des messages CAN sous IT
int _indexW, _indexR, _Ncan;    // index d'écriture et lecture, nb d'octets a lire
byte _CANoverflow = 0;          // flag overflow (buffer _Circule plein)

/*
* Routine de récupération des messages CAN dans la mémoire circulaire _Circule
* appelée par LOOP lorsque Flag_Recv = 1;
*/
void CAN_recup(MCP_CAN *apCan)
{
	unsigned char len = 0;  // nombre d'octets du message
	unsigned char buf[8];   // message
	unsigned long Id;   // Id (on devrait plutôt utiliser un int car il y a 11 bits)

	while (CAN_MSGAVAIL == apCan->checkReceive()) 
	{
		apCan->readMsgBuf(&len, buf);        // read data, len: data length, buf: data buf
		Id = apCan->getCanId();
		if ((unsigned int) (_Ncan + len + 2) < sizeof(_Circule)) 
		{ // il reste de la place dans _Circule
			_Circule[_indexW] = Id;         // enregistrement de Id
			_indexW++;
			_Ncan++;
			if (_indexW == sizeof(_Circule)) 
				_indexW = 0;
			_Circule[_indexW] = len;        // enregistrement de len
			_indexW++;
			_Ncan++;
			if (_indexW == sizeof(_Circule)) 
				_indexW = 0;
			for (byte z = 0; z<len; z++) 
			{
				_Circule[_indexW] = buf[z];    // enregistrement du message
				_indexW++;
				_Ncan++;
				if (_indexW == sizeof(_Circule))
					_indexW = 0;
			}
		}
		else 
		{
			_CANoverflow = 1;  // dépassement de la capacite de Circule
							   // le message est perdu
		}
	}
}

unsigned long CANCommanderClass::loop()
{
	if (Flag_Recv)
	{
		Flag_Recv = 0;  // Flag MCP2515 ready for a new  IRQ
		CAN_recup(this->pCan);    // Get all the messages
	}

	// Handle the first message in the buffer _Circule...
	byte RId;
	byte Rlen;
	byte Rbuf[8];

	while (_Ncan > 2) 
	{    // The minimal size of one message is 3 bytes long !
		_Ncan--;
		RId = _Circule[_indexR];        // recup Id
		_indexR++;
		if (_indexR == sizeof(_Circule)) 
			_indexR = 0;
		_Ncan--;
		Rlen = _Circule[_indexR];       // recup length
		_indexR++;
		if (_indexR == sizeof(_Circule)) 
			_indexR = 0;
#ifdef COMMANDERS_DEBUG_MODE
		Serial.print(F("CAN id "));
		Serial.print(RId);
		Serial.print(F(", data "));
#endif
		for (int k = 0; k < Rlen; k++) 
		{
			_Ncan--;
			Rbuf[k] = _Circule[_indexR];  // recup octets message
			_indexR++;
			if (_indexR == sizeof(_Circule)) 
				_indexR = 0;
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

		COMMANDERS_EVENT_TYPE eventType = (COMMANDERS_EVENT_TYPE)Rbuf[4];

		int foundData = Rbuf[6];
		foundData = foundData << 8;
		foundData |= Rbuf[5];
		
		Commander::RaiseEvent(foundID, eventType, foundData);

		Commanders::SetLastEventType(eventType);
		Commanders::SetLastEventData(foundData);
		return foundID;
	}

	return UNDEFINED_ID;
}
#endif
