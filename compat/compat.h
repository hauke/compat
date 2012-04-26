#ifndef _COMPAT_COMPAT_H
#define _COMPAT_COMPAT_H

#include <linux/version.h>

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,25))
/*
 * Backport work for QoS dependencies (kernel/pm_qos_params.c)
 * pm-qos stuff written by mark gross mgross@linux.intel.com.
 *
 * ipw2100 now makes use of:
 *
 * pm_qos_add_requirement(),
 * pm_qos_update_requirement() and
 * pm_qos_remove_requirement() from it
 *
 * mac80211 uses the network latency to determine if to enable or not
 * dynamic PS. mac80211 also and registers a notifier for when
 * the latency changes. Since older kernels do no thave pm-qos stuff
 * we just implement it completley here and register it upon cfg80211
 * init. I haven't tested ipw2100 on 2.6.24 though.
 *
 * This pm-qos implementation is copied verbatim from the kernel
 * written by mark gross mgross@linux.intel.com. You don't have
 * to do anythinig to use pm-qos except use the same exported
 * routines as used in newer kernels. The compat_pm_qos_power_init()
 * defned below is used by the compat module to initialize pm-qos.
 */
int compat_pm_qos_power_init(void);
int compat_pm_qos_power_deinit(void);

#else
/*
 * Kernels >= 2.6.25 have pm-qos and its initialized as part of
 * the bootup process
 */
static inline int compat_pm_qos_power_init(void)
{
	return 0;
}

static inline int compat_pm_qos_power_deinit(void)
{
	return 0;
}
#endif /* (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,25)) */


#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36))

void compat_system_workqueue_create(void);
void compat_system_workqueue_destroy(void);

#else

static inline void compat_system_workqueue_create(void)
{
}

static inline void compat_system_workqueue_destroy(void)
{
}

#endif /* (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)) */

#endif	/* _COMPAT_COMPAT_H */
