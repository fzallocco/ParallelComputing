/*
* 9/26/2023
* Filippo Zallocco
* CS610 - 71429
* Professor Turbé
* Assignment 1b
*/
/*Program that calculates permutation up to 8*/
#include <stdlib.h>

float x;
float arr[];
int i;
int t;
float p = 1.0;
main() {

	cout << "Enter a positive integer: " << endl; /*Prompting the user to enter a number. The program will run independently from the number's data type*/
	cin >> x; /*Storing the user's input in x*/
	
	for (i = 1; i < x + 1; i++) { /*Setting the for-loop to iterate from one till the end of the user input + 1*/
		p *= i; /*p is initialized to 1.0*/
		arr[i] = p; /*At each increment of i, the value p goes up by a factor equivalent to its preceeding product*/
		cout.precision(5); /*Setting the output precision to 5 decimal points*/
		cout << "index: " << i << " has value: " << arr[i] << endl;
	}
	/*This is my attempt to check the the array by printing it in reverse.*/
	/*for (t = i; 1 < t; t--) {
		
		cout << "index: " << t << " has value: " << arr[t] << endl;

	}*/
}