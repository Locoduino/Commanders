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

#ifdef DEBUG_MODE
void ButtonsCommander::CheckIndex(byte inIndex, const __FlashStringHelper *inFunc)
{
	if (this->size == 0)
	{
		Serial.print(F("Size undefined in "));
		Serial.println(inFunc);
	}
	else
		if (inIndex < 0 || inIndex >= this->size)
		{
			Serial.print(F("Index error in "));
			Serial.println(inFunc);
		}
}
#endif

ButtonsCommander::ButtonsCommander()
{
	this->size = 0;
	this->pButtons = 0;
	this->addCounter = 0;
	this->pLastSelectedButton = 0;
	this->EventHandler = 0;
}

void ButtonsCommander::Setup(byte inSize)
{
#ifdef DEBUG_MODE
	Serial.println(F("   ButtonsCommander Setup"));
#endif
	this->size = inSize;
	this->addCounter = 0;
	this->pButtons = new ButtonsCommanderButton*[inSize];
}

void ButtonsCommander::Setup(byte inSize, ButtonsCommanderButton *inpFirstState, ...)
{
	Setup(inSize);

	va_list argList;
	this->pButtons[this->addCounter++] = inpFirstState;
	inSize--;

	va_start(argList, inpFirstState);
	for (; inSize; inSize--)
	{
		ButtonsCommanderButton *but;
		but = va_arg(argList, ButtonsCommanderButton *);
		this->pButtons[this->addCounter++] = but;
	}
	va_end(argList);
#ifdef DEBUG_MODE
	if (inSize > 0)
		Serial.println(F("   Not enough items in the list !"));
#endif

}

// Returns the index of the new added accessory.
byte ButtonsCommander::Add(ButtonsCommanderButton *inpButton)
{
	CHECK(addCounter, "ButtonsCommander::Add");
	this->pButtons[addCounter++] = inpButton;

	return addCounter - 1;
}

void ButtonsCommander::Set(byte inIndex, ButtonsCommanderButton *inpButton)
{
	CHECK(inIndex, "ButtonsCommander::Set");
	this->pButtons[inIndex] = inpButton;
}

int ButtonsCommander::IndexOf(ButtonsCommanderButton *inpButton)
{
	for (int i = 0; i < this->addCounter; i++)
		if (this->pButtons[i] == inpButton)
			return i;

	return -1;
}

static int loopIndix = 0;

unsigned long ButtonsCommander::Loop()
{
	Commander::StaticData.CommanderPriorityLoop();

	if (loopIndix >= this->addCounter)
		loopIndix = 0;

	CHECK(loopIndix, "ButtonsCommander::Loop()");

	if (this->pButtons[loopIndix++]->Loop() == UNDEFINED_ID)
		return UNDEFINED_ID;

	bool found = false;
	/*
	if (this->pButtons[--loopIndix]->IsAnalog())
	{
		for (int i = 0; i < Accessories::AccessoriesFullList.AccessoriesAddCounter; i++)
			found |= Accessories::AccessoriesFullList.pAccessoriesFullList[i]->MovePosition(this->pButtons[loopIndix]->GetDccIdDecoder(), this->pButtons[loopIndix]->GetDccIdAccessory(), this->pButtons[loopIndix]->GetPosition());
	}
	else
	{
		for (int i = 0; i < Accessories::AccessoriesFullList.AccessoriesAddCounter; i++)
			found |= Accessories::AccessoriesFullList.pAccessoriesFullList[i]->DCCToggle(this->pButtons[loopIndix]->GetDccIdDecoder(), this->pButtons[loopIndix]->GetDccIdAccessory());
		for (int j = 0; j < AccessoryGroup::StaticData.AccessoryGroupAddCounter; j++)
			found |= AccessoryGroup::StaticData.pAccessoryGroupFullList[j]->DCCToggle(this->pButtons[loopIndix]->GetDccIdDecoder(), this->pButtons[loopIndix]->GetDccIdAccessory());
	}
	*/
	this->pButtons[loopIndix]->EndLoop();

	this->pLastSelectedButton = this->pButtons[loopIndix];

	return this->pButtons[loopIndix++]->GetId();
}

#endif
