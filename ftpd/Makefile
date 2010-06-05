CC		:= gcc
CCFLAGS	:= -O0 -Wall 

OBJS	:= main.o ../common/socket_helper.o ../common/debug.o ftp_session.o ../common/def.o ../common/string_helper.o

all: ftpd

ftpd: ${OBJS}
	${CC} ${CCFLAGS} -o$@ $^

${OBJS}: %.o: %.c
	${CC} ${CCFLAGS} -c -o$@ $^

clean:
	rm *.o ftpd 2> /dev/null
	rm ../common/*.o 2> /dev/null
