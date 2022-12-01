#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<math.h>

#define BLOCKSIZE 262144
int num_threads = 128;
int blockCount = 1;
int fd = 0;

void * readandcompute(void* thread_index)
{
	long xor = 0;
	int *buf = (int *)malloc(BLOCKSIZE);
	for(long x = (long)thread_index; x< blockCount; x+=num_threads){
		long off = x* BLOCKSIZE;
		int n = pread(fd, buf, BLOCKSIZE, off);
		for (int i = 0; i < n/4; i++)
        	xor ^= buf[i];
	}
	free(buf);
	pthread_exit((void *)xor);
}

int main(int argc,char** argv){
	pthread_t *threads;
    char* fileName = argv[1];
   	int n;
	long xor = 0;
	fd = open(fileName, O_RDONLY);
	if (fd < 0)
	{
		perror("Error while opening file");
		exit(1);
	}
	double fileSize = lseek(fd, 0, SEEK_END);
	blockCount = ceil(fileSize/BLOCKSIZE);

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
	close(fd);
	free(threads);
	free(xorlist);

	return 0;
}