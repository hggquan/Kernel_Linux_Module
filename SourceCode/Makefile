KDIR = /lib/modules/`uname -r`/build
all:
	make -C $(KDIR) M=`pwd`
	$(CC) Test.c -o Test
clean:
	make -C $(KDIR) M=`pwd` clean
	rm Test
