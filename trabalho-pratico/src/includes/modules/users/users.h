/**
 * @file users.h
 * @brief Header quer inicializa as funções do ficheiro users.c
 */

#ifndef USERS_H
#define USERS_H

#include "user.h"
#include "../../hashtable/hashtable.h"

/** Definição dos catálogos dos users */
typedef struct users *USERS;

USERS create_users();
void set_users();
void delete_users();
void insert_user(USERS rs, char* key, USER r);
USER get_user(USERS rs, char* key);
USER get_user_no_mem_cpy(USERS rs, char* key);
ht* get_users_table(USERS rs);
#endif