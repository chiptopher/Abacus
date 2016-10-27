/**
 * Filename:	GenerateEquation.cpp
 * Author:		Christopher Boyer
 * 
 * Contains implementation of methods used to generate equation and defiphering
 * their answers.
 */


#include <math.h>
#include <stdbool.h>

#include <stdio.h>
#include <stdlib.h>

#define NUMBER_COUNT 6 /* the math equation will have six digits */
#define EQUATION_SIZE 12 /* eleven numbers and operators and null terminator */
#define OPERATION_COUNT 5 /* the number of operations is NUMBER_COUNT minus 1 */

/*
 * Defines a type containing information about a value. Used in getting
 * the reuslt of a randomly generated equation.
 */
char equation[EQUATION_SIZE];

typedef struct valueNode {

	int identifier;
	double value;

} ValueNode;

/* defines a type that points to an operation function */
typedef void (* OperationPointer) (ValueNode * firstValueNode, 
								   ValueNode * secondValueNode,
		 						   ValueNode ** allValueNodePTRs,
		 						   int valueNodeCount);

/*
 * Define a type containing information about an operation. Used in 
 * getting the result of a randomly generated equation
 */ 
typedef struct operationNode {

	OperationPointer operation;
	char operationCharacter;
	ValueNode * firstValueNode;
	ValueNode * secondValueNode;

} OperationNode;

/**
 * Determines if the given character is a multiplication or division sign
 * @param  inputCharacter the character in question
 * @return                true if the cahracter is mult/div; false otherwise
 */
bool isMultiplicationOrDivision(char inputCharacter)
{
	return (inputCharacter == '*' || inputCharacter == '/');
}

/**
 * Determines if the given character is an addition or subtraction sign
 * @param  inputCharacter the character in question
 * @return                true if the character is add/sub; false otherwise
 */
bool isAdditionOrSubtraction(char inputCharacter)
{
	return (inputCharacter == '+' || inputCharacter == '-');
}

/**
 * Determines if the given character is an operation sign
 * @param  inputCharacter the character in question
 * @return                ture if the character is an operator; false otherwise
 */
bool isOperator(char inputCharacter)
{
	return isAdditionOrSubtraction(inputCharacter) || 
		   isMultiplicationOrDivision(inputCharacter);
}

/**
 * Update the given array of ValueNodes with the value and identifier of
 * firstValueNode. All ValueNodes with the same identifier as firstValueNode
 * or secondValueNode are replaced.
 * ValueNodes 
 * @param firstValueNode   the first given ValueNode
 * @param secondValueNode  the second given ValueNode
 * @param allValueNodePTRs the array of ValueNodes
 * @param valueNodeCount   the number of values in ValueNode
 */
void updateValueNodes(ValueNode * firstValueNode, 
					  ValueNode * secondValueNode,
					  ValueNode ** allValueNodePTRs,
					  int valueNodeCount)
{
	
	double firstNodeValue = firstValueNode->value;
	int firstNodeIdentifier = firstValueNode->identifier;
	int secondNodeIdentifier = secondValueNode->identifier;

	int i;
	for (i = 0; i < valueNodeCount; i++)
	{
		int currentIdentifier = allValueNodePTRs[i]->identifier;
		if (currentIdentifier == firstNodeIdentifier || 
			currentIdentifier == secondNodeIdentifier)
		{
			allValueNodePTRs[i]->value = firstNodeValue;
			allValueNodePTRs[i]->identifier = firstNodeIdentifier;
		}
	}
}

/**
 * Add the values of the first and second ValueNode, and then update all the
 * array of ValueNodes accordingly.
 * @param firstValueNode   the first given ValueNode
 * @param secondValueNode  the second given ValueNode
 * @param allValueNodePTRs the array of ValueNodes
 * @param valueNodeCount   the number of values in ValueNode
 */
void add(ValueNode * firstValueNode, 
		 ValueNode * secondValueNode,
		 ValueNode ** allValueNodePTRs,
		 int valueNodeCount)
{
	firstValueNode->value = firstValueNode->value + secondValueNode->value;

	updateValueNodes(firstValueNode, secondValueNode, 
					 allValueNodePTRs, valueNodeCount);

}

/**
 * Subtract the values of the first and second ValueNode, and then update all the
 * array of ValueNodes accordingly.
 * @param firstValueNode   the first given ValueNode
 * @param secondValueNode  the second given ValueNode
 * @param allValueNodePTRs the array of ValueNodes
 * @param valueNodeCount   the number of values in ValueNode
 */
void subtract(ValueNode * firstValueNode,
			  ValueNode * secondValueNode,
			  ValueNode ** allValueNodePTRs,
			  int valueNodeCount)
{
	firstValueNode->value = firstValueNode->value - secondValueNode->value;
	updateValueNodes(firstValueNode, secondValueNode, 
					 allValueNodePTRs, valueNodeCount);

}

/**
 * Multiply the values of the first and second ValueNode, and then update all
 * the array of ValueNodes accordingly.
 * @param firstValueNode   the first given ValueNode
 * @param secondValueNode  the second given ValueNode
 * @param allValueNodePTRs the array of ValueNodes
 * @param valueNodeCount   the number of values in ValueNode
 */
void multiply(ValueNode * firstValueNode,
			  ValueNode * secondValueNode,
			  ValueNode ** allValueNodePTRs,
			  int valueNodeCount)
{
	firstValueNode->value = firstValueNode->value * secondValueNode->value;
	updateValueNodes(firstValueNode, secondValueNode, 
					 allValueNodePTRs, valueNodeCount);

}

/**
 * Divide the values of the first and second ValueNode, and then update all the
 * array of ValueNodes accordingly.
 * @param firstValueNode   the first given ValueNode
 * @param secondValueNode  the second given ValueNode
 * @param allValueNodePTRs the array of ValueNodes
 * @param valueNodeCount   the number of values in ValueNode
 */
void divide(ValueNode * firstValueNode,
			ValueNode * secondValueNode,
			ValueNode ** allValueNodePTRs,
			int valueNodeCount)
{
	double firstValue = firstValueNode->value;
	double secondValue = secondValueNode->value;
	double finalValue = firstValue / secondValue;
	firstValueNode->value = finalValue;
	updateValueNodes(firstValueNode, secondValueNode, 
					 allValueNodePTRs, valueNodeCount);

}

/**
 * Get a function pointer to one of the operation functions
 * @param  inputCharacter		the char determining the operation
 * @return               		the function pointer to the desired operation
 */
OperationPointer getOperationPointer(char inputCharacter)
{
	switch (inputCharacter)
	{
		case '+': return add;
		case '-': return subtract;
		case '*': return multiply;
		case '/': return divide;
	}
}

/**
 * Sor the given collection of OperationNodes that is of the given size
 * @param operations     the OperationNodes to be sorted
 * @param operationsSize the size of the collection of OperationNodes
 */
void sortOperations(OperationNode * operations, int operationsSize)
{
	/* current position working from the end to the sorted position */
	int i = operationsSize-1;

	int endSortedPosition = 0;
	OperationNode tempOperationNode;
	
	/* 
	 * inclues sorted portion in case the first position was multiplication
	 * or division
	 */
	while (i >= endSortedPosition)
	{
		/*
		 * If the operation at the given position is mulitplication or
		 * division, move it to the first position, and move everything else
		 * back a position. The sorted portion is also incremented because the
		 * operations moved to the beginning don't need to be updated agian.
		 * Don't increment the current position becuase when everything gets
		 * moved back, another multiplication or division might have been
		 * pushed into the current position.
		 */
		if (isMultiplicationOrDivision(operations[i].operationCharacter))
		{
			tempOperationNode = operations[i];
			endSortedPosition++;

			int j;/* the current position sorting from i to the beginning */
			for (j = i; j > 0; j--)
			{
				operations[j] = operations[j-1];
			}
			operations[0] = tempOperationNode;
		}
		else
		{
			i--;
		}
	}
}

/**
 * Get the result of the given equation in string form.
 * @param  mathEquation		the string that a result is desire of
 * @return              	the result of the given equation
 */
long getResult(char * mathEquation)
{
	OperationNode operations[OPERATION_COUNT];
	ValueNode numbers[NUMBER_COUNT];
	ValueNode * numberPTRs[NUMBER_COUNT];

	int i;
	for (i = 0; i < NUMBER_COUNT; i++)
	{
		numberPTRs[i] = &numbers[i];
	}

	int equationPos, numbersFound = 0, operationsFound = 0;
	for (equationPos = 0; equationPos < EQUATION_SIZE-1; equationPos++)
	{
		char currentChar = mathEquation[equationPos];
		if (!isOperator(currentChar))
		{
			// TODO make a setter for ValueNode
			numbers[numbersFound].value = currentChar - '0';
			numbers[numbersFound].identifier = numbersFound;
			numbersFound++;
		}
		else
		{
			// TODO make a setter for OperationNode
			operations[operationsFound].operation = 
											getOperationPointer(currentChar);
			operations[operationsFound].operationCharacter = currentChar;
			operations[operationsFound].firstValueNode = 
											numberPTRs[numbersFound-1];
			operations[operationsFound].secondValueNode = 
											numberPTRs[numbersFound];
			operationsFound++;
		}
	}

	sortOperations(operations, OPERATION_COUNT);

	double result = 0;
	for (i = 0; i < OPERATION_COUNT; i++)
	{
		operations[i].operation(operations[i].firstValueNode,
										 operations[i].secondValueNode,
										 numberPTRs, NUMBER_COUNT);
	}

	return lround(numbers[0].value);
}

/**
 * Generate a randomly created equation.
 * @return		a pointer to the randomly generated equation
 */
char * generateEquation()
{
	int operators[4] = {'+', '-', '*', '/'}; /* operation characters */

	equation[0] = ((rand() % 8) + 1 ) + '0'; /* random value between 1 and 9 */
	equation[11] = '\0';

	int equationPos = 1;
	while(equationPos < EQUATION_SIZE-1)
	{
		equation[equationPos] = operators[rand()%4];
		equationPos++;
		equation[equationPos] = ((rand() % 8) + 1 ) + '0';
		equationPos++;
	}

	return equation;
}

int getEquationSize()
{
	return EQUATION_SIZE;
}