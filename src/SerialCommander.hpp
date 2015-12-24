//-------------------------------------------------------------------
#ifndef __serialCommander_H__
#define __serialCommander_H__
//-------------------------------------------------------------------

#ifndef NO_SERIALCOMMANDER
#include "Commander.hpp"

#ifdef VISUALC
#include "VStudio/Serial.hpp"
#endif

//-------------------------------------------------------------------

class SerialCommander : Commander
{
	private:
		byte serialPort;
		int addCounter;
	
	public:
		inline SerialCommander() : Commander() { this->addCounter = 0; }
		
	public:
		void Setup(unsigned long inSpeed, byte inSerialPort);
		unsigned long Loop();
};

//-------------------------------------------------------------------
#endif
#endif
//-------------------------------------------------------------------
