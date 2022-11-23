/**
 * @file ride.h
 * @brief Header quer inicializa as funções do ficheiro ride.c
 */

#ifndef RIDER_H
#define RIDER_H

#define RIDER_COLUMNS 10

/** Definição dos módulos do driversitório */
typedef struct ride *RIDE;

void *create_ride();
void delete_ride(void *r);

void set_ride_id(RIDE r, char *s);
void set_ride_date(RIDE r, char *s);
void set_ride_driver(RIDE r, char *s);
void set_ride_user(RIDE r, char *s);
void set_ride_city(RIDE r, char *s);
void set_ride_distance(RIDE r, char *s);
void set_ride_score_user(RIDE r, char *s);
void set_ride_score_driver(RIDE r, char *s);
void set_ride_tip(RIDE r, char *s);
void set_ride_comment(RIDE r, char *s);

char *get_ride_id(void *r);
char *get_ride_driver(void *r);
char *get_ride_user(void *r);
char *get_ride_distance(void *r);
char *get_ride_score_user(void *r);
char *get_ride_score_driver(void *r);
char *get_ride_tip(void *r);

void set_ride(RIDE r, char *s);

int is_valid_ride(RIDE r);

void copy_ride(void* dst, void* src);

#endif