/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <Buttons Commander>
*************************************************************/

#ifndef NO_BUTTONSCOMMANDER
#include "Commanders.h"

#ifdef VISUALSTUDIO
#include<stdarg.h>
#endif

#ifdef DEBUG_MODE
#define CHECK(val, text)	CheckIndex(val, F(text))
#else
#define CHECK(val, text)
#endif

#ifdef VISUALSTUDIO
#include<stdarg.h>
#endif

ButtonsCommander::ButtonsCommander()
{
	this->pLastSelectedButton = 0;
}

void ButtonsCommander::begin()
{
#ifdef DEBUG_MODE
	Serial.println(F("   ButtonsCommander begin"));
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

ButtonsCommanderButton* ButtonsCommander::GetFromId(unsigned long inId) const
{
	ButtonsCommanderButton *pCurr = ButtonsCommander::pFirstButton;

	while (pCurr != 0)
	{
		ButtonsCommanderButton *pButton = pCurr->GetFromId(inId);
		if (pButton != 0)
			return pButton;
		pCurr = pCurr->GetNextButton();
	}

	return 0;
}

void ButtonsCommander::RaiseEvent(unsigned long inId, COMMANDERS_EVENT_TYPE inEvent, int inData)
{
	Commander::RaiseEvent(inId, inEvent, inData);
}

static ButtonsCommanderButton *pCurrentLoopButton = 0;

unsigned long ButtonsCommander::loop()
{
	Commander::CommanderPriorityLoop();

	if (pCurrentLoopButton != 0)
		pCurrentLoopButton = pCurrentLoopButton->GetNextButton();

	if (pCurrentLoopButton == 0)
		pCurrentLoopButton = this->pFirstButton;

	if (pCurrentLoopButton == 0)
		return UNDEFINED_ID;

	unsigned long ID = pCurrentLoopButton->loop();

#ifdef DEBUG_VERBOSE_MODE
	Serial.print(F("ButtonsCommanderButton id:"));
	Serial.print(ID, DEC);
	Serial.println(F(" checked !"));
#endif

	if (ID == UNDEFINED_ID)
		return UNDEFINED_ID;

#ifdef DEBUG_MODE
	Serial.print(F("ButtonsCommanderButton id:"));
	Serial.print(ID, DEC);
	Serial.println(F(" selected !"));
#endif

	pCurrentLoopButton->EndLoop();

	this->pLastSelectedButton = this->GetFromId(ID);

	Commanders::SetLastEventType(ButtonsCommanderButton::GetLastEventType());
	Commanders::SetLastEventData(ButtonsCommanderButton::GetLastEventData());

	return ID;
}

#endif
