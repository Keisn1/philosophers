# Expecting the logs to be printed to stdout

import subprocess
import time


def test_synchronization():
    process = subprocess.Popen(
        "tests/sync_test/sync_test",
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,  # Captures both stdout and stderr
        text=True,  # Ensure output is in text mode (Python 3.7+)
    )

    line = process.stdout.readline()
    start_time = int(line[len("timestamp in c: ") :])
    start = int(time.time_ns()) / 10 ** (6)

    for i, line in enumerate(iter(process.stdout.readline, "")):
        passed = int(time.time_ns()) / 10 ** (6) - start
        diff = abs(start_time + round(passed) - int(line[len("timestamp in c: ") :]))
        assert diff <= 5, f"Failure: {diff} is greater than 1; {i}th iteration"

    process.stdout.close()
    process.wait()
