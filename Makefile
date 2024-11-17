##
# Philosophers
#
# @file
# @version 0.1

test-build:
	cc -Wall -Wextra -Werror -fsanitize=address philo/main.c -o philo/philo

test:
	cmake -S . -B build

integration-test: test-build
	pytest tests/integration_tests/

# end
