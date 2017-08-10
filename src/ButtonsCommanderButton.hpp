//-------------------------------------------------------------------
#ifndef __buttonsCommanderButton_H__
#define __buttonsCommanderButton_H__
//-------------------------------------------------------------------

#include <Commanders.h>
#ifndef NO_BUTTONSCOMMANDER

/**
A button is a hardware device allowing user to act when this device is used. Its id is sent, 
and the listener of the commander events can do the job.

This is the base class for all buttons, this class should not be instanciate !
*/
class ButtonsCommanderButton
{
 protected:
	 /** Id of this button. This id should be unique ! */
	unsigned long Id;

 private:
	ButtonsCommanderButton *pNextButton;

 public:
	/** Default constructor.
	@param inId	each button can be identified by at least one id.
	*/
	ButtonsCommanderButton(unsigned long inId);
	/** Gets the button's id.*/
	inline unsigned long GetId() const { return this->Id; }
	/** Gets the button's address from its id.
	@param inId	id to find.
	@return found button address or NULL if not found.
	*/
	inline virtual ButtonsCommanderButton* GetFromId(unsigned long inId) { if (this->Id == inId) return this; return NULL; }

	/** Gets the current position of the button. Only possible on analog buttons like potentiometers or encoders.
	@return Current value, or 0 if the button does not allow this.
	*/
	inline virtual int GetPosition() const { return 0; }

	/** This function will be called after sketch Setup(), but before the first call to loop().
	It will be called only one time.*/
	inline virtual void beforeFirstLoop() { }
	/** Main loop function. */
	inline virtual unsigned long loop()
	{ 
		Commanders::SetLastEventType(COMMANDERS_EVENT_NONE);
		Commanders::SetLastEventData(0);
		return UNDEFINED_ID; 
	}
	/** Reinitialize the button for future use. Called at the end of the loop, if an event has been raised.*/
	inline virtual void EndLoop() {}

	/** Adds a new button at the end of the button linked list.
	@param inpButton new button to add.
	*/
	inline void SetNextButton(ButtonsCommanderButton *inpButton) { this->pNextButton = inpButton; }
	/** Gets the next button of 'this' in the linked list.
	@return address of the next button, or NULL if this was the end of the list.
	*/
	inline ButtonsCommanderButton *GetNextButton() const { return this->pNextButton; }
#ifdef COMMANDERS_PRINT_COMMANDERS
	/** Print this Commander on the console.
	@remark Only available if COMMANDERS_PRINT_COMMANDERS is defined.
	*/
	inline virtual void printCommander() {}
#endif
};

//-------------------------------------------------------------------
#endif
#endif
