/* Tim Martin and Matt Paek*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <time.h>
#include <semaphore.h>

#define MAXIMUM_SIZE 256
#define FILE_NAME "sample4_in.txt"

int resource_amount;
int customer_amount;
int **max;
int **allocated;
int **need;
int *seq;
int *available;
int is_safe;

void processFile(char *file_name);
void *runThread(void *t);
int *safetyAlgorithm();

// // function headers

int read_file_count(char *fileName); // function to read input file and determine count


int main(int argc, char *argv[])
{
	if (argc < 2){
		printf("No file name given... exiting \n");
		return -1;
	}

	resource_amount = argc - 1; //set up the array of available resources
	available = malloc(sizeof(int) * resource_amount);
	for (int i = 1; i < argc; i++){
		available[i - 1] = atoi(argv[i]);
	}

	customer_amount = read_file_count(FILE_NAME);

	processFile(FILE_NAME); //read file contents and populate data

	allocated = malloc(sizeof(int *) * customer_amount);
	need = malloc(sizeof(int *) * customer_amount);
	for (int i = 0; i < customer_amount; i++){
		allocated[i] = malloc(sizeof(int) * resource_amount);
		need[i] = malloc(sizeof(int) * resource_amount);
	}

	is_safe = 0; //start safe flag at 0

	char *user_input = malloc(sizeof(char) * MAXIMUM_SIZE);
	printf("Number of Customers: %d\n", customer_amount);
	printf("Currently Available resources: ");
	for (int i = 0; i < resource_amount; i++){
		printf("%d", available[i]);
		if (i < resource_amount - 1){
			printf(" ");
		}
	}
	printf("\n");
	
	printf("Maximum resources from file:\n");
	for (int i = 0; i < customer_amount; i++){
		printf("in the max loop \n");
		for (int j = 0; j < resource_amount; j++){
			printf("in the inner\n");
			printf("%d", max[i][j]);
			/*
			if (j < customer_amount - 1){
				printf(" ");
			}*/
		}
		printf("\n");
	}

	
	while (1) //loop to ask for commands from user for RQ, RL, Status and Exit
	{
		printf("Commands are, 'RQ'request resources, 'RL' release resources, 'Status' output current info, 'Run' to calculate a safe sequence, or 'exit' to quit\n");
		printf("Enter command: \n");
		fgets(user_input, MAXIMUM_SIZE, stdin);
		if (user_input[strlen(user_input) - 1] == '\n' && strlen(user_input) > 0){
			user_input[strlen(user_input) - 1] = '\0';
		}
		
		if (strstr(user_input, "RQ")){ //RQ command
			int amount = 0;
			int *user_input_arr = malloc(sizeof(int) * (resource_amount +1));
			char *token = NULL;
			token = strtok(user_input, " ");
			while (token != NULL){
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
			} else if (strstr(user_input, "RL")){ //RL command
				int amount = 0;
				int *user_input_arr = malloc(sizeof(int) * (resource_amount +1));
				char *token = NULL;
				token = strtok(user_input, " ");
				while (token != NULL){
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
			} else if (strstr(user_input, "Status")){ //Status command
				printf("Currently Available resources: ");
				for (int i = 0; i < resource_amount; i++){
					printf("%d", available[i]);
					if (i < resource_amount - 1){
						printf(" ");
					}
					printf("\n");
				}
				printf("Maximum resources:\n");
				for (int i = 0; i < customer_amount; i++){
					for (int j = 0; j < resource_amount; j++){
						printf("%d", max[i][j]);
						if (j < customer_amount - 1){
							printf(" ");
						}
					}
					printf("\n");
				}
				printf("Allocation resources:\n");
				for (int i = 0; i < customer_amount; i++){
					for (int j = 0; j < resource_amount; j++){
						printf("%d", allocated[i][j]);
						if (j < customer_amount - 1){
							printf(" ");
						}
					}
					printf("\n");
				}
				printf("Need resources:\n");
				for (int i = 0; i < customer_amount; i++){
					for (int j = 0; j < resource_amount; j++){
						printf("%d", need[i][j]);
						if (j < customer_amount - 1){
							printf(" ");
						}
					}
					printf("\n");
				}

			} else if (strstr(user_input, "Run")){ //Run command
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
			} else if (strstr(user_input, "exit")){
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

int read_file_count(char *fileName)
{
	int count = 0;
	char c;
	
	FILE *fp = fopen(fileName, "r");
	if (!fp)
	{
	printf("Error in opening the input file...exit with error code -1\n");
		return -1;
	}
	
	while(!feof(fp)){
	
	c = fgetc(fp);
		if (c == 'n')
		{
			count ++;
		}
	
	}
	
	fclose(fp);
	return count;
}


void processFile(char *fileName)
{
	FILE *in = fopen(fileName, "r");

	/*removed this as we process the file already so we know it works
	if (!in)
	{
		printf("Error in opening the input file...exit with error code -1\n");
		return -1;
	}
	*/
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
	command = strtok(content_of_file, "\r\n");
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

	return;
}

int* safetyAlgorithm(){ //bankers algorithm for resource management
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
		return seq;
	}
	}
	free(running);
	free(done);
	return seq;
}

void *runThread(void *t){ //thread used for the Run command
	int *thread_id = (int *)t;
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


