//-------------------------------------------------------------------
#ifndef __dccCommander_H__
#define __dccCommander_H__
//-------------------------------------------------------------------

#include "Commanders.h"

#ifndef NO_DCCCOMMANDER

#ifdef VISUALSTUDIO
#include "../VStudio/Dcc_Decoder.hpp"
#else
#include "DCC_Decoder.hpp"
#endif

#define DccCommander DccCommanderClass::GetCurrent()

//-------------------------------------------------------------------

typedef void(*DccAccDecoderPacket)(int address, boolean activate, uint8_t data);

// A dcc id is a long int with accessory id * 10000 + dcc id .
#define DCCID(id)			((id) % 10000)
#define DCCACTIVATION(id)	((uint8_t)((id) / 10000))
#define DCCINT(id, acc)		((acc) * 10000UL + (id))

class DccCommanderClass : Commander
{
	private:
		unsigned long LastDccId;

	public:
		boolean UseRawDccAddresses;
		static DccAccDecoderPacket    func_AccPacket;

		inline DccCommanderClass() : Commander() {}
		
		void begin(int i, int j, int k, boolean inInterruptMonitor = false, boolean inUseRawDccAddresses = false);
		void PriorityLoop();
		unsigned long loop();
		static void SetAccessoryDecoderPacketHandler(DccAccDecoderPacket func);
		static void DccAccessoryDecoderPacket(int address, boolean activate, uint8_t data);

		inline unsigned long GetLastDccId() { return LastDccId; }

	public:
		static DccCommanderClass *pDccCommander;
		static inline DccCommanderClass &GetCurrent() 
		{ 
			if (pDccCommander == NULL)
				pDccCommander = new DccCommanderClass();

			return *(DccCommanderClass::pDccCommander); 
		}

#ifdef COMMANDERS_DEBUG_MODE
public:
	void CheckIndex(unsigned char inIndex, const __FlashStringHelper *infunc);
	static void printEvent(unsigned long inId, COMMANDERS_EVENT_TYPE inEventType, int inEventData);
#endif
#ifdef COMMANDERS_PRINT_COMMANDERS
	int8_t Interrupt;
	void printCommander();
#endif
};

//-------------------------------------------------------------------
#endif
#endif
//-------------------------------------------------------------------
