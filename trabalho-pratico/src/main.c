/**
 * @file main.c
 * @brief Ficheiro que contém a função principal do programa
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "./includes/modules/drivers/drivers.h"
#include "./includes/modules/users/users.h"
#include "./includes/modules/rides/rides.h"

#include "./includes/modules/drivers/driver.h"
#include "./includes/modules/users/user.h"
#include "./includes/modules/rides/ride.h"

#include "./includes/hashtable/hashtable.h"

#include "./includes/parser.h"
#include "./includes/parsing.h"
#include "./includes/define.h"
#include "./includes/queries/queries.h"
#include "./includes/interface.h"

// valgrind --leak-check=full --show-leak-kinds=all -s ./teste 


/**
 * @brief Função fileExists
 *
 * Verifica se ficheiro existe
 * @returns 1 se ficheiro existe
 * @returns 0 se ficheiro não existe
 */
int fileExists(char *filepath)
{
	FILE *f = fopen(filepath, "r");
	if (f == NULL)
	{
		printf("Error, %s doesn't exist.\n", filepath);
		return 0;
	}
	fclose(f);
	return 1;
	
}

int display_sub_interface_IO(int *page_num, int last_page_num, PAGINACAO pg){
    puts("\n");
    print_results(*page_num, pg);
    printf("-------- Página %d de %d --------\n", *page_num, last_page_num);
    printf("P     -> Próxima\n");
    printf("A     -> Anterior\n");
    printf("S <N> -> Saltar para página\n");
    printf("Q     -> Sair\n");
    printf("\nInsira opção:\n");

    char str[10];
    fgets(str, 10, stdin);
    
    if((str[0]=='P' || str[0]=='p') && *page_num<last_page_num){
        (*page_num)++;
        return 1;
    } 
    else if((str[0]=='A' || str[0]=='a') && *page_num>1){
        (*page_num)--;
        return 1;
    }
    else if((str[0]=='S' || str[0]=='s') && atoi(&str[2])<=last_page_num){
        *page_num = atoi(&str[2]);
        return 1;
    } 
    else if((str[0]=='Q' || str[0]=='q')) return 0;

    return 1;
} 

void sub_interface_IO(PAGINACAO pg){
    int page_num = 1;
    int last_page_num = get_pg_size(pg)/5;
    while(display_sub_interface_IO(&page_num, last_page_num, pg));
    system("clear||cls");
}

/**
 * @brief Função interface_IO
 *
 * Função para caso não sejam dados quaisquer argumentos na execução do executável
 * 
 */
void interface_IO(char *drivers_path, char *rides_path, char *users_path){

    PAGINACAO pg = create_paginacao();
    system("clear||cls");
    int ans_data;
    char * n_data = malloc(50*sizeof(char)); char * data1 = malloc(50*sizeof(char)); char * data2 = malloc(50*sizeof(char)); char * lang = malloc(50*sizeof(char));
    char *query_param[4];

    puts("\n|---------------------------------------------------------------------------------------|\n");
    puts("| 1 |                                Resumo de um perfil                                |\n");
    puts("|---------------------------------------------------------------------------------------|\n");
    puts("| 2 |                     Top N condutores com maior avaliacao media                    |\n");
    puts("|---------------------------------------------------------------------------------------|\n");
    puts("| 3 |                   Top N utilizadores com maior distancia viajada                  |\n");
    puts("|---------------------------------------------------------------------------------------|\n");
    puts("| 4 |                  Preco medio das viagens numa determinada cidade                  |\n");
    puts("|---------------------------------------------------------------------------------------|\n");
    puts("| 5 |                     Preco medio das viagens num dado intervalo                    |\n");
    puts("|---------------------------------------------------------------------------------------|\n");
    puts("| 6 |  Distancia media percorrida, numa determinada cidade num dado intervalo de tempo  |\n");
    puts("|---------------------------------------------------------------------------------------|\n");
    puts("| 7 |                     Top N condutores numa determinada cidade,                     |\n");
    puts("|---------------------------------------------------------------------------------------|\n");
    puts("| 8 | Viagens com o condutor e utilizador do mesmo genero e acima ou igual a uma idade  |\n");
    puts("|---------------------------------------------------------------------------------------|\n");
    puts("| 9 |        Todas as viagens onde houve uma gorjeta num certo intervalo de tempo       |\n");
    puts("|---------------------------------------------------------------------------------------|\n");

    printf("\nInsira opção: ");
    scanf("%d",&ans_data); 

    switch (ans_data){

        case 1 :
			puts("Insira o nome do utilizador:\n");
			scanf("%s",n_data);
			putchar('\n');
			query_param[1] = n_data;
			read_queries_2(1,query_param,pg, drivers_path, rides_path, users_path);
			sub_interface_IO(pg);
            break;

        case 2 :
			//Stuff for query 2
            break;

        case 3 :
			//Stuff for query 3
            break;

        case 4 :
			//Stuff for query 4
            break;

        case 5 :
			//Stuff for query 5
            break;

        case 6 :
            puts("Insira a cidade:\n");
			scanf("%s",n_data);
            puts("Insira a data inferior:\n");
            scanf("%s",data1);
            puts("Insira a data superior:\n");
            scanf("%s",data2);
			putchar('\n');
			query_param[1] = n_data;
            query_param[2] = data1;
            query_param[3] = data2;
			read_queries_2(6,query_param,pg, drivers_path, rides_path, users_path);
            //printf("Tamanho da pagina: %d\n", get_pg_size(pg));
			sub_interface_IO(pg);            
            break;

        case 7 :
			//Stuff for query 7
            break;

        case 8 :
            puts("Insira o género:\n");
			scanf("%s",n_data);
            puts("Insira a idade:\n");
            scanf("%s",data1);
			putchar('\n');
			query_param[1] = n_data;
            query_param[2] = data1;
			read_queries_2(8,query_param,pg, drivers_path, rides_path, users_path);
            //printf("Tamanho da pagina: %d\n", get_pg_size(pg));
			sub_interface_IO(pg);            
            break;
        case 9 :
			//Stuff for query 9
            break;
    }

    // do smth

}

/**
 * @brief Função main
 *
 * Ponto de partida da execução do nosso programa.
 * @returns 0
 */
int main (int argc, char * argv[]) {

	double time_spent = 0.0;
	clock_t begin = clock();

	char folder[256];

	if(argc == 1) {
		//ask the user for the folder
		printf("Please enter the folder path: ");
		char tmp[256];
		scanf("%s", tmp);
		sprintf(folder, "%s", tmp);
	} else {
		// Keep the folder where the csv files are located
		sprintf(folder, "%s", argv[1]);
	}

	// Keep the path to the drivers.csv file
	char drivers_filepath[512];
	sprintf(drivers_filepath, "%s/%s", folder, DRIVERS_FILEPATH2);

	// Keep the path to the rides.csv file
	char rides_filepath[512];
	sprintf(rides_filepath, "%s/%s", folder, RIDES_FILEPATH2);

	// Keep the path to the users.csv file
	char users_filepath[512];
	sprintf(users_filepath, "%s/%s", folder, USERS_FILEPATH2);

	if (!fileExists(drivers_filepath) || !fileExists(rides_filepath) || !fileExists(users_filepath))
		return 0;

	if(argc == 1) {
		opt = 0;
		interface_IO(drivers_filepath, rides_filepath, users_filepath);
	} 
	else if(argc == 3){
		if(!fileExists(argv[2])) return 0;
		if(!fileExists(*argv)) return 0;
		else {
			opt = 1;
			read_queries(argv[2], drivers_filepath, rides_filepath, users_filepath);
		}
	}
	
	clock_t end = clock();
	time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
	printf("The elapsed time is %f seconds\n", time_spent);

	return 0;
}
