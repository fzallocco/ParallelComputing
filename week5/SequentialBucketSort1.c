/* Sequential bucket sort of n integers in range [0, n] */
#include <stdlib.h>
#include <math.h>
#define n 2000 /* number of values to be sorted */
#define m 100  /* each number is in range [0, m-1] */
#define bucket_number 10 /* each bucket for all numbers with the same 10th digit value */
int data[n+1], sortedData[n+1]; /* data to be sorted, and data sorted */
int bucket[bucket_number][n+1]; /* bucket[2][0] is number of values with leading 2; bucket[2][1..] holds all values with 10th digit equal to 2; in the worst case, all n values have the same 10th digit value */
int i, j;

/* sort d[1:m] */
void sort(int d[]) {
  /* d[0]: # of integers in d[1:m] to be sorted */
  int j, k, max, max_index, temp;
  for (j = d[0]; j > 1; j--) {
	max = d[j];
	max_index = j;
	for (k = 1; k < j; k++) {
      if (d[k] > max) {
	    max = d[k];
		max_index = k;
	  }
	}
	/* swap d[j] and d[max_index] */
	temp = d[j];
	d[j] = max;
	d[max_index] = temp;
  }

  return;
}

main() {
  /* generate n random integers in [0, m-1] to be sorted */
  /* for better speed-up of its parallel version, read Appendic C on Page 540 to use "seqoff" and "seqon" statements to exclude running time for such sequential code not essential for sorting */
  int v, b_i, index;

  for (i = 1; i <= n; i++)
	data[i] = rand() % m;

  for (i = 1; i <= n; i++) {
	v = data[i];
	b_i = (int)(v/10.0); /* bucket-index = the 10th digit */
	/* cout << v << ": " << b_i << "; " << endl; */
    index = ++bucket[b_i][0]; /* find index of first unused box for v; */
	bucket[b_i][index] = v;
  }

  for (i = 0; i <= 9; i++)
	sort(bucket[i]);

  int k = 1;
  for (i = 0; i <= 9; i++)
	for (j = 1; j < bucket[i][0]; j++)
	  sortedData[k++] = bucket[i][j];

  k--;
  for (i = 1; i <= k; i++)
	cout << "d[" << i << "]=" << sortedData[i] << endl;
}
