ifneq ($(KERNELRELEASE),)

obj-m := hello.o

else
KDIR:=/lib/modules/`uname -r`/build

default:
	make -C $(KDIR) M=$(PWD) modules
install:
	insmod hello.ko
uninstall:
	rmmod hello

clean:
	rm -f *.ko *.o *.mod.o *.mod.c *.order *.symvers

endif
