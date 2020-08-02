/*

CP 386 Assignment 4

GitHub URL: https://github.com/avvinlad/CP386-A4

Name: Avin Lad
GitHub Login: avvinlad
Student ID: 180647870

*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <time.h>
#include <semaphore.h>
#include <fcntl.h>

void *customerThread(void *thread); //function for the thread/customer
int readFile(char *fileName, int maximum[5][4]);
int safetyAlgorithm(int nCustomers);
void requestResources(int customer, int r0, int r1, int r2, int r3);
void releaseResources(int customer, int r0, int r1, int r2, int r3);
void runFunction();
void overview();

#define P 5 //processes
#define R 4 //resources
int threadCount;
int safeSequence[R];
int available[R]; //Currently available resources.
int maximum[P][R];
int allocation[P][R];
int need[P][R];

int main(int argc, char *argv[])
{

	if (argc != 5)
	{
		printf("Please use 4 arguments only. (Example: \"./Assignment_4 1 2 3 4\")\n");
		return -1;
	}

	for (int i = 0; i < argc - 1; i++)
	{
		available[i] = atoi(argv[i + 1]);
	}

	threadCount = readFile("sample4_in.txt", maximum);
	printf("Number of Customers: %d\n", threadCount);

	for (int j = 0; j < P; j++)
	{
		for (int k = 0; k < R; k++)
		{
			allocation[j][k] = 0;		// right now nothing is allocated
			need[j][k] = maximum[j][k]; // the need will start with max
		}
	}

	printf("Currently Available Resources: ");
	for (int i = 0; i < R; i++){
		printf("%d ", available[i]);
	}
	printf("\n");

	printf("Maximum Resources From File:\n");
	for (int i = 0; i < P; i++){
		for (int j = 0; j < R; j++){
			printf("%d ", maximum[i][j]);
		}
		printf("\n");
	}

	//Command selection
	int customer;
	int r0, r1, r2, r3;
	char command[100], cmd[2];
	while (strcmp(command, "Q") != 0)
	{
		printf("\nEnter Command (Q to quit): ");
		fgets(command, 100, stdin);

		char *ptr = strtok(command, " \r\n");
		int j = 0;
		strcpy(cmd, ptr);
		while (ptr != NULL){
			if (j == 1){ customer = atoi(ptr); }
			else if (j == 2){ r0 = atoi(ptr); }
			else if (j == 3){ r1 = atoi(ptr); }
			else if (j == 4){ r2 = atoi(ptr); }
			else if (j == 5){ r3 = atoi(ptr); }
			j++;
			ptr = strtok(NULL, " \r\n");
		}

		// printf("User Entered: %d %d %d %d %d\n", customer, r0, r1, r2, r3);

		if (strstr(cmd, "RQ")){
			printf("Processing Resouce Request\n...\n");
			requestResources(customer, r0, r1, r2, r3);
		}
		else if (strstr(cmd, "RL")){
			printf("Processing Resouce Release\n...\n");
			releaseResources(customer, r0, r1, r2, r3);
		}
		else if (strstr(cmd, "RUN")){
			runFunction();
		}
		else if (strstr(cmd, "*")){
			overview();
			printf("\n");

		}
		else if (strstr(cmd, "Q")){ 
			printf("Program Exited.\n");
			break; 
		}
	}
}

//function to read file and populate maximum array.
int readFile(char *fileName, int maximum[5][4])
{
	FILE *in = fopen(fileName, "r");
	if (!in)
	{
		printf("Child A: Error in opening input file...exiting with error code -1\n");
		return -1;
	}

	struct stat st;
	fstat(fileno(in), &st);
	char *fileContent = (char *)malloc(((int)st.st_size + 1) * sizeof(char));
	fileContent[0] = '\0';
	while (!feof(in))
	{
		char line[100];
		if (fgets(line, 100, in) != NULL)
		{
			strncat(fileContent, line, strlen(line));
		}
	}
	fclose(in);

	char *command = NULL;
	int threadCount = 0;
	char *fileCopy = (char *)malloc((strlen(fileContent) + 1) * sizeof(char));
	strcpy(fileCopy, fileContent);
	command = strtok(fileCopy, "\r\n");
	while (command != NULL)
	{
		threadCount++;
		command = strtok(NULL, "\r\n");
	}

	char *lines[threadCount];
	command = NULL;
	int i = 0;
	command = strtok(fileContent, "\r\n");
	while (command != NULL)
	{
		lines[i] = malloc(sizeof(command) * sizeof(char));
		strcpy(lines[i], command);
		i++;
		command = strtok(NULL, "\r\n");
	}

	for (int k = 0; k < threadCount; k++)
	{
		char *token = NULL;
		int j = 0;
		token = strtok(lines[k], ",");
		while (token != NULL)
		{
			maximum[k][j] = atoi(token);
			allocation[k][j] = atoi(token);
			need[k][j] = atoi(token);
			j++;
			token = strtok(NULL, ",");
		}
	}
	return threadCount;
}

void runFunction(){
	printf("Safe Sequence: <");
	for (int i = 0; i < P; i++){
		printf("%d", safeSequence[i]);
		if (i <(P-1)){ printf(" "); }
	}
	printf(">\n");


	printf("Executing Threads:\n");
	for (int j = 0; j < threadCount; j++){
		pthread_t thread; 
		pthread_create(&thread, NULL, customerThread, &j);
		pthread_join(thread, NULL);
	}
}

void *customerThread(void *thread){
	int customer = *(int *)thread;
	printf("--> Customer [%d]\n", customer);

	printf("\tAllocated resources: ");
	for (int i = 0; i < R; i++){
		printf("%d ", allocation[customer][i]);
	}
	printf("\n");

	printf("\tNeeded: ");
	for (int i = 0; i < R; i++){
		printf("%d ", need[customer][i]);
	}
	printf("\n");

	printf("\tAvailable: ");
	for (int i = 0; i < R; i++){
		printf("%d ", available[i]);
	}
	printf("\n");

	printf("\tNeeded: ");
	for (int i = 0; i < R; i++){
		printf("%d ", need[customer][i]);
	}
	printf("\n");

	printf("\tThread started\n");
	
	printf("\tThread releasing resources: ");
	for (int i = 0; i < R; i++){
		printf("%d ", need[customer][i]);
	}
	printf("\n");
	for (int i = 0; i < R; i++){
		available[i] += allocation[customer][i];
		allocation[customer][i] = 0;
		need[customer][i] = maximum[customer][i];
	}

	printf("\tNew Available: ");
	for (int i = 0; i < R; i++){
		printf("%d ", available[i]);
	}
	printf("\n");

	printf("\tThread has finished\n");

	pthread_exit(0);
}

void requestResources(int customer, int r0, int r1, int r2, int r3)
{
	int requestedResources[R];

	requestedResources[0] = r0;
	requestedResources[1] = r1;
	requestedResources[2] = r2;
	requestedResources[3] = r3;


	if (r0 <= need[customer][0] && r1 <= need[customer][1] 
	&& r2 <= need[customer][2] && r3 <= need[customer][3]){

		if (r0 <= available[0] && r1 <= available[1] 
		&& r2 <= available[2] && r3 <= available[3]){

			for (int i = 0; i < R; i++)
			{
				available[i] -= requestedResources[i];
				allocation[customer][i] += requestedResources[i];
				need[customer][i] -= requestedResources[i];
			}

			int isSafe = safetyAlgorithm(threadCount);

			if (isSafe == -1)
			{
				for (int i = 0; i < R; i++)
				{
					available[i] += requestedResources[i];
				}

				for (int j = 0; j < R; j++)
				{
					allocation[customer][j] -= requestedResources[j];
					need[customer][j] += requestedResources[j];
				}
				printf("Cannot fufill requested resourcess. Please wait patiently.\n");
			}
			else
			{
				printf("Requested Resources SUCCESSFUL.\n");
			}

		}
	}

	return;
}

void releaseResources(int customer, int r0, int r1, int r2, int r3)
{
	int releasedResources[R];

	releasedResources[0] = r0;
	releasedResources[1] = r1;
	releasedResources[2] = r2;
	releasedResources[3] = r3;


	if (r0 <= allocation[customer][0] && r1 <= allocation[customer][1] 
		&& r2 <= allocation[customer][2] && r3 <= allocation[customer][3]){

		for (int j = 0; j < R; j++)
		{
			available[j] += releasedResources[j];
			need[customer][j] += releasedResources[j];
			allocation[customer][j] -= releasedResources[j];
		}

		printf("Released Resources SUCCESSFUL.\n");
	}
	else{
		printf("Cannot release resources.\n");
	}

	return;
}


int safetyAlgorithm(int nCustomers)
{
	int avl[R];
	int all[P][R];
	int needCopy[P][R];

	for (int i = 0; i < R; i++){
		avl[i] = available[i];
	}

	for (int j = 0; j < P; j++){
		for (int k = 0; k < R; k++){
			all[j][k] = allocation[j][k];
			needCopy[j][k] = need[j][k];
		}
	}

	int finish[5] = {0, 0, 0, 0, 0};
	int isSafe = -1;
	int execute = 0;
	int total_customers = nCustomers;

	while (total_customers != 0){
		isSafe = -1;
		for (int i = 0; i < P; i++)
		{
			if (finish[i] == 0)
			{
				execute = 1;
				for (int j = 0; j < R; j++)
				{
					if (needCopy[i][j] > avl[j])
					{
						execute = 0;
						break;
					}
				}

				if (execute == 1)
				{
					isSafe = 0;
					finish[i] = 1;
					safeSequence[P - total_customers] = i;
					total_customers--;
					for (int j = 0; j < R; j++)
					{
						avl[j] += all[i][j];
					}
					break;
				}
			}
		}

		if (isSafe == -1)
		{
			printf("The process is unsafe.\n");
			break;
		}
	}

	return isSafe;
}



void overview(){
	printf("Available: ");
	for (int i = 0; i < R; i++){
		printf("%d ", available[i]);
	}
	printf("\n\n");

	printf("Maximum: \n");
	for (int i = 0; i < P; i++){
		for (int j = 0; j < R; j++){
			printf("%d ", maximum[i][j]);
		}
		printf("\n");
	}
	printf("\n");

	printf("Allocation: \n");
	for (int i = 0; i < P; i++){
		for (int j = 0; j < R; j++){
			printf("%d ", allocation[i][j]);
		}
		printf("\n");
	}
	printf("\n");

	printf("Need: \n");
	for (int i = 0; i < P; i++){
		for (int j = 0; j < R; j++){
			printf("%d ", need[i][j]);
		}
		printf("\n");
	}
}
