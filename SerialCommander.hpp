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

#ifdef DEBUG_MODE
#endif
};

//-------------------------------------------------------------------

//===================================================================
// -> DO NOT WRITE ANYTHING BETWEEN HERE...
// 		This section is reserved for automated code generation
// 		This process tries to detect all user-created
// 		functions in main_sketch.cpp, and inject their  
// 		declarations into this file.
// 		If you do not want to use this automated process,  
//		simply delete the lines below, with "&MM_DECLA" text 
//===================================================================
//---- DO NOT DELETE THIS LINE -- @MM_DECLA_BEG@---------------------
//---- DO NOT DELETE THIS LINE -- @MM_DECLA_END@---------------------
// -> ...AND HERE. This space is reserved for automated code generation!
//===================================================================


//-------------------------------------------------------------------
#endif
#endif
//-------------------------------------------------------------------
