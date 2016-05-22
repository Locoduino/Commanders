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

		// Variables globales pour la gestion des Messages reçus et émis
		byte IdR;                       // Id pour la routine CAN_recup()
		unsigned char lenR = 0;         // Longueur "    "       "
		unsigned char bufR[8];          // tampon de reception      "
		unsigned char bufS[8];          // tampon d'emission

										// Variable globale Mémoire circulaire pour le stockage des messages reçus
		unsigned char Circule[256];     // récepteur circulaire des messages CAN sous IT
		int indexW, indexR, Ncan;       // index d'écriture et lecture, nb d'octets a lire
		byte CANoverflow = 0;           // flag overflow (buffer _Circule plein)

	public:
		inline CANCommanderClass() : Commander() {}
		
		void begin(byte inPin, byte inSpeed, byte inInterrupt, uint16_t inId);
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
};

//-------------------------------------------------------------------
#endif
#endif
//-------------------------------------------------------------------
