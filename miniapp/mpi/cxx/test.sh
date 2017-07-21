for dim in 128 256 512
do
    echo ======= ${dim}x${dim}
    for p in 1 2 4 8
    do
        for t in 1 2 4 8 12
        do
            printf "%3d nodes, %3d ranks,  %3d threads : "  $p $p $t
            OMP_NUM_THREADS=$t srun -n$p -N$p -c12 --hint=nomultithread ./main $dim $dim 100 0.01 | grep took;
        done
    done
done
