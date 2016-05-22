/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <Basic button.>
*************************************************************/

#include "Commanders.h"
#ifndef NO_BUTTONSCOMMANDER

ButtonsCommanderButton::ButtonsCommanderButton(unsigned long inId) 
{ 
	this->Id = inId; 
	this->pNextButton = 0; 

	ButtonsCommanderClass::AddButton(this);
}
#endif