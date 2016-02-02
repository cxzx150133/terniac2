#ifndef TERNUTILS_H
#define TERNUTILS_H

#define WIDTH 12
#define RANGE 531441

/* ********************* */
/* Non-instruction Utils */
/* ********************* */
void zeroTryte(int * tryte);
int tern2dec(int * tryte);

/* ************ */
/* Output Utils */
/* ************ */
void dispTryte(int * tryte);
void printhelp();

/* ******************** */
/* Initialization Utils */
/* ******************** */
void load(int **mem, char * filename);

#endif

