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
  register int c1, c3, c5;

#pragma scop
if ((N >= 1)) {
  for (c1 = 0; c1 <= (N + -1); c1++) {
    for (c3 = 0; c3 <= (N + -1); c3++) {
      C[c1][c3]=C[c1][c3]*alpha;
      for (c5 = 0; c5 <= (N + -1); c5++) {
        C[c1][c3]+=beta*A[c1][c5]*B[c5][c3];
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
