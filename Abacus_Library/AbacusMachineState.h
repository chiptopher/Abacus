/**
 * Filename:	AbacusMachineState.h
 * Author:		Christopher Boyer
 * 
 * Defines a type for and the possible state of the alarm clock.
 */

#ifndef ABACUS_MACHINE_STATE
#define ABACUS_MACHINE_STATE

typedef enum {

	DISPLAY_TIME,
	NO_ALARM,
	SET_ALARM,
	SET_TIME,
	ALARM_ON

} MachineState;


#endif