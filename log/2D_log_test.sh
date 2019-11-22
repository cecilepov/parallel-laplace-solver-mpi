(base) cecile@cecile-VivoBook-ASUS-Laptop-X505ZA-X505ZA:~/Documents/pvm$ mpicc -o laplace_2D laplace_2D.c -lm
(base) cecile@cecile-VivoBook-ASUS-Laptop-X505ZA-X505ZA:~/Documents/pvm$ mpirun -np 9 ./laplace_2D 12
Min: 0.000031 seconds.  Max: 0.008479 seconds.  Avg:  0.002794 seconds.

Recv data is :

 Matrix printed by me: 0

 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1
 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2
 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3
 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4
 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5
 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6
 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7
 8 8 8 8 8 8 8 8 8 8 8 8 8 8 8 8

 -------------------------------
Intermediate data is :

 Matrix printed by me: 0

 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3
 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6
 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1
 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4
 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7
 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2
 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5
 8 8 8 8 8 8 8 8 8 8 8 8 8 8 8 8

 -------------------------------
Final solution is:

 Matrix printed by me: 0
 6 6 6 6 7 7 7 7 8 8 8 8
 6 6 6 6 7 7 7 7 8 8 8 8
 6 6 6 6 7 7 7 7 8 8 8 8
 6 6 6 6 7 7 7 7 8 8 8 8
 3 3 3 3 4 4 4 4 5 5 5 5
 3 3 3 3 4 4 4 4 5 5 5 5
 3 3 3 3 4 4 4 4 5 5 5 5
 3 3 3 3 4 4 4 4 5 5 5 5
 0 0 0 0 1 1 1 1 2 2 2 2
 0 0 0 0 1 1 1 1 2 2 2 2
 0 0 0 0 1 1 1 1 2 2 2 2
 0 0 0 0 1 1 1 1 2 2 2 2

 -------------------------------


 Matrix printed by me: 0

 -1 -1 -1 -1 -1 -1
 -1 0 0 0 0 1
 -1 0 0 0 0 1
 -1 0 0 0 0 1
 -1 0 0 0 0 1
 -1 3 3 3 3 -1


 Matrix printed by me: 1

 -1 -1 -1 -1 -1 -1
 0 1 1 1 1 2
 0 1 1 1 1 2
 0 1 1 1 1 2
 0 1 1 1 1 2
 -1 4 4 4 4 -1


 Matrix printed by me: 2

 -1 -1 -1 -1 -1 -1
 1 2 2 2 2 -1
 1 2 2 2 2 -1
 1 2 2 2 2 -1
 1 2 2 2 2 -1
 -1 5 5 5 5 -1


 Matrix printed by me: 3

 -1 0 0 0 0 -1
 -1 3 3 3 3 4
 -1 3 3 3 3 4
 -1 3 3 3 3 4
 -1 3 3 3 3 4
 -1 6 6 6 6 -1


 Matrix printed by me: 4

 -1 1 1 1 1 -1
 3 4 4 4 4 5
 3 4 4 4 4 5
 3 4 4 4 4 5
 3 4 4 4 4 5
 -1 7 7 7 7 -1


 Matrix printed by me: 5

 -1 2 2 2 2 -1
 4 5 5 5 5 -1
 4 5 5 5 5 -1
 4 5 5 5 5 -1
 4 5 5 5 5 -1
 -1 8 8 8 8 -1


 Matrix printed by me: 6

 -1 3 3 3 3 -1
 -1 6 6 6 6 7
 -1 6 6 6 6 7
 -1 6 6 6 6 7
 -1 6 6 6 6 7
 -1 -1 -1 -1 -1 -1


 Matrix printed by me: 7

 -1 4 4 4 4 -1
 6 7 7 7 7 8
 6 7 7 7 7 8
 6 7 7 7 7 8
 6 7 7 7 7 8
 -1 -1 -1 -1 -1 -1


 Matrix printed by me: 8

 -1 5 5 5 5 -1
 7 8 8 8 8 -1
 7 8 8 8 8 -1
 7 8 8 8 8 -1
 7 8 8 8 8 -1
 -1 -1 -1 -1 -1 -1
(base) cecile@cecile-VivoBook-ASUS-Laptop-X505ZA-X505ZA:~/Documents/pvm$
