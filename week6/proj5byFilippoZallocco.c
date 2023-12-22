/*
* Filippo Zallocco
* CS610
* 11/7/2023
* Professor Turbe’
* Assignment 5 - Parallel Jacobi relaxation algorithm with local barrier and aggregation function
*/
#include <stdlib.h>
#include <math.h>
#define n 12 /*12*/ /*24*/ /*32*/ /*number of rows*/
#define numiter 2 /*number of iterations*/
#define tolerance .10

int stream higher[n + 1], stream lower[n + 1]; /*declaring streams globally for efficient local barriers*/
int count; /*declaration of count variable for Aggregation function*/
spinlock Arrival, Departure; /*declaring spinlocks for both Barrier and Aggregation functions*/
boolean globaldone; /*boolean variable declared globally for aggregation function*/

float A[n + 2][n + 2], B[n + 2][n + 2]; /*delcaration of matrices*/
int i, j; /*declaration of pointers*/
float change, maxchange; /*variables for accuracy evealuation*/

void Localbarrier(int me) { /*Efficient local barrier*/
	int dummy;
	
	if (me > 1)
		send(higher[me - 1], 1); /*send to Process me-1*/
	if (me < n) {
		send(lower[me + 1], 1); /*send to Process me+1*/
		recv(higher[me], dummy); /*receive from Process i-1*/
	}
	if (me > 1)
		recv(lower[me], dummy); /*receive from Process i+1*/
}

boolean Aggregate(boolean mydone) {
	boolean result;

	/*Arrival Phase - Count the processes arriving*/
	Lock(Arrival);
		count = count + 1;
		globaldone = globaldone && mydone; /*aggregation*/
	if (count < n)
		Unlock(Arrival); /*continue Arrival Phase*/
		else Unlock(Departure); /*end Arrival Phase*/
	/*Departure Phase - Count the processes leaving*/
	Lock(Departure);
		count = count - 1;
		result = globaldone; /*return 'done' flag*/
	if (count > 0)
		Unlock(Departure); /*continue Departure Phase*/
		else {
			Unlock(Arrival); /*terminate Departure Phase*/
			globaldone = true; /*reset for new Aggregation*/
		}

	return(result);
}
main() {
	count = 0; /*Initialize 'count' and spinlocks*/
	Unlock(Arrival); 
	Lock(Departure);
	globaldone = true; /*initializing global flag*/
	/*Initializing matrix A*/
	/*int z, y;*/
	forall i = 1 to n+1 do {
		int j;
		for (j = 1; j <= n+1; j++) {
			float Random = (rand() % 200) / 200.0;
			A[i][j] = Random;
		}
	}

	B = A;
	forall i = 1 to n do { /*creating the processes*/
		int j;
		float change, maxchange;
		boolean done;
		do {
			maxchange = 0;
			for (j = 1; j <= n; j++) { /*compute new value for each point in my row*/
				B[i][j] = (A[i - 1][j] + A[i + 1][j] + A[i][j - 1] + A[i][j + 1]) / 4; /*averaging out values left, right, up and down of the current value*/
				change = fabs(B[i][j] - A[i][j]);
				if (change > maxchange) maxchange = change;
			}
			Localbarrier(i);
			A[i] = B[i];
			Localbarrier(i);
			done = Aggregate(maxchange < tolerance);
		} while (!done); /*iterate process by process until function is done*/
	}

	cout.precision(3);
	cout << "Filippo Zallocco's project 5 output " << endl;

	for (i = 0; i <= n + 1; i++) {
		cout << "Row:  " << i << "   Result" << endl;
		for (j = 0; j <= n + 1; j++) {
			if ((j > 0) && (j % 10) == 0) {
				cout << endl;
			}
			cout << A[i][j] << "  ";
		}
		cout << endl;
	}
}