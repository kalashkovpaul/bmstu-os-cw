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