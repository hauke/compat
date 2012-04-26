#ifndef _COMPAT_LINUX_ETHERDEVICE_H
#define _COMPAT_LINUX_ETHERDEVICE_H

#include_next <linux/etherdevice.h>

#include <linux/version.h>

#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,4,0))

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,12))
static inline void eth_hw_addr_random(struct net_device *dev)
{
#error eth_hw_addr_random() needs to be implemented for < 2.6.12
}
#else  /* kernels >= 2.6.12 */

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,31))
static inline void eth_hw_addr_random(struct net_device *dev)
{
	get_random_bytes(dev->dev_addr, ETH_ALEN);
	dev->dev_addr[0] &= 0xfe;       /* clear multicast bit */
	dev->dev_addr[0] |= 0x02;       /* set local assignment bit (IEEE802) */
}
#else /* kernels >= 2.6.31 */

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36))
/* So this is 2.6.31..2.6.35 */

/* Just have the flags present, they won't really mean anything though */
#define NET_ADDR_PERM          0       /* address is permanent (default) */
#define NET_ADDR_RANDOM                1       /* address is generated randomly */
#define NET_ADDR_STOLEN                2       /* address is stolen from other device */

static inline void eth_hw_addr_random(struct net_device *dev)
{
	random_ether_addr(dev->dev_addr);
}

#else /* 2.6.36 and on */
static inline void eth_hw_addr_random(struct net_device *dev)
{
	dev_hw_addr_random(dev, dev->dev_addr);
}
#endif /* (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,31)) */

#endif /* (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,31)) */
#endif /* (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,12)) */

#endif /* (LINUX_VERSION_CODE < KERNEL_VERSION(3,4,0)) */

#endif	/* _COMPAT_LINUX_ETHERDEVICE_H */
