#include "EventStack.hpp"

EventStack EventStack::EventsStack;

void EventStack::FreeEvent(byte inEvent) 
{
	id[inEvent] = UNDEFINED_ID;
	type[inEvent] = COMMANDERS_EVENT_NONE;
	data[inEvent] = 0; 
}

void EventStack::RaiseEvent(unsigned long inId, COMMANDERS_EVENT_TYPE inType, int inData)
{
	for (int i =0; i < EVENT_MAXNUMBER; i++)
		if (this->id[i] == UNDEFINED_ID)
		{
			this->id[i] = inId;
			this->type[i] = inType;
			this->data[i] = inData;
			return;
		}

#ifdef COMMANDERS_DEBUG_MODE
	Serial.println("Error : an event has been lost !");
#endif
}

byte EventStack::GetPendingEvent()
{
	for (int i = 0; i < EVENT_MAXNUMBER; i++)
		if (this->id[i] != UNDEFINED_ID)
			return i;

	return 255;
}

void EventStack::PushEvent(byte inEvent, unsigned long *inpId, COMMANDERS_EVENT_TYPE *inpType, int *inpData)
{
	*inpId = this->id[inEvent];
	*inpType = this->type[inEvent];
	*inpData = this->data[inEvent];
	FreeEvent(inEvent);
}
