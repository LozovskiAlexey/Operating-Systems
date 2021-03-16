#include "mouse_driver.h"
#include "mouse_control_funcs.h"


MODULE_DESCRIPTION("Virtual mouse driver. Imitates mouse behavior by using keyboard.");
MODULE_AUTHOR("Alex Lozovski IU7-84B");
MODULE_LICENSE("GPL"); 


// Функция-обработчик для notifier_block
int kb_notifier_fn(struct notifier_block *tom, unsigned long action, void* data){
	
	struct keyboard_notifier_param *kp = (struct keyboard_notifier_param*)data;	
	int value = kp->value;               /* сканкод нажатой клавиши */
	static bool shift_pressed = 0;       /* флаг, отвечающий за нажатие клавиш shift */
	
	
	switch (value){
		case KEY_RIGHTSHIFT:                      /* Если нажата right-shift или left-shift */
		case KEY_LEFTSHIFT: 
			if (kp->down) {
				shift_pressed = 1;    /* Установить флаг */
				printk("mouse_driver.c: shift was pressed!\n");
			}else{
				shift_pressed = 0;   /* Снять флаг */
				printk("mouse_driver.c: shift was released!\n");
			}
			break;
														
			
		case KEY_UP: move_pointer_up(virtual_mouse);    /* Если нажата клавиша KEY_UP - сдвинуть курсор вверх*/
			printk("mouse_driver.c: KEY_UP was pressed!\n");
			break;
			
		case KEY_DOWN: move_pointer_down(virtual_mouse); /* Если нажата клавиша KEY_DOWN - сдвинуть курсор вниз*/
			printk("mouse_driver.c: KEY_DOWN was pressed!\n");
			break;
			
		case KEY_RIGHT:
			if (shift_pressed) right_click(virtual_mouse, kp->down); /* Если нажата клавиша KEY_RIGHT и SHIFT- нажатие правой кнопки мыши*/
			else move_pointer_right(virtual_mouse);                  /* Если только KEY_RIGHT - сдвиг курсора вправо */
			printk("mouse_driver.c: KEY_RIGHT was pressed!\n");
			break;
			
		case KEY_LEFT:
			if (shift_pressed) left_click(virtual_mouse, kp->down); /* Если нажата клавиша KEY_LEFT и SHIFT- нажатие левой кнопки мыши*/
			else move_pointer_left(virtual_mouse);                  /* Если только KEY_LEFT - сдвиг курсора влево */
			printk("mouse_driver.c: KEY_LEFT was pressed!\n");
			break;
	}
	
	
	input_sync(virtual_mouse);  /* используется для разделения и объединения событий REL_X, REL_Y*/
	return 0;
}


int init_module(void)
{	
	int error;
	
	// регистрация notifier_block
	nb.notifier_call = kb_notifier_fn;
	register_keyboard_notifier(&nb);

	// выделение памяти для виртуальной мыши
	virtual_mouse = input_allocate_device();
	if (!virtual_mouse) {
		printk(KERN_ERR "mouse_driver.c: Not enough memory\n");
        error = -ENOMEM;
		goto err_free_notifier_block;
	}
		
	virtual_mouse->name = "virtual_mouse";
	
	// установка сигналов, которые может подавать устройство 
	set_bit(EV_REL, virtual_mouse->evbit);   /* устройство поддерживает события EV_REL */
	set_bit(REL_X, virtual_mouse->relbit);   /* устанавливаем поддержку работы с относительными координатами оси Х */
	set_bit(REL_Y, virtual_mouse->relbit);	 /* устанавливаем поддержку работы с относительными координатами оси Y */
	set_bit(EV_KEY, virtual_mouse->evbit);   /* устройство поддерживает события EV_KEY */
	set_bit(BTN_LEFT, virtual_mouse->keybit);    /* устанавливаем поддержку кнопок устройства ЛКМ */
	set_bit(BTN_RIGHT, virtual_mouse->keybit);   /* устанавливаем поддержку кнопок устройства ПКМ */
	
	// регистрация виртуального устройства
	error = input_register_device(virtual_mouse);
	if (error) {
		printk(KERN_ERR "mouse_driver.c: Failed to register device\n");
		goto err_free_dev;
    }
	
	// успешное завершение регистрации устройства
	printk("mouse_driver.c: device was successfully registered!\n");
	return 0;
	
	// ошибка регистрации устройства
	err_free_dev:
        input_free_device(virtual_mouse);
	// ошибка выделения памяти под структуру struct input_device
	err_free_notifier_block:
        unregister_keyboard_notifier(&nb);
        return error;
}


void cleanup_module(void) 
{
	unregister_keyboard_notifier(&nb);
	input_unregister_device(virtual_mouse);
	printk("mouse_driver.c: device was successfully unregistered!\n");
}

