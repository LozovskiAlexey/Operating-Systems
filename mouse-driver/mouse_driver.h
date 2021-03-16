#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include "mouse_control_funcs.h"


struct notifier_block nb;
struct input_dev *virtual_mouse;
