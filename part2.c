long part2(long blockSize, char* fileName){
    double fileSize = 0;
    long blockCount = 1;
    double timeTaken = 0;
    double start, end;
    while(1){
        fileSize = (blockCount * blockSize);
        start = now();
        part1(blockSize, blockCount, fileName, 'r');
        end = now();
        timeTaken = end - start;
        if(timeTaken > 3)
            break;
        blockCount*=2;
    }
    return blockCount;
}