/*
* Filippo Zallocco
* CS610
* 11/15/2023
* Professor Turbe’
* Assignment 6 - Parallel numeric integration on a 2-D mesh
*/
ARCHITECTURE MESH2(25);
#include <stdlib.h>
#include <math.h>

#define numproc 40 /*150* /*40*/ /*number of processes*/
#define numpoints 30 /*70*/ /*30*/ /*number of points for numerical integration*/
float processList[numproc]; /*array containing all the processes*/
float stream globalSumValue[numproc]; /*stream data structure acting as a queue stores the processes' values*/

float startPoint, endPoint, spaceValue, sumValue, answer; /*floating-point variables to hold values */
int i, j, r; /*pointers for the for-loops*/

float f(float t) { /*function for integration*/
	return(sqrt(4 - t * t)); /*calculate the square root of the difference between 4 and the square of a given number*/
}

void Integrate(int myindex, float startPoint, float endPoint, float spaceValue) {

	float localsum = 0; /*variable that stores the local sum*/

	float t;

	int j; /*pointer*/

	t = startPoint + myindex * (endPoint - startPoint) / numproc; /*start position*/

	for (j = 1; j <= numpoints; j++) {

		localsum = localsum + f(t); /*pass new position into integration function and add its output to local sum*/

		t = t + spaceValue; /*update start position with space*/

	}

	localsum = spaceValue * localsum; /*local sum is updated with product of space and localsum*/

	send(globalSumValue[myindex], localsum); /*write local sum into stream - FIFO basis -*/

}

main() {
	/*initialize values of end points 'a' and 'b'*/

	startPoint = 0; /*starting boundary point*/

	endPoint = 2; /*end boundary points*/

	int localSum = 0;

	spaceValue = (endPoint - startPoint) / (numproc * numpoints); /*spacing of points*/

	forall i = 0 to numproc - 1 do /*Create processes*/
		Integrate(i, startPoint, endPoint, spaceValue); /*Calling the integration function*/

	for (r = 0; r < numproc; r++) {

		recv(globalSumValue[r], processList[r]); /*read stream values*/

	}

	for (i = 0; i < numproc; i++) {

		sumValue = sumValue + processList[i]; /*Updating sum with number of process*/
	}

	cout.precision(10); /*setting the print statement precision to 10 digits after the decimal*/

	answer = sumValue + spaceValue / 2 * (f(endPoint) - f(startPoint)); /*calculating the approximation of Pi to the nearest tenth digit*/

	cout << "Integral of f(x) from a to b: " << answer;
}