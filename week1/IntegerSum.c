/*
* 9/18/2023
* Filippo Zallocco
* CS610 - 71429
* Professor Turbé
* Assignment 1a
*/
/*Program that adds up all integers from 1 to 100*/
#include <stdlib.h>
#define n 101 /*Define n constant as 101*/

int arr[n]; /*The length of the integer-based array is set to 101 because we start indexing from 1 to 100 included and thus need an additional position in the array*/
int x; /*Declare global value x for the for loop*/
int sum = 0; /*Initializing the current sum to 0*/

main() {

	for (x = 1; x < n; x++) { /*From x set to 1 on, as long as x is less than 101, execute the below statements and increment x by 1*/
		arr[x] = x; /*Save x's current value in the array*/
		cout << "At index : " << x << " the array value is: " << arr[x] << endl; /*Ouput to the terminal the value of the array at a specific index*/
		sum += arr[x]; /*Add the value of x to sum, thus updating the variable sum*/
	}

	cout << "sum is " << sum; /*Display the total sum in the terminal window*/
}

