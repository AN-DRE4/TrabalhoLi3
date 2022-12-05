/**
 * @file main.c
 * @brief Ficheiro que contém a função principal do programa
 */

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "./includes/modules/drivers/drivers.h"
#include "./includes/modules/users/users.h"
#include "./includes/modules/rides/rides.h"

#include "./includes/modules/drivers/driver.h"
#include "./includes/modules/users/user.h"
#include "./includes/modules/rides/ride.h"

#include "./includes/hashtable/hashtable.h"

#include "./includes/parser.h"
#include "./includes/parsing.h"
#include "./includes/define.h"
#include "./includes/queries/queries.h"

// valgrind --leak-check=full --show-leak-kinds=all -s ./teste 


/**
 * @brief Função fileExists
 *
 * Verifica se ficheiro existe
 * @returns 1 se ficheiro existe
 * @returns 0 se ficheiro não existe
 */
int fileExists(char *filepath)
{
	FILE *f = fopen(filepath, "r");
	if (f == NULL)
	{
		printf("Error, %s doesn't exist.\n", filepath);
		return 0;
	}
	fclose(f);
	return 1;
	
}

/**
 * @brief Função main
 *
 * Ponto de partida da execução do nosso programa.
 * @returns 0
 */
int main (int argc, char * argv[]) {

	double time_spent = 0.0;
	clock_t begin = clock();

	char *folder;

	if(argc == 1) {
		//ask the user for the folder
		printf("Please enter the folder path: ");
		scanf("%s", folder);
	} else {
		// Keep the folder where the csv files are located
		folder = argv[1];
	}

	printf("Folder: %s\n", folder);

	// Keep the path to the drivers.csv file
	char drivers_filepath[100];
	sprintf(drivers_filepath, "%s/%s", folder, DRIVERS_FILEPATH2);

	// Keep the path to the rides.csv file
	char rides_filepath[100];
	sprintf(rides_filepath, "%s/%s", folder, RIDES_FILEPATH2);

	// Keep the path to the users.csv file
	char users_filepath[100];
	sprintf(users_filepath, "%s/%s", folder, USERS_FILEPATH2);

	if (!fileExists(drivers_filepath) || !fileExists(rides_filepath) || !fileExists(users_filepath) || !fileExists(argv[2]))
		return 0;

	if(argc == 1) {
		opt = 0;
	} 
	else if(argc == 3){
		if (fileExists(*argv) == 0) return 0;
		read_queries(argv[2], drivers_filepath, rides_filepath, users_filepath);
	}
	
	clock_t end = clock();
	time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
	printf("The elapsed time is %f seconds\n", time_spent);

	return 0;
}
