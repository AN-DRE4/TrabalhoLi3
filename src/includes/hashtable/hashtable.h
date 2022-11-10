/**
 * @file hashtable.h
 * @brief Header que inicializa as funções do ficheiro hashtable.c
 */

#ifndef HASHTABLE_H
#define HASHTABLE_H

typedef struct ht_entry ht_entry;
typedef struct ht ht;

ht* ht_create(int size);

void ht_del(ht *ht, char *key);
void ht_insert(ht *ht, char *key, void *d);
void ht_insert_2(ht *ht, char *key, void* d);
void ht_insert_3(ht *ht, char *key, void *d);
void ht_incrementation_insert(ht *ht, char* key);


void* ht_get(ht *ht, char *key, void*(*create_type)(void),void(*copy_data)(void*, void*));
void* ht_get_no_mem_cpy(ht *ht, char *key);
char* ht_get_s(ht *ht, int *slot, int *node, void** d);

int ht_count(ht *ht);

void ht_destroy(ht *ht, void(*delete_data)(void *));
void ht_destroy_no_mem_cpy(ht *ht);

void ht_print_table(ht *ht);
#endif
