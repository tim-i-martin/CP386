/* Tim Martin and Matt Paek*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <time.h>
#include <semaphore.h>

#define MAX_INPUT 256
#define FILE_NAME "sample4_in.txt"

int resource_amount;
int customer_amount;
int **max;
int **allocated;
int **need;
int *seq;
int *available;
int is_safe;

int **readFile(char *file_name);
void *runThread(void *t);
int *safetyAlgorithm();

// typedef struct customer // represent each customer as a thread
// {
// 	int id;			  // track customers as int id's
// 	int maximum[4];	  // note maximum demand for a customer
// 	int allocated[4]; // resources currently allocated
// 	int need[4];	  // value for resources still needed

// } Customer;

// // variables

// int available[4];	// array to hold our available resources
// char *command[5];	// user command
// char userInput[50]; // take input from user

// // function headers

// int readFile(char *fileName, Customer **customers); // function to read input file

// int requestResources(char **command, Customer *customers, int *available); // requesting resources -> returns 0 -1
// // int releaseResources -> releasing resoucrs
// // no return -> display status based on *
// // no return -> individual status print

// // calculate a safe sequence if possible "Run"

// main program
int main(int argc, char *argv[])
{
	printf("in the main\n");
	if (argc < 2){
		printf("Input file name missing... exiting with error code -1\n");
		return -1;
	}

	resource_amount = argc - 1; //set up the array of available resources
	available = malloc(sizeof(int) * resource_amount);
	for (int i = 1; < argc; i++){
		available[i - 1] = atoi(argv[i]);
	}

	max = readFile(FILE_NAME); //set up max array from the file

	allocated = malloc(sizeof(int *) * customer_amount);
	need = malloc(sizeof(int *) * customer_amount);
	for (int i = 0; i < customer_amount; i++){
		allocated[i] = malloc(sizeof(int) * resource_amount);
		need[i] = malloc(sizeof(int) * resource_amount);
	}

	is_safe = 0; //start safe flag at 0

	char *user_input = malloc(sizeof(char) * MAX_INPUT);
	printf("Number of Customers: ", customer_amount);
	printf("Currently Available resources: ");
	for (int i = 0; i < resource_amount; i++){
		printf("%d", available[i]);
		if (i < m - 1){
			printf(" ");
		}
		printf("\n");
	}
	printf("Maximum resources from file:\n");
	for (int i = 0; i < customer_Count; i++){
		for (int j = 0; j < resource_amount; j++){
			printf("%d", max[i][j]);
			if (j < n - 1){
				print(" ");
			}
		}
		printf("\n");
	}



	// printf("get here\n");
	// Customer *customer = NULL;
	// int customerCount = readFile(argv[1], &customer);  // processing incoming file

	// // initialize the available array from the command line
	// available[0] = atoi(argv[2]);
	// available[1] = atoi(argv[3]);
	// available[2] = atoi(argv[4]);
	// available[3] = atoi(argv[5]);

	// need to implement a loop to allow users to play with resources
	while (1)
	{
		printf("Commands are, 'RQ'request resources, 'RL' release resources, 'Status' output current info, 'Run' to calculate a safe sequence, or 'exit' to quit\n");
		printf("Enter command: \n");
		fgets(user_input, MAX_INPUT, stdin);
		if (user_input[strlen(user_input) - 1] == '\n' && strlen(user_input) > 0){
			user_input[strlen(user_input) - 1] = '\0';
		}
		
		if (strstr(user_input, "RQ")){
			int amount = 0;
			int *user_input_arr = malloc(sizeof(int) * (resource_amount +1));
			char *token = NULL;
			token = strtok(user_input, " ");
			while (toke != NULL){
				if (amount > 0){
					user_input_arr[amount - 1] = atoi(token);
				}
				token = strtok(NULL, " ");
				amount = amount + 1;
			}
			int customer_allocation = user_input_arr[0];
			if (amount == resource_amount +2 && customer_allocation < customer_amount){
				for (int i = 0; i < resource_amount; i++){
					allocated[customer_allocation][i] = user_input_arr[i + 1];
					need[customer_allocation][i] = max[customer_allocation][i] - allocated[customer_allocation][i];
					if (need[customer_allocation][i] < 0){
						need[customer_allocation][i] = 0;
					}
				}
			} else {
				if (customer_allocation >= customer_amount){
					printf("Thread not available please do another \n");
				} else {
					printf("not enough parameters. try again \n");
				}
				}
			free(user_input_arr);
			seq = safetyAlgorithm();
			printf("State is safe, request granted\n");
			if (seq[0] == -1){
				is_safe = 0;
			} else {
				is_safe = 1;
			}
			} else if (strstr(user_input, "RL")){
				int amount = 0;
				int *user_input_arr = malloc(sizeof(int) * (resource_amount +1));
				char *token = NULL;
				token = strtok(user_input, " ");
				while (toke != NULL){
				if (amount > 0){
					user_input_arr[amount - 1] = atoi(token);
				}
				token = strtok(NULL, " ");
				amount = amount + 1;
			}
			int customer_allocation = user_input[0];
			if (amount == resource_amount +2 && customer_allocation < customer_amount){
				for (int i = 0; i < resource_amount; i++){
					if (user_input_arr[i+1] <= allocated[customer_allocation][i]){
						allocated[customer_allocation][i] -= user_input_arr[i+1];
						need[customer_allocation][i] = max[customer_allocation][i] - allocated[customer_allocation][i];
					} else {
						printf("unable to release more than max allocated\n");
						break;
					}
				}				
			} else {
				if (customer_allocation >= customer_amount){
					printf("Thread not available please do another \n");
				} else {
					printf("not enough parameters. try again \n");
				}
			}
			free(user_input_arr);
			seq = safetyAlgorithm();
			printf("State is safe, request granted\n");
			if (seq[0] == -1){
				is_safe = 0;
			} else {
				is_safe = 1;
			}
			} else if (strstr(user_input, "Status")){
				printf("Currently Available resources: ");
				for (int i = 0; i < resource_amount; i++){
					printf("%d", available[i]);
					if (i < m - 1){
						printf(" ");
					}
					printf("\n");
				}
				printf("Maximum resources:\n");
				for (int i = 0; i < customer_amount; i++){
					for (int j = 0; j < resource_amount; j++){
						printf("%d", max[i][j]);
						if (j < n - 1){
							print(" ");
						}
					}
					printf("\n");
				}
				printf("Allocation resources:\n");
				for (int i = 0; i < customer_amount; i++){
					for (int j = 0; j < resource_amount; j++){
						printf("%d", allocated[i][j]);
						if (j < n - 1){
							print(" ");
						}
					}
					printf("\n");
				}
				printf("Need resources:\n");
				for (int i = 0; i < customer_amount; i++){
					for (int j = 0; j < resource_amount; j++){
						printf("%d", need[i][j]);
						if (j < n - 1){
							print(" ");
						}
					}
					printf("\n");
				}

			} else if (strstr(user_input, "Run")){
				seq = safetyAlgorithm();
				if (is_safe == 1){
					for (int i = 0; i < customer_amount; i++){
						int new_thread = seq[i];
						pthread_t thread_id;
						pthread_attr_t attribute_thread;
						pthread_attr_init(&attribute_thread);
						pthread_create(&thread_id, &attribute_thread, runThread, (void *)&new_thread);
						pthread_join(thread_id, NULL);
					}
				}
			} else if (strstr(, user_input, "exit")){
				free(allocated);
				free(need);
				free(available);
				free(allocated);
				free(seq);
				return 0;
			} else {
				printf("Command not valid please enter \"RQ\", \"RL\", \"Status\" or \"Exit\"\n");
			}

		}
		return 0;
	}

// 		char *token = strtok(userInput, " ");
// 		int i = 0;

// 		while (token != NULL)
// 		{
// 			command[i] = token;
// 			token = strtok(NULL, " ");
// 			i++;
// 		}

// 		if (strcmp(command[0], "RQ") == 0)
// 		{
// 			requestResources(command, customer, available);
// 			printf("Requested resources \n");
// 		}
// 		else if (strcmp(command[0], "RL") == 0)
// 		{
// 			printf("released resources \n");
// 		}
// 		else if (strcmp(command[0], "*") == 0)
// 		{
// 			printf("requested status\n");
// 		}
// 		else if (strcmp(command[0], "Run") == 0)
// 		{
// 			printf("running a safe sequence\n");
// 		}
// 		else if (strcmp(command[0], "exit") == 0)
// 		{
// 			printf("exiting the program\n");
// 		}
// 		else
// 		{

// 			printf("\n token is %s \n", token);
// 			printf("invalid entry, please try again\n");
// 		}

// 	} while (strcmp(command[0], "exit") != 0);

// 	printf("made it with %d \n", customerCount);
// 	return 0;
// }

int **readFile(char *fileName)
{
	FILE *in = fopen(fileName, "r");
	if (!in)
	{
		printf("Error in opening the input file...exit with error code -1\n");
		return -1;
	}

	struct stat st;
	fstat(fileno(in), &st);
	char *content_of_file = (char *)malloc(((int)st.st_size + 1) * sizeof(char));
	content_of_file[0] = '\0';
	while (!feof(in))
	{
		char line[100];
		if (fgets(line, 100, in) != NULL)
		{
			strncat(content_of_file, line, strlen(line));
		}
	}
	fclose(in);
	char *command = NULL;
	char *copy_file = (char *)malloc((strlen(content_of_file) + 1) * sizeof(char));
	strcpy(copy_file, content_of_file);
	command = strtok(copy_file, "\r\n");
	while (command != NULL)
	{
		customer_amount++;
		command = strtok(NULL, "\r\n");
	}

	strcpy(copy_file, content_of_file);
	char *lines[customer_amount];
	int i = 0;
	command = strtok(fileContent, "\r\n");
	while (command != NULL)
	{
		lines[i] = malloc(sizeof(command) * sizeof(char));
		strcpy(lines[i], command);
		i++;
		command = strtok(NULL, "\r\n");
	}
	int **max = malloc(sizeof(int *) * customer_amount);
	for (int k = 0; k < customer_amount; k++)
	{
		int *variable = malloc(sizeof(int *) * customer_amount);
		char *token = NULL;
		int j = 0;
		token = strtok(lines[k], ",");
		while (token != NULL)
		{
			variable[j] = atoi(token);
			j++;
			token = strtok(NULL, ",");
		}
		max[k] = variable;
	}

	return max;
}

int safteyAlgorithm(){
	int *running = malloc(sizeof(int) * resource_amount);
	int *seq = malloc(sizeof(int) * customer_amount);
	int *done = malloc(sizeof(int) * customer_amount);

	for (int i = 0; i <resource_amount; i++){
		running[i] = available[i];
	}
	int amount = 0;
	while (amount < customer_amount){
		int safe_flag = 0;
		for (int i =0; i < customer_amount; i++){
			if (done[0] == 0){
				int safe_int = 1;
				for (int j = 0; j < resource_amount; j++){
					if (need[i][j] > running[j]){
						safe_int = 0;
						break;
					}
				}
				if (safe_int ==1){
				seq[amount] = i;
				done[i] = 1;
				amount = amount +1;
				safe_flag = 1;
				for (int j =0; j < resource_amount; j++){
					running[j] += allocated[i][j];
					}
				}
			}
		}
	if (safe_flag ==0){
		for (int l =0; l< customer_amount; l++){
			seq[l]=-1;
		}
		free(running);
		free(done);
		return seq
	}
	}
	free(running);
	free(done);
	return seq
}

void *runThread(void *t){
	int thread_id = (int *)t;
	printf("--> Customer/Thread: %d\n", *thread_id);
	printf("	allocated resources: ");
	for (int i =0;i< resource_amount; i++){
		printf("	%d", allocated[*thread_id][i]);
	}
	printf("\n");
	printf("	Needed: ");
	for (int i =0;i< resource_amount; i++){
		printf("	%d", need[*thread_id][i]);
	}
	printf("\n");
	printf("	Available: ");
	for (int i =0;i< resource_amount; i++){
		printf("	%d", available[i]);
	}
	printf("\n");
	printf("    Thread has started\n");
    sleep(0.5);
    printf("    Thread has finished\n");
    sleep(0.5);
    printf("    Thread is releasing resources\n");
    sleep(0.5);
    printf("    New Available: ");
	for (int i =0 ; i<resource_amount;  i++){
		available[i] += allocated[*thread_id][i];
		printf("%d", available[i]);
	}
	printf("\n");
	pthread_exit(NULL);
}

// int requestResources(char **command, Customer *customers, int *available)
// {

// 	// command 1 = customer
// 	// command 2-5 = resources

// 	// need to implement a semaphore or something on AVAILABLE and CUSTOMER

// 	/* to do
// 1) verify request is less than total available -> or will fail
// 2) verify request does not exceed maximum for the thread (so needs to be < need)
// 3) if we meet both of the above we allocate, else we return -1

// */
// 	for (int i = 0; i < 4; i++)
// 	{
// 		printf("in this loop\n");
// 		char *value = command[i + 2];
// 		printf("%c", *value);
// 		printf("got to this spot\n");
// 		int resources = atoi(value);

// 		printf("in the first loop \n");
// 		printf("value is %d\n", resources);
// 		printf("fault above\n");
// 		if (atoi(value) > available[i])
// 		{
// 			printf("did I get in this loop\n");
// 			return -1;
// 		}
// 	}

// 	printf("got between the loops\n");

// 	for (int j = 0; j < 4; j++)
// 	{
// 		if (available[j] > customers[atoi(command[2])].need[j])
// 		{
// 			return -1;
// 		}
// 	}

// 	printf("need is %d\n", customers[0].need[0]);

// 	printf("got here! yEAH!!!\n\n");

// 	return 0;
// }
