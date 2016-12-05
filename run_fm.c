#ifndef RUN_FM_C
#define RUN_FM_C

#include "coeff.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Reads numbers from a file into an equation.
 *
 * @param file
 *          The file to read from.
 * @param nVar
 *          A pointer to an integer to contain the number of coefficients.
 * @param isC
 *          A flag specifying whether or not a file of constants is read.
 * @return
 *          A pointer to an array of coefficients describing an equation.
 */
EQN_T** parseEquation(FILE* file, INT_T nVar, INT_T isC)
{
    EQN_T **arr = (EQN_T **) malloc(sizeof(EQN_T*) * nVar);
    INT_T tmp = 0;
    INT_T i;
    
    if (isC)
    {
        fscanf(file, "%hi\n", &tmp);
        for (i = 0; i < nVar; ++i)
        {
            fscanf(file, "%hi\n", &tmp);
            arr[i] = newCoeff(tmp, 1);
        }
        return arr;
    }

    for (i = 0; i < nVar; ++i)
    {
        fscanf(file, "%hi", &tmp);
        arr[i] = newCoeff(tmp, 1);
    }

    return arr;
}

/**
 * Reads a file into an equation block. The first file is expected to have two
 * numbers on each line delimited by a singular space, the other file is
 * expected to have an isolated number on each line.
 *
 *
 * @param fileA
 *          The file to read containing the coefficients.
 * @param fileC
 *          The file to read containing the constant solutions to the
 *          relational equations.
 * @param nEqn
 *          A pointer to an integer to contain the number of equations.
 * @param nVar
 *          A pointer to an integer to contain the number of coefficients.
 * @return
 *          A pointer to an array of equations describing a system of equations.
 */
            
EQN_T*** parseSystem(FILE* fileA, FILE* fileC, INT_T* nEqn, INT_T* nVar)
{
    INT_T i;

    fscanf(fileA, "%hu %hu\n", nEqn, nVar);
    
    EQN_T** cs = parseEquation(fileC, *nEqn, 1);

    EQN_T*** eqns = (EQN_T ***) malloc(sizeof(EQN_T **) * (*nEqn));

    for (i = 0; i < (*nEqn); ++i)
    {
        eqns[i] = parseEquation(fileA, *nVar, 0);
        cs[i]->nom *= -1;
        eqns[i][*nVar] = cs[i];
    }
    free(cs);

    return eqns;
}

#endif
