#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>

/** 
 * Returns OK or FAIL as boolean string instead of 1 and 0.
 * Used in printing.
 */
const char *bool_to_pass(int b)
{
    return b ? "OK" : "FAIL";
}

/**
 * This function creates a random array with length of "length"
 * we make use of the lib rand() function to generate a random array
 * and a for loop to add all random values to array.
 * 
 * On line 27 we use calloc which allocates a block of memory
 * for an array of n elements of size and initializes all bytes
 * in the allocated storage to zero. If allocation succeeds, 
 * returns a pointer to the lowest (first) byte
 * in the allocated memory block that is suitably aligned for any object type.
 */
int *create_random_array(int length, int min, int max)
{
    int *t = calloc(length, sizeof(int));
    for (int i = 0; i < length; i++)
    {
        t[i] = min + (rand() % (max + 1));
    }
    return t;
}

/**
 * Function clones specified array.
 * Returns the clone.
 */
int *clone_array(int* t, int length)
{
    int *newArray = calloc(length, sizeof(int));
    memcpy(newArray, t, length * sizeof(int));
    return newArray;
}

/**
 * Method swaps the values stored at the specified memory adressess.
 */
void swap(int *i, int *j)
{
    int k = *i;
    *i = *j;
    *j = k;
}

/*
* Function median3sort is an aid algorithm for the main quicksort algorithm.
* This function finds the median of the first, last and the middle number
* in the array and uses that as its partitional placement.
*
* The purpose is to ensure that a sorted array still has perfect partitioning
* while we increase the probability for a good partition in an unsorted array.
*/
int median3sort(int *t, int l, int r)
{
    int m = (l + r) / 2;
    if (t[l] > t[m]) swap(&t[l], &t[m]);
    if (t[m] > t[r])
    {
        swap(&t[m], &t[r]);
        if (t[l] > t[m]) swap(&t[l], &t[m]);
    }
    return m;
}

/**
 * Method splits specified table and returns a pivot position/index.
 * 
 * First it finds the median with median3sort. The element at this index
 * is then moved to the last index to not be in the way. 
 * The method runs a for loop until a break statement is called.
 * For each iteration it checks wheter the left indexed table value is 
 * smaller than the median table value. The left index increments until 
 * its tabel value is larger than the median tabel value.
 * Same applies for rightindexed only it begins at a big index and decremnts.
 * 
 * If the left and right indexes have not passed eachother then the table
 * values at theses indexes will swap place. 
 * 
 * When the indexes eventuelly passes eachother the loop breaks and the median
 * table value is moved back to its original position. 
 * 
 * returns the leftindex as pivot position
 */
int split(int *t, int l, int r)
{
    int il, ir;
    int m = median3sort(t, l, r);
    int sv = t[m];
    swap(&t[m], &t[r - 1]);
    for (il = l, ir = r - 1;;)
    {
        while (t[++il] < sv);
        while (t[--ir] > sv);
        if (il >= ir) break;
        swap(&t[il], &t[ir]);
    }
    swap(&t[il], &t[r - 1]);
    return il;
}

/*
* This is the implementation of the original quicksort
*
*
*/
void quicksort_impl(int *t, int l, int r)
{
    if (r - l > 2)
    {
        int pivot = split(t, l, r);
        quicksort_impl(t, l, pivot - 1);
        quicksort_impl(t, pivot + 1, r);
    }
    else
    {
        median3sort(t, l, r);
    }
}

void quicksort(int *t, int length)
{
    quicksort_impl(t, 0, length - 1);
}

/*
* This is the improved quicksort which adds a check
* for each pivot and ensures that we have a lower bound (l)
* greater that 0, that the higher bound (r) is less that the length - 1,
* and also that the value at the index of [l - 1]
* is equal to the value at the index of [t + 1]
*
*/
void quicksort_plus_impl(int *t, int length, int l, int r)
{
    if (l > 0 && r < length - 1 && t[l - 1] == t[r + 1]) return;

    if (r - l > 2)
    {
        int pivot = split(t, l, r);
        quicksort_plus_impl(t, length, l, pivot - 1);
        quicksort_plus_impl(t, length, pivot + 1, r);
    }
    else
    {
        median3sort(t, l, r);
    }
}

// This is just a simplified function signature to ensure 
// that noone forgets to use the lenght -1 parameter
void quicksort_plus(int *t, int length)
{
    quicksort_plus_impl(t, length, 0, length - 1);
}

//Check to ensure that the sum of the array is the same
int checksum(int *t, int length)
{
    int sum = 0;
    for (int i = 0; i < length; i++)
    {
        sum += t[i];
    }
    return sum;
}

// Checks that the element on index i
// is greater than the element on i + 1
int sanity_check(int *t, int length)
{
    for(int i = 0; i < length - 1; i++)
    {
        if(t[i] > t[i + 1])
        {
            return 0;
        }
    }
    return 1;
}

/*
*
*
*/
typedef void quicksort_func(int *t, int length);
/*
*
* Function to run the tests and to measure the time of each method.
* 
* The tests are checksum and sanity. 
*
* quicksort is run from the typedef quicksort_func with the sort param.
*
* 
*/
void run_sort_test(const char *test_name, quicksort_func sort, int *t, int length)
{
    printf("%s:\n", test_name);
    int chk = checksum(t, length);

    struct timespec start, end;
    if (clock_gettime(CLOCK_REALTIME, &start))
    {
        printf("time failure");
        return;
    }

    //Quicksort done from the t with random integer inputs on index from for-loop
    sort(t, length);

    if (clock_gettime(CLOCK_REALTIME, &end))
    {
        printf("time failure");
        return;
    }
    
    unsigned long long ns = (end.tv_sec - start.tv_sec) * 1000000000LL + (long long)(end.tv_nsec - start.tv_nsec);
    double ms = ns / 1000000.0;

    printf(" - Sanity check : %s\n", bool_to_pass(sanity_check(t, length)));
    printf(" - Checksum     : %s\n", bool_to_pass(checksum(t, length) == chk));
    printf(" - Time         : %08lluns (%4.01fms)\n", ns, ms);
}
/*
* 
* 
* 
* 
* 
* 
* 
*/
void print_results(const char *title, int length, int upper_range)
{
    int *array_a, *array_b;
    
    printf("\n### %s (n=%d) ###\n", title, length);
    array_a = create_random_array(length, 0, upper_range);
    array_b = clone_array(array_a, length);

    run_sort_test("Quicksort", quicksort, array_a, length);
    run_sort_test("Quicksort (sorted)", quicksort, array_a, length);

    run_sort_test("Quicksort+", quicksort_plus, array_b, length);
    run_sort_test("Quicksort+ (sorted)", quicksort_plus, array_b, length);

    free(array_a);
    free(array_b);
}

int main()
{
    const int n_min = 100000, n_max = 10000000;
    const int small_range = 10, big_range = 10000;
    
    for (int i = n_min; i <= n_max; i *= 10)
    {
        print_results("Sparse numbers", i, big_range);
        print_results("Close numbers", i, small_range);
    }
}

