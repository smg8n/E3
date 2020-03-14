#include "myheaders.h"
int sharedID;
struct itimerval myTime;

int GetSharedIDFromFile()
{
        FILE * fptr;
        int num = 0;
        fptr = fopen(sharedIDFile, "r");
        fscanf(fptr,"%d", &num);
	fclose(fptr);
        return num;
}

void RelaxTheCells(int bins, int binSize)
{
	sharedID = GetSharedIDFromFile();
	arr = (int*)shmat(sharedID, NULL, 0);

	for(int i = 0; i <bins; i++)
	{	
		arr[i+2] = arr[(i*binSize)+2];
	}	
	shmdt(arr);
}


void PrintArray(int size)
{
	sharedID = GetSharedIDFromFile();
        arr = (int*)shmat(sharedID, NULL, 0);
        
        for(int x = 0; x < size; x++)
        {
                printf("%d ", arr[x]);
        }
        printf("\n");
        shmdt(arr);
}

void UpdateTime()
{
	struct itimerval childTime;
	getitimer(ITIMER_REAL, &childTime);
	sharedID = GetSharedIDFromFile();
	arr = (int*)shmat(sharedID, NULL, 0);
	arr[0] = myTime.it_value.tv_sec - childTime.it_value.tv_sec;
	arr[1] = childTime.it_value.tv_usec;
	shmdt(arr);
}

void CatchSignal(int sig)
{
	if(sig == SIGINT)
	{
		printf("\nKill signal was executed\n");
	}
	else if(sig == SIGALRM)
	{
		printf("Time has expired\n");
	}
	sem_close(mutex);
	shmctl(sharedID, IPC_RMID, NULL);
	for(int i = 0; i < 20; i++)
		kill(pids[i], SIGKILL);
	kill(getpid(), SIGKILL);
	exit(0);		
}
int GetBinZero()
{
	int returnInt = 0;
	arr = (int*)shmat(sharedID, NULL, 0);
	returnInt = arr[2];
	shmdt(arr);

	return returnInt;
}

void MethodOne(int size,int binSize)
{
	int pid;
	int tempSize = size;
	int bins = 0;
	int maxAllowed = 20;
	int aliveChilds = 0;
	int intExec = 0;
	int pida = 0;
	int status = 0;
	int exitFlag = 0;
	int innerExitFlag = 0;
	int anotherTemp = 0;
	char ** argToPass = (char**)malloc(sizeof(char *) * 2);

	for(int i = 0; i < 2; i++)
		argToPass[i] = (char*)malloc(sizeof(int) * 16);


	do
	{
		do
		{
			 
			if(maxAllowed > aliveChilds && tempSize > 0)
			{		
				pid = fork();
				if(pid < 0)
				{
					perror("Creation of child process was unsuccessful\n");
				}
				else if(pid == 0)
				{
	        anotherTemp = tempSize;
       	  if(tempSize > binSize)
       	     	anotherTemp = binSize;
					char st[16];
					sprintf(st, "%d", (bins *binSize));
					strcpy(argToPass[0], st);
					sprintf(st, "%d", anotherTemp);
					strcpy(argToPass[1], st);
					argToPass[2] = NULL;
          fprintf(stderr, "Exec PID=%d xx=%s yy=%s\n", getpid(), argToPass[0], argToPass[1]);
					intExec = execv("bin_adder",argToPass);
				}
				else if(pid > 0)
				{
					aliveChilds++;
					pids[bins] = pid;
		 	    bins++;
		      tempSize = tempSize - binSize;
				}
			}
			for(int k = 0; k < aliveChilds; k++)	
			{
				pida = waitpid(pida, &status, WNOHANG);
				if(pida == -1)
				{
				}		
				else if(pida == 0)
				{
				}
				else if(pida > 0)
				{
					aliveChilds--;
					if(tempSize <= 0 && aliveChilds == 0)
						innerExitFlag = 1;
				}
			}
			UpdateTime();
		}while(innerExitFlag == 0 && aliveChilds > 0);

		if(tempSize <= 0)
		{	
			RelaxTheCells(bins, 2);
			innerExitFlag = 0;
			tempSize = bins;
			if(bins == 1)
			{
				exitFlag = 1;		
				innerExitFlag = 1;
			}
			bins = 0;
		}
	}while(innerExitFlag == 0 && exitFlag == 0);
	free(argToPass);

}

void MethodTwo(int size, int binSize)
{

	int maxAllowed = 20;
	int aliveChilds = 0;
	int tempSize = size;
	int bins = 0;
	int pid = 0;
	int pida = 0;
	int anotherTemp = 0;
	int intExec = 0;
	int status = 0;
	printf("Method Two binSize %d\n",binSize);
        char ** argToPass = (char**)malloc(sizeof(char *) * 2);
        for(int i = 0; i < 2; i++)
                argToPass[i] = (char*)malloc(sizeof(int) * 8);

	do
	{
		if(maxAllowed > aliveChilds && bins < ceil(size/(binSize*1.0)))
		{	
			pid = fork();
			if(pid < 0)
			{
				perror("Creation of child Process was unsuccessful\n");
			}
			else if(pid == 0)
			{
				if(tempSize > binSize)
				{
					anotherTemp = binSize;
				}
				else
					anotherTemp = tempSize;
	                                char * st;
                                st = (char*)malloc(sizeof(char) * 10);
                                sprintf(st, "%d", ((bins*binSize)));
                                strcpy(argToPass[0], st);
                                sprintf(st, "%d", anotherTemp);
                                strcpy(argToPass[1], st);
                                argToPass[2] = NULL;
                                intExec = execv("bin_adder",argToPass);
                                printf("Exec %d\n", intExec);
				free(st);
                                exit(0);
			}
			else
			{
				pids[bins] = pid;
				bins++;
				tempSize = tempSize - binSize;
				aliveChilds++;			
			}
		}

                for(int k = 0; k < aliveChilds; k++)
                {
                        pida = waitpid(pida, &status, WNOHANG);
                        if(pida == -1)
                        {
                        }
                        else if(pida == 0)
                        {
			}
                        else if(pida > 0)
			{
                            aliveChilds--;
                        }
                 }
 		UpdateTime();
	}while(aliveChilds > 0);
	free(argToPass);
	RelaxTheCells(bins, binSize);	
	MethodOne(bins, 2);
}

int GetInputPlaceInSharedMem(const char* inputFile, int num)
{
	int ret = 0;
	int  intVar = 0;
	key_t key = ftok(sharedKey, sharedInt);
	int shmid = shmget(key, (num+2)*  sizeof(int), 0666|IPC_CREAT);
	
	printf("print shmid %d\n", shmid);
	arr = (int*) shmat(shmid,NULL ,0);
	arr[0] = 0;
	FILE * fptr;
	if((fptr = fopen(inputFile, "r")) == NULL)
	{
	  perror("file problems\n");
	  exit(1);
	}
	ret = fscanf(fptr, "%d", &intVar);
	arr[0] = 0;
	arr[1] = 0;
	for(int i = 0;ret != EOF && i < num; i++)
	{
		arr[2+i] = intVar;
		ret = fscanf(fptr, "%d", &intVar);
		printf("%d ", arr[2+i]);
	}
	printf("\n");
	fclose(fptr);
	shmdt(arr);
	return shmid;
}

void SetSharedID(int num)
{

	FILE * fptr;
	if((fptr = fopen(sharedIDFile, "w")) == NULL)
	{
		perror("file problems\n");
		exit(1);
	}
	
	fprintf(fptr, "%d",num);
	fclose(fptr);
}

void ResetNumbers(const char* inputFile, int num)
{

	int intVar = 0;
	arr = (int*)shmat(sharedID, NULL, 0);
	FILE * fptr;
	if((fptr = fopen(inputFile, "r")) == NULL)
	{
		perror("file problems\n");
		exit(1);
	}
	fscanf(fptr, "%d", &intVar);
	for(int i = 0; i < num; i++)
	{
		arr[i+2] = intVar;
		fscanf(fptr, "%d", &intVar);
	}
	fclose(fptr);
	shmdt(arr);

}

// count lines in InputFile
int countLinesInInputFile(const char* inputFile)
{
	int count = 0;
	int num = 0;
	int total = 0;
	int ret = 0;
	FILE * fptr;
	if((fptr = fopen(inputFile, "r")) == NULL)
	{
		perror("file problems");
		exit(1);
	}
	ret = fscanf(fptr, "%d", &num);
	while(ret != EOF)
	{
	
		total = total + num;
		count++;
		ret = fscanf(fptr, "%d", &num);
	}

	printf("Summation: %d total numbers: %d\n", total, count);
	fclose(fptr);
	return count;
}

int main(int argc, char ** argv)
{
	char inputFile[128];
	int numElem = 0;
	int method1Summation = 0;
	int method2Summation = 0;
	struct itimerval method1Time, method2Time;
	
	if(argc > 1) {
    	 char c;
         /*Use getopt to parse command arguements, h: means
            * h require an extra argument*/
         while ((c = getopt(argc, argv, "h")) != -1)
            switch (c) {
                /*Help option*/
                case 'h' :
                    printf("\nusage master <input file>\n");
                    exit(EXIT_SUCCESS);
         }
         strcpy(inputFile, argv[1]);
        } else {
           printf("\nusage master <input file>\n");
           exit(EXIT_SUCCESS);
        }
	
	signal(SIGALRM, CatchSignal);
	signal(SIGINT, CatchSignal);
	myTime.it_value.tv_sec = 100;
	myTime.it_value.tv_usec = 0;
	myTime.it_interval = myTime.it_value;
	mutex = sem_open(semaphoreName, O_CREAT|O_EXCL, 0666, 63);
        FILE * fptr;	
	fptr = 	fopen(outputFile, "w");
	fclose(fptr);

	numElem = countLinesInInputFile(inputFile); // first iteration to find the count of numbers

	printf("Start Method1 binary tree algorithm\n");
	sharedID = GetInputPlaceInSharedMem(inputFile, numElem);
	SetSharedID(sharedID);
	setitimer(ITIMER_REAL, &myTime, NULL);
	MethodOne(numElem,2); //method 1;
	method1Summation = GetBinZero();
	printf("Sum from  Method1=%d\n", method1Summation);
	getitimer(ITIMER_REAL, &method1Time);

	printf("Start Method2\n");
	int binSize = ceil(numElem/(log10(numElem)/log10(2)));
	ResetNumbers(inputFile, numElem);
	MethodTwo(numElem, binSize);
	method2Summation = GetBinZero();
	getitimer(ITIMER_REAL, &method2Time);
	printf("Method 1 Summation %d completed in %ld seconds and %ld micro seconds\n",method1Summation,method1Time.it_interval.tv_sec - method1Time.it_value.tv_sec, method1Time.it_value.tv_usec);
	printf("Method 2 Summation %d completed in %ld seconds and %ld micro seconds\n",method2Summation,method1Time.it_value.tv_sec - method2Time.it_value.tv_sec, method1Time.it_value.tv_usec - method2Time.it_value.tv_usec);
	if(shmctl(sharedID, IPC_RMID, NULL) < 0)
		fprintf(stderr, "Shared memory was not deallocated: remove it manually\n");

	remove(sharedIDFile);
	sem_destroy(mutex);	
	return 0;
}
