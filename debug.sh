#!/bin/sh

C_FILE=$1
OUT_FILE=${C_FILE%.c}
INPUT="$2 $3"

# && valgrind --leak-check=yes
gcc ${C_FILE} -o ${OUT_FILE} ./${OUT_FILE} ${INPUT}