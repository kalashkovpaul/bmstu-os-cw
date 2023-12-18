obj-m = file_protection.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

modify: modify.c
	gcc modify.c -o modify

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
