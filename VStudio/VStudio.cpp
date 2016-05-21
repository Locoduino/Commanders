// UAD.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "math.h"
#include "windows.h"
#include "ButtonsCommanderKeyboard.hpp"

void setup();
void loop();

int _tmain(int argc, _TCHAR* argv[])
{
	setup();

	DWORD        mode;
	HANDLE       hstdin;
	INPUT_RECORD inrec;
	DWORD        count;

	/* Set the console mode to no-echo, raw input, */
	/* and no window or mouse events.              */
	hstdin = GetStdHandle(STD_INPUT_HANDLE);
	if (hstdin == INVALID_HANDLE_VALUE
		|| !GetConsoleMode(hstdin, &mode)
		|| !SetConsoleMode(hstdin, 0))
		return 0;

	while (true)
	{
		FlushConsoleInputBuffer(hstdin);

		/* Get a single key RELEASE */
		ReadConsoleInput(hstdin, &inrec, 1, &count);

		if (inrec.EventType == KEY_EVENT && inrec.Event.KeyEvent.bKeyDown)
			ButtonsCommanderKeyboard::lastLoopKey = inrec.Event.KeyEvent.wVirtualKeyCode;

		loop();
	}

	/* Restore the original console mode */
	SetConsoleMode(hstdin, mode);

	return 0;
}

//-----------------------------------------
//			ino PART
//-----------------------------------------
/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <I2C commander demo>
*************************************************************/

#include "Commanders.h"


//////////////////////////////////////////////////
// Push buttons declarations
ButtonsCommanderPush push_simple;
ButtonsCommanderPush push_special_event;
ButtonsCommanderPush push_event_list;

//////////////////////////////////////////////////
// Analog push buttons
ButtonsCommanderAnalogPushes pushes;

#define NUMBER_OF_ANALOG_PUSHES	5

int pushes_values[NUMBER_OF_ANALOG_PUSHES] = { 0, 145, 329, 505, 741 };			// Analog values for voltages
unsigned long pushes_ids[NUMBER_OF_ANALOG_PUSHES] = { 300, 301, 302, 303, 304 };	// Id for each button

//////////////////////////////////////////////////
// switches
ButtonsCommanderSwitch switch_simple;
ButtonsCommanderSwitch switch_motor;
ButtonsCommanderSwitch switch_light;
ButtonsCommanderSwitch switch_rotator;

//////////////////////////////////////////////////
// Encoders
ButtonsCommanderEncoder encoder_absolute;
ButtonsCommanderEncoder encoder_relative;

//////////////////////////////////////////////////
// Potentiometers
ButtonsCommanderPotentiometer potentiometer;

//SERIAL_COMMANDER(Serial);

void CommandersEventHandler(unsigned long inId, COMMANDERS_EVENT_TYPE inEvent, int inData)
{
	Commanders::printEvent(inId, inEvent, inData);
}

void setup()
{
	Commanders::SetStatusLedPin(LED_BUILTIN);
	Commanders::SetEventHandler(CommandersEventHandler);

	//ButtonsCommander.begin();
//	I2CCommander.begin(01);
//	DccCommander.begin(0, 0, 1);
//	CANCommander.begin(0, 0, 1, 1);
//	SerialCommander.begin(115200);

	////////////////////////////////////////////////////////////////////////////////////
	// Pushes
	// Simple button for toggle event, with Id 100 and pin 10
	push_simple.begin(100, 10);

	// Simple button for moving event at pos 90, with Id 101, on pin 11
	push_special_event.begin(101, 11, COMMANDERS_EVENT_ABSOLUTEMOVE, 90);

	// Complex push button which iterates through a list of three events : id 200 TOGGLE, id 201 MOVESTOP and id 202 MOVE to 135..
	push_event_list.AddEvent(201, COMMANDERS_EVENT_MOVESTOP);
	push_event_list.AddEvent(202, COMMANDERS_EVENT_ABSOLUTEMOVE, 135);
	push_event_list.begin(200, 12);	// begin only when all events are pushed in the class !

	////////////////////////////////////////////////////////////////////////////////////
	// Analog pushes
	// Five buttons connected through resistors to the pin A0:
	pushes.begin(A0, NUMBER_OF_ANALOG_PUSHES, pushes_ids, pushes_values, 20); // 20 : optional tolerancy for value recognition.

	////////////////////////////////////////////////////////////////////////////////////
	// Switches
	// Simple switch acting as a push, on pin 13 and with Id 401 !
	switch_simple.AddEvent(13, 401);
	switch_simple.begin();

	// Switch button switching a motor between two positions, with pins 14 and 15, and Ids 402 & 403
	switch_motor.AddEvent(14, 402, COMMANDERS_EVENT_MOVELEFT);
	switch_motor.AddEvent(15, 403, COMMANDERS_EVENT_MOVERIGHT);
	switch_motor.begin();

	// Switch button switching a light on and off, with pins 16 and 17, and Ids 404 and 405
	switch_light.AddEvent(16, 404, COMMANDERS_EVENT_MOVELEFT);	// light on
	switch_light.AddEvent(17, 405, COMMANDERS_EVENT_MOVESTOP);	// light off
	switch_light.begin();

	// Rotator with four positions, commanding a servo, using pins 18 to 21 and Ids from 410 to 413
	switch_rotator.AddEvent(18, 410, COMMANDERS_EVENT_ABSOLUTEMOVE, 15);
	switch_rotator.AddEvent(19, 411, COMMANDERS_EVENT_ABSOLUTEMOVE, 45);
	switch_rotator.AddEvent(20, 412, COMMANDERS_EVENT_ABSOLUTEMOVE, 100);
	switch_rotator.AddEvent(21, 413, COMMANDERS_EVENT_ABSOLUTEMOVE, 135);
	switch_rotator.begin();

	////////////////////////////////////////////////////////////////////////////////////
	// Encoders
	// This encoder will always returns a value between -100 and +100. The starting position is 0.
	// Connected to pins 22 and 23, with Id 500.
	encoder_absolute.begin(22, 23, 500, 0, -100, +100);

	// This encoder will always returns -1 or +1
	// Connected to pins 24 and 25, with Id 501.
	encoder_relative.begin(24, 25, 501);

	////////////////////////////////////////////////////////////////////////////////////
	// Potentiometer
	// This potentiometer will always returns a value between -100 and +100, using analog pin A1, and Id 600.
	potentiometer.begin(A1, 600, -100, +100, 2); // 2 : optional minimum movement in the final unit.

	Commanders::EndSetup();
}

void loop()
{
	Commanders::loop();
}

//-----------------------------------------
//			end ino PART
//-----------------------------------------
