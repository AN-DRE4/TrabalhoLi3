#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../../parser.h"
#include "../../define.h"
#include "driver.h"

/** 
 * Definição dos módulos de driver
 * 
 * Contém todos os elementos de um driver, como char*
 * 
 */

struct driver {
    char *id;
    char *name;
    char *birth_day;
    char *gender;
    char *car_class;
    char *license_plate;
    char *city;
    char *account_creation;
    char *account_status;
};

/**
 * @brief Função create_driver
 *
 * Função que aloca na memoria o espaco necessario para um driver
 * 
 * @returns driver
 */
void *create_driver() {
	DRIVER d = malloc(sizeof *d);
	return d;
}

/**
 * @brief Função delete_driver
 *
 * Função que liberta da memoria o espaco alocado por um driver
 * 
 */
void delete_driver(void* r) {
	DRIVER rc = (DRIVER)r;
    free(rc->id);
    free(rc->name);
    free(rc->birth_day);
    free(rc->gender);
    free(rc->car_class);
    free(rc->license_plate);
    free(rc->city);
    free(rc->account_creation);
    free(rc->account_status);
	free(rc);
}

/**
 * @brief Função set_driver_id
 *
 * Função que define driver_id
 * 
 */
void set_driver_id(DRIVER r, char *s) {
	r->id = strdup(s);
}

/**
 * @brief Função set_driver_driver_name
 *
 * Função que define driver_driver_name
 * 
 */
void set_driver_name(DRIVER r, char *s) {
	r->name = strdup(s);
}

/**
 * @brief Função set_driver_birth_day
 *
 * Função que define driver_birth_day
 * 
 */
void set_driver_birth_day(DRIVER r, char *s) {
	r->birth_day = strdup(s);
}

/**
 * @brief Função set_driver_gender
 *
 * Função que define driver_gender
 * 
 */
void set_driver_gender(DRIVER r, char *s) {
	r->gender = strdup(s);
}

/**
 * @brief Função set_driver_car_class
 *
 * Função que define driver_car_class
 * 
 */
void set_driver_car_class(DRIVER r, char *s) {
	r->car_class = strdup(s);
}

/**
 * @brief Função set_driver_license_plate
 *
 * Função que define driver_license_plate
 * 
 */
void set_driver_license_plate(DRIVER r, char *s) {
	r->license_plate = strdup(s);
}

/**
 * @brief Função set_driver_city
 *
 * Função que define driver_city
 * 
 */
void set_driver_city(DRIVER r, char *s) {
	r->city = strdup(s);
}

/**
 * @brief Função set_driver_account_creation
 *
 * Função que define driver_account_creation
 * 
 */
void set_driver_account_creation(DRIVER r, char *s) {
	r->account_creation = strdup(s);
}

/**
 * @brief Função set_driver_account_status
 *
 * Função que define driver_account_status
 * 
 */
void set_driver_account_status(DRIVER r, char *s) {
	r->account_status = strdup(s);
}

/**
 * @brief Função set_driver
 *
 * Função que define driver, utilizando as funções anterioes
 * 
 */
void set_driver(DRIVER r, char *line) {
	char *p = NULL, *temp_line = strdup(line), *os;
	os = temp_line;
	int i = 0;
	while ((p = strsep(&temp_line, ";")) != NULL) {
		switch(i) {
			case 0:
				set_driver_id(r, p);
				break;
			case 1:
				set_driver_name(r, p);
				break;
			case 2:
				set_driver_birth_day(r, p);
				break;
			case 3:
				set_driver_gender(r, p);
				break;
			case 4:
				set_driver_car_class(r, p);
				break;
			case 5:
				set_driver_license_plate(r, p);
				break;
			case 6:
				set_driver_city(r, p);
				break;
			case 7:
				set_driver_account_creation(r, p);
				break;
			case 8:
				set_driver_account_status(r, p);
				break;
		}
		i++;
	}	
	free(os);
}

/**
 * @brief Função is_valid_driver
 *
 * Função que verifica se um driver é válida
 * 
 * @returns bool
 */
int is_valid_driver(DRIVER r) {
	if (!(is_valid_number(r->id)))
		return 0;
    if (!(is_valid_gender(r->gender)))
        return 0;
    if (!(is_valid_car_class(r->car_class)))
        return 0;
    if (!(is_valid_license_plate(r->license_plate)))
        return 0;
	if ((!(is_valid_date(r->birth_day))) || (!(is_valid_date(r->account_creation))))
		return 0;
	if (!(is_valid_name(r->name)) || !(is_valid_name(r->city)))
		return 0;
	return 1;
}

/**
 * @brief Função get_driver_id
 *
 * Função que devolve uma copia do id de um driver
 * 
 * @returns id de um driver
 */
char* get_driver_id(void* r) {
	DRIVER rc = (DRIVER) r;
	return strdup(rc->id);
}

/**
 * @brief Função get_driver_name
 * 
 * Função que devolve uma copia do nome de um driver
 * 
 * @returns nome de um driver
 */

char* get_driver_name(void* r) {
	DRIVER rc = (DRIVER) r;
	return strdup(rc->name);
}

/**
 * @brief Função get_driver_gender
 * 
 * Função que devolve uma copia do género de um driver
 * 
 * @returns género de um driver
 */

char *get_driver_gender(void *r) {
	DRIVER rc = (DRIVER) r;
	return strdup(rc->gender);
}

/**
 * @brief Função get_driver_birth_day
 * 
 * Função que devolve uma copia do dia de nascimento de um driver
 * 
 * @returns dia de nascimento de um driver
 */

char *get_driver_birth_day(void *r) {
	DRIVER rc = (DRIVER) r;
	return strdup(rc->birth_day);
}

/**
 * @brief Função get_user_age
 * 
 * Função que retorna a idade de um user
 * 
 * @returns idade
 */

int get_driver_age(void *r) {
    DRIVER rc = (DRIVER) r;
    char *str = get_driver_birth_day(rc);
    char *token = strsep(&str, "/");
    char *dia = token;
    token = strsep(&str, "/");
    char *mes = token;
    token = strsep(&str, "/");
    char *ano = token;

    int dia_i = atoi(dia);
    int mes_i = atoi(mes);
    int ano_i = atoi(ano);

    //Comparar a data de nascimento com a data atual e retornar a idade
    
    if(mes_i > LAST_DATE_MES || (mes_i == LAST_DATE_MES && dia_i >= LAST_DATE_DIA))
        return LAST_DATE_ANO - ano_i;
    else
        return LAST_DATE_ANO - ano_i - 1;
}

/**
 * @brief Função get_driver_car_class
 *
 * Função que devolve uma copia do car_class de um driver
 * 
 * @returns car_class de um driver
 */
char* get_driver_car_class(void* r) {
	DRIVER rc = (DRIVER) r;
	return strdup(rc->car_class);
}

/**
 * @brief Função get_driver_account_status
 * 
 * Função que devolve uma copia do account_status de um driver
 * 
 * @returns account_status de um driver
 */

char* get_driver_account_status(void* r) {
	DRIVER rc = (DRIVER) r;
	return strdup(rc->account_status);
}

/**
 * @brief Função copy_driver
 *
 * Função que copia um driver para outro
 * 
 */
void copy_driver(void* dst, void* src) {
	DRIVER dstc = (DRIVER) dst;
	DRIVER srcc = (DRIVER) src;
	dstc->id = strdup(srcc->id);
    dstc->name = strdup(srcc->name);
    dstc->birth_day = strdup(srcc->birth_day);
	dstc->gender = strdup(srcc->gender);
	dstc->car_class = strdup(srcc->car_class);
	dstc->license_plate = strdup(srcc->license_plate);
	dstc->city = strdup(srcc->city);
	dstc->account_creation = strdup(srcc->account_creation);
	dstc->account_status = strdup(srcc->account_status);
}
