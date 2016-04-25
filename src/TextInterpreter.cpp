#include "TextInterpreter.hpp"

TextInterpreter::TextInterpreter()
{
	this->Init();
}

void TextInterpreter::Init()
{
	this->id = 0;
	this->eventType = COMMANDERS_EVENT_NONE;
	this->data = 0;
	this->neg_sign = false;
	this->step = TEXTINTERPRETER_STEP_ID;
}

unsigned long TextInterpreter::SendChar(char inCharacter)
{
	if (inCharacter == '\n')
	{
		unsigned long foundID = UNDEFINED_ID;

		/* end of the command by a '\n' : data OK */
		if (this->id != UNDEFINED_ID && this->id != 0)
		{
			{
				if (this->eventType == COMMANDERS_EVENT_NONE)
					this->eventType = COMMANDERS_EVENT_SELECTED;

#ifdef DEBUG_MODE
				if (this->eventType != COMMANDERS_EVENT_SELECTED)
				{
					Serial.print(F("data = "));
					if (this->neg_sign)
						Serial.print(F("-"));
					Serial.println(this->data);
				}
#endif	

				if (this->neg_sign == true)
					this->data = -this->data;
			}

			Commander::RaiseEvent(this->id, this->eventType, this->data);
			Commanders::SetLastEventType(this->eventType);
			Commanders::SetLastEventData(this->data);
			foundID = this->id;
		}

		this->Init();
		return foundID;
	}

	if (inCharacter == ' ') 
		return UNDEFINED_ID;   /* spaces dont matter */
	if (inCharacter == (char)-1) 
		return UNDEFINED_ID;  /* loop if empty buffer */

	if (inCharacter == ',' || inCharacter == ';' || inCharacter == '/' || inCharacter == ':')
	{
#ifdef DEBUG_VERBOSE_MODE
		Serial.println(F("separator"));
#endif
			switch (step)
			{
			case TEXTINTERPRETER_STEP_ID:	
#ifdef DEBUG_MODE
				Serial.print(F("id = "));
				Serial.println(this->id);
#endif
				this->step = TEXTINTERPRETER_STEP_TYPE;
				break;
			case TEXTINTERPRETER_STEP_TYPE:	
#ifdef DEBUG_MODE
				Serial.print(F("event = "));
				Serial.println(this->eventType);
#endif
				this->step = TEXTINTERPRETER_STEP_DATA;
				this->data = 0;	
				break;
			case TEXTINTERPRETER_STEP_DATA:	
#ifdef DEBUG_MODE
				Serial.print(F("data = "));
				if (this->neg_sign)
					Serial.print(F("-"));
				Serial.println(this->data);
#endif
				this->step = TEXTINTERPRETER_STEP_END;
				break;
			default:								
				break;
			}
		return UNDEFINED_ID;
	}

	if (step == TEXTINTERPRETER_STEP_END)	/* end of useful input ! */
		return UNDEFINED_ID;

#ifdef DEBUG_VERBOSE_MODE
	Serial.print(F("read ")); \
	Serial.println(inCharacter, DEC);
#endif

	switch (this->step)
	{
		case TEXTINTERPRETER_STEP_ID:
			if (inCharacter >= 48 && inCharacter <= 57)
			{
				this->id *= 10;                    /* *10 => shift to left */
				this->id = ((inCharacter - 48) + this->id);  /* 48 because of ASCII value (1 => 49 in ASCII) */
				this->data = 0;
			}
			break;

		case TEXTINTERPRETER_STEP_TYPE:
			if (this->eventType == COMMANDERS_EVENT_NONE)
			{
				if (inCharacter == 's' || inCharacter == 'S')		this->eventType = COMMANDERS_EVENT_SELECTED;
				if (inCharacter == 'a' || inCharacter == 'A')		this->eventType = COMMANDERS_EVENT_ABSOLUTEMOVE;
				if (inCharacter == 'r' || inCharacter == 'R')		this->eventType = COMMANDERS_EVENT_RELATIVEMOVE;
			}
			break;

		case TEXTINTERPRETER_STEP_DATA:
			if (inCharacter == '-')
				this->neg_sign = true;
			if (inCharacter >= 48 && inCharacter <= 57)
			{
				this->data *= 10;                    /* *10 => shift to left */
				this->data = ((inCharacter - 48) + this->data);  /* 48 because of ASCII value (1 => 49 in ASCII) */
			}

			break;
		default:
			break;
	}
	return UNDEFINED_ID;
}

unsigned long TextInterpreter::SendString(char *inpString)
{
	char *p = inpString;

	while (*p != 0)
		this->SendChar(*(p++));

	return this->SendChar('\n'); // End of command
}