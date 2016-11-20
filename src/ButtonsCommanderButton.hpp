//-------------------------------------------------------------------
#ifndef __buttonsCommanderButton_H__
#define __buttonsCommanderButton_H__
//-------------------------------------------------------------------

#include "Commanders.h"
#ifndef NO_BUTTONSCOMMANDER

//-------------------------------------------------------------------
// A button is a hardware device allowing user to act when this device 
// is used. Its id is sent, and the listener of the commander events
// can do the job.
//-------------------------------------------------------------------

class ButtonsCommanderButton
{
 protected:
	unsigned long Id;

 private:
	ButtonsCommanderButton *pNextButton;

 public:
	ButtonsCommanderButton(unsigned long inId);
	inline unsigned long GetId() const { return this->Id; }
	inline virtual ButtonsCommanderButton* GetFromId(unsigned long inId) { return this; }

	//inline virtual bool IsAnalog() const { return false; }
	inline virtual int GetPosition() const { return 0; }

	inline virtual unsigned long loop()
	{ 
		Commanders::SetLastEventType(COMMANDERS_EVENT_NONE);
		Commanders::SetLastEventData(0);
		return UNDEFINED_ID; 
	}
	inline virtual void EndLoop() {}

	inline void SetNextButton(ButtonsCommanderButton *inButton) { this->pNextButton = inButton; }
	inline ButtonsCommanderButton *GetNextButton() const { return this->pNextButton; }
#ifdef COMMANDERS_PRINT_COMMANDERS
	inline virtual void printCommander() {}
#endif
};

//-------------------------------------------------------------------
#endif
#endif
