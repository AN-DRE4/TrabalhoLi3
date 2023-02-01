/**
 * @file users.c
 * @brief Ficheiro que contém as funções para a gestão dos catálogos dos users
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
/*#include "../../parser.h"
#include "../../define.h"
#include "user.h"
#include "users.h"
#include "../../hashtable/hashtable.h"*/

#include "../../../../includes/parser.h"
#include "../../../../includes/define.h"
#include "../../../../includes/user.h"
#include "../../../../includes/users.h"
#include "../../../../includes/hashtable.h"

/** 
 * Definição dos catálogos dos users
 * 
 * Contém uma hashtable
 * 
 */
struct users {
    ht *users;
};

/**
 * @brief Função create_users
 *
 * Função que aloca na memoria o espaco necessario para um catalogo de users
 * 
 * @returns catálogo de users
 */
USERS create_users() {
    USERS rs = malloc(sizeof(*rs));
    if (rs == NULL) {
		printf("Erro ao criar users\n");
		return NULL;
    }
    ht *ht = ht_create(TABLE_SIZE);
    rs->users = ht;
    return rs;
}

/**
 * @brief Função delete_users
 *
 * Função que liberta da memoria espaco alocado por uma hashtable de users
 * 
 */
void delete_users(USERS rs) {
    ht_destroy(rs->users, delete_user);
    free(rs);        
}

/**
 * @brief Função insert_user
 *
 * Função que insere um user no seu catálogo
 * 
 */
void insert_user(USERS rs, char* key, USER r){
	ht_insert(rs->users, key, r);
}

/**
 * @brief Função get_user
 *
 * Função que devolve uma nova instancia de um user dentro do catalogo dos users
 * 
 * @returns user
 */
USER get_user(USERS rs, char* key) {
    //printf("Entrei no get_user\n");
    return ht_get(rs->users, key, create_user, copy_user); 
}

/**
 * @brief Função get_user_no_mem_cpy
 *
 * Função que devolve um user dentro do catalogo dos users
 * 
 * @returns user
 */
USER get_user_no_mem_cpy(USERS rs, char* key) {
   return ht_get_no_mem_cpy(rs->users, key); 
}

/**
 * @brief Função get_users_table
 *
 * Função que obtem a hashtable com os users
 * 
 * @returns hashtable de users
 */
ht* get_users_table(USERS rs) {
    return rs->users;
}