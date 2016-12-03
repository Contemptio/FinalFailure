#ifndef ZMK_FM_fast_C
#define ZMK_FM_fast_C

#include "eqn.c"
#include "util.c"
#include <short.h>
#include <stdio.h>
#include <stdlib.h>

/*
 *  Type macros for easily changing.
 */
#define EQN_T short
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
 *  @param len
 *          The number of equations.
 */
void add_eqn_to_set_fast(EQN_T **eqns, EQN_T* minEqn, INT_T len)
{
    INT_T i;
    for (i = 0; i < len; ++i)
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
 *  @param len
 *          The number of equations.
 *  @return
 *          the index of the equation with the lowest coefficient of b.
 */
INT_T minimum_equation_fast(EQN_T** eqns, INT_T len)
{
    INT_T min = 0;
    INT_T i;
    for (i = 0; i < len - 1; ++i)
    {
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
 *  <p>
 *  I first considered making division faster with bit-shifting, but
 *  as I gathered it, the optimizer handles that on its own...
 *
 *  @param eqns
 *          The equations on which to perform the division.
 *  @param len
 *          The number of equations.
 *  @param coeff
 *          The specified coefficient (0 -> a, !0 -> b).
 */
void divide_equations_fast(EQN_T** eqns, INT_T len, INT_T coeff)
{
    INT_T i;
    for (i = 0; i < len; ++i)
    {
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
 *  @param len
 *          The number of equations.
 */
void sort_bubble_equation_fast(EQN_T** eqns, INT_T len)
{
    INT_T a, b;

    for (a = 0; a < len; ++a)
    {
        for (b = 0; b < len - a - 1; ++b)
        {
            EQN_T* eqn1 = &eqns[b];
            EQN_T* eqn2 = &eqns[b + 1];
            
            if (eqn1[B_POS] == 0) {
                swap_equations_fast(&eqn1, &eqn2);

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
 *  Adds the selected minimum equation to all positive equations.
 *
 *  @param eqns
 *          The set of equations.
 *  @param minEqn
 *          The minimum equation.
 *  @param len
 *          The number of equations.
 */
void add_minimum_equation_to_positive_fast(EQN_T** eqns,
        EQN_T* minEqn, INT_T len)
{
    INT_T i;

    for (i = 0; i < len; ++i)
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
INT_T find_constraints_fast(EQN_T **eqns, INT_T len)
{
    EQN_T min = FLT_MAX;
    EQN_T max = FLT_MIN;

    INT_T i;
    for (i = 0; i < len; ++i)
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

/**
 *  Attempts to perform Fourier-Motzkin elimination as fast as possible.
 *
 *  @param eqns
 *          The set of equations on which to perform
 *          Fourier-Motzkin elimination.
 *  @return
 *          Non-zero integer if a solution was found, zero otherwise.
 */
INT_T zmk_fast(EQN_T** eqns, INT_T len)
{
    sort_bubble_equation_fast(eqns, len);
    divide_equations_fast(eqns, len, 1);
    add_minimum_equation_to_positive_fast(eqns,
            eqns[minimum_equation_fast(eqns)], len);
    divide_equations_fast(eqns, len, 0);
    return find_constraints_fast(eqns, len);
}

INT_T zmk_fast_debug(EQN_T** eqns, INT_T len)
{
    sort_bubble_equation_fast(eqns, len);
    printf("Sorted equations.\n");
    print_eqn_matrix(eqns, len);

    divide_equations_fast(eqns, len, 1);
    printf("Divided equations.\n");
    print_eqn_matrix(eqns, len);
    
    EQN_T* minEqn = minimum_equation_fast(eqns, len);
    printf("Selected smallest equation.\n");
    print_eqn_array(minEqn);
    
    add_minimum_equation_to_positive_fast(eqns, minEqn, len);
    printf("Added minimum equation to positive equations.\n");
    print_eqn_matrix(eqns, len);
            
    divide_equations_fast(eqns, len, 0);
    printf("Divided equations again.\n");
    print_eqn_matrix(eqns, len);
    
    find_constraints_fast(eqns, minEqn, len);
    printf("Found constraints.\n");
    print_eqn_matrix(eqns, len);
}

#endif
