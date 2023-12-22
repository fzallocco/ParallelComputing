/*
* 10/19/2023
* Filippo Zallocco
* CS610 - 71429
* Professor Turbé
* Project 3 - linear equations using lower triangular & BackSubstitution algorithm
*/
#define n 4 
float A[n + 1][n + 1] = { 0, 0, 0, 0, 0,
						  0, 40, 0, 0, 0,
						  0, 55, 60, 0, 0,
						  0, 70, 75, 80, 0,
		                  0, 82, 83, 84, 85 }; 
/*two-dimensional array A is a known data structure to the computer*/
float B[n + 1] = { 0, 6, 7, 8, 9 };  /*Array B is another known data structure to the computer, but the factor array x is not*/
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
	cout << "x["<<i<<"]  is : " << x[i] << endl;
}

main() {

	forall i = 1 to n do
		PipeProcess(i);

	cout << "DONE" << endl;
}