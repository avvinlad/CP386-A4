#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <time.h>
#include <semaphore.h>
#include <fcntl.h> 

void* customerThread(void* thread); //function for the thread/customer 
int readFile(char *fileName, int maximum[5][4]);
bool safetyAlgorithm(int nCustomers);
void requestResources(int process, int r1, int r2, int r3, int r4);
void releaseResources(int process, int r1, int r2, int r3, int r4);

int P = 5; //processes
int R = 4  //resources
int threadCount;
int safeSequence[R]; 
int available[R]; //Currently available resources.
int maximum[P][R];
int allocation[P][R];
int need[P][R];


int main(int argc, char *argv[]) {
    
	if (argc != 5){
		printf("Please use 4 arguments only.");
	}

	for (int i=0; i < argc-1; i++){
		available[i] = atoi(argv[i+1]);
	}

	threadCount = readFile("sample4_in.txt", maximum);
	printf("Thread Count %d\n", threadCount);

	for (int j = 0; j < P; j++){
		for (int k = 0; k < R; k++){
			allocation[j][k] = 0; // right now nothing is allocated
			need[j][k] = maximum[j][k]; // the need will start with max
		}
	}

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
			allocation[k][j] = atoi(token);
			need[k][j] = atoi(token);
			j++;
			token = strtok(NULL,",");
		}
	}
	return threadCount;
}

int safetyAlgorithm(int nCustomers){
	int finish[5] = {0, 0, 0, 0, 0};
	bool safe = true;
	int total_customers = nCustomers;

	while (total_customers > 0) {
        int safe = false;
		int execute = 0;
        for (i = 0; i < P; i++) {
            if (finish[i] == 0) {
                execute = 1;
                for (j = 0; j < R; j++) {
                    if (need[i][j] > available[j]) {
                        execute = 0;
                        break;
                    }
                }

                if (execute == 1) {
                    printf("\nProcess[%d] is executing.\n", i + 1);
                    finish[i] = 1;
					safeSequence[P - total_customers] = i;
                    total_customers--;
                    safe = true;
                    for (j = 0; j < R; j++)
                        available[j] += curr[i][j];
                    break;
                }
            }
        }

		if (safe){
			printf("\nThe process is in safe state.\n");
		}
		else{
			printf("\nThe process is unsafe.\n");
		}


	return safe;
}

void requestResources(int process, int r1, int r2, int r3, int r4){

}

void releaseResources(int process, int r1, int r2, int r3, int r4){

}





