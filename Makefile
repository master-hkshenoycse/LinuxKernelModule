obj-m += lkm1.o 
obj-m += lkm2.o 
obj-m += lkm3.o 
obj-m += lkm5.o 
obj-m += lkm4.o

all:
	make -C /lib/modules/`uname -r`/build M=$(PWD) modules

clean:
	make -C /lib/modules/`uname -r`/build M=$(PWD) clean
