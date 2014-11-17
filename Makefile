ifneq ($(KERNELRELEASE),)
	obj-m := hello.o
else
	KERNELDIR := /home/finlay/raspberry_pi/linux
    CCPREFIX := /home/finlay/tools/arm-bcm2708/arm-bcm2708-linux-gnueabi/bin/arm-bcm2708-linux-gnueabi-
	PWD :=$(shell pwd)
default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules ARCH=arm CROSS_COMPILE=${CCPREFIX}
clean:
	rm -rf *.ko *.o *.mod.o *.mod.c *.symvers modul* .h* .b* .M* .t*
endif

#obj-m := helloworld.o
#helloworld-objs := helloworld_main.o
#PWD :=$(shell pwd)
#make -C /lib/modules/3.12.22+/build SUBDIRS=$(PWD) modules
