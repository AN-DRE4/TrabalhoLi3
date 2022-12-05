/**
 * @file queries.h
 * @brief Header quer inicializa as funções do ficheiro queries.c
 */

#ifndef QUERIES_H
#define QUERIES_H

typedef struct paginacao *PAGINACAO;

extern int opt;

PAGINACAO create_paginacao();

void read_queries(char *f, char *drivers_path, char *rides_path, char *users_path);
void read_queries_2(int query, char *query_param[4], PAGINACAO pg, char* dri_path, char* rid_path, char* use_path);


#endif