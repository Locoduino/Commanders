//-------------------------------------------------------------------
#ifndef __serialCommander_H__
#define __serialCommander_H__
//-------------------------------------------------------------------

#include "Commanders.h"

#define SerialCommander SerialCommanderClass::GetCurrent()

//-------------------------------------------------------------------

// SERIAL_PORT argument can be any of the available serials of the used card, but also any other
// class implementing begin(), read() and available() functions, like in SoftwareSerial or AltSoftSerial !

#define SERIAL_COMMANDER(SERIAL_PORT) \
\
class SerialCommanderClass : Commander \
{\
private:\
	TextInterpreter TI;\
\
public:\
	inline SerialCommanderClass() : Commander() { }\
	\
	inline void begin() { }\
	\
	inline unsigned long loop() \
	{\
		unsigned long foundID = UNDEFINED_ID;\
		Commander::CommanderPriorityLoop(); \
\
		if (SERIAL_PORT.available() > 0)\
		{\
			while (1)			/* loop until 'enter' is received */ \
			{\
				char incomingByte = SERIAL_PORT.read(); \
\
				foundID = TI.SendChar(incomingByte);\
				if (incomingByte == '\n') break;   /* exit the while(1) : data OK */ \
			}\
		}\
\
		return foundID;\
	}\
\
public:\
	static SerialCommanderClass *pSerialCommander; \
	static inline SerialCommanderClass &GetCurrent() \
	{ \
		if (pSerialCommander == NULL) \
			pSerialCommander = new SerialCommanderClass(); \
		return *(SerialCommanderClass::pSerialCommander);\
	}\
	void printCommander() { Serial.println(F("Commander: SerialCommander")); }\
\
};\
SerialCommanderClass *SerialCommanderClass::pSerialCommander; 

//-------------------------------------------------------------------
#endif
