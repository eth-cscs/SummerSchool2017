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
    float * data, * fulldata;
    data = new float [nsize];
    fulldata = new float [size*nsize];
    for (i=0; i<nsize; i++) data[i] = float(rank);  
    for (i=0; i<nsize; i++) fulldata[i] = 0.0;
  
    // Write data (processor 0 collecting data and writing)

    MPI_Barrier(MPI_COMM_WORLD);
    times[1] = MPI_Wtime();

    MPI_Gather(data, nsize, MPI_FLOAT, fulldata, nsize, MPI_REAL, 0, MPI_COMM_WORLD);
    
    if (rank == 0)
    {
       FILE *fp = fopen("masterslave.0000", "wb");
       fwrite(fulldata, sizeof(float), size*nsize, fp);
       fclose(fp);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    times[2] = MPI_Wtime();
    if(rank == 0)printf("Time for I/O (sec) = %f\n", times[2] - times[1]);

    delete [] data;
    delete [] fulldata;
    // Finalize the MPI environment.
    MPI_Finalize();

}
