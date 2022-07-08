OBJS = Ship.o Astroid.o Bullet.o
FILES = *.cpp
CFLAG = -Wall -g -pthread
CC = g++
INCLUDE =
LIBS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network

build: ${OBJ}
	${CC} -c ${FILES}

cli: cli
	${CC} ${CFLAG} -o client cli.o ${OBJS} ${LIBS}
srv: srv
	${CC} ${CFLAG} -o server srv.o ${OBJS} ${LIBS}


clean:
	-rm -f *.o core *.core


