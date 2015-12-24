/*************************************************************
project: <Universal Accessory Decoder>
author: <Thierry PARIS>
description: <Buttons Commander>
*************************************************************/

#ifndef NO_BUTTONSCOMMANDER
#include "ButtonsCommander.hpp"

#ifdef VISUALC
#include<stdarg.h>
#endif

#ifdef DEBUG_MODE
#define CHECK(val, text)	CheckIndex(val, F(text))
#else
#define CHECK(val, text)
#endif

#ifdef VISUALC
#include<stdarg.h>
#endif

ButtonsCommander::ButtonsCommander()
{
	this->pLastSelectedButton = 0;
}

void ButtonsCommander::Setup()
{
#ifdef DEBUG_MODE
	Serial.println(F("   ButtonsCommander Setup"));
#endif
}

ButtonsCommanderButton *ButtonsCommander::Add(ButtonsCommanderButton *inButton)
{
	if (ButtonsCommander::pFirstButton == 0)
	{
		ButtonsCommander::pFirstButton = inButton;
		return inButton;
	}

	ButtonsCommanderButton *pCurr = ButtonsCommander::pFirstButton;

	while (pCurr->GetNextButton() != 0)
		pCurr = pCurr->GetNextButton();

	pCurr->SetNextButton(inButton);

	return inButton;
}

static ButtonsCommanderButton *pCurrentLoopButton = 0;

unsigned long ButtonsCommander::Loop()
{
	Commander::CommanderPriorityLoop();

	if (pCurrentLoopButton != 0)
		pCurrentLoopButton = pCurrentLoopButton->GetNextButton();

	if (pCurrentLoopButton == 0)
		pCurrentLoopButton = this->pFirstButton;

	if (pCurrentLoopButton == 0)
		return UNDEFINED_ID;

	if (pCurrentLoopButton->Loop() == UNDEFINED_ID)
		return UNDEFINED_ID;

	pCurrentLoopButton->EndLoop();

	this->pLastSelectedButton = pCurrentLoopButton;

	return pCurrentLoopButton->GetId();
}

#endif
