CC = gcc
CFLAGS = -Wall -ansi -pedantic `sdl-config --cflags`
BIN = spycmd
OBJS = spylib.o main.o serport.o nqc.o input.o
LIBS = `sdl-config --libs`

all: ${BIN}

${BIN}: ${OBJS}
	${CC} -o $@ ${OBJS} ${LIBS}

.c.o:
	${CC} -c ${CFLAGS} -o $@ $<

clean:
	rm -f ${BIN} *.o
