#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {
    
   int available[argc - 1]; //Currently available resources.
   for (int i=0; i < argc-1; i++){
       available[i] = atoi(argv[i+1]);
   }

   //Command selection.
   char command[10];
   while (strcmp(command,"Q") != 0 ){
       printf("Enter Command (Q to quit): ");
       scanf("%s",command);
   }
}

//function to read file and populate maximum array.
int readFile(char *fileName, int *maximum[]){
    FILE *in = fopen(fileName, "r");
	if(!in)
	{
		printf("Child A: Error in opening input file...exiting with error code -1\n");
		return -1;
	}

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
    
	

}


