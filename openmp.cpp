# include <stdlib.h>
# include <stdio.h>
# include <omp.h>
# include <iostream>

int main(int argc, char* argv[]);
void prime_number_sweep(int n_lo, int n_hi, int n_factor);
int prime_number(int n);


int main(int argc, char* argv[])


{
    int n_factor;
    int n_hi;
    int n_lo;


    printf("\n");
    printf("  Number of processors available = %d\n", omp_get_num_procs());
    printf("  Number of threads =              %d\n", omp_get_max_threads());

    n_lo = 10000000;
    n_hi = 50000000;
    n_factor = 2;

    prime_number_sweep(n_lo, n_hi, n_factor);

    printf("\nEnd");
    

    return 0;
}

void prime_number_sweep(int n_lo, int n_hi, int n_factor)
{
    int n;
    int primes;
    double wtime;

    printf("         N        Pi          Time\n");
    printf("\n");

    n = n_lo;

    while (n <= n_hi)
    {
        wtime = omp_get_wtime();

        primes = prime_number(n);

        wtime = omp_get_wtime() - wtime;

        printf("  %8d  %8d  %14f\n", n, primes, wtime);

        n = n * n_factor;
    }

    return;
}

int prime_number(int n)

{
    int i;
    int j;
    int prime;
    int total = 0;

# pragma omp parallel \
  shared ( n ) \
  private ( i, j, prime )


# pragma omp for reduction ( + : total )
    for (i = 2; i <= n; i++)
    {
        prime = 1;

        for (j = 2; j * j <= i; j++)
        {
            if (i % j == 0)
            {
                prime = 0;
                break;
            }
        }
        total = total + prime;
    }

    return total;
}