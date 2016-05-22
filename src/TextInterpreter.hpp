//-------------------------------------------------------------------
#ifndef __textInterpreter_H__
#define __textInterpreter_H__
//-------------------------------------------------------------------

#include "Commanders.h"

// Ahis class 'interprets' a string and send an event when found.
//
// The accepted syntax is 
// id separator event separator data
// where id can be a number from 0 to 4 000 000 000,
// where separator can be ',' , ';' , '/' or ':'
// where event can be any string of 9 characters long maximum, but must begin with
//		't' to toggle
//		'm' to make a move, data is necessary a COMMANDERS_MOVE_TYPE enum value in this case
//		'p' to make a move to a given position, data is necessary in this case
//		'c' to change configuration, data is necessary in this case : high byte is the address, low byte the value.
// and where the data can be a COMMANDERS_MOVE_TYPE enum value if this is a MOVE event,
// or any number between -32767 and 32768 for a move position event.
//
// ex : 123 means ID 123 toggled
// ex : 456 | T means ID 456 toggled
// ex : 789 , p , 100  means ID 789 make an absolute movement to 100 .

enum TEXTINTERPRETER_STEP
{
	TEXTINTERPRETER_STEP_ID,
	TEXTINTERPRETER_STEP_TYPE,
	TEXTINTERPRETER_STEP_DATA,
	TEXTINTERPRETER_STEP_END
};

class TextInterpreter
{
private:
	unsigned long id;
	COMMANDERS_EVENT_TYPE lastEventType;
	int data;

	bool neg_sign;
	TEXTINTERPRETER_STEP step;

public:
	TextInterpreter();

	void Init();
	unsigned long SendChar(char inCharacter);
	unsigned long SendString(char *inpString);
};

#endif
