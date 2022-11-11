/**
 * @file parsing.h
 * @brief Header que com as assinaturas as funções do ficheiro parsing.c
 */

#ifndef PARSING_H
#define PARSING_H

#include "modules/users/user.h"
#include "modules/drivers/driver.h"
#include "modules/rides/ride.h"

#include "hashtable/hashtable.h"

USER create_users_catalog();
DRIVER create_drivers_catalog();
RIDE create_rides_catalog(USER us, DRIVER ds, ht *ht_user_ride, ht *ht_driver_ride);

#endif