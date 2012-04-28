example : pcqueue.o example.o
	gcc pcqueue.o example.o -o example

pcqueue.o : pcqueue.c pcqueue.h
	gcc -c pcqueue.c

example.o : example.c
	gcc -c example.c

clean:
	rm -rf *o example
