#ifndef INT_PAIR_C
#define INT_PAIR_C

#include "util.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct int_pair
{
	int *a;
	int *b;
} int_pair;

/**
 *	Allocates memory for an integer pair.
 */
 int_pair* new_int_pair(int a, int b)
 {
	int_pair* pair = (int_pair *) malloc(sizeof(int_pair));
	if (pair == NULL)
	{
		free(pair);
		error("Error allocating memory for integer pair.");
	}
	
	pair->a = (int *) malloc(sizeof(int));
	pair->b = (int *) malloc(sizeof(int));
	*(pair->a) = a;
	*(pair->b) = b;
	
	if (pair->a == NULL || pair->b == NULL)
	{
		free(pair->a);
		free(pair->b);
		free(pair);
		error("Error allocating memory for integer pair member.");
	}
	
	return pair;
 }
 
 void swap_int_pairs(int_pair *x, int_pair *y)
 {
	int ta = *(x->a);
	int tb = *(x->b);
	*(x->a) = *(y->a);
	*(x->b) = *(y->b);
	*(y->a) = ta;
	*(y->b) = tb;
 }
 
 /**
  *	Free an integer pair.
  */
 void free_int_pair(int_pair* pair)
 {
	 free(pair->a);
	 free(pair->b);
	 free(pair);
 }

#endif