#include "EventsSequencer.hpp"

#ifndef NO_EVENTSSEQUENCER
EventsSequencer *EventsSequencer::pFirstSequencer = NULL;

EventsSequencer::EventsSequencer()
{
	this->pFirst = NULL;
	this->flags = 0;
	this->pCurrent = NULL;
	this->startDate = 0;

	this->pNextSequencer = NULL;
	if (this->pFirstSequencer == NULL)
		this->pFirstSequencer = this;
	else
		this->GetLastSequencer()->pNextSequencer = this;
}

void EventsSequencer::AddEvent(unsigned long inId, COMMANDERS_EVENT_TYPE inType, int inData, unsigned long inDelay)
{
#ifdef COMMANDERS_DEBUG_MODE
	if (this->IsPPointer())
	{
		Serial.println(F("Warning : a PROGMEM event list is already defined !"));
	}
#endif
	EventsSequencerItem *pNew = new EventsSequencerItem();//inId, inType, inData, inDelay);
	pNew->delay = inDelay;
	pNew->id = inId;
	pNew->type = inType;
	pNew->data = inData;
	pNew->next = NULL;
	if (this->pFirst == NULL)
		this->pFirst = pNew;
	else
		this->GetLastItem()->SetNext(pNew);
}

void EventsSequencer::AddEvents(const EventsSequencerItem *apEvents)
{
#ifdef COMMANDERS_DEBUG_MODE
	if (!this->IsPPointer() && this->pFirst != NULL)
	{
		Serial.println(F("Warning : an event list is already defined !"));
	}
#endif
	this->pFirst = (EventsSequencerItem *)apEvents;
	this->SetPPointer();
}

EventsSequencerItem *EventsSequencer::GetLastItem() const
{
	if (this->IsPPointer())
		return NULL;

	EventsSequencerItem *curr = this->pFirst;
	while (curr != NULL && curr->GetNext() != NULL)
		curr = curr->GetNext();

	return curr;
}

EventsSequencerItem *EventsSequencer::GetFirstItem(EventsSequencerItem *apLocal) const
{
	if (this->IsPPointer())
	{
		memcpy_P(apLocal, this->pFirst, sizeof(EventsSequencerItem));
		apLocal->next = this->pFirst + 1;
		return apLocal;
	}

	return this->pFirst;
}

EventsSequencerItem *EventsSequencer::GetCurrentItem(EventsSequencerItem *apLocal) const
{
	if (this->IsPPointer())
	{
		memcpy_P(apLocal, this->pCurrent, sizeof(EventsSequencerItem));
		apLocal->next = this->pCurrent + 1;
		return apLocal;
	}

	return this->pCurrent;
}

void EventsSequencer::StartItem(EventsSequencerItem *inItem)
{ 
	if (inItem == NULL)
		inItem = this->pFirst;
	this->pCurrent = inItem; 
	EventsSequencerItem item;
	this->GetCurrentItem(&item);
	Commanders::RaiseEvent(item.id, item.type, item.data); 
	this->startDate = millis(); 
}

bool EventsSequencer::IsCurrentItemLast()
{
	if (this->IsPPointer())
	{
		EventsSequencerItem item;
		this->GetCurrentItem(&item);

		if (item.id == UNDEFINED_ID)
			return true;

		this->pCurrent += 1;
		this->GetCurrentItem(&item);
		this->pCurrent -= 1;

		if (item.id == UNDEFINED_ID)
			return true;
	}
	else
	{
		if (this->pCurrent->GetNext() == NULL)
			return true;
	}

	return false;
}

bool EventsSequencer::loop()
{
	if (this->pCurrent == NULL)
		return false;	// Sequence stopped.

	EventsSequencerItem item;
	this->GetCurrentItem(&item);

	if (millis() - this->startDate < item.GetDelay())
		return false;

	if (this->IsCurrentItemLast())
	{
		if (this->IsPerpetual())
			this->StartItem();
		else
			this->startDate = 0;
		return false;
	}

	if (this->IsPPointer())
		this->StartItem(this->pCurrent + 1);
	else
		this->StartItem(this->pCurrent->GetNext());
	return true;
}

EventsSequencer *EventsSequencer::GetLastSequencer()
{
	EventsSequencer *curr = EventsSequencer::pFirstSequencer;
	while (curr != NULL && curr->GetNextSequencer() != NULL)
		curr = curr->GetNextSequencer();

	return curr;
}

void EventsSequencer::loops()
{
	EventsSequencer *curr = EventsSequencer::pFirstSequencer;
	while (curr != NULL)
	{
		if (!curr->IsPaused())
			curr->loop();
		curr = curr->GetNextSequencer();
	}
}

#ifdef COMMANDERS_PRINT_COMMANDERS
void EventsSequencer::printSequencer()
{
	Serial.print(F("    Sequencer "));
	Serial.println((long) this, HEX);

	EventsSequencerItem item, *pItem, *pNext;
	
	pItem = this->GetFirstItem(&item);

	while (pItem != NULL && pItem->id != UNDEFINED_ID)
	{
		Serial.print(F("        Event - Id: "));
		Serial.print(pItem->id);
		Serial.print(F(" / type: "));
		Commanders::printEventType(pItem->type, true);
		Commanders::printEventData(pItem->type, pItem->data);
		Serial.print(F(" / delay: "));
		Serial.print(pItem->delay);
		Serial.println(F(""));
	
		if (this->IsPPointer())
		{
			pNext = pItem->next;
			memcpy_P(pItem, pItem->next, sizeof(EventsSequencerItem));
			pItem->next = pNext + 1;
		}
		else
			pItem = pItem->next;
	}
}

void EventsSequencer::printAllSequencers()
{
	EventsSequencer *curr = EventsSequencer::pFirstSequencer;
	while (curr != NULL)
	{
		curr->printSequencer();
		curr = curr->GetNextSequencer();
	}
}
#endif

#endif
