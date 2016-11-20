//-------------------------------------------------------------------
#ifndef __commanders_H__
#define __commanders_H__
//-------------------------------------------------------------------

////////////////////////////////////////////////////////
// Add a '//' at the beginning of the line to be in 
// release mode.
//#define COMMANDERS_DEBUG_MODE

///////////////////////////////////////////////////////
// Verbose mode lets you see all actions done by the 
// library, but with a real flood of text to console...
// Has no effect if COMMANDERS_DEBUG_MODE is not activated.
//#define COMMANDERS_DEBUG_VERBOSE_MODE

///////////////////////////////////////////////////////
// The function Commanders::printCommanders()
// is very heavy in program memory. So to avoid problems
// you can make this function available by uncomment the next 
// line.
//#define COMMANDERS_PRINT_COMMANDERS

#define  GPIO2_PREFER_SPEED    1

#include "DIO2.h"

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
//NO_SERIALCOMMANDER
//	SerialCommander.hpp
//	TextInterpreter.cpp
//	TextInterpreter.hpp
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
#define NO_CANCOMMANDER
//#define NO_DCCCOMMANDER
//#define NO_I2CCOMMANDER
//#define NO_SERIALCOMMANDER

/////////////////////////////////////

#include "Commander.hpp"
#include "Commanders.hpp"

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

#ifndef NO_SERIALCOMMANDER
#include "SerialCommander.hpp"
#include "TextInterpreter.hpp"
#endif

#ifdef VISUALSTUDIO
#include "../VStudio/ButtonsCommanderKeyboard.hpp"
#endif

#endif