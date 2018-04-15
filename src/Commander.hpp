//-------------------------------------------------------------------
#ifndef __commander_HPP__
#define __commander_HPP__
//-------------------------------------------------------------------

#include <Commanders.h>
#include "DIO2.h"
#include "Events.h"

#define COMMANDERSCONFIG(address, value)	( ((int)address<<8) | value )
#define COMMANDERSCONFIGADDRESS(data)		highByte((int)data)
#define COMMANDERSCONFIGVALUE(data)			lowByte((int)data)

/**
This is the base class for all the commanders.

DO NOT INSTANCIATE THIS CLASS !

A commander is a class with a loop function which is able to raise events for a source.
Each commander is an item into a linked list of commanders.
There is one and only one Commander of a given type during a library session.
*/
class Commander
{
	private:
		Commander *pNextCommander;
		static Commander *pFirstCommander;

	public:
		/** Default constructor. */
		inline Commander() { pNextCommander = 0; AddCommander(this); }
		
		/** Priority loop function.
		A commander can need more call to its loop than normal. The priority loop functions of all the commanders
		will be called before any commander loop.
		*/
		inline virtual void PriorityLoop() { }
		/** This function will be called after sketch Setup(), but before the first call to loop().
		It will be called only one time.*/
		inline virtual void beforeFirstLoop() { }
		/** Main loop of this commander.
		@return Event Id if an event has been raised, or UNDEFINED_ID if not.
		*/
		inline virtual unsigned long loop() { return UNDEFINED_ID; }

		/** This function will call all PriorityLoop() in sequence.*/
		static void CommanderPriorityLoops();
		/** This function will call all BeforeFirstLoop() in sequence.*/
		static void BeforeFirstLoops();
		/** This function will call all loop() in sequence.
		@return Event Id if an event has been raised, or UNDEFINED_ID if not.*/
		static unsigned long loops();

	private:
		/** Add a new commander to the main list.*/
		static void AddCommander(Commander *inCommander);

	public:
#ifdef COMMANDERS_PRINT_COMMANDERS
		/** Print all commanders and items on console.
		@remark Only available if COMMANDERS_PRINT_COMMANDERS is defined.
		*/
		static void printCommanders();
		/** Print this commander and items on console.
		@remark Only available if COMMANDERS_PRINT_COMMANDERS is defined.
		*/
		virtual void printCommander() {}
#endif
};

#ifdef COMMANDERS_PRINT_COMMANDERS
/** This define is empty if COMMANDERS_PRINT_COMMANDERS is not defined. */
#define PRINT_COMMANDERS()	Commander::printCommanders();
#else
#define PRINT_COMMANDERS()
#endif

//-------------------------------------------------------------------
#endif
