//-------------------------------------------------------------------
#ifndef __EventsSequencer_Hpp__
#define __EventsSequencer_Hpp__
//-------------------------------------------------------------------

#include <Commanders.h>

#ifndef NO_EVENTSSEQUENCER

/** Event sequencer item. This class represents one item , one event.
Each event has an id, a type and a data. Each item contains a delay before the following event.
*/
struct EventsSequencerItem
{
	unsigned long id;	/**< id of this event.*/
	COMMANDERS_EVENT_TYPE type;	/**< Type of this event.*/
	int data;	/**< associated data of this event.*/
	unsigned long delay;			/**< delay for 'this' command.*/
	EventsSequencerItem *next;	/**< next event in the chained list.*/

	/** Get the next event in the chained list.*/
	EventsSequencerItem *GetNext() { return this->next; }
	/** Link to a next event in the chained list of events.*/
	void SetNext(EventsSequencerItem *inNext) { this->next = inNext; }
	/** Get the delay in millisecond for this event, before passing to the next event.*/
	unsigned long GetDelay() { return this->delay; }
};

/** Bit number of the 'perpetual' flag.
*/
#define FLAG_PERPETUAL		0
/** Bit number of the PROGMEM pointer flag.
*/
#define FLAG_PPOINTER		1
/** Bit number of the 'paused' flag.
*/
#define FLAG_PAUSED			2

/** This class represents an event sequencer : a list of events raised at given intervals.
This is the way to program some animations using several lights/motors and other accessories.

The sequence can be started from the beginning or from any item of the list.
When started, it can be completely stopped, or just paused. When paused, the timer is stopped and restart at its previous position when unpaused.
*/
class EventsSequencer
{
private:
	EventsSequencer * pNextSequencer;
	static EventsSequencer *pFirstSequencer;

	EventsSequencer *GetNextSequencer() { return this->pNextSequencer; }

	static EventsSequencer *GetLastSequencer(); 

	unsigned long int startDate;
	byte flags;

	/** Gets the first item.
	*/
	EventsSequencerItem *GetFirstItem(EventsSequencerItem *apLocal = NULL) const;
	/** Gets the current item.
	*/
	EventsSequencerItem *GetCurrentItem(EventsSequencerItem *apLocal = NULL) const;
	/** Gets the last item of the list.
	*/
	EventsSequencerItem *GetLastItem() const; 

	/** Updates the PROGMEM pointer indicator flags of this sequencer.
	@param flag	new value of this flag.
	*/
	void WritePPointer(bool flag) { bitWrite(this->flags, FLAG_PPOINTER, flag); }
	/** Sets the PROGMEM pointer flag of this sequencer.
	*/
	void SetPPointer() { bitSet(this->flags, FLAG_PPOINTER); }
	/** Resets the PROGMEM pointer flag of this sequencer.
	*/
	void ResetPPointer() { bitClear(this->flags, FLAG_PPOINTER); }

	/** Updates the perpetual flag of this sequencer.
	@param flag	new value of this flag.
	*/
	void WritePerpetual(bool flag) { bitWrite(this->flags, FLAG_PERPETUAL, flag); }
	/** Updates the 'paused' flag.
	@param flag	new value of this flag.
	*/
	void WritePaused(bool flag) { bitWrite(this->flags, FLAG_PAUSED, flag); }

	/** Sets the perpetual flag of this sequencer.
	*/
	void SetPerpetual() { bitSet(this->flags, FLAG_PERPETUAL); }
	/** Sets the 'paused' flag of this sequencer.
	*/
	void SetPaused() { bitSet(this->flags, FLAG_PAUSED); }

	/** Resets the perpetual flag of this sequencer.
	*/
	void ResetPerpetual() { bitClear(this->flags, FLAG_PERPETUAL); }
	/** Resets the 'paused' flag of this sequencer.
	*/
	void ResetPaused() { bitClear(this->flags, FLAG_PAUSED); }

	bool IsCurrentItemLast();

public:
	/** First item of the chained list of sequences.
	*/
	EventsSequencerItem *pFirst;
	/** Current executed item.
	*/
	EventsSequencerItem *pCurrent;

	/** Default constructor
	*/
	EventsSequencer();

	/** Checks if the sequence is perpetual : at the end, restart from the begginning.
	@return	true if the sequence is perpetual.
	*/
	bool IsPerpetual() const { return bitRead(this->flags, FLAG_PERPETUAL) > 0; }
	/** Checks if the sequence storage is in a PROGMEM container
	@return	true if the container has been declared with PROGMEM and added with AddEvents().
	*/
	bool IsPPointer() const { return bitRead(this->flags, FLAG_PPOINTER) > 0; }
	/** Checks if the sequence is paused : the Pause() function has been called..
	@return	true if the sequence is paused.
	*/
	bool IsPaused() const { return bitRead(this->flags, FLAG_PAUSED) > 0; }

	/** Begin of the EventsSequencer.
	@param inPerpetual if true, the sequencer will never stop, returning to the first item after the last one.
	*/
	void begin(bool inPerpetual) { this->WritePerpetual(inPerpetual); }

	/** Add a new event	to the list.
	@param inId		id of the event
	@param inType	type
	@param inData	associated data
	@param inDelay	time in milliseconds before the next event.
	*/
	void AddEvent(unsigned long inId, COMMANDERS_EVENT_TYPE inType, int inData, unsigned long inDelay);
	/** Add an event list in program memory PROGMEM.
	@param apEvents	List of events declared with PROGMEM
	*/
	void AddEvents(const EventsSequencerItem *apEvents);
	/** Sets the current item to first to start the execution.
	*/
	void StartItem(EventsSequencerItem *inItem = NULL);

	/** Sets the current item to first to start the execution.
	*/
	void Start() { this->StartItem(); }
	/** Stops the execution by nulling the current item.
	*/
	void Stop() { this->pCurrent = NULL; }
	/** Pause the execution.
	*/
	void Pause() { this->SetPaused(); }
	/** Continue the execution after a pause.
	*/
	void Continue() { this->ResetPaused(); }

	/** Main loop of this sequencer
	@return	true if a new event has been raised.
	*/
	bool loop();

	/** Main loop of all sequencers
	*/
	static void loops();

#ifdef COMMANDERS_PRINT_COMMANDERS
	/** Print this Sequencer on the console.
	@remark Only available if COMMANDERS_PRINT_COMMANDERS is defined.
	*/
	void printSequencer();
	/** Print all Sequencers on the console.
	@remark Only available if COMMANDERS_PRINT_COMMANDERS is defined.
	*/
	static void printAllSequencers();
#endif
};


#endif
#endif