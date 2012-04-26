#ifndef LINUX_3_4_COMPAT_H
#define LINUX_3_4_COMPAT_H

#include <linux/version.h>

#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,4,0))

#include <linux/etherdevice.h>
#include <linux/skbuff.h>

#endif /* (LINUX_VERSION_CODE < KERNEL_VERSION(3,4,0)) */

#endif /* LINUX_5_4_COMPAT_H */
