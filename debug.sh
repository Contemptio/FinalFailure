#!/bin/sh

C_FILE=$1
OUT_FILE=${C_FILE%.c}.exe
INPUT="$2 $3"
VAL=$4

rm ${OUT_FILE}
gcc ${C_FILE} -o ${OUT_FILE}
if [ "$4" != "noval" ]; then
	valgrind --leak-check=yes ./${OUT_FILE} ${INPUT}
fi
