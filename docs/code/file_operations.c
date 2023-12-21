struct file_operations {
    // ...
	ssize_t (*read) (struct file *, char __user *,
        size_t, loff_t *);
	ssize_t (*write) (struct file *, const char __user *,
        size_t, loff_t *);
	ssize_t (*read_iter) (struct kiocb *, struct iov_iter *);
	ssize_t (*write_iter) (struct kiocb *, struct iov_iter *);
    int (*open) (struct inode *, struct file *);
    // ...
} __randomize_layout;
