#ifndef UTIL_H
#define UTIL_H

#define CHAR_BUF (50)

void concat(char*, char*);
void error(char*);
void printLn(char*);
void swap(void*, void*);
void freeEquation(EQN_T**, INT_T);
void freeSystem(EQN_T***, INT_T, INT_T);
void printCoeff(EQN_T*);
void printEquation(EQN_T**, INT_T);
void printSystem(EQN_T***, INT_T, INT_T);

#endif