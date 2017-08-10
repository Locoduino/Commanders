#pragma once
/** @file Events.h
\brief Events which can be thrown.
*/

/**	Describes the associated data with some particular cases of event type COMMANDERS_EVENT_MOVE.
These values will be interpreted by the receiving object. Some values could not have meaning in the context...*/
enum COMMANDERS_MOVE_TYPE
{
	COMMANDERS_MOVE_MORE = +1,		/**< Add an incremental value, like an encoder can do.*/
	COMMANDERS_MOVE_LESS = -1,		/**< Substract an incremental value, like an encoder can do.*/
	COMMANDERS_MOVE_STOP = 0,		/**< Stop the current movement.*/
	COMMANDERS_MOVE_LEFT = -2,		/**< Move to left side.*/
	COMMANDERS_MOVE_RIGHT = -3,		/**< Move to right side.*/
	COMMANDERS_MOVE_CENTER = -4,	/**< Move to center.*/
	COMMANDERS_MOVE_TOP = -5,		/**< Move to top.*/
	COMMANDERS_MOVE_BOTTOM = -6,	/**< Move to bottom.*/
	COMMANDERS_MOVE_STRAIGHT = -7,	/**< Set the turnout to straight position.*/
	COMMANDERS_MOVE_DIVERGE = -8,	/**< Set the turnout to deviated position.*/
	COMMANDERS_MOVE_ON = -9,		/**< Set accessory on.*/
	COMMANDERS_MOVE_OFF = -10		/**< Set accessory off.*/
};

/**	Describes the main events thrown by Commanders. */
enum COMMANDERS_EVENT_TYPE
{
	COMMANDERS_EVENT_NONE = 0,				/**< Should never appear.*/
	COMMANDERS_EVENT_TOGGLE = 1,			/**< If a push button or similar is pressed, invert the state/position.*/
	COMMANDERS_EVENT_MOVE = 2,				/**< If a push 'left' button or similar is pressed, data is a COMMANDERS_MOVE_TYPE.*/
	COMMANDERS_EVENT_MOVEPOSITION = 3,		/**< Move to a given position value given by data.*/
	COMMANDERS_EVENT_MOVEPOSITIONINDEX = 4,	/**< Move to an indexed position from pMovingPosition given by data.*/
	COMMANDERS_EVENT_MOVEPOSITIONID = 5,	/**< Move to an identified position from pMovingPosition, given by the event id.*/
	COMMANDERS_EVENT_CONFIG = 6,			/**< Data is the configuration address and value.*/
};

#ifndef UNDEFINED_ID
/** Undefined id ! Used by all loop() functions if no event has been detected.*/
#define UNDEFINED_ID	((unsigned long)-1)
#endif
