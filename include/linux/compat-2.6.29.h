#ifndef LINUX_26_29_COMPAT_H
#define LINUX_26_29_COMPAT_H

#include <linux/version.h>
#include <linux/netdevice.h>
#include <linux/if_link.h>

/*
 * I kow this looks odd.. but 2.6.32 added the netdev_tx_t
 * and we backport that there so inlcude that header first
 * as we need it for the netdev ops.
 */
#include <linux/compat-2.6.32.h>

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,29))

#include <linux/skbuff.h>
#include <linux/usb.h>
#include <linux/types.h>

/* backports  */
static inline void usb_autopm_put_interface_async(struct usb_interface *intf)
{ }
static inline int usb_autopm_get_interface_async(struct usb_interface *intf)
{ return 0; }

#if \
	defined(CONFIG_ALPHA) || defined(CONFIG_AVR32) || \
	defined(CONFIG_BLACKFIN) || defined(CONFIG_CRIS) || \
	defined(CONFIG_H8300) || defined(CONFIG_IA64) || \
	defined(CONFIG_M68K) ||  defined(CONFIG_MIPS) || \
	defined(CONFIG_PARISC) || defined(CONFIG_S390) || \
	defined(CONFIG_PPC64) || defined(CONFIG_PPC32) || \
	defined(CONFIG_SUPERH) || defined(CONFIG_SPARC) || \
	defined(CONFIG_FRV) || defined(CONFIG_X86) || \
	defined(CONFIG_M32R) || defined(CONFIG_M68K) || \
	defined(CONFIG_MN10300) || defined(CONFIG_XTENSA)
#include <asm/atomic.h>
#else
typedef struct {
	volatile int counter;
} atomic_t;

#ifdef CONFIG_64BIT
typedef struct {
	volatile long counter;
} atomic64_t;
#endif /* CONFIG_64BIT */

#endif

#define  PCI_EXP_LNKCTL_ES     0x0080  /* Extended Synch */

static inline int ndo_do_ioctl(struct net_device *dev,
			       struct ifreq *ifr,
			       int cmd)
{
	if (dev->do_ioctl)
		return dev->do_ioctl(dev, ifr, cmd);
	return -EOPNOTSUPP;
}

/**
 *	skb_queue_is_first - check if skb is the first entry in the queue
 *	@list: queue head
 *	@skb: buffer
 *
 *	Returns true if @skb is the first buffer on the list.
 */
static inline bool skb_queue_is_first(const struct sk_buff_head *list,
				      const struct sk_buff *skb)
{
	return (skb->prev == (struct sk_buff *) list);
}

/**
 *	skb_queue_prev - return the prev packet in the queue
 *	@list: queue head
 *	@skb: current buffer
 *
 *	Return the prev packet in @list before @skb.  It is only valid to
 *	call this if skb_queue_is_first() evaluates to false.
 */
static inline struct sk_buff *skb_queue_prev(const struct sk_buff_head *list,
					     const struct sk_buff *skb)
{
	/* This BUG_ON may seem severe, but if we just return then we
	 * are going to dereference garbage.
	 */
	BUG_ON(skb_queue_is_first(list, skb));
	return skb->prev;
}


static inline struct net_device_stats *dev_get_stats(struct net_device *dev)
{
	return dev->get_stats(dev);
}

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,23))
#if defined(CONFIG_USB) || defined(CONFIG_USB_MODULE)
extern void usb_unpoison_anchored_urbs(struct usb_anchor *anchor);
#endif /* CONFIG_USB */
#endif

#define DIV_ROUND_CLOSEST(x, divisor)(			\
{							\
	typeof(divisor) __divisor = divisor;		\
	(((x) + ((__divisor) / 2)) / (__divisor));	\
}							\
)

extern int eth_mac_addr(struct net_device *dev, void *p);
extern int eth_change_mtu(struct net_device *dev, int new_mtu);
extern int eth_validate_addr(struct net_device *dev);

#ifdef CONFIG_NET_NS

static inline void write_pnet(struct net **pnet, struct net *net)
{
	*pnet = net;
}

static inline struct net *read_pnet(struct net * const *pnet)
{
	return *pnet;
}

#else

#define write_pnet(pnet, net)	do { (void)(net);} while (0)
#define read_pnet(pnet)		(&init_net)

/*
 * swap - swap value of @a and @b
 */
#define swap(a, b) \
	do { typeof(a) __tmp = (a); (a) = (b); (b) = __tmp; } while (0)

#endif

extern int		init_dummy_netdev(struct net_device *dev);

#define compat_pci_suspend(fn)						\
	int fn##_compat(struct pci_dev *pdev, pm_message_t state) 	\
	{								\
		int r;							\
									\
		r = fn(&pdev->dev);					\
		if (r)							\
			return r;					\
									\
		pci_save_state(pdev);					\
		pci_disable_device(pdev);				\
		pci_set_power_state(pdev, PCI_D3hot);			\
									\
		return 0;						\
	}

#define compat_pci_resume(fn)						\
	int fn##_compat(struct pci_dev *pdev)				\
	{								\
		int r;							\
									\
		pci_set_power_state(pdev, PCI_D0);			\
		r = pci_enable_device(pdev);				\
		if (r)							\
			return r;					\
		pci_restore_state(pdev);				\
									\
		return fn(&pdev->dev);					\
	}

#else /* (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,29)) */

/* Kernels >= 2.6.29 follows */

/* XXX: this can probably just go upstream! */
static inline int ndo_do_ioctl(struct net_device *dev,
			       struct ifreq *ifr,
			       int cmd)
{
	if (dev->netdev_ops && dev->netdev_ops->ndo_do_ioctl)
		return dev->netdev_ops->ndo_do_ioctl(dev, ifr, cmd);
	return -EOPNOTSUPP;
}

#define compat_pci_suspend(fn)
#define compat_pci_resume(fn)

#endif /* (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,29)) */

#endif /*  LINUX_26_29_COMPAT_H */
