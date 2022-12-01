#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>
#include "common.c"
#include "part1.c"
#include "part2.c"

int main(int argc,char** argv){
    char* fileName = argv[1];
	long blockSize = atol(argv[2]);
    long blockCount = part2(blockSize, fileName);
    printf("%ld\n", blockCount);
    
    return 0;
}