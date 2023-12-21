struct dentry {
	unsigned int d_flags;                     /* dentry flags */
	seqcount_spinlock_t d_seq;               /* entry seqlock */
	struct hlist_bl_node d_hash;/* list of hash table entries */
	struct dentry *d_parent;              /* parent directory */
	struct qstr d_name;                        /* dentry name */
	struct inode *d_inode;                /* associated inode */
	unsigned char d_iname[DNAME_INLINE_LEN];   /* small names */
	struct lockref d_lockref; /* per-dentry lock and refcount */
	const struct dentry_operations *d_op;       /* operations */