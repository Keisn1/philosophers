##
# Philosophers
#
# @file
# @version 0.1

CC := cc
CFLAGS := -Wall -Wextra -Werror
FSANITIZE_A := -fsanitize=address
FSANITIZE_T := -fsanitize=thread

SRC_DIR :=
OBJ_DIR := obj
BUILD_DIR := build

SRC_FILES := $(wildcard *.c)
OBJ_FILES := $(SRC_FILES:%.c=$(OBJ_DIR)/%.o)

NAME := philo_bonus

all:		$(NAME)

$(NAME): $(OBJ_FILES)
	$(CC) $(CFLAGS) $(OBJ_FILES) $(INCLUDES)  -o $(NAME)

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
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
	rm -f $(NAME)

re: fclean all

.PHONY: clean fclean re

#############
# TESTING
#############
unittest:
	cmake -S . -B build -DBUILD_PHILO=OFF && \
	cmake --build build && \
	./build/run_unittests

test-scenarios-build:
	$(CC) $(CFLAGS) $(FSANITIZE) $(SRC_FILES) -o philosophers


compile_commands:
	cmake -S . -B build -DBUILD_TEST=ON -DBUILD_PHILO=ON -DCMAKE_EXPORT_COMPILE_COMMANDS=ON && \
	mv build/compile_commands.json ./compile_commands.json

# end
