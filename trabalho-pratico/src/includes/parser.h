/**
 * @file parser.h
 * @brief Header quer inicializa as funções do ficheiro parser.c
 */

#ifndef PARSER_H
#define PARSER_H

char *strsep(char **stringp, const char *delim);
int is_valid_date(char *s);
int is_valid_number(char *s);
int is_valid_gender(char *s);
int is_valid_payment_method(char *s);
int is_valid_car_class(char *s);
int is_valid_license_plate(char *s);
int is_valid_account_status(char *s);
int is_valid_name(char *s);
int elem_list(char *s);
void remove_possible_new_line(char *line);
int hash(char *key);

int file_count(char *file);
void insertion_sort(int arr[], int n);

#endif
