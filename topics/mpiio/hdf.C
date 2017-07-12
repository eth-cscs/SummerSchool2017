#include <hdf5.h>
#include <stdio.h>
#include <mpi.h>
#include <string>
#define M_DIM 2
#define F_DIM 1
using namespace std;

int main(int argc, char **argv)
{

// initialize MPI engine
    int mpi_size,mpi_rank;
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&mpi_size);
    MPI_Comm_rank(MPI_COMM_WORLD,&mpi_rank);

// create data
    float * data;
    long nx, ny; //local size of data arrays
    long nsize;

    nx = 1600;
    ny = 1600;
    nsize = nx*ny;
    data = new float [nsize];
    for (long i=0; i<nsize; i++) data[i] = float(mpi_rank);

// create HDF5 file with parallel I/O properties
    herr_t ret;
    hid_t acc_tpl1;
    MPI_Info info = MPI_INFO_NULL;
    acc_tpl1 = H5Pcreate (H5P_FILE_ACCESS);
    ret = H5Pset_fapl_mpio(acc_tpl1, MPI_COMM_WORLD, info);
    
    hid_t fid1;
    string filename = "data_ind.h5";
    fid1 = H5Fcreate(filename.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, acc_tpl1);
    hid_t fid2;
    string filename1 = "data_coll.h5";
    fid2 = H5Fcreate(filename1.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, acc_tpl1);
    
// create the dataspaces: memory
    hsize_t mstart[M_DIM];
    hsize_t mcount[M_DIM];
    mstart[0] = 0;
    mstart[1] = 0;
    mcount[0] = nx;
    mcount[1] = ny;

    hid_t mem_dataspace;
    mem_dataspace = H5Screate_simple (M_DIM, mcount, NULL);

// create the dataspaces: file
    hsize_t fstart[F_DIM];
    hsize_t fstride[F_DIM];
    hsize_t fcount[F_DIM];
    hsize_t gfcount[F_DIM];
    hsize_t fblock[F_DIM];
    fstart[0] = mpi_rank*nx*ny;
    fcount[0] = nx*ny;
    fstride[0] = 1;
    fblock[0] = 1;
    
    gfcount[0] = mpi_size*fcount[0];

    hid_t file_dataspace;
    file_dataspace = H5Screate_simple (F_DIM, gfcount, NULL);

// Create the dataset
    hid_t data_id;
    data_id = H5Dcreate1(fid1,"mydata",H5T_NATIVE_FLOAT,file_dataspace,H5P_DEFAULT);

// set the hyperslabs
    ret = H5Sselect_hyperslab(file_dataspace,H5S_SELECT_SET,fstart,fstride,fcount,fblock);
    
    MPI_Barrier(MPI_COMM_WORLD);
    double t1 = MPI_Wtime();
// write data independently (default):
    ret = H5Dwrite(data_id,H5T_NATIVE_FLOAT,mem_dataspace,file_dataspace,H5P_DEFAULT, data);
    MPI_Barrier(MPI_COMM_WORLD);
    double t2 = MPI_Wtime();
    H5Dclose(data_id);

// write data collectively
    data_id = H5Dcreate1(fid2,"mydata",H5T_NATIVE_FLOAT,file_dataspace,H5P_DEFAULT);
    hid_t acc_tpl2 = H5Pcreate(H5P_DATASET_XFER);
    ret = H5Pset_dxpl_mpio(acc_tpl2, H5FD_MPIO_COLLECTIVE); // H5FD_MPIO_INDEPENDENT 
 
    MPI_Barrier(MPI_COMM_WORLD);
    double t3 = MPI_Wtime();
    ret = H5Dwrite(data_id,H5T_NATIVE_FLOAT,mem_dataspace,file_dataspace,H5P_DEFAULT, data);
    MPI_Barrier(MPI_COMM_WORLD);
    double t4 = MPI_Wtime();

// print timings
    if(mpi_rank == 0) printf("Time for independent %f (sec), time for collective %f (sec)\n",t2-t1,t4-t3);

// Finalize everything
    H5Sclose(mem_dataspace);
    H5Sclose(file_dataspace);
    H5Dclose(data_id);
    H5Fclose(fid1);
    H5Fclose(fid2);
    MPI_Finalize();

}
