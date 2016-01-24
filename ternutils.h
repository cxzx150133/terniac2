#ifndef TERNUTILS_H
#define TERNUTILS_H

#define WIDTH 9
#define RANGE 19683

/* ********************* */
/* Non-instruction Utils */
/* ********************* */
void zeroTryte(int * tryte);
int tern2dec(int * tryte);

/* ************ */
/* Output Utils */
/* ************ */
void dispTryte(int * tryte);
void dump(int ** mem);
void printhelp();

/* ******************** */
/* Initialization Utils */
/* ******************** */
void load(int **mem, char * filename);

#endif

