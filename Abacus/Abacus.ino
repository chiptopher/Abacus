/*
 * Filename: Abacus.ino
 * 
 * An alarm clock that, when the alarm goes off, displays a math problem that
 * must be solved before the alarm can be muted. The time and alarm can be set
 * by the user.
 * 
 * @author Christopher Boyer
 */

#include <LiquidCrystal.h>
#include <TimeLib.h>
#include <Abacus.h>

/* pins for the button matrix */
#define INPUT1 5
#define INPUT2 4
#define INPUT3 3
#define INPUT4 2

#define OUTPUT1 9
#define OUTPUT2 8
#define OUTPUT3 7
#define OUTPUT4 6

#define MILLIS_IN_MINUTE 60000

/* array of inputs and outputs */
int inputPins[] = { INPUT1, INPUT2, INPUT3, INPUT4 };
int outputPins[] = { OUTPUT1, OUTPUT2, OUTPUT3, OUTPUT4 };


int matrix[4][4] = {

    {6, 7, 8, 1},
    {9, 10, 11, 2},
    {12, 13, 14, 3},
    {4, 15, 5, 0}
    
};
 
/* pins for output */
const int latchPin_ = 11;
const int dataPin_ = 10;
const int clockPin_ = 12;

/* pins for the LCD screen */
const int registerSelectPin = A5;  /* Register select pin of LCD  */
const int enablePin = A4;          /* Enable pin of LCD           */
const int d1Pin = A3;              /* First data pin of LCD       */
const int d2Pin = A2;              /* Second data pin of LCD      */
const int d3Pin = A1;              /* Third data pin of LCD       */
const int d4Pin = A0;              /* Fourth data pin of LCD      */

long alarmTime_;
long setTime_;
long currentAnswer_;
long answer_;

bool alarmAM_;

AlarmState alarmState_;
MachineState machineState_;
MachineState prevMachineState_;

char equation_[12] = { '\0' };
char topMessage_[17] = { '\0' };
char bottomMessage_[17] = { '\0' };

unsigned long inputDelay_[16] = { 0 };

unsigned long alarmDelay;
bool alarmSolved;

int currentDigit_;

#define SOUNDS_DELAY 300 /* allow sounds to play for 300ms before writing the change */
bool soundPlaying;
unsigned long startDelay;

/* LCD object for using an LCD */
LiquidCrystal lcd(registerSelectPin, enablePin, d1Pin, d2Pin, d3Pin, d4Pin);


/**
 * Start the clock after having lost power. Clock begins at 12AM. The date
 * is set to January 1, 2000.
 */
void setup() 
{
    Serial.begin(9600);
    randomSeed(analogRead(0));
    
    pinMode(latchPin_, OUTPUT);
    pinMode(dataPin_, OUTPUT);
    pinMode(clockPin_, OUTPUT);

    pinMode(INPUT1, INPUT);
    pinMode(INPUT2, INPUT);
    pinMode(INPUT3, INPUT);
    pinMode(INPUT4, INPUT);
    pinMode(OUTPUT1, OUTPUT);
    pinMode(OUTPUT2, OUTPUT);
    pinMode(OUTPUT3, OUTPUT);
    pinMode(OUTPUT4, OUTPUT);

    soundPlaying = false;
    alarmSolved = false;

    lcd.begin(16,2);
    setTime(0, 0, 0, 1, 1, 2000);

    alarmTime_ = 0;
    setTime_ = 0;
    currentAnswer_ = 0;
    answer_ = 0;
    alarmAM_ = true;

    alarmState_ = ALARM_OFF;
    machineState_ = DISPLAY_TIME;

    alarmDelay = 0;

    int i;
    for (i = 0; i < 10; i++)
    {
        inputDelay_[i] = millis();
    }
    
    currentDigit_ = 0;

    strcpy(topMessage_,    "have a wonderful");
    strcpy(bottomMessage_, "day, nerd!      ");

    printTopMessage();
    printBottomMessage();
}

/**
 * Read all of the inputs. Based on those inputs, loop through the possible
 * edges based on the current inputs and complete the appropriate action, and
 * then output all the information to the appropriate displays.
 */
void loop() 
{
    int outputRegister[16] = { 0 };
    int inputRegister[19] = { 0 };
    InterimResult interimResult;
    InterimResult * interimResultPTR = &interimResult;
    
    readInputs(interimResultPTR, inputRegister);

    int i = 0;
    bool found = false;
    while(!found && i < getEdgeCount())
    {
        if (edges[i].currentState == machineState_ && edges[i].verifier(inputRegister))
        {
            found = true;
            edges[i].action(interimResultPTR);
            prevMachineState_ = machineState_;
            machineState_ = edges[i].nextState;
        }

        i++;
    }

    writeOutputs(interimResultPTR, outputRegister);
}
            
void readInputs(InterimResult * interimResult, int * inputRegister)
{
    int i = 0;
    for (i = 0; i < 19; i++)
    {
        inputRegister[i] = 0;
    }
    
    readButtonMatrix(inputRegister);
    validInput(inputRegister, inputDelay_);

    long currentTime = (format12Hour(hour()) * 100) + minute();

    
    
    if (currentTime == alarmTime_ && updateAlarmSolved())
    {
        inputRegister[16] = 1;
    }

    if (setTime_ >= 100 && setTime_<= 1260)
    {
        inputRegister[17] = 1;
    }

    if (currentAnswer_ == answer_)
    {
        inputRegister[18] = 1;
    }

    interimResult->alarmTime = alarmTime_;
    interimResult->setTime = setTime_;
    interimResult->answer = answer_;
    interimResult->currentAnswer = currentAnswer_;
    interimResult->isAM = isAM();
    interimResult->alarmAM = alarmAM_;
    interimResult->setAM = inputRegister[4];
    interimResult->changeTime = false;
    interimResult->changeText = false;
    interimResult->alarmState = alarmState_;
    interimResult->machineState = machineState_;
    interimResult->equation[11] = '\0';
    interimResult->topMessage[16] = '\0';
    interimResult->bottomMessage[17] = '\0';
    strcpy(interimResult->equation, equation_);
    strcpy(interimResult->topMessage, topMessage_);
    strcpy(interimResult->bottomMessage, bottomMessage_);
    
}

void writeOutputs(InterimResult * interimResult, int * outputRegister)
{
    AlarmState prevAlarmState = alarmState_;
    alarmTime_ = interimResult->alarmTime;
    setTime_ = interimResult->setTime;
    answer_ = interimResult->answer;
    currentAnswer_ = interimResult->currentAnswer;
    alarmAM_ = interimResult->alarmAM;
    alarmState_ = interimResult->alarmState;
    strcpy(equation_, interimResult->equation);
    strcpy(topMessage_, interimResult->topMessage);
    strcpy(bottomMessage_, interimResult->bottomMessage);
    
    if (interimResult->changeTime)
    {
        setTime(format24Hour(interimResult->setTime / 100, interimResult->setAM), 
                interimResult->setTime % 100, second(), 1, 1, 2000);
    }   
    
    
    writeTime(outputRegister, currentDigit_, format12Hour(hour()), minute());
    writeTimeLED(outputRegister, isAM());
    /* if there isn't enough of a delay when shifting out to the alarm pins, it won't render
     *  properly. It's mostly so the jingle actually plays. */
    if (soundPlaying)
    {
        if ((millis() - startDelay) > 300)
        {
            soundPlaying = false;
        }else
        {
            alarmState_ = prevAlarmState;
        }
    }
    
    if (prevAlarmState != alarmState_)
    {
        soundPlaying = true;
        startDelay = millis();
    }
    
    writeAlarmState(outputRegister, alarmState_);
    newShiftOut(dataPin_, clockPin_, latchPin_, outputRegister);

    currentDigit_++;
    if (currentDigit_ == 4) {
        currentDigit_ = 0;
    }

    if (interimResult->changeText)
    {
        printTopMessage();
        printBottomMessage();
    }

    /* Stop alarm from starting again for 60 seconds after the alarm
     *  is solved */
    if (prevMachineState_ == ALARM_ON && machineState_ == DISPLAY_TIME)
    {
        alarmDelay = millis();
        alarmSolved = true;
    }
    
}

void readButtonMatrix(int * inputRegister)
{
    for (int i = 0; i < 4; i++)
    {
        digitalWrite(outputPins[i], HIGH);
        for (int j = 0;  j < 4; j++)
        {
            if (digitalRead(inputPins[j]))
            {
                inputRegister[matrix[i][j]] = HIGH;
            }
        }
        digitalWrite(outputPins[i], LOW);
    }
}

/**
 * Shift the given array of data out to two output registers.
 * @param dataPin   the pin data is sent to the output register
 * @param clockPin  the clock pin of the output registers
 * @param latchPin  the latch pin of the output registers
 * @param dataArray the array of data sent to the output registers
 */
void newShiftOut(int dataPin, int clockPin, int latchPin, int dataArray[])
{
    digitalWrite(latchPin, 0);
    digitalWrite(dataPin, 0);
    digitalWrite(clockPin, 0);

    int i;
    for (i = 0; i < 16; i++)
    {
        digitalWrite(clockPin, 0);
        digitalWrite(dataPin, dataArray[i]);
        //register shifts bits on upstroke of clock pin  
        digitalWrite(clockPin, 1);
        //zero the data pin after shift to prevent bleed through
        digitalWrite(dataPin, 0);
    }
    digitalWrite(clockPin, 0);
    digitalWrite(latchPin, 1);
}

/**
 * Print the top message variable to the LCD.
 */
void printTopMessage()
{
    lcd.setCursor(0,0);
    lcd.print(topMessage_);
}

/**
 * Print the bottom message variable to the LCD.
 */
void printBottomMessage()
{
    lcd.setCursor(0,1);
    lcd.print(bottomMessage_);
}

/**
 * Determine if enough time has passed for the current input to be valid.
 * @param inputArray    the array of inputs
 * @param inputDelay    determines if enough time has passed since the last time
 *                      that input was pressed
 */
void validInput(int * inputArray, unsigned long * inputDelay)
{
    int i, startingPoint = 6;
    
    if ((millis() - inputDelay[0]) < 500)
    {
        inputArray[0] = 0;
    }
    else
    {
        inputDelay[0] = millis();
    }
    
    for (i = 0; i < 10; i++)
    {
        if ((millis() - inputDelay[i + startingPoint]) < 500)
        {
            inputArray[i + startingPoint] = 0;
        }
        else
        {
            inputDelay[i + startingPoint] = millis();
        }
    }
}

bool updateAlarmSolved()
{
    if ((millis() - alarmDelay) > MILLIS_IN_MINUTE)
    {
        alarmSolved = false;
    }

    return !alarmSolved;
}

