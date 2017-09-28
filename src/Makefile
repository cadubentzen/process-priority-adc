CC = gcc
override CFLAGS += -I../include

SOURCES = main.c \
		  gpio.c

OBJ = $(SOURCES:c=o)

all: $(OBJ)
	$(CC) $^ -o ../priority $(CFLAGS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	-rm -f *.o
	-rm -f ../priority
