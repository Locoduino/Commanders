//-------------------------------------------------------------------
#ifndef __buttonsCommanderButton_H__
#define __buttonsCommanderButton_H__
//-------------------------------------------------------------------

#ifdef VISUALSTUDIO
	#include "../VStudio/arduino.h"
#else
	#include "Arduino.h"
#endif
#include "Commander.hpp"

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
	 inline ButtonsCommanderButton(unsigned long inId) { this->Id = inId; this->pNextButton = 0; }
	 inline unsigned long GetId() const { return this->Id; }
	 inline virtual ButtonsCommanderButton* GetFromId(unsigned long inId) { return this; }

	 inline virtual bool IsAnalog() const { return false; }
	 inline virtual int GetPosition() const { return 0; }

	 inline virtual CommanderEvent Loop() { return EmptyEvent; }
	 inline virtual void EndLoop() {}

	 inline void SetNextButton(ButtonsCommanderButton *inButton) { this->pNextButton = inButton; }
	 inline ButtonsCommanderButton *GetNextButton() const { return this->pNextButton; }
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
