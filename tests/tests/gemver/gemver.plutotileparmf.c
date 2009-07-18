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

#define ceild(n,d)  ceil(((double)(n))/((double)(d)))
#define floord(n,d) floor(((double)(n))/((double)(d)))
#define max(x,y)    ((x) > (y)? (x) : (y))
#define min(x,y)    ((x) < (y)? (x) : (y))







  register int lbv, ubv, lb, ub, lb1, ub1, lb2, ub2;
  register int c0, c0t, newlb_c0, newub_c0, c1, c1t, newlb_c1, newub_c1, c2, c2t, newlb_c2, newub_c2, c3, c3t, newlb_c3, newub_c3, c4, c4t, newlb_c4, newub_c4, c5, c5t, newlb_c5, newub_c5, c6, c6t, newlb_c6, newub_c6;

/* Generated from PLUTO-produced CLooG file by CLooG 0.14.0-UNKNOWN 64 bits in 0.02s. */
#pragma scop
if (N >= 1) {
  for (c0=0;c0<=floord(N+63,32);c0++) {
    if (c0 <= floord(N-1,32)) {
      for (c2=0;c2<=floord(N-1,32);c2++) {
        for (c3=max(0,32*c0);c3<=min(N-1,32*c0+31);c3++) {
          for (c5=max(0,32*c2);c5<=min(N-1,32*c2+31);c5++) {
            A[c5][c3]=A[c5][c3]+u1[c5]*v1[c3]+u2[c5]*v2[c3];;
            x[c3]=x[c3]+A[c5][c3]*y[c5];;
          }
        }
      }
    }
    if ((c0 <= floord(N+31,32)) && (c0 >= 1)) {
      for (c3=max(0,32*c0-32);c3<=min(N-1,32*c0-1);c3++) {
        x[c3]=x[c3]+z[c3];;
      }
    }
    if (c0 >= 2) {
      for (c2=0;c2<=floord(N-1,32);c2++) {
        for (c3=max(0,32*c0-64);c3<=min(N-1,32*c0-33);c3++) {
          for (c5=max(0,32*c2);c5<=min(N-1,32*c2+31);c5++) {
            w[c5]=w[c5]+A[c5][c3]*x[c3];;
          }
        }
      }
    }
  }
}
#pragma endscop

    print_array(argv);

    return 0;
}
