//-------------------------------------------------------------------
#ifndef __i2cCommander_H__
#define __i2cCommander_H__
//-------------------------------------------------------------------

#include "Commanders.h"

#ifndef NO_I2CCOMMANDER
#ifdef VISUALSTUDIO
	#include "../VStudio/Wire.h"
#else
	#include <Wire.h>
#endif

/*-------------------------------------------------------------------

Only one receiver can be defined inside a UAD session. This is why 
the I2CSlaveId and LastEventId are defined in static, and initialized
by the static method begin().

-------------------------------------------------------------------*/

class I2CCommander : Commander
{
	public:
		static uint8_t I2CSlaveId;
		static unsigned long LastEventId;

		inline I2CCommander() : Commander() {}
		unsigned long loop();

		static void begin(uint8_t inI2CSlaveID);
};

//-------------------------------------------------------------------
#endif
#endif
//-------------------------------------------------------------------
