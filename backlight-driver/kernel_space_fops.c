#include "kernel_space_fops.h"

// =================== работа с файлом из пространства ядра ===================

struct file *file_open(const char *path, int flags, int rights) {
	struct file *fp = NULL;
	mm_segment_t old_fs;
	int error = 0;

	/* Save current process address limit. */
	old_fs = get_fs();
	/* Set current process address limit to that of the kernel, allowing
 	 * the system call to access kernel memory.
	 */ 
	set_fs(KERNEL_DS);
	fp = filp_open(path, flags, rights);
	/* Restore address limit to current process. */
	set_fs(old_fs);

	if(IS_ERR(fp)){
		error = PTR_ERR(fp);
		printk("tmp brightness: %d", error);
		return NULL;
	}

	return fp;
}


int file_read(struct file *fp, unsigned long long offset, unsigned char *data, unsigned int size) {
    mm_segment_t oldfs;
    int ret;
	
    oldfs = get_fs();
    set_fs(KERNEL_DS);
	
    ret = vfs_read(fp, data, size, &offset);
	
    set_fs(oldfs);
    return ret;
}


int file_write(struct file *fp, unsigned long long offset, unsigned char *data, unsigned int size) {
	
	mm_segment_t old_fs;
	int ret;

	old_fs = get_fs();
	set_fs(KERNEL_DS);

	ret = vfs_write(fp, data, size, &offset);
	vfs_fsync(fp, 0);

	set_fs(old_fs);
	return ret;
}


void file_close(struct file *file) {
    filp_close(file, NULL);
}

