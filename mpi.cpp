#define _CRT_SECURE_NO_WARNINGS

# include <math.h>
# include <mpi.h>
# include <stdio.h>
# include <stdlib.h>
# include <time.h>

int main(int argc, char* argv[]);
int prime_number(int n, int id, int p);

int main(int argc, char* argv[])
{
    int id;
    int ierr;
    int n;
    int n_factor;
    int n_hi;
    int n_lo;
    int p;
    int primes;
    int primes_part;
    double wtime;

    n_lo = 10000000;
    n_hi = 50000000;
    n_factor = 2;
    
    ierr = MPI_Init(&argc, &argv);

    ierr = MPI_Comm_size(MPI_COMM_WORLD, &p);
 
    ierr = MPI_Comm_rank(MPI_COMM_WORLD, &id);

    if (id == 0)
    {
        printf("\n");
        printf("  The number of processes is %d\n", p);
        printf("\n");
        printf("         N        Pi          Time\n");
        printf("\n");
    }

    n = n_lo;

    while (n <= n_hi)
    {
        if (id == 0)
        {
            wtime = MPI_Wtime();
        }
        ierr = MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

        primes_part = prime_number(n, id, p);

        ierr = MPI_Reduce(&primes_part, &primes, 1, MPI_INT, MPI_SUM, 0,
            MPI_COMM_WORLD);

        if (id == 0)
        {
            wtime = MPI_Wtime() - wtime;
            printf("  %8d  %8d  %14f\n", n, primes, wtime);
        }
        n = n * n_factor;
    }

    ierr = MPI_Finalize();
 
    if (id == 0)
    {
        fflush(stdout);
        printf("\n");
        printf("DONE.\n");
        printf("\n");
        fflush(stdout);
    }

    return 0;
}

int prime_number(int n, int id, int p)

{
    int i;
    int j;
    int prime;
    int total;

    total = 0;

    for (i = 2 + id; i <= n; i = i + p)
    {
        prime = 1;
        for (j = 2; j * j <= i; j++)
        {
            if ((i % j) == 0)
            {
                prime = 0;
                break;
            }
        }
        total = total + prime;
    }
    return total;
}




