/**
 * @file parser.c
 * @brief Ficheiro que contém as funções auxiliares usadas ao longo do projeto
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
//#include "define.h"

#include "../../includes/define.h"

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
 * @brief Função is_equal_ignore_case
 * 
 * Função que compara duas strings ignorando as diferenças entre maiúsculas e minúsculas
 * 
 * @returns int
*/

int is_equal_ignore_case(const char* word, const char* target) {
    if (strlen(word) != strlen(target)) {
        return 0;
    }

    for (int i = 0; i < strlen(word); i++) {
        if (tolower(word[i]) != tolower(target[i])) {
            return 0;
        }
    }

    return 1;
}

/**
 * @brief Função is_valid_date
 *
 * Função que verifica se a string é uma data válida
 * 
 * @returns bool
 */
int is_valid_date(const char* date) {
  int day, month, year;
  
  int items_parsed = sscanf(date, "%d/%d/%d", &day, &month, &year);
  
  if (items_parsed != 3) {
      // The date string is not in the format "dd/mm/yyyy"
      return 0;
  }

  // Check if the month is out of range
  if (month < 1 || month > 12) {
    return 0;
  }

  // Check if the day is out of range for the given month
  if (day < 1 || day > 31) {
    return 0;
  }
  // Check for months with fewer than 31 days
  if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) {
    return 0;
  }
  // Check for February
  if (month == 2) {
    // Check for leap years
    if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) {
      if (day > 29) {
        return 0;
      }
    } else {
      if (day > 28) {
        return 0;
      }
    }
  }

  // If all checks pass, the date is valid
  return 1;
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
 * @brief Função is_valid_double
 * 
 * Função que verifica se a string é um double válido
 * 
 * @returns bool
 */

int is_valid_double(char *s) {
	char *p = s;
	if (p[0] == '\0' || p[0] == '-')
		return 0;
	int count = 0;
	while (*p)
	{
		if (*p == '.') {
			count++;
			if (count > 1)
				return 0;
		}
		else if (*p < '0' || *p > '9')	
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
 * @brief Função is_valid_name
 *
 * Função que verifica se a string é um nome válido
 * 
 * @returns bool
 */

int is_valid_name(char *str) {
    return (str != NULL && str[0] != '\0') ? 1 : 0;
}

/**
 * @brief Função is_valid_car_class
 *
 * Função que verifica se a classe do carro é válida
 * 
 * @returns bool
 */
int is_valid_car_class(char *s) {
	return (is_equal_ignore_case(s, "green") == 1 || is_equal_ignore_case(s, "premium") == 1 || is_equal_ignore_case(s, "basic") == 1) ? 1 : 0;
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
 * @brief Função compare_dates
 * 
 * Função que compara duas datas e retorna -1 se a primeira for menor, 1 se a primeira for maior e 0 se forem iguais
 * 
 * @returns int
*/

int compare_dates(const char* date1, const char* date2) {
  int day1, month1, year1;
  int day2, month2, year2;

  // Parse the day, month, and year from the first date
  sscanf(date1, "%d/%d/%d", &day1, &month1, &year1);

  // Parse the day, month, and year from the second date
  sscanf(date2, "%d/%d/%d", &day2, &month2, &year2);

  // Compare the years
  if (year1 < year2) {
    return -1;
  } else if (year1 > year2) {
    return 1;
  } else {
    // If the years are equal, compare the months
    if (month1 < month2) {
      return -1;
    } else if (month1 > month2) {
      return 1;
    } else {
      // If the months are equal, compare the days
      if (day1 < day2) {
        return -1;
      } else if (day1 > day2) {
        return 1;
      } else {
        // If the days are equal, the dates are equal
        return 0;
      }
    }
  }
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