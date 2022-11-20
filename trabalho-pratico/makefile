CC = gcc
CFLAGS = -Wall -g -o
MODULES_PATH_COMMITS = src/includes/modules/drivers/
MODULES_PATH_USERS = src/includes/modules/users/
MODULES_PATH_REPOS = src/includes/modules/rides/
MODULES_1 = $(MODULES_PATH_COMMITS)driver.c $(MODULES_PATH_USERS)user.c $(MODULES_PATH_REPOS)ride.c
MODULES_2 = $(MODULES_PATH_COMMITS)drivers.c $(MODULES_PATH_USERS)users.c $(MODULES_PATH_REPOS)rides.c
FILES = $(MODULES_1) $(MODULES_2) src/includes/parser.c src/includes/parsing.c src/includes/hashtable/hashtable.c src/includes/queries/queries.c
TARGET = LI3_2022_23
TARGET_2 = saida

main:
	@echo "Compiling"
	gcc $(FILES) src/main.c $(CFLAGS) $(TARGET)

clean:
	rm -r $(TARGET) $(TARGET_2)
