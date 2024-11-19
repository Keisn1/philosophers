import subprocess


def test_scenario_1():
    nbr_of_philosopher = 1
    time_to_die = 100
    time_to_eat = 1
    time_to_sleep = 1
    nbr_must_eat = 3  # number_of_times_each_philosopher_must_eat
    process = subprocess.Popen(
        [
            "philo/philo",
            str(nbr_of_philosopher),
            str(time_to_die),
            str(time_to_eat),
            str(time_to_sleep),
            str(nbr_must_eat),
        ],
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,  # Captures both stdout and stderr
        text=True,  # Ensure output is in text mode (Python 3.7+)
    )

    line = process.stdout.readline()
    assert "1 is thinking\n" in line

    line = process.stdout.readline()
    assert "100 1 died\n" == line
