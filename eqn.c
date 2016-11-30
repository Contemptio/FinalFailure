#ifndef EQUATION_C
#define EQUATION_C

#include "util.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 *	A structure defining an equation on the form ax + by = c.
 *	<p>
 *	The lt-member is a flag indicating if the relation is less-than (if lt is
 *	non-zero) or greater-than (lt is zero).
 */
typedef struct equation
{
	double *a;
	double *b;
	double *c;
	short *lt;
} equation_t;

/**
 *	Allocates memory for an integer eqn.
 *
 *	@param a
 *			The coefficient of x.
 *	@param b
 *			The coefficient of y.
 *	@param c
 *			The solution to the equation.
 *	@param lt
 *			Indicator of whether or not the relation is less-than
 *			or greater-than.
 *	
 */
 equation_t* new_equation(double a, double b, double c, short lt)
 {
	equation_t* eqn = (equation_t *) malloc(sizeof(equation_t));
	if (eqn == NULL)
	{
		free(eqn);
		error("Error allocating memory for integer eqn.");
	}
	
	eqn->a = (double *) malloc(sizeof(double));
	eqn->b = (double *) malloc(sizeof(double));
	eqn->c = (double *) malloc(sizeof(double));
	eqn->lt = (short *) malloc(sizeof(short));
	*(eqn->a) = a;
	*(eqn->b) = b;
	*(eqn->c) = c;
	*(eqn->lt) = lt;
	
	if (eqn->a == NULL || eqn->b == NULL || eqn->c == NULL)
	{
		free(eqn->a);
		free(eqn->b);
		free(eqn->c);
		free(eqn->lt);
		free(eqn);
		error("Error allocating memory for integer eqn member.");
	}
	
	return eqn;
 }

/**
 *	Prints an equation.
 *
 *	@param eqn
 *			The equation to print.
 */
void print_equation(equation_t* eqn)
{
	printf("\t%.02lfx + %.02lfy %c%c %.02lf\n", *(eqn->a),
		*(eqn->b), ((*(eqn->lt) ? '<' : '>')), '=', *(eqn->c));
}

/**
 *	Prints an array of equations.
 *
 *	@param eqns
 *			The equations to print.
 *	@param len
 *			The number of equations in eqns.
 */
void print_equations(equation_t** eqns, size_t len)
{
	int i;
	char* pre = "";
	printf("[\n");
	for (i = 0; i < len; ++i)
	{
		print_equation(eqns[i]);
	}
	println("]");
}
 
void swap_equations(equation_t **x, equation_t **y)
{
	equation_t *t = *x;
	*x = *y;
	*y = t;
}
 
/**
 *	Frees an equation.
 *
 *	@param eqn
 *			The equation to free.
 */
void free_equation(equation_t *eqn)
{
	free(eqn->a);
	free(eqn->b);
	free(eqn->c);
	free(eqn->lt);
	free(eqn);
}
 
/**
 *	Frees an array of equations.
 *
 *	@param eqns
 *			The equations to free.
 *	@param n
 *			The number of equations in eqns.
 */
void free_equations(equation_t **eqns, int n)
{
	int i;
	for (i = 0; i < n; ++i)
	{
		free_equation(eqns[i]);
	}
	free(eqns);
}

#endif