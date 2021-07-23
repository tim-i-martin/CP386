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
	int max_demand[4]; 	// note maximum demand for a customer
	
}  Customer;









int readFile(char *fileName, Customer **customers);  // function to read input file


int main(int argc, char *argv[]){
	printf("in the main\n");
	if (argc < 2){
		printf("Input file name missing... exiting with error code -1\n");
		return -1;
	}
	printf("get here\n");
	Customer *customer = NULL;
	int customerCount = readFile(argv[1], &customer);  // processing incoming file

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
			(*customer)[k].max_demand[j] = atoi(token);			
			
			j++;					
			token = strtok(NULL, ",");
		}
	}	
	
	return customerCount;	
	
}



































