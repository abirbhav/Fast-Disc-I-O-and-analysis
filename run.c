#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>
#include "part1.c"
#include "common.c"

// Initialize Values
void initialize(int argc, char** argv, int *readFlagPtr, int*writeFlagPtr, int *blockSizePtr, int* blockCountPtr, char ** fileNamePtr){
	//NOTE: According to prof file size will always be greater than block size*block count, so not checking
	int a;
	while((a = getopt(argc,argv,"rw"))!=-1){
		switch(a){
			case 'r' :
				*readFlagPtr=1;
				break;
			case 'w' :
				*writeFlagPtr=1;
				break;
			case '?' :
				printf("Unknown option character %c\n",optopt);
				exit(1);
				break;
		}
	}
	*fileNamePtr = argv[2];
	*blockSizePtr = atol(argv[3]);
	*blockCountPtr = atol(argv[4]);
}

int main(int argc,char** argv){
	int readFlag = 0, writeFlag = 0, blockSize = 0, blockCount = 0;
	char * fileName;
	initialize(argc, argv, &readFlag, &writeFlag, &blockSize, &blockCount, &fileName);

	unsigned int xor = 0;
	if(readFlag){
		xor = part1(blockSize, blockCount, fileName, 'r');
		printf("%08x\n", xor);
	}
	else if(writeFlag){
		part1(blockSize, blockCount, fileName, 'w');
	}
}