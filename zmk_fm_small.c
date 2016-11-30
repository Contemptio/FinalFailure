#ifndef ZMK_FM_SMALL_C
#define ZMK_FM_SMALL_C

/*
#include "eqn.c"
#include "util.c"
*/
#include <float.h>
#include <stdio.h>
#include <stdlib.h>


/*
 *  The number of equations.
 */
#define LEN (5)

/*
 *  Type macros for easily changing.
 */
#define EQN_T float
#define INT_T unsigned short
/*
 *  Position macros for coefficients and the 'lt-flag.'
 */
 #define A_POS  (0)
 #define B_POS  (1)
 #define C_POS  (2)
 #define LT_POS (3)

/*
 *  Using a double matrix to represent equations:
 *  The matrix has n rows (number of equations f(x, y)) of 
 *  4 columns (the coefficients of x, y, and f(x, y) resp.,
 *  and a flag for greater than (zero) or less than (non-zero)).
 */

/**
 *  Sets the equivalence 'eqn <= eqns[i]' for i in 0 to nPos. The result is
 *  stored in eqns.
 *
 *  @param eqns
 *          The equations to which to add eqn.
 *  @param eqn
 *          The index of the equation which to add to the rest.
 */
void add_eqn_to_set_small(EQN_T **eqns, INT_T eqn)
{
    INT_T i;
    for (i = 0; i < LEN; ++i)
    {
        if (!eqns[i][LT_POS])
        {
            continue;
        }
        eqns[i][A_POS] = eqns[i][A_POS] - eqns[eqn][A_POS];
        eqns[i][B_POS] = eqns[i][B_POS] - eqns[eqn][B_POS];
        eqns[i][C_POS] = eqns[i][C_POS] - eqns[eqn][C_POS];
    }
}
 
 /**
 *  Finds the 'minimum' equation in the set, i.e. the equation with the lowest
 *  coefficient of b.
 *
 *  @param eqns
 *          The equations among which to search.
 *  @return
 *          the index of the equation with the lowest coefficient of b.
 */
INT_T minimum_equation_small(EQN_T** eqns)
{
    INT_T min = 0;
    INT_T i;
    for (i = 0; i < LEN - 1; ++i)
    {
        
        if (eqns[i][LT_POS]) {
            if (!eqns[i + 1][LT_POS]) {
                min = i + 1;
            }
        } else if (!eqns[i + 1][LT_POS]) {
            if (eqns[i][B_POS] > eqns[i + 1][B_POS]) {
                min = i + 1;
            }
        }
    }
    return min;
}
 
/**
 *  Divides each equation in an array with a specified coefficient.
 *
 *  @param eqns
 *          The equations on which to perform the division.
 *  @param coeff
 *          The specified coefficient (0 -> a, !0 -> b).
 */
void divide_equations_small(EQN_T** eqns, INT_T coeff)
{
    INT_T i;
    for (i = 0; i < LEN; ++i)
    {
        if (coeff)
        {
            if (eqns[i][B_POS] == 0)
            {
                continue;
            }
            
            if (eqns[i][B_POS] < 0)
            {
                eqns[i][LT_POS] = !eqns[i][LT_POS];
            }
            eqns[i][A_POS] /= eqns[i][B_POS];
            eqns[i][C_POS] /= eqns[i][B_POS];
            eqns[i][B_POS] = 1;
        } else {
            if (eqns[i][A_POS] == 0)
            {
                continue;
            }
            
            if (eqns[i][A_POS] < 0)
            {
                eqns[i][LT_POS] = !eqns[i][LT_POS];
            }
            eqns[i][B_POS] /= eqns[i][A_POS];
            eqns[i][C_POS] /= eqns[i][A_POS];
            eqns[i][A_POS] = 1;
        }
    }
}

/**
 *  Swaps two equations, i.e. changes rows of the equations.
 *
 *  @param x
 *          An equation to change the position of.
 *  @param y
 *          An equation to change the position of.
 */
void swap_equations_small(EQN_T **x, EQN_T **y)
{
    EQN_T *t = *x;
    *x = *y;
    *y = t;
}
 
/**
 *  Sorts a set of equations according to the coefficient of b.
 *
 *  @param eqns
 *          The equations to sort.
 */
void sort_bubble_equation_small(EQN_T** eqns)
{
    INT_T a, b;
    
    for (a = 0; a < LEN; ++a)
    {
        for (b = 0; b < LEN - a - 1; ++b)
        {
            if (eqns[b][B_POS] == 0) {
                swap_equations_small(&eqns[b], &eqns[b + 1]);
            } else if (eqns[b + 1][B_POS] > 0 && eqns[b][B_POS] < 0) {
                swap_equations_small(&eqns[b], &eqns[b + 1]);
            } else if (eqns[b][B_POS] * eqns[b + 1][B_POS] > 0 &&
                    eqns[b + 1][B_POS] < eqns[b][B_POS]) {

                swap_equations_small(&eqns[b], &eqns[b + 1]);
            }
        }
    }
}

/**
 *
 */
void add_minimum_equation_to_positive_small(EQN_T** eqns, EQN_T* eqn)
{
    INT_T i;
    for (i = 0; i < LEN; ++i)
    {
        if (eqns[i][LT_POS] && eqns[i][B_POS] > 0) {
            (eqns[i][A_POS]) = eqns[i][A_POS] - eqn[A_POS];
            (eqns[i][B_POS]) = eqns[i][B_POS] - eqn[B_POS];
            (eqns[i][C_POS]) = eqns[i][C_POS] - eqn[C_POS];
        }
    }
}

/**
 *  Finds the constraints of the minimal interval set by an array of equations.
 *
 *  @param eqns
 *          The set of equations.
 */
void find_constraints_small(EQN_T **eqns)
{
    EQN_T *min;
    EQN_T *max;
    
    *min = FLT_MAX;
    *max = FLT_MIN;
    
    // TODO:
    // Make sure iteration is only performed over positive equations.
    INT_T i;
    for (i = 0; i < LEN; ++i)
    {
        if (!(eqns[i][LT_POS] && eqns[i][B_POS] > 0))
        {
            continue;
        }
        
        if (eqns[i][LT_POS]) {
            if (*max < eqns[i][C_POS]) {
                *max = eqns[i][C_POS];
            }
        } else {
            if (*min > eqns[i][C_POS]) {
                *min = eqns[i][C_POS];
            }
        }
    }
    printf("Solution found in the interval [%.02lf, %.02lf].\n", *min, *max);
}

void zmk_small(EQN_T** eqns)
{
    sort_bubble_equation_small(eqns);
    divide_equations_small(eqns, 1);
    add_minimum_equation_to_positive_small(eqns,
            eqns[minimum_equation_small(eqns)]);
    divide_equations_small(eqns, 0);
    find_constraints_small(eqns);
}

#endif