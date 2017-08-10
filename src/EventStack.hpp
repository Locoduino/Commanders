//-------------------------------------------------------------------
#ifndef __EventStack_Hpp__
#define __EventStack_Hpp__
//-------------------------------------------------------------------

#include <Commanders.h>

// Commanders dont have need for more events at this moment : more events, more memory consumed !
// A debug message will be shown if an event is lost because of this tiny size !
#define EVENT_MAXNUMBER	3

/** The event stack is a small location for a few events ready to be send to the caller.
This is a barely a list. If the 'id' for the given index is UNDEFINED_ID, so no event is defined
at this index.
Each RaiseEvent will be push the new one in the first free index of the list.
Each PushEvent will returns the first not free event content, and free the event list for this index.
There is no priority in the list...
*/
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
	/** Stack of events itself.*/
	static EventStack EventsStack;

	/** Add a new event	to the stack.
	@param inId		id of the event
	@param inType	type
	@param inData	associated data
	*/
	void PushEvent(unsigned long inId, COMMANDERS_EVENT_TYPE inType, int inData);
	/** Gets the given event from the stack, at the given index.
	@param inIndex	index of the event to get.
	@param inpId	address to fill with the id of the event
	@param inpType	address to fill with the type
	@param inpData	address to fill with the associated data
	@remark A previous call to GetPendingEventIndex() will give the first available event index.
	*/
	void GetEvent(byte inIndex, unsigned long *inpId, COMMANDERS_EVENT_TYPE *inpType, int *inpData);
	/** Gets the first available event index in the stack. There is no history of stack filling, so the 
	first one is not necessary the oldest in the list...
	*/
	byte GetPendingEventIndex();
};


#endif