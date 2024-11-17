# Scenario 1
# 1 Philosopher
# implies only one fork
# implies that she can not eat
# implies that the philosopher dies after time_to_die milliseconds
# implies that the simulations stops after she has died

import subprocess


def test1():
    result = subprocess.run(
        ["philo/philo", "1", "100", "100", "100"], capture_output=True, text=True
    )
    got = result.stdout.split("\n")
    assert "1 has taken a fork" in got[0]
