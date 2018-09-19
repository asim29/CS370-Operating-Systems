CC=gcc
CFLAGS=-I.

%.o: %.c 
	$(CC) -c -o $@ $< $(CFLAGS)

simple-shell: simple-shell.o 
	$(CC) -o simple-shell simple-shell.o 

clean:
	rm *.o simple-shell

