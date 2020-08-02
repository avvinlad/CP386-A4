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
int bankersAlgorithm(int nCustomers);
void requestResources(int customer, int r0, int r1, int r2, int r3);
void releaseResources(int customer, int r0, int r1, int r2, int r3);

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
		printf("Please use 4 arguments only.");
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

	//Command selection
	int customer; 
	int r0, r1, r2, r3;

	char command[100];
	while (strcmp(command, "Q") != 0)
	{
		printf("Enter Command (Q to quit): ");
		fgets(command, 100, stdin);

		char *p = strtok(command, " ");
		int i = 0;
		while (p){
			if (i == 0){ customer = atoi(p); }
			else if (i == 1){ r0 = atoi(p); }
			else if (i == 2){ r1 = atoi(p); }
			else if (i == 3){ r2 = atoi(p); }
			else if (i == 4){ r3 = atoi(p); }
			i++;
			p = strtok(NULL, " ");
		}

		if (strstr(command, "RQ")){
			printf("Processing Resouce Request...\n");
			requestResources(customer, r0, r1, r2, r3);
			// printf("User Entered: %d %d %d %d %d\n", customer, r0, r1, r2, r3);
		}
		else if (strstr(command, "RL")){
			printf("rl %d %d %d %d %d\n", customer, r0, r1, r2, r3);
		}
		else if (strstr(command, "RUN")){
			printf("run %d %d %d %d %d\n", customer, r0, r1, r2, r3);
		}
		else if (strstr(command, "*")){
			printf("* %d %d %d %d %d\n", customer, r0, r1, r2, r3);
		}
		else if (strstr(command, "Q")){ break; }
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

int bankersAlgorithm(int nCustomers)
{
	int finish[5] = {0, 0, 0, 0, 0};
	int safe = 0;
	int total_customers = nCustomers;

	while (total_customers > 0)
	{
		safe = -1;
		int execute = 0;
		for (int i = 0; i < P; i++)
		{
			if (finish[i] == 0)
			{
				execute = 1;
				for (int j = 0; j < R; j++)
				{
					if (need[i][j] > available[j])
					{
						execute = 0;
						break;
					}
				}

				if (execute == 1)
				{
					printf("\nProcess[%d] is executing.\n", i + 1);
					finish[i] = 1;
					safeSequence[P - total_customers] = i;
					total_customers--;
					safe = 0;
					for (int j = 0; j < R; j++)
					{
						available[j] += allocation[i][j];
					}
					break;
				}
			}
		}

		if (safe == -1)
		{
			printf("\nThe process is unsafe.\n");
			break;
		}
	}
	return safe;
}

void requestResources(int customer, int r0, int r1, int r2, int r3)
{
	int canRequest = 1;
	int i, j;
	int requestedResources[R];

	requestedResources[0] = r0;
	requestedResources[1] = r1;
	requestedResources[2] = r2;
	requestedResources[3] = r3;

	for (i = 0; (i < R) && (canRequest == 0); i++)
	{
		if (requestedResources[i] > need[customer][i])
		{
			canRequest = 0;
		}
		if (requestedResources[i] > available[i])
		{
			canRequest = 0;
		}
	}

	if (!canRequest)
	{
		printf("Cannot fufill reuqested resourcess.\n");
	}
	else
	{
		for (i = 0; i < R; i++)
		{
			available[i] -= requestedResources[i];
		}

		for (j = 0; j < R; j++)
		{
			allocation[customer][j] += requestedResources[j];
			need[customer][j] -= requestedResources[j];
		}

		int isSafe = bankersAlgorithm(threadCount);

		if (isSafe == 0)
		{
			for (i = 0; i < R; i++)
			{
				available[i] += requestedResources[i];
			}

			for (j = 0; j < R; j++)
			{
				allocation[customer][j] -= requestedResources[j];
				need[customer][j] += requestedResources[j];
			}

			printf("Cannot fufill reuqested resourcess. Please wait patiently.\n");
		}
		else
		{
			printf("Reuqested Resources Successful.\n");
		}
	}

	return;
}

void releaseResources(int customer, int r0, int r1, int r2, int r3)
{
	int canRelease = 1;
	int i, j;
	int releasedResources[R];

	releasedResources[0] = r0;
	releasedResources[1] = r1;
	releasedResources[2] = r2;
	releasedResources[3] = r3;

	for (i = 0; (i < R) && (canRelease == 1); i++)
	{
		if (releasedResources[i] > allocation[customer][i])
		{
			canRelease = 0;
		}
	}

	if (!canRelease)
	{
		printf("Cannot complete release of resources.\n");
	}
	else
	{
		for (j = 0; j < R; j++)
		{
			available[j] += releasedResources[j];
			need[customer][j] += releasedResources[j];
			allocation[customer][j] -= releasedResources[j];
		}

		printf("Resources Released Successfully.\n");
	}

	return;
}
