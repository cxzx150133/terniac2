#include <stdio.h>
#include <assert.h>
#include "ternutils.h"

#define WIDTH 12
#define RANGE 531441

void zeroTryte(int * tryte);
int tern2dec(int * tryte);

void dispTryte(int * tryte);
void printhelp();

void load(int **mem, char * filename);

/* ********************* */
/* Non-instruction Utils */
/* ********************* */
void zeroTryte(int * tryte){
	int i;
	for(i=0; i<WIDTH; i++) tryte[i] = 0;
}

int tern2dec(int * tryte){
	int i;
	int placeval = 1;
	int accum = 0;

	for(i=0; i<WIDTH; i++){
		accum = accum + (tryte[i]*placeval);
		placeval = placeval*3;
	}
	return accum;
}

/* ************ */
/* Output Utils */
/* ************ */
void dispTryte(int * tryte){
	int i;
	for(i=0; i<WIDTH; i++){
		switch(tryte[i]){
		case 0:
			putchar('0');
			break;
		case -1:
			putchar('-');
			break;
		case 1:
			putchar('+');
			break;
		default:
			putchar(tryte[i]);
			break;
		}
	}
}

void printhelp(){
	puts("Specify input ternary executable as first argument");
	puts("Valid options are: -dump -flow -reg");
}

/* ******************** */
/* Initialization Utils */
/* ******************** */

void load(int **mem, char * filename){
	int i;
	FILE *f;
	f = fopen(filename, "r");
	assert(f != NULL);

	for(i=0; i<RANGE; i++){
		if(WIDTH != fscanf(f, "%d%d%d%d%d%d%d%d%d%d%d%d\n", &mem[i][0], &mem[i][1], &mem[i][2], &mem[i][3], &mem[i][4], &mem[i][5], &mem[i][6], &mem[i][7], &mem[i][8], &mem[i][9], &mem[i][10], &mem[i][11])) {
			/* Seems to prevent an error. Comment out for a good time. */
			zeroTryte(mem[i]);
		}
	}
}

