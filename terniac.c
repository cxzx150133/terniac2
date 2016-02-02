#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "instructions.h"
#include "ternutils.h"

#define WIDTH 12
#define RANGE 531441
#define STAHP 50

void dispTryte(int * tryte);
void load(int **mem, char * filename);
int execute(int **mem, int *pc, int flow, int *areg, int *breg, int *creg);

int itryte[12];

int intexp(int a, int b){
	int acc;
	assert(b>=0);
	if(0==b) return 1;
	acc=a;
	b--;
	while(b > 0){
		acc = acc*a;
		b--;
	}
	return acc;
}

/* Stores conversion result in global itryte[] */
void dec2tern(int num){
	int i;

	for(i=0;i<WIDTH;i++){
		itryte[i]=0;
	}

	/* Replaces some nasty spagetti */
	for(i=WIDTH-1;i>=0;i--){
		if(num<0){
			if(num<(-((intexp(3,(i))-1)/2))){
				itryte[i]=-1;
				num+=(int)intexp(3,i);
			}

		}else{
			if(num>((intexp(3,(i))-1)/2)){
				itryte[i]=1;
				num-=(int)intexp(3,i);
			}
		}
	}
	return;
}

/* Output helper */
char tern2ascii(int * tryte){
	int num;
	num=tern2dec(tryte);
	/* digits */
	if ((num >= 0) && (num <= 9)) return (char) (num+48);
	/* uppercase letters */
	else if ((num >= 10) && (num <= 35)) return (char) (num+55);
	else return '?';
}

/* Input helper */
int ascii2dec(char c){
	/* digits */
	if ((c >= 48) && (c <= 57)) return (c-48);
	else if ((c >= 65) && (c <= 75)) return (c-55);
	else return 42;
}

int main(int argc, char *argv[]){
	int i;
	int quit;
	int regdisp=0;
	int flow=0;
	int cycles=0;
	char c;

	/* One Program Counter */
	int lowmem = -((RANGE-1)/2);
	int * pc =malloc(sizeof(int));

	/* Three Registers */
	int areg[WIDTH];
	int breg[WIDTH];
	int creg[WIDTH];
	
	/* Verbosity settings */
	for (i = 1; i < argc; i++) {
		if (!strcmp(argv[i],"-flow"))
			flow = 1;
		else if (!strcmp(argv[i],"-reg"))
			regdisp = 1;
		else if (!strcmp(argv[i],"-help")){
			printhelp();
			return 0;
		}
	}

	/* mem is array of pointers to arrays of ints */
	int **mem = malloc(RANGE*sizeof(int*));
	for (i=0; i<RANGE; i++){
		mem[i] = malloc(WIDTH*sizeof(int));
	}

	*pc=0;

	/* Zero everything */
	zeroTryte(areg);
	zeroTryte(breg);
	zeroTryte(creg);
	for(i=0; i<RANGE; i++) zeroTryte(mem[i]);

	/* Load from file */
	puts("Loading Program");
	load(mem, argv[1]);

	/* This is the main loop */
	puts("Beginning execution...");
	while(*pc < RANGE){
		if(flow) printf("%d: ",*pc+lowmem);
		quit = (execute(mem, pc, flow, areg, breg, creg));
		cycles++;
		/* Input */
		if(tern2dec(mem[RANGE-4]) != 0) {
			system("/bin/stty raw");
			c = getchar();
			system("/bin/stty cooked");
			dec2tern((ascii2dec(c)));
			for(i=0;i<WIDTH;i++){
				mem[RANGE-3][i] = itryte[i];
			}
			zeroTryte(mem[RANGE-4]);
		}
		/* Output */
		if(tern2dec(mem[RANGE-2]) != 0) {
			c = tern2ascii(mem[RANGE-1]);
			printf("%c",c);
			zeroTryte(mem[RANGE-2]);
		}
		if(quit) break;
		assert(STAHP >= cycles);
	}

	printf("Execution finished after %d cycles.\n",cycles);
	if(regdisp){
		puts("REG A");
		dispTryte(areg);
		i=tern2dec(areg);
		printf("  %dd\n",i);
		puts("REG B");
		dispTryte(breg);
		i=tern2dec(breg);
		printf("  %dd\n",i);
		puts("REG C");
		dispTryte(creg);
		i=tern2dec(creg);
		printf("  %dd\n\n",i);
	}
	return 0;
}

/* This is the messy part. This is what happens when you copy-paste one thing that works to make the rest of it work */
int execute(int **mem, int *pc, int flow, int *areg, int *breg, int *creg){
	int i;
	int *tryte;
	tryte = mem[*pc];

	/* output instructions */
	if (tryte[0]==-1&&tryte[1]==-1&&tryte[2]==0){
		if(flow) puts("OUT A ");
		printf("PRINT %d\n",tern2dec(areg));
		*pc+=1;
	}else if (tryte[0]==0&&tryte[1]==-1&&tryte[2]==0){
		if(flow) puts("OUT B ");
		printf("PRINT %d\n",tern2dec(breg));
		*pc+=1;
	}else if (tryte[0]==1&&tryte[1]==-1&&tryte[2]==0){
		if(flow) puts("OUT C ");
		printf("PRINT %d\n",tern2dec(creg));
		*pc+=1;

	/* input instructions */
	}else if (tryte[0]==-1&&tryte[1]==0&&tryte[2]==1){
		if(flow) puts("IN A ");
		printf("IN A? :");
		scanf("%d",&i);
		dec2tern(i);
		for(i=0;i<WIDTH;i++){
			areg[i] = itryte[i];
		}
		*pc+=1;
	}else if (tryte[0]==0&&tryte[1]==0&&tryte[2]==1){
		if(flow) puts("IN B ");
		printf("IN B? :");
		scanf("%d",&i);
		dec2tern(i);
		for(i=0;i<WIDTH;i++){
			breg[i] = itryte[i];
		}
		*pc+=1;
	}else if (tryte[0]==1&&tryte[1]==0&&tryte[2]==1){
		if(flow) puts("IN C ");
		scanf("%d",&i);
		dec2tern(i);
		for(i=0;i<WIDTH;i++){
			creg[i] = itryte[i];
		}
		*pc+=1;

		*pc+=1;

	/* load instructions */
	}else if (tryte[0]==-1&&tryte[1]==-1&&tryte[2]==-1){
		if(flow) puts("LD A ");
		for(i=0;i<WIDTH;i++){
			areg[i]=mem[265720+tern2dec(mem[*pc+1])][i];
		}
		*pc+=2;
	}else if (tryte[0]==0&&tryte[1]==-1&&tryte[2]==-1){
		if(flow) puts("LD B ");
		for(i=0;i<WIDTH;i++){
			breg[i]=mem[265720+tern2dec(mem[*pc+1])][i];
		}
		*pc+=2;
	}else if (tryte[0]==1&&tryte[1]==-1&&tryte[2]==-1){
		if(flow) puts("LD C ");
		for(i=0;i<WIDTH;i++){
			creg[i]=mem[265720+tern2dec(mem[*pc+1])][i];
		}
		*pc+=2;

	/* increment instructions */
	}else if (tryte[0]==-1 &&tryte[1]==0 &&tryte[2]==-1){
		if(flow) puts("INC A");
		incrTryte(areg);
		*pc+=1;
	}else if (tryte[0]==0 &&tryte[1]==0 &&tryte[2]==-1){
		if(flow) puts("INC B");
		incrTryte(breg);
		*pc+=1;
	}else if (tryte[0]==1 &&tryte[1]==0 &&tryte[2]==-1){
		if(flow) puts("INC C");
		incrTryte(creg);
		*pc+=1;

	/* conditional skip instructions */
	}else if (tryte[0]==-1&&tryte[1]==1&&tryte[2]==-1){
		if(flow) puts("SKP A ");
		if(condSkip(areg)) *pc+=3;
		else *pc+=1;
	}else if (tryte[0]==0&&tryte[1]==1&&tryte[2]==-1){
		if(flow) puts("SKP B ");
		if(condSkip(breg)) *pc+=3;
		else *pc+=1;
	}else if (tryte[0]==1&&tryte[1]==1&&tryte[2]==-1){
		if(flow) puts("SKP C ");
		if(condSkip(creg)) *pc+=3;
		else *pc+=1;

	/* 3 ADD instructions */
	}else if(tryte[0]==-1 &&tryte[1]==1 &&tryte[2]==1){
		if(flow) puts("ADD A");
		addTryte(areg,mem[265720+tern2dec(mem[*pc+1])]);
		*pc+=2;

	}else if(tryte[0]==0 &&tryte[1]==1 &&tryte[2]==1){
		if(flow) puts("ADD B");
		addTryte(breg,mem[265720+tern2dec(mem[*pc+1])]);
		*pc+=2;

	}else if(tryte[0]==1 &&tryte[1]==1 &&tryte[2]==1){
		if(flow) puts("ADD C");
		addTryte(creg,mem[265720+tern2dec(mem[*pc+1])]);
		*pc+=2;

	/* 3 STO instructions */
	}else if(tryte[0]==-1 &&tryte[1]==-1 &&tryte[2]==1){
		if(flow) puts("STO A");
		STO(areg,mem[265720+tern2dec(mem[*pc+1])]);
		*pc+=2;
	}else if(tryte[0]==0 &&tryte[1]==-1 &&tryte[2]==1){
		if(flow) puts("STO B");
		STO(breg,mem[265720+tern2dec(mem[*pc+1])]);
		*pc+=2;
	}else if(tryte[0]==1 &&tryte[1]==-1 &&tryte[2]==1){
		if(flow) puts("STO C");
		STO(creg,mem[265720+tern2dec(mem[*pc+1])]);
		*pc+=2;

	/* HALT Instruction */
	}else if(tryte[0]==-1 &&tryte[1]==0 &&tryte[2]==0){
		if(flow) puts("HALT");
		return 1;

	/* Bitwise invert instructions */
	}else if(tryte[0]==-1 &&tryte[1]==1 &&tryte[2]==0){
		if(flow) puts("INV A");
		invTryte(areg);
		*pc+=1;
	}else if(tryte[0]==0 &&tryte[1]==1 &&tryte[2]==0){
		if(flow) puts("INV B");
		invTryte(breg);
		*pc+=1;
	}else if(tryte[0]==1 &&tryte[1]==1 &&tryte[2]==0){
		if(flow) puts("INV C");
		invTryte(creg);
		*pc+=1;

	/* Unconditional JP instruction */
	}else if (tryte[0]==1&&tryte[1]==0&&tryte[2]==0){
		if(flow) puts("JP");
		JP(mem[*pc+1],pc);
		
	/* NOOP instruction */
	}else if (tryte[0]==0&&tryte[1]==0&&tryte[2]==0){
		if(flow) puts("NOOP");
		*pc+=1;
	}else{ 
		if(flow){
			dispTryte(tryte);
			puts("?");
		}
		*pc+=1;
	}

	return 0;
}

