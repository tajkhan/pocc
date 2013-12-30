/*
 * Temporarily defining some constants to make PoCC compile
 *
 *
 *
 */
#ifndef DUMMY_DEFINES_H
# define DUMMY_DEFINES_H

/* LetSee constants*/

//from letsee/options.h
# define LS_TYPE_FS	8
# define LS_TYPE_MULTI  5

# define LS_HEURISTIC_EXHAUST  1
# define LS_HEURISTIC_M1       9
# define LS_HEURISTIC_SKIP    10
# define LS_HEURISTIC_RANDOM  17
# define LS_HEURISTIC_DH      18

# define LS_HEURISTIC_M1_SCHEME_ITER		1
# define LS_HEURISTIC_M1_SCHEME_ITERPARAM	2
# define LS_HEURISTIC_M1_SCHEME_FULL		3
# define LS_HEURISTIC_M1_SCHEME_NONE		4

# define LS_HEURISTIC_MAX_SCHEME_SIZE		64


/* PLUTO constants*/

//from pluto/libpluto.h
#define SMART_FUSE 2



/* PONOS constants*/

//from ponos/solver.h
# define PONOS_SOLVER_PIP 1

//from ponos/objectives.h
# define PONOS_OBJECTIVES_NONE		0
# define PONOS_OBJECTIVES_CODELET	1
# define PONOS_OBJECTIVES_PLUTO		2
# define PONOS_OBJECTIVES_CUSTOM       	3

//from ponos/options.h
# define PONOS_CONSTRAINTS_SUM_ITER_POS		0
# define PONOS_CONSTRAINTS_PARAM_COEF_ZERO	1
# define PONOS_OBJECTIVES_MAX_OUTER_PAR		2
# define PONOS_OBJECTIVES_MAX_INNER_PAR		3
# define PONOS_OBJECTIVES_MAX_PERMUTABILITY	4
# define PONOS_OBJECTIVES_MIN_DEP_DISTANCE	5
# define PONOS_OBJECTIVES_MAX_DEP_SOLVE		6
# define PONOS_CONSTRAINTS_LINEAR_INDEP		7
# define PONOS_OBJECTIVES_GAMMA_POS		8

#endif // DUMMY_DEFINES_H
