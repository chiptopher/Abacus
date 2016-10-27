
/**
 * Filename:	Abacus.cpp
 * Auhor:		Christopher Boyer
 *
 * Contains some of the basic functionality for writing data to the 
 * alarm clocks outpur register as well as converting between the 12
 * hour and the 24 time format.
 */

#include "Abacus.h"
#include <stdbool.h>

#define NUM_EDGES 46

#define A_SEGMENT_POS 1
#define B_SEGMENT_POS 12
#define C_SEGMENT_POS 3
#define D_SEGMENT_POS 4
#define E_SEGMENT_POS 5
#define F_SEGMENT_POS 2
#define G_SEGMENT_POS 8
#define DECIMAL_POS 6
#define DIGIT_1_POS 0
#define DIGIT_2_POS 10
#define DIGIT_3_POS 11
#define DIGIT_4_POS 9
#define AM_LED_POS 7
#define ALM_ON_LED_POS 15
#define SPEAKER_IO_A 14
#define SPEAKER_IO_B 13


/* Number pattern on 7-segment display, corresponding to position in array */
unsigned int NUMBERS[10] = { 0xFC,  0x60, 0xDA, 0xF2, 0x66, 0xB6, 0xBE, 0xE0,
                           0xFE, 0xE6 };
/* The signal for for a specific digit, corresponding to position in array */
unsigned int DIGITS[4] = { 0x7, 0xB, 0xD, 0xE };

int getEdgeCount()
{
	return NUM_EDGES;
}

/**
 * Write the given time and current digit to the given output register.
 * @param outputRegister the location where the infomration will be written to
 * @param currentDigit   the digit being displayed
 * @param currentHour    the current hour of the clock
 * @param currentMinute  the current minute of the clock
 */
void writeTime(int * outputRegister,
			   int currentDigit,
			   int currentHour,
			   int currentMinute)
{
	unsigned int numberData;

	if (currentDigit == 0) {
		numberData = NUMBERS[currentHour / 10];
	} else if (currentDigit == 1) {
		numberData = NUMBERS[currentHour % 10] | 1;
	} else if (currentDigit == 2) {
		numberData = NUMBERS[currentMinute / 10];
	} else {
		numberData = NUMBERS[currentMinute % 10];
	}

	outputRegister[DECIMAL_POS] = numberData % 2;
	numberData = numberData / 2;
	outputRegister[G_SEGMENT_POS] = numberData % 2;
	numberData = numberData / 2;
	outputRegister[F_SEGMENT_POS] = numberData % 2;
	numberData = numberData / 2;
	outputRegister[E_SEGMENT_POS] = numberData % 2;
	numberData = numberData / 2;
	outputRegister[D_SEGMENT_POS] = numberData % 2;
	numberData = numberData / 2;
	outputRegister[C_SEGMENT_POS] = numberData % 2;
	numberData = numberData / 2;
	outputRegister[B_SEGMENT_POS] = numberData % 2;
	numberData = numberData / 2;
	outputRegister[A_SEGMENT_POS] = numberData % 2;
	numberData = numberData / 2;
	
	unsigned int digitData = DIGITS[currentDigit];

	outputRegister[DIGIT_4_POS] = digitData % 2;
	digitData = digitData / 2;
	outputRegister[DIGIT_3_POS] = digitData % 2;
	digitData = digitData / 2;
	outputRegister[DIGIT_2_POS] = digitData % 2;
	digitData = digitData / 2;
	outputRegister[DIGIT_1_POS] = digitData % 2;
	digitData = digitData / 2;

}

/**
 * Write the given alarm state to the given ourput register.
 * @param outputRegister the outpur register the alarm state is being written to
 * @param alarmState     the alarm state being written
 */
void writeAlarmState(int * outputRegister, AlarmState alarmState)
{
	if (alarmState == ALARM_OFF) {
		outputRegister[SPEAKER_IO_A] = 0;
		outputRegister[SPEAKER_IO_B] = 0;
	} else if (alarmState == ALARM_BLAST) {
		outputRegister[SPEAKER_IO_A] = 0;
		outputRegister[SPEAKER_IO_B] = 1;
	} else if (alarmState == ALARM_JINGLE) {
		outputRegister[SPEAKER_IO_A] = 1;
		outputRegister[SPEAKER_IO_B] = 0;
	} else if (alarmState == ALARM_GO) {
		outputRegister[SPEAKER_IO_A] = 1;
		outputRegister[SPEAKER_IO_B] = 1;
	}
}

/**
 * Write the status of the time to the AM/PM LED. alarmAM determines if the
 * alarm is currently AM (true). The LED is written to HIGH when based one 
 * a logical NOT of the current status of the alarm.
 * @param outputRegister the register where our information is written to
 * @param alarmAM        true if the time is AM; false otherwise
 */
void writeTimeLED(int * outputRegister, bool alarmAM)
{
	if (!alarmAM)
	{
		outputRegister[ALM_ON_LED_POS] = 1;
	}
	else
	{
		outputRegister[ALM_ON_LED_POS] = 0;
	}
	
}

/**
 * Format the given hour into the 12 hour format.
 * @param  inputHour the current hour in 24 hour format
 * @return           the given hour in 12 hour format
 */
int format12Hour(int inputHour)
{
	if (inputHour > 12) {
		return inputHour % 12;
	} else  if (inputHour > 0 && inputHour <= 12) {
		return inputHour;
	} else {
		return 12;
	}
}

/**
 * Format the given hour into the 24 hour format.
 * @param  inputHour the current hour in 12 hour format.
 * @param  isAM      discerns if it's AM or PM
 * @return           the given hour in 24 hour format
 */
int format24Hour(int inputHour, bool isAM)
{
	if(!isAM) {
		return inputHour + 12;
	} else if (isAM && inputHour > 0 && inputHour < 12) {
		return inputHour; 
	} else {
		return 0;
	}
}