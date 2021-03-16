#include "brightness_keyboard_controller.h"
#include "brightness_control_ops.h"

MODULE_DESCRIPTION("Brightness module, allows to controll brightness by using keyboard.");
MODULE_LICENSE("GPL");


// =================== работа тасклета ===================

// Логгирует событие изменения яркости
// Изменяет яркость
void tasklet_brightness_controller(unsigned long info)
{
    static int shift = 0;
	static int bright_flag = 0; // 0 - не изменять, 1 - повысить, -1 - понизить
	
    switch(kb_data.scancode){
	
		// если нажали шифт - установили флажок
		case SHIFT_PR: shift = 1; break;
		// если отпустили - сняли флажок
        case SHIFT_REL: shift = 0; break;
		// если нажали к шифту b - установить флаг повышения яркости
		case INCR_BRIGHTNESS_PR: 
			if (shift) bright_flag = 1;
			break; 
		// если нажали к шифту v - установить флаг понижения яркости
		case DECR_BRIGHTNESS_PR:
			if (shift) bright_flag = -1;
			break;
			
		default: 
			break;	
    }
    
	backlight_change(bright_flag);
	bright_flag = 0;
}

DEFINE_SPINLOCK(k_lock);
DECLARE_TASKLET(my_tasklet, tasklet_brightness_controller, 0);


// =================== Обработка сигнала с клавиатуры ===================

// обработчик прерывания, читает информацию о нажатой клавише через порт 0х60 
// информация представлена в восьмибитном формате, где 7ой бит 
// 0 если клавиша отпущена, 1 иначе.
irq_handler_t kb_irq_handler(int irq, void *dev_id, struct pt_regs *regs)
{
	spin_lock(&k_lock);
    kb_data.scancode = inb(0x60);
	spin_unlock(&k_lock);

	// помещаем наш тасклет в очередь на выполнение в отдельном потоке,
	// чтобы прерывание выполнялось без задержек
    tasklet_schedule(&my_tasklet);
	
    return (irq_handler_t)IRQ_HANDLED;
}
	

int init_module(void)
{
    int ret;
	
    printk(KERN_INFO "brightness_controller.c: initializing...");

	// регистрируем обработчик прерывания
    ret = request_irq(KB_IRQ, (irq_handler_t)kb_irq_handler, IRQF_SHARED, "custom handler", &data);
    if(ret != 0) printk(KERN_INFO "brightness_controller.c: Cannot request IRQ for keyboard.\n");
	
	// инициализируем структуру для работы с яркостью
	backlight_init();
    
    printk(KERN_INFO "brightness_controller.c: initialization complete.");
	
	

    return ret;
}


void cleanup_module(void) 
{
    tasklet_kill(&my_tasklet);

    free_irq(KB_IRQ, &data);

    printk(KERN_INFO "brightness_controller.c: unloaded.");
}




