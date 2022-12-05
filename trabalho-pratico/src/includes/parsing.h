/**
 * @file parsing.h
 * @brief Header que com as assinaturas as funções do ficheiro parsing.c
 */

#ifndef PARSING_H
#define PARSING_H

#include "modules/users/users.h"
#include "modules/drivers/drivers.h"
#include "modules/rides/rides.h"

#include "hashtable/hashtable.h"

USERS create_users_catalog(char* users_path);
DRIVERS create_drivers_catalog(char* drivers_path);
RIDES create_rides_catalog(USERS us, DRIVERS ds, ht *ht_user_ride, ht *ht_driver_ride, char* rides_path);
#endif
