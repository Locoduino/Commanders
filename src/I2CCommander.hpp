//-------------------------------------------------------------------
#ifndef __i2cCommander_H__
#define __i2cCommander_H__
//-------------------------------------------------------------------

#include <Commanders.h>

#ifndef NO_I2CCOMMANDER
#ifdef VISUALSTUDIO
	#include "Wire.hpp"
#else
	#include <Wire.h>
#endif

#define I2CCommander I2CCommanderClass::GetCurrent()

/** This Commanders receive orders from I2C bus, and translate it into Commanders events.

On an I2C bus, there is one and only one master, and one or more slaves. In Commanders implementation,
only one slave can be defined. The master send messages to a specific slave, defined by a number: the slave ID.

A sample of the library give a master, sender of events to Commanders.

To work, the master must send a message with the form of
\verbatim
IIIIEDD
\endverbatim
where IIII is an unsigned long (four bytes) for the event id, E is the event type (see values of COMMANDERS_EVENT_TYPE), and DD the associated data.

Events thrown:

reason           | id | type | data
-----------------|----|------|------
message received | id | type | data
*/

class I2CCommanderClass : Commander
{
	private:
		uint8_t I2CSlaveId;
		static I2CCommanderClass *pI2cCommander;

	public:
		/** Default constructor.*/
		inline I2CCommanderClass() : Commander() {}

		/** Initialize the instance.
		@param inI2CSlaveID	I2C Slave ID. This Id must be unique on all connected devices to the I2C master.
		*/
		void begin(uint8_t inI2CSlaveID);

		/** Main loop function. */
		unsigned long loop();

	public:
		/** Get the current instance of DccCommanderClass
		@remark This is an internal function.
		*/
		static inline I2CCommanderClass &GetCurrent()
		{
			if (pI2cCommander == NULL)
				pI2cCommander = new I2CCommanderClass();

			return *(I2CCommanderClass::pI2cCommander);
		}
#ifdef COMMANDERS_PRINT_COMMANDERS
		/** Print this Commander on the console.
		@remark Only available if COMMANDERS_PRINT_COMMANDERS is defined.
		*/
		void printCommander();
#endif
};

//-------------------------------------------------------------------
#endif
#endif
//-------------------------------------------------------------------
