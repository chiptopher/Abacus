/**
 * Filename:	AbacusAction.cpp
 * Author:		Christopher Boyer
 *
 * Contains implementations of the possible actions of the machine edges.
 */


#include "AbacusAction.h"
#include "AbacusAlarmState.h"
#include "AbacusMachineState.h"
#include "GenerateEquation.h"
#include "string.h"

#include <stdio.h>
#include <stdlib.h>


/**
 * Update the math current answer of the interim result.
 * @param InterimResult	 the current information on this stage of the loop
 * @param inputValue	 the input value from the user
 */
void updateMath(InterimResult * interimResult, long inputValue)
{
	long newValue = ((interimResult->currentAnswer * 10) + inputValue) % 1000000;
	interimResult->currentAnswer = newValue;

	int i = 15;
	while (newValue > 0 && i > 9)
	{
		interimResult->bottomMessage[i] = (newValue % 10) + '0';
		newValue = newValue / 10;
		i--;
	}
}

/**
 * Update the math current time being set of the interim result.
 * @param InterimResult	 the current information on this stage of the loop
 * @param inputValue	 the input value from the user
 */
void updateTime(InterimResult * interimResult, long inputValue)
{
	long newValue = ((interimResult->setTime * 10) + inputValue) % 10000;
	interimResult->setTime = newValue;

	interimResult->bottomMessage[7] = (newValue / 1000) + '0'; 
	interimResult->bottomMessage[8] = ((newValue % 1000) / 100) + '0'; 
	interimResult->bottomMessage[9] = ':';
	interimResult->bottomMessage[10] = ((newValue % 100) / 10)  + '0';
	interimResult->bottomMessage[11] = (newValue % 10) + '0';
	
}

/**
 * Ensure that no other larm is currently playing.
 * @param interimResult the current information on this stage of the loop
 */
void doClockAction (InterimResult * interimResult)
{
	interimResult->alarmState = ALARM_GO;
}

/**
 * Do nothing to chnage the current information.
 * @param interimResult	 the current information on this stage of the loop
 */
void doNothingAction(InterimResult * interimResult)
{
	interimResult->alarmState = ALARM_OFF;
}

/**
 * Prepare to set the time of the clock.
 * @param interimResult	 the current informtation on this stage of the loop
 */
void prepSetTimeAction(InterimResult * interimResult)
{
	strcpy(interimResult->topMessage, 	 "Setting clock   ");
	strcpy(interimResult->bottomMessage, "time:           ");

	interimResult->setTime = 0;
	interimResult->changeText = true;

}

/**
 * Prepare to set the alarm of the clock.
 * @param interimResult  the current informtation on this stage of the loop
 */
void prepSetAlarmAction(InterimResult * interimResult)
{
	strcpy(interimResult->topMessage,	 "Setting alarm   ");
	strcpy(interimResult->bottomMessage, "time:           ");

	interimResult->setTime = 0;
	interimResult->changeText = true;
}

/**
 * Set the time of the clock, play a noise on the alarm, and update the LCD message.
 * @param interimResult  the current information on this stage of the loop
 */
void setTimeAction(InterimResult * interimResult)
{
	interimResult->changeTime = true;

	if (interimResult->setAM) { 
		interimResult->isAM = true;
	} else { 
		interimResult->isAM = false;
	}

	strcpy(interimResult->topMessage, 	 "Have a wonderful");
	strcpy(interimResult->bottomMessage, "day, nerd!      ");

	interimResult->alarmState = ALARM_JINGLE;
	interimResult->changeText = true;
}

/**
 * Reset the time being entered to the clock back to zero. Make a sound on the
 * buzzer to let the user know they suck.
 * @param interimResult  the current informtaiton on this stage of the loop
 */
void resetAction(InterimResult * interimResult)
{
	interimResult->setTime = 0;

	strcpy(interimResult->bottomMessage, "time:           ");

	interimResult->alarmState = ALARM_BLAST;
	interimResult->changeText = true;
}

/**
 * Set the alarm of the clock, play a noise on the alarm, and update the LCD message.
 * @param interimResult	 the current information on this stage of the loop
 */
void setAlarmAction(InterimResult * interimResult)
{
	interimResult->alarmTime = interimResult->setTime;

	if (interimResult->setAM) { 
		interimResult->alarmAM = true;
	} else { 
		interimResult->alarmAM = false;
	}

	strcpy(interimResult->topMessage, 	 "Have a wonderful");
	strcpy(interimResult->bottomMessage, "day, nerd!      ");

	interimResult->alarmState = ALARM_JINGLE;
	interimResult->changeText = true;
}

/**
 * Start the alarm sound, and prepare for the answer to be entered.
 * @param interimResult  the current information on this stage of the loop
 */
void startAlarmAction(InterimResult * interimResult)
{
	interimResult->alarmState = ALARM_GO;
	strcpy(interimResult->equation, generateEquation());
	interimResult->answer = getResult(interimResult->equation);
	strcpy(interimResult->topMessage, 	"Q:              ");
	strcpy(interimResult->bottomMessage,"A:              ");

	interimResult->changeText = true;


	int i;
	int startingPosition = 3;
	int equationSize = getEquationSize();

	for (i = 0; i < equationSize-1; i++)
	{
		interimResult->topMessage[i+startingPosition] = interimResult->equation[i];
	}

	interimResult->currentAnswer = 0;
}

/**
 * Reset the currently enterd answer to zero, and update the LCD display.
 * @param interimResult  the current information on this stage of the loop
 */
void resetMathAction(InterimResult * interimResult)
{
	interimResult->currentAnswer = 0;
	strcpy(interimResult->bottomMessage, "A:              ");

	interimResult->changeText = true;
}

/**
 * Set the alarm to be off, play a little jingle, and update the LCD display.
 * @param interimResult  the current information on this stage of the loop
 */
void turnOffAlarmAction(InterimResult * interimResult)
{
	interimResult->alarmState = ALARM_JINGLE;

	strcpy(interimResult->topMessage, 	 "Have a wonderful");
	strcpy(interimResult->bottomMessage, "day, nerd!      ");

	interimResult->changeText = true;
}

/**
 * Increase the target value by the entered value (one).
 * @param interimResult  the current information on this stage of the loop
 */
void oneAction(InterimResult * interimResult)
{
	MachineState machineState = interimResult->machineState;

	if (machineState == SET_TIME || machineState == SET_ALARM)
	{
		updateTime(interimResult, 1);
	}
	else if (machineState == ALARM_ON)
	{
		updateMath(interimResult, 1);
	}

	interimResult->changeText = true;
}

/**
 * Increase the target value by the entered value (two).
 * @param interimResult  the current information on this stage of the loop
 */
void twoAction(InterimResult * interimResult)
{
	MachineState machineState = interimResult->machineState;

	if (machineState == SET_TIME || machineState == SET_ALARM)
	{
		updateTime(interimResult, 2);
	}
	else if (machineState == ALARM_ON)
	{
		updateMath(interimResult, 2);
	}

	interimResult->changeText = true;
}

/**
 * Increase the target value by the entered value (three).
 * @param interimResult  the current information on this stage of the loop
 */
void threeAction(InterimResult * interimResult)
{
	MachineState machineState = interimResult->machineState;

	if (machineState == SET_TIME || machineState == SET_ALARM)
	{
		updateTime(interimResult, 3);
	}
	else if (machineState == ALARM_ON)
	{
		updateMath(interimResult, 3);
	}

	interimResult->changeText = true;
}

/**
 * Increase the target value by the entered value (four).
 * @param interimResult  the current information on this stage of the loop
 */
void fourAction(InterimResult * interimResult)
{
	MachineState machineState = interimResult->machineState;

	if (machineState == SET_TIME || machineState == SET_ALARM)
	{
		updateTime(interimResult, 4);
	}
	else if (machineState == ALARM_ON)
	{
		updateMath(interimResult, 4);
	}

	interimResult->changeText = true;
}

/**
 * Increase the target value by the entered value (five).
 * @param interimResult  the current information on this stage of the loop
 */
void fiveAction(InterimResult * interimResult)
{
	MachineState machineState = interimResult->machineState;

	if (machineState == SET_TIME || machineState == SET_ALARM)
	{
		updateTime(interimResult, 5);
	}
	else if (machineState == ALARM_ON)
	{
		updateMath(interimResult, 5);
	}

	interimResult->changeText = true;
}

/**
 * Increase the target value by the entered value (six).
 * @param interimResult  the current information on this stage of the loop
 */
void sixAction(InterimResult * interimResult)
{
	MachineState machineState = interimResult->machineState;

	if (machineState == SET_TIME || machineState == SET_ALARM)
	{
		updateTime(interimResult, 6);
	}
	else if (machineState == ALARM_ON)
	{
		updateMath(interimResult, 6);
	}

	interimResult->changeText = true;
}

/**
 * Increase the target value by the entered value (seven).
 * @param interimResult  the current information on this stage of the loop
 */
void sevenAction(InterimResult * interimResult)
{
	MachineState machineState = interimResult->machineState;

	if (machineState == SET_TIME || machineState == SET_ALARM)
	{
		updateTime(interimResult, 7);
	}
	else if (machineState == ALARM_ON)
	{
		updateMath(interimResult, 7);
	}	

	interimResult->changeText = true;
}

/**
 * Increase the target value by the entered value (eight).
 * @param interimResult  the current information on this stage of the loop
 */
void eightAction(InterimResult * interimResult)
{
	MachineState machineState = interimResult->machineState;

	if (machineState == SET_TIME || machineState == SET_ALARM)
	{
		updateTime(interimResult, 8);
	}
	else if (machineState == ALARM_ON)
	{
		updateMath(interimResult, 8);
	}	

	interimResult->changeText = true;
}

/**
 * Increase the target value by the entered value (nine).
 * @param interimResult  the current information on this stage of the loop
 */
void nineAction(InterimResult * interimResult)
{
	MachineState machineState = interimResult->machineState;

	if (machineState == SET_TIME || machineState == SET_ALARM)
	{
		updateTime(interimResult, 9);
	}
	else if (machineState == ALARM_ON)
	{
		updateMath(interimResult, 9);
	}	

	interimResult->changeText = true;
}

/**
 * Increase the target value by the entered value (zero).
 * @param interimResult  the current information on this stage of the loop
 */
void zeroAction(InterimResult * interimResult)
{
	MachineState machineState = interimResult->machineState;

	if (machineState == SET_TIME || machineState == SET_ALARM)
	{
		updateTime(interimResult, 0);
	}
	else if (machineState == ALARM_ON)
	{
		updateMath(interimResult, 0);
	}	

	interimResult->changeText = true;
}


