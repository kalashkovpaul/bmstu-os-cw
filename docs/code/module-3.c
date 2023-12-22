int open_hook(struct inode* inode, struct file* filep)
{
    int access = -EACCES;
    if(original_fop->open == NULL)
        access = -EINVAL;
    else if (!(filep->f_mode & FMODE_WRITE))
        access =  original_fop->open(inode, filep);
    return access;
}

int setattr_hook(struct user_namespace *un, struct dentry *d,
                                          struct iattr *attr)
{
    return -EACCES;
}

struct file_operations* new_file_operations(void)
{
	struct file_operations* new_fop = kmalloc(
        sizeof(struct file_operations), GFP_KERNEL);
    if (new_fop)
    {
        memcpy(new_fop, original_fop,
                sizeof(struct file_operations));
        new_fop->write_iter = write_iter_hook;
        new_fop->open = open_hook;
    }
	return new_fop;
}