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

#define CANCommander CANCommanderClass::GetCurrent()

//-------------------------------------------------------------------

class CANCommanderClass : Commander
{
	private:
		MCP_CAN *pCan;
		static unsigned long lastEventId;

	public:
		inline CANCommanderClass() : Commander() {}
		
		void begin(byte inPin, byte inSpeed, byte inInterrupt, uint16_t inId);
		unsigned long loop();

	public:
		static CANCommanderClass *pCANCommander;
		static inline CANCommanderClass &GetCurrent()
		{
			if (pCANCommander == NULL)
				pCANCommander = new CANCommanderClass();

			return *(CANCommanderClass::pCANCommander);
		}
};

//-------------------------------------------------------------------
#endif
#endif
//-------------------------------------------------------------------
