/*
Oscar Hedblad
COP 3502, 0003
Program 3 (BigInt Multiplication)
Description: Same as Program 1, reads in file containing two integers
and uses multiplication to find their product. This program, however,
uses Linked List.

Modified version of Sean's Program One that he released on Canvas.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Mandatory struct provided by Professor Tappen */
typedef struct integer
{
	int digit;
	struct integer *next;
} BigInt;


/*Preconditions: p is a pointer to a big integer, stored in
    reverse order, least to most significant
    digit, with no leading zeros.
Postconditions: The big integer pointed to by p is printed out.
*/
void print(BigInt *p)
{
	if (p->next == NULL) {

		printf("%d", p->digit);
		return;
	}
	else {
		print(p->next);
		printf("%d", p->digit);
	}
}

/*
Preconditions: the first parameter is string that stores
    only contains digits, doesn't start with
    0, and is 10000 or fewer characters long.
Postconditions: The function will read the digits of the
    large integer character by character,
    convert them into integers and place them in
    nodes of a linked list. The pointer to the
    head of the list is returned.
*/
BigInt* makeBigInt (char* stringInt){

	int i = 0;

/* Creates head and malloc (store space for typedef struct BigInt) */
	BigInt *head = malloc(sizeof(BigInt));
	head->next = NULL;

/* Creates length variable and sets it equal to the length of the string */
	int length = strlen(stringInt);

/* Sets head->digit to final number in the character array */
	head->digit = stringInt[length - 1] - '0';

/* Creates help pointer and sets it equal to head */
	BigInt *help = head;

/* Creates a new node */
	for (i = 0; i<length - 1; i++){

		BigInt *node = malloc(sizeof(BigInt));
		node->next = NULL;
		help->next = node;
		help = help->next;
		node->digit = stringInt[length - 2 - i] - '0';
	}

	return head;
}

/* Function that checks if it is zero */
int isZero(struct integer *p)
{
	return (p->digit == 0 && p->next == NULL);
}



/* Gets the length of the Linked List */
int getLength (BigInt *p) {

	int counter = 0;

	while (p != NULL) {
		counter++;
		p = p->next;
	}

	return counter;
}

/*
Preconditions: p and q are pointers to big integers,
    stored in reverse order, least to most
    significant digit, with no leading zeros.
Postconditions: A new big integer is created that stores
    the product of the integers pointed to by
    p and q and a pointer to it is returned.
*/
BigInt *multiply(BigInt *p, BigInt *q)
{
/* Declaration of Variables */
	int i, j, carry = 0;
	int length1 = getLength(p);
	int length2 = getLength(q);
	int ansLength = length1 + length2;

	BigInt * answer = NULL;

	BigInt tempAnswer, *pTemp, *qTemp, *prevPtr;

	pTemp = p;
	qTemp = q;

/* Terminates quickly in any case of multiplication by zero */
	if (isZero(p) || isZero(q))
		return makeBigInt("0");

/* Creates nodes for solution linked list */
	for(i=0; i< ansLength; i++) {
		BigInt *node = (BigInt *)malloc(sizeof(BigInt));
		node->next = answer;
		node->digit = 0;
		answer = node;
	}

	tempAnswer = answer;
    prevPtr = tempAnswer;
/* Perform multiplication; no attention to any addition overflows */
	for (i=0; i<length2; i++){
		for (j=0; j<length1; j++){

			tempAnswer->digit += pTemp->digit * qTemp->digit;
			pTemp = pTemp->next;
			tempAnswer = tempAnswer->next;
		}
		prevPtr = prevPtr->next;
		tempAnswer = prevPtr;
		qTemp = qTemp->next;
		pTemp = p;
	}

/* Sets the temp pointers back to the start of list */
	tempAnswer = answer;
	pTemp = p;
	qTemp = q;

/* Compensates for any addition overflows */
	for (i = carry = 0; i < ansLength; i++)
	{
		tempAnswer->digit += carry;
		carry = tempAnswer->digit / 10;
		tempAnswer->digit = tempAnswer->digit % 10;
		tempAnswer = tempAnswer->next;
	}

	if (carry >0 && tempAnswer != NULL)
        tempAnswer->digit = carry;

/* Set the tempAnswer pointer back to the start of list */
	tempAnswer = answer;

/* Checks and removes any leading zeros if they are present */
	while (tempAnswer->next->next != NULL)
		tempAnswer = tempAnswer->next;
	if (tempAnswer->next->digit == 0)
		tempAnswer->next = NULL;

	return answer;
}


/* Frees the memory, frees tmp */
struct integer *decapitate(BigInt *p)
{
	BigInt *tmp;
	tmp = p;

	while(p != NULL) {
		tmp = p;
		p = p->next;
		free(tmp);
	}

	return NULL;
}


/* Main method. Reads in file from  "bigint.txt" which lets the reader know how many operations to perform,
followed by the actual numbers to multiply. */
int main(void)
{
	int N, i;
/* Set the buffer to 10001 to allow it a number consisting of 10000 digits */
	char buffer[10001];
	BigInt *p, *q, *m;

	FILE *ifp = fopen("bigint.txt", "r");

	fscanf(ifp, "%d", &N);
	for (i = 1; i <= N; i++)
	{
		fscanf(ifp, "%s", buffer);
		p = makeBigInt(buffer);

		fscanf(ifp, "%s", buffer);
		q = makeBigInt(buffer);

		m = multiply(p, q);

/* Prints the problem # and the performed multiplication */
		printf("Problem #%d: ", i);
		print(p);
		printf(" * ");
		print(q);
		printf(" = ");
		print(m);
		printf("\n");


/* Manages the memory and destroys the list */
		p = decapitate(p);
		q = decapitate(q);
	}

/* Closes the file and returns zero */
	fclose(ifp);
	return 0;
}
