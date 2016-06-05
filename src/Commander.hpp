//-------------------------------------------------------------------
#ifndef __commander_H__
#define __commander_H__
//-------------------------------------------------------------------

#ifdef VISUALSTUDIO
	#include "../../DIO2/VStudio/DIO2.h"
#else
	#include "arduino.h"
	#include "DIO2.h"
#endif

enum COMMANDERS_MOVE_TYPE
{
	COMMANDERS_MOVE_MORE = +1,
	COMMANDERS_MOVE_LESS = -1,
	COMMANDERS_MOVE_STOP = 0,
	COMMANDERS_MOVE_LEFT = -2,
	COMMANDERS_MOVE_RIGHT = -3,
	COMMANDERS_MOVE_CENTER = -4,
	COMMANDERS_MOVE_TOP = -5,
	COMMANDERS_MOVE_BOTTOM = -6,
	COMMANDERS_MOVE_STRAIGHT = -7,
	COMMANDERS_MOVE_DIVERGE = -8,
	COMMANDERS_MOVE_ON = -9,
	COMMANDERS_MOVE_OFF = -10
};

enum COMMANDERS_EVENT_TYPE
{
	COMMANDERS_EVENT_NONE = 0,			// Should never appear
	COMMANDERS_EVENT_TOGGLE = 1,		// If a push button or similar is pressed, invert the state/position
	COMMANDERS_EVENT_MOVE = 2,			// If a push 'left' button or similar is pressed, data is a COMMANDERS_MOVE_TYPE
	COMMANDERS_EVENT_MOVEPOSITION = 3,	// If a potentiometer or similar is moved, data is the position to reach
	COMMANDERS_EVENT_CONFIG = 4			// Data is the configuration address and value
};

#ifndef UNDEFINED_ID
#define UNDEFINED_ID	((unsigned long)-1)
#endif

#define COMMANDERSCONFIG(address, value)	( ((int)address<<8) | value )
#define COMMANDERSCONFIGADDRESS(data)		highByte((int)data)
#define COMMANDERSCONFIGVALUE(data)			lowByte((int)data)

class Commander
{
	public:
		Commander *pNextCommander;

		// Start of the linked list of all commanders. Each commander have the address of its follower or NULL !
		static Commander *pFirstCommander;

	public:
		inline Commander() { AddCommander(this); pNextCommander = 0; }
		
		inline virtual unsigned long loop() { return UNDEFINED_ID; }
		inline virtual void PriorityLoop() { }
		void CommanderPriorityLoop();

		static unsigned long loops();

	private:
		static void AddCommander(Commander *inCommander);

#ifdef COMMANDERS_DEBUG_MODE
	public:
		void CheckIndex(byte inIndex, const __FlashStringHelper *infunc);
#endif
};

//-------------------------------------------------------------------
#endif
