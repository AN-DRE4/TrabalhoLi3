/**
 * @file queries.c
 * @brief Ficheiro que contém as funções para a execução das queries
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include "../define.h"
#include "../hashtable/hashtable.h"
#include "../parser.h"
#include "../parsing.h"

#include "../modules/drivers/drivers.h"
#include "../modules/users/users.h"
#include "../modules/rides/rides.h"

#include "../modules/drivers/driver.h"
#include "../modules/users/user.h"
#include "../modules/rides/ride.h"

#include "../interface.h"
#include "queries.h"

int output_n = 1;
int opt;

/**
 * @brief Função get_output_dir_file
 *
 * Função que cria uma dir de output
 * 
 * @returns dir de output
 */
static void get_output_dir_file(char * f){

    mkdir(f,0777);
    //mkdir(f);

}

/**
 * @brief Função get_output_file
 *
 * Função que cria um ficheiro de output
 *
 * @returns ficheiro de output
 */
static FILE *get_output_file()
{

	char output_file[50];

	sprintf(output_file, "%s%d%s", "Resultados/command", output_n, "_output.txt");

	output_n++;
	// printf("%s\n", output_file);
	FILE *f = fopen(output_file, "w");
	if (!f)
		perror("fopen");
	return f;
}

/**
 * @brief Função get_output_file_tests
 *
 * Função que cria um ficheiro de output para os testes
 * 
 * @returns string com o nome ficheiro de output
 */
static FILE* get_output_file_tests() {

  	char output_file[50];
    
  	sprintf(output_file, "%s", "Resultados/tests_output.txt");

  	FILE *f = fopen(output_file, "w");
    if (!f)
    	perror("fopen");
    return f;
}

/**
 * @brief Função get_ride_total
 * 
 * Função que retorna o custo total de uma ride
 * 
 * @param r ride
 * 
 * @returns custo total da ride
 */

double get_ride_total(void *r, DRIVERS ds) {
    RIDE rc = (RIDE) r;
	double total = 0;
	char *driver = get_ride_driver(rc);
	DRIVER d = get_driver(ds, driver);
    if (strcmp(get_driver_car_class(d), "green") == 0) {
		total += 4.00;
		total += 0.79 * atof(get_ride_distance(rc));
		return total += atof(get_ride_tip(rc));
	}
	else if (strcmp(get_driver_car_class(d), "basic") == 0) {
		total += 3.25;
		total += 0.62 * atof(get_ride_distance(rc));
		return total += atof(get_ride_tip(rc));
	}
	else {
		total += 5.20;
		total += 0.94 * atof(get_ride_distance(rc));
		return total += atof(get_ride_tip(rc));
	}
}

/**
 * @brief Função get_avaliacao_media_user
 * 
 * Função que calcula a avaliação média de um user
 * 
 * @param u user
 * 
 * @returns avaliação média do user
 */


static double get_avaliacao_media_user(USER u, ht *ht_user_ride, RIDES rs, DRIVERS ds, int *n, double *n2) {
	void *ride = ht_get_no_mem_cpy(ht_user_ride, get_user_username(u));
	int score_total = 0;
	int count = 0;

	
	if(ride != NULL){ 
		while(ride != NULL) {
			char *ride_score_user = get_ride_score_user(ride);
			score_total += atoi(ride_score_user);
			(*n)++;
			(*n2) += get_ride_total(ride, ds);
			free(ride_score_user);
			if (ht_get_next(ht_user_ride, get_user_username(u), count) != NULL) 
				ride = ht_get_next(ht_user_ride, get_user_username(u), count);
			else break;
			count++;
		}
	}
	if (score_total == 0) return 0;
	else return (double)score_total/(*n);
}

/**
 * @brief Função get_avaliacao_media_driver
 * 
 * Função que calcula a avaliação média de um driver
 * 
 * @param d driver
 * 
 * @returns avaliação média do driver
 */

static double get_avaliacao_media_driver(DRIVER d, ht *ht_driver_ride, RIDES rs, DRIVERS ds, int *n, double *n2) {
	void *ride = ht_get_no_mem_cpy(ht_driver_ride, get_driver_id(d));
	int score_total = 0;
	int count = 0;

	if(ride != NULL){ 
		while(ride != NULL) {
			char *ride_score_driver = get_ride_score_driver(ride);
			score_total += atoi(ride_score_driver);
			(*n)++;
			(*n2) += get_ride_total(ride, ds);
			free(ride_score_driver);
			if (ht_get_next(ht_driver_ride, get_driver_id(d), count) != NULL) 
				ride = ht_get_next(ht_driver_ride, get_driver_id(d), count);
			else break;
			count++;
		}
	}
	if (score_total == 0) return 0;
	else return (double)score_total/(*n);
}

/**
 * @brief Função query1
 *
 * Função que executa a query 1 que recebe um id de um user, um catalogo 
 * de users e um catalogo de drivers onde consoante o seu tipo, 
 * ou seja se for um numero é um driver e se for um nome é um user, 
 * devolve os seguintes dados neste formato:
 * Caso seja user: nome;genero;idade;avaliacao_media;numero_viagens;total_gasto
 * Caso seja driver: nome;genero;idade;avaliacao_media;numero_viagens;total_auferido
 *
 */

static void query_1(char *id, USERS us, DRIVERS ds, RIDES rs, ht *ht_user_ride, ht *ht_driver_ride, PAGINACAO pg) {
	
	/*FILE *f;
	if (opt) 
		f = get_output_file();*/

	int temp = atoi(id);
	if(temp != 0){
		DRIVER d = get_driver(ds, id);
		char *status = get_driver_account_status(d);
		if(is_equal_ignore_case(status, "inactive") == 1 || d == NULL) {
			//Se a conta estiver inativa ou o utilizador nao existir, o ficheiro de output é fechado e a função termina
			//if (opt) 
			FILE *f = get_output_file();
			fclose(f);
			return;
		}
		else {
			int num = 0;
			double total = 0;
			char *name = get_driver_name(d);
			char *gender = get_driver_gender(d);
			int age = get_driver_age(d);
			double score = get_avaliacao_media_driver(d, ht_driver_ride, rs, ds, &num, &total);
			int num_rides = num;
			double total_aferido = total;
			//Imprimir no ficheiro de output
			if (opt) {
				FILE *f = get_output_file();
				fprintf(f, "%s;%s;%d;%.3f;%d;%.3f\n", name, gender, age, score, num_rides, total_aferido); 
				fclose(f);

				free(name);
				free(gender);
				delete_driver(d);
			} else {
				char line[256];

				sprintf(line, "%s|%s|%d|%.3f|%d|%.3f", name, gender, age, score, num_rides, total_aferido);
				push_pagina(pg, line);

				free(name);
				free(gender);
				delete_driver(d);
			}
			return;
		}

		
	} else {
		USER u = get_user(us, id);
		char *status = get_user_account_status(u);
		if(is_equal_ignore_case(status, "inactive") == 1 || u == NULL) {
			//Se a conta estiver inativa ou o utilizador nao existir, o ficheiro de output é fechado e a função termina
			//if (opt) 
			FILE *f = get_output_file();
			fclose(f);
			return;
		} else {
			int num = 0;
			double total = 0;
			char *name = get_user_name(u);
			char *gender = get_user_gender(u);
			int age = get_user_age(u);
			double score = get_avaliacao_media_user(u, ht_user_ride, rs, ds, &num, &total);
			int num_rides = num;
			double total_gasto = total;
			if (opt) {
				//Imprimir no ficheiro de output
				FILE *f = get_output_file();
				fprintf(f, "%s;%s;%d;%.3f;%d;%.3f\n", name, gender, age, score, num_rides, total_gasto); 
				fclose(f);
			} else {
				char line[256];
				sprintf(line, "%s|%s|%d|%.3f|%d|%.3f", name, gender, age, score, num_rides, total_gasto);
				push_pagina(pg, line);
				return;
			}
			return;
		}
	}
}

static void query_2(char *inp,DRIVERS ds,USERS us,RIDES rs,ht *ht_driver_ride,ht *ht_user_ride)
{
	int n = atoi(inp);
    FILE *f = get_output_file();
    if(n<=0){
        fclose(f);
        return;
    }
    else{
        while(n>0){
            int i=0;
            double score_final = 0;
            double array_av[n-1];
            char *id_final[n-1];
            char *nome_final[n-1];
            array_av[0] = 0;
            for(int id=1;id<=10000;id++){
                char string[10000];
                //if(condicção para ser diferente do que ja temos)
                sprintf(string,"%d",id);
                DRIVER d = get_driver(ds,string);  //atoi(id)
                int num=0;
                double total=0;
                double score = get_avaliacao_media_driver(d,ht_driver_ride,rs,ds,&num,&total);
                for(int j=0;j<=i;j++){
                    if((score>score_final)&&(array_av[j]!=score)) {
                        score_final = score;
                        id_final[i]=get_driver_id(d);
                        nome_final[i]=get_driver_name(d);
                    }
                }
            }
        array_av[i]=score_final;
        fprintf(f,"%s;%s;%.3f\n",id_final[i],nome_final[i],array_av[i]);
        i++;n--;
        }
    }
}

static void query_6(char* city, char* start_date, char* end_date, RIDES rides, PAGINACAO pg) {
	// Convert the dates to time_t values
	/*struct tm start_tm, end_tm;
	memset(&start_tm, 0, sizeof(struct tm));
	memset(&end_tm, 0, sizeof(struct tm));
	sscanf(start_date, "%d/%d/%d", &start_tm.tm_mday, &start_tm.tm_mon, &start_tm.tm_year);
	sscanf(end_date, "%d/%d/%d", &end_tm.tm_mday, &end_tm.tm_mon, &end_tm.tm_year);
	start_tm.tm_mon--; // tm_mon is 0-indexed
	end_tm.tm_mon--;
	start_tm.tm_year -= 1900; // tm_year is the number of years since 1900
	end_tm.tm_year -= 1900;
	time_t start_time = mktime(&start_tm);
	time_t end_time = mktime(&end_tm);*/

	int i = 0, j = 0;
	void *u = NULL;
	ht *rides_ht = get_rides_table(rides);

	// Create an array to hold the distances of the matching rides
	float* distances = malloc(ht_count(rides_ht) * sizeof(RIDE));
	if (distances == NULL) {
		printf("Error allocating memory!\n");
		return;
	}

	// Iterate through all the rides in the rides hashtable and add the distances of the matching rides to the array
	int num_distances = 0;
	while(ht_get_s(rides_ht, &i, &j, &u)!=NULL) {
		if (u == NULL) continue;

		// Check if the ride is in the specified city and within the specified date range
		if (strcmp(get_ride_city(u), city) == 0 && compare_dates(get_ride_date(u), start_date) >= 0 && compare_dates(get_ride_date(u), end_date) <= 0) {
			// If the ride matches, add its distance to the array
			distances[num_distances++] = atof(get_ride_distance(u));
		}
	}

	// Calculate the average distance
	float average = 0;
	for (int i = 0; i < num_distances; i++) {
		average += distances[i];
	}
	average /= num_distances;

	// Free the array and return the median
	free(distances);

	//print into a folder the median distance
	if (opt) {
		FILE *f = get_output_file();
		fprintf(f, "%.3f", average);
		fclose(f);
	} else {
		char line[256];
		sprintf(line, "%.3f", average);
		push_pagina(pg, line);
	}

	return;
}

int compare_rides1(const void* a, const void* b, USERS users, DRIVERS drivers) {
	//printf("aqui\n");
  RIDE ride_a = *(RIDE*)a;
  RIDE ride_b = *(RIDE*)b;

  // Get the driver and user for each ride
  DRIVER driver_a = get_driver(drivers, get_ride_driver(ride_a));
  USER user_a = get_user(users, get_ride_user(ride_a));
  DRIVER driver_b = get_driver(drivers, get_ride_driver(ride_b));
  USER user_b = get_user(users, get_ride_user(ride_b));

	/*printf("driver_a: %s\n", get_driver_id(driver_a));
	printf("driver_b: %s\n", get_driver_id(driver_b));
	printf("user_a: %s\n", get_user_username(user_a));
	printf("user_b: %s\n", get_user_username(user_b));*/

  // Compare the driver ages
  if (compare_dates(get_driver_account_creation(driver_a), get_driver_account_creation(driver_b)) != 0) {
	//printf("Teste1\n");
    return compare_dates(get_driver_account_creation(driver_a), get_driver_account_creation(driver_b));
  }

  // If the driver ages are the same, compare the user ages
  if (compare_dates(get_user_account_creation(user_a), get_user_account_creation(user_b)) != 0) {
	//printf("Teste2\n");
    return compare_dates(get_user_account_creation(user_a), get_user_account_creation(user_b));
  }

  // If the driver and user ages are the same, compare the ride ids
	//printf("Teste3\n");
  return atoi(get_ride_id(ride_a)) - atoi(get_ride_id(ride_b)); 
}

int compare_rides(void* two, const void* a, const void* b) {
	//save both elems of two
	USERS users = ((void**)two)[0];
	DRIVERS drivers = ((void**)two)[1];
	return compare_rides1(a, b, users, drivers);
}

void ordenaDecrescente(float *score_driver_final, char *driver_id_final,int num_drivers_final)
{
    for (int i = 0; i < num_drivers_final - 1; i++)
    {
        for (int j = i; j < num_drivers_final - 1; j++)
        {
            if (score_driver_final[i] < score_driver_final[j])
            {
                int temp = score_driver_final[i];
				int tomp = driver_id_final[i];
                score_driver_final[i] = score_driver_final[j];
				driver_id_final[i] = driver_id_final[j];
                score_driver_final[j] = temp;
				driver_id_final[j] = tomp;
            }
        }
    }
}

//função auxiliar para a querie_7 que verifica se um valor se encontra em um dado array
int not_in(char id,char *id_final, int N){
	int res=0;

	for(int i=0;i<N;i++){
		if(strcmp(id_final[i],id)==0){
			res=-1;
			break;
		}
	}
	return res;
}

float calcula_media_score_driver(double id,char *driver_id,float *score_driver,int N){
	float media;
	float valores=0;
	int nr_valores=0;

	for(int i=0;i<N;i++){
		if(id==driver_id[i]){
			valores+=score_driver[i];
			nr_valores++;
		}
	}

	media = valores/nr_valores;

	return media;
}

/**
 * @brief Função query_7
 * 
 * Função que imprime no ficheiro de output
 * 
 * 
*/
static void query_7(int n, char* city, DRIVERS drivers, RIDES rides, PAGINACAO pg) {
	int i = 0, j = 0;
	void *u = NULL;
	ht *rides_ht = get_rides_table(rides);

	// RIDE *matching_rides = malloc(ht_count(rides_ht) * sizeof(RIDE));
	// if (matching_rides == NULL) {
	// 	printf("Error allocating memory!\n");
	// 	return;
	// }

	// Create an array to hold the driver of the matching rides
	char* driver_id = malloc(ht_count(rides_ht) * sizeof(RIDE));
	if (driver_id == NULL) {
		printf("Error allocating memory!\n");
		return;
	}

	// Create an array to hold the score_driver of the matching rides
	float* score_driver = malloc(ht_count(rides_ht) * sizeof(RIDE));
	if (score_driver == NULL) {
		printf("Error allocating memory!\n");
		return;
	}


	// Iterate through all the rides in the rides hashtable and add the driver and score_driver of the matching rides to the array
	int num_drivers = 0;
	while(ht_get_s(rides_ht, &i, &j, &u)!=NULL) {
		if (u == NULL) continue;

		// Check if the ride is in the specified city
		if (strcmp(get_ride_city(u), city) == 0){
			// If the ride matches, add its id and score_driver to the array
			driver_id[num_drivers] = atof(get_ride_driver(u));
			score_driver[num_drivers] = atof(get_ride_score_driver(u));
			num_drivers++;
		}
	}

	// Create an final array to hold the driver of the matching rides
	// to not appear more than 1 time the same driver
	char* driver_id_final = malloc(ht_count(rides_ht) * sizeof(RIDE));
	if (driver_id_final == NULL) {
		printf("Error allocating memory!\n");
		return;
	}

	// Create an final array to hold the score_driver of the matching rides
	// to not appear more than 1 time a score of the same driver
	float* score_driver_final = malloc(ht_count(rides_ht) * sizeof(RIDE));
	if (score_driver_final == NULL) {
		printf("Error allocating memory!\n");
		return;
	}

	//percorre a lista dos drivers
	int num_driver_final=0;
	for(int i=0;i<num_drivers;i++){
		if(not_in(driver_id[i],&driver_id_final,num_drivers)==0) {
			score_driver_final[num_driver_final] = calcula_media_score_driver(driver_id[i],driver_id,score_driver,num_drivers);
			driver_id_final[num_driver_final] = driver_id[i];
			num_driver_final++;
		}
	}

	free(driver_id);
	free(score_driver);

	//pegar no n e criar novo array só com os n maiores a partir de 
	//num_driver_final e score_driver_final

	ordenaDecrescente(float *score_driver_final, char *driver_id_final,int num_drivers_final);

	//get the name of n first by id
	char* name = malloc(ht_count(rides_ht) * sizeof(RIDE));
	if (name == NULL) {
		printf("Error allocating memory!\n");
		return;
	}

	for(int i=0;i<n;i++){
		name[i]=get_driver_name(get_driver(drivers,driver_id_final[i]));
	}

	//print into a folder the id;nome;avaliação_media
	FILE *fp;

	char line[256];
	if (opt) {
		fp = get_output_file();
		if (fp == NULL) {
				printf("Error opening file!\n");
				return;
		}
	}
	for (int i = 0; i < num_driver_final; i++) {
		if (opt) {
			
			fprintf(fp, "%s;%s;%.3f\n", driver_id_final[i], name[i], score_driver_final[i]); 
			//fclose(fp);
		} else {
			sprintf(line, "%s;%s;%.3f\n", driver_id_final[i], name[i], score_driver_final[i]);
			push_pagina(pg, line);
			printf("Tamanho da pagina: %d\n", get_pg_size(pg));
		}
	}

	// Close the file
	free(driver_id_final);
	free(score_driver_final);
	if(opt) fclose(fp);

}

/**
 * @brief Função query_8
 * 
 * Função que imprime no ficheiro de output os todas as viagens onde 
 * o utilizador e o condutor são do mesmo género e onde ambos têm uma
 * idade igual ou superior à passada.
*/
static void query_8(char *gender, char* age, USERS users, DRIVERS drivers, RIDES rides, PAGINACAO pg) {
	// Open a file for writing

	int i = 0, j = 0;
	void *u = NULL;
	ht *temp = get_rides_table(rides);

	RIDE *matching_rides = malloc(ht_count(temp) * sizeof(RIDE));
	if (matching_rides == NULL) {
		printf("Error allocating memory!\n");
		return;
	}

  	// Iterate through all the rides in the rides hashtable
  	//for (int i = 0; i < rides->size; i++) {
	int num_matching_rides = 0;
	while(ht_get_s(temp, &i, &j, &u)!=NULL) {
		//struct ride* current_ride = rides->entries[i];
		//if (u == NULL) continue;
		
		// Get the driver and user for the current ride
		DRIVER current_driver = get_driver(drivers, get_ride_driver(u));
		USER current_user = get_user(users, get_ride_user(u));
		
		//printf("BRUH");

		if(is_equal_ignore_case(get_driver_account_status(current_driver), "active") == 1 && is_equal_ignore_case(get_user_account_status(current_user), "active") == 1) {
			// Check if the driver and user have the same gender and are at least the age specified in the parameter
			if (strcmp(get_driver_gender(current_driver), gender) == 0 && strcmp(get_user_gender(current_user), gender) == 0 && get_driver_account_age(current_driver) >= atoi(age) && get_user_account_age(current_user) >= atoi(age)) {
				// If they match, print the ride id to the file
				matching_rides[num_matching_rides++] = u;
				//fprintf(fp, "%s;%s;%s;%s\n", get_ride_driver(u), get_driver_name(current_driver), get_ride_user(u), get_user_name(current_user)); 
			}
		}
	}
	printf("Number of matching rides: %d\n", num_matching_rides);
	printf("Number of rides: %d\n", ht_count(temp));

	//store both users and drivers in an array
	void *two[2];
	two[0] = users;
	two[1] = drivers;

	qsort_s(matching_rides, num_matching_rides, sizeof(RIDE), compare_rides, two);

	FILE *fp;

	char line[256];
	if (opt) {
		fp = get_output_file();
		if (fp == NULL) {
				printf("Error opening file!\n");
				return;
		}
	}
	for (int i = 0; i < num_matching_rides; i++) {
		if (opt) {
			
			fprintf(fp, "%s;%s;%s;%s\n", get_ride_driver(matching_rides[i]), get_driver_name(get_driver(drivers, get_ride_driver(matching_rides[i]))), get_ride_user(matching_rides[i]), get_user_name(get_user(users, get_ride_user(matching_rides[i])))); 
			//fclose(fp);
		} else {
			sprintf(line, "%s|%s|%s|%s\n", get_ride_driver(matching_rides[i]), get_driver_name(get_driver(drivers, get_ride_driver(matching_rides[i]))), get_ride_user(matching_rides[i]), get_user_name(get_user(users, get_ride_user(matching_rides[i]))));
			push_pagina(pg, line);
			printf("Tamanho da pagina: %d\n", get_pg_size(pg));
		}
	}

	// Close the file
	free(matching_rides);
	if(opt) fclose(fp);
}

/**
 * @brief Função read_queries
 *
 * Função que lê as queries solicitadas no ficheiro de input
 *
 */
void read_queries(char *f, char* dri_path, char* rid_path, char* use_path)
{

	ht *ht_user_ride = ht_create(TABLE_SIZE);
	ht *ht_driver_ride = ht_create(TABLE_SIZE);

	USERS us = create_users_catalog(use_path);
	DRIVERS ds = create_drivers_catalog(dri_path);
	RIDES rs = create_rides_catalog(us, ds, ht_user_ride, ht_driver_ride, rid_path);

	char line[1024];
	FILE *input_commands = fopen(f, "r");
	get_output_dir_file("Resultados");
    FILE *test_file = get_output_file_tests();

	double time_spent;
    clock_t begin;
	clock_t end;

	int idk = 1;

	while (fgets(line, 1024, input_commands) != NULL)
	{
		printf("Number: %d\n", idk);
		idk++;
		remove_possible_new_line(line);

		char *temp_line = strdup(line), *os;
		os = temp_line;
		char *p = NULL;
		char *query_param[4];
		int i = 0;
		while ((p = strsep(&temp_line, " ")) != NULL)
		{
			query_param[i] = strdup(p);
			i++;
		}
		free(temp_line);

		switch (atoi(query_param[0]))
		{
		case 1:
			time_spent = 0.0;
			begin = clock();
			query_1(query_param[1], us, ds, rs, ht_user_ride, ht_driver_ride, NULL);
			end = clock();
			time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    		fprintf(test_file,"query 1 done in %f\n", time_spent);
			break;
		case 2:
			//TODO
			query_2(query_param[1],ds,us,rs,ht_driver_ride,ht_user_ride);
			break;
		/*case 3:
			//TODO
			query_3(ht_repo_colabs);
			break;
		case 4:
		//TODO
			query_4(n_commits, bots, orgs, users);
			break;
		case 5:
		//TODO
			query_5(atoi(query_param[1]), query_param[2], query_param[3], ds, us);
			break;*/
		case 6:
		//TODO
			query_6(query_param[1], query_param[2], query_param[3], rs, NULL);
			break;
		case 7:
		//TODO
			query_7(query_param[1],query_param[2], ds, rs, NULL);
			break;
		case 8:
		//TODO
			query_8(query_param[1], query_param[2], us, ds, rs, NULL);
			break;
		/*case 9:
		//TODO
			query_9(atoi(query_param[1]), us, ds, rs);
			break;*/
		}

		free(os);

		for (int j = 0; j < i; j++)
			free(query_param[j]);
	}

	fclose(input_commands);

	delete_users(us);
	delete_drivers(ds);
	delete_rides(rs);

	ht_destroy_no_mem_cpy(ht_user_ride);
	ht_destroy_no_mem_cpy(ht_driver_ride);
}

/**
 * @brief Função read_queries_2
 *
 * Função que lê as queries solicitadas no input de IO()
 * 
 */
void read_queries_2(int query, char *query_param[4], PAGINACAO pg, char* dri_path, char* rid_path, char* use_path) { // deve receber numero da query e o array de argumentos

	ht *ht_user_ride = ht_create(TABLE_SIZE);
	ht *ht_driver_ride = ht_create(TABLE_SIZE);

	USERS us = create_users_catalog(use_path);
	DRIVERS ds = create_drivers_catalog(dri_path);
	RIDES rs = create_rides_catalog(us, ds, ht_user_ride, ht_driver_ride, rid_path);

	get_output_dir_file("Resultados");

	double time_spent;
    clock_t begin;
	clock_t end;

	switch (query) {
		case 1:
			time_spent = 0;
			begin = clock();
			query_1(query_param[1], us, ds, rs, ht_user_ride, ht_driver_ride, pg);
			end = clock();
    		time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
			break;
		case 2:
			//
			break;
		case 3:
			//
			break;
		case 4:
			//
			break;
		case 5:
			//
			break;
		case 6:
			query_6(query_param[1], query_param[2], query_param[3], rs, pg);
			break;

		case 7:
			query_7(query_param[1],query_param[2],ds,rs,pg);
			break;
		case 8:
			query_8(query_param[1], query_param[2], us, ds, rs, pg);
			break;
		case 9:
			//
			break;

	}

	delete_users(us);
	delete_drivers(ds);
	delete_rides(rs);

	ht_destroy_no_mem_cpy(ht_user_ride);
	ht_destroy_no_mem_cpy(ht_driver_ride);

}