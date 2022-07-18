OBJS = main.o mytbf.o 
CFLAGS += -pthread -Wall
LDLIBS += -pthread
CC = gcc

mytbf: $(OBJS)
	$(CC) $^ $(LDLIBS) -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) $^ -o $@

clean:
	rm -rf mytbf *.o