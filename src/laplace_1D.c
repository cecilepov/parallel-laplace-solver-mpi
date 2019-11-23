/*
----------------------------------------------------------------------

Parallel implementation of Laplace equation
V1 : Using rows/1D decomposition only
Authors : POV Cécile - CARNEIRO ESPINDOLA Stela - JOLY Morgane
Date : 01/10/2019

*****
To run and compile the code:
$ mpicc -o laplace_1D laplace_1D.c -lm
$ mpirun -np [nb of processors] ./laplace_1D [square matrix dimension]

Examples:
$ mpirun -np 4 ./laplace_1D 12
$ mpirun -np 3 ./laplace_1D 12

Note : for performance evaluation, remember to comment all printing/file saving steps.
The ones outside the performance evaluation section can be kept.

----------------------------------------------------------------------
*/


#include "mpi.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>


/**
 * Print the matrix given in parameters, in increasing indexes order
 */
void print_matrix(int me, int N, float *local_tab, int nb_rows)
{
    int i,j;
    printf("\n \n Matrix printed by me: %d \n\n", me);
    for(i = 0; i<nb_rows; i++)
    {
        for(j = 0; j<N; j++)
        {
            printf(" %.2f", *(local_tab+j+i*N)); // Change ".2f" to "%d" for more clarity (test mode, without laplace calculation)
            //printf(" %d",(int) *(local_tab+j+i*N));
        }
        printf("\n");
    }
}


/**
 * Update values of the adjacent rows of all the local matrices
 */
void update_matrix (float *local_tab, int nb_rows, int N, int NPROC, int me)
{
    MPI_Status status;

    /* ---- SENDING ---- */
    if (me != 0) // If I am not the processor 0
        MPI_Send(local_tab+N, N, MPI_FLOAT, me-1, 1, MPI_COMM_WORLD); // I send the 2nd row (1st index: local_tab+N) of my local tab to previous processor (TAG = 1)

    if (me != NPROC-1) // If am not the last processor
        MPI_Send(local_tab+(nb_rows-2)*N, N, MPI_FLOAT, me+1, 2, MPI_COMM_WORLD); // I send the second-last row (1st index: local_tab+(nb_rows-2)*N) of my local tab to the next processor (TAG = 2)


    /* ---- RECEIVING ADJACENT ROWS ---- */
    if (me != 0) // If I am not the processor 0
        MPI_Recv(local_tab, N, MPI_FLOAT, me-1, 2, MPI_COMM_WORLD, &status); // I receive the msg of TAG = 1 and I update the 1st row of my local tab

    if (me != NPROC-1) // If I am not the last processor
        MPI_Recv(local_tab+(nb_rows-1)*N, N, MPI_FLOAT, me+1, 1, MPI_COMM_WORLD, &status); // I receive the msg of TAG = 2 and I update the last row of my tab
}


/**
 * Compute the laplacian equation
 */
void laplace(float* local_tab, int nb_rows, int N, int NPROC, int me)
{
	float *new_tab = (float*)malloc(N*nb_rows*sizeof(float));
    if (new_tab == NULL) { exit(-1); } // Check if the memory has been well allocated

	double PRECISION = 1.0e-2; // Precision/required accuracy
	double global_error = +INFINITY;
    int iter_count = 0;

	while(global_error >= PRECISION )  // while the error is not as accurated as we want (PRECISION), we continue the loop
	{
		double local_error_sum = 0;
        iter_count++;

		for (int i=1; i < nb_rows-1; i++)
		{
			for(int j=0; j<N; j++)
			{
				float top_neighbor     = *(local_tab+j+(i-1)*N);
				float bottom_neighbor  = *(local_tab+j+(i+1)*N);

                float left_neighbor, right_neighbor;

				// Left edge effect handling
				if(j==0)   {left_neighbor = -1;}
				else       {left_neighbor = *(local_tab+(j-1)+i*N);}

				// Right edge effect handling
				if(j==N-1) {right_neighbor = -1;}
				else       {right_neighbor = *(local_tab+(j+1)+i*N);}

				*(new_tab+j+i*N) = 0.25*(bottom_neighbor + top_neighbor + left_neighbor + right_neighbor);

				local_error_sum += (*(new_tab+j+i*N) - *(local_tab+j+i*N))
				                 * (*(new_tab+j+i*N) - *(local_tab+j+i*N));
			}
		}

        // Replace local_tab values by new_tab values
		for (int i=1; i < nb_rows-1; i++)
		{
			for(int j=0; j<N; j++)
			{
				*(local_tab+j+i*N) = *(new_tab+j+i*N);
			}
		}

        update_matrix (local_tab, nb_rows, N, NPROC, me) ;
		double global_error_sum = 0;
		MPI_Allreduce( &local_error_sum, &global_error_sum, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD ); // we sum errors of all processors and put the result in global_error_sum

        global_error = sqrt(global_error_sum);
        if (me == 0)
        {
            printf( "Iteration %d - error = %e\n", iter_count, global_error );
        }
	}
	free(new_tab);
}


/**
 * Initialize the local matrix : all the values are set to the rank of the processor, except the adjacent values (set to -1)
 * Example : for processor of rank = 0 and a local matrix 5x10, the result is :
   -1 -1 -1 -1 -1 -1 -1 -1 -1 -1
    0  0  0  0  0  0  0  0  0  0
    0  0  0  0  0  0  0  0  0  0
    0  0  0  0  0  0  0  0  0  0
   -1 -1 -1 -1 -1 -1 -1 -1 -1 -1
 */
void initialize_local_matrix(int me, int NPROC, int N, float *local_tab, int nb_rows)
{
    int i,j;
    for(i = 0; i<nb_rows; i++)
    {
        for(j = 0; j<N; j++)
        {
            if ((i == 0) || (i == nb_rows-1))
            {
                *(local_tab+j+i*N) = -1;
            }
            else
            {
                *(local_tab+j+i*N) = me;
            }
        }
    }
}


/**
 * Gather all the local matrices data from the processors and print the whole reconstructed matrix
 */
void print_final_matrix(int me, float* local_tab, int N , int NPROC)
{
    float *final_matrix = (float*)malloc(N*N*sizeof(float));
    if (final_matrix == NULL) { exit(-1); } // Check if the memory has been well allocated

    int root_id = 0; // numero of the processor responsible of gathering the data
    MPI_Gather( local_tab+N,N*(N/NPROC), MPI_FLOAT, final_matrix, N*(N/NPROC), MPI_FLOAT, root_id, MPI_COMM_WORLD ); // we gather all the data from other processors

    if (me == 0)
    {
        printf( "Final solution is:\n" );
        for (int i=N-1; i>=0; i--) // reverse printing
        {
            for (int j=0; j<N; j++)
            {
                printf("%.2f ", *(final_matrix + j + i*N)); // Change ".2f" to "%d" for more clarity (test mode, without laplace calculation)
                //printf("%d ",(int) *(final_matrix + j + i*N));
            }
            printf("\n");
        }
    }
    free(final_matrix);
}


/**
 * Gather all the local matrices data from the processors and save the reconstructed matrix in a file
 */
void save_file_final_matrix (char filename[], int me, float* local_tab, int N , int NPROC)
{
    FILE *f;
    int i,j;
    float *final_matrix = (float*)malloc(N*N*sizeof(float));
    if (final_matrix == NULL) { exit(-1); }  // Check if the memory has been well allocated

    int root_id = 0 ; // numero of the processor responsible of gathering the data
    MPI_Gather(local_tab+N, N*(N/NPROC), MPI_FLOAT, final_matrix, N*(N/NPROC), MPI_FLOAT, root_id, MPI_COMM_WORLD );

    if (me == 0)
    {
        if ((f = fopen (filename, "w")) == NULL) { perror ("matrix_save: fopen "); }
        for (i = N-1; i>=0; i--)
        {
            for (j=0; j<N; j++)
            {
                fprintf (f, "%f ", *(final_matrix + j + i*N) ); // Change "%f" to "%d" for more clarity (test mode, without laplace calculation)
                //fprintf (f, "%d ", (int) *(final_matrix + j + i*N) );
            }
            fprintf (f, "\n");
        }
        fclose (f);
    }
    free(final_matrix);
}


/**
 * Main function
 */
int main( int argc, char *argv[] )
{
    if (argc < 2)
    {
        printf("Argument missing. Usage: mpirun -np [number of processors] ./my_exe [matrix dimension]\nexample: mpirun -np 6 ./my_exe 12\n");
        exit(-1);
    }

    MPI_Init( &argc, &argv );
    int N = atoi(argv[1]); // NPROC : number of processors, me : rank of the actual processor
    int NPROC, me ;
    double start_time, max_time, min_time, avg_time, local_time ;

    MPI_Comm_rank( MPI_COMM_WORLD, &me );
    MPI_Comm_size( MPI_COMM_WORLD, &NPROC );
    MPI_Barrier(MPI_COMM_WORLD);  //synchronize all processes

    start_time = MPI_Wtime();  // get time just before work section

    if (N%NPROC != 0)
    {
        printf("ERROR: In this version, number of processors must be a multiple of matrix dimension.\n");
        MPI_Finalize();
        exit(-1);
    }

    // 1D PARTIIONING
    int nb_rows    = (N/NPROC)+2  ;
    float* local_tab = NULL;
    local_tab = (float *)malloc(sizeof(float)*N*nb_rows);
    if (local_tab == NULL) { exit(-1);} // Check if the memory has been well allocated

    // COMPUTATION AND MATRIX FILLING
    initialize_local_matrix(me, NPROC, N, local_tab, nb_rows);
    update_matrix (local_tab, nb_rows, N, NPROC, me);
    laplace(local_tab, nb_rows, N, NPROC, me); // laplace computation. Comment this line to verify message sending/receiving and data structures.

    // PERFORMANCE EVALUATION
    local_time = MPI_Wtime() - start_time;  // get time just after work section

    MPI_Reduce(&local_time, &max_time, 1, MPI_DOUBLE, MPI_MAX, 0,MPI_COMM_WORLD);
    MPI_Reduce(&local_time, &min_time, 1, MPI_DOUBLE, MPI_MIN, 0,MPI_COMM_WORLD);
    MPI_Reduce(&local_time, &avg_time, 1, MPI_DOUBLE, MPI_SUM, 0,MPI_COMM_WORLD);

    if (me == 0)
    {
        avg_time /= NPROC;
        printf("\nMin: %lf  Max: %lf  Avg:  %lf\n", min_time, max_time, avg_time);
    }

    print_final_matrix(me, local_tab, N , NPROC);
    save_file_final_matrix("result_laplace_1D.txt",me, local_tab, N, NPROC);

    // Print all the local matrices (not for performance evaluation section)
    for (int i = 0; i < NPROC ; i++)
    {
        if (me == i)
        {
            print_matrix(i, N, local_tab, nb_rows);
        }
        MPI_Barrier(MPI_COMM_WORLD);  // synchronize all processes to prevent "overlap" during the printing
    }

    MPI_Finalize();
    free(local_tab);
    return 0;
}
