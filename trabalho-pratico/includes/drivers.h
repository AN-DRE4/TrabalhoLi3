/**
 * @file drivers.h
 * @brief Header quer inicializa as funções do ficheiro drivers.c
 */

#ifndef DRIVERS_H
#define DRIVERS_H

#include "driver.h"
//#include "../../hashtable/hashtable.h"
#include "hashtable.h"

/** Definição dos catálogos dos drivers */
typedef struct drivers *DRIVERS;

DRIVERS create_drivers();
void set_drivers();
void delete_drivers();
void insert_driver(DRIVERS rs, char* key, DRIVER r);
DRIVER get_driver(DRIVERS rs, char* key);
DRIVER get_driver_no_mem_cpy(DRIVERS rs, char* key);
ht* get_drivers_table(DRIVERS rs);
#endif