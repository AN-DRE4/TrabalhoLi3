/**
 * @file user.h
 * @brief Header quer inicializa as funções do ficheiro user.c
 */

#ifndef USER_H
#define USER_H

#define USER_COLUMNS 7

/** Definição dos módulos do driversitório */
typedef struct user *USER;

void *create_user();
void delete_user(void *r);

void set_user_username(USER u, char *s);
void set_user_name(USER u, char *s);
void set_user_gender(USER u, char *s);
void set_user_birth_date(USER u, char *s);
void set_user_account_creation(USER u, char *s);
void set_user_pay_method(USER u, char *s);
void set_user_account_status(USER u, char *s);

char *get_user_username(void *r);
char *get_user_name(void *r);
char *get_user_gender(void *r);
char *get_user_birth_date(void *r);
int get_user_age(void *r);
char *get_user_account_creation(void *r);
int get_user_account_age(void *r);
char *get_user_account_status(void *r);

void set_user(USER r, char *s);

int is_valid_user(USER r);

void copy_user(void* dst, void* src);
#endif