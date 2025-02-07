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

fclean:
	rm -rf build

test-scenarios-build:
	$(CC) $(CFLAGS) $(FSANITIZE) $(SRC_FILES) -o philosophers

test-sync-build:
	$(CC) $(CFLAGS) $(FSANITIZE) tests/sync_test/main.c -o tests/sync_test/sync_test

unittest:
	cmake -S . -B build && \
	cmake --build build && \
	./build/run_unittests

test-scenarios: test-scenarios-build
	pytest tests/scenarios/

sync-test: test-sync-build
	pytest tests/sync_test/

compile_commands:
	cmake -S . -B build -DBUILD_TEST=ON -DBUILD_PHILO=ON -DCMAKE_EXPORT_COMPILE_COMMANDS=ON && \
	mv build/compile_commands.json ./compile_commands.json

# end
