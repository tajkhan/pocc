#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/time.h>

#define N 2048

double alpha = 1.54645764567;
double beta = 1.768679678;

double A[N][N];
double B[N][N];
double C[N][N];




#ifdef TIME
#define IF_TIME(foo) foo;
#else
#define IF_TIME(foo)
#endif

void init_array()
{
    int i, j;

    for (i=0; i<N; i++) {
        for (j=0; j<N; j++) {
            A[i][j] = ((double) i*j)/N;
            B[i][j] = ((double) i*j)/N;
        }
    }
}


void print_array(char** argv)
{
    int i, j;
    if (! strcmp(argv[0], ""))
      {
	for (i=0; i<N; i++) {
	  for (j=0; j<N; j++) {
	    fprintf(stderr, "%0.2lf ", C[i][j]);
	  if (i%80 == 20) fprintf(stderr, "\n");
	  }
	}
	fprintf(stderr, "\n");
      }
}


double rtclock()
{
    struct timezone Tzp;
    struct timeval Tp;
    int stat;
    stat = gettimeofday (&Tp, &Tzp);
    if (stat != 0) printf("Error return from gettimeofday: %d",stat);
    return(Tp.tv_sec + Tp.tv_usec*1.0e-6);
}

#ifdef TIME
#define IF_TIME(foo) foo;
#else
#define IF_TIME(foo)
#endif





int main(int argc, char** argv)
{
    double t_start, t_end;
    int i, j, k;

    init_array();

    IF_TIME(t_start = rtclock());

#define ceild(n,d)  ceil(((double)(n))/((double)(d)))
#define floord(n,d) floor(((double)(n))/((double)(d)))
#define max(x,y)    ((x) > (y)? (x) : (y))
#define min(x,y)    ((x) < (y)? (x) : (y))





  register int lbv, ubv, lb, ub, lb1, ub1, lb2, ub2;
  register int c0, c0t, newlb_c0, newub_c0, c1, c1t, newlb_c1, newub_c1, c2, c2t, newlb_c2, newub_c2, c3, c3t, newlb_c3, newub_c3, c4, c4t, newlb_c4, newub_c4, c5, c5t, newlb_c5, newub_c5, c6, c6t, newlb_c6, newub_c6;

/* Generated from PLUTO-produced CLooG file by CLooG 0.14.0-UNKNOWN 64 bits in 0.07s. */
#pragma scop
if (N >= 1) {
  for (c0=0;c0<=floord(N-1,32);c0++) {
    for (c1=0;c1<=floord(N-1,32);c1++) {
      for (c2=0;c2<=floord(N-1,32);c2++) {
        if ((N >= 2) && (c2 <= 0) && (c2 >= 0)) {
          for (c3=max(0,32*c0);c3<=min(N-1,32*c0+31);c3++) {
            for (c4=max(0,32*c1);c4<=min(N-1,32*c1+31);c4++) {
              C[c3][c4]=C[c3][c4]*alpha;;
              C[c3][c4]+=beta*A[c3][0]*B[0][c4];;
              for (c5=1;c5<=min(N-1,32*c2+31);c5++) {
                C[c3][c4]+=beta*A[c3][c5]*B[c5][c4];;
              }
            }
          }
        }
        if (N == 1) {
          C[0][0]=C[0][0]*alpha;;
          C[0][0]+=beta*A[0][0]*B[0][0];;
        }
        if (c2 >= 1) {
          for (c3=max(0,32*c0);c3<=min(N-1,32*c0+31);c3++) {
            for (c4=max(0,32*c1);c4<=min(N-1,32*c1+31);c4++) {
              for (c5=32*c2;c5<=min(N-1,32*c2+31);c5++) {
                C[c3][c4]+=beta*A[c3][c5]*B[c5][c4];;
              }
            }
          }
        }
      }
    }
  }
}
#pragma endscop

    IF_TIME(t_end = rtclock());
    IF_TIME(printf("%0.6lfs\n", t_end - t_start));


    print_array(argv);


    return 0;
}
