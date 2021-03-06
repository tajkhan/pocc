#include <omp.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>

#define N 4000

#define alpha 1
#define beta 1

double A[N][N];
double B[N][N];

double x[N];
double u1[N];
double u2[N];
double v2[N];
double v1[N];
double w[N];
double y[N];
double z[N];

void init_array()
{
    int i, j;

    for (i=0; i<N; i++) {
        u1[i] = i;
        u2[i] = (i+1)/N/2.0;
        v1[i] = (i+1)/N/4.0;
        v2[i] = (i+1)/N/6.0;
        y[i] = (i+1)/N/8.0;
        z[i] = (i+1)/N/9.0;
        x[i] = 0.0;
        w[i] = 0.0;
        for (j=0; j<N; j++) {
            A[i][j] = ((double) i*j)/N;
        }
    }
}

void print_array(char** argv)
{
    int i, j;
#ifndef TEST
    if (! strcmp (argv[0], ""))
#endif
    for (i=0; i<N; i++) {
        fprintf(stderr, "%0.2lf ", w[i]);
        if (i%80 == 20) fprintf(stderr, "\n");
    }
    fprintf(stderr, "\n");
}



int
main(int argc, char** argv)
{
    int i, j;

    init_array();

#ifdef ceild
# undef ceild
#endif
#ifdef floord
# undef floord
#endif
#ifdef max
# undef max
#endif
#ifdef min
# undef min
#endif
#define ceild(n,d)  ceil(((double)(n))/((double)(d)))
#define floord(n,d) floor(((double)(n))/((double)(d)))
#define max(x,y)    ((x) > (y)? (x) : (y))
#define min(x,y)    ((x) < (y)? (x) : (y))




  register int lbv, ubv, lb, ub, lb1, ub1, lb2, ub2;
  register int c1, c2, c3, c4;

#pragma scop
if ((N >= 1)) {
  for (c1 = 0; c1 <= floord((N + -1), 32); c1++) {
    for (c2 = 0; c2 <= floord((N + -1), 32); c2++) {
      for (c3 = (32 * c1); c3 <= min((N + -1), ((32 * c1) + 31)); c3++) {
        for (c4 = (32 * c2); c4 <= min((N + -1), ((32 * c2) + 31)); c4++) {
          A[c4][c3]=A[c4][c3]+u1[c4]*v1[c3]+u2[c4]*v2[c3];
          x[c3]=x[c3]+A[c4][c3]*y[c4];
        }
      }
    }
  }
  for (c1 = 0; c1 <= floord((N + -1), 32); c1++) {
    for (c3 = (32 * c1); c3 <= min((N + -1), ((32 * c1) + 31)); c3++) {
      x[c3]=x[c3]+z[c3];
    }
  }
  for (c1 = 0; c1 <= floord((N + -1), 32); c1++) {
    for (c2 = 0; c2 <= floord((N + -1), 32); c2++) {
      for (c3 = (32 * c1); c3 <= min((N + -1), ((32 * c1) + 31)); c3++) {
        for (c4 = (32 * c2); c4 <= min((N + -1), ((32 * c2) + 31)); c4++) {
          w[c3]=w[c3]+A[c3][c4]*x[c4];
        }
      }
    }
  }
}
#pragma endscop

    print_array(argv);

    return 0;
}
