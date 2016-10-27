/**
 * Filename:	AbacusAction.h
 * Author:		Christoher Boyer
 *
 * Contains all definitions of the possible machine actions.
 */

#ifndef ABACUS_ACTION
#define ABACUS_ACTION

#include "AbacusInterim.h"

void doClockAction(InterimResult * interimResult);
void doNothingAction(InterimResult * interimResult);
void prepSetTimeAction(InterimResult * interimResult);
void prepSetAlarmAction(InterimResult * interimResult);
void setTimeAction(InterimResult * interimResult);
void resetAction(InterimResult * interimResult);
void setAlarmAction(InterimResult * interimResult);
void startAlarmAction(InterimResult * interimResult);
void resetMathAction(InterimResult * interimResult);
void turnOffAlarmAction(InterimResult * interimResult);
void oneAction(InterimResult * interimResult);
void twoAction(InterimResult * interimResult);
void threeAction(InterimResult * interimResult);
void fourAction(InterimResult * interimResult);
void fiveAction(InterimResult * interimResult);
void sixAction(InterimResult * interimResult);
void sevenAction(InterimResult * interimResult);
void eightAction(InterimResult * interimResult);
void nineAction(InterimResult * interimResult);
void zeroAction(InterimResult * interimResult);

#endif