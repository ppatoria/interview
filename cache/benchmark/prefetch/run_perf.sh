#!/bin/bash

# Check if the executable path is provided
if [ $# -eq 0 ]; then
    echo "Usage: $0 <executable_path>"
    exit 1
fi

EXECUTABLE=$1

# Check if the file exists and is executable
if [ ! -x "$EXECUTABLE" ]; then
    echo "Error: $EXECUTABLE does not exist or is not executable."
    exit 2
fi

# Apply the temporary fix for perf_event_paranoid
echo "Setting perf_event_paranoid to 1 (temporary fix)..."
echo 1 | sudo tee /proc/sys/kernel/perf_event_paranoid >/dev/null

# Run perf with the executable
echo "Running perf for $EXECUTABLE..."
perf stat -e cache-references,cache-misses,cycles,instructions "$EXECUTABLE"

# Restore default value (optional)
# Uncomment the following lines if you want to revert the setting
# echo "Restoring perf_event_paranoid to its default value (4)..."
# echo 4 | sudo tee /proc/sys/kernel/perf_event_paranoid >/dev/null
