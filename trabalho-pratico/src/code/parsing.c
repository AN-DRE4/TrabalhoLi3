/**
 * @file parsing.c
 * @brief Ficheiro que contém as funções para a criação dos catálogos
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
/*#include "define.h"
#include "parsing.h"
#include "parser.h"
#include "modules/users/users.h"
#include "modules/drivers/drivers.h"
#include "modules/rides/rides.h"
#include "modules/users/user.h"
#include "modules/drivers/driver.h"
#include "modules/rides/ride.h"
#include "hashtable/hashtable.h"*/

#include "../../includes/define.h"
#include "../../includes/parsing.h"
#include "../../includes/parser.h"
#include "../../includes/users.h"
#include "../../includes/drivers.h"
#include "../../includes/rides.h"
#include "../../includes/user.h"
#include "../../includes/driver.h"
#include "../../includes/ride.h"
#include "../../includes/hashtable.h"


#include <time.h>

/**
 * @brief Função create_users_catalog
 *
 * Criação dos catálogos dos users
 * //Recebe como parametros o endereco de inteiros bots, orgs e useres em que sao posteriormente
 * //incrementados ao longo da criacao do catalgo o numero de bots, orgs e users existentes no catalogo
 * 
 * Linhas acima nao necessarias, visto que nao sao utilizadas
 * 
 * @returns catálogo dos users
 */
//USERS create_users_catalog(int *bots, int *orgs, int *users) {
USERS create_users_catalog(char *users_path) {

	USERS us = create_users();
	
	char line[LINE_BUFFER];

	FILE *f = fopen(users_path, "r");

	fgets(line, LINE_BUFFER, f);
	
	while(fgets(line, LINE_BUFFER, f) != NULL) {
		remove_possible_new_line(line);

        USER u = create_user();
		set_user(u, line);

        char *key = get_user_username(u);
		
		insert_user(us, key, u);

		free(key);
	 }
	fclose(f);

	return us;
}

/**
 * @brief Função create_drivers_catalog
 * 
 * Criação do catálogo dos drivers
 * 
 * @returns catálogo dos drivers
 */

DRIVERS create_drivers_catalog(char *drivers_path) {
	
	DRIVERS dr = create_drivers();

	char line[LINE_BUFFER];

	FILE *f = fopen(drivers_path, "r");

	fgets(line, LINE_BUFFER, f);
	
	while(fgets(line, LINE_BUFFER, f) != NULL) {
		remove_possible_new_line(line);

		DRIVER d = create_driver();
		set_driver(d, line);

		char *key = get_driver_id(d);
		
		insert_driver(dr, key, d);

		free(key);
	 }
	fclose(f);

	return dr;
}

/**
 * @brief Função create_rides_catalog
 * 
 * Criação do catálogo das rides
 * 
 * @returns catálogo das rides
 */

RIDES create_rides_catalog(USERS us, DRIVERS ds, ht *ht_user_ride, ht *ht_driver_ride, char *rides_path) {


	printf("%s\n", rides_path);

	double time_spent = 0.0;
	clock_t begin = clock();
	
	RIDES rd = create_rides();

	char line[LINE_BUFFER];

	FILE *f = fopen(rides_path, "r");
	if (f == NULL)
	{
		printf("Error, %s doesn't exist.\n", rides_path);
		return 0;
	}

	fgets(line, LINE_BUFFER, f);

	int count = 0;
	int count2 = 0;
	
	printf("Creating rides catalog...\n");

	while(fgets(line, LINE_BUFFER, f) != NULL) {
		remove_possible_new_line(line);

		RIDE r = create_ride();
		set_ride(r, line);

		char *key = get_ride_id(r);

		/*Tabela do user ride*/

		char *user_id = get_ride_user(r);

		USER u = get_user_no_mem_cpy(us, user_id);

		if(u != NULL) {
			ht_insert_2(ht_user_ride, user_id, r);
		}

		free(user_id);

		/*Tabela do driver ride*/

		char *driver_id = get_ride_driver(r);

		DRIVER d = get_driver_no_mem_cpy(ds, driver_id);

		if(d != NULL) {
			ht_insert_2(ht_driver_ride, driver_id, r);
		}

		free(driver_id);
		
		insert_ride(rd, key, r);

		free(key);
	}

	fclose(f);

	clock_t end = clock();
	time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
	printf("The elapsed time to create the rides table is %f seconds\n", time_spent);

	return rd;
}

