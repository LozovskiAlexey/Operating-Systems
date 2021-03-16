#include "brightness_control_ops.h"


// ================== операции над яркостью ===================

// Считать значение из файла
int backlight_read_value(struct file *file){
	
	int brightness_int = 0;
	
	file_read((struct file *) file, 0, brightness_str, MAX_BRINGHTNESS_CLASS);
	brightness_str[MAX_BRINGHTNESS_CLASS-1] = '\0';
	kstrtoint(brightness_str, 10, &brightness_int);
	
	memset(brightness_str, 0, sizeof(brightness_str));
	
	return brightness_int;
}

// инициализация структуры backlight_data значениями из файлов brightness и max_brightness
void backlight_init(void){
	// GFP_KERNEL - выделение производится от имени процесса, запущенного в пространстве ядра.
	data = kmalloc(sizeof(struct backlight_data), GFP_KERNEL);
	memset(brightness_str, 0, sizeof(brightness_str));
	
	// инициализация текущей яркости
	backlight_file = file_open(BACKLIGHT_PATH, O_RDWR, 0);
	data->tmp_brightness = backlight_read_value(backlight_file);
	// file_close((struct file *) backlight_file);
	printk(KERN_INFO "brightness_control_ops.c: Tmp_brightness was initialized with %d!\n", data->tmp_brightness);
	
	// инициализация максимальной яркости
	struct file *max_backlight_file = file_open(MAX_BACKLIGHT_PATH, O_RDONLY, 0);
	data->max_brightness = backlight_read_value(max_backlight_file);
	file_close((struct file *) max_backlight_file);
	printk(KERN_INFO "brightness_control_ops.c: Max_brightness was initialized with %d!\n", data->max_brightness);
	
	// вычислить шаг
	data->brightness_step = data->max_brightness / BRIGHTNESS_STEPS;
	printk(KERN_INFO "brightness_control_ops.c: Brightness_step was initialized with %d!\n", data->brightness_step);
	
	printk(KERN_INFO "brightness_control_ops.c: Backlight structure was initialized!\n");
}

// обновление содержимого файла brightness (текущей яркости)
void brightness_update(void){

	// struct file *backlight_file = file_open(BACKLIGHT_PATH, O_WRONLY, 0);
	
	snprintf(brightness_str, MAX_BRINGHTNESS_CLASS, "%d", data->tmp_brightness);
	file_write(backlight_file, 0, brightness_str, MAX_BRINGHTNESS_CLASS);
	memset(brightness_str, 0, sizeof(brightness_str));
	
	// file_close((struct file *) backlight_file);
	
	printk(KERN_INFO "brightness_control_ops.c: Backlight tmp_brightness was updated!\n");
}

void backlight_change(int flag){
	
	if (flag == 0) return;
	
	if (flag == 1){
		data->tmp_brightness += data->brightness_step;
		if (data->tmp_brightness  > data->max_brightness) data->tmp_brightness = data->max_brightness;
		printk(KERN_INFO "brightness_control_ops.c: Backlight tmp_brightness was increased!\n");
	} 
	else if (flag == -1){
		data->tmp_brightness -= data->brightness_step;
		if (data->tmp_brightness <= 8) data->tmp_brightness = 8;
		printk(KERN_INFO "brightness_control_ops.c: Backlight tmp_brightness was decreased!\n");
	}
	
	brightness_update();
}

// увеличение яркости
// void backlight_increase(void){
//	
//	data->tmp_brightness += data->brightness_step;
//	if (data->tmp_brightness  > data->max_brightness) data->tmp_brightness = data->max_brightness;
//	printk(KERN_INFO "brightness_control_ops.c: Backlight tmp_brightness was increased!\n");
	
//	brightness_update();	
//}

// уменьшение яркости
//void backlight_decrease(void){
	
//	data->tmp_brightness -= data->brightness_step;
//	if (data->tmp_brightness  <= 0) data->tmp_brightness = 1;
//	printk(KERN_INFO "brightness_control_ops.c: Backlight tmp_brightness was decreased!\n");
	
//	brightness_update();
//}


void brightness_exit(void){
	kfree(data);
	
	printk(KERN_INFO "brightness_control_ops.c: Backlight structure was uninitialized!\n");
}