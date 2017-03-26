CC = gcc

CCFLAGS = -Wall -I./inc -g3 -std=c11
LDFLAGS = -lglpk

SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)

EXEC = deploiement

.PHONY: all rebuild clean

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(LDFLAGS) -o $(EXEC) $^

%.o: %.c
	$(CC) $(CCFLAGS) -o $@ -c $<

rebuild:
	make clean
	make

clean:
	rm -fv src/*.o
	rm -fv $(EXEC)
