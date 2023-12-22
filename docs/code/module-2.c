void enable_write_protection(void)
{
    write_cr0_custom(initial_cr0);
}

struct inode* protect_inode(void)
{
	int error;
	struct inode *inode;
	struct path protected_path;
	error = kern_path(PROTECTED_FILE, LOOKUP_FOLLOW,
                                    &protected_path);
	if (!error)
    {
        protected_dentry = protected_path.dentry;
        inode = protected_path.dentry->d_inode;
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