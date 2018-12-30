//-------------------------------------------------------------------
#ifndef __dccCommanderNMRA_H__
#define __dccCommanderNMRA_H__
//-------------------------------------------------------------------

#include <Commanders.h>

#ifndef NO_DCCCOMMANDERNMRA

#include <NmraDcc.h>

/** In your sketch, you use DccCommander, but in reality, this is the current instance of the DccCommanderClass !*/
#define DccCommanderNMRA DccCommanderNMRAClass::GetCurrent()

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
deactivating. Bits 1 and 2 of byte two are used to indicate which of 4 pairs of outputs the packet is controlling. The
most significant bits of the 9-bit address are bits 4-6 of the second data byte. By convention these bits (bits 4-6 of
the second data byte) are in ones complement.</blockquote>

When an accessory is activated through my MS2, these packets are sent:
\verbatim
id / data / 1
id / data / 1
id / data / 1
id / data / 0
\endverbatim

One an Ecos, only the '1' packets are sent, out of the standard...
\verbatim
id / data / 1
id / data / 1
id / data / 1
\endverbatim

The last flag is to activate for a while (three times at 1 !) and then deactivate the motor !
DccCommander will react only on the first event to avoid double events. By default the '0' packet is used to raise an event,
but for an Ecos or a '1' only Dcc central, use RaiseEventWhen() function with first argument to true.

In the Commanders interpretation of a DCC packet, id is given by 'A' bits, data from the 'D' bits, and the activate flag by the 'C' bit.
The id is not the number you can see on a MS2, and on most DCC controllers. There is a small computation between id and data to obtain 
the real MS2 accessory number : real_id (1 to 320) and real_data (0 or 1 for right or left). It works on all known controllers, except on Multimaus...

The translated id and the activation flag are then translated into an unsigned long to be used as a Commanders id. The formula is
\verbatim
real_data * 10000 + real_id   .
\endverbatim
DCCINT is the macro used to build the unsigned long, DCCID the one to use to get the real id number from this long int, and DCCACTIVATION the 
other to get the real_data.

Events thrown:

reason   |      id      |         type          | data
---------|--------------|-----------------------|-----------------------
data = 0 | accessory id | COMMANDERS_EVENT_MOVE | COMMANDERS_MOVE_RIGHT
data = 1 | accessory id | COMMANDERS_EVENT_MOVE | COMMANDERS_MOVE_LEFT

The NMRA version of the decoder handles the 'Roco mode'. On some Roco controllers, the first board of four accessories is numbered 1, instead of 0
for all the other manufacturers. This gives a shift of 4 to the accessories number... To avoid this shift, set the RocoMode flag to true in beginByPin()
function. The default is false, of course.
*/

class DccCommanderNMRAClass : Commander
{
	private:
		unsigned long lastDccId;
		unsigned long lastEventDate;
		boolean raiseEventOnActivation;
		boolean rocoMode;
		int raiseEventDebounceDelay;
		static DccCommanderNMRAClass *pDccCommander;

	public:									
		static NmraDcc Dcc;

		/** Default constructor.*/
		inline DccCommanderNMRAClass() : Commander() {}
		
		/** Initialize the instance.
		@param inManufacturerId	Manufacturer ID of this decoder. Can be 0 in common case...
		@param inVersionId	Manufacturer version number for this decoder. Can be 0 in common case...
		@param interruptPinNumber	Interrupt pin number used by DCC_Decoder. The interrupt is raised when a signal is detected on the associated pin.
		@param inInterruptMonitor	If true, the Commanders status led will blink when DccCommander receive a new accessory packet. Default is false.
		@param inRocoMode	On some commercial Dcc stations, like those from Roco, the list of boards start at 1, not 0, giving a shift of 4 to the accessory number.
											To avoid this shift, set this argument to true.
		the original values can be preserved instead of these converted value. Default is false.
		*/
		void beginByPin(int inManufacturerId, int inVersionId, int interruptNumber, boolean inInterruptMonitor = false, boolean inRocoMode = false);

		/** Defines on which DCC packet the Commanders event should be raised. By default, an event is raised when the deactivation packet is received.
		After the reception on the 'event' packet, there is no other reception on the accessory during a given delay, 200ms by default.
		@param inRaiseEventOnActivation	if true, the event will be raised only at reception of a deactivation packet.
		@param When a 'event' packet has been received, no other event will be raised on this accessory during this delay.
		*/
		void RaiseEventWhen(boolean inRaiseEventOnActivation, int inRaiseEventDebounceDelay = 200);

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
		inline unsigned long GetLastDccId() { return this->lastDccId; }
		/** Gets the Roco mode flag.*/
		inline bool GetRocoMode() { return this->rocoMode; }

		/** For internal use only... */
		static void CheckLastReception(uint16_t inBoardAddr, uint8_t inOutputPair, uint8_t inDirection, uint8_t inOutputPower);

	public:
		/** Get the current instance of DccCommanderClass
		@remark This is an internal function.
		*/
		static inline DccCommanderNMRAClass &GetCurrent()
		{ 
			if (pDccCommander == NULL)
				pDccCommander = new DccCommanderNMRAClass();

			return *(DccCommanderNMRAClass::pDccCommander);
		}

#ifdef COMMANDERS_DEBUG_MODE
	public:
		/** Print the given event on the console.
		@remark Only available if COMMANDERS_DEBUG_MODE is defined.
		*/
		static void printEvent(unsigned long inId, COMMANDERS_EVENT_TYPE inEventType, int inEventData);
		#define PRINT_EVENT_DCC(id, type, data)		DccCommanderNMRA.printEvent(id, type, data);
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
