#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "common.c"
#include "part1.c"
#include "part2.c"

struct syscall{
    char * syscall;
    long loopCount;
    int fd;
} syscalls[3];

void initializeSyscalls(){
    syscalls[0].syscall = "lseek";
    syscalls[0].loopCount = 15000000;
    syscalls[1].syscall = "getpid";
    syscalls[1].loopCount = 15000000;
	syscalls[2].syscall = "stat";
    syscalls[2].loopCount = 15000000;
}

int main(int argc, char **argv)
{
    char *fileName = argv[1];
    long blockSize = 1;
    long blockCount = 1;
    double timeTaken = 0;
    double start, end;
    double fileSize = 0;

    //System Calls
    printf("Computing results for various system calls\n");
    
    initializeSyscalls();
    for(int sc = 0; sc < 3; sc++){
        printf("\nSys Call: %s\n", syscalls[sc].syscall);
        long loopCount = syscalls[sc].loopCount; //This is the total times we will run the system call
        if(syscalls[sc].syscall == "lseek"){
            int fd = open(fileName, O_RDONLY);
            start = now();
            for (long i = 0; i < loopCount; i++)
            {
                lseek(fd, 0, SEEK_SET);
            }
            end = now();
            close(fd);
        }
        else if(syscalls[sc].syscall == "getpid"){
            start = now();
            for (long i = 0; i < loopCount; i++)
            {
                getpid();
            }
            end = now();
        }
		else if(syscalls[sc].syscall == "stat"){
            struct stat fileStat;	
	        start=now();
            for (long i = 0; i < loopCount; i++)
            {
                stat(fileName, &fileStat);
            }
            end = now();
        }
        
        timeTaken = end - start;
        double syscallspeed = loopCount / (end - start);
        printf("Ran %ld times, Time Taken = %f secs, Speed (Sys Calls/sec) = %f \n", loopCount, end - start, syscallspeed);
        
    printf("\n-----------------------------------------------------------------------\n");
    }
    //Starting File I/O
    printf("\nStarting File I/O\n");
    printf("For each block size, we will first call part2 and get ideal block count to test on.\n");
    printf("Then we will (in a loop, %d times) call part1 (without clearing cache) to calculate speed and take avg.\n", TESTCOUNT);
    printf("Then we will (in a loop, %d times) clear cache and call part1 to calculate speed and take avg.\n", TESTCOUNT);
    //Run till blockSize = 128 MiB
    for (blockSize = 1; blockSize <= 128 * MiBinB; blockSize *= 2)
    {
        printf("\nComputing results for blockSize = %ld\n", blockSize);
        blockCount = part2(blockSize, fileName);
        fileSize = (blockSize * blockCount);
        fileSize/=MiBinB;
        printf("We got block count = %ld and file size = %f MiB\n", blockCount, fileSize);
        //Now we have block count to run our tests
        double totalTime = 0, avgTime = 0;
        int testCount = TESTCOUNT;
        double avgSpeed = 0;
        
        
        printf("Running without clearing cache %d times\n", TESTCOUNT);
        while(testCount --){
            start = now();
            part1(blockSize, blockCount, fileName, 'r');
            end = now();
            timeTaken = end - start;
            totalTime += timeTaken;
        }
        avgTime = totalTime / TESTCOUNT;
        avgSpeed = fileSize / avgTime;
        printf("Avg Time taken = %f seconds, avg speed = %f MiB/s\n", avgTime, avgSpeed);
        if (blockSize == 1)
        {
            printf("\nBock Size is 1, so outputting speed in B/s as well. Speed in B/s (Sys calls/sec) = %f\n\n", avgSpeed * MiBinB);
        }
        
        
        testCount = TESTCOUNT;
        totalTime = avgTime = 0;
        printf("Clearing cache and running %d times\n", TESTCOUNT);
        while(testCount --){
            //Clear Cache
            system("sudo sh -c \"/usr/bin/echo 3 > /proc/sys/vm/drop_caches\"");
            start = now();
            part1(blockSize, blockCount, fileName, 'r');
            end = now();
            timeTaken = end - start;
            totalTime += timeTaken;
        }
        avgTime = totalTime / TESTCOUNT;
        avgSpeed = fileSize / avgTime;
        printf("Avg Time taken = %f seconds, avg speed = %f MiB/s\n", avgTime, avgSpeed);
                    
        
        
        
        printf("\n################dd performance:#################\n");
	    char command[1000];
	    sprintf(command, "dd if=%s%s%ld%s%ld", fileName, " of=/dev/null bs=",blockSize," count=",blockCount);
	    printf("%s\n",command);
	    system(command);
	    printf("\n");

        system("sudo sh -c \"/usr/bin/echo 3 > /proc/sys/vm/drop_caches\"");
	    printf("################dd performance: after clearing cache #################\n");
	    printf("%s\n",command);
	    system(command);
	    printf("\n");
    }

    return 0;
}