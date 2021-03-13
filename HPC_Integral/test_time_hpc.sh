#!/bin/bash

EXEC_FILE=./main.out

NUM_THREADS_FROM=1
NUM_THREADS_TO=4

NUM_REPEATS=1

TIME=$(which time)

for (( num_threads = $NUM_THREADS_FROM; num_threads <= $NUM_THREADS_TO; num_threads++))
do
    for (( i_repeat = 0; i_repeat < $NUM_REPEATS; i_repeat++))
    do
        $TIME -f "$num_threads: %e s" $EXEC_FILE $num_threads > /dev/null
    done

    sleep 2
done
