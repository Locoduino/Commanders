/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <Basic button.>
*************************************************************/

#include "Commanders.h"

COMMANDERS_EVENT_TYPE ButtonsCommanderButton::eventType;
int ButtonsCommanderButton::eventData;

ButtonsCommanderButton::ButtonsCommanderButton(unsigned long inId) 
{ 
	this->Id = inId; 
	this->pNextButton = 0; 

	ButtonsCommanderClass::AddButton(this);
}
