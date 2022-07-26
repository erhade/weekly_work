OBJS = main.o relayer.o
CC = gcc
CFLAGS += -Wall -pthread
LDLIBS += -pthread

myrelayer: $(OBJS)
	$(CC) -o $@ $^ $(LDLIBS)

%.o: %.c
	$(CC) -c $(CFLAGS) $^ -o $@

clean:
	rm -rf myrelayer *.o