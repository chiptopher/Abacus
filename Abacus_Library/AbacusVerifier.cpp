/**
 * Filename:	AbacusVerifier.cpp
 * Author:		Christopher Boyer
 * 
 * Contains the implementation of the edge verifiers of the machine.
 */

#include "AbacusVerifier.h"
#include "Arduino.h"

#define ALARM_STATUS_POS 0
#define ENTER_POS 1
#define SET_TIME_POS 2
#define SET_ALARM_POS 3
#define AM_POS 4 /* used in main stage */
#define PM_POS 5 /* used in main stage */
#define ONE_POS 6
#define TWO_POS 7
#define THREE_POS 8
#define FOUR_POS 9
#define FIVE_POS 10
#define SIX_POS 11
#define SEVEN_POS 12
#define EIGHT_POS 13
#define NINE_POS 14
#define ZERO_POS 15
#define ALARM_TIME_POS 16
#define VALID_TIME_POS 17
#define CORRECT_ANSWER_POS 18

/**
 * Tells the status us the alarm on/off switch. A HIGH, 4];one, or true indicates
 * that the alarm is going to be off.
 * @param  information	used to determine if the alarm is on/off.
 * @return             	true if the alarm is off; false otherwise.
 */
bool noAlarmVerifier(int * information)
{
	return information[ALARM_STATUS_POS];
}

/**
 * Tells if the set time button has been pressed.
 * @param  information	used to determine if the set time button was pressed
 * @return             	true if set time button was pressed; false otherwise
 */
bool setTimeVerifier(int * information)
{
	return information[SET_TIME_POS];
}

/**
 * Tells if the set alarm button has been pressed.
 * @param  information	used to determine if the set alarm button was pressed
 * @return            	true if set alarm button was pressed; false otherwise
 */
bool setAlarmVerifier(int * information)
{
	return information[SET_ALARM_POS];
}

/**
 * Tells if it's time for the alarm to turn on.
 * @param  information	used to determine if the alarm should be on 
 * @return             	true if the the alarm should be on; false otherwise
 */
bool alarmTimeVerifier(int * information)
{
	return information[ALARM_TIME_POS];
}

/**
 * Tells if it's not time for the alarm to be going off.
 * @param  information	used to determine if the alarm shouldn't be one
 * @return            	true if alarm shouldn't be going off; false otherwise
 */
bool notAlarmTimeVerifier(int * information)
{
	return !information[ALARM_TIME_POS];
}

/**
 * Tells if the alarm should be turned on. False/LOW/0 means that the alarm
 * should be set to on.
 * @param  information	used to determine if the alam should be set to on
 * @return            	true if the alarm should be set to on; false otherwise
 */
bool alarmOnVerifier(int * information)
{
	return !information[ALARM_STATUS_POS];
}

/**
 * Tells if no numbers were pressed. Since it's any or the other, one being
 * on would set it to true. Then we return the opposite of that.
 * @param  information	used to determine if a button was pressed
 * @return             	true if no button was pressed; false otherwise
 */
bool noInputVerifier(int * information)
{
	return !(information[ONE_POS] 		| information[TWO_POS] 	| 
		     information[THREE_POS] 	| information[FOUR_POS] |
		     information[FIVE_POS] 		| information[SIX_POS] 	|  
		     information[SEVEN_POS] 	| information[EIGHT_POS]|
		     information[NINE_POS] 		| information[ZERO_POS]);
}

/**
 * Tells if the one button has been pressed.
 * @param  information	used to determin if the one button was pressed
 * @return            	true if one was pressed; false otherwise
 */
bool oneVerifier(int * information)
{
	return information[ONE_POS];
}

/**
 * Telss if the two button has been pressed.
 * @param  information	used to determine if the two button has been pressed
 * @return            	true if two was pressed; false otherwise
 */
bool twoVerifier(int * information)
{
	return information[TWO_POS];
}

/**
 * Tells if the three button has been pressed.
 * @param  information	used to determine if the three button has been pressed
 * @return            	true if three was pressed; false otherwise
 */
bool threeVerifier(int * information)
{
	return information[THREE_POS];
}

/**
 * Tells if the four button has been pressed.
 * @param  information	used to determine if the four button has been pressed
 * @return            	true if four was pressed; false otherwise
 */
bool fourVerifier(int * information)
{
	return information[FOUR_POS];
}

/**
 * Tells if the five button has been pressed.
 * @param  information	used to determine if the five button has been pressed
 * @return            	true if five was pressed; false otherwise
 */
bool fiveVerifier(int * information)
{
	return information[FIVE_POS];
}

/**
 * Tells if the six button has been pressed.
 * @param  information	used to determine if the six button has been pressed
 * @return            	true if six was pressed; false otherwise
 */
bool sixVerifier(int * information)
{
	return information[SIX_POS];
}

/**
 * Tells if the seven button has been pressed.
 * @param  information	used to determine if the seven button has been pressed
 * @return            	true if seven was pressed; false otherwise
 */
bool sevenVerifier(int * information)
{
	return information[SEVEN_POS];
}

/**
 * Tells if the eight button has been pressed.
 * @param  information	used to determine if the eight button has been pressed
 * @return            	true if eight was pressed; false otherwise
 */
bool eightVerifier(int * information)
{
	return information[EIGHT_POS];
}

/**
 * Tells if the nine button has been pressed.
 * @param  information	used to determine if the nine button has been pressed
 * @return            	true if nine was pressed; false otherwise
 */
bool nineVerifier(int * information)
{
	return information[NINE_POS];
}

/**
 * Tells if the zero button has been pressed.
 * @param  information	used to determine if the zero button has been pressed
 * @return            	true if zero was pressed; false otherwise
 */
bool zeroVerifier(int * information)
{
	return information[ZERO_POS];
}

/**
 * Tells if the current time being entered is a valid time to enter.
 * @param  information	used to determine if entered time is valid
 * @return            	true if the time is valid; false otherwise
 */
bool validTimeVerifier(int * information)
{
	return (information[AM_POS] | information[PM_POS]) & information[VALID_TIME_POS];
}

/**
 * Tells if the current time being enteres is not a valid time to enter.
 * @param  information	used to determine if the entered time is invalid
 * @return            	true if the time is invalid; false otherwise
 */
bool invalidTimeVerifier(int * information)
{
	return (information[AM_POS] | information[PM_POS]) & !information[VALID_TIME_POS];
}

/**
 * Tells if the input answer is not the correct answer.
 * @param  information	used to determine if the entered answer is wrong
 * @return             	true if the answer is invalid; false otherwise
 */
bool wrongAnswerVerifier(int * information)
{
	return information[ENTER_POS] & !information[CORRECT_ANSWER_POS];
}

/**
 * Tells if the input answer is the correct answer.
 * @param  information	used to determine if the entered answer is correct
 * @return            	true if the answer is valid; false otherwise
 */
bool correctAnswerVerifier(int * information)
{
	return information[ENTER_POS] & information[CORRECT_ANSWER_POS];
}