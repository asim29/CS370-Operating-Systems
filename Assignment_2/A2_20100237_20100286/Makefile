# CC=gcc
# CFLAGS=-I.

# %.o: %.c 
# 	$(CC) -c -o $@ $< $(CFLAGS)

# simple-shell: simple-shell.o 
# 	$(CC) -o simple-shell simple-shell.o 

# clean:
# 	rm *.o simple-shell

obj-m += listTasks.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
