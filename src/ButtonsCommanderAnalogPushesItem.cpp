/*************************************************************
project: <Commanders>
author: <Thierry PARIS>
description: <Item button from analog array of push buttons with debounce.>
*************************************************************/

#include <Commanders.h>
#ifndef NO_BUTTONSCOMMANDER
#ifndef NO_BUTTONSCOMMANDERANALOGPUSHES

bool ButtonsCommanderAnalogPushesItem::IsPushed(int inCurrentValue)
{
	if (inCurrentValue >= this->analogValue - this->readingTolerancy && inCurrentValue <= this->analogValue + this->readingTolerancy)
		return true;

	return false;
}

#ifdef COMMANDERS_PRINT_COMMANDERS
void ButtonsCommanderAnalogPushesItem::printCommander()
{
	Serial.print(F("Push Id: "));
	Serial.print(this->Id);
	Serial.print(F(" / value: "));
	Serial.println(this->analogValue);
}
#endif
#endif
#endif