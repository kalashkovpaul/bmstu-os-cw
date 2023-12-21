	kgid_t i_gid;                        /* group id of owner */
	unsigned int i_flags;                 /* filesystem flags */
#ifdef CONFIG_FS_POSIX_ACL              /* if FS supports ACL */
	struct posix_acl * i_acl;                     /* ACL mode */
	struct posix_acl * i_default_acl;          /* default ACL */
#endif
	const struct inode_operations *i_op;   /*inode operations */
	struct super_block *i_sb;           /* superblock pointer */
	struct address_space	*i_mapping; /* associated mapping */
#ifdef CONFIG_SECURITY             /* if with security module */
	void *i_security;                      /* security module */
#endif
	unsigned long i_ino;                      /* inode number */
	union {
		const unsigned int i_nlink;   /* number of hard links */
		unsigned int __i_nlink;       /* number of hard links */
	};
	dev_t i_rdev;                         /* real device node */
	loff_t i_size;                      /* file size in bytes */
	struct timespec64 i_atime;            /* last access time */
	struct timespec64 i_mtime;            /* last modify time */
	struct timespec64 i_ctime;            /* last change time */
	spinlock_t i_lock;                            /* spinlock */
	unsigned short i_bytes;                 /* bytes consumed */
	u8 i_blkbits;                       /* block size in bits */
	u8 i_write_hint;                      /* hint about block */
	blkcnt_t i_blocks;                 /* file size in blocks */
#ifdef __NEED_I_SIZE_ORDERED             /* sequence counters */
	seqcount_t i_size_seqcount;                    /* counter */
#endif
	unsigned long i_state;                     /* state flags */