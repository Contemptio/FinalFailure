#ifndef COEFF_C
#define COEFF_C

#include "coeff.h"
#include <stdio.h>
#include <stdlib.h>

/* ========= *
 *  Object.  *
 * ========= */

/**
 *  Re-evaluates the sign of the coefficient, only used locally.
 *  <p>
 *  This is done so that a negative coefficient's sign is always on the
 *  nominator, and that both the nominator and denominator can not be negative.
 *
 *  @param lhs
 *          The left coefficient.
 *  @param lhs
 *          The right coefficient.
 *  @return
 *          The result of the relation.
 */
void fixSign(EQN_T* coeff)
{
    if (coeff->nom < 0)
    {
        if (coeff->denom < 0)
        {
            coeff->nom *= -1;
            coeff->denom *= -1;
        }
    } else {
        if (coeff->denom < 0) {
            coeff->nom *= -1;
            coeff->denom *= -1;
        }
    }
}

/**
 *  Allocates memory for a new coefficient.
 *
 *  @param nom
 *          The value of the nominator.
 *  @param denom
 *          The value of the denominator.
 *  @return
 *          A pointer to the new coefficient object.
 */
EQN_T* newCoeff(INT_T nom, INT_T denom)
{
    EQN_T* coeff = (EQN_T*) malloc(sizeof(EQN_T));
    coeff->nom = nom;
    coeff->denom = denom;
    return coeff;
}

/**
 *  Copies a coefficient.
 *
 *  @param coeff
 *          The coefficient to copy.
 *  @return
 *          A new coefficient object with the same values as {@code coeff}.
 */
EQN_T* copyCoeff(EQN_T* coeff)
{
    EQN_T* copy = newCoeff(0, 0);
    copy->nom = coeff->nom;
    copy->denom = coeff->denom;
    return copy;
}

/* ============= *
 *  Arithmetic.  *
 * ============= */

/**
 *  Returns whether or not a coefficient is less than another.
 *
 *  @param lhs
 *          The left coefficient.
 *  @param lhs
 *          The right coefficient.
 *  @return
 *          The result of the relation.
 */
short lessThanCoeff(EQN_T* lhs, EQN_T* rhs)
{
    return evalCoeff(lhs) < evalCoeff(rhs);
}

/**
 *  Adds a coefficient to another.
 *
 *  @param lhs
 *          The left coefficient.
 *  @param lhs
 *          The right coefficient.
 *  @return
 *          The result of the addition.
 */
void addCoeff(EQN_T lhs, EQN_T rhs)
{
    if (lhs.denom == rhs.denom)
    {
        lhs.nom += rhs.nom;
    } else {
        lhs.nom = (lhs.nom * rhs.denom + rhs.nom * lhs.denom);
        lhs.denom *= rhs.denom;
    }
    fixSign(&lhs);
}

/**
 *  Subtracts a coefficient from another.
 *
 *  @param lhs
 *          The left coefficient.
 *  @param lhs
 *          The right coefficient.
 *  @return
 *          The result of the subtraction.
 */
EQN_T* subCoeff(EQN_T lhs, EQN_T rhs)
{
    EQN_T* coeff = newCoeff(0, 0);
    if (lhs.denom == rhs.denom)
    {
        coeff->nom = lhs.nom - rhs.nom;
        coeff->denom = lhs.denom;
    } else {
        coeff->nom = (lhs.nom * rhs.denom - rhs.nom * lhs.denom);
        coeff->denom = lhs.denom * rhs.denom;
    }
    fixSign(coeff);
    return coeff;
}

/**
 *  Divides a coefficient with another.
 *
 *  @param lhs
 *          The left coefficient.
 *  @param lhs
 *          The right coefficient.
 *  @return
 *          The result of the division.
 */
void divCoeff(EQN_T* lhs, EQN_T* rhs)
{
    if (lhs->denom == rhs->denom)
    {
        lhs->denom = rhs->nom;
    } else {
        lhs->nom *= rhs->denom;
        lhs->denom *= rhs->nom;
    }
    fixSign(lhs);
}

/**
 *  Evaluates a coefficient, i.e. returns {@code nom / denom}.
 *
 *  @param coeff
 *          The coefficient to evaluate.
 *  @return
 *          The rational value described by the coefficient.
 */
float evalCoeff(EQN_T* coeff)
{
    return ((float) coeff->nom) / ((float) coeff->denom);
}

#endif