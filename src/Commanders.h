//-------------------------------------------------------------------
#ifndef __commanders_H__
#define __commanders_H__
//-------------------------------------------------------------------

////////////////////////////////////////////////////////
// Add a '//' at the beginning of the line to be in 
// release mode.
#define COMMANDERS_DEBUG_MODE

///////////////////////////////////////////////////////
// Verbose mode lets you see all actions done by the 
// library, but with a real flood of text to console...
// Has no effect if COMMANDERS_DEBUG_MODE is not activated.
//#define DEBUG_VERBOSE_MODE

#define  GPIO2_PREFER_SPEED    1

#ifdef VISUALSTUDIO
#include "../../DIO2/VStudio/DIO2.h"
#else
#include "arduino.h"
#include "DIO2.h"
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
//NO_CANCOMMANDER
//	CANCommander.cpp
//	CANCommander.hpp
//
//NO_DCCCOMMANDER
//	DccCommander.cpp
//	DccCommander.hpp
//	DCC_Decoder.cpp
//	DCC_Decoder.hpp
//
//NO_I2CCOMMANDER
//	I2CCommander.cpp
//	I2CCommander.hpp
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
//NO_BUTTONSCOMMANDERANALOGPUSHES
//	ButtonsCommanderAnalogPush.cpp
//	ButtonsCommanderAnalogPush.hpp
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
//#define NO_CANCOMMANDER
//#define NO_DCCCOMMANDER
//#define NO_I2CCOMMANDER

/////////////////////////////////////

#include "Commander.hpp"

#ifndef NO_CANCOMMANDER
#include "CANCommander.hpp"
#endif

#ifndef NO_DCCCOMMANDER
#include "DccCommander.hpp"
#endif

#ifndef NO_I2CCOMMANDER
#include "I2CCommander.hpp"
#endif

#ifndef NO_BUTTONSCOMMANDER
#include "ButtonsCommander.hpp"
#endif

#include "SerialCommander.hpp"
#include "TextInterpreter.hpp"

#include "Commanders.hpp"

////////////////////////////////////////////////
// Macro area...

// Buttons
#define DECLARE_BUTTONS_COMMANDER		\
			ButtonsCommander macro_buttons

#define START_BUTTONS_COMMANDER_SETUP \
			Commanders::StartSetup(); \
			macro_buttons.begin()

#define START_BUTTONS_COMMANDER_SETUPEVENT(eventFct) \
			Commanders::StartSetup(eventFct); \
			macro_buttons.begin()

// Dcc
#define DECLARE_DCC_COMMANDER(INTER)	\
			const byte interrupt = INTER; \
			DccCommander macro_dcc

#define START_DCC_COMMANDER_SETUP \
			Commanders::StartSetup(); \
			macro_dcc.begin(0x00, 0x00, interrupt)

#define START_DCC_COMMANDER_SETUPEVENT(eventFct) \
			Commanders::StartSetup(eventFct); \
			macro_dcc.begin(0x00, 0x00, interrupt)

// Serial
#define DECLARE_SERIAL_COMMANDER(PORT, SPEED)	\
			SERIAL_COMMANDER(PORT) \
			const unsigned long speed = SPEED; \
			SerialCommander macro_serial;

#define START_SERIAL_COMMANDER_SETUP \
			Commanders::StartSetup(); \
			macro_serial.begin(speed)

#define START_SERIAL_COMMANDER_SETUPEVENT(eventFct) \
			Commanders::StartSetup(eventFct); \
			macro_serial.begin(speed)

// CAN
#define DECLARE_CAN_COMMANDER(SLAVE_NUMBER)	\
			CANCommander macro_can(SLAVE_NUMBER);

#define START_CAN_COMMANDER_SETUP(inPin, inSpeed, inInterrupt) \
			Commanders::StartSetup(); \
			macro_can.begin(inPin, inSpeed, inInterrupt)

#define START_CAN_COMMANDER_SETUPEVENT(eventFct, inPin, inSpeed, inInterrupt) \
			Commanders::StartSetup(eventFct); \
			macro_can.begin(inPin, inSpeed, inInterrupt)

// I2C
#define DECLARE_I2C_COMMANDER(SLAVE_NUMBER)	\
			const uint8_t I2CslaveNb = SLAVE_NUMBER; \
			I2CCommander macro_i2c;

#define START_I2C_COMMANDER_SETUP \
			Commanders::StartSetup(); \
			macro_i2c.begin(I2CslaveNb)

#define START_I2C_COMMANDER_SETUPEVENT(eventFct) \
			Commanders::StartSetup(eventFct); \
			macro_i2c.begin(I2CslaveNb)

// Common
#define COMMANDERS_SET_EVENTHANDLER(func)	Commanders::SetEventHandler(func)
#define COMMANDERS_SET_STATUSLED(pin)		Commanders::SetStatusLedPin(pin)
#define END_COMMANDERS_SETUP 	Commanders::EndSetup()
#define COMMANDERS_LOOP			Commanders::loop()

#endif