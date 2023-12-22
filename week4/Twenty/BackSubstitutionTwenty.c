/*
* 10/19/2023
* Filippo Zallocco
* CS610 - 71429
* Professor Turbé
* Project 3 - linear equations using lower triangular & BackSubstitution algorithm
*/
#define n 20 
float A[n + 1][n + 1] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						 0, 61, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						 0, 39,50, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						 0, 16,18,63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						 0, 67,68,10,19, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						 0, 21,62,73,26,30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						 0, 65,42,55,48,68,75, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						 0, 76,13,96,87,53,85,96, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						 0, 60,75,91,58,76,52,33,73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						 0, 31,22,73,12,57,68,80,18,48, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						 0, 80,18,38,31,51,90,91,58,31,87, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						 0, 66,42,76,75,44,38,67,69,83,91,76, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						 0, 97,36,85,37,24,27,66,64,36,53,35,51, 0, 0, 0, 0, 0, 0, 0, 0,
						 0, 29,71,93,58,98,20,89,11,90,37,19,49,60, 0, 0, 0, 0, 0, 0, 0,
						 0, 41,56,83,56,54,36,93,40,72,69,78,95,87,48, 0, 0, 0, 0, 0, 0,
						 0, 13,51,28,40,36,26,29,36,70,68,96,25,18,63,26, 0, 0, 0, 0, 0,
						 0, 23,84,19,62,54,75,23,29,56,42,43,86,92,81,60,68, 0, 0, 0, 0,
						 0, 61,88,32,75,83,13,32,92,46,92,15,88,33,77,59,13,92, 0, 0, 0,
						 0, 42,65,85,16,66,67,42,84,89,12,85,74,35,52,64,50,22,21, 0, 0,
						 0, 70,67,76,71,62,88,66,49,19,35,55,49,63,86,62,41,95,67,11, 0, 
						 0, 53,40,54,38,23,19,17,68,32,69,23,84,81,46,45,50,73,53,47, 70};
/*two-dimensional array A is a known data structure to the computer*/
float B[n + 1] = { 0,47,46,43,21,91,79,15,70,11,81,43,13,52,86,61,40,26,60,32 };  /*Array B is another known data structure to the computer, but the factor array x is not*/
float x[n + 1]; /*the factor array x is not*/
float stream pipechan[n + 2]; /*stream array created to update data into the pipeline*/
int i; /*pointer for loop*/

void PipeProcess(int i) {
	/*Calculates equation i to solve x[i]*/
	int j;
	float sum, xvalue;

	sum = 0;
	for (j = 1; j <= i - 1; j++) {
		recv(pipechan[i], xvalue); /*this instruction tells the Process i to look up x[j] at the far left*/
		send(pipechan[i + 1], xvalue); /*this instruction tells the Process i to send x[j] down to the pipeline as soon as Pi receives it*/
		sum = sum + A[i][j] * xvalue; /*Subsequently, we compute the sum of the previous products of a[i][j]s and x[j]s*/
	}
	x[i] = (B[i] - sum) / A[i][i]; /*Finally, we use the updated sum to determine the value of the current x[i], adding it to the pipeline for last*/
	send(pipechan[i + 1], x[i]);
	cout << "x["<<i<<"]"<< " is : " << x[i] << endl;
}

main() {

	forall i = 1 to n do
		PipeProcess(i);

	cout << "DONE" << endl;
}