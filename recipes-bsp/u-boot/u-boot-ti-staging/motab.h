/*
 * motab.h
 *
 */
#ifndef __CONFIG_MOTAB_H
#define __CONFIG_MOTAB_H

#include <linux/sizes.h>
#include <asm/arch/omap.h>

/*
 * DDR information.  If the CONFIG_NR_DRAM_BANKS is not defined,
 * we say (for simplicity) that we have 1 bank, always, even when
 * we have more.  We always start at 0x80000000, and we place the
 * initial stack pointer in our SRAM. Otherwise, we can define
 * CONFIG_NR_DRAM_BANKS before including this file.
 */
#define CFG_SYS_SDRAM_BASE		0x80000000
#define CFG_MAX_RAM_BANK_SIZE	(1024 << 20)	/* 1GB */

/* Timer information. */
#define CFG_SYS_TIMERBASE	0x48040000	/* Use Timer2 */

/* Clock Defines */
#define V_OSCK				24000000  /* Clock output from T2 */
#define V_SCLK				(V_OSCK)

/* NS16550 Configuration */
#define CFG_SYS_NS16550_COM1		0x44e09000	/* UART0 as debug UART */
#define CFG_SYS_NS16550_CLK		48000000

#ifdef CONFIG_SPL_BUILD
/*
#define CONFIG_SYS_NS16550_SERIAL
#ifndef CONFIG_DM_SERIAL
#define CONFIG_SYS_NS16550_REG_SIZE	(-4)
#endif
*/
#else

#ifdef CONFIG_MOTAB_DVL_BUILD
#define AUTOLOAD_ENVSET  "autoload=no\0" 
#define BOOTCMD
#else
#define AUTOLOAD_ENVSET
#define BOOTCMD  "bootcmd="                               \
                 "ubi part rootpart; "                    \
                 "ubifsmount ubi0:rootfs; "               \
                 "ubifsload ${fdtaddr} boot/motab.dtb; "  \
                 "ubifsload ${loadaddr} boot/uImage; "    \
                 "bootm ${loadaddr} - ${fdtaddr}\0"
#endif

#define CFG_EXTRA_ENV_SETTINGS                                                               \
  "fdtaddr=0x88000000\0"                                                                     \
  "rdaddr=0x88080000\0"                                                                      \
  "mtdids=nand0=usernand\0"                                                                  \
  "mtdparts=mtdparts=usernand:-(rootpart)\0"                                                                \
  AUTOLOAD_ENVSET                                                                                           \
  BOOTCMD

#endif

/*
 * GPMC NAND block.  We support 1 device and the physical address to
 * access CS0 at is 0x8000000.
 */
#ifdef CONFIG_MTD_RAW_NAND
#ifndef CFG_SYS_NAND_BASE
#define CFG_SYS_NAND_BASE		0x8000000
#endif
/*#define CONFIG_SYS_MAX_NAND_DEVICE	1*/

#define CFG_SYS_NAND_ECCPOS		{ 2, 3, 4, 5, 6, 7, 8, 9, \
					 10, 11, 12, 13, 14, 15, 16, 17, \
					 18, 19, 20, 21, 22, 23, 24, 25, \
					 26, 27, 28, 29, 30, 31, 32, 33, \
					 34, 35, 36, 37, 38, 39, 40, 41, \
					 42, 43, 44, 45, 46, 47, 48, 49, \
					 50, 51, 52, 53, 54, 55, 56, 57, }

#define CFG_SYS_NAND_ECCSIZE   512
#define CFG_SYS_NAND_ECCBYTES  14
#endif /* CONFIG_MTD_RAW_NAND */


#endif	/* ! __CONFIG_MOTAB_H */
