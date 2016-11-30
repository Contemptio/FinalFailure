#ifndef ZMK_FM_C
#define ZMK_FM_C

// #include "eqn.c"
// #include "util.c"
#include <float.h>
#include <stdio.h>
#include <stdlib.h>

#define LEN (5)

/**
 *  Sets the equivalence 'eqn <= eqns[i]' for i in 0 to nPos. The result is
 *  stored in eqns.
 *
 *  @param eqns
 *          The equations to which to add eqn.
 *  @param eqn
 *          The equation which to add to the rest.
 *  @param
 *          The number of equations in eqns.
 */
void add_eqn_to_set(equation_t **eqns, equation_t *eqn, size_t nPos)
{
    int i;
    for (i = 0; i < nPos; ++i)
    {
        equation_t *pe = eqns[i];
        if (!*(pe->lt))
        {
            continue;
        }
        *(pe->a) = *(pe->a) - *(eqn->a);
        *(pe->b) = *(pe->b) - *(eqn->b);
        *(pe->c) = *(pe->c) - *(eqn->c);
    }
}

/**
 *  Finds the 'minimum' equation in the set, i.e. the equation with the lowest
 *  coefficient of b.
 *
 *  @param eqns
 *          The equations among which to search.
 *  @return
 *          the equation with the lowest coefficient of b.
 */
equation_t* find_min(equation_t** eqns)
{
    equation_t *sol = eqns[0];
    int i;
    for (i = 0; i < LEN - 1; ++i)
    {
        equation_t *t = eqns[i + 1];
        
        short s_pos = *(sol->lt);
        short t_pos = *(t->lt);
        
        if (s_pos) {
            if (!t_pos) {
                sol = t;
            }
        } else if (!t_pos) {
            if (*(sol->b) > *(t->b)) {
                sol = t;
            }
        }
    }
    return sol;
}

/**
 *  Divides each equation in an array with a specified coefficient.
 *
 *  @param eqns
 *          The equations on which to perform the division.
 *  @param n_pos
 *          The number of equations in the array eqns.
 *  @param coeff
 *          The specified coefficient (0 -> a, 1 -> b).
 */
void divide_equations(equation_t** eqns, size_t n_pos, unsigned short coeff)
{
    int i;
    for (i = 0; i < n_pos; ++i)
    {
        equation_t *e = eqns[i];
        double t = coeff ? *(e->b) : *(e->a);
        double div = (t != 0) ? t : 1;
        
        *(e->a) /= div;
        *(e->b) /= div;
        *(e->c) /= div;
        *(e->lt) = (div > 0) ? *(e->lt) : !*(e->lt);
    }
}

/**
 *  Finds the constraints of the minimal interval set by an array of equations.
 *
 *  @param eqns
 *          The set of equations.
 *  @param min
 *          A pointer where to store the minimum value of the interval.
 *  @param max
 *          A pointer where to store the maximum value of the interval.
 *  @param n
 *          The number of equations in eqns.
 */
void find_constraints(equation_t **eqns, double *min, double *max, size_t n)
{
    *min = DBL_MAX;
    *max = DBL_MIN;
    
    int i;
    for (i = 0; i < n; ++i)
    {
        double sol = *(eqns[i]->c);
        short lt = *(eqns[i]->lt);
        
        if (lt) {
            if (*max < sol) {
                *max = sol;
            }
        } else {
            if (*min > sol) {
                *min = sol;
            }
        }
    }
}

/**
 *  Collects all positive equations, i.e. equations with a positive coefficient
 *  of b.
 *
 *  @param eqns
 *          The equations among which to search for 'positive' ones.
 *  @param pos_eqns
 *          A pointer where to store the positive equations.
 *  @return
 *          The number of 'positive' equations.
 */
int positive_equations(equation_t** eqns, equation_t** pos_eqns)
{
    int i, p = 0;
    for (i = 0; i < LEN; ++i)
    {
        if (*(eqns[i]->lt) && *(eqns[i]->b) > 0) {
            pos_eqns[p++] = eqns[i];
        }
    }
    
    return p;
}

/**
 *  Sorts a set of equations according to the coefficient of b.
 *
 *  @param eqns
 *          The equations to sort.
 */
void sort_bubble_equation(equation_t** eqns)
{
    int a, b;
    
    for (a = 0; a < LEN; ++a)
    {
        for (b = 0; b < LEN - a - 1; ++b)
        {
            double y1 = *(eqns[b]->b);
            double y2 = *(eqns[b + 1]->b);
            
            if (y2 == 0) {
                continue;
            }
            
            if (y1 == 0) {
                swap_equations(&eqns[b], &eqns[b + 1]);
            } else if (y2 > 0 && y1 < 0) {
                swap_equations(&eqns[b], &eqns[b + 1]);
            } else if (y1 * y2 > 0 && y2 < y1) {
                swap_equations(&eqns[b], &eqns[b + 1]);
            }
        }
    }
}

/**
 *  Finds an interval for the coefficient a solving a set of equations using
 *  Fourier-Motzkin elimination. This solver is limited to equations of two
 *  unknowns.
 *  
 *  @param eqns
 *          The set of equations.
 */
void zmk_fm(equation_t** eqns)
{
    println("Received equation set:");
    print_equations(eqns, LEN);
    println("\n");
    
    printf("Sorting equations...\n");
    sort_bubble_equation(eqns);
    print_equations(eqns, LEN);
    println("\n");
    
    println("Dividing with coefficient of x_2:");
    divide_equations(eqns, LEN, 1);
    print_equations(eqns, LEN);
    println("\n");
    
    printf("Selecting minimum equation:\n\t");
    equation_t *min_eqn = find_min(eqns);
    print_equation(min_eqn);
    println("\n");

    printf("Sorting out positive equations. ");
    equation_t **pos_eqns = (equation_t **) malloc(sizeof(equation_t *) * LEN);
    int n_pos = positive_equations(eqns, pos_eqns);
    printf("%d positive equations found:\n", n_pos);
    print_equations(pos_eqns, n_pos);
    println("\n");

    println("Adding minimum equation to positive equations:");
    add_eqn_to_set(pos_eqns, min_eqn, n_pos);
    print_equations(pos_eqns, n_pos);
    println("\n");
    
    printf("Dividing positive equations with the coefficient of x_1:\n");
    divide_equations(pos_eqns, n_pos, 0);
    print_equations(pos_eqns, n_pos);
    println("\n");
    
    double min, max;
    find_constraints(pos_eqns, &min, &max, n_pos);
    
    printf("Solution found in the interval [%.02lf, %.02lf].\n", min, max);
    println("\n");
    
    free(pos_eqns);
}

#endif