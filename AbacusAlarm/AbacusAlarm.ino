/**
 * Filename: AbacusAlarm.ino
 * 
 * Control circuit for the speaker. Input is recieved from the main
 * mictrcontroller to dictate if a "success jingle," an "incorrect
 * blast," an on or an off noise should be played.
 * 
 * @author Christopher Boyer
 */

#include "AbacusAlarmState.h"

/* Discerns when to overwrite the current state with a new state */
bool stateHeirarchy [4][4] = {

    {true,  false,  false,  false },
    {true,  true,   false,  false },
    {true,  true,   true,   false },
    {true,  true,   true,   true }

};

AlarmState currentState_;

#define BUZZER_PIN 0    /* the pin connected to the buzzer */
#define A_I_PIN    2    /* the pin connected to the first input */
#define B_I_PIN    1    /* the pin connected to the second input */


void setup()
{
    randomSeed(analogRead(1));
    currentState_ = ALARM_OFF;
    pinMode(BUZZER_PIN, OUTPUT);
    digitalWrite(BUZZER_PIN, LOW);
    pinMode(A_I_PIN,   INPUT);
    pinMode(B_I_PIN,   INPUT);

    delay(2000);
}

void loop()
{
    currentState_ = updateState(currentState_, getCurrentState());
    switch (currentState_)
    {
       case ALARM_OFF:    alarmOffAction(currentState_);    break;
       case ALARM_GO:     alarmGoAction(currentState_);     break;
       case ALARM_BLAST:  alarmBlastAction(currentState_);  break;
       case ALARM_JINGLE: alarmJingleAction(currentState_); break; 
    }
}

AlarmState getCurrentState()
{
    int aStatus = digitalRead(A_I_PIN);
    int bStatus = digitalRead(B_I_PIN);

    if (!aStatus && !bStatus) {
        return ALARM_OFF;        
    } else if (aStatus && bStatus) {
        return ALARM_GO;
    } else if (!aStatus && bStatus) {
        return ALARM_BLAST;
    } else {
        return ALARM_JINGLE;
    }
}

/**
 * Doesn't do anything ebcause the alarm is off.
 * @param currentState  the state of the machine at the time of function execution
 */
void alarmOffAction(AlarmState currentState)
{
    currentState_ = ALARM_OFF;
}

/**
 * Plays a short alarm blast to let the user know the action they did was wrong.
 * @param currentState  the state of the machine at the time of function execution
 */
void alarmBlastAction(AlarmState currentState)
{
    alarmHigh(currentState, 2000);
    alarmLow(currentState, 1000);
    digitalWrite(BUZZER_PIN, LOW);
    currentState_ = ALARM_OFF;
}

/**
 * Plays a jingle to let the user know that they did something right.
 * @param currentState  the state of the machine at the time of function execution
 */
void alarmJingleAction(AlarmState currentState)
{
    #define NOTE1 175
    #define NOTE2 150
    #define NOTE3 100
    #define NOTE4 40
    #define REST  25
    alarmHigh(currentState, NOTE1);
    alarmLow(currentState, REST);
    alarmHigh(currentState, NOTE2);
    alarmLow(currentState, REST);
    alarmHigh(currentState, NOTE3);
    alarmLow(currentState, REST);
    alarmHigh(currentState, NOTE4);
    alarmLow(currentState, REST);
    alarmHigh(currentState, NOTE1);
    alarmLow(currentState, REST);
    alarmHigh(currentState, NOTE4);
    alarmLow(currentState, REST);
    digitalWrite(BUZZER_PIN, LOW);
    currentState_ = ALARM_OFF;
}

/**
 * Play the alarm sound. The alarm sound is of a random length of .25 adn 2 seconds long
 * with a 1 second delay between all sounds.
 * @param currentState  the state of the machine at the time of function execution
 */
void alarmGoAction(AlarmState currentState)
{
    alarmHigh(currentState, random(500, 2500));
    alarmLow(currentState, 500);
    digitalWrite(BUZZER_PIN, LOW);
    currentState_ = ALARM_OFF;
}

/**
 * Sed the speaker to HIGH for the given amount of time and while the current state
 * is the same as the state of the machine.
 * @param currentState  the state of the machine at the time of function execution
 * @param soundTime     the amound of them that the alarm should be on
 */
void alarmHigh(AlarmState currentState, unsigned long soundTime)
{
    unsigned long endTime = millis() + soundTime;
    while (millis() < endTime && currentState == updateState(currentState, getCurrentState()))
    {
        digitalWrite(BUZZER_PIN, HIGH);
    }
}

/**
 * Sed the speaker to LOW for the given amount of time and while the current state
 * is the same as the state of the machine.
 * @param currentState  the state of the machine at the time of function execution
 * @param delayTime     the ammount of time that the alarm should be off
 */
void alarmLow(AlarmState currentState, unsigned long delayTime)
{
    unsigned long endTime = millis() + delayTime;
    while (millis() < endTime && currentState == getCurrentState())
    {
        digitalWrite(BUZZER_PIN, LOW);
    }
}

/**
 * Update the current state with the new state according to the state hierarchy table.
 * @param currentState  the working state of the machine
 * @param newState      the updated state of the machine
 */
AlarmState updateState(AlarmState currentState, AlarmState newState)
{
    return (stateHeirarchy[currentState][newState]) ? newState : currentState;
}





