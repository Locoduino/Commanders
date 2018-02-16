//-------------------------------------------------------------------
#ifndef __canCommander_H__
#define __canCommander_H__
//-------------------------------------------------------------------

#include <Commanders.h>

#ifndef NO_CANCOMMANDER

#include <mcp_can.h>

#define CANCommander CANCommanderClass::GetCurrent()

/** This Commanders receive orders from the CAN bus, and translate it into Commanders events.

This CAN bus used a MCP2515 chip, and use the library mcp_can for that. This library uses also the SPI bus to
get data.

A sample of the library Commanders give a sender of CAN events to Commanders.

To work, the sender must send a message with the form of
\verbatim
IIIIEDD
\endverbatim
where IIII is an unsigned long (four bytes) for the event id, E is the event type (see values of COMMANDERS_EVENT_TYPE), and DD the associated data.

Events thrown:

reason           | id | type | data
-----------------|----|------|------
message received | id | type | data
*/
class CANCommanderClass : Commander
{
	private:
		static CANCommanderClass *pCANCommander;
		MCP_CAN *pCan;

		// Global variables for reading and writing messages.
		uint8_t IdR;				// Id for CAN_recup()
		unsigned char lenR = 0;		// Current length of the message
		unsigned char bufR[8];		// Receiving buffer.
		unsigned char bufS[8];		// Sending buffer.

		// Global variables for the circular buffer.
		unsigned char Circule[256];	// Circular buffer for read messages, handled by interrupt.
		int indexW, indexR, Ncan;	// Index of writing and reading buffer, and byte number to read.
		uint8_t CANoverflow = 0;	// flag overflow (Circular buffer full)

		void CAN_recup();

	public:
		/** Default constructor.*/
		inline CANCommanderClass() : Commander() {}
		
		/** Initialize the instance.
		@param inSPIpin pin for SPI to get data.
		@param inSpeed The speed can be (of course, use the defines from mcp_can_dfs.h)

		Value  | Speed (define name)
		------ | -------------------
		1	   | CAN_5KBPS    
		2	   | CAN_10KBPS   
		3	   | CAN_20KBPS   
		4	   | CAN_25KBPS   
		5	   | CAN_31K25BPS 
		6	   | CAN_33KBPS   
		7	   | CAN_40KBPS   
		8	   | CAN_50KBPS   
		9	   | CAN_80KBPS   
		10	   | CAN_83K3BPS  
		11	   | CAN_95KBPS   
		12	   | CAN_100KBPS  
		13	   | CAN_125KBPS  
		14	   | CAN_200KBPS  
		15	   | CAN_250KBPS  
		16	   | CAN_500KBPS  
		17	   | CAN_666KBPS  
		18	   | CAN_1000KBPS 

		@param inInterrupt	interrupt number for the reception.
		@param inId			CAN id. Only the messages with this ID in the header will be read.
		*/
		void begin(uint8_t inSPIpin, uint8_t inSpeed, uint8_t inInterrupt, uint16_t inId);
		/** Main loop function. */
		unsigned long loop();

	public:
		/** Get the current instance of DccCommanderClass
		@remark This is an internal function.
		*/
		static inline CANCommanderClass &GetCurrent()
		{
			if (pCANCommander == NULL)
				pCANCommander = new CANCommanderClass();

			return *(CANCommanderClass::pCANCommander);
		}
#ifdef COMMANDERS_PRINT_COMMANDERS
	private:
		uint8_t SPIpin;
		uint8_t Speed;
		uint8_t Interrupt;
		uint16_t Id;
	public:
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
