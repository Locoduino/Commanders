/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <Commanders DCC sample by macros>
*************************************************************/

#include "Commanders.h"

/* INTERRUPT values as macro argument :
Board			int.0	int.1	int.2	int.3	int.4	int.5
Uno, Ethernet	2		3
Mega2560		2		3		21		20		19		18
Leonardo		3		2		0		1		7
*/
DECLARE_DCC_COMMANDER(3);  // interrupt number

void CommandersEventHandler(unsigned long inId, COMMANDERS_EVENT_TYPE inEvent, int inData)
{
	DccCommander::printEvent(inId, inEvent, inData);
}

void setup()
{
	START_DCC_COMMANDER_SETUPEVENT(CommandersEventHandler);

	END_COMMANDERS_SETUP;
}

void loop()
{
	COMMANDERS_LOOP;
}
