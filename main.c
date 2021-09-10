#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>

const char *bool_to_str(int b)
{
    return b ? "True" : "False";
}

void bytt(int *j, int *i)
{
    int k = *j;
    *j = *i;
    *i = k;
}

int median3sort(int *t, int v, int h)
{
    int m = (v + h) / 2;
    if (t[v] > t[m])
        bytt(&t[v], &t[m]);
    if (t[m] > t[h])
    {
        bytt(&t[m], &t[h]);
        if (t[v] > t[m])
            bytt(&t[v], &t[m]);
    }
    return m;
}

int splitt(int *t, int v, int h)
{
    int iv, ih;
    int m = median3sort(t, v, h);
    int dv = t[m];
    bytt(&t[m], &t[h - 1]);
    for (iv = v, ih = h - 1;;)
    {
        while (t[++iv] < dv);
        while (t[--ih] > dv);
        if (iv >= ih) break;
        bytt(&t[iv], &t[ih]);
    }
    bytt(&t[iv], &t[h - 1]);
    return iv;
}

void quicksort_impl(int *t, int v, int h)
{
    if (h - v > 2)
    {
        int delepos = splitt(t, v, h);
        quicksort_impl(t, v, delepos - 1);
        quicksort_impl(t, delepos + 1, h);
    }
    else
    {
        median3sort(t, v, h);
    }
}

void quicksort(int *array, int length)
{
    quicksort_impl(array, 0, length - 1);
}

void quicksort_plus_impl(int *t, int l, int v, int h)
{
    if (v > 0 && h < l - 1 && t[v - 1] == t[h + 1]) return;

    if (h - v > 2)
    {
        int delepos = splitt(t, v, h);
        quicksort_plus_impl(t, l, v, delepos - 1);
        quicksort_plus_impl(t, l, delepos + 1, h);
    }
    else
    {
        median3sort(t, v, h);
    }
}

void quicksort_plus(int *array, int length)
{
    quicksort_plus_impl(array, length, 0, length - 1);
}

int *create_random_array(int length, int min, int max)
{
    int *array = calloc(length, sizeof(int));
    for (int i = 0; i < length; i++)
    {
        array[i] = min + (rand() % (max + 1));
    }
    return array;
}

int *clone_array(int* array, int length)
{
    int *newArray = calloc(length, sizeof(int));
    memcpy(newArray, array, length * sizeof(int));
    return newArray;
}

int checksum(int *array, int length)
{
    int chkSum = 0;
    int chkXor = 0;
    for (int i = 0; i < length; i++)
    {
        chkSum += array[i];
        chkXor ^= array[i];
    }
    return chkSum ^ chkXor;
}

int sanity_check(int *t, int l)
{
    for (int i = 1; i < l - 1; i++)
    {
        if (t[i - 1] > t[i])
        {
            return 0;
        }
    }
    return 1;
}

typedef void quicksort_func(int *t, int l);
void run_sort_test(const char *test_name, quicksort_func sort, int *t, int l)
{
    printf("### %s ###\n", test_name);
    int chk = checksum(t, l);

    struct timespec start, end;
    if (clock_gettime(CLOCK_REALTIME, &start))
    {
        printf("time failure");
        return;
    }

    //Quicksort done from the array with random integer inputs on index from for-loop
    sort(t, l);

    if (clock_gettime(CLOCK_REALTIME, &end))
    {
        printf("time failure");
        return;
    }
    
    unsigned long long ns = (end.tv_sec - start.tv_sec) * 1000000000LL + (long long)(end.tv_nsec - start.tv_nsec);
    double ms = ns / 1000000.0;

    printf("   Array sanity check passed : %s\n", bool_to_str(sanity_check(t, l)));
    printf("   Array checksum passed     : %s\n", bool_to_str(checksum(t, l) == chk));
    printf("   Time: %08lluns (%4.01fms)\n", ns, ms);
}

int main()
{
    const int length = 1000000;
    int *array_a, *array_b;

    printf("||| Close numbers |||\n");
    array_a = create_random_array(length, 0, 10);
    array_b = clone_array(array_a, length);

    run_sort_test("Quicksort: ", quicksort, array_a, length);
    run_sort_test("Resorting array: ", quicksort, array_a, length);

    run_sort_test("Quicksort+: ", quicksort_plus, array_b, length);
    run_sort_test("Resorting array: ", quicksort_plus, array_b, length);

    free(array_a);
    free(array_b);

    printf("\n||| Sparse numbers |||\n");
    array_a = create_random_array(length, 0, 4000000);
    array_b = clone_array(array_a, length);

    run_sort_test("Quicksort: ", quicksort, array_a, length);
    run_sort_test("Resorting array: ", quicksort, array_a, length);

    run_sort_test("Quicksort+: ", quicksort_plus, array_b, length);
    run_sort_test("Resorting array: ", quicksort_plus, array_b, length);

    free(array_a);
    free(array_b);
}
