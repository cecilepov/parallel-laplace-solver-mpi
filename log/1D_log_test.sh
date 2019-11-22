(base) cecile@cecile-VivoBook-ASUS-Laptop-X505ZA-X505ZA:~/Documents/pvm$ mpicc -o laplace_1D laplace_1D.c -lm
(base) cecile@cecile-VivoBook-ASUS-Laptop-X505ZA-X505ZA:~/Documents/pvm$ mpirun -np 4 ./laplace_1D 12
Min: 0.000012  Max: 0.000021  Avg:  0.000017
Final solution is:
3 3 3 3 3 3 3 3 3 3 3 3
3 3 3 3 3 3 3 3 3 3 3 3
3 3 3 3 3 3 3 3 3 3 3 3
2 2 2 2 2 2 2 2 2 2 2 2
2 2 2 2 2 2 2 2 2 2 2 2
2 2 2 2 2 2 2 2 2 2 2 2
1 1 1 1 1 1 1 1 1 1 1 1
1 1 1 1 1 1 1 1 1 1 1 1
1 1 1 1 1 1 1 1 1 1 1 1
0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0


 Matrix printed by me: 0

 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1
 0 0 0 0 0 0 0 0 0 0 0 0
 0 0 0 0 0 0 0 0 0 0 0 0
 0 0 0 0 0 0 0 0 0 0 0 0
 1 1 1 1 1 1 1 1 1 1 1 1


 Matrix printed by me: 1

 0 0 0 0 0 0 0 0 0 0 0 0
 1 1 1 1 1 1 1 1 1 1 1 1
 1 1 1 1 1 1 1 1 1 1 1 1
 1 1 1 1 1 1 1 1 1 1 1 1
 2 2 2 2 2 2 2 2 2 2 2 2


 Matrix printed by me: 2

 1 1 1 1 1 1 1 1 1 1 1 1
 2 2 2 2 2 2 2 2 2 2 2 2
 2 2 2 2 2 2 2 2 2 2 2 2
 2 2 2 2 2 2 2 2 2 2 2 2
 3 3 3 3 3 3 3 3 3 3 3 3


 Matrix printed by me: 3

 2 2 2 2 2 2 2 2 2 2 2 2
 3 3 3 3 3 3 3 3 3 3 3 3
 3 3 3 3 3 3 3 3 3 3 3 3
 3 3 3 3 3 3 3 3 3 3 3 3
 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1
(base) cecile@cecile-VivoBook-ASUS-Laptop-X505ZA-X505ZA:~/Documents/pvm$
