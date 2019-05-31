//-------------------------------------------------------------------
#ifndef __buttonsCommanderKeyboard_HPP__
#define __buttonsCommanderKeyboard_HPP__
//-------------------------------------------------------------------

#include <Commanders.h>
#ifndef NO_BUTTONSCOMMANDER

struct KeybdEvent
{
	unsigned long Id;
	COMMANDERS_EVENT_TYPE EventType;
	int Data;
};

//-------------------------------------------------------------------
// A keyboard button is a used by debugging in VisualStudio.
//-------------------------------------------------------------------

class ButtonsCommanderKeyboard : public ButtonsCommanderButton
{
 private:
	int key;	// character to scan...
	int lastKeyPressed;

	CMDRSCHAINEDLIST<KeybdEvent> Events;

public:
	ButtonsCommanderKeyboard();

	void begin(unsigned long inId, int inKey, COMMANDERS_EVENT_TYPE inEventType = COMMANDERS_EVENT_MOVEPOSITIONID, int inData = 0);
	void AddEvent(unsigned long inId, COMMANDERS_EVENT_TYPE inEventType = COMMANDERS_EVENT_MOVEPOSITIONID, int inData = 0);
	unsigned long loop();
#ifdef COMMANDERS_PRINT_COMMANDERS
	void printCommander();
#endif
};

//-------------------------------------------------------------------
#endif
#endif
