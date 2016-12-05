#ifndef COEFF_H
#define COEFF_H

#define EQN_T coeff_t
#define INT_T short

typedef struct coeff {
    INT_T nom;
    INT_T denom;
} coeff_t;

void fixSign(EQN_T* coeff);
EQN_T* copyCoeff(EQN_T*);
EQN_T* newCoeff(INT_T, INT_T);

short lessThanCoeff(EQN_T*, EQN_T*);
void addCoeff(EQN_T, EQN_T);
EQN_T* subCoeff(EQN_T, EQN_T);
void divCoeff(EQN_T*, EQN_T*);
float evalCoeff(EQN_T*);

#endif