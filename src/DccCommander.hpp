//-------------------------------------------------------------------
#ifndef __dccCommander_H__
#define __dccCommander_H__
//-------------------------------------------------------------------

#include "Commanders.h"

#ifdef VISUALSTUDIO
#include "../VStudio/Dcc_Decoder.hpp"
#else
#include "DCC_Decoder.hpp"
#endif

#ifndef NO_DCC

//-------------------------------------------------------------------

typedef void(*DccAccDecoderPacket)(int address, boolean activate, byte data);

// A dcc id is a long int with accessory id * 10000 + dcc id .
#define DCCID(id)			((id) % 10000)
#define DCCACTIVATION(id)	((byte)((id) / 10000))
#define DCCINT(id, acc)		((acc) * 10000UL + (id))

class DccCommander : Commander
{
	private:
		static unsigned long LastDccId;

	public:
		static boolean UseRawDccAddresses;
		static DccAccDecoderPacket    func_AccPacket;

		inline DccCommander() : Commander() {}
		
		void begin(int i, int j, int k, boolean inUseRawDccAddresses = false);
		void PriorityLoop();
		unsigned long loop();
		static void SetAccessoryDecoderPacketHandler(DccAccDecoderPacket func);
		static void DccAccessoryDecoderPacket(int address, boolean activate, byte data);

		inline unsigned long GetLastDccId() const { return LastDccId; }

#ifdef DEBUG_MODE
public:
	void CheckIndex(unsigned char inIndex, const __FlashStringHelper *infunc);
#endif
};

//-------------------------------------------------------------------
#endif
#endif
//-------------------------------------------------------------------
