#include "TextInterpreter.hpp"

TextInterpreter::TextInterpreter()
{
	this->Init();
}

void TextInterpreter::Init()
{
	this->id = 0;
	this->lastEventType = COMMANDERS_EVENT_NONE;
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
				if (this->lastEventType == COMMANDERS_EVENT_NONE)
					this->lastEventType = COMMANDERS_EVENT_TOGGLE;

#ifdef COMMANDERS_DEBUG_MODE
				if (this->lastEventType != COMMANDERS_EVENT_TOGGLE)
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

			Commander::RaiseEvent(this->id, this->lastEventType, this->data);
			Commanders::SetLastEventType(this->lastEventType);
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
#ifdef COMMANDERS_DEBUG_MODE
				Serial.print(F("id = "));
				Serial.println(this->id);
#endif
				this->step = TEXTINTERPRETER_STEP_TYPE;
				break;
			case TEXTINTERPRETER_STEP_TYPE:	
#ifdef COMMANDERS_DEBUG_MODE
				Serial.print(F("event = "));
				Serial.println(this->lastEventType);
#endif
				this->step = TEXTINTERPRETER_STEP_DATA;
				this->data = 0;	
				break;
			case TEXTINTERPRETER_STEP_DATA:	
#ifdef COMMANDERS_DEBUG_MODE
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
			if (this->lastEventType == COMMANDERS_EVENT_NONE)
			{
				if (inCharacter == 't' || inCharacter == 'T')		this->lastEventType = COMMANDERS_EVENT_TOGGLE;
				if (inCharacter == 'm' || inCharacter == 'M')		this->lastEventType = COMMANDERS_EVENT_MOVE;
				if (inCharacter == 'p' || inCharacter == 'P')		this->lastEventType = COMMANDERS_EVENT_MOVEPOSITION;
				if (inCharacter == 'c' || inCharacter == 'C')		this->lastEventType = COMMANDERS_EVENT_CONFIG;
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
