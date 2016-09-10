#pragma once

enum COMMANDERS_MOVE_TYPE
{
	COMMANDERS_MOVE_MORE = +1,
	COMMANDERS_MOVE_LESS = -1,
	COMMANDERS_MOVE_STOP = 0,
	COMMANDERS_MOVE_LEFT = -2,
	COMMANDERS_MOVE_RIGHT = -3,
	COMMANDERS_MOVE_CENTER = -4,
	COMMANDERS_MOVE_TOP = -5,
	COMMANDERS_MOVE_BOTTOM = -6,
	COMMANDERS_MOVE_STRAIGHT = -7,
	COMMANDERS_MOVE_DIVERGE = -8,
	COMMANDERS_MOVE_ON = -9,
	COMMANDERS_MOVE_OFF = -10
};

enum COMMANDERS_EVENT_TYPE
{
	COMMANDERS_EVENT_NONE = 0,				// Should never appear
	COMMANDERS_EVENT_TOGGLE = 1,			// If a push button or similar is pressed, invert the state/position
	COMMANDERS_EVENT_MOVE = 2,				// If a push 'left' button or similar is pressed, data is a COMMANDERS_MOVE_TYPE
	COMMANDERS_EVENT_MOVEPOSITION = 3,		// Move to a given position value given by data.
	COMMANDERS_EVENT_MOVEPOSITIONINDEX = 4,	// Move to an indexed position from pMovingPosition given by data.
	COMMANDERS_EVENT_MOVEPOSITIONID = 5,	// Move to an identified position from pMovingPosition, given by the event id.
	COMMANDERS_EVENT_CONFIG = 6,			// Data is the configuration address and value
};

#ifndef UNDEFINED_ID
#define UNDEFINED_ID	((unsigned long)-1)
#endif
