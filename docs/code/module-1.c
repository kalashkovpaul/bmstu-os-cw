#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/unistd.h>
#include <asm/uaccess.h>
#include <linux/namei.h>
#include <asm/string.h>

#define PROTECTED_FILE "/home/paul/Desktop/bmstu-os-cw/\
    src/protected"
MODULE_LICENSE ("GPL");

struct dentry* protected_dentry;
struct inode* protected_inode;
struct file_operations *original_fop, *new_fop;
struct inode_operations *original_iop, *new_iop;
unsigned int initial_dentry_flags;
int permissions;
unsigned long initial_cr0;

inline void write_cr0_custom(unsigned long cr0)
{
    unsigned long __force_order;
    asm volatile("mov %0,%%cr0" : "+r"(cr0),
                "+m"(__force_order));
}

void disable_write_protection(void)
{
    write_cr0_custom(initial_cr0 & (~0x10000));
}