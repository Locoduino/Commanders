//-------------------------------------------------------------------
#ifndef __serialCommander_H__
#define __serialCommander_H__
//-------------------------------------------------------------------

#include "Commanders.h"

#ifdef VISUALSTUDIO
#include "VStudio/Serial.hpp"
#endif

//-------------------------------------------------------------------

#ifdef DEBUG_MODE
	#define SERIAL_MACRO_DEBUG_SEPARATOR		Serial.println(F("separator"));
	#define SERIAL_MACRO_DEBUG_READ \
			Serial.print(F("read ")); \
			Serial.println(incomingByte, DEC); 
	#define SERIAL_MACRO_DEBUG_EVENT \
			Serial.print(F("Event "));\
			Serial.print(foundID, DEC);\
			Serial.print(F(" / "));\
			Serial.print((int)foundEvent, DEC);\
			Serial.print(F(" / "));\
			Serial.println(foundData, DEC);
#endif

// SERIAL_PORT argument can be any of the available serials of the used card, but also any other
// class implementing begin(), read() and available() functions, like in SoftwareSerial or AltSoftSerial !
//
// The accepted syntax is 
// id separator event separator data
// where id can be a number from 0 to 4 000 000 000,
// where separator can be ',' , '-' , '|' or ';'
// event can be any string of 9 characters long maximum, but must begin with
//		's' or 'S' to select
//		'a' or 'A' to make an absolute move, data is necessary in this case
//		'r' or 'R' to make a relative move, data is necessary in this case
// and where the data can be any number	between -32767 and 32768
//
// ex : 123 means ID 123 selected
// ex : 456 | s means ID 456 selected
// ex : 789 - a - 100  means ID 789 make an absolute movement to 100 .

#define SERIAL_COMMANDER(SERIAL_PORT) \
\
class SerialCommander : Commander \
{\
public:\
	inline SerialCommander() : Commander() { }\
\
	inline void Setup(unsigned long inSpeed) { SERIAL_PORT.begin(inSpeed); }\
\
	inline unsigned long Loop() \
	{\
		enum STEP\
		{\
			STEP_ID,\
			STEP_TYPE,\
			STEP_DATA,\
			STEP_END\
		};\
\
		unsigned long foundID = UNDEFINED_ID;\
		COMMANDERS_EVENT_TYPE foundEvent = COMMANDERS_EVENT_NONE; \
		int foundData = 0; \
\
		Commander::CommanderPriorityLoop();\
\
		if (SERIAL_PORT.available() > 0)\
		{\
			char buffer[10];\
			byte buffer_pos = 0;\
			bool neg_sign = false;\
			STEP step = STEP_ID;\
\
			foundID = 0;		/* reset */ \
			while (1)			/* loop until 'enter' is received */ \
			{\
				char incomingByte = SERIAL_PORT.read();\
\
				if (incomingByte == '\n') break;   /* exit the while(1) : data OK */ \
				if (incomingByte == (char)-1) continue;  /* loop if empty buffer */ \
\
				if (incomingByte == ',' || incomingByte == ';' || incomingByte == '/' || incomingByte == '-')\
				{\
					SERIAL_MACRO_DEBUG_SEPARATOR \
					switch (step)\
					{\
					case STEP_ID:\
						step = STEP_TYPE;\
						for (int i = 0; i < 10; i++)\
							buffer[i] = 0;\
						buffer_pos = 0;\
						break;\
					case STEP_TYPE:\
						if (buffer[0] == 's' || buffer[0] == 'S')\
							foundEvent = COMMANDERS_EVENT_SELECTED;\
						if (buffer[0] == 'a' || buffer[0] == 'A')\
							foundEvent = COMMANDERS_EVENT_ABSOLUTEMOVE;\
						if (buffer[0] == 'r' || buffer[0] == 'R')\
							foundEvent = COMMANDERS_EVENT_RELATIVEMOVE;\
						step = STEP_DATA;\
						foundData = 0;\
						break;\
					case STEP_DATA:\
						step = STEP_END;\
						break;\
					default:\
						break;\
					}\
\
					continue;\
				}\
\
				if (step == STEP_END)	/* end of useful input ! */ \
					break;\
				SERIAL_MACRO_DEBUG_READ \
				switch (step)\
				{\
				case STEP_ID:\
					if (incomingByte < 48 || incomingByte > 57)\
						break;\
					foundID *= 10;                    /* *10 => shift to left */ \
					foundID = ((incomingByte - 48) + foundID);  /* 48 because of ASCII value (1 => 49 in ASCII) */ \
					foundEvent = COMMANDERS_EVENT_SELECTED;/* If an ID has been used, use it as a selection */ \
					foundData = 0; \
					break;\
				case STEP_TYPE:\
					if (buffer_pos < 10)\
						buffer[buffer_pos++] = incomingByte;\
					break;\
				case STEP_DATA:\
					if (incomingByte == '-')\
						neg_sign = true;\
					if (incomingByte < 48 || incomingByte > 57)\
						break;\
					foundData *= 10;                    /* *10 => shift to left */ \
					foundData = ((incomingByte - 48) + foundData);  /* 48 because of ASCII value (1 => 49 in ASCII) */ \
					break;\
				default:\
					break;\
				}\
			}\
\
\
		if (neg_sign == true)\
			foundData = -foundData;\
 \
		SERIAL_MACRO_DEBUG_EVENT \
		}\
\
		if (foundID != UNDEFINED_ID)\
			Commander::RaiseEvent(foundID, foundEvent, foundData);\
		Commanders_SetLastEventType(foundEvent); \
		Commanders_SetLastEventData(foundData); \
\
		return foundID;\
	}\
\
};

//-------------------------------------------------------------------
#endif
