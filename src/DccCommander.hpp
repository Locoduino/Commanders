//-------------------------------------------------------------------
#ifndef __dccCommander_H__
#define __dccCommander_H__
//-------------------------------------------------------------------

#include <Commanders.h>

#ifndef NO_DCCCOMMANDER

#include <DCC_Decoder.hpp>

/** In your sketch, you use DccCommander, but in reality, this is the current instance of the DccCommanderClass !*/
#define DccCommander DccCommanderClass::GetCurrent()

//-------------------------------------------------------------------

typedef void(*DccAccDecoderPacket)(int address, boolean activate, uint8_t data);

// A dcc id is a long int with accessory id * 10000 + dcc id .
#define DCCID(id)			((id) % 10000)
#define DCCACTIVATION(id)	((uint8_t)((id) / 10000))
#define DCCINT(id, acc)		((acc) * 10000UL + (id))

/**
This commanders receive accessory packets from DCC controller, and convert it in Commanders events.
The event id is a compressed version of dcc id and dcc accessory id.

Text from NMRA standard definition (https://www.nmra.org/sites/default/files/s-9.2.1_2012_07.pdf , paragraph 420):

<blockquote>The format for packets intended for Accessory Digital Decoders is:
\verbatim
		{preamble} 0 10AAAAAA 0 1AAACDDD 0 EEEEEEEE 1
\endverbatim
Accessory Digital Decoders can be designed to control momentary or constant-on devices, the duration of time each
output is active being controlled by configuration variables CVs #515 through 518. Bit 3 of the second byte "C" is
used to activate or deactivate the addressed device. (Note if the duration the device is intended to be on is less than
or equal the set duration, no deactivation is necessary.) Since most devices are paired, the convention is that bit "0"
of the second byte is used to distinguish between which of a pair of outputs the accessory decoder is activating or
430 deactivating. Bits 1 and 2 of byte two are used to indicate which of 4 pairs of outputs the packet is controlling. The
most significant bits of the 9-bit address are bits 4-6 of the second data byte. By convention these bits (bits 4-6 of
the second data byte) are in ones complement.</blockquote>

When an accessory is activated through my MS2, these packets are sent:
\verbatim
id / data / 1
id / data / 1
id / data / 1
id / data / 0
\endverbatim

In the Commanders interpretation of a DCC packet, id is given by 'A' bits, data from the 'D' bits, and the activate flag by the 'C' bit.
The id is not the number you can see on a MS2, and on most DCC controllers. There is a small computation between id and data to obtain 
the real MS2 accessory number : real_id (1 to 320) and real_data (0 or 1 for right or left). It works on all known controllers, except on Multimaus...

The translated id and the activation flag are then translated into an unsigned long to be used as a Commanders id. The formula is
\verbatim
real_data * 10000 + real_id   .
\endverbatim
DCCINT is the macro used to build the unsigned long, DCCID the one to use to get the real id number from this long int, and DCCACTIVATION the 
other to get the real_data.

This flag is to activate for a while (three times at 1 !) and then deactivate the motor !
DccCommander will react only on the deactivate flag to avoid double events.

Events thrown:

  reason |      id      |         type          | data
---------|--------------|-----------------------|-----------------------
data = 0 | accessory id | COMMANDERS_EVENT_MOVE | COMMANDERS_MOVE_RIGHT
data = 1 | accessory id | COMMANDERS_EVENT_MOVE | COMMANDERS_MOVE_LEFT
*/

class DccCommanderClass : Commander
{
	private:
		unsigned long LastDccId;
		boolean UseRawDccAddresses;
		static DccAccDecoderPacket    func_AccPacket;
		static DccCommanderClass *pDccCommander;

	public:									
		/** Default constructor.*/
		inline DccCommanderClass() : Commander() {}
		
		/** Initialize the instance.
		@param i	Manufacturer ID of this decoder. Can be 0 in common case...
		@param j	Manufacturer version number for this decoder. Can be 0 in common case...
		@param k	Interrupt number used by DCC_Decoder. The interrupt is raised when a signal is detected on the associated pin.
		@param inInterruptMonitor	If true, the Commanders status led will blink when DccCommander receive a new accessory packet. Default is false.
		@param inUseRawDccAddresses	Dcc id and accessory id are converted into a more readable way to be used, but if this flag is true, 
		the original values can be preserved instead of these converted value. Default is false.
		*/
		void begin(int i, int j, int k, boolean inInterruptMonitor = false, boolean inUseRawDccAddresses = false);
		/** Priority loop function.	Just a new call to the loop function. By this way, the DCC commander loop is sure to be called two or more times
		when other commanders are only called once !
		*/
		void PriorityLoop();
		/** Main loop function. */
		unsigned long loop();
		/** Set the callback function which will be called when an accessory packet will be decoded. Set it to NULL or 0 to inactivate this function.*/
		static void SetAccessoryDecoderPacketHandler(DccAccDecoderPacket func);
		/** Decode the packet data, and raise a Commander event.
		@param address	raw address of the decoder, directly got from the DCC packet.
		@param activate flag from the DCC packet. True means activate the accessory motor.
		@param data additional data, adding bits to decoder accessory, and including the number of the accessory.
		*/
		static void DccAccessoryDecoderPacket(int address, boolean activate, uint8_t data);
		/** Gets the last DCC id received.*/
		inline unsigned long GetLastDccId() { return this->LastDccId; }

	public:
		/** Get the current instance of DccCommanderClass
		@remark This is an internal function.
		*/
		static inline DccCommanderClass &GetCurrent() 
		{ 
			if (pDccCommander == NULL)
				pDccCommander = new DccCommanderClass();

			return *(DccCommanderClass::pDccCommander); 
		}

#ifdef COMMANDERS_DEBUG_MODE
public:
	/** Print the given event on the console.
	@remark Only available if COMMANDERS_DEBUG_MODE is defined.
	*/
	static void printEvent(unsigned long inId, COMMANDERS_EVENT_TYPE inEventType, int inEventData);
	#define PRINT_EVENT_DCC(id, type, data)		DccCommander::printEvent(id, type, data);
#else
	#define PRINT_EVENT_DCC(id, type, data)
#endif
#ifdef COMMANDERS_PRINT_COMMANDERS
private:
	int8_t Interrupt;
public:
	/** Print this commander on the console.
	@remark Only available if COMMANDERS_PRINT_COMMANDERS is defined.
	*/
	void printCommander();
#endif
};

//-------------------------------------------------------------------
#endif
#endif
//-------------------------------------------------------------------
