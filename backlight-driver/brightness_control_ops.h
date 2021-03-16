#include <linux/string.h>
#include "kernel_space_fops.h"

#define BACKLIGHT_PATH "/sys/class/backlight/intel_backlight/brightness"
#define MAX_BACKLIGHT_PATH "/sys/class/backlight/intel_backlight/max_brightness"

#define MAX_BRINGHTNESS_CLASS 4
#define BRIGHTNESS_STEPS      10


struct backlight_data{
	int max_brightness;       // максимальное значение яркости
	int tmp_brightness;       // текущее значение яркости
	int brightness_step;      // шаг с которым будет изменяться яркость
};


static struct backlight_data *data;
static char brightness_str[MAX_BRINGHTNESS_CLASS];


// ================== операции над яркостью ===================

int backlight_read_value(struct file *file);
void backlight_init(void);
void brightness_update(void);
void backlight_change(int flag);
//void backlight_increase(void);
//void backlight_decrease(void);
void brightness_exit(void);