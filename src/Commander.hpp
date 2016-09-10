//-------------------------------------------------------------------
#ifndef __commander_HPP__
#define __commander_HPP__
//-------------------------------------------------------------------

#ifdef VISUALSTUDIO
	#include "../../DIO2/VStudio/DIO2.h"
#else
	#include "DIO2.h"
#endif

#include "Events.h"

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
		inline Commander() { pNextCommander = 0; AddCommander(this); }
		
		inline virtual unsigned long loop() { return UNDEFINED_ID; }
		inline virtual void PriorityLoop() { }
		void CommanderPriorityLoop();

		static unsigned long loops();

	private:
		static void AddCommander(Commander *inCommander);

#ifdef COMMANDERS_DEBUG_MODE
	public:
#ifdef ARDUINO_ARCH_SAM
		void CheckIndex(uint8_t inIndex, const char *infunc);
#else
		void CheckIndex(uint8_t inIndex, const __FlashStringHelper *infunc);
#endif
#endif
};

//-------------------------------------------------------------------
#endif
