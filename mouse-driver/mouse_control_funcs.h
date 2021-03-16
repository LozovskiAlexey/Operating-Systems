#include <linux/input.h>
#include <linux/keyboard.h>
#include <linux/module.h>
#include <linux/init.h>


#define MOVEMENT_STEP 4


void move_pointer_left(struct input_dev *device);
void move_pointer_right(struct input_dev *device);
void move_pointer_up(struct input_dev *device);
void move_pointer_down(struct input_dev *device);
void left_click(struct input_dev *device, int is_pressed);
void right_click(struct input_dev *device, int is_pressed);
