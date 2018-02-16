//-------------------------------------------------------------------
#ifndef __serialCommander_H__
#define __serialCommander_H__
//-------------------------------------------------------------------

#include <Commanders.h>

/**Defines the normal name of the class.*/
#define SerialCommander SerialCommanderClass::GetCurrent()

#ifdef NO_SERIALCOMMANDER
#pragma message ("Commanders : No Serial commander !")
#endif

/** @file SerialCommander.hpp

This is the most curious commander, because the class itself does not exists until you add the macro SERIAL_COMMANDER to your sketch.

    #include "Commanders.h"

    SERIAL_COMMANDER(Serial);

and in this case, it has the usual form of a Commander :

    class SerialCommanderClass : Commander
    {
    public:
		inline SerialCommanderClass() : Commander() { }
    
		inline void begin() { }
    
		inline unsigned long loop() 
		{
			...
    	}
    	
    	static inline SerialCommanderClass &GetCurrent() 
    	{
    		...
     	}
    	void printCommander() { ... }
    };    

You should avoid to declare a SERIAL_COMMANDER if not necessary, because the Serial library delivered with the Arduino IDE
allocates a lot of memory for each serial channel used.
The only variable thing in this macro is the name of the serial channel to use : Serial for a UNO or a NANO, but Serial or
Serial1 to 3 on a Mega or a Due. Even a SoftwareSerial (from SoftwareSerial library (https://www.arduino.cc/en/Reference/SoftwareSerial) ) or a AltSoftSerial
(from the well named library.. AltSoftSerial (https://github.com/PaulStoffregen/AltSoftSerial) !) can be used.

The syntax of the message to get to throw a new event is given by the class TextInterpreter.

Events thrown:

reason           | id | type | data
-----------------|----|------|------
message received | id | type | data
*/

//-------------------------------------------------------------------

/** Use this macro to define a SerialCommander.
@param SERIAL_PORT	name of the serial port. See the file description for more information.
*/
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
