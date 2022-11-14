/**
 * @file rides.c
 * @brief Ficheiro que contém as funções para a gestão dos catálogos dos rides
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../../parser.h"
#include "../../define.h"
#include "ride.h"
#include "rides.h"
#include "../../hashtable/hashtable.h"

/** 
 * Definição dos catálogos dos rides
 * 
 * Contém uma hashtable
 * 
 */
struct rides {
    ht *rides;
};

/**
 * @brief Função create_rides
 *
 * Função que aloca na memoria o espaco necessario para um catalogo de rides
 * 
 * @returns catálogo de rides
 */
RIDES create_rides() {
    RIDES rs = malloc(sizeof(*rs));
    if (rs == NULL) {
        printf("Erro ao criar rides\n");
        return NULL;
    }
    ht *ht = ht_create(TABLE_SIZE);
    rs->rides = ht;
    return rs;
}

/**
 * @brief Função delete_rides
 *
 * Função que liberta da memoria espaco alocado por uma hashtable de rides
 * 
 */
void delete_rides(RIDES rs) {
    ht_destroy(rs->rides, delete_ride);
    free(rs);        
}

/**
 * @brief Função insert_ride
 *
 * Função que insere um ride no seu catálogo
 * 
 */
void insert_ride(RIDES rs, char* key, RIDE r){
    ht_insert(rs->rides, key, r);
}

/**
 * @brief Função get_ride
 *
 * Função que devolve uma nova instancia de um ride dentro do catalogo dos rides
 * 
 * @returns ride
 */
RIDE get_ride(RIDES rs, char* key) {
   return ht_get(rs->rides, key, create_ride, copy_ride); 
}

/**
 * @brief Função get_ride_no_mem_cpy
 *
 * Função que devolve um ride dentro do catalogo dos rides
 * 
 * @returns ride
 */
RIDE get_ride_no_mem_cpy(RIDES rs, char* key) {
   return ht_get_no_mem_cpy(rs->rides, key); 
}

/**
 * @brief Função get_rides_table
 *
 * Função que obtem a hashtable com os rides
 * 
 * @returns hashtable de rides
 */
ht* get_rides_table(RIDES rs) {
    return rs->rides;
}