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
  register int c1, c2, c4, c5, c3, c6;

#pragma scop
if ((N >= 1)) {
  for (c1 = 0; c1 <= floord((N + -1), 32); c1++) {
    for (c2 = 0; c2 <= floord((N + -1), 32); c2++) {
      for (c4 = (32 * c1); c4 <= min((N + -1), ((32 * c1) + 31)); c4++) {
        for (c5 = (32 * c2); c5 <= min((N + -1), ((32 * c2) + 31)); c5++) {
          C[c4][c5]=C[c4][c5]*alpha;
        }
      }
    }
  }
  for (c1 = 0; c1 <= floord((N + -1), 32); c1++) {
    for (c2 = 0; c2 <= floord((N + -1), 32); c2++) {
      for (c3 = 0; c3 <= floord((N + -1), 32); c3++) {
        for (c4 = (32 * c1); c4 <= min((N + -1), ((32 * c1) + 31)); c4++) {
          for (c5 = (32 * c2); c5 <= min((N + -1), ((32 * c2) + 31)); c5++) {
            for (c6 = (32 * c3); c6 <= min((N + -1), ((32 * c3) + 31)); c6++) {
              C[c4][c5]+=beta*A[c4][c6]*B[c6][c5];
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
