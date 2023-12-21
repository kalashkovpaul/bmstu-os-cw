struct inode {
	umode_t i_mode;                     /* access permissions */
	unsigned short i_opflags;          /* xattr support flags */
	kuid_t i_uid;                         /* user id of owner */