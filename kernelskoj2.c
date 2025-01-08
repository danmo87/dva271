
#include <linux/init.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>

/*
Use a Makefile containing:

CONFIG_MODULE_SIG=n
obj-m += kernelskoj2.o
PWD := $(CURDIR)
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

 ##############################################################

Load the module with 
sudo insmod kernelskoj2.ko 

Unload the module with 
sudo rmmod kernelskoj2.ko

Try reading with "cat /proc/arduinouno"
Try writing with  "echo 'hello' >> /proc/arduinouno"

Read the kernel module output with
sudo dmesg | tail
*/

#define MESSAGE_LENGTH 5
static struct proc_dir_entry* proc_entry;
#define PROCFS_MAX_SIZE     20
static char read_proc_buffer[PROCFS_MAX_SIZE];
static unsigned long read_proc_buffer_size = 0;


static ssize_t read_proc(struct file* file, char __user* user_buffer, size_t count, loff_t* offset)
{
  /* Reads from buffer (same as proc_write writes to) */
printk(KERN_INFO "Called read_proc\n");

if (*offset > 0 || count < PROCFS_MAX_SIZE){return 0;}

/* Start of Arduino Read thingy
 struct file* testfd = filp_open("/dev/ttyACM1",O_RDWR,0);
 ....
 ssize_t rdr = kernel_read(testfd, &buf, sizeof(buf),NULL);
 ....
 filp_close(testfd, NULL);
 End of Arduino thingy*/

if(copy_to_user(user_buffer, read_proc_buffer, read_proc_buffer_size)){return -EFAULT;}
*offset = read_proc_buffer_size;

return read_proc_buffer_size;
}


static ssize_t write_proc(struct file* file, const char __user* user_buffer, size_t count, loff_t* offset)
{
printk(KERN_INFO "Called write_proc\n");

int tmp_len;
char *tmp = kzalloc((count+1),GFP_KERNEL);

if (!tmp){return -ENOMEM;}

if(copy_from_user(tmp,user_buffer,count))
{
  kfree(tmp);
  return EFAULT;
}

/* Start of Arduino Write thingy
 struct file* testfd = filp_open("/dev/ttyACM1",O_RDWR,0);
 ....
 ssize_t wr = kernel_write(....);
 ....
 filp_close(testfd, NULL);
 End of Arduino Write thingy*/

tmp_len = PROCFS_MAX_SIZE;
if (count > PROCFS_MAX_SIZE) {tmp_len = count;}

memcpy(&read_proc_buffer, tmp, tmp_len);
printk(KERN_INFO "write_proc: %s\n", read_proc_buffer);
read_proc_buffer_size = tmp_len;
kfree(tmp);
return tmp_len;
}



static const struct proc_ops hello_proc_fops = {
  .proc_read = read_proc, 
  .proc_write = write_proc,
};
 
static int __init construct(void) {
  proc_entry = proc_create("arduinouno", 0666, NULL, &hello_proc_fops);
  pr_info("Loading 'arduinouno' module!\n");
return 0;
}



static void __exit destruct(void) {
  proc_remove(proc_entry);
  pr_info("First kernel module has been removed\n");
}



module_init(construct);
module_exit(destruct);

MODULE_LICENSE("GPL");

