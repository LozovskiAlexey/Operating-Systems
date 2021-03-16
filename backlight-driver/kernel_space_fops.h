#include <linux/fs.h>
#include <asm/segment.h>
#include <asm/uaccess.h>
#include <linux/slab.h>


// =================== работа с файлом из пространства ядра ===================
static struct file *backlight_file;

struct file *file_open(const char *path, int flags, int rights);
int file_read(struct file *file, unsigned long long offset, unsigned char *data, unsigned int size);
int file_write(struct file *file, unsigned long long offset, unsigned char *data, unsigned int size);
void file_close(struct file *file);
