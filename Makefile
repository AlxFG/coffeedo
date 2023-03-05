CC=clang
CFLAGS=
LDLIBS=
build:
	${CC} ${CFLAGS} src/main.c src/cof3.c -o coffeedo ${LDLIBS} -Wall
