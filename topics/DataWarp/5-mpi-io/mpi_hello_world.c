// Author: Wes Kendall
// Copyright 2011 www.mpitutorial.com
// This code is provided freely with the tutorials on mpitutorial.com. Feel
// free to modify it for your own use. Any distribution of the code must
// either provide a link to www.mpitutorial.com or keep this header intact.
//
// An intro MPI hello world program that uses MPI_Init, MPI_Comm_size,
// MPI_Comm_rank, MPI_Finalize, and MPI_Get_processor_name.
//
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {

  setenv("MPICH_MPIIO_HINTS_DISPLAY", "1", 1);

  // Initialize the MPI environment. The two arguments to MPI Init are not
  // currently used by MPI implementations, but are there in case future
  // implementations might need the arguments.
  MPI_Init(NULL, NULL);

  // Get the number of processes
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // Get the rank of the process
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  // Get the name of the processor
  char processor_name[MPI_MAX_PROCESSOR_NAME];
  int name_len;
  MPI_Get_processor_name(processor_name, &name_len);

  // Print off a hello world message
  printf("Hello world from processor %s, rank %d out of %d processors\n",
         processor_name, world_rank, world_size);

  // START MPI-IO EXAMPLE
  char filename[1024];
  const char* p = getenv("DW_JOB_STRIPED");
  sprintf(filename, "%sout.dat", p);
  int ierr;
  MPI_File fh;
  MPI_Info info;
  MPI_Info_create(&info);
  //MPI_Info_set(info, "ind_rd_buffer_size", "16777216");
  //MPI_Info_set(info, "ind_wr_buffer_size", "16777216");
  //MPI_Info_set(info, "romio_ds_write", "disable");
  ierr = MPI_File_open(MPI_COMM_WORLD, filename,
                       MPI_MODE_CREATE|MPI_MODE_WRONLY,
                       info, &fh);
                       //MPI_INFO_NULL, &fh);
  printf("Open  %d: %d\n", world_rank, ierr);
  MPI_Info_free(&info);

  MPI_Status sts;
  char buff[50];
  sprintf(buff, "I'm rank %d 12345678901234567890\n", world_rank);
  ierr = MPI_File_write_at_all(fh, world_rank*32, buff, 32, MPI_BYTE, &sts); 
  printf("Write %d: %d\n", world_rank, ierr);

  ierr = MPI_File_close(&fh);
  printf("Close %d: %d\n", world_rank, ierr);
  
  // Finalize the MPI environment. No more MPI calls can be made after this
  MPI_Finalize();
}
