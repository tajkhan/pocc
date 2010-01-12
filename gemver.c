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
      {
	for (i=0; i<N; i++) {
	  fprintf(stderr, "%0.2lf ", w[i]);
	  if (i%80 == 20) fprintf(stderr, "\n");
	}
	fprintf(stderr, "\n");
      }
}



int
main(int argc, char** argv)
{
    int i, j;

    init_array();

#pragma scop
    for (i=0; i<N; i++)
        for (j=0; j<N; j++)
            A[i][j] = A[i][j] + u1[i]*v1[j] + u2[i]*v2[j];

    for (i=0; i<N; i++)
        for (j=0; j<N; j++)
            x[i] = x[i] + A[j][i]*y[j];

    for (i=0; i<N; i++)
        x[i] = x[i] + z[i];

    for (i=0; i<N; i++)
        for (j=0; j<N; j++)
            w[i] = w[i] +  A[i][j]*x[j];
#pragma endscop

    print_array(argv);

    return 0;
}
