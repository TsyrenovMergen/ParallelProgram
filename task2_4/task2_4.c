#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <stdlib.h>

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
    double e=0.6;
    double norm1, norm2, criter;
    double summa, summa2;
    double start, end, dif;
    int count=10;
    int k, flag, flag3;
    int nproc[7]={1, 2, 4, 8, 16, 20, 40};
    for (int i=0; i<7; i++)
    {
        omp_set_num_threads(nproc[i]);
        dif=0;
        for (int j=0; j<count; j++)
        {
            summa=0;
            summa2=0;
            k=0;
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
            flag=0;
            flag3=0;
            #pragma omp parallel
            {
                double sum=0, sum2=0;
                int nthreads = omp_get_num_threads();
                int threadid = omp_get_thread_num();
                int items_per_thread = n / nthreads;
                int a = threadid * items_per_thread;
                int b = (threadid == nthreads - 1) ? n : (a + items_per_thread);
                while(flag3==0)
                {
                    if (flag>0)
                    {
                        continue;
                    }
                    sum=0;
                    sum2=0;
                    for (int d=a; d<b; d++)
                    {
                        mul_a_x[d]=0;
                        for (int c=0; c<n; c++)
                        {
                            mul_a_x[d]+=matrix[d*n+c]*result[c];
                        }
                        mul_a_x[d]=mul_a_x[d]-v[d];
                        next_result[d]=result[d]-r*mul_a_x[d];
                        sum+=mul_a_x[d]*mul_a_x[d];
                        sum2+=v[d]*v[d];
                    }
                    for (int i=a; i<b; i++)
                    {
                        result[i]=next_result[i];
                    }
                    #pragma omp atomic
                    summa+=sum;
                    #pragma omp atomic
                    summa2+=sum2;
                    #pragma omp atomic
                    flag+=1;
                    if (flag==nthreads)
                    {
                        flag=0;
                        norm1=sqrt(summa);
                        norm2=sqrt(summa2);
                        if (norm1/norm2<e)
                        {
                            flag3=1;
                        }
                    }
                }
                /*
                criter=norm1/norm2;
                printf("%lf\n", criter);
                */
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
    free(matrix);
    free(v);
    free(mul_a_x);
    free(result);
    free(next_result);
    return 0;
}