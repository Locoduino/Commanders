//-------------------------------------------------------------------
#ifndef __EventStack_Hpp__
#define __EventStack_Hpp__
//-------------------------------------------------------------------

#include "Commanders.h"

/*
The event stack is a small location for a few events ready to be send to the caller.
This is a barely a list. If the 'id' for the given index is UNDEFINED_ID, so no event is defined
at this index.
Each RaiseEvent will be push the new one in the first free index of the list.
Each PushEvent will returns the first not free event content, and free the event list for this index.
There is no priority in the list...
*/

// Commanders dont have need for more events at this moment !
// A debug message will be shown if an event is lost because of this tiny size !
#define EVENT_MAXNUMBER	3

class EventStack
{
private:
	unsigned long id[EVENT_MAXNUMBER];
	COMMANDERS_EVENT_TYPE type[EVENT_MAXNUMBER];
	int data[EVENT_MAXNUMBER];

	EventStack()
	{
		for (byte i = 0; i < EVENT_MAXNUMBER; i++)
			FreeEvent(i);
	}

	void FreeEvent(byte inEvent); 

public:
	static EventStack EventsStack;

	void RaiseEvent(unsigned long inId, COMMANDERS_EVENT_TYPE inType, int inData);
	void PushEvent(byte inIndex, unsigned long *inpId, COMMANDERS_EVENT_TYPE *inpType, int *inpData);
	byte GetPendingEvent();
};


#endif