CFLAGS	= -g -Wall -Wextra -Werror -std=c99

CC	= gcc
OUT	= fm
OBJS	= main.o fast.o small.o

all: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(OUT)
	./fm 1

clean:
	rm -f $(OUT) $(OBJS) *.gcda small fast
