/*************************************************************
project: <DCC Accessory Decoder>
author: <Thierry PARIS>
description: <Sample for 3 leds signals>
*************************************************************/
#include "UniversalAccessoryDecoder.h"

//------------------------------------------------------------------------------
// SignalArduino declaration

class SignalArduino : public AccessoryGroup
{
protected:
	byte nbLeds;
	unsigned char *accNb;
	
public:
	SignalArduino(Accessories *inpAccessories, DriverArduino *inpDriver, byte inNbLeds, int *inpPins, int inFirstPort = 0);
	virtual void SetupSignal(Accessories *inpAccessories, int inStartingDcc);
};

//------------------------------------------------------------------------------
// SignalArduino definition

SignalArduino::SignalArduino(Accessories *inpAccessories, DriverArduino *inpDriver, byte inNbLeds, int *inpPins, int inFirstPort)
{
	this->nbLeds = inNbLeds;
	for (int led = 0; led < inNbLeds; led++)
	{
		inpDriver->SetupPortMotor(inFirstPort + led, inpPins[led], DIGITAL_INVERTED);
	}

	this->accNb = new unsigned char[inNbLeds];

	for (int led = 0; led < inNbLeds; led++)
	{
		this->accNb[led] = inpAccessories->Add(new AccessoryLight(1000 + inFirstPort + led, 0));
	}

	for (int led = 0; led < inNbLeds; led++)
	{
		// Led number is also port number...
		LIGHT(*inpAccessories, this->accNb[led])->Setup(inpDriver, inFirstPort + led);
	}
}

void SignalArduino::SetupSignal(Accessories *inpAccessories, int inStartingDcc)
{
	int led_on = 0;

	this->Setup((int)this->nbLeds + 1);

	// Used dcc codes are
	//                 Led  0    1    2
	// inStartingDcc   / 0  on  off  off
	// inStartingDcc   / 1  off on   off
	// inStartingDcc+1 / 0  off off  on
	// inStartingDcc+1 / 1  off off  off

	for (int etat = 0; etat < this->nbLeds; etat++)
	{
		int dccDecoder;
		byte dccState;

		switch(etat)
		{
		case 0:
			dccDecoder = inStartingDcc;
			dccState = 0;
			break;
		case 1:
			dccDecoder = inStartingDcc;
			dccState = 1;
			break;
		case 2:
			dccDecoder = inStartingDcc + 1;
			dccState = 0;
			break;
		}

		// Each state describes 2 leds off and one on.
		GroupState *pState = new GroupState(dccDecoder, dccState);
		pState->Setup(this->nbLeds);

		for (int led = 0; led < this->nbLeds; led++)
		{
			GroupStateItem *pItem = new GroupStateItem((*inpAccessories)[this->accNb[led]], led_on == led ? LIGHTON : LIGHTOFF);
			pState->Add(pItem);
		}
		this->Add(pState);
		led_on++;
	}

	// The last state is only to set off all leds.
	GroupState *pState = new GroupState(inStartingDcc + 1, 1);
	pState->Setup(this->nbLeds);

	for (int led = 0; led < this->nbLeds; led++)
	{
		GroupStateItem *pItem = new GroupStateItem((*inpAccessories)[this->accNb[led]], LIGHTOFF);
		pState->Add(pItem);
	}
	this->Add(pState);

    // Light on at startup
	for (int led = 0; led < this->nbLeds; led++)
	{
		((AccessoryLight *)(*inpAccessories)[this->accNb[led]])->LightOn();
	}

    delay(1000);
	for (int led = 0; led < this->nbLeds; led++)
	{
		((AccessoryLight *)(*inpAccessories)[this->accNb[led]])->LightOff();
	}
    delay(1000);
    this->DCCToggle(inStartingDcc, 0);
}

//------------------------------------------------------------------------------
// Classic INO area

/* kDCC_INTERRUPT values :
Board         int.0   int.1   int.2   int.3   int.4   int.5
Uno, Ethernet   2      3
Mega2560        2      3      21      20      19      18
Leonardo        3      2      0       1       7
*/
#define kDCC_INTERRUPT            5

#define NB_LEDS		3
#define NB_ETATS	3
#define NB_FEUX		5

SignalArduino* signaux[NB_FEUX];

// Accessories

Accessories accessories;
DccCommander dccCommander;
ButtonsCommander buttonsCommander;

// Drivers

DriverArduino *arduino;

int pins[NB_FEUX][NB_ETATS] = {
		{ 29, 28, 27 },
		{ 26, 25, 24 },
		{ 23, 22, 6 },
		{ 7, 8, 9 },
		{ 10, 11, 12 }
};

int dcc_codes[NB_FEUX] = { 10, 12, 14, 16, 18 };

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Setup
//
void setup()
{
	UAD_StartSetup();

	dccCommander.Setup(0x00, 0x00, kDCC_INTERRUPT);
	dccCommander.SetStatusLedPin(18);

	// Small push button to check all signal states manually.
	buttonsCommander.Setup(1,
		new ButtonsCommanderPush(NB_FEUX * NB_ETATS)
		);

	for (int feu = 0; feu < NB_FEUX; feu++)
	{
		for (int etat = 0; etat < NB_ETATS; etat++)
		{
			PUSH(buttonsCommander, 0)->AddDccId(dcc_codes[feu], etat);
		}
	}

	PUSH(buttonsCommander, 0)->Setup(56);           // port A2

	// NB_LEDS * NB_FEUX ports to initialize
	arduino = new DriverArduino(NB_LEDS * NB_FEUX, 0);
	arduino->Setup();

	accessories.Setup(NB_LEDS * NB_FEUX);

	for (int feu = 0; feu < NB_FEUX; feu++)
	{
		signaux[feu] = new SignalArduino(&accessories, arduino, NB_LEDS, pins[feu], feu * NB_LEDS);
		signaux[feu]->SetupSignal(&accessories, dcc_codes[feu]);
	}

	UAD_EndSetup();
}

void loop()
{
	if (dccCommander.Loop())
	{
		accessories.Loop();
		buttonsCommander.Loop();
		for (int feu = 0; feu < NB_FEUX; feu++)
			signaux[feu]->Loop();
	}
}
