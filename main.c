#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread_time.h>

void bytt(int *j, int *i)
{
    int k=*j;
    *j=*i;
    *i=k;
}

int median3sort(int* t, int v, int h)
{
    int m = (v+h)/2;
    if(t[v]>t[m]) bytt(&t[v],&t[m]);
    if(t[m]>t[h])
    {
        bytt(&t[m], &t[h]);
        if(t[v]>t[m]) bytt(&t[v], &t[m]);
    }
    return m;
}

int splitt(int *t, int v, int h)
{
    int iv, ih;
    int m = median3sort(t,v,h);
    int dv = t[m];
    bytt(&t[m], &t[h-1]);
    for( iv=v, ih=h-1;;)
    {
        while (t[++iv]<dv);
        while (t[--ih]>dv);
        if(iv>=ih)break;
        bytt(&t[iv], &t[ih]);
    }
    bytt(&t[iv], &t[h-1]);
    return iv;
}

void quicksort(int *t, int v, int h)
{
    if(h-v>2)
    {
        int delepos = splitt(t,v,h);
        quicksort(t, v, delepos-1);
        quicksort(t, delepos+1, h);
    }
    else
    {
    median3sort(t,v,h);
    }
}

int main()
{
    int length = 10000;
    int array[10000] = {};


    //Filling the array with random integers and printing vaules
    for(int i = 0; i<length; i++)
    {
        array[i] = rand() %100; //random numbers in range 80
        //printf("lol");
        printf("%d  ", array[i]);
    }

    struct timespec start,end;

    if(clock_gettime(CLOCK_REALTIME, &start))
    {
        printf("time failure");
        return -1;
    }
    //Quicksort done from the array with random integer inputs on index from for-loop
    quicksort(array, 0, length-1);

    if(clock_gettime(CLOCK_REALTIME, &end))
    {
        printf("time failure");
        return -1;
    }

    printf("sorted:\n");
    //Printing sorted list
    for(int i = 0; i<length; i++)
    {
        printf("%d \n", array[i]);
    }


    double ms = (end.tv_sec-start.tv_sec)*1000 + (end.tv_nsec - start.tv_nsec)/1000000;
    double ns= (end.tv_nsec - start.tv_nsec);
    printf("Time: %4.01f", ms);
}
