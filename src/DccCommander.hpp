//-------------------------------------------------------------------
#ifndef __dccCommander_H__
#define __dccCommander_H__
//-------------------------------------------------------------------

#define  GPIO2_PREFER_SPEED    1
#ifdef VISUALC
#include "VStudio/arduino2.hpp"
#include "VStudio/Dcc_Decoder.hpp"
#else
#include "arduino2.hpp"
#include "DCC_Decoder.hpp"
#endif

#include "Commander.hpp"

#ifndef NO_DCC
#ifndef NO_COMMANDER

//-------------------------------------------------------------------

typedef void(*DccBasicAccDecoderPacket)(int address, boolean activate, byte data);

// A dcc id is a long int with accessory id * 10000 + dcc id .
#define DCCID(id)			((id) % 10000)
#define DCCACCESSORY(id)	((byte)((id) / 10000))
#define DCCINT(id, acc)		((acc) * 10000UL + (id))

class DccCommander : Commander
{
	public:
		static GPIO_pin_t dccStatusLedPin;
		static boolean UseRawDccAddresses;
		static DccBasicAccDecoderPacket    func_BasicAccPacket;

	public:
		inline DccCommander() : Commander() { this->dccStatusLedPin = DP0; }
		
	public:
		void Setup(int i, int j, int k, boolean inUseRawDccAddresses = false);
		void SetStatusLedPin(int inPin);
		void PriorityLoop();
		unsigned long Loop();
		static void SetBasicAccessoryDecoderPacketHandler(DccBasicAccDecoderPacket func);

#ifdef DEBUG_MODE
public:
	void CheckIndex(unsigned char inIndex, const __FlashStringHelper *infunc);
#endif
};

//-------------------------------------------------------------------
#endif
#endif
#endif
//-------------------------------------------------------------------
