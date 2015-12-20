/*************************************************************
project: <Universal Accessory Decoder>
author: <Thierry PARIS>
description: <Base Commander>
*************************************************************/

#include "Commander.hpp"

#ifndef NO_COMMANDER
StaticCommanderData Commander::StaticData;
EventHandlerFunction Commander::EventHandler;

#ifdef DEBUG_MODE
#define CHECK(val, text)	CheckIndex(val, F(text))
#else
#define CHECK(val, text)
#endif

StaticCommanderData::StaticCommanderData()
{
	this->CommanderSize = 4;
	this->pCommanderList = new Commander*[CommanderSize];
	this->CommanderAddCounter = 0;
}

void StaticCommanderData::CommanderPriorityLoop()
{
	for (int i = 0; i < this->CommanderAddCounter; i++)
		this->pCommanderList[i]->PriorityLoop();
}

void Commander::AddCommander(Commander *inCommander)
{
	if (Commander::StaticData.CommanderAddCounter == Commander::StaticData.CommanderSize)
	{
		Commander **pNewList = new Commander*[Commander::StaticData.CommanderSize + 1];

		for (int i = 0; i < Commander::StaticData.CommanderSize; i++)
			pNewList[i] = Commander::StaticData.pCommanderList[i];

		Commander::StaticData.CommanderSize++;
		delete Commander::StaticData.pCommanderList;
		Commander::StaticData.pCommanderList = pNewList;
	}

	Commander::StaticData.pCommanderList[Commander::StaticData.CommanderAddCounter++] = inCommander;
}

void Commander::Loops()
{
	for (int i = 0; i < Commander::StaticData.CommanderAddCounter; i++)
		Commander::StaticData.pCommanderList[i]->Loop();
}

#ifdef DEBUG_MODE
void Commander::CheckIndex(byte inIndex, const __FlashStringHelper *inFunc)
{
}
#endif

#endif
