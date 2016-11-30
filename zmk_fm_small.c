#ifndef ZMK_FM_SMALL_C
#define ZMK_FM_SMALL_C

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
 *  Type macros for easily changing types.
 */
#define EQN_T float
#define INT_T unsigned short
/*
 *  Position macros for coefficients and the "lt-flag."
 */
 #define A_POS  (0)
 #define B_POS  (1)
 #define C_POS  (2)
 #define LT_POS (3)
 
/**
 *  Finds the "minimum" equation in the set, i.e. the index of the equation
 *  with the lowest coefficient of b.
 *
 *  @param eqns
 *          The equations among which to search.
 *  @return
 *          a pointer to the minimum equation.
 */
EQN_T* minimum_equation_small(EQN_T** eqns)
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
    return eqns[min];
}
 
/**
 *  Divides each equation in an array with a specified coefficient.
 *
 *  @param eqns
 *          The equations on which to perform the division.
 *  @param coeff
 *          The specified coefficient (0: a, !0: b).
 */
void divide_equations_small(EQN_T** eqns, INT_T coeff)
{
    INT_T i;
    for (i = 0; i < LEN; ++i)
    {
        /*
         *  The {@code coeff} operator is purely a selection of
         *  which coefficient to divide with.
         */
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
 *  Adds the selected minimum equation to all positive equations.
 *
 *  @param eqns
 *          The set of equations.
 *  @param minEqn
 *          The minimum equation.
 */
void add_minimum_equation_to_positive_small(EQN_T** eqns, EQN_T* minEqn)
{
    INT_T i;
    for (i = 0; i < LEN; ++i)
    {
        /*
         *  If this is the minimum equation, ignore.
         */
        if (eqns[i] == minEqn) {
            continue;
        } else if (!eqns[i][LT_POS] || eqns[i][B_POS] <= 0) {
            /*
             *  If this is a non-positive equation, mark it.
             */
            eqns[i][A_POS] = eqns[i][B_POS] = eqns[i][C_POS] = 0;
            continue;
        }
        eqns[i][A_POS] = eqns[i][A_POS] - minEqn[A_POS];
        eqns[i][B_POS] = eqns[i][B_POS] - minEqn[B_POS];
        eqns[i][C_POS] = eqns[i][C_POS] - minEqn[C_POS];
    }
}

/**
 *  Finds the constraints of the minimal interval set by an array of equations.
 *
 *  @param eqns
 *          The set of equations.
 *  @param minEqn
 *          The minimum equation.
 */
void find_constraints_small(EQN_T **eqns, EQN_T *minEqn)
{
    EQN_T min = FLT_MAX;
    EQN_T max = FLT_MIN;

    INT_T i;
    for (i = 0; i < LEN; ++i)
    {
        /*
         *  Second expression in the else-statement is to manage the same
         *  case explained in
         *  {@link add_minimum_equation_to_positive_small(EQN_T**, EQN_T*)}.
         */
        if (eqns[i] == minEqn ||
                !eqns[i][A_POS] && !eqns[i][B_POS] && !eqns[i][C_POS])
        {
            continue;
        }
        
        if (eqns[i][LT_POS]) {
            if (max < eqns[i][C_POS]) {
                max = eqns[i][C_POS];
            }
        } else {
            if (min > eqns[i][C_POS]) {
                min = eqns[i][C_POS];
            }
        }
    }
    printf("Solution found in the interval [%.02f, %.02f].\n", min, max);
}

/**
 *  Attempts to perform Fourier-Motzkin elimination using as little space
 *  as possible.
 *  <p>
 *  N.B.: {@link using minimum_equation_small(eqns)} twice is certainly
 *  redundant, but saves us a pointer.
 *
 *  @param eqns
 *          The set of equations on which to perform
 *          Fourier-Motzkin elimination.
 */
void zmk_small(EQN_T** eqns)
{
    sort_bubble_equation_small(eqns);
    divide_equations_small(eqns, 1);
    add_minimum_equation_to_positive_small(eqns, minimum_equation_small(eqns));
    divide_equations_small(eqns, 0);
    find_constraints_small(eqns, minimum_equation_small(eqns));
}

/**
 *  Identical to {@link zmk_small(EQN_T**)} but has prints for debugging.
 *
 *  @param eqns
 *          The set of equations on which to perform
 *          Fourier-Motzkin elimination.
 *  @see {@link zmk_small(EQN_T**)}.
 */
void zmk_small_debug(EQN_T** eqns)
{
    sort_bubble_equation_small(eqns);
    printf("Sorted equations.\n");
    print_float_matrix(eqns);

    divide_equations_small(eqns, 1);
    printf("Divided equations.\n");
    print_float_matrix(eqns);
    
    EQN_T* minEqn = minimum_equation_small(eqns);
    printf("Selected smallest equation.\n");
    print_float_array(minEqn);
    
    add_minimum_equation_to_positive_small(eqns, minEqn);
    printf("Added minimum equation to positive equations.\n");
    print_float_matrix(eqns);
            
    divide_equations_small(eqns, 0);
    printf("Divided equations again.\n");
    print_float_matrix(eqns);
    
    find_constraints_small(eqns, minEqn);
    printf("Found constraints.\n");
    print_float_matrix(eqns);
}

#endif
