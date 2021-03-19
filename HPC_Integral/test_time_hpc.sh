#!/bin/bash

EXEC_FILE=./main.out

NUM_THREADS_FROM=1
NUM_THREADS_TO=16

NUM_REPEATS=4
TIME_SLEEP_REPEAT=0
NAME_FILE="result.txt"

TIME=$(which time)

for (( i_repeat = 0; i_repeat < $NUM_REPEATS; i_repeat++ ))
do
    for (( num_threads = $NUM_THREADS_FROM; num_threads <= $NUM_THREADS_TO; num_threads++ ))
    do
        $TIME -f "$num_threads: %e s" $EXEC_FILE $num_threads > /dev/null
    done

    echo ""
    sleep $TIME_SLEEP_REPEAT
done
