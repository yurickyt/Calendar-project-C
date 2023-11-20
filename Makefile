CC = gcc
CFLAGS = -ansi -Wall -g -O0 -Wwrite-strings -Wshadow \
		 -pedantic-errors -fstack-protector-all -Wextra
PROGS = public01 public02 public03 public04 public05 student_tests

.PHONY: all clean

all: $(PROGS)

public01: calendar.o public01.o my_memory_checker_216.o
	$(CC) calendar.o public01.o my_memory_checker_216.o -o public01

public02: calendar.o public02.o my_memory_checker_216.o
	$(CC) calendar.o public02.o my_memory_checker_216.o -o public02

public03: calendar.o public03.o my_memory_checker_216.o
	$(CC) calendar.o public03.o my_memory_checker_216.o -o public03

public04: calendar.o public04.o my_memory_checker_216.o
	$(CC) calendar.o public04.o my_memory_checker_216.o -o public04

public05: calendar.o public05.o my_memory_checker_216.o
	$(CC) calendar.o public05.o my_memory_checker_216.o -o public05

student_tests: calendar.o student_tests.o my_memory_checker_216.o
	$(CC) calendar.o student_tests.o my_memory_checker_216.o -o student_tests

public01.o: public01.c event.h calendar.h
	$(CC) $(CFLAGS) -c public01.c

public02.o: public02.c event.h calendar.h
	$(CC) $(CFLAGS) -c public02.c

public03.o: public03.c event.h calendar.h
	$(CC) $(CFLAGS) -c public03.c

public04.o: public04.c event.h calendar.h
	$(CC) $(CFLAGS) -c public04.c

public05.o: public05.c event.h calendar.h
	$(CC) $(CFLAGS) -c public05.c

student_tests.o: student_tests.c event.h calendar.h	
	$(CC) $(CFLAGS) -c student_tests.c

calendar.o: calendar.c calendar.h
	$(CC) $(CFLAGS) -c calendar.c -o calendar.o

my_memory_checker_216.o: my_memory_checker_216.c my_memory_checker_216.h
	$(CC) $(CFLAGS) -c my_memory_checker_216.c -o my_memory_checker_216.o

clean:
	rm -f *.o $(PROGS) a.out