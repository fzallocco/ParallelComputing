/*
* Filippo Zallocco
* CS610
* 10/26/2023
* Professor Turbe’
* Assignment 4 option A
*/
#include <stdlib.h>
/* Number of lines in <image>.pgm header */
#define header_lines 4 
#define image_columns 142
#define image_rows 110
#define MaxGrayComponentValue 255
#define carriage_return '\n'

/* Here's the image array you will read into */
int image_in[image_rows + 1][image_columns + 1];
/*this is the histogram array*/
int hist[MaxGrayComponentValue + 2];
/*this is the spinlock array*/
spinlock L[MaxGrayComponentValue + 1];
/*here's your iterators to scan through the image*/
int i, j;
/* End of Global variables Shared Memory*/

void read_image_header()
{
    /* this procedure reads the four header lines and throw them away*/
    char buffer[header_lines + 1][80]; /* buffer for storing input line */
    char c;
    int m;
    for (m = 1; m <= header_lines; ++m)
        do
        {
            cin >> c;  /* input next character */
        } while (c != carriage_return);
}
void read_image_values()
{/* read image into array */
    for (i = 1; i <= image_rows; i++)
        for (j = 1; j <= image_columns; j++)
            cin >> image_in[i][j];
}

main()
{

    read_image_header();

    read_image_values();

    /*for (i = 1; i <= image_rows; i++) {
        for (j = 1; j <= image_columns; j++) {
            cout << i <<  j << image_in[i][j] << endl;
        }
    }*/

    for (i = 1; i <= MaxGrayComponentValue; i++) /*initializing histogram array to a length of 257 integers*/
        hist[i] = 0;
    forall i = 1 to image_rows do { /*Parallelizing the program to 110 processes using image's rows for easy analysis*/
        int j, intensity; /*declaring block-specific pointers to prevent shared data interference among processes*/
        for (j = 1; j <= image_columns; j++) {
            intensity = image_in[i][j]; /*fetching the value intensity from 2D array*/
            Lock(L[intensity]); /*Every process locks the intensity value inside the L array and then process it and finally unlocks it*/
                hist[intensity] = hist[intensity] + 1; /*Here, the intensity value is passed into the histogram array*/
            Unlock(L[intensity]); 
        }
    }
    for (i = 1; i <= MaxGrayComponentValue; i++) /*This function prints the output to the terminal; however, the analysis will not focus on the values of the array*/
        cout << hist[i] << endl;
}