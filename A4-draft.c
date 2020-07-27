#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <time.h>
#include <semaphore.h>
#include <fcntl.h> 


int main(int argc, char *argv[]) {
    
   int available[argc - 1]; //Currently available resources.
   for (int i=0; i < argc-1; i++){
       available[i] = atoi(argv[i+1]);
   }
   int maximum[5][4];
   int threadCount = readFile("sample4_in.txt", &maximum);
   printf("%d", threadCount);

   //Command selection.
   /*
   char command[10];
   while (strcmp(command,"Q") != 0 ){
       printf("Enter Command (Q to quit): ");
       scanf("%s",command);
   } */
}

//function to read file and populate maximum array.
int readFile(char *fileName, int maximum[5][4]){
    FILE *in = fopen(fileName, "r");
	if(!in)
	{
		printf("Child A: Error in opening input file...exiting with error code -1\n");
		return -1;
	}

	struct stat st;
	fstat(fileno(in), &st);
	char* fileContent = (char*)malloc(((int)st.st_size+1)* sizeof(char));
	fileContent[0]='\0';	
	while(!feof(in))
	{
		char line[100];
		if(fgets(line,100,in)!=NULL)
		{
			strncat(fileContent,line,strlen(line));
		}
	}
	fclose(in);

	char* command = NULL;
	int threadCount = 0;
	char* fileCopy = (char*)malloc((strlen(fileContent)+1)*sizeof(char));
	strcpy(fileCopy,fileContent);
	command = strtok(fileCopy,"\r\n");
	while(command!=NULL)
	{
		threadCount++;
		command = strtok(NULL,"\r\n");
	}
	//*maximum = (int*) malloc(sizeof(int)*threadCount);

	char* lines[threadCount];
	command = NULL;
	int i=0;
	command = strtok(fileContent,"\r\n");
	while(command!=NULL)
	{
		lines[i] = malloc(sizeof(command)*sizeof(char));
		strcpy(lines[i],command);
		i++;
		command = strtok(NULL,"\r\n");
	}

	for(int k=0; k<threadCount; k++)
	{
		char* token = NULL;
		int j = 0;
		token =  strtok(lines[k],",");
		while(token!=NULL)
		{
			maximum[k][j] = atoi(token);
			j++;
			token = strtok(NULL,",");
		}
	}
	return threadCount;
}


