CC      = gcc
CFLAGS  = -std=c11 -Wall -Wextra -g
OBJS    = main.o parser.o rodcut.o

all: rodcutA rodcutB

rodcutA: $(OBJS) policy_a.o
	$(CC) $(CFLAGS) -o $@ $(OBJS) policy_a.o

rodcutB: $(OBJS) policy_b.o
	$(CC) $(CFLAGS) -o $@ $(OBJS) policy_b.o

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o rodcutA rodcutB