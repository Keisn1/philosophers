# Scenario 1
# 1 Philosopher
# implies only one fork
# implies that she can not eat
# implies that the philosopher dies after time_to_die milliseconds
# implies that the simulations stops after she has died

import subprocess
import time


def test_1philosopher():

    process = subprocess.Popen(
        [
            "philo/philo",
            "1",
            "100",
            "100",
            "100",
        ],
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,  # Captures both stdout and stderr
        text=True,  # Ensure output is in text mode (Python 3.7+)
    )

    start = int(time.time_ns()) / 10 ** (6) + 5

    line = process.stdout.readline()
    now = int(time.time_ns()) / 10 ** (6)
    print(f"timestamp in p: {now - start}")
    print(line)

    now = int(time.time_ns()) / 10 ** (6)
    for line in iter(process.stdout.readline, ""):
        now = int(time.time_ns()) / 10 ** (6)
        print(f"timestamp in p: {round(now - start)}")
        print(line)

    process.stdout.close()
    process.wait()
