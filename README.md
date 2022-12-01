# Fast-Disc-I-O-and-analysis
The project aimed at trying to get Disc I/O to work as fast as possible and to evaluate the effects of caches and the cost of system calls.

# Prompt
[Original Project Prompt](https://www.notion.so/kyall/CS-GY-6233-Final-Project-f47fb948159e425da6a22ddfd318bb17)\
[Duplicate in case the original doesn't work](https://www.notion.so/CS-GY-6233-Final-Project-246c44f9179c4d96b287ffe926feb1ba)

# Instructions to Run:

## Part1
For part 1 execute the following: ./run <filename> [-r|-w] <block_size> <block_count>\
Output: xor of all 4-byte integers for the block count and size specified.

## Part 2
For part 2 execute the following: ./run2 <filename> <block_size>\
Output: Block count for which the program runs for a ‘reasonable’ amount of time i.e. 3 seconds or greater.
  
## Part 3 to 5
For parts 3 to 5, execute the following: ./complete <filename>\
This conducts all the experiments required for part 3 to 5.

## Part 6
For part 6, execute the following: ./fast <filename>\
Output: xor of all 4-byte integers in the file.\
  
In this part, we have also created two additional files:
1. fasttest.c (Notice that it is fasttest, not fastest)\
This program will help to compute the performance v/s threads, i.e. it will change the number of threads incrementally and calculate the performance in each case.\
Execution command: ./fasttest <filename>
2. fast2.c\
This program is the exact same as fast.c, except that it programmatically calculates the number of threads to be used instead of using a hard-coded value.\
Execution command: ./fast2 <filename>

# Final report
See report.pdf
  
---
A project by:\
[Abirbhav Dutta](https://github.com/abirbhav) ┊ Yashika Dhawan
