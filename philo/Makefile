##
# Philosophers
#
# @file
# @version 0.1

CC := cc
CFLAGS := -Wall -Wextra -Werror
FSANITIZE := -fsanitize=address

SRC_DIR :=
OBJ_DIR := obj
BUILD_DIR := build

SRC_FILES := $(wildcard *.c)
OBJ_FILES := $(SRC_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

NAME := philo

all:		$(NAME)

$(NAME): $(OBJ_FILES)
	$(CC) $(CFLAGS) $(OBJ_FILES) $(INCLUDES) $(FSANITIZE) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

#############
# Requisites
#############

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

#############
# CLEANING
#############

clean:
	@rm -f $(OBJ_FILES)

fclean:
	rm -rf $(OBJ_DIR)
	rm -rf $(BUILD_DIR)
	rm $(NAME)

re: fclean all

#############
# TESTING
#############
unittest:
	cmake -S . -B build -DBUILD_PHILO=OFF && \
	cmake --build build && \
	./build/run_unittests

test-scenarios-build:
	$(CC) $(CFLAGS) $(FSANITIZE) $(SRC_FILES) -o philosophers

test-sync-build:
	$(CC) $(CFLAGS) $(FSANITIZE) tests/sync_test/main.c -o tests/sync_test/sync_test

test-scenarios: test-scenarios-build
	pytest tests/scenarios/

sync-test: test-sync-build
	pytest tests/sync_test/

compile_commands:
	cmake -S . -B build -DBUILD_TEST=ON -DBUILD_PHILO=ON -DCMAKE_EXPORT_COMPILE_COMMANDS=ON && \
	mv build/compile_commands.json ./compile_commands.json

# end
