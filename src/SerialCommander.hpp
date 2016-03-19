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
			Serial.print(found.ID, DEC);\
			Serial.print(F(" / "));\
			Serial.print((int)found.Event, DEC);\
			Serial.print(F(" / "));\
			Serial.println(found.Data, DEC);
#endif

// SERIAL_PORT argument can be any of the available serials of the used card, but also any other
// class implementing begin(), read() and available() functions, like in SoftwareSerial or AltSoftSerial !

#define SERIAL_COMMANDER(SERIAL_PORT) \
\
class SerialCommander : Commander \
{\
public:\
	inline SerialCommander() : Commander() { }\
\
	inline void Setup(unsigned long inSpeed) { SERIAL_PORT.begin(inSpeed); }\
\
	inline CommanderEvent Loop() \
	{\
		enum STEP\
		{\
			STEP_ID,\
			STEP_TYPE,\
			STEP_DATA,\
			STEP_END\
		};\
\
		CommanderEvent found = EmptyEvent;\
\
		Commander::CommanderPriorityLoop();\
\
		if (SERIAL_PORT.available() > 0)\
		{\
			char buffer[10];\
			byte buffer_pos = 0;\
			STEP step = STEP_ID;\
\
			found.ID = 0;		/* reset */ \
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
							found.Event = COMMANDERS_EVENT_SELECTED;\
						if (buffer[0] == 'a' || buffer[0] == 'A')\
							found.Event = COMMANDERS_EVENT_ABSOLUTEMOVE;\
						if (buffer[0] == 'r' || buffer[0] == 'R')\
							found.Event = COMMANDERS_EVENT_RELATIVEMOVE;\
						step = STEP_DATA;\
						found.Data = 0;\
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
					found.ID *= 10;                    /* *10 => shift to left */ \
					found.ID = ((incomingByte - 48) + found.ID);  /* 48 because of ASCII value (1 => 49 in ASCII) */ \
					break;\
				case STEP_TYPE:\
					if (buffer_pos < 10)\
						buffer[buffer_pos++] = incomingByte;\
					break;\
				case STEP_DATA:\
					if (incomingByte < 48 || incomingByte > 57)\
						break;\
					found.Data *= 10;                    /* *10 => shift to left */ \
					found.Data = ((incomingByte - 48) + found.Data);  /* 48 because of ASCII value (1 => 49 in ASCII) */ \
					break;\
				default:\
					break;\
				}\
			}\
\
		SERIAL_MACRO_DEBUG_EVENT \
		}\
\
		if (found.ID != UNDEFINED_ID)\
			Commander::RaiseEvent(found.ID, found.Event, found.Data);\
		return found;\
	}\
\
};

//-------------------------------------------------------------------
#endif
