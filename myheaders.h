#ifndef MYHEADERS_H
#define MYHEADERS_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <semaphore.h>
#include <pthread.h>
#include <fcntl.h>
#include <math.h>	

	pid_t pids[20];
	sem_t * mutex;
	int sharedID;
	int * arr;
	#define NOTIMETEST 0
//	const char inputFile[] = "./InputNumberFile.txt";
	const char sharedKey[] = "./master.c";
	const char outputFile[] = "./adder_log";
	const char semaphoreName[] = "semSignal";
	const char sharedIDFile[] = "./sharedID.txt";
	const int sharedInt = 63;
#endif	
