/*
* Filippo Zallocco
* CS610
* 12/8/2023
* Professor Turbe’
* Assignment 8 - MPI-based parallel program for spacial filtering of a visual image
*/
ARCHITECTURE HYPERCUBE(7); /* A hypercube is a network topology similar to a mesh, and we use it to connect processing nodes in a parallel computing system with two dimensions F_Zallocco*/
#include <mpi.h>
#include <stdlib.h>
#include <math.h>

#define n 640 /*dimension of image F_Zallocco*/
#define m 5 /*number of rows in each partition F_Zallocco*/
int inrows[m + 2][n + 2], outrows[m][n]; /*creating two 2D-arrays, one for the input pixel values and another one for the ouput pixel values F_Zallocco*/
int filter[3][3] = { 1, 1, 1, 1, 1, 1, 1, 1, 1}; /*creating a 3 by 3 filter to apply over the pixels of an image F_Zallocco*/
MPI_Status status;
int myrank, totalproc, blocksize;

void input_image() { /*input and partition the image F_Zallocco*/
	int i, j, k, dest, tag, source; /*declaring the pointers for the below instructions F_Zallocco*/
	/*input pixel values for image F_Zallocco*/
	int image[n + 2][n + 2]; 
	for (i = 0; i <= n + 1; i++)
		for (j = 0; j <= n + 1; j++)
			image[i][j] = rand() % 254; /*Random number generator populates the 2D image array F_Zallocco*/

	/*send partition to each process F_Zallocco*/
	for (k = 0; k < totalproc; k++) {
		dest = k; tag = 1;
		MPI_Send(&image[k * m][0], blocksize, MPI_INT, dest, tag, MPI_COMM_WORLD); /*sending a large block of data with a single call starting from image array with k by m F_Zallocco*/
	}
}
main() {
	int i, j, k, dest, tag, source; /*declaring the pointers again for the below instructions F_Zallocco*/
	MPI_Init();
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	MPI_Comm_size(MPI_COMM_WORLD, &totalproc);
	blocksize = (m + 2) * (n + 2); /*Size of the partition declared F_Zallocco*/
	if (myrank == 0) input_image(); /*Calling input function F_Zallocco*/

	/*receive my block of image F_Zallocco*/
	source = 0; tag = 1;
	MPI_Recv(&inrows[0][0], blocksize, MPI_INT, source, tag, MPI_COMM_WORLD, &status);

	/*apply filter mask to my partition F_Zallocco*/

	for (i = 1; i <= m; i++)
		for (j = 1; j <= n; j++)
			outrows[i - 1, j - 1] =
			(int)((filter[0, 0] * inrows[i - 1, j - 1] /*multiply the value of the pixel by the value of the filter F_Zallocco*/
				+ filter[0, 1] * inrows[i - 1, j] /*Add all the products up and divide by 9 F_Zallocco*/
				+ filter[0, 2] * inrows[i - 1, j + 1]
				+ filter[1, 0] * inrows[i, j - 1]
				+ filter[1, 1] * inrows[i, j - 1]
				+ filter[1, 2] * inrows[i, j]
				+ filter[2, 0] * inrows[i + 1, j - 1]
				+ filter[2, 1] * inrows[i + 1, j]
				+ filter[2, 2] * inrows[i + 1, j + 1]) / 9); /*replacing the value with the weighted sum of the neighboring pixel values F_Zallocco*/

	/*send filtered partition back to process 0 F_Zallocco*/
	blocksize = m * n;
	dest = 0; tag = 2;
	MPI_Send(&outrows[0][0], blocksize, MPI_INT, dest, tag, MPI_COMM_WORLD); /*Sending new values to processes F_Zallocco*/

	/*assemble partitions into filtered image as output F_Zallocco*/

	if (myrank == 0) {
		for (k = 0; k < totalproc; k++) {
			source = k; tag = 2;
			MPI_Recv(&outrows[0][0], blocksize, MPI_INT, source, tag, MPI_COMM_WORLD, &status); /*Processors received partitioned output starting from outrows[0][0] and assemble them back up F_Zallocco*/
			for (i = 0; i < m; i++) {
				for (j = 0; j < n; j++) {
					cout << outrows[i][j];
					if ((j % 20) == 9) {
						cout << endl;
					}
				}
				cout << endl;
			}
		}
	}

	MPI_Finalize();
}