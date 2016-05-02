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

enum COMMANDERS_EVENT_TYPE
{
	COMMANDERS_EVENT_NONE = 0,			// Should never appear
	COMMANDERS_EVENT_TOGGLE = 1,		// If a push button or similar is pressed
	COMMANDERS_EVENT_MOVELEFT = 2,		// If a push 'left' button or similar is pressed
	COMMANDERS_EVENT_MOVERIGHT = 3,		// If a push 'right' button or similar is pressed
	COMMANDERS_EVENT_MOVESTOP = 4,		// If a push 'stop' button or similar is pressed
	COMMANDERS_EVENT_ABSOLUTEMOVE = 5,	// If a potentiometer or similar is moved
	COMMANDERS_EVENT_RELATIVEMOVE = 6	// If an encoder or similar is moved
};

typedef void(*CommandersEventHandlerFunction)(unsigned long inId, COMMANDERS_EVENT_TYPE inEvent, int inData);

#ifndef UNDEFINED_ID
#define UNDEFINED_ID	((unsigned long)-1)
#endif

class Commander
{
	public:
		unsigned long Id;
		Commander *pNextCommander;

		// Start of the linked list of all commanders. Each commander have the address of its follower or NULL !
		static Commander *pFirstCommander;
		static CommandersEventHandlerFunction EventHandler;
		static GPIO_pin_t StatusLedPin;
		
	public:
		inline Commander() { AddCommander(this); pNextCommander = 0; }
		
		virtual void begin(int inStatusLedPin = 0) {}
		inline virtual unsigned long loop() { return UNDEFINED_ID; }
		inline virtual void PriorityLoop() { }
		static void StatusBlink();
		void CommanderPriorityLoop();
		static void RaiseEvent(unsigned long inId, COMMANDERS_EVENT_TYPE inEvent, int inData);

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
//-------------------------------------------------------------------
