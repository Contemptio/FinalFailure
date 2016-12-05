#ifndef UTIL_C
#define UTIL_C

#include "coeff.h"
#include "util.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ========= *
 *  General. *
 * ========= */

/**
 *  Concatenates two strings.
 *
 *  @param str1
 *          The destination string.
 *  @param str2
 *          The string to append to str1.
 */
void concat(char* str1, char* str2)
{
    char c[CHAR_BUF];
    strcpy(c, str2);
    strcat(str1, c);
}

/**
 *  Prints a message and then exits with error code 1.
 *
 *  @param msg
 *          The message to print.
 */
void error(char* msg)
{
    printLn(msg);
    exit(1);
}

/**
 *  Prints a string and starts at the next line.
 *
 *  @param msg
 *          The message to print.
 */
void printLn(char *msg)
{
    printf("%s\n", msg);
}

/**
 *  Re-points two pointers to each others' objects.
 *
 *  @param a
 *          A pointer to switch.
 *  @param b
 *          The other pointer to switch.
 */
void swap(void *a, void *b)
{
    void *t = a;
    a = b;
    b = t;
}

/* =========== *
 *  Equations. *
 * =========== */

/**
 *  Frees an EQN_T array.
 *
 *  @param arr
 *          The array to free.
 *  @param rows
 *          The number of rows in the array.
 */
void freeEquation(EQN_T** arr, INT_T cols)
{
    INT_T i;
    for (i = 0; i < cols; ++i)
    {
        free(arr[i]);
    }
    free(arr);
}

/**
 *  Frees an EQN_T matrix.
 *
 *  @param mat
 *          The matrix to free.
 *  @param rows
 *          The number of rows in the matrix.
 */
void freeSystem(EQN_T*** mat, INT_T rows, INT_T cols)
{
    INT_T i;
    for (i = 0; i < rows; ++i)
    {
        freeEquation(mat[i], cols);
    }
    free(mat);
}

void printCoeff(EQN_T* eqn)
{
    printf("(%hi/%hi)", eqn->nom, eqn->denom);
}

/**
 *  Prints an EQN_T array.
 *
 *  @param eqn
 *          The EQN_T array to print.
 */
void printEquation(EQN_T** eqn, INT_T nVar)
{
    printf("[");
    char* pre = " + ";
    INT_T i;
    for (i = 0; i < nVar; ++i)
    {
        printf("%s", pre);
        printCoeff(eqn[i]);
        pre = "\t";
    }
    printf("]\n");
}

/**
 *  Prints a matrix of equations.
 *
 *  @param eqns
 *          The equations to print.
 */
void printSystem(EQN_T*** eqns, INT_T len, INT_T nVar)
{
    INT_T i;
    printf("[\n");
    for (i = 0; i < len; ++i)
    {
        printEquation(eqns[i], nVar);
    }
    printLn("]");
}

#endif