OBJS = main.o cal.o
CC = gcc
CFLAGS += -Wall -g

my_cal: $(OBJS)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) -o $@ -c $(CFLAGS) $^

clean:
	$(RM) -f my_cal *.o