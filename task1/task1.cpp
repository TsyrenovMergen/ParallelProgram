#include <iostream>
#include <time.h>
#include <math.h>

#define n 10000000
#define M_PI 3.14159265358979323846
#define flag 0

#ifdef Use_float
#   define flag 1
#endif
using namespace std;
template <typename type>
type sum()
{
    type result = 0;
    double value = 0;
    double step = 2 * M_PI / (double)n;
    clock_t start = clock();
    for (int i = 0; i < n; i++)
    {
        result += sin(value);
        value += step;
    }
    clock_t end = clock();
    float seconds = (float)(end - start) / CLOCKS_PER_SEC;
    cout << "Your calculations took " << seconds << endl;
    return result;
}

int main()
{
    if (flag == 0)
    {
        cout << sum<double>();
    }
    else
    {
        cout << sum<float>();
    }
    return 0;
}