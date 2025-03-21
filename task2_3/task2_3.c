#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <stdlib.h>

void dgemv(int n, int m, double* matrix, double* v, double* result)
{
    #pragma omp parallel 
    {
        int nthreads = omp_get_num_threads();
        int threadid = omp_get_thread_num();
        int items_per_thread = m / nthreads;
        int a = threadid * items_per_thread;
        int b = (threadid == nthreads - 1) ? n : (a + items_per_thread);
        for (int i=a; i<b; i++)
        {
            result[i]=0;
            for (int j=0; j<m; j++)
            {
                result[i]+=matrix[i*n+j]*v[j];
            }
        }
    }
    return;
}
double norm(int n, double* v)
{
    double summa=0;
    #pragma omp parallel 
    {
        double sum=0;
        int nthreads = omp_get_num_threads();
        int threadid = omp_get_thread_num();
        int items_per_thread = n / nthreads;
        int a = threadid * items_per_thread;
        int b = (threadid == nthreads - 1) ? n : (a + items_per_thread);
        for (int i=a; i<b; i++)
        {
            sum+=v[i]*v[i];
        }
        #pragma omp atomic
        summa+=sum;
    }
    summa=sqrt(summa);
    return summa;
}
double cpuSecond()
{
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return ((double)ts.tv_sec + (double)ts.tv_nsec * 1.e-9);
}
int main()
{
    int n=20000;
    double* matrix=(double*)malloc(sizeof(double)*n*n);
    double* v=(double*)malloc(sizeof(double)*n);
    double* result=(double*)malloc(sizeof(double)*n);
    double* next_result=(double*)malloc(sizeof(double)*n);
    double* mul_a_x=(double*)malloc(sizeof(double)*n);
    double time_1, boost;
    double r=0.01;
    if (n==20000)
    {
        r=0.00005;
    }
    if (n==40000)
    {
        r=0.000025;
    }
    double e=0.0000005;
    double norm1, norm2, criter;
    double start, end, dif;
    int count=1;
    int nproc[7]={1, 2, 4, 8, 16, 20, 40};
    for (int i=0; i<1; i++)
    {
        dif=0;
        omp_set_num_threads(nproc[i]);
        for (int j=0; j<count; j++)
        {
            for (int d=0; d<n*n; d++)
            {
                if (d % n==0)
                {
                    matrix[d]=2;
                }
                else
                {
                    matrix[d]=1;
                }
            }
            for (int d=0; d<n; d++)
            {
                v[d]=n+1;
                mul_a_x[d]=0;
                result[d]=0;
                next_result[d]=0;
            }
            start=cpuSecond();
            while(1)
            {
                dgemv(n, n, matrix, result, mul_a_x);
                for (int d=0; d<n; d++)
                {
                    mul_a_x[d]=mul_a_x[d]-v[d];
                    next_result[d]=result[d]-r*mul_a_x[d];
                }
                norm1=norm(n, mul_a_x);
                norm2=norm(n, v);
                if (norm1/norm2<e)
                {
                    break;
                }
                /*
                criter=norm1/norm2;
                printf("%lf\n", criter);
                */
                for (int d=0; d<n; d++)
                {
                    result[d]=next_result[d];
                }
            }
            end=cpuSecond();
            dif+=end-start;
        }
        dif=dif/count;
        if (i==0)
        {
            time_1=dif;
        }
        boost=time_1/dif;
        printf("number[%d] = %lf\n", i, dif);
        printf("boost[%d] =%lf\n", i, boost);
    }
    /*
    FILE* file=fopen("result.txt", "wt");
    if(file==NULL)
    {
        perror("Error occured while opening data28.txt");
        return 1;
    }
    for (int i=0; i<n; i++)
    {
        fprintf(file, "%lf ", result[i]);
    }
    fclose(file);
    */
    free(matrix);
    free(v);
    free(mul_a_x);
    free(result);
    free(next_result);
    return 0;
}