OBJS = main.o semaphore.o 
CFLAGS += -pthread -Wall
LDLIBS += -pthread -lm
CC = gcc

mysema: $(OBJS)
	$(CC) $^ $(LDLIBS) -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) $^ -o $@

clean:
	rm -rf mysema *.o