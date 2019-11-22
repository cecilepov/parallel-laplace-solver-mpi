/*
----------------------------------------------------------------------

Parallel implementation of Laplace equation
V2 : Using 2D decomposition
Authors : POV Cécile - CARNEIRO ESPINDOLA Stela - JOLY Morgane
Date : 01/10/2019

*****
To run and compile the code:
$ mpicc -o laplace_2D laplace_2D.c -lm
$ mpirun -np [nb of processors] ./laplace_2D [square matrix dimension]

Examples:
$ mpirun -np 9 ./laplace_2D 12
$ mpirun -np 4 ./laplace_2D 12

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
 * Print the matrix given in parameter, in increasing indexes order
 */
void print_matrix(int me,  float *tab, int nb_rows, int nb_cols)
{
    printf("\n \n Matrix printed by me: %d \n \n", me);
    for(int i = 0; i<nb_rows; i++)
    {
        for(int j = 0; j<nb_cols; j++)
        {
            printf(" %.2f", *(tab+j+i*nb_cols));  // Change ".2f" to "%d" for more clarity (test mode, without laplace calculation)
            //printf(" %d",(int) *(tab+j+i*nb_cols));
        }
        printf("\n");
    }
}


/**
 * Print the matrix given in parameter, in decreasing rows index order
 */
void print_matrix_reverse(int me, float *tab, int nb_rows, int nb_cols)
{
    printf("\n \n Matrix printed by me: %d \n", me);
    for (int i = nb_rows-1; i>=0; i--)
    {
        for(int j = 0; j<nb_cols; j++)
        {
            printf(" %.2f",*(tab+j+i*nb_cols)); // Change ".2f" to "%d" for more clarity (test mode, without laplace calculation)
            //printf(" %d",(int) *(tab+j+i*nb_cols));
        }
        printf("\n");
    }
}


/**
 * Update values of the adjacent rows of all the local matrices
 */
void update_rows(float *local_tab, int Nlocal_rows, int Nlocal_cols, int NPROC, int me, int NBCUTS)
{
    MPI_Status status;
    int msg_tag1 = 1;
    int msg_tag2 = 2;

    /*
     *    1. Communicate my first SIGNIFICANT data row to refresh the last ADJACENT row of the previous processor above me (Message TAG = 1)
     */
    if (me > NBCUTS-1) // If I am not part of the first row of processors
        MPI_Send(local_tab+Nlocal_cols+1, Nlocal_cols-2, MPI_FLOAT, me-NBCUTS, msg_tag1, MPI_COMM_WORLD); // I send the 2nd row (1st index: local_tab+Nlocal_cols+1) of my local tab to precessor above me

    if(me < NPROC - NBCUTS) // If I am not part of the last row of processors
        MPI_Recv(local_tab+1+(Nlocal_rows-1)*Nlocal_cols, Nlocal_cols-2, MPI_FLOAT, me+NBCUTS, msg_tag1, MPI_COMM_WORLD, &status); // I receive the msg of TAG = 1 and I update the last row of my tab

    /*
     *    2. Communicate my last SIGNIFICANT data row to refresh the first ADJACENT row of the next processor under me (Message TAG = 2)
     */
    if(me < NPROC - NBCUTS) // If I am not part of the last row of processors
        MPI_Send(local_tab+1+(Nlocal_rows-2)*Nlocal_cols, Nlocal_cols-2, MPI_FLOAT, me+NBCUTS, msg_tag2, MPI_COMM_WORLD); // I send the second-last row (1st index: local_tab+1+(Nlocal_rows-2)*Nlocal_cols) of my local tab to the processor under me

    if (me > NBCUTS-1) // If I am not part of the first row of processors
        MPI_Recv(local_tab+1, Nlocal_cols-2, MPI_FLOAT, me-NBCUTS, msg_tag2, MPI_COMM_WORLD, &status); // I receive the msg of TAG = 2 and I update the 1st row of my local tab
}


/**
 * Update values of the adjacent columns of all the local matrices
 */
void update_cols(float *local_tab, int Nlocal_rows, int Nlocal_cols, int NPROC, int me, int NBCUTS)
{
    MPI_Status status;
    int msg_tag3 = 3;
    int msg_tag4 = 4;

    MPI_Datatype column;
    MPI_Type_vector(Nlocal_rows-2, 1, Nlocal_cols, MPI_FLOAT, &column); // define
    MPI_Type_commit(&column);

    /*
     *    3. Communicate my last SIGNIFICANT data column to refresh the first ADJACENT column of the next processor - on my right (Message TAG = 3)
     */
    if (me%NBCUTS != NBCUTS-1) // If I am not part of the last column of processors
        MPI_Send(local_tab+Nlocal_cols*2-2, 1, column, me+1, msg_tag3, MPI_COMM_WORLD); // I send the second-last column (1st index : local_tab+Nlocal_cols*2-2) of my local tab to the next processor

    if (me%NBCUTS != 0) // If I am not part of the first column of processors
        MPI_Recv(local_tab+Nlocal_cols, 1, column, me-1, msg_tag3, MPI_COMM_WORLD,  &status );  // I receive the msg of TAG = 3 and I update the 1st column of my local tab

    /*
     *    4. Communicate my first SIGNIFICANT data column to refresh the last ADJACENT row of the previous processor - on my left (Message TAG = 4)
     */
    if (me%NBCUTS != 0) // If I am not part of the first column of processors
        MPI_Send(local_tab+Nlocal_cols+1, 1, column, me-1, msg_tag4, MPI_COMM_WORLD); // I send the second column (1st index : local_tab+Nlocal_cols+1) of my local tab to the previous processor

    if (me%NBCUTS != NBCUTS-1) // // If I am not part of the last column of processors
        MPI_Recv(local_tab+Nlocal_cols*2-1, 1, column, me+1, msg_tag4, MPI_COMM_WORLD,  &status );  // I receive the msg of TAG = 4 and I update the last column of my local tab
}


/**
 * Update values of the adjacent data (rows and columns) of all the local matrices
 */
void update_matrix (float *local_tab, int Nlocal_rows, int Nlocal_cols, int NPROC, int me, int NBCUTS)
{
    update_rows(local_tab, Nlocal_rows, Nlocal_cols, NPROC, me, NBCUTS);
    update_cols(local_tab, Nlocal_rows, Nlocal_cols, NPROC, me, NBCUTS);
}


/**
 * Compute the laplacian equation
 */
void laplace( float* local_tab, int Nlocal_rows, int Nlocal_cols, int NPROC, int me, int NBCUTS)
{
    float *new_tab = (float*)malloc(Nlocal_rows*Nlocal_cols*sizeof(float)); // temporary tab to store new values of local_tab
    if (new_tab == NULL) { exit(-1); } // Check if the memory has been well allocated

    double PRECISION = 1.0e-2; // Precision/required accuracy
    double global_error = +INFINITY;

    int iter_count = 0;

    while(global_error >= PRECISION ) // while the error is not as accurated as we want (PRECISION), we continue the loop
    {
        double local_error_sum = 0;
        iter_count++;

        for (int i=1; i < Nlocal_rows-1; i++)
        {
            for(int j=1; j<Nlocal_cols-1; j++)
            {
                float top_neighbor     = *(local_tab+j+(i-1)*Nlocal_cols);
				float bottom_neighbor  = *(local_tab+j+(i+1)*Nlocal_cols);
                float left_neighbor    = *(local_tab+(j-1)+i*Nlocal_cols);
                float right_neighbor   = *(local_tab+(j+1)+i*Nlocal_cols);

                *(new_tab+j+i*Nlocal_cols) = 0.25*(bottom_neighbor + top_neighbor + left_neighbor + right_neighbor); // laplace equation formula

                local_error_sum += (*(new_tab+j+i*Nlocal_cols) - *(local_tab+j+i*Nlocal_cols))
                                 * (*(new_tab+j+i*Nlocal_cols) - *(local_tab+j+i*Nlocal_cols)) ; // calculate the error and add it to local_error_sum
            }
        }

        //  Replace local_tab values by new_tab values
        for (int i=1; i < Nlocal_rows-1; i++)
        {
            for(int j=1; j<Nlocal_cols-1; j++)
            {
                *(local_tab+j+i*Nlocal_cols) = *(new_tab+j+i*Nlocal_cols);
            }
        }

        update_matrix (local_tab, Nlocal_rows, Nlocal_cols, NPROC, me,NBCUTS) ;
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
 * Gather all the local matrices data from the processors and print the whole reconstructed matrix
 */
void print_and_save_final_matrix(char filename[], int me, float* local_tab, int Nlocal_rows, int Nlocal_cols, int N ,int NPROC, int NBCUTS)
{
    int i,j;
    MPI_Status status;
    MPI_Request req;

    MPI_Datatype mysubarray; // creates a datatype for a subarray of a regular, multidimensional array : we will use it on local_tab to extract only significant values
    int starts[2] = {1, 1}; // starting coordinates (i,j) of the subarray : our first significant element in local_tab is at (1,1)
    int subsizes[2]  = {Nlocal_rows-2, Nlocal_cols-2}; // dimensions of the subarray (significant values) to extract
    int bigsizes[2]  = {Nlocal_rows, Nlocal_cols}; // dimensions of the array (local_tab) in which we want to extract a subarray (significant values)

    MPI_Type_create_subarray(2, bigsizes, subsizes, starts, MPI_ORDER_C, MPI_FLOAT, &mysubarray); // create the new datatype ; 2 = number of dimensions
    MPI_Type_commit(&mysubarray); // commit the datatype

    MPI_Isend(local_tab, 1, mysubarray, 0, me, MPI_COMM_WORLD, &req); // send significant values of local_tab (using new datatype) to processor 0

    MPI_Type_free(&mysubarray);
    MPI_Barrier(MPI_COMM_WORLD);

    int nb_subdata = (Nlocal_rows-2) * (Nlocal_cols-2) ; // number of significant data (no adjacent data) in local_tab

    if(me == 0) // processor 0 is responsible of gathering all the data
    {
        /*
            STEP 1 : Receive the data from all processor
            recv_matrix is of the form : row 0 - data received from processor 0 , row 1 - data received from processor 1 ; etc.
            0 0 0 0 0 0 0 0 0 0 -> data from proc0
            1 1 1 1 1 1 1 1 1 1 -> data from proc1
            2 2 2 2 2 2 2 2 2 2 -> data from proc2
            3 3 3 3 3 3 3 3 3 3 -> data from proc3
        */
        float *recv_matrix = (float*)malloc(N*N*sizeof(float));
        if (recv_matrix == NULL) { exit(-1); } // Check if the memory has been well allocated

        for(i = 0; i < NPROC; i++) // for all the processors, processor 0 stores the received values in a single row of recv_matrix
        {
            MPI_Recv(recv_matrix+i*nb_subdata, nb_subdata, MPI_FLOAT, i, i, MPI_COMM_WORLD, &status);
        }

        printf( "\nRecv data is :" );
        print_matrix(me, recv_matrix, NPROC, nb_subdata);
        printf( "\n ------------------------------- \n" );

        /*
            STEP 2 : Reorder rows by the "modulo" of their processor rank
            0 0 0 0 0 0 0 0 0 0 -> data from proc0 => modulo 0
            2 2 2 2 2 2 2 2 2 2 -> data from proc2 => modulo 0
            1 1 1 1 1 1 1 1 1 1 -> data from proc1 => modulo 1
            3 3 3 3 3 3 3 3 3 3 -> data from proc3 => modulo 1
        */
        float *ordered_matrix = (float*)malloc(N*N*sizeof(float));
        if (ordered_matrix == NULL) { exit(-1); } // Check if the memory has been well allocated
        int pivot_inter = 0;
        int modulo_result = 0;
        for (int modulo_result = 0 ; modulo_result < NBCUTS ; modulo_result++)
        {
            for (int i = 0; i<NPROC; i++)
            {
                if (i%NBCUTS == modulo_result)
                {
                        for (int j = 0; j<nb_subdata;j++)
                        {
                            *(ordered_matrix+j+pivot_inter*nb_subdata) = *(recv_matrix+j+i*nb_subdata) ;
                        }
                        pivot_inter = pivot_inter + 1;
                }
            }
        }

        printf( "Intermediate data is :" );
        print_matrix(me, ordered_matrix, NPROC, nb_subdata);
        printf( "\n ------------------------------- \n" );


        /*
            STEP 3 : Get the final matrix by reordering by "blocks"
            0 0 0 0 0 1 1 1 1 1                                     2 2 2 2 2 3 3 3 3 3
            0 0 0 0 0 1 1 1 1 1    ------------------------->       2 2 2 2 2 3 3 3 3 3
            2 2 2 2 2 3 3 3 3 3      print_matrix_reverse()         0 0 0 0 0 1 1 1 1 1
            2 2 2 2 2 3 3 3 3 3                                     0 0 0 0 0 1 1 1 1 1
        */
        float *final_matrix = (float*)malloc(N*N*sizeof(float));
        if (final_matrix == NULL) { exit(-1); } // Check if the memory has been well allocated
        int offset = 0;

        for (int vertical_step = 0 ; vertical_step < NBCUTS; vertical_step++)
        {
            for (int i = 0; i<N ; i++)
            {
                for (int j = (Nlocal_cols-2)*vertical_step ; j < ((Nlocal_cols-2)*vertical_step)+(Nlocal_cols-2) ; j++)
                {
                    *(final_matrix+j+i*N) = *(ordered_matrix+offset);
                    offset = offset+1;
                }
            }
        }

        // Print the final matrix
        printf( "Final solution is:" );
        print_matrix_reverse(me, final_matrix, N, N);
        printf( "\n ------------------------------- \n" );

        // Save the final matrix in a file (reverse order)
        FILE *f;
        if (me == 0)
        {
            if ((f = fopen (filename, "w")) == NULL) { perror ("matrix_save: fopen "); }
            for (int i = N-1; i>=0; i--)

            {
                for (int j=0; j<N; j++)
                {
                    fprintf (f, "%f ", *(final_matrix + j + i*N) ); // Change ".2f" to "%d" for more clarity (test mode, without laplace calculation)
                    //fprintf (f, "%d ", (int) *(final_matrix + j + i*N) );
                }
                fprintf (f, "\n");
            }
            fclose (f);
        }

        free(recv_matrix);
        free(ordered_matrix);
        free(final_matrix);
    }
}


/**
 * Initialize the local matrix : all the values are set to the rank of the processor, except the adjacent values (set to -1)
 * Example : for processor of rank = 0 and a local matrix 5x5, the result is :
 * -1 -1 -1 -1 -1
   -1  0  0  0 -1
   -1  0  0  0 -1
   -1  0  0  0 -1
   -1 -1 -1 -1 -1
 */
void initialize_local_matrix(int me, int NPROC, int nb_cols, float *local_tab, int nb_rows)
{
    int count = 0 ; // useful for debug mode
    for(int i = 0; i<nb_rows; i++)
    {
        for(int j = 0; j<nb_cols; j++)
        {
            if ((i == 0) || (i == nb_rows-1))
            {
                *(local_tab+j+i*nb_cols) = -1;
            }
            else if ((j == 0) || (j == nb_cols-1))
            {
                *(local_tab+j+i*nb_cols) = -1;
            }
            else
            {
                *(local_tab+j+i*nb_cols) = me ;
                //count++;
            }
        }
    }
}


/**
 * Check if the number if a perfect square or not
 * Problems may occurs for a very big number
 */
int is_perfect_square(int number)
{
    int intResult;
    float floatResult;

    floatResult =sqrt((double)number);
    intResult = floatResult;

    if (intResult==floatResult) return 1;
    else return 0;
}


/**
 * Main function
 */
int main( int argc, char *argv[] )
{
    if (argc < 2)
    {
        printf("Argument missing. Usage: mpirun -np [number of processors] ./my_exe [N square matrix dimension]\nexample: mpirun -np 9 ./my_exe 12\n");
        exit(-1);
    }

    MPI_Init( &argc, &argv );
    int NPROC, me ; // NPROC : number of processors, me : rank of the actual processor
    double start_time, max_time, min_time, avg_time, local_time;
    int N = atoi(argv[1]); // square matrix dimension

    MPI_Comm_rank( MPI_COMM_WORLD, &me );
    MPI_Comm_size( MPI_COMM_WORLD, &NPROC );
    MPI_Barrier(MPI_COMM_WORLD);  // synchronize all processes

    start_time = MPI_Wtime();  //get time just before work section


    // 2D PARTITIONING
    int NBCUTS; // In how many parts rows and columns of the original matrix is cut.
    if (is_perfect_square(NPROC) == 1) // If it is a perfect square, we calculate the square root
    {
        NBCUTS = sqrt(NPROC);
    }
    else
    {
        printf("ERROR: In this version, number of cuts must be the same for both columns and rows. Please choose a perfect square number of processors (as 4 or 9 for example.).\n");
        MPI_Finalize();
        exit(-1);
    }

    const int NBLOCK = N/NBCUTS;  //number of SIGNIFICANT rows and columns in a BLOCK
    if (N != NBCUTS*NBLOCK)
    {
        printf("ERROR: imcompatible number of processors and matrix size. Multiplication between number of cuts (NBCUTS - how many parts rows and columns of the original matrix is cut) and number of significant rows and columns in a block should be equal to N the number of rows and columns of the original matrix. We expect N = NBCUTS x NBLOCK, but we have %d = %d x %d\n", N, NBCUTS, NBLOCK);
        MPI_Finalize();
        exit(-1);
    }

    int Nlocal    = NBLOCK+2  ; // "real" number of rows of a local matrix. We added +2 for the neibhbors (ADJACENT values).
    float* local_tab = NULL; // all the processors have their own local matrix containing the values of the original matrix their are responsible of + neighbor values
    local_tab = (float *)malloc(sizeof(float)*Nlocal*Nlocal);
    if (local_tab == NULL) { exit(-1); } // Check if the memory has been well allocated

    // COMPUTATION AND MATRIX FILLING
    initialize_local_matrix(me, NPROC, Nlocal, local_tab, Nlocal);
    update_matrix (local_tab, Nlocal, Nlocal, NPROC, me, NBCUTS); // first update of neighbors values
    laplace(local_tab, Nlocal, Nlocal, NPROC, me, NBCUTS); // laplace computation. Comment this line to verify message sending/receiving and data structures.


    // PERFORMANCE EVALUATION
    local_time = MPI_Wtime() - start_time;  // get time just after work section

    MPI_Reduce(&local_time, &max_time, 1, MPI_DOUBLE, MPI_MAX, 0,MPI_COMM_WORLD);
    MPI_Reduce(&local_time, &min_time, 1, MPI_DOUBLE, MPI_MIN, 0,MPI_COMM_WORLD);
    MPI_Reduce(&local_time, &avg_time, 1, MPI_DOUBLE, MPI_SUM, 0,MPI_COMM_WORLD);

    if (me == 0)
    {
        avg_time /= NPROC;
        printf("\nMin: %lf seconds.  Max: %lf seconds.  Avg:  %lf seconds.\n", min_time, max_time, avg_time);
    }

    print_and_save_final_matrix("result_laplace_2D.txt", me, local_tab, Nlocal, Nlocal, N, NPROC, NBCUTS);

    // Print all the local matrices (not for performance evaluation section)
    for (int i = 0; i < NPROC ; i++)
    {
        if (me == i)
        {
            print_matrix(i, local_tab,Nlocal, Nlocal);
        }
        MPI_Barrier(MPI_COMM_WORLD);  // synchronize all processes to prevent "overlap" during the printing
    }

    MPI_Finalize();
    free(local_tab);
    return 0;
}
