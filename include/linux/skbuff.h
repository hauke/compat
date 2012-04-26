#ifndef _COMPAT_LINUX_SKBUFF_H
#define _COMPAT_LINUX_SKBUFF_H

#include_next <linux/skbuff.h>

#include <linux/version.h>

#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,3,0))

extern struct sk_buff *__pskb_copy(struct sk_buff *skb,
				   int headroom, gfp_t gfp_mask);

static inline void skb_complete_wifi_ack(struct sk_buff *skb, bool acked)
{
	WARN_ON(1);
}
#endif /* (LINUX_VERSION_CODE < KERNEL_VERSION(3,3,0)) */

#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,4,0))

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,28))
#define skb_add_rx_frag(skb, i, page, off, size, truesize) \
	v2_6_28_skb_add_rx_frag(skb, i, page, off, size)

extern void v2_6_28_skb_add_rx_frag(struct sk_buff *skb, int i, struct page *page,
			    int off, int size);
#else
#define skb_add_rx_frag(skb, i, page, off, size, truesize) \
	skb_add_rx_frag(skb, i, page, off, size)
#endif

#endif /* (LINUX_VERSION_CODE < KERNEL_VERSION(3,4,0)) */

#endif	/* _COMPAT_LINUX_SKBUFF_H */