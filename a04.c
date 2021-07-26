/* Tim Martin and Matt Paek*/


#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <time.h>


typedef struct customer // represent each customer as a thread
{
	int id;		// track customers as int id's
	int maximum[4]; 	// note maximum demand for a customer
	int allocated[4];	// resources currently allocated
	int need[4];		// value for resources still needed
	
}  Customer;


// variables

	int available[4];	// array to hold our available resources
	char *command[5];	// user command	
	char userInput[50];	// take input from user

// function headers

int readFile(char *fileName, Customer **customers);  // function to read input file

int requestResources(char **command, Customer *customers, int *available); // requesting resources -> returns 0 -1
// int releaseResources -> releasing resoucrs
// no return -> display status based on *
// no return -> individual status print

// calculate a safe sequence if possible "Run"



// main program
int main(int argc, char *argv[]){
	printf("in the main\n");
	if (argc < 2){
		printf("Input file name missing... exiting with error code -1\n");
		return -1;
	}
	printf("get here\n");
	Customer *customer = NULL;
	int customerCount = readFile(argv[1], &customer);  // processing incoming file

	// initialize the available array from the command line
	available[0] = atoi(argv[2]);
	available[1] = atoi(argv[3]);	
	available[2] = atoi(argv[4]);	
	available[3] = atoi(argv[5]);


// need to implement a loop to allow users to play with resources

	do
	{
		printf("Commands are, 'RQ'request resources, 'RL' release resources, '*' output current info, 'Run' to calculate a safe sequence, or 'exit' to quit\n");  
		printf("Enter command: \n");
		scanf("%s", userInput);
	
		char *token = strtok(userInput, " ");
		int i = 0;
		
		while (token != NULL){
			command[i] = token;
			token = strtok(NULL, " ");
			i++;
		}
	
		
		if (strcmp(command[0], "RQ") == 0){
			requestResources(command, customer, available);
			printf("Requested resources \n");		
		}
		else if (strcmp(command[0], "RL") == 0){
			printf("released resources \n");
		}
		else if (strcmp(command[0], "*") == 0){
			printf("requested status\n");
		}
		else if (strcmp(command[0], "Run") == 0){
			printf("running a safe sequence\n");
		}
		else if (strcmp(command[0], "exit") == 0){
			printf("exiting the program\n");
		}
		else{
		
			printf("\n token is %s \n",token);
			printf("invalid entry, please try again\n");
		}
		
	
	
	}	while (strcmp(command[0], "exit")!=0);





printf("made it with %d \n",customerCount);
	return 0;
	
}


int readFile(char *fileName, Customer **customer)
{
	printf("do I get in read");
	FILE *in = fopen(fileName, "r");
	if (!in){
		printf("Error in opening the input file...exit with error code -1\n");
		return -1;
	}	

	struct stat st;
	fstat(fileno(in), &st);
	char * fileContent = (char*) malloc(((int) st.st_size +1) * sizeof(char));
	fileContent[0] ='\0';
	while (!feof(in)){
		char line[100];
		if (fgets(line,100,in) != NULL){
			strncat(fileContent, line, strlen(line));
		}
	}
	
	fclose(in);
	
	char *command = NULL;
	int customerCount = 0;
	char *fileCopy = (char*) malloc((strlen(fileContent) + 1)* sizeof(char));
	strcpy(fileCopy, fileContent);
	command = strtok(fileCopy,"\r\n");
	while (command != NULL){
		customerCount++;
		command = strtok(NULL, "\r\n");
	}
	
	*customer = (Customer*) malloc(sizeof(Customer)*customerCount);

	char *lines[customerCount];
	command = NULL;
	int i = 0;
	command = strtok(fileContent, "\r\n");
	while(command != NULL){
		lines[i] = malloc(sizeof(command)  * sizeof(char));
		strcpy(lines[i],command);
		i++;
		command = strtok(NULL, "\r\n");
	}

	for (int k = 0; k < customerCount; k++){
		(*customer)[k].id = k;
		char *token = NULL;
		int j = 0;
		token = strtok(lines[k],",");
		while (token != NULL) {
			//int x = atoi(token)
			(*customer)[k].maximum[j] = atoi(token);			
			(*customer)[k].need[j] = (*customer)[k].maximum[j];
			j++;					
			token = strtok(NULL, ",");
		}
	}	
	
	return customerCount;	
	
}


int requestResources(char **command, Customer *customers, int *available) 
{

	// command 1 = customer
	// command 2-5 = resources
	
// need to implement a semaphore or something on AVAILABLE and CUSTOMER
	
	
/* to do
1) verify request is less than total available -> or will fail
2) verify request does not exceed maximum for the thread (so needs to be < need)
3) if we meet both of the above we allocate, else we return -1

*/
	for (int i = 0; i < 4; i++){
		printf("in this loop\n");
		char *value = command[i+2];
		printf("%c",*value);
		printf("got to this spot\n");
		int resources = atoi(value);
		
		printf("in the first loop \n");
		printf("value is %d\n",resources);
		printf("fault above\n");
		if (atoi(value) > available[i]){
			printf("did I get in this loop\n");
			return -1;
		}
	}
	
	printf("got between the loops\n");
	
	for (int j = 0; j < 4; j++){
		if (available[j] > customers[atoi(command[2])].need[j]){
			return -1;
		} 
	}
	
	printf("need is %d\n",customers[0].need[0]);

	printf("got here! yEAH!!!\n\n");


	return 0;
}































