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

    MPI_Status *status;
    MPI_File fh;
    int nnsize = nsize;

    for (i=0; i<nsize; i++) data[i] = float(rank);  
  
    // Write data (one file, MPI-IO, WRITE_AT)

    MPI_Barrier(MPI_COMM_WORLD);
    times[1] = MPI_Wtime();
    
    MPI_Offset offset = rank*nsize*sizeof(float);

    MPI_File_open(MPI_COMM_WORLD, "writeat.0000", MPI_MODE_WRONLY + MPI_MODE_CREATE,MPI_INFO_NULL, &fh);
    MPI_File_write_at(fh, offset, data, nnsize, MPI_FLOAT, status);
    MPI_File_close(&fh);
    //printf("--------------- 4\n");fflush(stdout);

    MPI_Barrier(MPI_COMM_WORLD);
    times[2] = MPI_Wtime();

    if(rank == 0)printf("Time for I/O WRITE AT (sec) = %f\n", times[2] - times[1]);

    // Write data (one file, MPI-IO, SIMPLE VIEW)

    MPI_Barrier(MPI_COMM_WORLD);
    times[1] = MPI_Wtime();

    MPI_File_open(MPI_COMM_WORLD, "view.0000", MPI_MODE_WRONLY + MPI_MODE_CREATE, MPI_INFO_NULL, &fh);
    MPI_File_set_view(fh, offset, MPI_FLOAT, MPI_FLOAT, "native", MPI_INFO_NULL);
    MPI_File_write(fh, data, nnsize, MPI_FLOAT, status);
    MPI_File_close(&fh);
   
    MPI_Barrier(MPI_COMM_WORLD);
    times[2] = MPI_Wtime();
    
    if(rank == 0)printf("Time for I/O VIEW (sec) = %f\n", times[2] - times[1]);

    // Write data (one file, MPI-IO, COLLECTIVE)

    MPI_Barrier(MPI_COMM_WORLD);
    times[1] = MPI_Wtime();

    MPI_File_open(MPI_COMM_WORLD, "collective.0000", MPI_MODE_WRONLY + MPI_MODE_CREATE, MPI_INFO_NULL, &fh);
    MPI_File_set_view(fh, offset, MPI_FLOAT, MPI_FLOAT, "native", MPI_INFO_NULL);
    MPI_File_write_all(fh, data, nnsize, MPI_FLOAT, status);
    MPI_File_close(&fh);

    MPI_Barrier(MPI_COMM_WORLD);
    times[2] = MPI_Wtime();
    
    if(rank == 0)printf("Time for I/O COLLECTIVE (sec) = %f\n", times[2] - times[1]);

    //

    delete [] data;
    // Finalize the MPI environment.
    MPI_Finalize();

}
