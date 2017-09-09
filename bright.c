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

#define FILENAME "/sys/class/backlight/intel_backlight/brightness"
#define BRIGHTMAX 150
#define BRIGHTMIN 1

FILE * out;

int newValue; // value to be set

// Read current value from file
int readCurrentValue ();

// Apply user input to current value arithmetically, return sum/difference
int inferNewValueFromModifier (char * argv[], int multiplier);


int inferNewValueFromModifier (char * argv[], int multiplier)
{
	int modifier;
	sscanf(&argv[1][1], "%d", &modifier);
	return (modifier * multiplier) + readCurrentValue();
}



// Main
int main (int argc, char * argv[])
{
	int res;

	if (argc <= 1) {
		printf("Usage: $0 <brightness>\nCurrent brightness: ");
		printf("%d\n", readCurrentValue());
	}
	else {
		int argIntPart;
		sscanf(argv[1], "%d", &argIntPart);
		if (argv[1][0] == '-')
			if (strlen(&argv[1][1]))
				newValue = readCurrentValue() + argIntPart; // argIntPart will be negative if there was a minus on the command line
			else
				newValue = readCurrentValue() / 2;
		else if (argv[1][0] == '+')
			if (strlen(&argv[1][1]))
				newValue = readCurrentValue() + argIntPart;
			else
				newValue = readCurrentValue() * 2;
		else
			newValue = argIntPart;
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
		res = fprintf(out, "%d", newValue);
		if (res < 1) {
			fputs("Failed to write to output file " FILENAME "\n", stderr);
			exit(2);
		}
		fclose(out);
		fprintf(stdout, "Set brigtness to %d\n", newValue);
	}
}

int readCurrentValue ()
{
	int curValue, res;
	FILE * in = fopen(FILENAME, "r");
	if (!in)
	{
		fputs("Can't open input file " FILENAME "\n", stderr);
		exit(3);
	}
	res = fscanf(in, "%d", &curValue);
	if (res < 1)
	{
		fputs("Failed to read current value from input file " FILENAME "\n", stderr);
		exit(4);
	}
	fclose(in);
	return curValue;

}
