# Parallel implementation of Laplace equation

This project purposes a parallel implementation for solving Laplace equation using Jacobi method. 
I used the message passing library MPI for this implementation.

I invested 2 differents approaches:
- 1D decomposition;
- 2D decomposition.

## Repo organization

In the following folder, you will find:
- src: sources files and executables;
- output: examples of output files, for laplace computation ("compute" files) and data structures testing ("test" files, without laplace computation)  ;
- log: execution traces of each program.

## Packages installation

This project has been run on Ubuntu 16.04. A version of MPI must be installed, I used the open source version MPICH provided here: https://www.mpich.org/downloads/

If you are working on Ubuntu, you can easily install it be running the following command lines:
```shell

$ sudo apt-get update      # download the package list from their repo and update them
$ sudo apt install mpich   # install mpich in your system
```

## Compiling and running the code

To run and compile the code:

### laplace_1D. c:
```shell
$ mpicc -o laplace_1D laplace_1D.c -lm    # -lm is required because we use the <math.h> package
$ mpirun -np [nb of processors] ./laplace_1D [square matrix dimension]
```
Examples:
```shell
$ mpirun -np 4 ./laplace_1D 12    # 4 processors and a 12x12 square matrix
$ mpirun -np 3 ./laplace_1D 12    # 3 processors and a 12x12 square matrix
```

### laplace_2D. c:
```shell
$ mpicc -o laplace_2D laplace_2D.c -lm    # -lm is required because we use the <math.h> package
$ mpirun -np [nb of processors] ./laplace_2D [square matrix dimension]
```
Examples:
```shell
$ mpirun -np 9 ./laplace_2D 12    # 9 processors and a 12x12 square matrix
$ mpirun -np 4 ./laplace_2D 12    # 4 processors and a 12x12 square matrix
```

*Notes*: 
- the output file is written in your parent directory;
- for performance evaluation, remember to comment all printing/file saving steps. The ones outside the performance evaluation section can be kept.
