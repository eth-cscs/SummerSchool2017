#include <stdio.h>
#include <mpi.h>
#define nsize 1000000

int main(int argc, char** argv) {

    int i, rank, size, ierror;
    double times[10];
    for (i=0; i<10; i++) times[i] = 0.0;

    // Initialize the MPI environment
    MPI_Init(&argc,&argv);

    // Get the number of processes and rank
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Allocate and initializae main array
    float * data, fulldata;
    data = new float [nsize];
    for (i=0; i<nsize; i++) data[i] = float(rank);  
  
    // Write data (one file per processor)

    MPI_Barrier(MPI_COMM_WORLD);
    times[1] = MPI_Wtime();
    
    int iunit = 1000+rank;
    char sunit[9];
    sprintf(sunit, "data.%d",iunit);
    //printf("%s\n",sunit); 
    FILE *fp = fopen(sunit, "wb");
    fwrite(data, sizeof(float), nsize, fp);
    fclose(fp);

    MPI_Barrier(MPI_COMM_WORLD);
    times[2] = MPI_Wtime();
    if(rank == 0)printf("Time for I/O (sec) = %f\n", times[2] - times[1]);

    delete [] data;
    // Finalize the MPI environment.
    MPI_Finalize();

}
