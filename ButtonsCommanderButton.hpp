//-------------------------------------------------------------------
#ifndef __buttonsCommanderButton_H__
#define __buttonsCommanderButton_H__
//-------------------------------------------------------------------

#include "Arduino.h"
#include "Commander.hpp"

//-------------------------------------------------------------------
// A button is a hardware device allowing user to activate an accessory or a group.
// when this device is used, a DCC id is sent, and the according accessory
// can do the job.
//-------------------------------------------------------------------

class ButtonsCommanderButton
{
 private:
	 unsigned long Id;

 public:
	 inline ButtonsCommanderButton(unsigned long inId) { this->Id = inId; }
	 inline virtual unsigned long GetId() const { return this->Id; }
	 inline virtual bool IsAnalog() const { return false; }
	 inline virtual int GetPosition() const { return 0; }

	 inline virtual unsigned long Loop() { return UNDEFINED_ID; }
	 inline virtual void EndLoop() {}
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
