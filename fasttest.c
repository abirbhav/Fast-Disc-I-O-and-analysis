#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/time.h>
#include<pthread.h>
#include<math.h>

int blockSize = 262144;
int num_threads = 10;
int blockCount = 1;
int fd = 0;

double now() {
    struct timeval tv;
    gettimeofday(&tv, 0);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}
void * readandcompute(void* thread_index)
{
	long xor = 0;
	int *buf = (int *)malloc(blockSize);
	for(long x = (long)thread_index; x< blockCount; x+=num_threads){
		long off = x* blockSize;
		int n = pread(fd, buf, blockSize, off);
		for (int i = 0; i < n/4; i++)
        	xor ^= buf[i];
	}
	free(buf);
	pthread_exit((void *)xor);
}

int main(int argc,char** argv){
	pthread_t *threads;
	// int num_logical = sysconf(_SC_NPROCESSORS_CONF);
	// printf("Num of logical cores = %d\n", num_logical);
    char* fileName = argv[1];	
   	int n;
	long xor = 0;
    double start, end;
	start = now();
	fd = open(fileName, O_RDONLY);
	if (fd < 0)
	{
		perror("Error while opening file");
		exit(1);
	}
	double fileSize = lseek(fd, 0, SEEK_END);
	blockCount = ceil(fileSize/blockSize);
	fileSize /= 1048576;
    for(num_threads = 1; num_threads < 129; num_threads++){
        start = now();
	    
		xor = 0;
        threads = (pthread_t *) malloc(sizeof(pthread_t)*num_threads);
	    if (!threads) {
            perror("Error while creating threads");
		    exit(1);
        }
        for (long i = 0; i < num_threads; i++) {
            pthread_create(&threads[i], NULL, readandcompute, (void *)i);
        }
	    long *xorlist = (long *) malloc(sizeof(long) * num_threads);
        for (int i = 0; i < num_threads; i++) {
            pthread_join(threads[i],(void **)&xorlist[i]);
		    xor ^= xorlist[i];
        }
        unsigned int xor_ui = (unsigned int) xor;
        printf("xor: %08x\n",xor_ui);
        free(threads);
		free(xorlist);
        
        end = now();
	    double timeTaken = end - start;
	    
	    double speed = fileSize/timeTaken;
	    printf("Block Size = %d, Num_Threads: %d, Time Taken = %f seconds, speed = %f MiB/s\n", blockSize, num_threads, timeTaken, speed);
    }
    close(fd);
	
    return 0;
}