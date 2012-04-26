#ifndef _COMPAT_LINUX_NETDEV_FEATURES_H
#define _COMPAT_LINUX_NETDEV_FEATURES_H

#include <linux/version.h>

#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,3,0))
#include <linux/netdevice.h>
#else
#include_next <linux/netdev_features.h>
#endif

#include <linux/version.h>

#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,3,0))

#include <linux/types.h>

typedef u32 netdev_features_t;

#endif /* (LINUX_VERSION_CODE < KERNEL_VERSION(3,3,0)) */

#endif	/* _COMPAT_LINUX_NETDEV_FEATURES_H */
