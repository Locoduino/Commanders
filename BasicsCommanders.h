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

#ifdef VISUALC
#include "VStudio/arduino.h"
#include "VStudio/arduino2.hpp"
#include "VStudio/Serial.hpp"
#else
#include "arduino2.hpp"
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
//	SerialCommander.cpp
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
//#define NO_BUTTONSCOMMANDERSWITCH
//#define NO_BUTTONSCOMMANDERPOTENTIOMETER
//#define NO_DCCCOMMANDER

// Beware of this comander, which uses 150 bytes of dynamic memory for each serial channel open !
#define NO_SERIALCOMMANDER

#ifndef NO_SERIALCOMMANDER
// Just comment the serial line you want to use !
#define NO_SERIALCOMMANDER_SERIAL
#define NO_SERIALCOMMANDER_SERIAL1
#define NO_SERIALCOMMANDER_SERIAL2
#define NO_SERIALCOMMANDER_SERIAL3
#endif

/////////////////////////////////////

#include "Commander.hpp"

#ifndef NO_BUTTONSCOMMANDER
#ifndef NO_BUTTONSCOMMANDERENCODER
#include "ButtonsCommanderEncoder.hpp"
#endif
#ifndef NO_BUTTONSCOMMANDERPUSH
#include "ButtonsCommanderPush.hpp"
#endif
#ifndef NO_BUTTONSCOMMANDERSWITCH
#include "ButtonsCommanderSwitch.hpp"
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

void BasicsCommanders_StartSetup();
void BasicsCommanders_EndSetup();
void BasicsCommanders_Loop();
