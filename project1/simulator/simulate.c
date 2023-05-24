/* LC-2K Instruction-level simulator */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NUMMEMORY 65536 /* maximum number of words in memory */
#define NUMREGS 8 /* number of machine registers */
#define MAXLINELENGTH 1000

typedef struct stateStruct {
	int pc;
	int mem[NUMMEMORY];
	int reg[NUMREGS];
	int numMemory;
} stateType;

void printState(stateType *);
void getArgs(int, int *, int *, int *, int *);
int convertNum(int);

int main(int argc, char *argv[])
{
	char line[MAXLINELENGTH];
	stateType state;
	FILE *filePtr;

	if (argc != 2) {
		printf("error: usage: %s <machine-code file>\n", argv[0]);
		exit(1);
	}

	filePtr = fopen(argv[1], "r");

	if (filePtr == NULL) {
		printf("error: can't open file %s", argv[1]);
		perror("fopen");
		exit(1);
	}

	/* read in the entire machine-code file into memory */
	for (state.numMemory = 0; fgets(line, MAXLINELENGTH, filePtr) != NULL; state.numMemory++) {
		if (sscanf(line, "%d", state.mem+state.numMemory) != 1) {
			printf("error in reading address %d\n", state.numMemory);
			exit(1);
		}
		printf("memory[%d]=%d\n", state.numMemory, state.mem[state.numMemory]);
	}

	state.pc = 0;
	for(int i=0; i<NUMREGS; i++) {
		state.reg[i] = 0;
	}

	printState(&state);

	int halt = 0;
	int instcnt = 0;
	int opcode, arg0, arg1, arg2;
	while(1) {
		getArgs(state.mem[state.pc], &opcode, &arg0, &arg1, &arg2);
		state.pc++;
		instcnt++;

		switch(opcode) {
			case 0: //add
				state.reg[arg2] = state.reg[arg0] + state.reg[arg1];
				break;
			case 1: //nor
				state.reg[arg2] = ~ (state.reg[arg0] | state.reg[arg1]);
				break;
			case 2: //lw
				state.reg[arg1] = state.mem[state.reg[arg0] + arg2];
				break;
			case 3: //sw
				state.mem[state.reg[arg0] + arg2] = state.reg[arg1];
				break;
			case 4: //beq
				if (state.reg[arg0] == state.reg[arg1])
					state.pc += arg2;
				break;
			case 5: //jalr
				state.reg[arg1] = state.pc;
				state.pc = state.reg[arg0];
				break;
			case 6: //halt
				halt = 1;
				break;
			case 7: //noop
				break;
			default:
				printf("error: unrecognized opcode\n");
				exit(1);
		}

		if (halt)
			break;

		printState(&state);
	}

	printf("machine halted\n");
	printf("total of %d instructions executed\n", instcnt);
	printf("final state of machine:");
	printState(&state);

	fclose(filePtr);

	exit(0);
}

void printState(stateType *statePtr)
{
	int i;
	printf("\n@@@\nstate:\n");
	printf("\tpc %d\n", statePtr->pc);
	printf("\tmemory:\n");

	for (i=0; i<statePtr->numMemory; i++) {
		printf("\t\tmem[ %d ] %d\n", i, statePtr->mem[i]);
	}

	printf("\tregisters:\n");

	for (i=0; i<NUMREGS; i++) {
		printf("\t\treg[ %d ] %d\n", i, statePtr->reg[i]);
	}

	printf("end state\n");
}

void getArgs(int mc, int *opcode, int *arg0, int *arg1, int *arg2) {
	*opcode = (mc>>22) & 7;
	*arg0 = (mc>>19) & 7;
	*arg1 = (mc>>16) & 7;
	*arg2 = convertNum(mc & 0xffff);
}

int convertNum(int num)
{
	/* convert a 16-bit number into a 32-bit Linux integer */
	if (num & (1<<15)) {
		num -= (1<<16);
	}
	return(num);
}
