# Do not edit the contents of this file.
CC = gcc
CFLAGS = -Werror -Wall -g -std=gnu99
LDFLAGS = -lrt -lpthread
TARGET = a04
OBJFILES = a04.o 
#all: $(TARGET)
Question1: a04.c
	$(CC) $(CFLAGS) -o a04 a04.c $(LDFLAGS)

runq1: Question1
	./a04 sample4_in.txt

clean:
	rm -f $(OBJFILES) $(TARGET) *~ 
