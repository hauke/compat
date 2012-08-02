#ifndef LINUX_26_32_COMPAT_H
#define LINUX_26_32_COMPAT_H

#include <linux/version.h>

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,32))

#include <linux/netdevice.h>
#include <linux/compat.h>
#include <net/iw_handler.h>
#include <linux/workqueue.h>
#include <net/genetlink.h>
#include <net/sch_generic.h>

#define TCQ_F_CAN_BYPASS        4

static inline int qdisc_qlen(const struct Qdisc *q)
{
	return q->q.qlen;
}

#define SDIO_VENDOR_ID_INTEL			0x0089
#define SDIO_DEVICE_ID_INTEL_IWMC3200WIMAX	0x1402
#define SDIO_DEVICE_ID_INTEL_IWMC3200WIFI	0x1403
#define SDIO_DEVICE_ID_INTEL_IWMC3200TOP	0x1404
#define SDIO_DEVICE_ID_INTEL_IWMC3200GPS	0x1405
#define SDIO_DEVICE_ID_INTEL_IWMC3200BT		0x1406

/*
 * Backports 5e928f77a09a07f9dd595bb8a489965d69a83458
 * run-time power management cannot really be backported
 * given that the implementation added bus specific
 * callbacks that we won't have on older kernels. If
 * you really want run-time power management or good
 * power management upgrade your kernel. We'll just
 * compile this out as if run-time power management was
 * disabled just as the kernel disables run-time power management
 * when CONFIG_PM_RUNTIME is disabled.
 */
static inline void pm_runtime_init(struct device *dev) {}
static inline void pm_runtime_remove(struct device *dev) {}
static inline int pm_runtime_get(struct device *dev)
{
	return 0;
}

static inline int pm_runtime_get_sync(struct device *dev)
{
	return 0;
}

static inline int pm_runtime_put(struct device *dev)
{
	return 0;
}

static inline int pm_runtime_put_sync(struct device *dev)
{
	return 0;
}

static inline int pm_runtime_set_active(struct device *dev)
{
	return 0;
}

static inline void pm_runtime_set_suspended(struct device *dev)
{
}

static inline void pm_runtime_disable(struct device *dev)
{
}

static inline void pm_runtime_put_noidle(struct device *dev) {}
static inline void pm_runtime_get_noresume(struct device *dev) {}

static inline void flush_delayed_work(struct delayed_work *dwork)
{
	if (del_timer_sync(&dwork->timer)) {
		/*
		 * This is what would happen on 2.6.32 but since we don't have
		 * access to the singlethread_cpu we can't really backport this,
		 * so avoid really *flush*ing the work... Oh well. Any better ideas?

		struct cpu_workqueue_struct *cwq;
		cwq = wq_per_cpu(keventd_wq, get_cpu());
		__queue_work(cwq, &dwork->work);
		put_cpu();

		*/
	}
	flush_work(&dwork->work);
}

/*
 * struct genl_multicast_group was made netns aware through
 * patch "genetlink: make netns aware" by johannes, we just
 * force this to always use the default init_net
 */
#define genl_info_net(x) &init_net
/* Just use init_net for older kernels */
#define get_net_ns_by_pid(x) &init_net

/* net namespace is lost */
#define genlmsg_multicast_netns(a, b, c, d, e)	genlmsg_multicast(b, c, d, e)
#define genlmsg_multicast_allns(a, b, c, d)	genlmsg_multicast(a, b, c, d)
#define genlmsg_unicast(net, skb, pid)	genlmsg_unicast(skb, pid)

#define dev_change_net_namespace(a, b, c) (-EOPNOTSUPP)

#define SET_NETDEV_DEVTYPE(netdev, type)

#ifdef __KERNEL__
/* Driver transmit return codes */
enum netdev_tx {
	BACKPORT_NETDEV_TX_OK = NETDEV_TX_OK,       /* driver took care of packet */
	BACKPORT_NETDEV_TX_BUSY = NETDEV_TX_BUSY,         /* driver tx path was busy*/
	BACKPORT_NETDEV_TX_LOCKED = NETDEV_TX_LOCKED,  /* driver tx lock was already taken */
};
typedef enum netdev_tx netdev_tx_t;
#endif /* __KERNEL__ */

/*
 * dev_pm_ops is only available on kernels >= 2.6.29, for
 * older kernels we rely on reverting the work to old
 * power management style stuff. On 2.6.29 the pci calls
 * weren't included yet though, so include them here.
 */
#if (LINUX_VERSION_CODE == KERNEL_VERSION(2,6,29))
/*
 * Note the lack of "static" in this definition. The code may put
 * a "static" in front of using the define, which makes it impossible
 * to put static here. We lose that static if it is present (it ends
 * up being on the suspend_fn) but that doesn't really matter much
 * unless a driver defines multiple pm_ops structs with the same name
 * (in which case we'll need to fix it)
 */
#define SIMPLE_DEV_PM_OPS(name, suspend_fn, resume_fn)		\
int __compat_suspend_fn_ ## name(struct device *dev)		\
{								\
	int ret = suspend_fn(dev);				\
	if (ret)						\
		return ret;					\
	if (dev->bus == &pci_bus_type) {			\
		pci_save_state(to_pci_dev(dev));		\
		pci_disable_device(to_pci_dev(dev));		\
		pci_set_power_state(to_pci_dev(dev), PCI_D3hot);\
	}							\
	return 0;						\
}								\
int __compat_resume_fn_ ## name(struct device *dev)		\
{								\
	int ret;						\
	if (dev->bus == &pci_bus_type) {			\
		pci_set_power_state(to_pci_dev(dev), PCI_D0);	\
		ret = pci_enable_device(to_pci_dev(dev));	\
		if (ret)					\
			return ret;				\
		pci_restore_state(to_pci_dev(dev));		\
	}							\
	return resume_fn(dev);					\
}								\
struct dev_pm_ops name = {					\
	.suspend = __compat_suspend_fn_ ## name,		\
	.resume = __compat_resume_fn_ ## name,			\
	.freeze = __compat_suspend_fn_ ## name,			\
	.thaw = __compat_resume_fn_ ## name,			\
	.poweroff = __compat_suspend_fn_ ## name,		\
	.restore = __compat_resume_fn_ ## name,			\
}
#elif (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,30))
/*
 * Use this if you want to use the same suspend and resume callbacks for suspend
 * to RAM and hibernation.
 */
#define SIMPLE_DEV_PM_OPS(name, suspend_fn, resume_fn) \
struct dev_pm_ops name = { \
	.suspend = suspend_fn, \
	.resume = resume_fn, \
	.freeze = suspend_fn, \
	.thaw = resume_fn, \
	.poweroff = suspend_fn, \
	.restore = resume_fn, \
}
#else
#define SIMPLE_DEV_PM_OPS(name, suspend_fn, resume_fn)
#endif /* >= 2.6.29 */

#define wireless_send_event(a, b, c, d) wireless_send_event(a, b, c, (char * ) d)

/* The export symbol in changed in compat/patches/15-symbol-export-conflicts.patch */
#define ieee80211_rx(hw, skb) mac80211_ieee80211_rx(hw, skb)

#define dev_to_sdio_func(d)	container_of(d, struct sdio_func, dev)

#define lockdep_assert_held(l)			do { } while (0)

/*
 * Similar to the struct tm in userspace <time.h>, but it needs to be here so
 * that the kernel source is self contained.
 */
struct tm {
	/*
	 * the number of seconds after the minute, normally in the range
	 * 0 to 59, but can be up to 60 to allow for leap seconds
	 */
	int tm_sec;
	/* the number of minutes after the hour, in the range 0 to 59*/
	int tm_min;
	/* the number of hours past midnight, in the range 0 to 23 */
	int tm_hour;
	/* the day of the month, in the range 1 to 31 */
	int tm_mday;
	/* the number of months since January, in the range 0 to 11 */
	int tm_mon;
	/* the number of years since 1900 */
	long tm_year;
	/* the number of days since Sunday, in the range 0 to 6 */
	int tm_wday;
	/* the number of days since January 1, in the range 0 to 365 */
	int tm_yday;
};

void time_to_tm(time_t totalsecs, int offset, struct tm *result);

#endif /* (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,32)) */

#endif /* LINUX_26_32_COMPAT_H */
