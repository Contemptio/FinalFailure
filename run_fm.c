#ifndef RUN_FM_C
#define RUN_FM_C

#include "eqn.c"
#include "zmk_fm.c"
#include "zmk_fm_fast.c"
#include "zmk_fm_small.c"
#include "util.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_FAST "zmk_fast"
#define NAME_SMALL "zmk_small"

#define EQN_T float
#define SCANF_FORMAT "%f %f"
#define BUF_SIZ (50)
#define LEN     (5)

 /**
  * Reads a file into a float array. The file is expected to have an isolated
  * number on each line.
  *
  * @param path
  *         Path to the file to read.
  * @return
  *         Pointer to the float array.
  */
 EQN_T* parse_float_array(char* path)
 {
    FILE *file = file_open(path);
    
    EQN_T *arr = (EQN_T *) malloc(sizeof(EQN_T) * LEN);
    int i;
    
    for (i = 0; i < LEN; ++i)
    {
        fscanf(file, "%f", &arr[i]);
    }
    
    file_close(file);
    return arr;
 }
 
  /**
  * Reads a file into an equation block. The first file is expected to have two
  * numbers on each line delimited by a singular space, the other file is
  * expected to have an isolated number on each line.
  *
  * @param pathA
  *         Path to the file to read containing the coefficients of x and y.
  * @param pathC
  *         Path to the file to read containing the constant solutions to the
  *         relational equations.
  * @return
  *         Pointer to the equation block.
  */
EQN_T** parse_eqns_numerical(char *pathA, char *pathC)
{
    FILE *fileA = file_open(pathA);
    EQN_T* cs = parse_float_array(pathC);
    
    int i;
    EQN_T** pairs = (EQN_T **) malloc(sizeof(EQN_T *) * LEN);
    
    for (i = 0; i < LEN; ++i)
    {
        pairs[i] = (EQN_T *) malloc(sizeof(EQN_T) * 4);
        
        fscanf(fileA, SCANF_FORMAT, &pairs[i][0], &pairs[i][1]);
        pairs[i][2] = cs[i];
        pairs[i][3] = 1;
    }
    
    file_close(fileA);
    free(cs);
    
    return pairs;
}
 
 /**
  * Reads a file into an equation array. The first file is expected to have two
  * numbers on each line delimited by a singular space, the other file is
  * expected to have an isolated number on each line.
  *
  * @param pathA
  *         Path to the file to read containing the coefficients of x and y.
  * @param pathC
  *         Path to the file to read containing the constant solutions to the
  *         relational equations.
  * @return
  *         Pinter to the equation array.
  */
equation_t** parse_eqns(char *pathA, char *pathC)
{
    EQN_T** d = parse_eqns_numerical(pathA, pathC);
    equation_t** pairs = (equation_t **) malloc(sizeof(equation_t *) * LEN);
    
    int i;
    for (i = 0; i < LEN; ++i)
    {
        pairs[i] = new_equation(d[i][0], d[i][1], d[i][2], d[i][3]);
    }
    
    return pairs;
}

/**
 *  This program takes one argument, which is the path to a directory containing
 *  the files 'A', 'c', and 'result.' It reads the files 'A' and 'c', creates
 *  equation objects from them, and solves the set of relational equations
 *  specified by them using Fourier-Motzkin elimination. If a solution is found
 *  it prints the interval within which x can be.
 */
int main(int argc, char** argv)
{
    if (argc < 2)
    {
        char msg[BUF_SIZ];
        sprintf(msg, "Usage: ./%s <FOLDER_PATH> [version]", argv[0]);
        error(msg);
    }
    
    char A[BUF_SIZ], c[BUF_SIZ], result[BUF_SIZ];
    
    strcpy(A, argv[1]);
    strcpy(c, argv[1]);
    strcpy(result, argv[1]);
    
    concat(A, "/A");
    concat(c, "/c");
    concat(result, "/result");
    
    if (argc >= 3) {
        EQN_T** eqns = parse_eqns_numerical(A, c);

        if (!strcmp(NAME_FAST, argv[2])) {
            zmk_fast(eqns);
        } else if (!strcmp(NAME_SMALL, argv[2])) {
            zmk_small(eqns);
        }
    } else {
        equation_t** eqns = parse_eqns(A, c);
        zmk_fm(eqns);
        int i;
        
        for (i = 0; i < LEN; ++i)
        {
            free_equation(eqns[i]);
        }
        free(eqns);
    }
    
    return 0;
}

#endif