/**
 * @file parsing.c
 * @brief Ficheiro que contém as funções para a criação dos catálogos
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "define.h"
#include "parsing.h"
#include "parser.h"
#include "modules/users/user.h"
#include "modules/drivers/driver.h"
#include "modules/rides/ride.h"
#include "modules/users/user.h"
#include "modules/drivers/driver.h"
#include "modules/rides/ride.h"
#include "hashtable/hashtable.h"

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
USER create_users_catalog() {

	USER us = create_users();
	
	char line[LINE_BUFFER];

	FILE *f = fopen(USERS_FILEPATH, "r");

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

DRIVER create_drivers_catalog() {
	
	DRIVER dr = create_drivers();

	char line[LINE_BUFFER];

	FILE *f = fopen(DRIVERS_FILEPATH, "r");

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

RIDE create_rides_catalog(USER us, DRIVER ds, ht *ht_user_ride, ht *ht_driver_ride) {
	
	RIDE rd = create_rides();

	char line[LINE_BUFFER];

	FILE *f = fopen(RIDES_FILEPATH, "r");

	fgets(line, LINE_BUFFER, f);
	
	while(fgets(line, LINE_BUFFER, f) != NULL) {
		remove_possible_new_line(line);

		RIDE r = create_ride();
		set_ride(r, line);

		char *key = get_ride_id(r);

		/*Tabela do user ride*/

		char *user_id = get_ride_user(r);

		USER u = get_user_no_mem_cpy(us, user_id);

		if(u != NULL) {
			ht_insert_3(ht_user_ride, user_id, r);
		}

		free(user_id);

		/*Tabela do driver ride*/

		char *driver_id = get_ride_driver(r);

		DRIVER d = get_driver_no_mem_cpy(ds, driver_id);

		if(d != NULL) {
			ht_insert_3(ht_driver_ride, driver_id, r);
		}

		free(driver_id);
		
		insert_ride(rd, key, r);

		free(key);
	 }
	fclose(f);

	return rd;
}
