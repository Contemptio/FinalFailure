#ifndef UTIL_C
#define UTIL_C

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EQN_T  short
#define LEN    (5)
#define A_POS  (0)
#define B_POS  (1)
#define C_POS  (2)
#define LT_POS (3)

#define BUF_SIZ (50)

void file_close(FILE *file);
FILE* file_open(char *path);
void println(char *msg);
void swap(void *a, void *b);

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
    char c[BUF_SIZ];
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
    println(msg);
    exit(1);
}

/**
 *  Closes a file.
 *
 *  @param file
 *          The file to close.
 */
void file_close(FILE *file)
{
    fclose(file);
}
 
/**
 *  Closes a file.
 *
 *  @param path
 *          Path to the file to open.
 *  @return
 *          A pointer to the file object.
 */
FILE* file_open(char *path)
{
    FILE *file;
    if ((file = fopen(path, "r")) == NULL)
    {
        error("Error reading file.");
    }
    return file;
}

/**
 *  Frees an EQN_T matrix.
 *
 *  @param mat
 *          The matrix to free.
 *  @param rows
 *          The number of rows in the matrix.
 */
void free_eqn_matrix(EQN_T** mat, size_t rows)
{
    size_t i;
    for (i = 0; i < rows; ++i)
    {
        free(mat[i]);
    }
    free(mat);
}

/**
 *  Prints an EQN_T array.
 *
 *  @param eqn
 *          The EQN_T array to print.
 */
void print_eqn_array(EQN_T* eqn)
{
    printf("\t%.02fx + %.02fy %c%c %.02f\n", eqn[A_POS],
        eqn[B_POS], (eqn[LT_POS] ? '<' : '>'), '=', eqn[C_POS]);
}

/**
 *  Prints a matrix of equations.
 *
 *  @param eqns
 *          The equations to print.
 */
void print_eqn_matrix(EQN_T** eqns)
{
    int i;
    char* pre = "";
    printf("[\n");
    for (i = 0; i < LEN; ++i)
    {
        print_eqn_array(eqns[i]);
    }
    println("]");
}

/**
 *  Prints a string and starts at the next line.
 *
 *  @param msg
 *          The message to print.
 */
void println(char *msg)
{
    printf("%s\n", msg);
}

/**
 *  Switches the direction of two pointers.
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

#endif