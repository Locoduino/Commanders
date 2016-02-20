/*************************************************************
project: <Universal Accessory Decoder>
author: <Thierry PARIS>
description: <Buttons Commander>
*************************************************************/

#ifndef NO_BUTTONSCOMMANDER
#include "BasicsCommanders.h"

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

BasicsCommanderEvent ButtonsCommander::Loop()
{
	Commander::CommanderPriorityLoop();

	if (pCurrentLoopButton != 0)
		pCurrentLoopButton = pCurrentLoopButton->GetNextButton();

	if (pCurrentLoopButton == 0)
		pCurrentLoopButton = this->pFirstButton;

	if (pCurrentLoopButton == 0)
		return EmptyEvent;

	BasicsCommanderEvent event = pCurrentLoopButton->Loop();

#ifdef DEBUG_VERBOSE_MODE
	Serial.print(F("ButtonsCommanderButton id:"));
	Serial.print(pCurrentLoopButton->GetId(), DEC);
	Serial.println(F(" checked !"));
#endif

	if (event.ID == UNDEFINED_ID)
		return event;

#ifdef DEBUG_MODE
	Serial.print(F("ButtonsCommanderButton id:"));
	Serial.print(event.ID, DEC);
	Serial.println(F(" selected !"));
#endif

	pCurrentLoopButton->EndLoop();

	this->pLastSelectedButton = this->GetFromId(event.ID);

	return event;
}

#endif
