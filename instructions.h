#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#define WIDTH 12
#define RANGE 531441

int condSkip(int * tryte);
void invTryte(int * tryte);
void JP(int *tryte, int *pc);
void STO(int * reg, int * mem);
void incrTryte(int * tryte);
void addTryte(int * reg, int * tryte);

#endif

