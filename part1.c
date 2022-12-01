#include<fcntl.h>

unsigned int part1(int blockSize, int blockCount, char *fileName, char rw){
	int n;
    unsigned int xor = 0;
    int fd;
	if(rw == 'r'){
		fd = open(fileName, O_RDONLY);
        long fileSize = lseek(fd, 0, SEEK_END);
        lseek(fd, 0, SEEK_SET);
        if (fileSize < (long)blockSize * (long)blockCount){
            printf("File Size is not big enough for the block count and block size provided\n");
            exit(1);
        }
    }
	else if(rw == 'w')
		fd = open(fileName, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
    {
        perror("Error while opening file");
        exit(1);
    }
    
    //NOTE: Blocksize we will get will be a multiple of 4
    int *buf = (int *)malloc(blockSize);
    int loop = 0;
    if(rw == 'r'){
        while (n = read(fd, buf, blockSize)){
			for (int i = 0; i < n/4; i++){
        	    xor ^= buf[i];
            }
            loop++;
            if (loop >= blockCount)
                break;
		}
    }
    else if(rw == 'w'){
        while (n = write(fd, buf, blockSize)){
            loop++;
            if (loop >= blockCount)
                break;
			free(buf);
			buf = (int *)malloc(blockSize);
		}
    }
    
    free(buf);
    close(fd);
	return xor;
}
