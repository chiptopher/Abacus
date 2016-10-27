/**
 * Filename:    Abacus.h
 * Auhor:       Christopher Boyer
 *
 * Contains the definition of an edge as well as the array of edges that
 * run the machine.
 */

#ifndef ABACUS
#define ABACUS

#include <stdbool.h>
#include "AbacusAction.h"
#include "AbacusVerifier.h"
#include "AbacusMachineState.h"



typedef struct  {

    MachineState currentState;
    bool (* verifier) (int * information);
    void (* action) (InterimResult * interimResult);
    MachineState nextState;

} Edge;

static Edge edges[] = {
    
    // { DISPLAY_TIME,  &noAlarmVerifier,       &doNothingAction,       NO_ALARM },
    { DISPLAY_TIME, &setTimeVerifier,       &prepSetTimeAction,     SET_TIME },
    { DISPLAY_TIME, &setAlarmVerifier,      &prepSetAlarmAction,    SET_ALARM },
    { DISPLAY_TIME, &alarmTimeVerifier,     &startAlarmAction,      ALARM_ON },
    { DISPLAY_TIME, &notAlarmTimeVerifier,  &doNothingAction,       DISPLAY_TIME },

    // { NO_ALARM,      &noAlarmVerifier,       &doNothingAction,       NO_ALARM },
    // { NO_ALARM,      &alarmOnVerifier,       &doNothingAction,       DISPLAY_TIME },

    { SET_TIME,     &validTimeVerifier,     &setTimeAction,         DISPLAY_TIME },
    { SET_TIME,     &invalidTimeVerifier,   &resetAction,           SET_TIME },
    { SET_TIME,     &noInputVerifier,       &doNothingAction,       SET_TIME },
    { SET_TIME,     &oneVerifier,           &oneAction,             SET_TIME },
    { SET_TIME,     &twoVerifier,           &twoAction,             SET_TIME },
    { SET_TIME,     &threeVerifier,         &threeAction,           SET_TIME },
    { SET_TIME,     &fourVerifier,          &fourAction,            SET_TIME },
    { SET_TIME,     &fiveVerifier,          &fiveAction,            SET_TIME },
    { SET_TIME,     &sixVerifier,           &sixAction,             SET_TIME },
    { SET_TIME,     &sevenVerifier,         &sevenAction,           SET_TIME },
    { SET_TIME,     &eightVerifier,         &eightAction,           SET_TIME },
    { SET_TIME,     &nineVerifier,          &nineAction,            SET_TIME },
    { SET_TIME,     &zeroVerifier,          &zeroAction,            SET_TIME },

    { SET_ALARM,    &validTimeVerifier,     &setAlarmAction,        DISPLAY_TIME },
    { SET_ALARM,    &invalidTimeVerifier,   &resetAction,           SET_ALARM },
    { SET_ALARM,    &noInputVerifier,       &doNothingAction,       SET_ALARM },
    { SET_ALARM,    &oneVerifier,           &oneAction,             SET_ALARM },
    { SET_ALARM,    &twoVerifier,           &twoAction,             SET_ALARM },
    { SET_ALARM,    &threeVerifier,         &threeAction,           SET_ALARM },
    { SET_ALARM,    &fourVerifier,          &fourAction,            SET_ALARM },
    { SET_ALARM,    &fiveVerifier,          &fiveAction,            SET_ALARM },
    { SET_ALARM,    &sixVerifier,           &sixAction,             SET_ALARM },
    { SET_ALARM,    &sevenVerifier,         &sevenAction,           SET_ALARM },
    { SET_ALARM,    &eightVerifier,         &eightAction,           SET_ALARM },
    { SET_ALARM,    &nineVerifier,          &nineAction,            SET_ALARM },
    { SET_ALARM,    &zeroVerifier,          &zeroAction,            SET_ALARM },

    { ALARM_ON,     &wrongAnswerVerifier,   &resetMathAction,       ALARM_ON },
    { ALARM_ON,     &correctAnswerVerifier, &turnOffAlarmAction,    DISPLAY_TIME },
    { ALARM_ON,     &noInputVerifier,       &doClockAction,         ALARM_ON },
    { ALARM_ON,     &oneVerifier,           &oneAction,             ALARM_ON },
    { ALARM_ON,     &twoVerifier,           &twoAction,             ALARM_ON },
    { ALARM_ON,     &threeVerifier,         &threeAction,           ALARM_ON },
    { ALARM_ON,     &fourVerifier,          &fourAction,            ALARM_ON },
    { ALARM_ON,     &fiveVerifier,          &fiveAction,            ALARM_ON },
    { ALARM_ON,     &sixVerifier,           &sixAction,             ALARM_ON },
    { ALARM_ON,     &sevenVerifier,         &sevenAction,           ALARM_ON },
    { ALARM_ON,     &eightVerifier,         &eightAction,           ALARM_ON },
    { ALARM_ON,     &nineVerifier,          &nineAction,            ALARM_ON },
    { ALARM_ON,     &zeroVerifier,          &zeroAction,            ALARM_ON }

};

void writeTime(int * outputRegister, int currentDigit, int currentHour, int currentMinute);
void writeAlarmState(int * outputRegister, AlarmState alarmState);
void writeTimeLEDS(int * outputRegister, bool isAM);
void writeTimeLED(int * outputRegister, bool alarmAM);
int getEdgeCount();
int format12Hour(int inputHour);
int format24Hour(int inputHour, bool isAM);

#endif