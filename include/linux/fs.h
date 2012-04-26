#ifndef _COMPAT_LINUX_FS_H
#define _COMPAT_LINUX_FS_H

#include_next <linux/fs.h>

#include <linux/version.h>

#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,5,0))

extern int simple_open(struct inode *inode, struct file *file);

#endif /* (LINUX_VERSION_CODE < KERNEL_VERSION(3,5,0)) */

#endif	/* _COMPAT_LINUX_FS_H */
