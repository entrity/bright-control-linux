/*
  The build of this file should have the user id bit set and the owner should be root (and this must be on an unencrypted file system which honours the user id bit) because the system file it must modify is writable only by the root user.

  Usage:
    bright -- read current level
    bright <number> -- set bright to <number>
    bright +<number> -- increment bright by <number>
    bright -<number> -- decrement bright by <number>
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define FILENAME "/sys/class/backlight/intel_backlight/brightness"
#define BRIGHTMIN 1

FILE * out;

char isBatch = 0; // Just print numbers, no labels
int newValue; // value to be set

// Read current value from file
int readCurrentValue ();

void update(int newValue);

// Apply user input to current value arithmetically, return sum/difference
int computeNewValue (int prevValue, char operator, float operand)
{
	if (operand >= 0) {
		switch(operator) {
		case '+':
			return prevValue + operand;
		case 'x':
		case '*':
			return prevValue * operand;
		case '-':
			return prevValue - operand;
		case '/':
			return prevValue / operand;
		}
	} else {
		switch(operator) {
		case '+':
		case 'x':
		case '*':
			return prevValue * 2;
		case '-':
		case '/':
			return prevValue / 2;
		}
	}
}

int processArgs (int i, int argc, char * argv[])
{
	char operator[2]; // 1 for char, 1 for '\0'
	float operand;
	int curValue = readCurrentValue();
	for (; i < argc; i++) {
		if (0 == strcmp("-b", argv[i]))
			isBatch = 1;
		else if (2 == sscanf(argv[i], "%1[-+/x*]%f", &operator[0], &operand)) {
			curValue = computeNewValue(curValue, operator[0], operand);
		}
		else if (1 == sscanf(argv[i], "%1[-+/x*]", &operator[0])) {
			curValue = computeNewValue(curValue, operator[0], -1);
		}
		else if (1 == sscanf(argv[i], "%f", &operand)) {
			curValue = operand;
		}
	}
	return curValue;
}

// Main
int main (int argc, char * argv[])
{
	if (argc <= 1) {
		printf("Usage: $0 <brightness>\n");
		printf("Current brightness: %d\n", readCurrentValue());
	}
	else {
		// Apply non option args as arithemtic operations on the brightness value, set new value
		if (argc > 0) {
			int newValue = processArgs(0, argc, argv);
			update(newValue);
		}
		// Print current value (possibly updated)
		if (isBatch)
			printf("%d\n", readCurrentValue());
		else
			printf("\033[33mBrightness\t%d\033[0m\n", readCurrentValue());
	}
	return 0;
}

void update (int newValue)
{
	/* Require that newValue for bright never exceeds BRIGHTMAX and never exceeds BRIGHTMIN */
	if (newValue > BRIGHTMAX)
		newValue = BRIGHTMAX;
	else if (newValue < BRIGHTMIN)
		newValue = BRIGHTMIN;
	/* Write newValue for bright to file. The OS will observe the change and make the screen adjustment accordingly. */
	out = fopen(FILENAME, "w");
	if (!out) {
		fputs("Can't open output file " FILENAME "\n", stderr);
		exit(1);
	}
	if (fprintf(out, "%d", newValue) < 1) {
		fputs("Failed to write to output file " FILENAME "\n", stderr);
		exit(2);
	}
	fclose(out);
}

int readCurrentValue ()
{
	int curValue;
	FILE * in = fopen(FILENAME, "r");
	if (!in)
	{
		fputs("Can't open input file " FILENAME "\n", stderr);
		exit(3);
	}
	if (fscanf(in, "%d", &curValue) < 1)
	{
		fputs("Failed to read current value from input file " FILENAME "\n", stderr);
		exit(4);
	}
	fclose(in);
	return curValue;
}
