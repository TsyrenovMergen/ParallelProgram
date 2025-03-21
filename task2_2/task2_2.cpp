#include <iostream>
#include <omp.h>
#include <chrono>
#include <math.h>

double func(double x)
{
    return exp(-x * x);
}
using namespace std;
int main(int argc, char **argv)
{
    double start, end, time_1, dif;
    const double a = -4.0;
    const double b = 4.0;
    const int n = 10000000;
    cout<<"Numerical integration: "<<a<<" "<<b<<" "<< " "<<n<<endl;
    double h = (b - a) / n;
    double s;
    double sumloc[omp_get_max_threads()];
    int count_proc[7]={1, 2, 4, 8, 16, 20, 40};
    int count=10;
    for (int i=0; i<7; i++)
    {
        omp_set_num_threads(count_proc[i]);
        start= omp_get_wtime();
        for (int j=0; j<count; j++)
        {
            s=0.0;
            #pragma omp parallel
            {
                int nthreads = omp_get_num_threads();
                int tid = omp_get_thread_num();
                int points_per_thread = n / nthreads;
                int lo = tid * points_per_thread;
                int hi = (tid == nthreads - 1) ? (n-1) : lo + points_per_thread;
                sumloc[tid] = 0.0;
                for (int i = lo; i <= hi; i++)
                sumloc[tid] += func(a + h * (i + 0.5));
                #pragma omp atomic
                s += sumloc[tid];
            }
            s *= h;
        }
        end =omp_get_wtime();
        dif=(end-start)/count;
        if (i==0)
        {
            time_1=dif;
        }
        cout<<"number["<<i<<"] = "<<dif<<endl;
        cout<<"boost["<<i<<"] = "<<time_1/dif<<endl;
    }
    return 0;
}