//  Testing various sorting algorithms

//
//  Counting sort code used from algorithm created by
//  Harold H. Seward
//
//  ************* Timing Results *************
//
// Times for counting sort
// Fastest time at          5: 0.000000953674
// Fastest time at        500: 0.0000519753
// Fastest time at    100,000: 0.00191092
// Fastest time at  1,000,000: 0.0269771
// Fastest time at 1m presort: 0.011205
// Fastest time at 1m revsort: 0.011374
//
// Times for radix sort
// Fastest time at          5: 0.00000500679
// Fastest time at        500: 0.0000550747
// Fastest time at    100,000: 0.00964212
// Fastest time at  1,000,000: 0.112131
// Fastest time at 1m presort: 0.101783
// Fastest time at 1m revsort: 0.113702
//
// Times for modified quicksort
// Fastest time at          5: 0.000000953674
// Fastest time at        500: 0.0000450611
// Fastest time at    100,000: 0.0120869
// Fastest time at  1,000,000: 0.147764
// Fastest time at 1m presort: 0.0485899
// Fastest time at 1m revsort: 0.0421031


#include <iostream>
#include <sys/time.h>
#include <cmath>
using namespace std;

void countingsort(int [], int );
/*void radixsort(int [], int );
void quicksort(int[], int, int);*/

int main()
{
    struct timeval before, after;
    double timing;
    
    // Variables only used to print
    // and verify array was sorted
    /*int columns = 16;
    int counter = 0;*/
    int size;
    
    
    cout << "What is the size of the array? ";
    
    cin >> size;
    
    cout << size << endl;
    
    int *array = new int[size];
    
    // Fill array values from redirected input
    for (int i = 0; i < size; i++)
        cin >> array[i];
    
    gettimeofday(&before, 0);
    
    // Testing various sorts
    countingsort(array,size);
    /*radixsort(array,size);
    quicksort(array, 0, size);*/
    
    gettimeofday(&after, 0);
    timing = (double) ((double)after.tv_sec  + (double)after.tv_usec/(1000*1000)) -
    (double) ((double)before.tv_sec + (double)before.tv_usec/(1000*1000));
    
    cout << endl << "Total time taken: " << timing << endl << endl;
    
    /* outputs contents of array in pretty fashion
    for (int j = 0; j < size; j++)
    {
        if (counter == columns)
        {
            counter = 0;
            cout << endl;
        }
        cout << array[j] << "   \t";
        counter++;
    }*/
    
    delete [] array;
    
    return 0;
}
/* Algorithm used created by Harold H. Seward
 
    Counting sort creates k buckets that store the 
    number of times the kth element was seen in the 
    input array. It then makes two passes over the 
    input array. During the first pass, it increments 
    the count of the kth bucket. In the second pass, it 
    overwrites the original array by processing the 
    count values in the total ordering provided by k buckets.*/

void countingsort(int array[], int size)
{
    int i, j, k;
    int index = 0;
    int min, max;
    
    min = max = array[0];
    // Find min and max
    for(i = 1; i < size; i++)
    {
        min = (array[i] < min) ? array[i] : min;
        max = (array[i] > max) ? array[i] : max;
    }
    
    k = max - min + 1;
    // Create k buckets
    int *bucket = new int [k];
    
    // Each index i in bucket counts how many
    // elements in array have the value i
    for(i = 0; i < size; i++)
        bucket[array[i] - min]++;
    
    // Counts stored in bucket now used to
    // put elements into array at right position
    for(i = min; i <= max; i++)
        for(j = 0; j < bucket[i - min]; j++)
            array[index++] = i;
    
    
    delete [] bucket;
} // countingsort()

// After testing, Counting Sort beat out other sorts.
// Only countingsort will be used.

/*************** Code left for reference.************/

/*  Radix sort orders the contents position by position. 
    It is a non-comparitive sort. Starting from MSB
    (most significant bit) group the numbers into buckets. 
    When you move to next position again re-order them for 
    that position, while keeping the order of the previous 
    position re-orders. Once all the positions are done, 
    the whole list should be ordered.*/

/*void radixsort(int array[], int n)
{
    
    // Find the max number in the given list.
    // to be used in loop termination part.
    int maxNumber = array[0];
    for (int i = 1; i < n; i++)
    {
        if (array[i] > maxNumber)
            maxNumber = array[i];
    }
    
    // Run the loop for each of the decimal places
    int exp = 1;
    int *tmpBuffer = new int[n];
    while (maxNumber / exp > 0)
    {
        int decimalBucket[10] = {  0 };
        // Count the occurences in this decimal digit.
        for (int i = 0; i < n; i++)
            decimalBucket[array[i] / exp % 10]++;
        
        // Prepare the position counters to be used for
        //  re-ordering the numbers for this decimal place.
        for (int i = 1; i < 10; i++)
            decimalBucket[i] += decimalBucket[i - 1];
        
        // Re order the numbers in the tmpbuffer 
        // and later copy back to original buffer.
        for (int i = n - 1; i >= 0; i--)
            tmpBuffer[--decimalBucket[array[i] / exp % 10]] = array[i];
        for (int i = 0; i < n; i++)
            array[i] = tmpBuffer[i];
        
        // Move to next decimal place.
        exp *= 10;
        
    }
} // radixsort()

 
 A slightly modified quicksort function.
 
 We are choosing our mid point as the "left + (right - left) / 2".
 We are using this instead of the usual "(left + right) / 2" because
 the latter can cause overflow. This method is called median of three
 Overflow and stability improved/fixed by using our method
 since it is bound by the right pivot point.
 
 Naturally, we would think using rand() would be better, however, this
 adds a lot of overhead due to the recursive nature of quicksort.
 
 Note: instead of using swap() function, we are using a temp int. This
 is because calling another function adds unnecessary overhead and
 slows down the sorting.
 
void quicksort(int array[], int left, int right)
{
    int l = left, r = right, temp;
    int pivot = array[left + (right - left) / 2];
    
    while (l <= r)
    {
        while (array[l] < pivot)
            l++;
        while (array[r] > pivot)
            r--;
        if (l <= r)
        {
            temp = array[l];
            array[l] = array[r];
            array[r] = temp;
            l++;
            r--;
        }
    };
    
    if (left < r)
        quicksort(array, left, r);
    if (l < right)
        quicksort(array, l, right);
} // quicksort()*/
