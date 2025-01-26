#!/bin/bash

if [ "$1" == "disable" ]; then
    echo "Disabling Turbo Boost and setting performance governor..."
    echo 1 | sudo tee /sys/devices/system/cpu/intel_pstate/no_turbo
    sudo cpufreq-set -g performance
elif [ "$1" == "enable" ]; then
    echo "Enabling Turbo Boost and setting ondemand governor..."
    echo 0 | sudo tee /sys/devices/system/cpu/intel_pstate/no_turbo
    sudo cpufreq-set -g ondemand
elif [ "$1" == "run" ]; then
    # Check if the executable path is provided
    if [ $# -lt 2 ]; then
        echo "Usage: $0 run <executable_path>"
        exit 1
    fi

    EXECUTABLE=$2

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
else
    echo "Usage: $0 [disable|enable|run <executable_path>]"
fi
