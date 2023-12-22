struct inode_operations* new_inode_operations(void)
{
	struct inode_operations* new_iop = kmalloc(
        sizeof(struct inode_operations), GFP_KERNEL);
    if (new_iop)
    {
        memcpy(new_iop, original_iop,
                sizeof(struct inode_operations));
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
    	printk(KERN_INFO"Cannot get inode address\n\
                Maybe protected file doesn't exist\n");
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