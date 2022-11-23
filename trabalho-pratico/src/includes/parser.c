/**
 * @file parser.c
 * @brief Ficheiro que contém as funções auxiliares usadas ao longo do projeto
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "define.h"

#define __XOPEN_SOURCE
#include "time.h"

/**
 * @brief Função sh
 *
 * Função que remove um caracter de uma string
 * 
 */
static void sh (char *str)
{
	char *p = str;
	for (;*p; p++) *p = *(p+1);
}

/**
 * @brief Função strsep
 * 
 * Função que separa uma string por um caracter
 * 
 * @returns string
 */

char *strsep(char **stringp, const char *delim) {
    char *rv = *stringp;
    if (rv) {
        *stringp += strcspn(*stringp, delim);
        if (**stringp)
            *(*stringp)++ = '\0';
        else
            *stringp = 0; }
    return rv;
}

/**
 * @brief Função is_valid_type
 *
 * Função que remove (/n ou /r/n) do final de uma string
 * 
 */
void remove_possible_new_line(char line[]) {
	int len = 0;
	char *s = line;
	while (*s != '\0') { len++; s++; }
	if (line[len-2] == '\r') { //WINDOWS
		sh(s-1);
		sh(s-2);
	} else if (line[len-1] == '\n') // UNIX
		sh(s-1);
}

/**
 * @brief Função is_valid_account_status
 *
 * Função que verifica se o status de uma conta é válido
 * 
 */
int is_valid_account_status(char *s) {
	return (strcmp(s, "active") == 0 || strcmp(s, "inactive") == 0) ? 1 : 0;
}

/**
 * @brief Função remove_spaces
 *
 * Função que retira todos os espaços (' ') de uma dada string
 * 
 * @returns string sem espaços
 */
static char* remove_spaces(char *s) {
	char *p = s;
	while (*p)
	{
		if (*p == ' ')
			sh(p);
		p++;
	}
	return s;
}

/**
 * @brief Função is_valid_date
 *
 * Função que verifica se a string é uma data válida
 * 
 * @returns bool
 */
int is_valid_date(char *s) {
		int year, month, day, hour, minutes, seconds, matches;

	matches = sscanf(s,"%4d-%2d-%2d %2d:%2d:%2d", &year, &month, &day, &hour, &minutes, &seconds);
	if (matches == 6 && ((year == 2005 && month > 4) || (year == 2005 && month == 4 && day >=7) || year > 2005) && year <= 2021 && strlen(s) == 19)
	{
		if ((((month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) && day <= 31) || ((month == 4 || month == 6 || month == 9 || month == 11) && day <= 30) || (month == 2 && ((((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) && day <= 29) || day <= 28))) && day > 0)
		{
			if (hour >= 0 && hour <= 23 && minutes >= 0 && minutes <= 59 && seconds >= 0 && seconds <= 59)
				return 1;
		}

	}
	return 0;
}

/**
 * @brief Função is_valid_number
 *
 * Função que verifica se a string é um número válido
 * 
 * @returns bool
 */
int is_valid_number(char *s) {
	char *p = s;
	if (p[0] == '\0' || p[0] == '-')
		return 0;
	while (*p)
	{
		if (*p < '0' || *p > '9')	
			return 0;
		p++;
	}
	return 1;
}

/**
 * @brief Função is_valid_gender
 *
 * Função que verifica se o genero é válido
 * 
 * @returns bool
 */
int is_valid_gender(char *s) {
	char *p = s;
	if (p[0] == '\0')
		return 0;
	while (*p)
	{
		if (*p != 'M' && *p != 'F')	
			return 0;
		p++;
	}
	return 1;
}

/**
 * @brief Função is_valid_payment_method
 * 
 * Função que verifica se o método de pagamento é válido
 * 
 * @returns bool
 */
int is_valid_payment_method(char *s) {
	return (strcmp(s, "cash") == 0 || strcmp(s, "debit_card") == 0 || strcmp(s, "credit_card") == 0) ? 1 : 0;
}

/**
 * @brief Função is_valid_name
 *
 * Função que verifica se a string é um nome válido
 * 
 * @returns bool
 */
int is_valid_name(char *s) {
	char *p = s;
	for (; *p; p++) 
		if (p > s && *p == '/' && *(p+1) != '\0')
			return 1;
	return 0;
}

/**
 * @brief Função is_valid_pay_method
 * 
 * Função que verifica se o método de pagamento é válido
 * 
 * @returns bool
 */

int is_valid_pay_method(char *s) {
	return (strcmp(s, "cash") == 0 || strcmp(s, "credit_card") == 0 || strcmp(s, "debit_card") == 0) ? 1 : 0;
}

/**
 * @brief Função is_valid_car_class
 *
 * Função que verifica se a classe do carro é válida
 * 
 * @returns bool
 */
int is_valid_car_class(char *s) {
	return (strcmp(s, "green") == 0 || strcmp(s, "premium") == 0 || strcmp(s, "basic") == 0) ? 1 : 0;
}

/**
 * @brief Função is_valid_license_plate
 *
 * Função que verifica se a matricula de um carro é válida
 * Formato XX-XX-XX
 * 
 * @returns bool
 */

int is_valid_license_plate(char *s) {
	char *p = s;
	if (strlen(p) != 8)
		return 0;
	for (int i = 0; i < 8; i++)
	{
		if (i == 2 || i == 5)
		{
			if (p[i] != '-')
				return 0;
		}
		else
		{
			if ((p[i] < 'A' || p[i] > 'Z') && (p[i] < '0' || p[i] > '9'))
				return 0;
		}
	}
	return 1;
}

/**
 * @brief Função hash
 *
 * Função que através de determinada key calcula um valor para a sua hash, para a futura inserção na hashtable
 * 
 * @returns valor de hash
 */
int hash(char *key) {
    unsigned int value = 0;
    int i = 0;
    int key_len = strlen(key);

    for (; i < key_len; ++i) {
        value = value * 37 + key[i];
    }

    // 0 <= value < TABLE_SIZE
    value = value % TABLE_SIZE;

    return value;
}

/**
 * @brief Função file_count
 *
 * Conta o número de linhas existentes num dado ficheiro
 * 
 * @returns número de linhas num ficheiro
 */
int file_count(char *file) {
	char line[LINE_BUFFER];
    FILE *f = fopen(file, "r");
    fgets(line, LINE_BUFFER, f);
    int r = 0;
    while(fgets(line, LINE_BUFFER, f) != NULL)
    	r++;
    fclose(f);

    return r;
}

/**
 * @brief Função insertionSort
 *
 * Ordena um array utilizando o algoritmo de ordenação insertion_sort
 * 
 */
void insertion_sort(int arr[], int n)
{
    int i, key, j;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;
 
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}