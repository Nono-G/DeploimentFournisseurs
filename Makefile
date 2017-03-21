CC=gcc

CCFLAGS= -Wall -I./inc -g3 -std=c11
LDFLAGS=

SRC=$(wildcard src/*.c)
OBJ=$(SRC:.c=.o)

EXEC=deploiement

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $(EXEC) $^ $(LDFLAGS)

src/lp.o: src/lp.c
	$(CC) -lglpk $(CCFLAGS) -c $< -o $@ 

%.o: %.c
	$(CC) $(CCFLAGS) -o $@ -c $<

clean:
	rm -fv src/*.o
	rm -fv $(EXEC)
