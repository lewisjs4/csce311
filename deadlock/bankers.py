#!/usr/bin/env python3
#
"""
Copyright 2024 CSCE 311


Demonstrates the Safety Algorithm part of the Banker's Algorithm.

Source: Wikipedia, https://en.wikipedia.org/wiki/Banker%27s_algorithm#Example_2
"""

import numpy as np


N_PROCESSES = int(input("Number of processes? "))
N_RESOURCES = int(input("Number of resources? "))


AVAILABLE_RESOURCES = [int(x) for x in input("Claim vector? ").split(" ")]


CURRENTLY_ALLOCATED = np.array([
    [int(x) for x in input(f"Currently allocated for process {i + 1}? ").split(" ")]
    for i in range(N_PROCESSES)
])


MAX_DEMAND = np.array([
    [int(x) for x in input(f"Maximum demand from process {i + 1}? ").split(" ")]
    for i in range(N_PROCESSES)
])


TOTAL_AVAILABLE = AVAILABLE_RESOURCES - np.sum(CURRENTLY_ALLOCATED, axis=0)


RUNNING = np.ones(N_PROCESSES)  # An array with N_PROCESSES 1's to represent
                                # current each process' state: running or
                                # completed


print(f"\n\nAvailable: {TOTAL_AVAILABLE}")
while np.count_nonzero(RUNNING) > 0:
    ALLOCATION_FOUND = False  # assume no allocation is found
    for p in range(N_PROCESSES):
        if RUNNING[p]:  # skip completed procs
            print(f"\tNeed[proc_{p}]: {MAX_DEMAND[p] - CURRENTLY_ALLOCATED[p]}")
            if all(i >= 0 for i in  # can p's max demand be met?
                   TOTAL_AVAILABLE - (MAX_DEMAND[p] - CURRENTLY_ALLOCATED[p])):
                print(f"\tproc_{p}: {MAX_DEMAND[p] - CURRENTLY_ALLOCATED[p]} <= {TOTAL_AVAILABLE}")
                print(f"\t  proc_{p} is completed")
                RUNNING[p] = 0  # no longer running

                TOTAL_AVAILABLE += CURRENTLY_ALLOCATED[p]  # return resources
                print(f"\t  proc_{p} returns {CURRENTLY_ALLOCATED[p]}")

                ALLOCATION_FOUND = True  # an assignment made; continue

    if not ALLOCATION_FOUND:
        # no proc's resource request could be assigned and there are still
        # processes running
        print("Unsafe")
        break  # exit

    print(f"Available: {TOTAL_AVAILABLE}")
