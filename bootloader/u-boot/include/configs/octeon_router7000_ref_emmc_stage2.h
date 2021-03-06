
/*
 * (C) Copyright 2003
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * Copyright (C) 2004 - 2014 Cavium, Inc. <support@cavium.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

/*
 * This file contains the configuration parameters for
 * Octeon generic eMMC boards.
 */

#ifndef __CONFIG_H__
#define __CONFIG_H__

/**
 * Build configuration for cn70xx
 */
#define CONFIG_OCTEON_CN70XX

#define CONFIG_OCTEON_ROUTER7000_REF_MAJOR		1
#define CONFIG_OCTEON_ROUTER7000_REF_MINOR		0


/*
 * Define CONFIG_OCTEON_PCI_HOST = 1 to map the pci devices on the
 * bus.  Define CONFIG_OCTEON_PCI_HOST = 0 for target mode when the
 * host system performs the pci bus mapping instead.  Note that pci
 * commands are enabled to allow access to configuration space for
 * both modes.
 */
#ifndef CONFIG_OCTEON_PCI_HOST
# define CONFIG_OCTEON_PCI_HOST		0
#endif

#define CONFIG_OCTEON_MMC_MAX_FREQUENCY	52000000

#include "octeon_common_emmc_stage2.h"

/* Default DDR clock if tuple doesn't exist in EEPROM */
#define OCTEON_ROUTER7000_REF_DEF_DRAM_FREQ		666

/* Address of board EEPROM on TWSI bus */
#define CONFIG_SYS_I2C_EEPROM_ADDR	(0)
/* Default EEPROM address */
#define CONFIG_SYS_DEF_EEPROM_ADDR	CONFIG_SYS_I2C_EEPROM_ADDR

/* Set bootdelay to 0 for immediate boot */
#define CONFIG_BOOTDELAY	1	/* autoboot after X seconds	*/
#define CONFIG_BOOTCOMMAND	"bootstage3"

#define CONFIG_OCTEON_MD5

#undef	CONFIG_BOOTARGS

#include "octeon_cmd_conf.h"

#define CONFIG_DOS_PARTITION
#define CONFIG_EFI_PARTITION
#define CONFIG_PARTITION_UUIDS

/* Enable "date" command to set and get date from RTC */
#define CONFIG_CMD_FAT			/* FAT support			*/
#define CONFIG_CMD_EXT2			/* EXT2fs support		*/
#define CONFIG_CMD_EXT4			/* EXT4fs support		*/
#define CONFIG_CMD_LOADB
#undef CONFIG_CMD_LOADS

#undef CONFIG_CMD_OCTEON
#undef CONFIG_CMD_OCTEON_ELF
/*#undef CONFIG_CMD_OCTEON_MEM*/
#undef CONFIG_CMD_OCTEON_LINUX
#undef CONFIG_CMD_STRINGS
#undef CONFIG_CMD_SETEXPR
#undef CONFIG_CMD_MISC
#undef CONFIG_CMD_ITEST
#undef CONFIG_CMD_OCTEON_REGINFO
#undef CONFIG_CMD_ITEST
#undef CONFIG_CMD_ASKENV
#undef CONFIG_CMD_EDITENV
#undef CONFIG_CMD_SAVEENV
#undef CONFIG_CMD_CONSOLE
#undef CONFIG_CMD_SOURCE
#undef CONFIG_CMD_XIMG
#undef CONFIG_MII
#undef CONFIG_CMD_MII
#define CONFIG_CMD_UNZIP
#undef CONFIG_CMD_BOOTM
#undef CONFIG_CMD_OCTEON_BOOTLOADER_UPDATE
/*#undef CONFIG_CMD_MEMORY*/
/*#undef CONFIG_CMD_I2C*/
/* #undef CONFIG_CMD_OCTEON_EEPROM*/
#undef CONFIG_CMD_OCTEON_TLVEEPROM

/*
 * Miscellaneous configurable options
 */
/* Environment variables that will be set by default */
#define	CONFIG_EXTRA_ENV_SETTINGS					\
        "bootcmd=bootstage3\0"						\
        "bootdelay=0\0"

/*-----------------------------------------------------------------------
 * Cache Configuration
 */
#define CONFIG_SYS_DCACHE_SIZE		(32 * 1024)
#define CONFIG_SYS_ICACHE_SIZE		(78 * 1024)

/* Include shared board configuration, consisting mainly of DRAM details. */
#include "octeon_router7000_ref_shared.h"

#endif	/* __CONFIG_H__ */
