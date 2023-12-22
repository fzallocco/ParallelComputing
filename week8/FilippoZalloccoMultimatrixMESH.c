/*
* Filippo Zallocco
* CS610
* 11/28/2023
* Professor Turbe’
* Assignment 7 - Matrix Multiplication on a Torus
*/
ARCHITECTURE TORUS(7); /* A torus is a network topology similar to a mesh, and we use it to connect processing nodes in a parallel computing system with two dimensions F_Zallocco*/
#include <stdlib.h> /* importing standard library F_Zallocco*/
#define m 7 /* number of rows and columns of the matricies F_Zallocco*/
#define p 5 /* size of two-dimensional partition; each partition will be assigned to an individual processor F_Zallocco*/
typedef float partition[p+1][p+1]; /* F_Zallocco*/
partition A[m][m], B[m][m], C[m][m]; /* partitioned marticies F_Zallocco*/
partition stream Achan[m][m], stream Bchan[m][m]; /* partitioned streams F_Zallocco*/
int i,j; /* pointers for the matricies F_Zallocco*/

void Multiply(int row, int col, value partition myA, value partition myB, partition mainC)
{
  int i,j,k,iter,above,left; /* declaring all the pointers to make the matricies values rotate so all processors can work on their own values without having to wait too long on each other's calculations F_Zallocco*/
  partition myC = {0}; /* initializing particion of C matrix F_Zallocco*/
  if (row > 0) above = row-1; /* up neighbor F_Zallocco*/
    else above = m-1; /* F_Zallocco*/
  if (col > 0) left = col-1; /* left neighbor F_Zallocco*/
    else left = m-1; /* F_Zallocco*/
  for (iter = 1; iter <= m; iter++) { /* iterate over the indices of the matrices F_Zallocco*/
    send(Achan[row][left], myA); /* sends partitioned matrix A into stream of left-rotated row F_Zallocco*/
	  send(Bchan[above][col], myB); /* sends partitioned matrix B into stream of up-rotated column F_Zallocco*/
	  /*Here we multiply the A and B partitions*/
	  for (i = 1; i <= p; i++) /* for all rows starting from 1 till end of partitions F_Zallocco*/
	   for (j = 1; j <= p; j++) /* for all columns starting from 1 till end of partitions F_Zallocco*/
		  for (k = 1; k <= p; k++) /*for all vector indices starting from1 till end of partitons  F_Zallocco*/
		    myC[i][j] = myC[i][j] + myA[i][k] * myB[k][j]; /* each processor computes value and stores it F_Zallocco*/
	  recv(Achan[row][col], myA); /* stream receives new value from partitioned A F_Zallocco*/
	  recv(Bchan[row][col], myB); /* stream receives new vakue from partitioned B F_Zallocco*/
  }
  mainC = myC; /* saving product of partioned C matrix into matrix C F_Zallocco*/
}

main( ) {

  int k, l; /* declaring pointers to intialize all three matrices F_Zallocco*/
  for (i = 0; i < m; i++) /* for all rows F_Zallocco*/
	 for (j = 0; j < m; j++) /* for all columns F_Zallocco*/
	  for (k = 1; k <= p; k++) /* for all vector products F_Zallocco*/
	    for (l = 1; l <= p; l++) { /* for all indicies F_Zallocco*/
		    A[i][j][k][l] = (rand() % 100)/10.0; /* generate random floats for matrix A F_Zallocco*/
		    B[i][j][k][l] = (rand() % 100)/10.0; /* generate random floats for matrix B F_Zallocco*/
	     }

  for (i = 0; i < m; i++) /* for all rows F_Zallocco*/
	 for (j = 0; j < m; j++) /* for all columns F_Zallocco*/
	  fork (@i * m + j) /* separate all processes from main F_Zallocco*/
      Multiply(i, j, A[i][(j+i)%m], B[(i+j)%m][j], C[i][j]); /* Perform matrix multiply with Torus on matrices A, B, and C F_Zallocco*/

  for (i = 0; i < m; i++) /* for all rows F_Zallocco*/
	 for (j = 0; j < m; j++) /* for all columns F_Zallocco*/
	  join; /* join all completed processes to main F_Zallocco*/

  cout.precision(4); /* define output precision to four digits F_Zallocco*/
  for (i = 0; i < m; i++) { /* for all rows F_Zallocco*/
	 for (j = 0; j < m; j++) { /* for all columns F_Zallocco*/
	  cout << "partition[" << i << ", " << j << "]" << endl; /* printing out all values calculated on partitioned matrices F_Zallocco*/
	  for (k = 1; k <= p; k++) { /* F_Zallocco*/
		 for (l = 1; l <= p; l++) /* F_Zallocco*/
		  cout << C[i][j][k][l] << ", "; /* returning values F_Zallocco*/
	    cout << endl; /* F_Zallocco*/
	  }
	 }
  }
}
