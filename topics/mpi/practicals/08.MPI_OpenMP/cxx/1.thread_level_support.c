/****************************************************************
 *                                                              *
 * This file has been written as a sample solution to an        *
 * exercise in a course given at the CSCS Summer School.        *
 * It is made freely available with the understanding that      *
 * every copy of this file must include this header and that    *
 * CSCS take no responsibility for the use of the enclosed      *
 * teaching material.                                           *
 *                                                              *
 * Purpose: demonstrate thread level support                    *
 *                                                              *
 * Contents: C-Source                                           *
 *                                                              *
 ****************************************************************/

#include <mpi.h>
#include <stdio.h>
 
int main( int argc, char *argv[] )
{
    int provided, requested;
 
/* Select one of the following at a time, check the support level
    MPI_Init_thread( 0, 0, MPI_THREAD_SINGLE, &provided );
    MPI_Init_thread( 0, 0, MPI_THREAD_FUNNELED, &provided );
    MPI_Init_thread( 0, 0, MPI_THREAD_SERIALIZED, &provided );
    MPI_Init_thread( 0, 0, MPI_THREAD_MULTIPLE, &provided );
*/ 
    requested = MPI_THREAD_MULTIPLE;
    MPI_Init_thread(0, 0, requested, &provided );
        printf( "Requested thread level= %d  Init_thread provided level= %d\n", requested, provided );
 
    MPI_Finalize();
}
