/*************************************************************
project: <DCC Accessory Decoder sample>
author: <Thierry PARIS>
description: <10 leds for a french railroad signal>
*************************************************************/

#include <UniversalAccessoryDecoder.h>

#define NB_LEDS_MAX		20
#define PATTERN_NB_LEDS_MAX	4
#define PATTERN_END_LIST	127

//-------------------------------------------------------------------
// This class defines a light signal by giving patterns of lights:
// like the state -x--BB---- for a signal with 10 lights.
// Where x means light on, - means off, and B is blink.
//-------------------------------------------------------------------

class SignalArduinoPattern : public AccessoryGroup
{
protected:
	byte nbLeds;
	unsigned char *accNb;

public:
	SignalArduinoPattern(Accessories *inpAccessories, DriverArduino *inpDriver, byte inNbLeds, const int *inpPins, int inFirstPort = 0);
	void SetupSignal(Accessories *inpAccessories, int inStartingDcc, const byte *inPatterns, const byte *inpRealStates = 0);

	static int GetStatesNumber(const byte *pStates);
};

SignalArduinoPattern::SignalArduinoPattern(Accessories *inpAccessories, DriverArduino *inpDriver, byte inNbLeds, const int *inpPins, int inFirstPort)
{
	this->nbLeds = inNbLeds;
	for (int led = 0; led < inNbLeds; led++)
	{
		inpDriver->SetupPortMotor(inFirstPort + led, inpPins[led]);
	}

	this->accNb = new unsigned char[inNbLeds];

	for (int led = 0; led < inNbLeds; led++)
	{
		this->accNb[led] = inpAccessories->Add(new AccessoryLight(1000 + inFirstPort, led, 500));
	}

	for (int led = 0; led < inNbLeds; led++)
	{
		// Le numéro de la led est aussi le numéro du port.
		LIGHT(*inpAccessories, this->accNb[led])->Setup(inpDriver, inFirstPort + led);
	}
}

void SignalArduinoPattern::SetupSignal(Accessories *inpAccessories, int inStartingDcc, const byte *inPatterns, const byte *inpRealStates)
{
	int nb_etats = this->nbLeds;
	if (inpRealStates != 0)
	{
		nb_etats = SignalArduinoPattern::GetStatesNumber(inpRealStates);
	}

	this->Setup(nb_etats);
	int pospattern = 0;
	for (int etat = 0; etat < nb_etats; etat++)
	{
		GroupState *pState = new GroupState(inStartingDcc, etat);
		pState->Setup(this->nbLeds);

		// Build a list of states, one for each led.
		ACC_STATE states[NB_LEDS_MAX];

		// All leds off
		for (int led = 0; led < this->nbLeds; led++)
		{
			states[led] = LIGHTOFF;
		}

		// Change affected leds in the list
		for (int led = 0; led < PATTERN_NB_LEDS_MAX; led++)
		{
			ACC_STATE state;
			byte c = pgm_read_byte(inPatterns + pospattern++);

			if (c == 0)
				continue;

			if (c > 0 && c <= 100)
				state = LIGHTON;
			else
				if (c > 100)
				{
				state = LIGHTBLINK;
				c -= 100;
				}
			states[c - 1] = state;
		}

		// Then create one state by led.
		for (int led = 0; led < this->nbLeds; led++)
		{
			GroupStateItem *pItem = new GroupStateItem((*inpAccessories)[this->accNb[led]], states[led]);
			pState->Add(pItem);
		}
		this->Add(pState);
	}
}

int SignalArduinoPattern::GetStatesNumber(const byte *pStates)
{
	int count = 0;
	for (int i = 0;; i++)
	{
		byte c = pgm_read_byte(pStates + i);
		if (c == PATTERN_END_LIST)
			return i;
	}

	return 99999;
}

//-------------------------------------------------------------------
// Un feu Français commun a au maximum 9 ampoules disposées comme suit:
//
//		9
//	6	8
//	5	7
//	4
//	3
//	2
//	1
//
// mais il y a plusieurs tailles intermédiaires:
//
//								
//							6	7
//				5			5
//				4			4
//	3			3			3
//	2			2			2
//	1			1			1
//
// voire même circulaire, avec deux ampoules supplémentaires (10 et 11)
//
//	6		7
//		3
//	10		11
//		1
//
// ou horizontal. Le terme horizontal est impropre, parce que ce feu peut être vertical, avec le 1 en haut. Malgré tout son 
// fonctionnement est le même quelque soit son orientation.
//
//	1	2
//
// Une douzième ampoule, dite oeilleton, est placée en bas à gauche du feu, à côté de la led 1.
//
// Le but de cette classe est de décrire correctement tous les types d'états réglementaires
// pour ces feux.
//
//-------------------------------------------------------------------

// La norme SCNF veut qu'il y ait quatre leds allumées au maximum, oeilleton compris.
// La liste décrit, 4 par 4, les numéros des leds allumées : positif allumé,
// supérieur à 100 clignotant, 0 inutilisé. Ce qui signifie que les numéros commencent à 1.
// Le code texte sur chaque ligne "--x---------" symbolise plus clairement les états.
// Enfin les numéros de chaque ligne sont notés de 0 à 15. Ils vont être réutilisés plus tard.

const byte SignalFrStates[] PROGMEM = {
	//  123456789012
	3, 0, 0, 0,	// "--x---------"   0: voie libre
	103, 0, 0, 0,	// "--B---------"   1: voie libre limitée à 160km/h	
	101, 0, 0, 0,	// "B-----------"   2: Avertissement arrêt très proche.	
	1, 0, 0, 0,	// "x-----------"   3: Avertissement arrêt.	
	102, 0, 0, 0,	// "-B----------"   4: Conduite à vue à 15Km/h maxi	
	2, 12, 0, 0,	// "-x---------x"   5: arrêt intégral, franchissable selon conditions	
	2, 5, 0, 0,	// "-x--x-------"   6: carré, arrêt intégral, non franchissable.	
	6, 7, 0, 0,	// "-----xx-----"   7: Vitesse réduite à 30	
	7, 9, 0, 0,	// "------x-x---"   8: Rappel de vitesse réduite à 30	
	1, 7, 9, 12,	// "x-----x-x--x"   9: Rappel de vitesse réduite à 30 + avertissement	
	101, 7, 9, 12,	// "B-----x-x--x"  10: Rappel de vitesse réduite à 30 + avertissement	
	106, 107, 0, 0,	// "-----BB-----"  11: Vitesse réduite à 60	
	101, 106, 107, 12,	// "B----BB----x"  12: Vitesse réduite à 60 + avertissement	
	107, 109, 0, 0,	// "------B-B---"  13: Rappel de vitesse réduite à 60	
	1, 107, 109, 12,	// "x-----B-B--x"  14: Rappel de vitesse réduite à 60 + avertissement	
	101, 107, 109, 12,	// "B-----B-B--x"  15: Rappel de vitesse réduite à 60 + avertissement
	//  123456789012
	PATTERN_END_LIST	// end of the list !
};

// Pour chaque type de feu, il n'y a que certains états de la liste ci-dessus qui sont utilisables.

const byte SignalFr3[] PROGMEM = { 0, 1, 2, 3, 4, PATTERN_END_LIST };
const byte SignalFr5[] PROGMEM = { 0, 1, 2, 3, 4, 5, 6, PATTERN_END_LIST };
const byte SignalFr7[] PROGMEM = { 0, 1, 2, 3, 4, 5, 6, 7, 11, 12, PATTERN_END_LIST };
const byte SignalFr9[] PROGMEM = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, PATTERN_END_LIST };

// Liste des états pour un feu rond.

const byte SignalFrStatesRound[] PROGMEM = {
	//  123456789012
	3, 0, 0, 0,	// "--x---------"	0: voie libre
	103, 0, 0, 0,	// "--B---------"	1: voie libre limitée à 160km/h
	101, 0, 0, 0,	// "B-----------"	2: Avertissement arrêt très proche.
	1, 0, 0, 0,	// "x-----------"	3: Avertissement arrêt.
	10, 11, 0, 0,	// "---------xx-"	4: Conduite à vue (signal rond seulement)
	6, 7, 0, 0,	// "-----xx-----"	5: Vitesse réduite à 30
	106, 107, 0, 0,	// "-----BB-----"	6: Vitesse réduite à 60
	101, 106, 107, 0,	// "B----BB-----"	7: Vitesse réduite à 60 + avertissement
	//  123456789012
	PATTERN_END_LIST	// end of the list !
};

// Tous les états sont utilisables, à priori.

const byte SignalFrRound[] PROGMEM = { 0, 1, 2, 3, 4, 5, 6, 7, PATTERN_END_LIST };

// Four leds on only. First led is 1. Negative led number will blink.

// Liste des états pour un feu horizontal.

const byte SignalFrStatesHorizontal[] PROGMEM = {
	//  123456789012
	1, 0, 0, 0,		// "x-----------" 	0: Vitesse de manoeuvre (feu horizontal)
	101, 0, 0, 0,	// "B-----------" 	1: Vitesse de manoeuvre réduite (feu horizontal)
	2, 0, 0, 0,		// "-x----------" 	2: idem AbsoluteStop mais sur un signal horizontal ou deux feux.
	//  123456789012
	PATTERN_END_LIST	// End of the list
};

// Tous les états sont utilisables, à priori.

const byte SignalFrHorizontal[] PROGMEM = { 0, 1, 2, PATTERN_END_LIST };

/* kDCC_INTERRUPT values :
Board         int.0   int.1   int.2   int.3   int.4   int.5
Uno, Ethernet   2      3
Mega2560        2      3      21      20      19      18
Leonardo        3      2      0       1       7
*/
#define kDCC_INTERRUPT            5

SignalArduinoPattern* signal;

// Accessories

Accessories accessories;
DccCommander dccCommander;
ButtonsCommander buttonsCommander;

// Drivers

DriverArduino *arduino;

#define signalPattern	SignalFr9
#define NB_LEDS			12

int pins[16] = { 13, 12, 11, 10, 9, 8, 7, 6, 22, 23, 24, 25, 26, 27, 28, 29 };

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Setup
//
void setup()
{
	UAD_StartSetup();

	// Le 'true' signifie que l'on va recevoir les codes dcc bruts. A voir si c'est gérable.
	dccCommander.Setup(0x00, 0x00, kDCC_INTERRUPT, true);
	dccCommander.SetStatusLedPin(4);
	// Fonction qui sera appelée lors d'une réception de paquet accessoire.
	// mais en réalité, la gestion des groupes fait ça très bien !
	//dccCommander.SetBasicAccessoryDecoderPacketHandler(AccessoryDecoderPacket_Handler);

	int nb_etats = SignalArduinoPattern::GetStatesNumber(signalPattern);
	buttonsCommander.Setup(1,
		new ButtonsCommanderPush(nb_etats)
		);

	// Ce petit bouton va permettre de passer en revue tous les codes dcc des feux en séquence...
	/*
	for (int etat = 0; etat < nb_etats; etat++)
	{
	PUSH(buttonsCommander, 0)->AddDccId(10, etat);
	}
	*/
	PUSH(buttonsCommander, 0)->AddDccId(10, 10);
	PUSH(buttonsCommander, 0)->AddDccId(10, 14);
	PUSH(buttonsCommander, 0)->AddDccId(10, 15);

	PUSH(buttonsCommander, 0)->Setup(56);           // port A2

	accessories.Setup(NB_LEDS);

	arduino = new DriverArduino(NB_LEDS, 0);
	arduino->Setup();

	signal = new SignalArduinoPattern(&accessories, arduino, NB_LEDS, pins);
	signal->SetupSignal(&accessories, 10, SignalFrStates, signalPattern);

	UAD_EndSetup();
}

void loop()
{
    
	if (dccCommander.Loop())
	{
		accessories.Loop();
		buttonsCommander.Loop();
		signal->Loop();
	}
}

