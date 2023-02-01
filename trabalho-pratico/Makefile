CC = gcc
CFLAGS = -Wall -g -o
MODULES_PATH_COMMITS = src/code/modules/drivers/
MODULES_PATH_USERS = src/code/modules/users/
MODULES_PATH_REPOS = src/code/modules/rides/
MODULES_1 = $(MODULES_PATH_COMMITS)driver.c $(MODULES_PATH_USERS)user.c $(MODULES_PATH_REPOS)ride.c
MODULES_2 = $(MODULES_PATH_COMMITS)drivers.c $(MODULES_PATH_USERS)users.c $(MODULES_PATH_REPOS)rides.c
FILES = $(MODULES_1) $(MODULES_2) src/code/parser.c src/code/parsing.c src/code/hashtable/hashtable.c src/code/queries/queries.c src/code/interface.c
TARGET = programa-principal
TARGET_2 = Resultados

main:
	@echo "Compiling"
	gcc $(FILES) src/main.c $(CFLAGS) $(TARGET)

clean:
	rm -r $(TARGET) $(TARGET_2)
