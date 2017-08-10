/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <Buttons Commander>
*************************************************************/

#include <Commanders.h>
#ifdef NO_BUTTONSCOMMANDER
#pragma message ("Commanders : No Buttons commander !")
#else
#ifdef VISUALSTUDIO
#include <stdarg.h>
#endif

#ifdef COMMANDERS_DEBUG_MODE
#define CHECK(val, text)	CheckIndex(val, F(text))
#else
#define CHECK(val, text)
#endif

ButtonsCommanderClass *ButtonsCommanderClass::pButtonsCommander;

ButtonsCommanderClass::ButtonsCommanderClass()
{
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

static ButtonsCommanderButton *pCurrentLoopButton = NULL;

void ButtonsCommanderClass::beforeFirstLoop()
{
	ButtonsCommanderButton *pCurr = this->pFirstButton;

	while (pCurr != NULL)
	{
		pCurr->beforeFirstLoop();
		pCurr = pCurr->GetNextButton();
	}
}

unsigned long ButtonsCommanderClass::loop()
{
	if (pCurrentLoopButton != NULL)
		pCurrentLoopButton = pCurrentLoopButton->GetNextButton();

	if (pCurrentLoopButton == NULL)
		pCurrentLoopButton = this->pFirstButton;

	if (pCurrentLoopButton == NULL)
		return UNDEFINED_ID;

	unsigned long ID = pCurrentLoopButton->loop();

	if (ID == UNDEFINED_ID)
		return UNDEFINED_ID;

#ifdef COMMANDERS_DEBUG_MODE
	Serial.print(F("ButtonsCommanderButton id:"));
	Serial.print(ID, DEC);
	Serial.println(F(" selected !"));
#endif

	pCurrentLoopButton->EndLoop();
	return ID;
}

#ifdef COMMANDERS_PRINT_COMMANDERS
void ButtonsCommanderClass::printCommander()
{
	Serial.println(F("Commander: ButtonsCommander "));

	ButtonsCommanderButton *pCurr = this->pFirstButton;

	while (pCurr != NULL)
	{
		pCurr->printCommander();
		pCurr = pCurr->GetNextButton();
	}
}
#endif

#endif
