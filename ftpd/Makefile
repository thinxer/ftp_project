CC		:= gcc
CCFLAGS	:= -O0 -Wall 

all: ftpd

ftpd: main.c socket_helper.c debug.c ftp_session.c
	${CC} ${CCFLAGS} -o$@ $^

clean:
	rm *.o ftpd 2>nul
