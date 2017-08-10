//-------------------------------------------------------------------
#ifndef __textInterpreter_H__
#define __textInterpreter_H__
//-------------------------------------------------------------------

#include <Commanders.h>

#ifndef NO_SERIALCOMMANDER	  

enum TEXTINTERPRETER_STEP
{
	TEXTINTERPRETER_STEP_ID,
	TEXTINTERPRETER_STEP_ID2,
	TEXTINTERPRETER_STEP_TYPE,
	TEXTINTERPRETER_STEP_DATA,
	TEXTINTERPRETER_STEP_END
};

/** This class 'interprets' a string and send an event when found.

The accepted syntax is
\verbatim
id separator event separator data
\endverbatim
where id can be a number from 0 to 4 000 000 000,
where separator can be ',' , ';' , '/' or ' '
where event can be any string of 9 characters long maximum, but must begin with (case insensitive)
- 't' COMMANDERS_EVENT_TOGGLE: to toggle
- 'm' COMMANDERS_EVENT_MOVE: to make a move, data is necessary a COMMANDERS_MOVE_TYPE enum value in this case
- 'p' COMMANDERS_EVENT_MOVEPOSITION: to make a move to a given position by a value, data is necessary in this case
- 'd' COMMANDERS_EVENT_MOVEPOSITIONID: to make a move to a given position identified by its Id, data is necessary in this case
- 'i' COMMANDERS_EVENT_MOVEPOSITIONINDEX: to make a move to a given position identified by its index in positions list, data is necessary in this case
- 'c' COMMANDERS_EVENT_CONFIG: to change configuration, data is necessary in this case : high byte is the address, low byte the value.
and where the data can be a COMMANDERS_MOVE_TYPE enum value if this is a MOVE event,
or any number between -32767 and 32768 for a move position event.

id can be dccid:dccacc for Dcc id, where dccid is the dcc address and dccacc 0 or 1.

- ex : 123 means ID 123 toggled
- ex : 456 | T means ID 456 toggled
- ex : 789 , p , 100  means ID 789 make an absolute movement to 100 .
- ex : 20:0 T means Dcc id 20/0 toggled
*/

class TextInterpreter
{
private:
	unsigned long id;
	uint8_t id2;
	COMMANDERS_EVENT_TYPE lastEventType;
	int data;

	bool neg_sign;
	TEXTINTERPRETER_STEP step;

public:
	/** Default constructor. */
	TextInterpreter();

	/** Initialize internal data. */
	void Init();
	/** Send a new character to the class. 
	@return Id if an event was found, or UNDEFINED_ID.
	*/
	unsigned long SendChar(char inCharacter);
	/** Send a full string to the class.
	@return Id if an event was found, or UNDEFINED_ID.
	*/
	unsigned long SendString(char *inpString);
};

#endif
#endif
