/**
 * Filename:	AbacusVerifier.h
 * Author:		Christopher Boyer
 * 
 * Contains the defintions of the edge verifiers of the machine.
 */

#ifndef ABACUS_VERIFIER
#define ABACUS_VERIFIER

#include <stdbool.h>

/* Verifer Count: 22 */

bool noAlarmVerifier(int * information);
bool setTimeVerifier(int * information);
bool setAlarmVerifier(int * information);
bool alarmTimeVerifier(int * information);
bool notAlarmTimeVerifier(int * information);
bool stillNoAlarmVerifier(int * information);
bool alarmOnVerifier(int * information);
bool noInputVerifier(int * information);
bool oneVerifier(int * information);
bool twoVerifier(int * information);
bool threeVerifier(int * information);
bool fourVerifier(int * information);
bool fiveVerifier(int * information);
bool sixVerifier(int * information);
bool sevenVerifier(int * information);
bool eightVerifier(int * information);
bool nineVerifier(int * information);
bool zeroVerifier(int * information);
bool validTimeVerifier(int * information);
bool invalidTimeVerifier(int * information);
bool wrongAnswerVerifier(int * information);
bool correctAnswerVerifier(int * information);

#endif