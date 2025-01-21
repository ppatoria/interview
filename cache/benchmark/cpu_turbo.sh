#!/bin/bash

if [ "$1" == "disable" ]; then
    echo "Disabling Turbo Boost and setting performance governor..."
    echo 1 | sudo tee /sys/devices/system/cpu/intel_pstate/no_turbo
    sudo cpufreq-set -g performance
elif [ "$1" == "enable" ]; then
    echo "Enabling Turbo Boost and setting ondemand governor..."
    echo 0 | sudo tee /sys/devices/system/cpu/intel_pstate/no_turbo
    sudo cpufreq-set -g ondemand
else
    echo "Usage: $0 [disable|enable]"
fi
