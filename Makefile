##
# Philosophers
#
# @file
# @version 0.1
#
test-scenarios-build:
	cc -Wall -Wextra -Werror -fsanitize=address philo/ft_putnbr_ull_fd.c philo/ft_putchar_fd.c philo/ft_putstr_fd.c philo/main.c -o philo/philo

test-sync-build:
	cc -Wall -Wextra -Werror -fsanitize=address tests/sync_test/main.c -o tests/sync_test/sync_test

test:
	cmake -S . -B build

test-scenarios: test-scenarios-build
	pytest tests/scenarios/


sync-test: test-sync-build
	pytest tests/sync_test/


# end
