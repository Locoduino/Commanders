////////////////////////////////////////////////////////
// Add a '//' at the beginning of the line to be in 
// release mode.
//#define DEBUG_MODE

///////////////////////////////////////////////////////
// Verbose mode lets you see all actions done by the 
// library, but with a real flood of text to console...
// Has no effect if DEBUG_MODE is not activated.
//#define DEBUG_VERBOSE_MODE

#define  GPIO2_PREFER_SPEED    1

#ifdef VISUALSTUDIO
#include "../VStudio/arduino.h"
#include "../VStudio/arduino2.hpp"
#include "../VStudio/Serial.hpp"
#else
#include "arduino.h"
#include "arduino2.hpp"
#endif

#ifdef VISUALSTUDIO
#include "../VStudio/ButtonsCommanderKeyboard.hpp"
#endif

//////////////////////////////////////////
//  Exclusion area
//
// You can exclude some parts of library here, to avoid losing program and data memory
// on parts you dont use.
// For example, if you dont want servos
// just uncomment the line #define NO_SERVO by removing // at the beggining.
//
// Arduino IDE build of list of the files to compile from the content of the library directory
// in a makefile and give it to Gcc. So Gcc compiles all the files of the directory
// even if its content is not used. The result is, if an unused source file contains
// static declarations, these statics will be allocated a this memory will be lost.
// The only solution I have found is to rename the source files to something IDE and Gcc dont know...
//
// So if you want to lose less memory, you can rename the linked files
// from .cpp to .ori, and from .hpp to from ori.
// See below the file names related to each exclusion:
//
//NO_SERIALCOMMANDER
//	SerialCommander.hpp
//
//NO_DCCCOMMANDER
//	DccCommander.cpp
//	DccCommander.hpp
//	DCC_Decoder.cpp
//	DCC_Decoder.hpp
//
//NO_BUTTONSCOMMANDER
//	ButtonsCommander.cpp
//	ButtonsCommander.hpp
//	ButtonsCommanderButtons.cpp
//	ButtonsCommanderButtons.hpp
//	ButtonsCommanderEncoder.cpp
//	ButtonsCommanderEncoder.hpp
//	ButtonsCommanderPush.cpp
//	ButtonsCommanderPush.hpp
//	ButtonsCommanderSwitch.cpp
//	ButtonsCommanderSwitch.hpp
//	ButtonsCommanderPotentiometer.cpp
//	ButtonsCommanderPotentiometer.hpp
//
//NO_BUTTONSCOMMANDERENCODER
//	ButtonsCommanderEncoder.cpp
//	ButtonsCommanderEncoder.hpp
//
//NO_BUTTONSCOMMANDERPUSH
//	ButtonsCommanderPush.cpp
//	ButtonsCommanderPush.hpp
//
//NO_BUTTONSCOMMANDERSWITCH
//	ButtonsCommanderSwitch.cpp
//	ButtonsCommanderSwitch.hpp
//
//NO_BUTTONSCOMMANDERPOTENTIOMETER
//	ButtonsCommanderPotentiometer.cpp
//	ButtonsCommanderPotentiometer.hpp

//#define NO_BUTTONSCOMMANDER
//#define NO_BUTTONSCOMMANDERENCODER
//#define NO_BUTTONSCOMMANDERPUSH
//#define NO_BUTTONSCOMMANDERANALOGPUSHES
//#define NO_BUTTONSCOMMANDERSWITCH
//#define NO_BUTTONSCOMMANDERPOTENTIOMETER
//#define NO_DCCCOMMANDER

/////////////////////////////////////

#include "Commander.hpp"

#ifndef NO_BUTTONSCOMMANDER
#ifndef NO_BUTTONSCOMMANDERPUSH
#include "ButtonsCommanderPush.hpp"
#endif
#ifndef NO_BUTTONSCOMMANDERANALOGPUSHES
#include "ButtonsCommanderAnalogPushes.hpp"
#endif
#ifndef NO_BUTTONSCOMMANDERSWITCH
#include "ButtonsCommanderSwitch.hpp"
#endif
#ifndef NO_BUTTONSCOMMANDERENCODER
#include "ButtonsCommanderEncoder.hpp"
#endif
#ifndef NO_BUTTONSCOMMANDERPOTENTIOMETER
#include "ButtonsCommanderPotentiometer.hpp"
#endif
#endif

#ifndef NO_DCCCOMMANDER
#include "DccCommander.hpp"
#endif
#ifndef NO_SERIALCOMMANDER
#include "SerialCommander.hpp"
#endif
#ifndef NO_BUTTONSCOMMANDER
#include "ButtonsCommander.hpp"
#endif

void Commanders_StartSetup(CommandersEventHandlerFunction func);
void Commanders_StartSetup();
void Commanders_EndSetup();
unsigned long Commanders_Loop();

COMMANDERS_EVENT_TYPE Commanders_GetLastEventType();
int Commanders_GetLastEventData();

void Commanders_SetLastEventType(COMMANDERS_EVENT_TYPE inEvent);
void Commanders_SetLastEventData(int inEvent);

////////////////////////////////////////////////
// Macro area...

// Buttons
#define DECLARE_BUTTONS_COMMANDER		\
			ButtonsCommander macro_buttons

#define START_BUTTONS_COMMANDER_SETUP \
			Commanders_StartSetup(); \
			macro_buttons.Setup()

#define START_BUTTONS_COMMANDER_SETUPEVENT(eventFct) \
			Commanders_StartSetup(eventFct); \
			macro_buttons.Setup()

// Dcc
#define DECLARE_DCC_COMMANDER(INTER)	\
			const byte interrupt = INTER; \
			DccCommander macro_dcc

#define START_DCC_COMMANDER_SETUP \
			Commanders_StartSetup(); \
			macro_dcc.Setup(0x00, 0x00, interrupt); \
			macro_dcc.SetStatusLedPin(13)

#define START_DCC_COMMANDER_SETUPEVENT(eventFct) \
			Commanders_StartSetup(eventFct); \
			macro_dcc.Setup(0x00, 0x00, interrupt); \
			macro_dcc.SetStatusLedPin(13)

// Serial
#define DECLARE_SERIAL_COMMANDER(PORT, SPEED)	\
			SERIAL_COMMANDER(PORT) \
			const unsigned long speed = SPEED; \
			SerialCommander macro_serial;

#define START_SERIAL_COMMANDER_SETUP \
			Commanders_StartSetup(); \
			macro_serial.Setup(speed)

#define START_SERIAL_COMMANDER_SETUPEVENT(eventFct) \
			Commanders_StartSetup(eventFct); \
			macro_serial.Setup(speed)

// Common
#define END_COMMANDERS_SETUP 	Commanders_EndSetup()
#define COMMANDERS_LOOP			Commanders_Loop()