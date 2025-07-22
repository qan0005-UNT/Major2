CC = gcc
CFLAGS = -Wall -g
OBJS = main.o cd.o code_exit.o path.o myhistory.o alias.o pipeline.o signal_control.o IO_redirection.o

major2: $(OBJS)
	$(CC) $(CFLAGS) -o major2 $(OBJS)

main.o: main.c cd.h code_exit.h path.h myhistory.h alias.h pipeline.h signal_control.h IO_redirection.h
cd.o: cd.c cd.h
code_exit.o: code_exit.c code_exit.h
path.o: path.c path.h
myhistory.o: myhistory.c myhistory.h
alias.o: alias.c alias.h
pipeline.o: pipeline.c pipeline.h
signal_control.o: signal_control.c signal_control.h
IO_redirection.o: IO_redirection.c IO_redirection.h

clean:
	rm -f *.o major2
