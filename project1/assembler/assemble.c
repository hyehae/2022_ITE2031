#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXLINELENGTH 1000

int readAndParse(FILE *, char *, char *, char *, char *, char *);
int isNumber(char *);
int getOffset(FILE *, char *);

int main(int argc, char *argv[])
{
	char *inFileString, *outFileString;
	FILE *inFilePtr, *outFilePtr;
	FILE *inFilePtrTmp;
	char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH], arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];

	if (argc != 3) {
		printf("error: usage: %s <assembly-code-file> <machine-code-file>\n", argv[0]);
		exit(1);
	}

	inFileString = argv[1];
	outFileString = argv[2];
	inFilePtr = fopen(inFileString, "r");
	inFilePtrTmp = fopen(inFileString, "r");

	if (inFilePtr == NULL) {
		printf("error in opening %s\n", inFileString);
		exit(1);
	}

	outFilePtr = fopen(outFileString, "w");

	if (outFilePtr == NULL) {
		printf("error in opening %s\n", outFileString);
		exit(1);
	}
	
	while(readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)) {
		if (strcmp(label, "")) {
			int dup = 0;
			char curLabel[MAXLINELENGTH];
			strcpy(curLabel, label);

			while(readAndParse(inFilePtrTmp, label, opcode, arg0, arg1, arg2)) {
				if(!strcmp(curLabel, label))
					dup++;
			}
			
			if(dup > 1) {
				printf("error: duplicated label\n");
				exit(1);
			}
		}
	}

	rewind(inFilePtr);
	rewind(inFilePtrTmp);
	
	int pc = 0;
	while(readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)) {
		int op, regA, regB, destReg, offset;
		int mc;

		if (!strcmp(opcode, "add") || !strcmp(opcode, "nor")) {
			if(!strcmp(opcode, "add")) 
				op = 0;	
			else
				op = 1;		
			
			regA = atoi(arg0);
			regB = atoi(arg1);
			destReg = atoi(arg2);
			offset = atoi(arg2);
		}

		else if (!strcmp(opcode, "lw") || !strcmp(opcode, "sw")) {
			if(!strcmp(opcode, "lw"))
				op = 2;
			else
				op = 3;

			regA = atoi(arg0);
			regB = atoi(arg1);
			if(isNumber(arg2)) {
				destReg = atoi(arg2);
				offset = atoi(arg2);
			}
			else {
				destReg = atoi(arg2);
				offset = getOffset(inFilePtrTmp, arg2);
			}
		}
				
		else if (!strcmp(opcode, "beq")) {
			op = 4;
			regA = atoi(arg0);
			regB = atoi(arg1);
			if (isNumber(arg2))
				offset = atoi(arg2);
			else
				offset = getOffset(inFilePtrTmp, arg2) - (pc+1);	
		}
	
		else if (!strcmp(opcode, "jalr")) {
			op = 5;
			regA = atoi(arg0);
			regB = atoi(arg1);
			destReg = 0;
			offset = 0;
		}

		else if (!strcmp(opcode, "halt") || !strcmp(opcode, "noop")) {
			if(!strcmp(opcode, "halt"))
				op = 6;
			else 
				op = 7;

			regA = 0;
			regB = 0;
			destReg = 0;
			offset = 0;
		}

		else if (!strcmp(opcode, ".fill")) {
			if (isNumber(arg0)) 			
				mc = atoi(arg0);
			else 
				mc = getOffset(inFilePtrTmp, arg0);
				
			fprintf(outFilePtr, "%d\n", mc);
			continue;
		}

		else {
			printf("error: unrecognized opcode\n");
			printf("%s\n", opcode);
			exit(1);
		}

		if (offset < -32768 || offset > 32767) {
			printf("error: offsetField out of range\n");
			exit(1);
		}

		if (offset < 0)
			offset += 65536;

		mc = (op<<22) + (regA<<19) + (regB<<16) + offset;
		fprintf(outFilePtr, "%d\n", mc);

		pc++;
	}



//	/* here is an example for how to use readAndParse to read a line from in FilePtr */
//	if (! readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2) ) {
//		/* reached end of file */
//	}
//
//	/* this is how to rewind the file ptr so that you start reading from the beginning of the file */
//	rewind(inFilePtr);
//
//	/* after doing a readAndParse, you may want to do the following to test the opcode */
//	if (!strcmp(opcode, "add")) {
//		/* do whatever you need to do for opcode "add" */
//	}

	fclose(inFilePtr);
	fclose(inFilePtrTmp);
	fclose(outFilePtr);
	exit(0);
}

/*
* Read and parse a line of the assembly-language file. Fields are returned
* in label, opcode, arg0, arg1, arg2 (these strings must have memory already
* allocated to them). *
* Return values:
* 0 if reached end of file
* 1 if all went well *
* exit(1) if line is too long.
*/

int readAndParse(FILE *inFilePtr, char *label, char *opcode, char *arg0, char *arg1, char *arg2)
{
	char line[MAXLINELENGTH];
	char *ptr = line;

	/* delete prior values */
	label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';

	/* read the line from the assembly-language file */
	if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL) {
		/* reached end of file */
		return(0);
	}

	/* check for line too long (by looking for a \n) */
	if (strchr(line, '\n') == NULL) {
		/* line too long */
		printf("error: line too long\n");
		exit(1);
	}

	/* is there a label? */
	ptr = line;

	if (sscanf(ptr, "%[^\t\n\r ]", label)) {
		/* successfully read label; advance pointer over the label */
		ptr += strlen(label);
	}
	/*
	* Parse the rest of the line. Would be nice to have real regular
	* expressions, but scanf will suffice.
	*/
	sscanf(ptr, "%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]", opcode, arg0, arg1, arg2);
	return(1);
}

int isNumber(char *string)
{ /* return 1 if string is a number */
	int i;
	return( (sscanf(string, "%d", &i)) == 1);
}

int getOffset(FILE *file, char *findArg) {
	char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH], arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];
	rewind(file);

	int index = 0;
	while(readAndParse(file, label, opcode, arg0, arg1, arg2)) {
		if (!strcmp(label, findArg)) {
			return index;
		}
		index++;
	}

	printf("error: label undefined\n");
	printf("%s\n", findArg);
	exit(1);
}

