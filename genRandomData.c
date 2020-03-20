#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static void GenerateRandomNumbers(const char* inputFile, int num)
{
	srand(time(0)); //random seed
	int total = 0;
	int tempInt = 0;
	FILE * fptr;
	fptr = fopen(inputFile,"w");
	if(fptr == NULL)
	{
		perror("file problems\n");
		exit(1);
	}
	else
	{
		for(int i = 0; i < num; i++)
		{
			tempInt = rand() % 256;
			total = total + tempInt;
			fprintf(fptr,"%d\n", tempInt);
		}	
	}
	fclose(fptr);

	printf("Summation: %d\n", total);
} 

int main(int argc, char** argv)
{
  if (argc<3) {
   fprintf(stderr,"usage: %s <file> <number>\n",argv[0]);
   exit(1);
  }
  GenerateRandomNumbers(argv[1], atoi(argv[2]));
}
