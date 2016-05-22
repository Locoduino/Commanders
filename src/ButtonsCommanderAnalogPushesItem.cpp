/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <Item button from analog array of push buttons with debounce.>
*************************************************************/

#include "Commanders.h"
#ifndef NO_BUTTONSCOMMANDER

bool ButtonsCommanderAnalogPushesItem::IsPushed(int inCurrentValue)
{
	if (inCurrentValue >= this->analogValue - this->readingTolerancy && inCurrentValue <= this->analogValue + this->readingTolerancy)
		return true;

	return false;
}

#endif