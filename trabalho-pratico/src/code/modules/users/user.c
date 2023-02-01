#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
/*#include "../../parser.h"
#include "../../define.h"
#include "user.h"*/

#include "../../../../includes/parser.h"
#include "../../../../includes/define.h"
#include "../../../../includes/user.h"

/** 
 * Definição dos módulos de driver
 * 
 * Contém todos os elementos de um driver, como char*
 * 
 */

struct user {
    char *username;
    char *name;
    char *gender;
    char *birth_date;
    char *account_creation;
    char *pay_method;
    char *account_status;
};

/**
 * @brief Função create_user
 *
 * Função que aloca na memoria o espaco necessario para um user
 * 
 * @returns driver
 */

void *create_user() {
    USER u = malloc(sizeof *u);
    if (u == NULL)
		printf("Erro ao criar user\n");
    return u;
}

/**
 * @brief Função delete_user
 *
 * Função que liberta da memoria o espaco alocado por um user
 * 
 */

void delete_user(void* u) {
    USER uc = (USER)u;
    free(uc->username);
    free(uc->name);
    free(uc->gender);
    free(uc->birth_date);
    free(uc->account_creation);
    free(uc->pay_method);
    free(uc->account_status);
    free(uc);
}

/**
 * @brief Função set_user_username
 *
 * Função que atribui um username a um user
 */

void set_user_username(USER u, char *s) {
    u->username = strdup(s);
}

/**
 * @brief Função set_user_name
 *
 * Função que atribui um nome a um user
 */

void set_user_name(USER u, char *s) {
    u->name = strdup(s);
}

/**
 * @brief Função set_user_gender
 * 
 * Função que atribui um genero a um user
 */

void set_user_gender(USER u, char *s){
    u->gender = strdup(s);
}

/**
 * @brief Função set_user_birth_date
 * 
 * Função que atribui uma data de nascimento a um user
 */

void set_user_birth_date(USER u, char *s){
    u->birth_date = strdup(s);
}

/**
 * @brief Função set_user_account_creation
 * 
 * Função que atribui uma data de criação de conta a um user
 */

void set_user_account_creation(USER u, char *s){
    u->account_creation = strdup(s);
}

/**
 * @brief Função set_user_pay_method
 * 
 * Função que atribui um metodo de pagamento a um user
 */

void set_user_pay_method(USER u, char *s){
    u->pay_method = strdup(s);
}

/**
 * @brief Função set_user_account_status
 * 
 * Função que atribui um estado de conta a um user
 */

void set_user_account_status(USER u, char *s){
    u->account_status = strdup(s);
}

/**
 * @brief Função set_user
 * 
 * Função que atribui todos os elementos de um user
 */

void set_user(USER r, char *s) {
    char *p = NULL, *temp_line = strdup(s), *os;
	os = temp_line;
	int i = 0;
    while ((p = strsep(&temp_line, ";")) != NULL) {
        switch (i)
        {
        case 0:
            set_user_username(r, p);
            break;
        case 1:
            set_user_name(r, p);
            break;
        case 2:
            set_user_gender(r, p);
            break;
        case 3:
            set_user_birth_date(r, p);
            break;
        case 4:
            set_user_account_creation(r, p);
            break;
        case 5:
            set_user_pay_method(r, p);
            break;
        case 6:
            set_user_account_status(r, p);
            break;
        }
        i++;
    }
    free(os);
}

/**
 * @brief Função is_valid_user
 * 
 * Função que verifica se um user é valido
 */

int is_valid_user(USER r) {
    if (!(is_valid_gender(r->gender)))
        return 0;
	if (!(is_valid_date(r->birth_date)) || 
        !(is_valid_date(r->account_creation)))
		return 0;
    if (!(is_valid_account_status(r->account_status)))
        return 0;
	if (!(is_valid_name(r->username)) || 
        !(is_valid_name(r->name)) ||
        !(is_valid_name(r->pay_method)))
		return 0;
	return 1;
}

/**
 * @brief Função get_user_username
 * 
 * Função que retorna o username de um user
 */

char *get_user_username(void *r) {
    USER rc = (USER) r;
	return strdup(rc->username);
}

/**
 * @brief Função get_user_name
 * 
 * Função que retorna o nome de um user
 */

char *get_user_name(void *r) {
    USER rc = (USER) r;
    return strdup(rc->name);
}

/**
 * @brief Função get_user_gender
 * 
 * Função que retorna o genero de um user
 */

char *get_user_gender(void *r) {
    USER rc = (USER) r;
    return strdup(rc->gender);
}

/**
 * @brief Função get_user_birth_date
 * 
 * Função que retorna a data de nascimento de um user
 */

char *get_user_birth_date(void *r) {
    USER rc = (USER) r;
    return strdup(rc->birth_date);
}

/**
 * @brief Função get_user_age
 * 
 * Função que retorna a idade de um user
 * 
 * @returns idade
 */

int get_user_age(void *r) {
    USER rc = (USER) r;
    char *str = get_user_birth_date(rc);
    char *token = strsep(&str, "/");
    char *dia = token;
    token = strsep(&str, "/");
    char *mes = token;
    token = strsep(&str, "/");
    char *ano = token;

    int dia_i = atoi(dia);
    int mes_i = atoi(mes);
    int ano_i = atoi(ano);

    //Comparar a data de nascimento com a data atual e retornar a idade
    
    if(mes_i > LAST_DATE_MES || (mes_i == LAST_DATE_MES && dia_i >= LAST_DATE_DIA))
        return LAST_DATE_ANO - ano_i - 1;
    else
        return LAST_DATE_ANO - ano_i;
}

/**
 * @brief Função get_user_account_creation
 * 
 * Função que retorna a data de criação de conta de um user
 */

char *get_user_account_creation(void *r) {
    USER rc = (USER) r;
    return strdup(rc->account_creation);
}

/**
 * @brief Função get_user_account_age
 * 
 * Função que retorna a idade da conta de um user
 * 
 * @returns idade da conta
 */

int get_user_account_age(void *r) {
    USER rc = (USER) r;
    char *str = get_user_account_creation(rc);
    char *token = strsep(&str, "/");
    char *dia = token;
    token = strsep(&str, "/");
    char *mes = token;
    token = strsep(&str, "/");
    char *ano = token;

    int dia_i = atoi(dia);
    int mes_i = atoi(mes);
    int ano_i = atoi(ano);

    //Comparar a data de criação da conta com a data atual e retornar a idade da conta
    
    if(mes_i > LAST_DATE_MES || (mes_i == LAST_DATE_MES && dia_i >= LAST_DATE_DIA))
        return LAST_DATE_ANO - ano_i - 1;
    else
        return LAST_DATE_ANO - ano_i;
}

/**
 * @brief Função get_user_account_status
 * 
 * Função que retorna o estado de conta de um user
 */

char *get_user_account_status(void *r) {
    USER rc = (USER) r;
    return strdup(rc->account_status);
}

/**
 * @brief Função copy_user
 * 
 * Função que copia um user
*/

void copy_user(void* dst, void* src) {
    USER r = (USER) src;
    USER d = (USER) dst;
    set_user_username(d, r->username);
    set_user_name(d, r->name);
    set_user_gender(d, r->gender);
    set_user_birth_date(d, r->birth_date);
    set_user_account_creation(d, r->account_creation);
    set_user_pay_method(d, r->pay_method);
    set_user_account_status(d, r->account_status);
}