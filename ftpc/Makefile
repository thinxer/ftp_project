CC		:= gcc
CCFLAGS	:= -O0 -Wall 

OBJS	:= main.o ../common/socket_helper.o ../common/debug.o ../common/def.o

all: ftpc

ftpc: ${OBJS}
	${CC} ${CCFLAGS} -o$@ $^

${OBJS}: %.o: %.c
	${CC} ${CCFLAGS} -c -o$@ $^

clean:
	rm *.o ftpc 2> /dev/null
	rm ../common/*.o 2> /dev/null
