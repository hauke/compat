#ifndef _COMPAT_LINUX_PCI_H
#define _COMPAT_LINUX_PCI_H

#include_next <linux/pci.h>

#include <linux/version.h>

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,29))

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

#define compat_pci_suspend(fn)
#define compat_pci_resume(fn)

#endif /* (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,29)) */

#endif	/* _COMPAT_LINUX_PCI_H */
