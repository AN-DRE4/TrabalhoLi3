/**
 * @file rides.h
 * @brief Header quer inicializa as funções do ficheiro rides.c
 */

#ifndef RIDES_H
#define RIDES_H

#include "ride.h"
#include "../../hashtable/hashtable.h"

/** Definição dos catálogos dos rides */
typedef struct rides *RIDES;

RIDES create_rides();
void set_rides();
void delete_rides();
void insert_ride(RIDES rs, char* key, RIDE r);
RIDE get_ride(RIDES rs, char* key);
RIDE get_ride_no_mem_cpy(RIDES rs, char* key);
ht* get_rides_table(RIDES rs);
#endif