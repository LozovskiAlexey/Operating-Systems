#include <linux/module.h>     // для работы с модулем
#include <linux/init.h>
#include <linux/interrupt.h>  // для прерываний и тасклета
#include <asm/io.h>           // для работы с портом ввода-вывода
#include <linux/string.h>     // функции memset, memccpy и т.д.

#define KB_IRQ              1
#define SHIFT_PR            54 
#define SHIFT_REL           182 
#define INCR_BRIGHTNESS_PR  48      // b pressed 
#define DECR_BRIGHTNESS_PR  47      // v pressed
#define KBD_SCANCODE_MASK   0x7f
#define KBD_STATUS_MASK     0x80


// =================== Обработка сигнала с клавиатуры ===================

struct keyboard_data{
    unsigned char scancode;
} kb_data;

irq_handler_t kb_irq_handler(int irq, void *dev_id, struct pt_regs *regs);

// =================== работа тасклета ===================

void tasklet_brightness_controller(unsigned long info);



