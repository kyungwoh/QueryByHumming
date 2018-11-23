#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
	int delta;
	int pitch;
} _note;

int getMinNoteDiff(char* fname1, char* fname2);
int getNoteDiff(_note note1[], _note note2[], int size, int loop_nums);
int power(int base, int n);
