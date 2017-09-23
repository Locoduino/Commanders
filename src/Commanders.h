//-------------------------------------------------------------------
#ifndef __commanders_H__
#define __commanders_H__
//-------------------------------------------------------------------

/**	 @mainpage
A full explanation in french can be read at http://www.locoduino.org/spip.php?article165 .

The goal of this library is to virtualize input through a lot of ways (hardware buttons, bus, radio and so on)
in order to be able to drive the rest of the sketch without taking care of the way the information was got.

Passing this information from the commanders through the accessories is done by events of type COMMANDERS_TYPE_EVENT.
Each event have an Id, a type (MOVE, TOGGLE, MOVEPOSITION, LEFT, RIGHT, TOP, BOTTOM, etc... An additional integer
is added with some events to complete the event information : the position value for MOVEPOSITION for instance.

Each kind of Commanders can be excluded from compilation, for memory problems purpose or for naming conflicts.
Inside some Commanders, sub items can be excluded too, like the different buttons in the ButtonsCommander. 
You can exclude some parts of library here, to avoid losing program and data memory on parts you dont use.
For example, if you dont want DCC, just uncomment the line #define NO_DCCCOMMANDER by removing // at the beggining.

CANCommander is excluded by default, because this is a rare usage of the library, and the associated CAN libray mcp_can
consume memory for nothing if this commander is not used.

The Arduino IDE compiles all the files of the library 'src' directory even if its content is not used.
The result is, if an unused source file contains static declarations (SERIAL, DCC or CAN do !), these statics 
will be allocated and this memory will be lost. The only solution I have found is to rename the source files to
something IDE dont know...
So if you want to lose less memory, you can rename the linked files from .cpp to .ori, and from .hpp to .hppori.
See below in the exclusion area the file names related to each exclusion.
*/

/** @file Commanders.h
Main include file of the library.*/

///////////////////////////////////////////////////////////////////
// Remove the '//' at the begining of the line to be in debug mode.
//#define COMMANDERS_DEBUG_MODE

//////////////////////////////////////////////////////////////////
// Remove the '//' at the begining of the line to be in verbose debug mode.
//#define COMMANDERS_DEBUG_VERBOSE_MODE

//////////////////////////////////////////////////////////////////
// Remove the '//' at the begining of the line to activate the print function.
//#define COMMANDERS_PRINT_COMMANDERS

#ifdef DOXYGEN_SPECIFIC
/** If this is defined, the library will do many checks during setup and execution, and print errors, warnings and
information messages on console. These messages can take a lot of memory, so be careful about the free memory of
your program if you activate debug mode.*/
#define COMMANDERS_DEBUG_MODE
/** If this is defined, the Verbose mode lets you see all actions done by the  library, but with a real flood of
text to the console... It has no effect if COMMANDERS_DEBUG_MODE is not activated.*/
#define COMMANDERS_DEBUG_VERBOSE_MODE
/** If this is defined, the function Commanders::printCommanders() will become available. This is useful to try
to understand why a commander, or a commander item is not corretly defined.
This function uses a lot of memory, so activate it only if necessary, and be careful about your program's memory.
You can use the define PRINT_COMMANDERS() in your sketch instead of a call to Commanders::printCommanders().
If COMMANDERS_PRINT_COMMANDERS is not defined, PRINT_COMMANDERS is defined as empty, so you will not have a compilation error.*/
#define COMMANDERS_PRINT_COMMANDERS
#endif

/** This is used internally by DIO2.h */
#define  GPIO2_PREFER_SPEED    1

#include <DIO2.h>

//////////////////////////////////////////
//  Exclusion area
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
#define NO_DCCCOMMANDER
#define NO_I2CCOMMANDER
#define NO_SERIALCOMMANDER

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

#endif