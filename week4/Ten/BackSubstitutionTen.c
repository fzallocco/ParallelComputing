/*
* 10/19/2023
* Filippo Zallocco
* CS610 - 71429
* Professor Turbé
* Project 3 - linear equations using lower triangular & BackSubstitution algorithm
*/
#define n 10 
float A[n + 1][n + 1] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						  0,75,0, 0, 0, 0, 0, 0, 0, 0, 0,
						  0,95,48,0, 0, 0, 0, 0, 0, 0, 0,
						  0,17,74,79,0, 0, 0, 0, 0, 0, 0,
						  0,53,81,45,55,0, 0, 0, 0, 0, 0,
						  0,86,52,45,68,88,0, 0, 0, 0, 0,
						  0,67,76,51,43,34,75,0, 0, 0, 0,
						  0,82,11,84,25,67,26,61,0, 0, 0,
						  0,70,90,88,53,30,84,48,77,0, 0,
						  0,23,86,63,98,65,42,77,76,69,0,
						  0,68,45,45,10,12,71,98,76,76,54 };

/*two-dimensional array A is a known data structure to the computer*/
float B[n + 1] = {0, 60, 70, 80, 90, 33, 55, 66, 77, 91, 82};  /*Array B is another known data structure to the computer, but the factor array x is not*/
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
	cout << "x["<<i<<"]"<<" is : " << x[i] << endl;
}

main() {

	forall i = 1 to n do
		PipeProcess(i);

	cout << "DONE" << endl;
}