//-------------------------------------------------------------------
#ifndef __canCommander_H__
#define __canCommander_H__
//-------------------------------------------------------------------

#include "Commanders.h"

#ifndef _MCP2515_H_
#error To be able to compile this commander, the 'mcp_can' library must be installed. See 'extras' directory of Commanders library.
#endif

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

		// Variables globales pour la gestion des Messages reçus et émis
		uint8_t IdR;                       // Id pour la routine CAN_recup()
		unsigned char lenR = 0;         // Longueur "    "       "
		unsigned char bufR[8];          // tampon de reception      "
		unsigned char bufS[8];          // tampon d'emission

										// Variable globale Mémoire circulaire pour le stockage des messages reçus
		unsigned char Circule[256];     // récepteur circulaire des messages CAN sous IT
		int indexW, indexR, Ncan;       // index d'écriture et lecture, nb d'octets a lire
		uint8_t CANoverflow = 0;           // flag overflow (buffer _Circule plein)

	public:
		inline CANCommanderClass() : Commander() {}
		
		void begin(uint8_t inPin, uint8_t inSpeed, uint8_t inInterrupt, uint16_t inId);
		unsigned long loop();
		void CAN_recup();

	public:
		static CANCommanderClass *pCANCommander;
		static inline CANCommanderClass &GetCurrent()
		{
			if (pCANCommander == NULL)
				pCANCommander = new CANCommanderClass();

			return *(CANCommanderClass::pCANCommander);
		}
#ifdef COMMANDERS_PRINT_COMMANDERS
		uint8_t SPIpin;
		uint8_t Speed;
		uint8_t Interrupt;
		uint16_t Id;

		void printCommander();
#endif
};

//-------------------------------------------------------------------
#endif
#endif
//-------------------------------------------------------------------
