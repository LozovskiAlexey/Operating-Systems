#include "mouse_control_funcs.h"



void move_pointer_left(struct input_dev *device){
	input_report_rel(device, REL_X, -MOVEMENT_STEP); /* работаем с относительными координатами */
}

void move_pointer_right(struct input_dev *device){
	input_report_rel(device, REL_X, MOVEMENT_STEP);
}

void move_pointer_up(struct input_dev *device){
	input_report_rel(device, REL_Y, -MOVEMENT_STEP);
}

void move_pointer_down(struct input_dev *device){
	input_report_rel(device, REL_Y, MOVEMENT_STEP);
}

void left_click(struct input_dev *device,int is_pressed){
	input_report_key(device, BTN_LEFT, is_pressed);   /* нажатие ЛКМ */
}

void right_click(struct input_dev *device, int is_pressed){
	input_report_key(device, BTN_RIGHT, is_pressed);  /* нажатие ПКМ */
}
