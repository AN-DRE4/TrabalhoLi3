#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../../parser.h"
#include "../../define.h"
#include "ride.h"

/** 
 * Definição dos módulos de driver
 * 
 * Contém todos os elementos de um driver, como char*
 * 
 */

struct ride {
    char *id;
    char *date;
    char *driver;
    char *user;
    char *city;
    char *distance;
    char *score_user;
    char *score_driver;
    char *tip;
    char *comment;
};

/**
 * @brief Função create_ride
 *
 * Função que aloca na memoria o espaco necessario para uma ride
 * 
 * @returns driver
 */
void *create_ride() {
	RIDE d = malloc(sizeof *d);
	return d;
}

/**
 * @brief Função delete_ride
 *
 * Função que liberta da memoria o espaco alocado por uma ride
 * 
 */

void delete_ride(void* r) {
    RIDE rc = (RIDE)r;
    free(rc->id);
    free(rc->date);
    free(rc->driver);
    free(rc->user);
    free(rc->city);
    free(rc->distance);
    free(rc->score_user);
    free(rc->score_driver);
    free(rc->tip);
    free(rc->comment);
    free(rc);
}

/**
 * @brief Função set_ride_id
 *
 * Função que atribui um id a uma ride
 * 
 * @param r ride
 * @param s id
 */
void set_ride_id(RIDE r, char *s) {
    r->id = strdup(s);
}

/**
 * @brief Função set_ride_date
 *
 * Função que atribui uma data a uma ride
 * 
 * @param r ride
 * @param s data
 */

void set_ride_date(RIDE r, char *s) {
    r->date = strdup(s);
}

/**
 * @brief Função set_ride_driver
 *
 * Função que atribui um driver a uma ride
 * 
 * @param r ride
 * @param s driver
 */

void set_ride_driver(RIDE r, char *s) {
    r->driver = strdup(s);
}

/**
 * @brief Função set_ride_user
 *
 * Função que atribui um user a uma ride
 * 
 * @param r ride
 * @param s user
 */

void set_ride_user(RIDE r, char *s) {
    r->user = strdup(s);
}

/**
 * @brief Função set_ride_city
 *
 * Função que atribui uma cidade a uma ride
 * 
 * @param r ride
 * @param s cidade
 */

void set_ride_city(RIDE r, char *s) {
    r->city = strdup(s);
}

/**
 * @brief Função set_ride_distance
 *
 * Função que atribui uma distancia a uma ride
 * 
 * @param r ride
 * @param s distancia
 */

void set_ride_distance(RIDE r, char *s) {
    r->distance = strdup(s);
}

/**
 * @brief Função set_ride_score_user
 *
 * Função que atribui uma pontuação ao user de uma ride
 * 
 * @param r ride
 * @param s pontuação
 */

void set_ride_score_user(RIDE r, char *s) {
    r->score_user = strdup(s);
}

/**
 * @brief Função set_ride_score_driver
 *
 * Função que atribui uma pontuação ao driver de uma ride
 * 
 * @param r ride
 * @param s pontuação
 */

void set_ride_score_driver(RIDE r, char *s) {
    r->score_driver = strdup(s);
}

/**
 * @brief Função set_ride_tip
 *
 * Função que atribui um tip a uma ride
 * 
 * @param r ride
 * @param s tip
 */

void set_ride_tip(RIDE r, char *s) {
    r->tip = strdup(s);
}

/**
 * @brief Função set_ride_comment
 *
 * Função que atribui um comentario a uma ride
 * 
 * @param r ride
 * @param s comentario
 */

void set_ride_comment(RIDE r, char *s) {
    r->comment = strdup(s);
}

/**
 * @brief Função set_ride
 * 
 * Função que atribui todos os elementos de uma ride
 * 
 * @param r ride
 * @param s id
*/

void set_ride(RIDE r, char *s) {
    char *p = NULL, *temp_line = strdup(s), *os;
	os = temp_line;
	int i = 0;
    while ((p = strsep(&temp_line, ";")) != NULL) {
        switch (i)
        {
        case 0:
            set_ride_id(r, p);
            break;
        case 1:
            set_ride_date(r, p);
            break;
        case 2:
            set_ride_driver(r, p);
            break;
        case 3:
            set_ride_user(r, p);
            break;
        case 4:
            set_ride_city(r, p);
            break;
        case 5:
            set_ride_distance(r, p);
            break;
        case 6:
            set_ride_score_user(r, p);
            break;
        case 7:
            set_ride_score_driver(r, p);
            break;
        case 8:
            set_ride_tip(r, p);
            break;
        case 9:
            set_ride_comment(r, p);
            break;
        }
        i++;
    }
    free(os);
}

/**
 * @brief Função is_valid_ride
 *
 * Função que verifica se uma ride é valida
 * 
 * @param r ride
 * @returns 1 se a ride for valida, 0 caso contrario
 */

int is_valid_ride(RIDE r) {
	if (!(is_valid_number(r->id)) && 
        !(is_valid_number(r->driver)) &&
        !(is_valid_number(r->distance)) && 
        !(is_valid_number(r->score_user)) && 
        !(is_valid_number(r->score_driver)) && 
        !(is_valid_number(r->tip))) 
		return 0;
	if (!(is_valid_date(r->date)))
		return 0;
	if (!(is_valid_name(r->user) && is_valid_name(r->city)))
		return 0;
	return 1;
}

/**
 * @brief Função get_ride_id
 *
 * Função que retorna o id de uma ride
 * 
 * @param r ride
 * @returns id da ride
 */

char *get_ride_id(void *r) {
    RIDE rc = (RIDE) r;
	return strdup(rc->id);
}

/**
 * @brief Função get_ride_driver
 * 
 * Função que retorna o driver de uma ride
 * 
 * @param r ride
 * 
 * @returns driver da ride
 */

char *get_ride_driver(void *r) {
    RIDE rc = (RIDE) r;
    return strdup(rc->driver);
}

/**
 * @brief Função get_ride_user
 * 
 * Função que retorna o user de uma ride
 * 
 * @param r ride
 * 
 * @returns user da ride
 */

char *get_ride_user(void *r) {
    RIDE rc = (RIDE) r;
    return strdup(rc->user);
}

/**
 * @brief Função get_ride_distance
 * 
 * Função que retorna a distancia de uma ride
 * 
 * @param r ride
 * 
 * @returns distancia da ride
 */

char *get_ride_distance(void *r) {
    RIDE rc = (RIDE) r;
    return strdup(rc->distance);
}

/**
 * @brief Função get_ride_score_user
 * 
 * Função que retorna a pontuação do user de uma ride
 * 
 * @param r ride
 * @returns score do user
 */

char *get_ride_score_user(void *r) {
    RIDE rc = (RIDE) r;
    return strdup(rc->score_user);
}

/**
 * @brief Função get_ride_score_driver
 * 
 * Função que retorna a pontuação do driver de uma ride
 * 
 * @param r ride
 * @returns score do driver
 */

char *get_ride_score_driver(void *r) {
    RIDE rc = (RIDE) r;
    return strdup(rc->score_driver);
}

/**
 * @brief Função get_ride_tip
 * 
 * Função que retorna o tip de uma ride
 * 
 * @param r ride
 * 
 * @returns tip da ride
 */

char *get_ride_tip(void *r) {
    RIDE rc = (RIDE) r;
    return strdup(rc->tip);
}

/**
 * @brief Função copy_ride
 * 
 * Função que copia uma ride
 * 
 * @param r ride
 * @returns copia da ride
*/

void copy_ride(void* dst, void* src) {
    RIDE r = (RIDE) src;
    RIDE d = (RIDE) dst;
    d->id = strdup(r->id);
    d->date = strdup(r->date);
    d->driver = strdup(r->driver);
    d->user = strdup(r->user);
    d->city = strdup(r->city);
    d->distance = strdup(r->distance);
    d->score_user = strdup(r->score_user);
    d->score_driver = strdup(r->score_driver);
    d->tip = strdup(r->tip);
    d->comment = strdup(r->comment);
}
