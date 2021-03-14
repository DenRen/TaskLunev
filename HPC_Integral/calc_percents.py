#!/usr/bin/python3.6

name_file = 'result.txt'

with open (name_file, "r") as file:

    time_first = 0

    for line in file.readlines ():
        line = line.replace (":", "").replace ("s", "")
        
        tokens = line.split (" ")
        
        num_threads = tokens[0]
        if (num_threads.isdigit () == False):
            print (f"In {name_file} incorrect format. Stop parsing.")
            exit ()

        num_threads = int (num_threads)
        num_seconds = float (tokens[1])

        print (f"{num_threads}: {num_seconds} s", end='')
        if (num_threads == 1):
            time_first = num_seconds
            print ("")
        else:
            if (time_first == 0):
                print ("Where first thread? Stop parsing.")
                exit ()
            
            part = abs (time_first / num_seconds - num_threads)
            print (f" error: {round (part * 100, 2)}%")
