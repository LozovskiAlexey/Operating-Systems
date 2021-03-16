obj-m := my_mouse_driver.o

my_mouse_driver-objs += mouse_control_funcs.o
my_mouse_driver-objs += mouse_driver.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

