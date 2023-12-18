#include <linux/module.h>
#include <linux/kernel.h>
// #include <linux/init.h>
#include <linux/syscalls.h> // kfree
#include <linux/unistd.h>
// #include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/namei.h>
#include <asm/string.h>
// #include <linux/kallsyms.h>
// #include <linux/version.h>
// #include <linux/fs.h>

// #include <asm/cacheflush.h>

#define PROTECTED_FILE "/home/paul/Desktop/bmstu-os-cw/src/protected"
MODULE_LICENSE ("GPL");

struct inode* protected_inode;
struct file_operations* original_fop;
struct file_operations* new_fop;
struct inode_operations* original_iop;
struct inode_operations* new_iop;
unsigned int initial_dentry_flags;
unsigned int initial_inode_flags;
int permissions;

inline void write_cr0_custom(unsigned long cr0) 
{
    unsigned long __force_order;
    asm volatile("mov %0,%%cr0" : "+r"(cr0), "+m"(__force_order));
}

void disable_write_protection(void)
{
    write_cr0_custom(read_cr0() & (~0x10000));
}

void enable_write_protection(void)
{
    write_cr0_custom(read_cr0() | 0x10000);
}

struct inode* protect_inode(void)
{
	int error;
	struct inode *inode;
	struct path protected_path;
	error = kern_path(PROTECTED_FILE, LOOKUP_FOLLOW, &protected_path);
	if(!error)
    {
        inode=protected_path.dentry->d_inode;
        initial_dentry_flags = protected_path.dentry->d_flags;
        initial_inode_flags = inode->i_flags;
        protected_path.dentry->d_flags = FS_IMMUTABLE_FL;
        inode->i_flags = FS_IMMUTABLE_FL;
        return inode;
	}
	printk(KERN_INFO"kern_path error %llx\n", protected_path);
	return NULL;
}

void set_initial_flags(void)
{
    int error;
	struct inode *inode;
	struct path protected_path;
	error=kern_path(PROTECTED_FILE, LOOKUP_FOLLOW, &protected_path);
	if(!error)
    {
        protected_path.dentry->d_inode->i_flags = initial_inode_flags;
        protected_path.dentry->d_flags = initial_dentry_flags;
	}
}

ssize_t write_iter_hook(struct kiocb *X, struct iov_iter *Y)
{
	if(original_fop->write_iter == NULL)
		return -EINVAL;
	else
		return -EACCES;
}

int open_hook(struct inode* inode, struct file* filep)
{
    if(original_fop->open == NULL)
        return -EINVAL; 
    else if (filep->f_mode & FMODE_WRITE)
        return -EACCES;
    else
        return original_fop->open(inode, filep);
}

int setattr(struct user_namespace *un, struct dentry *d, struct iattr *attr)
{
    return -EACCES;
}

struct file_operations* new_file_operations(void)
{
	struct file_operations* ret = kmalloc(sizeof(struct file_operations), GFP_KERNEL);
	memcpy(ret, original_fop, sizeof(struct file_operations));
	ret->write_iter = write_iter_hook;
    ret->open = open_hook;
	return ret;
}

struct inode_operations* new_inode_operations(void)
{
	struct inode_operations* ret = kmalloc(sizeof(struct inode_operations), GFP_KERNEL);
	memcpy(ret, original_iop, sizeof(struct inode_operations));
	ret->setattr = setattr;
	return ret;
}
 
static int __init mod_init (void)
{
    protected_inode = protect_inode();
    if (protected_inode == NULL)
    {
    	printk(KERN_INFO"Cannot get inode address\nMaybe protected file doesn't exist\n");
    	return -1;
    }
    original_fop = protected_inode->i_fop;
    original_iop = protected_inode->i_op;
    disable_write_protection();
    new_fop = new_file_operations();
    new_iop = new_inode_operations();
    protected_inode->i_fop = new_fop;
    protected_inode->i_op = new_iop;
    printk(KERN_INFO"after: %llx\n", protected_inode->i_fop);
    enable_write_protection();                
    return 0;

}
    
static void mod_exit (void)
{
    disable_write_protection();
    set_initial_flags();
    protected_inode->i_fop = original_fop;
    protected_inode->i_op = original_iop;
    kfree(new_fop);
    kfree(new_iop);
}
    
module_init(mod_init);
module_exit(mod_exit);