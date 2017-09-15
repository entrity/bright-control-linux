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
#define BRIGHTMAX 150
#define BRIGHTMIN 1

FILE * out;

int newValue; // value to be set

// Read current value from file
int readCurrentValue ();

void update(int newValue);

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
	if (argc <= 1) {
		printf("Usage: $0 <brightness>\n");
		printf("Current brightness: %d\n", readCurrentValue());
	}
	else {
		char opt;
		char isBatch = 0;
		int newValue = -1;
		int curValue = readCurrentValue();
		opterr = 0;
		while ((opt = getopt(argc, argv, "b")) != -1) {
			switch(opt) {
			case 'b': isBatch = 1; break;
			case '?':
				if (sscanf(argv[optind-1], "%d", &newValue))
					newValue += curValue;
				break;
			}
		}
		for (int i = optind; i < argc; i++) {
			if (strlen(argv[i]) == 1) {
				if (argv[i][0] == '+')
					newValue = curValue * 2;
				else if (argv[i][0] == '-')
					newValue = curValue / 2;
			}
			if (sscanf(argv[i], "%d", &newValue))
				if (argv[i][0] == '+')
					newValue += curValue;
		}
		if (newValue >= 0)
			update(newValue);
		if (isBatch)
			printf("%d\n", readCurrentValue());
		else
			printf("Brightness\t%d\n", readCurrentValue());
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
