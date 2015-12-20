/*************************************************************
project: <Basics Commanders>
author: <Thierry PARIS>
description: <Demo sample>
*************************************************************/

#include "BasicsCommanders.h"

/* kDCC_INTERRUPT values :
Board			int.0	int.1	int.2	int.3	int.4	int.5
Uno, Ethernet	2		3
Mega2560		2		3		21		20		19		18
Leonardo		3		2		0		1		7
*/
#define kDCC_INTERRUPT            5

#define TURNOUT_LEFT		0
#define TURNOUT_DC			1
#define TURNOUT_RIGHT		2
#define TURNOUT_EPS			3
#define TURNOUT_TJD			4

ButtonsCommander buttons;
DccCommander dcc;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Setup
//
void setup()
{
	BasicsCommanders_StartSetup();

	// Buttons setups

	buttons.Setup(5);
	buttons.Add(new ButtonsCommanderPush(20UL)); // TURNOUT_LEFT	
	buttons.Add(new ButtonsCommanderPush(21UL)); // TURNOUT_DC
	buttons.Add(new ButtonsCommanderPush(22UL)); // TURNOUT_RIGHT
	buttons.Add(new ButtonsCommanderPush(23UL)); // TURNOUT_EPS
	buttons.Add(new ButtonsCommanderPush(24UL)); // TURNOUT_TJD

	PUSH(buttons, TURNOUT_LEFT)->Setup(30);
	PUSH(buttons, TURNOUT_DC)->Setup(34);
	PUSH(buttons, TURNOUT_RIGHT)->Setup(32);
	PUSH(buttons, TURNOUT_EPS)->Setup(36);
	PUSH(buttons, TURNOUT_TJD)->Setup(38);

	// Accessories setups

	dcc.Setup(0x00, 0x00, kDCC_INTERRUPT);
	dcc.SetStatusLedPin(13);

	BasicsCommanders_EndSetup();
}

void loop()
{
	BasicsCommanders_Loop();
}
