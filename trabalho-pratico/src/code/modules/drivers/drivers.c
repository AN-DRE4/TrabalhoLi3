/**
 * @file drivers.c
 * @brief Ficheiro que contém as funções para a gestão dos catálogos dos drivers
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
/*#include "../../parser.h"
#include "../../define.h"
#include "driver.h"
#include "drivers.h"
#include "../../hashtable/hashtable.h"*/

#include "../../../../includes/parser.h"
#include "../../../../includes/define.h"
#include "../../../../includes/driver.h"
#include "../../../../includes/drivers.h"
#include "../../../../includes/hashtable.h"

/** 
 * Definição dos catálogos dos drivers
 * 
 * Contém uma hashtable
 * 
 */
struct drivers {
    ht *drivers;
};

/**
 * @brief Função create_drivers
 *
 * Função que aloca na memoria o espaco necessario para um catalogo de drivers
 * 
 * @returns catálogo de drivers
 */
DRIVERS create_drivers() {
    DRIVERS rs = malloc(sizeof(*rs));
    if (rs == NULL) {
		printf("Erro ao criar drivers\n");
		return NULL;
    }
    ht *ht = ht_create(TABLE_SIZE);
    rs->drivers = ht;
    return rs;
}

/**
 * @brief Função delete_drivers
 *
 * Função que liberta da memoria espaco alocado por uma hashtable de drivers
 * 
 */
void delete_drivers(DRIVERS rs) {
    ht_destroy(rs->drivers, delete_driver);
    free(rs);        
}

/**
 * @brief Função insert_driver
 *
 * Função que insere um driver no seu catálogo
 * 
 */
void insert_driver(DRIVERS rs, char* key, DRIVER r){
	ht_insert(rs->drivers, key, r);
}

/**
 * @brief Função get_driver
 *
 * Função que devolve uma nova instancia de um driver dentro do catalogo dos drivers
 * 
 * @returns driver
 */
DRIVER get_driver(DRIVERS rs, char* key) {
   return ht_get(rs->drivers, key, create_driver, copy_driver); 
}

/**
 * @brief Função get_driver_no_mem_cpy
 *
 * Função que devolve um driver dentro do catalogo dos drivers
 * 
 * @returns driver
 */
DRIVER get_driver_no_mem_cpy(DRIVERS rs, char* key) {
   return ht_get_no_mem_cpy(rs->drivers, key); 
}

/**
 * @brief Função get_drivers_table
 *
 * Função que obtem a hashtable com os drivers
 * 
 * @returns hashtable de drivers
 */
ht* get_drivers_table(DRIVERS rs) {
    return rs->drivers;
}