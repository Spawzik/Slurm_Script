#!/usr/bin/env bash

# Arguments: n, datafile, compressor, ... (probably will need more)

# NOTE: probably don't run this until we know the python output is good, as it
# does not check whether command.txt contains a command which is okay to run!

# Check if the correct number of arguments is provided, display usage otherwise
if [ "$#" -ne 3 ]; then
    echo "Usage: $0 <n> <datafile> <compressor>"
    echo "    n: number of jobs to schedule"
    echo "    datafile: Path to the data"
    echo "    compressor: which compressor to use (sz3)"
    exit 1
fi

# give args usable names
n=$1
datafile=$2
compressor=$3


# loop. We use a while loop because we do not want to increment i for iterations
# we do not run a job, and a for loop makes that annoying.
i=0
while [ "$i" -lt "$n" ]; do
   
    whatsfree -c 192 > whatsfree.txt
    squeue > queue.txt

    # The Python script will take these slurm outputs and create the srun command
    # that we want to run to launch the job. i is passed to Python in case we want
    # to change the arguments to libpressio for each job we are running. Use i to
    # know which iteration we are on, or else don't use it.
    python parser.py whatsfree.txt queue.txt $compressor $datafile $i

    # If we determine there are not resources free to run the job yet, no command.txt
    # should be created. So if theres no command.txt, wait a while and try again
    # 5 seconds seems reasonable to me? But I don't know anything
    if [ ! -f "command.txt" ]; then
        sleep 5
        continue
    fi

    # so this should be the command we want to run based on how I want to do it.
    # if we want to take more arguments to send to libpressio, we should pass them
    # to python.
    SRUN_COMMAND=$(cat command.txt)
    
    eval "$SRUN_COMMAND"

    # Does srun give us a job number ID or something? we may want to save that 
    # and do something with it if so.

    # Delete command.txt so it doesn't get run again
    rm -f command.txt

    i=$((i + 1))

done

# Do we receive output from our srun jobs on the node we run the script on?
# We probably want to do something with the output here if so.