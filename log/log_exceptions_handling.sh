(base) cecile@cecile-VivoBook-ASUS-Laptop-X505ZA-X505ZA:~/Documents/pvm$ mpicc -o laplace_1D laplace_1D.c -lm
(base) cecile@cecile-VivoBook-ASUS-Laptop-X505ZA-X505ZA:~/Documents/pvm$ mpirun -np 9 ./laplace_1D
Argument missing. Usage: mpirun -np [number of processors] ./my_exe [matrix dimension]
example: mpirun -np 6 ./my_exe 12
Argument missing. Usage: mpirun -np [number of processors] ./my_exe [matrix dimension]
example: mpirun -np 6 ./my_exe 12
Argument missing. Usage: mpirun -np [number of processors] ./my_exe [matrix dimension]
example: mpirun -np 6 ./my_exe 12
Argument missing. Usage: mpirun -np [number of processors] ./my_exe [matrix dimension]
example: mpirun -np 6 ./my_exe 12
Argument missing. Usage: mpirun -np [number of processors] ./my_exe [matrix dimension]
example: mpirun -np 6 ./my_exe 12
Argument missing. Usage: mpirun -np [number of processors] ./my_exe [matrix dimension]
example: mpirun -np 6 ./my_exe 12
Argument missing. Usage: mpirun -np [number of processors] ./my_exe [matrix dimension]
example: mpirun -np 6 ./my_exe 12
Argument missing. Usage: mpirun -np [number of processors] ./my_exe [matrix dimension]
example: mpirun -np 6 ./my_exe 12
Argument missing. Usage: mpirun -np [number of processors] ./my_exe [matrix dimension]
example: mpirun -np 6 ./my_exe 12
(base) cecile@cecile-VivoBook-ASUS-Laptop-X505ZA-X505ZA:~/Documents/pvm$ mpirun -np 9 ./laplace_1D 12
ERROR: In this version, number of processors must be a multiple of matrix dimension.
ERROR: In this version, number of processors must be a multiple of matrix dimension.
ERROR: In this version, number of processors must be a multiple of matrix dimension.
ERROR: In this version, number of processors must be a multiple of matrix dimension.
ERROR: In this version, number of processors must be a multiple of matrix dimension.
ERROR: In this version, number of processors must be a multiple of matrix dimension.
ERROR: In this version, number of processors must be a multiple of matrix dimension.
ERROR: In this version, number of processors must be a multiple of matrix dimension.
ERROR: In this version, number of processors must be a multiple of matrix dimension.
(base) cecile@cecile-VivoBook-ASUS-Laptop-X505ZA-X505ZA:~/Documents/pvm$
(base) cecile@cecile-VivoBook-ASUS-Laptop-X505ZA-X505ZA:~/Documents/pvm$
(base) cecile@cecile-VivoBook-ASUS-Laptop-X505ZA-X505ZA:~/Documents/pvm$ mpicc -o laplace_2D laplace_2D.c -lm(base) cecile@cecile-VivoBook-ASUS-Laptop-X505ZA-X505ZA:~/Documents/pvm$ mpirun -np 5 ./laplace_2D Argument missing. Usage: mpirun -np [number of processors] ./my_exe [N square matrix dimension]
example: mpirun -np 9 ./my_exe 12
Argument missing. Usage: mpirun -np [number of processors] ./my_exe [N square matrix dimension]
example: mpirun -np 9 ./my_exe 12
Argument missing. Usage: mpirun -np [number of processors] ./my_exe [N square matrix dimension]
example: mpirun -np 9 ./my_exe 12
Argument missing. Usage: mpirun -np [number of processors] ./my_exe [N square matrix dimension]
example: mpirun -np 9 ./my_exe 12
Argument missing. Usage: mpirun -np [number of processors] ./my_exe [N square matrix dimension]
example: mpirun -np 9 ./my_exe 12
(base) cecile@cecile-VivoBook-ASUS-Laptop-X505ZA-X505ZA:~/Documents/pvm$ mpirun -np 6 ./laplace_2D 12
ERROR: In this version, number of cuts must be the same for both columns and rows. Please choose a perfect square number of processors (as 4 or 9 for example.).
ERROR: In this version, number of cuts must be the same for both columns and rows. Please choose a perfect square number of processors (as 4 or 9 for example.).
ERROR: In this version, number of cuts must be the same for both columns and rows. Please choose a perfect square number of processors (as 4 or 9 for example.).
ERROR: In this version, number of cuts must be the same for both columns and rows. Please choose a perfect square number of processors (as 4 or 9 for example.).
ERROR: In this version, number of cuts must be the same for both columns and rows. Please choose a perfect square number of processors (as 4 or 9 for example.).
ERROR: In this version, number of cuts must be the same for both columns and rows. Please choose a perfect square number of processors (as 4 or 9 for example.).
(base) cecile@cecile-VivoBook-ASUS-Laptop-X505ZA-X505ZA:~/Documents/pvm$
