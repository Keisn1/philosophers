import subprocess
import pytest


testdata = [
    (1, 100, 0, 0, 1),
    (1, 200, 0, 0, 1),
]


@pytest.mark.parametrize(
    "nbr_of_philosopher,time_to_die,time_to_eat,time_to_sleep,nbr_must_eat", testdata
)
def test_scenario_1(
    nbr_of_philosopher, time_to_die, time_to_eat, time_to_sleep, nbr_must_eat
):
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
    assert f"{time_to_die} 1 died\n" == line
