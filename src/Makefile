CC = gcc
override CFLAGS += -I../include

SOURCES = main.c \
		  gpio.c \
		  lcd.c

OBJ = $(SOURCES:c=o)

all: $(OBJ)
	$(CC) $^ -o ../reaction $(CFLAGS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	-rm -f *.o
	-rm -f ../reaction
