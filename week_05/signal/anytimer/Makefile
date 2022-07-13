OBJS = main.o anytimer.o
CFLAGS += -Wall
CC = gcc

myanytimer: $(OBJS)
	$(CC) $^ -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) $^ -o $@

clean:
	rm -rf myanytimer *.o