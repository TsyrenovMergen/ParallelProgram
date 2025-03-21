#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <unistd.h>

double cpuSecond()
{
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return ((double)ts.tv_sec + (double)ts.tv_nsec * 1.e-9);
}

int main()
{
    int n=40000;
    int m=40000;
    double time_1, boost;
    double start, end, dif;
    double* matrix=(double*)malloc(sizeof(double)*m*n);
    double* vector=(double*)malloc(sizeof(double)*m);
    double* result=(double*)malloc(sizeof(double)*n);
    int count_proc[7]={1, 2, 4, 8, 16, 20, 40};
    int count=5;
    for (int i=0; i<7; i++)
    {
        omp_set_num_threads(count_proc[i]);
        dif=0;
        for (int j=0; j<count; j++)
        {
            for (int d=0; d<n*m; d++)
            {
                matrix[d]=1;
            }
            for (int d=0; d<m; d++)
            {
                vector[d]=1;
            }
            start=cpuSecond();
            #pragma omp parallel 
            {
                int nthreads = omp_get_num_threads();
                int threadid = omp_get_thread_num();
                int items_per_thread = n / nthreads;
                int a = threadid * items_per_thread;
                int b = (threadid == nthreads - 1) ? n : (a + items_per_thread);
                for (int i=a; i<b; i++)
                {
                    result[i]=0;
                    for (int j=0; j<m; j++)
                    {
                        result[i]+=matrix[i*n+j]*vector[j];
                    }
                }
            }
            end=cpuSecond();   
            dif+=end-start;
        }
        dif=dif/count;
        printf("number[%d] = %lf\n", i, dif);
        if (i==0)
        {
            time_1=dif;
        }
        boost=time_1/dif;
        printf("boost[%d] = %lf\n", i, boost);
    }
    free(matrix);
    free(vector);
    free(result);
    return 0;
}
