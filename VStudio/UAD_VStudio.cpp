// UAD.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "math.h"

void setup();
void loop();

int _tmain(int argc, _TCHAR* argv[])
{
	setup();

	while (true)
		loop();

	return 0;
}

//-----------------------------------------
//			ino PART
//-----------------------------------------
/*************************************************************
project: <DCC Accessory Decoder>
author: <Thierry PARIS>
description: <Triple T-Trak module>
*************************************************************/

#include "UniversalAccessoryDecoder.h"

/* kDCC_INTERRUPT values :
Board			int.0	int.1	int.2	int.3	int.4	int.5
Uno, Ethernet	2		3
Mega2560		2		3		21		20		19		18
Leonardo		3		2		0		1		7
*/
#define kDCC_INTERRUPT            5

// total number of pushbuttons / accessories.
#define AccessoryNumber		5

#define TURNOUT_LEFT		0
#define TURNOUT_DC			1
#define TURNOUT_RIGHT		2
#define TURNOUT_EPS			3
#define TURNOUT_TJD			4

// Accessories

Accessories accessories;

// Drivers

DriverL293d *l293d;
DriverL298n *l298n;

ButtonsCommander buttons;
DccCommander dcc;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Setup
//
void setup()
{
	UAD_StartSetup();

	// Drivers setups

	l293d = new DriverL293d();
	l293d->Setup();
	l293d->SetupPortMotor(L293D_PORT_M1, MOTOR12_1KHZ);	//TURNOUT_RIGHT
	l293d->SetupPortMotor(L293D_PORT_M2, MOTOR12_1KHZ);	//TURNOUT_LEFT
	l293d->SetupPortMotor(L293D_PORT_M3, MOTOR34_1KHZ);	//TURNOUT_TJD
	l293d->SetupPortMotor(L293D_PORT_M4, MOTOR34_1KHZ);	//TURNOUT_EPS

	l298n = new DriverL298n();
	l298n->Setup();
	l298n->SetupPortMotor(L298N_PORT_OUT1, 50, 52);		//TURNOUT_DC
	l298n->SetupPortMotor(L298N_PORT_OUT2, 46, 48);		//NOTHING

	// Buttons setups

	buttons.Setup(5);
	buttons.Add(new ButtonsCommanderPush(DCCINT(20, 0))); // TURNOUT_LEFT	
	buttons.Add(new ButtonsCommanderPush(DCCINT(20, 1))); // TURNOUT_DC
	buttons.Add(new ButtonsCommanderPush(DCCINT(21, 0))); // TURNOUT_RIGHT
	buttons.Add(new ButtonsCommanderPush(DCCINT(21, 1))); // TURNOUT_EPS
	buttons.Add(new ButtonsCommanderPush(DCCINT(22, 0))); // TURNOUT_TJD

	PUSH(buttons, TURNOUT_LEFT)->Setup(30);
	PUSH(buttons, TURNOUT_DC)->Setup(34);
	PUSH(buttons, TURNOUT_RIGHT)->Setup(32);
	PUSH(buttons, TURNOUT_EPS)->Setup(36);
	PUSH(buttons, TURNOUT_TJD)->Setup(38);

	// Accessories setups

	accessories.Setup(AccessoryNumber);
	accessories.Add(new AccessoryMotorTwoWays(20, 0, 50));	// TURNOUT_LEFT	
	accessories.Add(new AccessoryMotorTwoWays(20, 1, 50));	// TURNOUT_DC
	accessories.Add(new AccessoryMotorTwoWays(21, 0, 300));	// TURNOUT_RIGHT
	accessories.Add(new AccessoryMotorTwoWays(21, 1, 300));	// TURNOUT_EPS
	accessories.Add(new AccessoryMotorTwoWays(22, 0, 300));	// TURNOUT_TJD

	MOTOR2WAYS(accessories, TURNOUT_LEFT)->Setup(l293d, L293D_PORT_M1, 150);
	MOTOR2WAYS(accessories, TURNOUT_RIGHT)->Setup(l293d, L293D_PORT_M2, 150);
	MOTOR2WAYS(accessories, TURNOUT_EPS)->Setup(l293d, L293D_PORT_M3, 150);
	MOTOR2WAYS(accessories, TURNOUT_TJD)->Setup(l293d, L293D_PORT_M4, 150);
	MOTOR2WAYS(accessories, TURNOUT_DC)->Setup(l298n, L298N_PORT_OUT2, 150);

	dcc.Setup(0x00, 0x00, kDCC_INTERRUPT);
	dcc.SetStatusLedPin(13);

	UAD_EndSetup();
}

void loop()
{
	UAD_Loop();
}

//-----------------------------------------
//			end ino PART
//-----------------------------------------
