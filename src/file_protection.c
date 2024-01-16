#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/unistd.h>
#include <asm/uaccess.h>
#include <linux/namei.h>
#include <asm/string.h>

#define PROTECTED_FILE "/home/paul/Desktop/reps/bmstu-os-cw/protected"
MODULE_LICENSE ("GPL");

struct dentry* protected_dentry;
struct inode* protected_inode;
struct file_operations* original_fop;
struct file_operations* new_fop;
struct inode_operations* original_iop;
struct inode_operations* new_iop;
unsigned int initial_dentry_flags;
unsigned long initial_cr0;

inline void write_cr0_custom(unsigned long cr0)
{
    unsigned long __force_order;
    asm volatile("mov %0,%%cr0" : "+r"(cr0), "+m"(__force_order));
}

void disable_write_protection(void)
{
    write_cr0_custom(initial_cr0 & (~0x10000));
}

void enable_write_protection(void)
{
    write_cr0_custom(initial_cr0);
}

struct inode* protect_inode(void)
{
	int error;
	struct inode *inode;
	struct path protected_path;
	error = kern_path(PROTECTED_FILE, LOOKUP_FOLLOW, &protected_path);
	if(!error)
    {
        protected_dentry = protected_path.dentry;
        inode=protected_path.dentry->d_inode;
        initial_dentry_flags = protected_path.dentry->d_flags;
        protected_path.dentry->d_flags = FS_IMMUTABLE_FL;
        return inode;
	}
	printk(KERN_INFO"kern_path error %llx\n", protected_path);
	return NULL;
}

ssize_t write_iter_hook(struct kiocb *X, struct iov_iter *Y)
{
    ssize_t access = -EACCES;
	if(original_fop->write_iter == NULL)
		access = -EINVAL;
	return access;
}

int open_hook(struct inode* inode, struct file* filep)
{
    int access = -EACCES;
    if(original_fop->open == NULL)
        access = -EINVAL;
    else if (!(filep->f_mode & FMODE_WRITE))
        access =  original_fop->open(inode, filep);
    return access;
}

int setattr_hook(struct mnt_idmap *m, struct dentry *d, struct iattr *attr)
{
    return -EACCES;
}

struct file_operations* new_file_operations(void)
{
	struct file_operations* new_fop = kmalloc(sizeof(struct file_operations), GFP_KERNEL);
    if (new_fop)
    {
        memcpy(new_fop, original_fop, sizeof(struct file_operations));
        new_fop->write_iter = write_iter_hook;
        new_fop->open = open_hook;
    }
	return new_fop;
}

struct inode_operations* new_inode_operations(void)
{
	struct inode_operations* new_iop = kmalloc(sizeof(struct inode_operations), GFP_KERNEL);
    if (new_iop)
    {
        memcpy(new_iop, original_iop, sizeof(struct inode_operations));
        new_iop->setattr = setattr_hook;
    }
	return new_iop;
}

static int __init mod_init (void)
{
    initial_cr0 = read_cr0();
    disable_write_protection();
    protected_inode = protect_inode();
    if (protected_inode == NULL)
    {
    	printk(KERN_INFO"Cannot get inode address\nMaybe protected file doesn't exist\n");
        enable_write_protection();
    	return -1;
    }
    original_fop = protected_inode->i_fop;
    original_iop = protected_inode->i_op;
    new_fop = new_file_operations();
    new_iop = new_inode_operations();
    if (!new_fop || ! new_iop)
    {
        kfree(new_fop);
        kfree(new_iop);
    }
    else
    {
        protected_inode->i_fop = new_fop;
        protected_inode->i_op = new_iop;
    }
    enable_write_protection();
    return 0;

}

static void mod_exit (void)
{
    disable_write_protection();
    protected_dentry->d_flags = initial_dentry_flags;
    protected_inode->i_fop = original_fop;
    protected_inode->i_op = original_iop;
    enable_write_protection();
    kfree(new_fop);
    kfree(new_iop);
}

module_init(mod_init);
module_exit(mod_exit);