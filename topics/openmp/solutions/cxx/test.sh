#!/bin/bash
executable=./a.out
if [ -f "$1.exe" ]; then
    executable="./$1.exe"
fi

echo "============ testing $executable ============"

for threads in 1 2 4 8 12
do
    echo "============  $threads threads ============"
    export OMP_NUM_THREADS=$threads
    srun -pdebug -t5 -Cgpu -n1 -c$threads --hint=nomultithread $executable
done

