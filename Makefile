CC = gcc
CFLAGS = -Wall -Wextra -std=c11

SRC = rod_cutting.c cache.c main.c

all: rod_cutting_A rod_cutting_B

rod_cutting_A: $(SRC) rod_cutting.h cache.h
	$(CC) $(CFLAGS) -o rod_cutting_A $(SRC) -D POLICY_A

rod_cutting_B: $(SRC) rod_cutting.h cache.h
	$(CC) $(CFLAGS) -o rod_cutting_B $(SRC) -D POLICY_B

clean:
	rm -f rod_cutting_A rod_cutting_B