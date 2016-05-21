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

#ifdef COMMANDERS_DEBUG_MODE
#define CHECK(val, text)	CheckIndex(val, F(text))
#else
#define CHECK(val, text)
#endif

#ifdef VISUALSTUDIO
#include<stdarg.h>
#endif

ButtonsCommanderClass *ButtonsCommanderClass::pButtonsCommander;

ButtonsCommanderClass::ButtonsCommanderClass()
{
	this->pLastSelectedButton = NULL;
	this->pFirstButton = NULL;
}

void ButtonsCommanderClass::begin()
{
#ifdef COMMANDERS_DEBUG_MODE
	Serial.println(F("   ButtonsCommander begin"));
#endif
}

ButtonsCommanderButton *ButtonsCommanderClass::AddButton(ButtonsCommanderButton *inButton)
{
	if (pButtonsCommander == NULL)
		pButtonsCommander = new ButtonsCommanderClass();

	return pButtonsCommander->Add(inButton);
}

ButtonsCommanderButton *ButtonsCommanderClass::Add(ButtonsCommanderButton *inButton)
{
	if (this->pFirstButton == NULL)				
	{
		this->pFirstButton = inButton;
		return inButton;
	}

	ButtonsCommanderButton *pCurr = this->pFirstButton;

	while (pCurr->GetNextButton() != NULL)
		pCurr = pCurr->GetNextButton();

	pCurr->SetNextButton(inButton);

	return inButton;
}

ButtonsCommanderButton* ButtonsCommanderClass::GetFromId(unsigned long inId) const
{
	ButtonsCommanderButton *pCurr = this->pFirstButton;

	while (pCurr != NULL)
	{
		ButtonsCommanderButton *pButton = pCurr->GetFromId(inId);
		if (pButton != NULL)
			return pButton;
		pCurr = pCurr->GetNextButton();
	}

	return 0;
}

void ButtonsCommanderClass::RaiseEvent(unsigned long inId, COMMANDERS_EVENT_TYPE inEvent, int inData)
{
	Commander::RaiseEvent(inId, inEvent, inData);
}

static ButtonsCommanderButton *pCurrentLoopButton = NULL;

unsigned long ButtonsCommanderClass::loop()
{
	Commander::CommanderPriorityLoop();

	if (pCurrentLoopButton != NULL)
		pCurrentLoopButton = pCurrentLoopButton->GetNextButton();

	if (pCurrentLoopButton == NULL)
		pCurrentLoopButton = this->pFirstButton;

	if (pCurrentLoopButton == NULL)
		return UNDEFINED_ID;

	unsigned long ID = pCurrentLoopButton->loop();

#ifdef DEBUG_VERBOSE_MODE
	Serial.print(F("ButtonsCommanderButton id:"));
	Serial.print(ID, DEC);
	Serial.println(F(" checked !"));
#endif

	if (ID == UNDEFINED_ID)
		return UNDEFINED_ID;

#ifdef COMMANDERS_DEBUG_MODE
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
