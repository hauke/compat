#ifndef LINUX_3_3_COMPAT_H
#define LINUX_3_3_COMPAT_H

#include <linux/version.h>

#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,3,0))

/* include to override NL80211_FEATURE_SK_TX_STATUS */
#include <linux/nl80211.h>
#include <linux/skbuff.h>

#define NL80211_FEATURE_SK_TX_STATUS 0

#endif /* (LINUX_VERSION_CODE < KERNEL_VERSION(3,3,0)) */

#endif /* LINUX_3_3_COMPAT_H */
