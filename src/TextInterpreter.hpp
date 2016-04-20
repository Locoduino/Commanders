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
// event can be any string of 9 characters long maximum, but must begin with
//		's' or 'S' to select
//		'a' or 'A' to make an absolute move, data is necessary in this case
//		'r' or 'R' to make a relative move, data is necessary in this case
// and where the data can be any number	between -32767 and 32768
//
// ex : 123 means ID 123 selected
// ex : 456 | s means ID 456 selected
// ex : 789 , A , 100  means ID 789 make an absolute movement to 100 .

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
	COMMANDERS_EVENT_TYPE eventType;
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
