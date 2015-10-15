//-------------------------------------------------------------------
#ifndef __commander_H__
#define __commander_H__
//-------------------------------------------------------------------

#define  GPIO2_PREFER_SPEED    1
#ifdef UAD_VC
#include "VStudio/arduino2.hpp"
#else
#include "arduino2.hpp"
#endif

class Commander;

// Describes static data only intialized once at the beginning of execution.

class StaticCommanderData
{
public:
	// List of all commanders declared.
	// For instance DCCCommander overrides a function PriorityLoop which must be called as often as possible.
	// Other loops (asscessories, groups) have just to call Commander::StaticData::CommanderPriorityLoop() to be sure
	// to call priority functions very often.
	Commander* *pCommanderList;
	int CommanderSize;
	int CommanderAddCounter;

	StaticCommanderData();

	void CommanderPriorityLoop();
};

typedef void(*EventHandlerFunction)(unsigned long inId, bool inAnalogData, int inData);
#define UNDEFINED_ID	((unsigned long)-1)

class Commander
{
	public:
		unsigned long Id;

		static StaticCommanderData StaticData;
		static EventHandlerFunction EventHandler;

	public:
		inline Commander() { AddCommander(this); }
		
	public:
		virtual void Setup() {}
		inline virtual unsigned long Loop() { return UNDEFINED_ID; }
		inline virtual void PriorityLoop() { }

		static void AddCommander(Commander *inCommander);
		static void Loops();

#ifdef DEBUG_MODE
	public:
		void CheckIndex(unsigned char inIndex, const __FlashStringHelper *infunc);
#endif
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
