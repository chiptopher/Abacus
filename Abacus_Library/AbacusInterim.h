/**
 * Filename:	AbacusInterim.h
 * Author:		Christopher Boyer
 * 
 * Defines a type for passing around the current information about
 * the alarm clock.
 */

#ifndef ABACUS_INTERIM
#define ABACUS_INTERIM

#include <stdbool.h>
#include "AbacusAlarmState.h"
#include "AbacusMachineState.h"

typedef struct {

	long alarmTime;
	long setTime;
	long answer;
	long currentAnswer;

	bool isAM;
	bool alarmAM;

	bool setAM;
	bool changeTime;

	bool changeText;

	AlarmState alarmState;
	MachineState machineState;

	char equation[12];
	char topMessage[17];
	char bottomMessage[17];

} InterimResult;


#endif