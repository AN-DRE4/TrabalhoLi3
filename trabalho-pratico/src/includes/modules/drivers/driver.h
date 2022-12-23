/**
 * @file driver.h
 * @brief Header quer inicializa as funções do ficheiro driver.c
 */

#ifndef DRIVER_H
#define DRIVER_H

#define DRIVER_COLUMNS 9

/** Definição dos módulos do driversitório */
typedef struct driver *DRIVER;

void *create_driver();
void delete_driver(void* r);

void set_driver_id(DRIVER r, char *s);
void set_driver_name(DRIVER r, char *s);
void set_driver_birth_day(DRIVER r, char *s);
void set_driver_gender(DRIVER r, char *s);
void set_driver_car_class(DRIVER r, char *s);
void set_driver_licence_plate(DRIVER r, char *s);
void set_driver_city(DRIVER r, char *s);
void set_driver_account_creation(DRIVER r, char *s);
void set_driver_account_status(DRIVER r, char *s);

char* get_driver_id(void* r);
char* get_driver_name(void* r);
char* get_driver_gender(void* r);
char* get_driver_birth_day(void* r);
int get_driver_age(void *r);
char* get_driver_car_class(void* r);
char* get_driver_licence_plate(void* r);
char* get_driver_city(void* r);
char* get_driver_account_creation(void* r);
int get_driver_account_age(void *r);
char* get_driver_account_status(void* r);

void set_driver(DRIVER r, char *line);

int is_valid_driver(DRIVER r);

void copy_driver(void* dst, void* src);
#endif
