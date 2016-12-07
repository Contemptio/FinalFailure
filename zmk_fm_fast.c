#ifndef ZMK_FM_fast_C
#define ZMK_FM_fast_C

#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <float.h>

void printEquation(EQN_T**, INT_T);
void printSystem(EQN_T***, INT_T, INT_T);

/* ========== *
 *  Utility.  *
 * ========== */

/**
 *  Prints an array of integers. Used for debugging.
 *
 *  @param array
 *          The array to print.
 *  @param len
 *          The number of elements in the array.
 *  @param name
 *          The name of the array.
 */
void printIntegerArray(INT_T* array, INT_T len, char* name)
{
    INT_T i;
    char* pre = "";
    printf("%s = [", name);
    for (i = 0; i < len; ++i) {
        printf("%s%hi", pre, array[i]);
        pre = ", ";
    }
    printf("]\n");
}

/* =========== *
 *  Equation.  *
 * =========== */

/**
 *  Allocates memory for a new system of equations.
 *  <p>
 *  @param nEqn
 *          The number of equations in the system.
 *  @return
 *          A pointer to the memory allocated for the system.
 */
EQN_T*** newSystem(INT_T nEqn)
{
    return (EQN_T***) malloc(sizeof(EQN_T**) * nEqn);
}

/**
 *  Allocates memory for a new equation.
 *  <p>
 *  @param nVar
 *          The number of coefficients in the equation. Note that memory is
 *          allocated for one additional coefficient (the constant).
 */
EQN_T** newEquation(INT_T nVar)
{
    return (EQN_T**) malloc(sizeof(EQN_T*) * (nVar + 1));
}

/**
 *  Copies an equation.
 *  <p>
 *  @param eqn
 *          The equation to copy.
 *  @param coeffPos
 *          The index of the coefficient most recently used to divide
 *          each equation.
 *  @return
 *          A new equation object containing the same coefficients
 *          as {@code eqn}.
 */
EQN_T** copyEquation(EQN_T** eqn, INT_T nVar)
{
    EQN_T** newEqn = newEquation(nVar);
    INT_T i;
    for (i = 0; i < nVar; ++i)
    {
        newEqn[i] = copyCoeff(eqn[i]);
    }

    return newEqn;
}

/**
 *  Copies an equation.
 *  <p>
 *  @param eqn
 *          The equation to reduce.
 *  @param coeffPos
 *          The index of the coefficient most recently used to divide
 *          each equation.
 *  @return
 *          A new equation object containing the same coefficients
 *          as {@code eqn}.
 */
EQN_T** reduceEquation(EQN_T** eqn, INT_T coeffPos)
{
    EQN_T** newEqn = newEquation(coeffPos);

    INT_T i;
    for (i = 0; i < coeffPos; ++i)
    {
        newEqn[i] = copyCoeff(eqn[i]);
    }
    newEqn[i] = copyCoeff(eqn[coeffPos + 1]);

    return newEqn;
}

/**
 *  Straight copy of the system of equations.
 *
 *  @param eqns
 *          The system to copy.
 *  @return
 *          A copy of the system.
 */
EQN_T*** copySystem(EQN_T*** eqns, INT_T nEqn, INT_T nVar)
{
    INT_T i;
    EQN_T*** newEqns = newSystem(nEqn);
    for (i = 0; i < nEqn; ++i)
    {
        newEqns[i] = copyEquation(eqns[i], nVar);
    }
    return newEqns;
}

/**
 *  Subtracts a "greater-than" relation with a "lesser-than" relation,
 *  producing a new "lesser-than" relation.
 *  <p>
 *
 *  @param pos
 *          An equation describing a "greater-than" relation.
 *  @param neg
 *          An equation describing a "lesser-than" relation.
 *  @param coeffPos
 *          The index of the coefficient most recently used to divide
 *          each equation.
 *  @return
 *          A new equation object describing a "lesser-than" relation.
 */
EQN_T** subEquations(EQN_T** pos, EQN_T** neg, INT_T coeffPos)
{
    EQN_T** newEqn = newEquation(coeffPos);
    INT_T i;
    INT_T cPos = coeffPos + 1;
    for (i = 0; i < coeffPos; ++i)
    {
        newEqn[i] = subCoeff(*pos[i], *neg[i]);
    }
    newEqn[i] = subCoeff(*pos[cPos], *neg[cPos]);
    return newEqn;
}

/* ============ *
 *  Algorithm.  *
 * ============ */

/**
 *  Investigates if the system of relation-equations produced by the
 *  Fourier-Motzkin elimination has a solution or not.
 *  <p>
 *
 *  @param eqns
 *          The system of equations.
 *  @param negIndices
 *          An array containing the indices of equations describing a
 *          "greater-than" relation.
 *  @param posIndices
 *          An array containing the indices of equations describing a
 *          "less-than" relation.
 *  @param nNeg
 *          The number of indices contained within {@code negIndices}.
 *  @param nPos
 *          The number of indices contained within {@code negIndices}.
 *  @param nEqn
 *          The number of equations in the equation system.
 *  @return
 *          Zero if no solution could be found, a non-zero integer otherwise.
 */
INT_T checkConstraints(EQN_T*** eqns, INT_T* negIndices,
        INT_T* posIndices, INT_T nNeg, INT_T nPos, INT_T nEqn) {
        
        float qi = FLT_MIN;
        float qj = FLT_MIN;
        float cmp;
        INT_T i;
        
        if (!nNeg || !nPos)
        {
            for (i = 0; i < nEqn; ++i)
            {
                if (!eqns[i][0]->nom) {
                    return 0;
                }
            }
            return 1;
        }

        for (i = 0; i < nNeg; ++i)
        {
            cmp = -evalCoeff(eqns[negIndices[i]][1]);
            qj = qj < cmp ? cmp : qj;
        }
        for (i = 0; i < nPos; ++i)
        {
            cmp = -evalCoeff(eqns[posIndices[i]][1]);
            qi = qi < cmp ? cmp : qj;
        }

        return qj != FLT_MIN && qj < qi;
}

/**
 *  Pairs equations describing "lesser-than" relations with equations describing
 *  "greater-than" relations, producing one new relation for each
 *  such pairing.
 *  <p>
 *
 *  @param eqns
 *          A pointer to the system of equations. When the function terminates,
 *          this pointer points to the new system of equations containing the
 *          pairings between "lesser-than" and "greater-than" relations.
 *  @param negIndices
 *          An array containing the indices of equations describing a
 *          "greater-than" relation.
 *  @param posIndices
 *          An array containing the indices of equations describing a
 *          "less-than" relation.
 *  @param nNeg
 *          The number of indices contained within {@code negIndices}.
 *  @param nPos
 *          The number of indices contained within {@code negIndices}.
 *  @param nEqn
 *          A pointer to an integer containing the number of equations in the
 *          equation system. This value is updated within this function.
 *  @param coeffPos
 *          The index of the coefficient most recently used to divide
 *          each equation.
 */
INT_T divideEquations(EQN_T*** eqns, INT_T* negIndices,
        INT_T* posIndices, INT_T* nNeg, INT_T* nPos,
        INT_T nEqn, INT_T coeffPos)
{
    INT_T i;
    INT_T j;
    INT_T cPos = coeffPos + 1;

    for (i = 0; i < nEqn; ++i)
    {
        EQN_T** eqn = eqns[i];
        EQN_T* div = eqn[coeffPos];

        if (!div->nom) {
            posIndices[(*nPos)++] = i;
            continue;
        }

        for (j = 0; j < coeffPos; ++j)
        {
            divCoeff(eqns[i][j], div);
        }
        divCoeff(eqns[i][cPos], div);
        
        if (div->nom < 0) {
            negIndices[(*nNeg)++] = i;
        } else {
            posIndices[(*nPos)++] = i;
        }
        eqns[i][coeffPos]->nom = eqns[i][coeffPos]->denom = 1;
    }
    if (!(*nNeg) && !(*nPos))
    {
        return -1;
    }
    return (*nNeg) && (*nPos);
}

/**
 *  Pairs equations describing "lesser-than" relations with equations describing
 *  "greater-than" relations, producing one new relation for each
 *  such pairing.
 *  <p>
 *
 *  @param eqns
 *          A pointer to the system of equations. When the function terminates,
 *          this pointer points to the new system of equations containing the
 *          pairings between "lesser-than" and "greater-than" relations.
 *  @param negIndices
 *          An array containing the indices of equations describing a
 *          "greater-than" relation.
 *  @param posIndices
 *          An array containing the indices of equations describing a
 *          "less-than" relation.
 *  @param nNeg
 *          The number of indices contained within {@code negIndices}.
 *  @param nPos
 *          The number of indices contained within {@code negIndices}.
 *  @param nEqn
 *          A pointer to an integer containing the number of equations in the
 *          equation system. This value is updated within this function.
 */
void pairEquations(EQN_T**** eqns, INT_T* negIndices, INT_T* posIndices,
    INT_T nNeg, INT_T nPos, INT_T* nEqn, INT_T coeffPos)
{
    INT_T i;
    INT_T j;

    INT_T p = 0;
    
    EQN_T*** newEqns = newSystem(!nNeg ? nPos : nNeg * nPos);
    
    for(i = 0; i < nPos; ++i)
    {

        EQN_T** pos = (*eqns)[posIndices[i]];
        if (pos[coeffPos]->nom == 0) {
            newEqns[p++] = reduceEquation(pos, coeffPos);
            continue;
        }
        
        for (j = 0; j < nNeg; ++j)
        {
            EQN_T** neg = (*eqns)[negIndices[j]];
            if (neg[coeffPos]->nom == 0)
            {
                newEqns[p++] = reduceEquation(neg, coeffPos);
                continue;
            }
            
            newEqns[p++] = subEquations(pos, neg, coeffPos);
        }
    }
    freeSystem(*eqns, *nEqn, coeffPos + 2);
    *nEqn = p;
    *eqns = newEqns;
}

/**
 *  Performs Fourier-Motzkin elimination on a given system of equations.
 *  <p>
 *
 *  @param eqns
 *          The system of equations.
 *  @param nEqn
 *          The number of equations.
 *  @param nVar
 *          The number of variables in each equation.
 *  @return
 *          Zero if no solution could be found, a non-zero integer otherwise.
 */
INT_T zmkFast(EQN_T*** eqns, INT_T nEqn, INT_T nVar)
{
    INT_T i;
    INT_T n = nEqn;
    INT_T currVar;
    INT_T nNeg = 0;
    INT_T nPos = 0;
    INT_T res = 0;
    INT_T* negIndices;
    INT_T* posIndices;

    for (i = 0; i < nVar; ++i)
    {
        nNeg = 0;
        nPos = 0;
        currVar = nVar - i - 1;
        
        negIndices = (INT_T*) malloc(sizeof(INT_T) * n);
        posIndices = (INT_T*) malloc(sizeof(INT_T) * n);
        
        divideEquations(eqns, negIndices, posIndices, &nNeg, &nPos,
                n, currVar);
        
        if (currVar == 0)
        {
            break;
        }
        
        pairEquations(&eqns, negIndices, posIndices, nNeg, nPos,
            &n, currVar);
        
        free(negIndices);
        free(posIndices);
    }

    res = checkConstraints(eqns, negIndices, posIndices, nNeg,
            nPos, n);
    
    freeSystem(eqns, n, currVar + 2);
    free(negIndices);
    free(posIndices);
    
    return res;
}

/**
 *  Performs Fourier-Motzkin elimination on a given system of equations.
 *  <p>
 *  This function includes debugging prints to the standard output, but is
 *  otherwise identical to {@code zmkFast}.
 *
 *  @param eqns
 *          The system of equations.
 *  @param nEqn
 *          The number of equations.
 *  @param nVar
 *          The number of variables in each equation.
 *  @return
 *          Zero if no solution could be found, a non-zero integer otherwise.
 */
INT_T zmkFastDebug(EQN_T*** eqns, INT_T nEqn, INT_T nVar)
{
    //EQN_T*** eqns = copySystem(origEqns, nEqn, nVar);
    printf("Received equations:\n");
    printSystem(eqns, nEqn, nVar + 1);
    INT_T i;
    INT_T n = nEqn;
    INT_T currVar;
    INT_T nNeg = 0;
    INT_T nPos = 0;
    INT_T res = 0;
    INT_T* negIndices;
    INT_T* posIndices;

    for (i = 0; i < nVar; ++i)
    {
        nNeg = 0;
        nPos = 0;
        currVar = nVar - i - 1;
        
        negIndices = (INT_T*) malloc(sizeof(INT_T) * n);
        posIndices = (INT_T*) malloc(sizeof(INT_T) * n);
        
        printf("Dividing for coeff %hu\n", currVar);
        divideEquations(eqns, negIndices, posIndices, &nNeg, &nPos,
                n, currVar);

        printIntegerArray(negIndices, nNeg, "Negative");
        printIntegerArray(posIndices, nPos, "Positive");
        printSystem(eqns, n, currVar + 2);
        
        if (currVar == 0)
        {
            break;
        }
        
        pairEquations(&eqns, negIndices, posIndices, nNeg, nPos,
            &n, currVar);
        
        free(negIndices);
        free(posIndices);
        
        printSystem(eqns, n, currVar + 1);
        printf("Current number of equations: %hi\n", n);
    }

    res = checkConstraints(eqns, negIndices, posIndices, nNeg,
            nPos, n);

    
    if (res)
    {
        printf("Solution found.\n\n");
    } else {
        printf("No solution.\n");
    }
    
    freeSystem(eqns, n, currVar + 2);
    free(negIndices);
    free(posIndices);
    
    return res;
}

#endif
