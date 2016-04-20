//-------------------------------------------------------------------
#ifndef __canCommander_H__
#define __canCommander_H__
//-------------------------------------------------------------------

#include "Commanders.h"

#ifndef NO_CANCOMMANDER
#ifdef VISUALSTUDIO
	#include "../VStudio/mcp_can.h"
#else
	#include <mcp_can.h>
#endif

//-------------------------------------------------------------------

class CANCommander : Commander
{
	private:
		MCP_CAN *pCan;
		static unsigned long lastEventId;

	public:
		inline CANCommander() : Commander() {}
		
		bool SendEvent(uint8_t inID, unsigned long inEventID, COMMANDERS_EVENT_TYPE inEventType, int inEventData);
		void begin(byte inPin, byte inSpeed, byte inInterrupt);
		unsigned long loop();
};

//-------------------------------------------------------------------
#endif
#endif
//-------------------------------------------------------------------
