##
# Philosophers
#
# @file
# @version 0.1

CC := cc
CFLAGS := -Wall -Wextra -Werror
FSANITIZE := -fsanitize=address

SRC_FILES := $(wildcard philo/*.c)

NAME := philosophers

test-scenarios-build:
	$(CC) $(CFLAGS) $(FSANITIZE) $(SRC_FILES) -o philosophers

test-sync-build:
	$(CC) $(CFLAGS) $(FSANITIZE) tests/sync_test/main.c -o tests/sync_test/sync_test

test:
	cmake -S . -B build

test-scenarios: test-scenarios-build
	pytest tests/scenarios/

sync-test: test-sync-build
	pytest tests/sync_test/


# end
