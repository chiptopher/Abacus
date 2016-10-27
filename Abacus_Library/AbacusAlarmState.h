/**
 * Filename: 	AbacusAlarmState.h
 * Author:		Christopher Boyer
 * 
 * Defines the possible states for the alarm buzzer to be in, and a corresponding
 * type. Pinout included the comments.
 */

#ifndef ABACUS_ALARM_STATE
#define ABACUS_ALARM_STATE

typedef enum {

	ALARM_BLAST = 0,	/* 01 */
	ALARM_JINGLE = 1,	/* 10 */
	ALARM_GO = 2,		/* 11 */
	ALARM_OFF = 3		/* 00 */

} AlarmState;

#endif