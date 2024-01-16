	union {
		const struct file_operations *i_fop /*file operations*/
		void (*free_inode)(struct inode *); /* free function */
	};
	struct file_lock_context *i_flctx;          /* for fcntl */
	struct address_space i_data;       /* mapping for device */
	struct list_head	i_devices;  /* list of block devices */
	union {
		struct pipe_inode_info *i_pipe;  /* pipe information */
		struct cdev *i_cdev;                  /*char devices */
		char *i_link;                             /* symlink */
		unsigned i_dir_seq;          /* opencoded seqcounter */
	};

	__u32 i_generation;              /* inode version number */

#ifdef CONFIG_FSNOTIFY
	__u32 i_fsnotify_mask;  /* all events inode cares about */
	struct fsnotify_mark_connector __rcu	*i_fsnotify_marks;
#endif

#ifdef CONFIG_FS_ENCRYPTION
	struct fscrypt_info	*i_crypt_info;
#endif

#ifdef CONFIG_FS_VERITY
	struct fsverity_info *i_verity_info;
#endif

	void *i_private;       /* fs or device private pointer */
} __randomize_layout;