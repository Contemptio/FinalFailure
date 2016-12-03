#ifndef ZMK_FM_fast_C
#define ZMK_FM_fast_C

#include "eqn.c"
#include "util.c"
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
void add_eqn_to_set_fast(EQN_T **eqns, EQN_T* minEqn)
{
    // TODO.
    // Save data from eqns[eqn] in local variables and
    // use them instead.
    INT_T i;
    for (i = 0; i < LEN; ++i)
    {
        EQN_T* eqn = eqns[i];
        if (!eqn[LT_POS])
        {
            continue;
        }
        EQN_T* a = &eqn[A_POS];
        EQN_T* b = &eqn[B_POS];
        EQN_T* c = &eqn[C_POS];
        *a = *a - minEqn[A_POS];
        *b = *b - minEqn[B_POS];
        *c = *c - minEqn[C_POS];
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
INT_T minimum_equation_fast(EQN_T** eqns)
{
    INT_T min = 0;
    INT_T i;
    for (i = 0; i < LEN - 1; ++i)
    {
        // TODO.
        // Manually check source files to see which comparisons hold true
        // the most often; put the comparisons in that order.
        EQN_T* eqn1 = eqns[i];
        EQN_T* eqn2 = eqns[i + 1];

        if (eqn2[LT_POS])
        {
            continue;
        }

        if (eqn1[LT_POS] || eqn1[B_POS] > eqn2[B_POS])
        {
            min = i + 1;
        }
    }
    return eqns[min];
}
 
/**
 *  Divides each equation in an array with a specified coefficient.
 *
 *  @param eqns
 *          The equations on which to perform the division.
 *  @param coeff
 *          The specified coefficient (0 -> a, !0 -> b).
 */
void divide_equations_fast(EQN_T** eqns, INT_T coeff)
{
    INT_T i;
    for (i = 0; i < LEN; ++i)
    {
        // TODO.
        // Manually check source files to see which comparisons hold true
        // the most often; put the comparisons in that order.
        // 
        // Save eqns[i][A_POS and B_POS] in local variables (if in the branches).
        EQN_T* div;
        if (!(div = eqns[i][(coeff ? B_POS : A_POS)]))
        {
            continue;
        }
        EQN_T* eqn = eqns[i];
        if (*div < 0)
        {
            eqn[LT_POS] = !eqn[LT_POS];
        }
        
        // Division can be made faster. Shift if division by even number.
        // I.e. check if last bit is 0, then just shift.
        eqn[A_POS] /= div;
        eqn[B_POS] /= div;
        eqn[C_POS] /= div;
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
void swap_equations_fast(EQN_T **x, EQN_T **y)
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
void sort_bubble_equation_fast(EQN_T** eqns)
{
    INT_T a, b;
    
    // TODO.
    // Very likely that these for-loops can be optimized.
    //
    // Save eqns[b][B_POS] in local variable.
    // Same for eqns[b + 1][B_POS].
    // Sign comparison is the fastest this way? Consider if and if.
    for (a = 0; a < LEN; ++a)
    {
        for (b = 0; b < LEN - a - 1; ++b)
        {
            EQN_T* eqn1 = &eqns[b];
            EQN_T* eqn2 = &eqns[b + 1];
            
            if (eqn1[B_POS] == 0) {
                swap_equations_fast(&eqn1, &eqn2);
                
            // Check first bit to see gtz and ltz.
            } else if (eqn2[B_POS] > 0 && eqn1[B_POS] < 0) {
                swap_equations_fast(&eqn1, &eqn2);
            } else if (eqn1[B_POS] * eqn2[B_POS] > 0 &&
                    eqn2[B_POS] < eqn1[B_POS]) {

                swap_equations_fast(&eqn1, &eqn2);
            }
        }
    }
}

/**
 *
 */
void add_minimum_equation_to_positive_fast(EQN_T** eqns, EQN_T* minEqn)
{
    INT_T i;
    // TODO can this for-loop be optimized?
    for (i = 0; i < LEN; ++i)
    {
        EQN_T* eqn = eqns[i];
        if (!eqn[LT_POS])
        {
            continue;
        }
        
        EQN_T* b = &eqn[B_POS];
        if (*b > 0) {
            EQN_T* a = &eqn[A_POS];
            EQN_T* c = &eqn[C_POS];
            *a = *a - minEqn[A_POS];
            *b = *b - minEqn[B_POS];
            *c = *c - minEqn[C_POS];
        }
    }
}

/**
 *  Finds the constraints of the minimal interval set by an array of equations.
 *
 *  @param eqns
 *          The set of equations.
 */
void find_constraints_fast(EQN_T **eqns)
{
    EQN_T min = FLT_MAX;
    EQN_T max = FLT_MIN;

    INT_T i;
    for (i = 0; i < LEN; ++i)
    {
        EQN_T* eqn = eqns[i];
        EQN_T* c = &eqn[C_POS];
        if (eqn == minEqn || !eqn[A_POS] && !eqn[B_POS] && !(*c))
        {
            continue;
        }
        
        if (eqn[LT_POS] && max < (*c)) {
            max = *c;
            continue;
        }
        if (min > (*c)) {
            min = *c;
        }
    }
    printf("Solution found in the interval [%.02f, %.02f].\n", min, max);
}

void zmk_fast_debug(EQN_T** eqns)
{
    printf("[ZMK_FAST]: NOT YET IMPLEMENTED.\n");
    return;
    
    sort_bubble_equation_fast(eqns);
    divide_equations_fast(eqns, 1);
    // TODO.
    // Sort out which equations are positive in divide_equations_fast,
    // return that result as an array of indices (INT_T).
    add_minimum_equation_to_positive_fast(eqns,
            eqns[minimum_equation_fast(eqns)]);
    divide_equations_fast(eqns, 0);
    find_constraints_fast(eqns);
}

#endif
