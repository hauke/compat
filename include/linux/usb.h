#ifndef _COMPAT_LINUX_USB_H
#define _COMPAT_LINUX_USB_H

#include_next <linux/usb.h>

#include <linux/version.h>

#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,3,0))

/**
 * module_usb_driver() - Helper macro for registering a USB driver
 * @__usb_driver: usb_driver struct
 *
 * Helper macro for USB drivers which do not do anything special in module
 * init/exit. This eliminates a lot of boilerplate. Each module may only
 * use this macro once, and calling it replaces module_init() and module_exit()
 */
#define module_usb_driver(__usb_driver) \
	module_driver(__usb_driver, usb_register, \
		       usb_deregister)

#endif /* (LINUX_VERSION_CODE < KERNEL_VERSION(3,3,0)) */

#endif	/* _COMPAT_LINUX_USB_H */
