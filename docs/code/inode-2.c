#ifdef __NEED_I_SIZE_ORDERED             /* sequence counters */
	seqcount_t i_size_seqcount;                    /* counter */
#endif
	unsigned long i_state;                     /* state flags */
	struct rw_semaphore i_rwsem;                 /* semaphore */
	unsigned long dirtied_when;  /* jiffies of first dirtying */
	unsigned long dirtied_time_when;         /* last dirtying */
	struct hlist_node i_hash;                    /* hash list */
	struct list_head i_io_list;        /* backing dev IO list */

#ifdef CONFIG_CGROUP_WRITEBACK /* flush dirty from page cache */
	struct bdi_writeback *i_wb;   /* the associated cgroup wb */
	/* foreign inode detection, see wbc_detach_inode() */
	int i_wb_frn_winner;
	u16 i_wb_frn_avg_time;
	u16 i_wb_frn_history;
#endif
	struct list_head i_lru;                /* inode LRU list */
	struct list_head i_sb_list;              /* sb of inodes */
	struct list_head i_wb_list;       /* backing dev wb list */
	union {
		struct hlist_head   i_dentry;    /* list of dentries */
		struct rcu_head     i_rcu;    /* list of rcu updates */
	};
	atomic64_t i_version;                  /* version number */
	atomic64_t i_sequence;                      /* see futex */
	atomic_t i_count;                   /* reference counter */
	atomic_t i_dio_count;                     /* DIO counter */
	atomic_t i_writecount;                /* writers counter */
#if defined(CONFIG_IMA) || defined(CONFIG_FILE_LOCKING) /*IMA*/
	atomic_t i_readcount;            /* struct files open RO */
#endif