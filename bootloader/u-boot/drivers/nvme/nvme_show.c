/*
 * Copyright (C) 2017 NXP Semiconductors
 * Copyright (C) 2017 Bin Meng <bmeng.cn@gmail.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#ifdef CONFIG_DM
# include <dm.h>
#endif
#include <errno.h>
#include <nvme.h>
#include "nvme.h"

extern block_dev_desc_t nvme_dev_desc[CONFIG_SYS_NVME_MAX_DEVICE];
extern struct nvme_dev nvme_devs[CONFIG_SYS_NVME_MAX_DEVICE];
extern struct nvme_ns nvme_nses[CONFIG_SYS_NVME_MAX_DEVICE];

static void print_optional_admin_cmd(u16 oacs, int devnum)
{
	printf("Blk device %d: Optional Admin Command Support:\n",
	       devnum);
	printf("\tNamespace Management/Attachment: %s\n",
	       oacs & 0x08 ? "yes" : "no");
	printf("\tFirmware Commit/Image download: %s\n",
	       oacs & 0x04 ? "yes" : "no");
	printf("\tFormat NVM: %s\n",
	       oacs & 0x02 ? "yes" : "no");
	printf("\tSecurity Send/Receive: %s\n",
	       oacs & 0x01 ? "yes" : "no");
}

static void print_optional_nvm_cmd(u16 oncs, int devnum)
{
	printf("Blk device %d: Optional NVM Command Support:\n",
	       devnum);
	printf("\tReservation: %s\n",
	       oncs & 0x10 ? "yes" : "no");
	printf("\tSave/Select field in the Set/Get features: %s\n",
	       oncs & 0x08 ? "yes" : "no");
	printf("\tWrite Zeroes: %s\n",
	       oncs & 0x04 ? "yes" : "no");
	printf("\tDataset Management: %s\n",
	       oncs & 0x02 ? "yes" : "no");
	printf("\tWrite Uncorrectable: %s\n",
	       oncs & 0x01 ? "yes" : "no");
}

static void print_format_nvme_attributes(u8 fna, int devnum)
{
	printf("Blk device %d: Format NVM Attributes:\n", devnum);
	printf("\tSupport Cryptographic Erase: %s\n",
	       fna & 0x04 ? "yes" : "No");
	printf("\tSupport erase a particular namespace: %s\n",
	       fna & 0x02 ? "No" : "Yes");
	printf("\tSupport format a particular namespace: %s\n",
	       fna & 0x01 ? "No" : "Yes");
}

static void print_format(struct nvme_lbaf *lbaf)
{
	u8 str[][10] = {"Best", "Better", "Good", "Degraded"};

	printf("\t\tMetadata Size: %d\n", le16_to_cpu(lbaf->ms));
	printf("\t\tLBA Data Size: %d\n", 1 << lbaf->ds);
	printf("\t\tRelative Performance: %s\n", str[lbaf->rp & 0x03]);
}

static void print_formats(struct nvme_id_ns *id, struct nvme_ns *ns)
{
	int i;

	printf("Blk device %d: LBA Format Support:\n", ns->devnum);

	for (i = 0; i < id->nlbaf; i++) {
		printf("\tLBA Format %d Support: ", i);
		if (i == ns->flbas)
			printf("(current)\n");
		else
			printf("\n");
		print_format(id->lbaf + i);
	}
}

static void print_data_protect_cap(u8 dpc, int devnum)
{
	printf("Blk device %d: End-to-End Data", devnum);
	printf("Protect Capabilities:\n");
	printf("\tAs last eight bytes: %s\n",
	       dpc & 0x10 ? "yes" : "No");
	printf("\tAs first eight bytes: %s\n",
	       dpc & 0x08 ? "yes" : "No");
	printf("\tSupport Type3: %s\n",
	       dpc & 0x04 ? "yes" : "No");
	printf("\tSupport Type2: %s\n",
	       dpc & 0x02 ? "yes" : "No");
	printf("\tSupport Type1: %s\n",
	       dpc & 0x01 ? "yes" : "No");
}

static void print_metadata_cap(u8 mc, int devnum)
{
	printf("Blk device %d: Metadata capabilities:\n", devnum);
	printf("\tAs part of a separate buffer: %s\n",
	       mc & 0x02 ? "yes" : "No");
	printf("\tAs part of an extended data LBA: %s\n",
	       mc & 0x01 ? "yes" : "No");
}

#ifdef CONFIG_DM
int nvme_print_info(struct udevice *udev)
{
	struct nvme_ns *ns = dev_get_priv(udev);
	struct nvme_dev *dev = ns->dev;
	struct nvme_id_ns buf_ns, *id = &buf_ns;
	struct nvme_id_ctrl buf_ctrl, *ctrl = &buf_ctrl;

	if (nvme_identify(dev, 0, 1, (dma_addr_t)ctrl))
		return -EIO;

	print_optional_admin_cmd(le16_to_cpu(ctrl->oacs), ns->devnum);
	print_optional_nvm_cmd(le16_to_cpu(ctrl->oncs), ns->devnum);
	print_format_nvme_attributes(ctrl->fna, ns->devnum);

	if (nvme_identify(dev, ns->ns_id, 0, (dma_addr_t)id))
		return -EIO;

	print_formats(id, ns);
	print_data_protect_cap(id->dpc, ns->devnum);
	print_metadata_cap(id->mc, ns->devnum);

	return 0;
}
#else
int nvme_print_info(int devnum)
{
	struct nvme_ns *ns = &nvme_nses[devnum];
	struct nvme_dev *dev = ns->dev;
	struct nvme_id_ns buf_ns, *id = &buf_ns;
	struct nvme_id_ctrl buf_ctrl, *ctrl = &buf_ctrl;

	if (nvme_identify(dev, 0, 1, cvmx_ptr_to_phys(ctrl)))
		return -EIO;

	print_optional_admin_cmd(le16_to_cpu(ctrl->oacs), ns->devnum);
	print_optional_nvm_cmd(le16_to_cpu(ctrl->oncs), ns->devnum);
	print_format_nvme_attributes(ctrl->fna, ns->devnum);

	if (nvme_identify(dev, ns->ns_id, 0, cvmx_ptr_to_phys(id)))
		return -EIO;

	print_formats(id, ns);
	print_data_protect_cap(id->dpc, ns->devnum);
	print_metadata_cap(id->mc, ns->devnum);

	return 0;
}
#endif
