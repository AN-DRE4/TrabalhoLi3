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

#include "../define.h"
#include "../hashtable/hashtable.h"
//#include "../linkedlist.h"
#include "../parser.h"
#include "../parsing.h"

#include "../modules/drivers/drivers.h"
#include "../modules/users/users.h"
#include "../modules/rides/rides.h"

#include "../modules/drivers/driver.h"
#include "../modules/users/user.h"
#include "../modules/rides/ride.h"

int output_n = 1;

/**
 * @brief Função get_output_dir_file
 *
 * Função que cria uma dir de output
 * 
 * @returns dir de output
 */
static void get_output_dir_file(char * f){

    //mkdir(f,0777);
	mkdir(f);
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

	sprintf(output_file, "%s%d%s", "saida/command", output_n, "_output.txt");

	output_n++;
	// printf("%s\n", output_file);
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
	//printf("Cheguei aqui 1\n");
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

static void query_1(char *id, USERS us, DRIVERS ds, RIDES rs, ht *ht_user_ride, ht *ht_driver_ride)
{
	FILE *f = get_output_file();
	int temp = atoi(id);
	if(temp != 0){
		DRIVER d = get_driver(ds, id);
		char *status = get_driver_account_status(d);
		if(strcmp(status, "inactive") == 0 || d == NULL) {
			//Se a conta estiver inativa ou o utilizador nao existir, o ficheiro de output é fechado e a função termina
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
			fprintf(f, "%s;%s;%d;%.3f;%d;%.3f\n", name, gender, age, score, num_rides, total_aferido); 
			fclose(f);
			return;
		}

		
	} else {
		USER u = get_user(us, id);
		char *status = get_user_account_status(u);
		if(strcmp(status, "inactive") == 0 || u == NULL) {
			//Se a conta estiver inativa ou o utilizador nao existir, o ficheiro de output é fechado e a função termina
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
			//Imprimir no ficheiro de output
			fprintf(f, "%s;%s;%d;%.3f;%d;%.3f\n", name, gender, age, score, num_rides, total_gasto); 
			fclose(f);
			return;
		}
	}
}

/**
 * @brief Função query2
 *
 * Função que executa a query 2 que recebe um N e devolve
 * os N condutores com maior avaliação média, em caso de 
 * empate, o resultado deverá ser ordenado de forma a que os  
 * condutores com a viagem mais recente surjam primeiro.
 * Caso haja novo empate, é usado o id do condutor para desempatar (ordem crescente).
 * Devolve os dados no seguinte formato:
 * id;nome;avaliação_media
 *
 */

static void query_2(int n,DRIVERS ds,USERS us,RIDES rs,ht *ht_driver_ride,ht *ht_user_ride)
{    int k=n;
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
            int id_final[n-1];
            char *nome_final[n-1];
            array_av[0] = 0;
            for(int id=1;id<=10000;id++){
                //if(condicção para ser diferente do que ja temos)
                DRIVER d = get_driver(ds,atoi(id));
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
        fprintf(f,"%d;%s;%.3f\n",id_final[i],nome_final[i],array_av[i]);
        i++;n--;
        }
    }
}

/**
 * @brief Função query3
 *
 * Função que executa a query 3 que recebe um N e retorna os N
 * utilizadores com maior distância viajada. Em caso de empate,
 * o resultado é ordenado de forma que os utilizadores com a 
 * viagem mais recente surjam primeiro. Caso haja novo empate,
 * deverá ser usado o username do utilizador para desempatar(por ordem crescente)
 * Devolve os dados no seguinte formato:
 * username;nome;distancia_total
 *
 */

static void query_3(int n,USERS us,RIDES rs){
    FILE *f = get_output_file();
    if(n<=0){
        fclose(f);
        return;
    }
    else{
        while(n>0){
            int elem=0;
            char *user_mais_distância[n];
            char *user_mais_distância[n];
            char *user_mais_distância[elem] = get_ride_user(get_ride(rs,atoi(1)));
            char *name_mais_distância[elem] = get_user_name(get_user(us,atoi(1)));
            int distancia[elem] = get_ride_distance(get_ride(rs,atoi(1)));
            int id=2;
            while(id<=1000000){
                RIDE r = get_ride(rs,atoi(id));
                USER u = get_user(us,atoi(id));
                if(get_ride_distance(r)>user_mais_distância[elem]){
                    user_mais_distância[elem]=get_ride_distance;
                }
                else if(get_ride_distance(r)==user_mais_distância[elem]){
                    //situação em que data da viagem mais recente ganha ----> criar get_ride_date
                    if(atoi(get_ride_user(rs))>atoi(get_ride_user(rs++))) user_mais_distância[elem]=get_ride_user(rs);
                    else user_mais_distância[elem]=get_ride_user(rs++);
                }
            id++;
            }
        fprintf(f,"%s;%s;%d",user_mais_distância[elem],name,distance);
        fclose(f);
        n--;
        elem++;
        }
    }
    return;
}


/**
 * @brief Função read_queries
 *
 * Função que lê as queries solicitadas no ficheiro de input
 *
 */
void read_queries(char *f)
{

	ht *ht_repo_colabs = ht_create(TABLE_SIZE);
	ht *ht_user_ride = ht_create(TABLE_SIZE);
	ht *ht_driver_ride = ht_create(TABLE_SIZE);

	USERS us = create_users_catalog();
	DRIVERS ds = create_drivers_catalog();
	RIDES rs = create_rides_catalog(us, ds, ht_user_ride, ht_driver_ride);

	char line[1024];
	FILE *input_commands = fopen(f, "r");
	get_output_dir_file("saida");
	printf("A tabela user_ride tem %d entradas.\n", ht_count(ht_user_ride));	
	/*if((ht_get_no_mem_cpy(ht_user_ride, "SaCruz110")) != NULL) printf("Pila1\n");
	if((ht_get_no_mem_cpy(ht_user_ride, "ClarPacheco48")) != NULL) printf("Pila2\n");
	if((ht_get_no_mem_cpy(ht_user_ride, "GabriJesus")) != NULL) printf("Pila3\n");*/
	/*if((ht_get_no_mem_cpy(ht_driver_ride, "000000004780")) != NULL) printf("Pila4\n");
	if((ht_get_no_mem_cpy(ht_driver_ride, "000000007141")) != NULL) printf("Pila5\n");
	if((ht_get_no_mem_cpy(ht_driver_ride, "000000003123")) != NULL) printf("Pila6\n");*/
	while (fgets(line, 1024, input_commands) != NULL)
	{
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
			printf("Query 1\n");
			//printf("Numero de rides do user %s na user_ride: %d\n", "SaCruz110",ht_count_keys(ht_user_ride, "SaCruz110"));
			//printf("%s\n", get_ride_id(ht_get_no_mem_cpy(ht_user_ride, "SaCruz110")));
			//printf("%s\n", get_ride_id(ht_get_next(ht_user_ride, "SaCruz110", 0)));
			ht_count_keys(ht_user_ride, "SaCruz110");
			query_1(query_param[1], us, ds, rs, ht_user_ride, ht_driver_ride);
			break;
		/*case 2:
			//TODO
			query_2(ht_repo_colabs, n_repos);
			break;
		case 3:
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
			break;
		case 6:
		//TODO
			query_6(atoi(query_param[1]), query_param[2], us, ds, rs);
			break;
		case 7:
		//TODO
			query_7(query_param[1], ds, rs);
			break;
		case 8:
		//TODO
			query_8(atoi(query_param[1]), query_param[2], ds, rs);
			break;
		case 9:
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

	ht_destroy_no_mem_cpy(ht_repo_colabs);
}
