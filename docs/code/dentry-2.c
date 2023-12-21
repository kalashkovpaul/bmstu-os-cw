	struct super_block *d_sb;  /* The root of the dentry tree */
	unsigned long d_time;                  /* revalidate time */
	void *d_fsdata;                       /* fs-specific data */

	union {
		struct list_head d_lru;                   /* LRU list */
		wait_queue_head_t *d_wait;     /* in-lookup ones only */
	};
	struct list_head d_child;         /* child of parent list */
	struct list_head d_subdirs;               /* our children */

	union {
		struct hlist_node d_alias;        /* inode alias list */
		struct hlist_bl_node d_in_lookup_hash;   /* in-lookup */
	 	struct rcu_head d_rcu;                 /* RCU locking */
	} d_u;
} __randomize_layout;