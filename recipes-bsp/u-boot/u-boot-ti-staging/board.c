// SPDX-License-Identifier: GPL-2.0+
/*
 * board.c
 * Board functions for TI AM335X based boards
 */

#include <common.h>
#include <dm.h>
#include <dm/root.h>
#include <env.h>
#include <errno.h>
#include <image.h>
#include <init.h>
#include <malloc.h>
#include <net.h>
#include <spl.h>
#include <serial.h>
#include <asm/arch/cpu.h>
#include <asm/arch/hardware.h>
#include <asm/arch/omap.h>
#include <asm/arch/ddr_defs.h>
#include <asm/arch/clock.h>
#include <asm/arch/gpio.h>
#include <asm/arch/sys_proto.h>
#include <asm/arch/mem.h>
#include <asm/arch/mux.h>
#include <asm/global_data.h>
#include <asm/io.h>
#include <asm/emif.h>
#include <asm/gpio.h>
#include <asm/omap_common.h>
#include <i2c.h>
#include <miiphy.h>
#include <cpsw.h>
#include <linux/bitops.h>
#include <linux/delay.h>
#include <power/tps65217.h>
#include <env_internal.h>
#include <stdio_dev.h>
#include <debug_uart.h>
#include <video.h>
#include <cpu_func.h>

#include <fdt_simplefb.h>

DECLARE_GLOBAL_DATA_PTR;

#if !CONFIG_IS_ENABLED(SKIP_LOWLEVEL_INIT)

static struct ctrl_dev *cdev = (struct ctrl_dev *)CTRL_DEVICE_BASE;

#define PULL_DISABLE         (1 << 3)
#define PULL_UP              (1 << 4)
#define INPUT_EN             (1 << 5)

#define PIN_INPUT            (INPUT_EN | PULL_DISABLE)
#define PIN_INPUT_PULLUP     (INPUT_EN | PULL_UP)
#define PIN_INPUT_PULLDOWN	  (INPUT_EN)

#define PIN_OUTPUT           (PULL_DISABLE)
#define PIN_OUTPUT_PULLUP    (PULL_UP)
#define PIN_OUTPUT_PULLDOWN  0

#define NO_PULL      (0b0001000)
#define PULLED_DOWN  (0b0000000)
#define PULLED_UP    (0b0010000)
#define RX_ACTIVE    (0b0100000)
#define SLEW_CTRL    (0b1000000)

static struct module_pin_mux uart0_pin_mux[] = {
  {OFFSET(uart0_rxd), MODE(0) | PULLED_UP   | RX_ACTIVE },
  {OFFSET(uart0_txd), MODE(0) | PULLED_UP               },
  {-1},
};

static struct module_pin_mux i2c0_pin_mux[] = {
  {OFFSET(i2c0_scl), MODE(0) | NO_PULL | RX_ACTIVE | SLEWCTRL },
  {OFFSET(i2c0_sda), MODE(0) | NO_PULL | RX_ACTIVE | SLEWCTRL },
  {-1},
};

static struct module_pin_mux spi0_pin_mux[] = {
  {OFFSET(spi0_sclk), MODE(0) | PULLED_DOWN | RX_ACTIVE },
  {OFFSET(spi0_d0),   MODE(0) | PULLED_UP   | RX_ACTIVE },
  {OFFSET(spi0_d1),   MODE(0) | PULLED_DOWN | RX_ACTIVE },
  {OFFSET(spi0_cs0),  MODE(0) | PULLED_UP   | RX_ACTIVE },
  {-1},
};

static struct module_pin_mux mii1_pin_mux[] = {
  {OFFSET(mii1_col),   MODE(0) | PULLED_DOWN | RX_ACTIVE },
  {OFFSET(mii1_crs),   MODE(0) | PULLED_DOWN | RX_ACTIVE },
  {OFFSET(mii1_rxerr), MODE(0) | PULLED_DOWN | RX_ACTIVE },
  {OFFSET(mii1_txen),  MODE(0) | NO_PULL                 },
  {OFFSET(mii1_rxdv),  MODE(0) | PULLED_DOWN | RX_ACTIVE },
  {OFFSET(mii1_txclk), MODE(0) | PULLED_UP   | RX_ACTIVE },
  {OFFSET(mii1_rxclk), MODE(0) | PULLED_UP   | RX_ACTIVE },
  {OFFSET(mii1_txd0),  MODE(0) | NO_PULL                 },
  {OFFSET(mii1_txd1),  MODE(0) | NO_PULL                 },
  {OFFSET(mii1_txd2),  MODE(0) | NO_PULL                 },
  {OFFSET(mii1_txd3),  MODE(0) | NO_PULL                 },
  {OFFSET(mii1_rxd0),  MODE(0) | PULLED_DOWN | RX_ACTIVE },
  {OFFSET(mii1_rxd1),  MODE(0) | PULLED_DOWN | RX_ACTIVE },
  {OFFSET(mii1_rxd2),  MODE(0) | PULLED_DOWN | RX_ACTIVE },
  {OFFSET(mii1_rxd3),  MODE(0) | PULLED_DOWN | RX_ACTIVE },
  {OFFSET(mdio_clk),   MODE(0) | PULLED_UP               },
  {OFFSET(mdio_data),  MODE(0) | PULLED_UP   | RX_ACTIVE },
  {-1},
};

static struct module_pin_mux nand_pin_mux[] = {
  {OFFSET(gpmc_ad0),      MODE(0) | NO_PULL     | RX_ACTIVE },
  {OFFSET(gpmc_ad1),      MODE(0) | NO_PULL     | RX_ACTIVE },
  {OFFSET(gpmc_ad2),      MODE(0) | NO_PULL     | RX_ACTIVE },
  {OFFSET(gpmc_ad3),      MODE(0) | NO_PULL     | RX_ACTIVE },
  {OFFSET(gpmc_ad4),      MODE(0) | NO_PULL     | RX_ACTIVE },
  {OFFSET(gpmc_ad5),      MODE(0) | NO_PULL     | RX_ACTIVE },
  {OFFSET(gpmc_ad6),      MODE(0) | NO_PULL     | RX_ACTIVE },
  {OFFSET(gpmc_ad7),	     MODE(0) | NO_PULL     | RX_ACTIVE },
  {OFFSET(gpmc_wait0),    MODE(0) | NO_PULL     | RX_ACTIVE },
  {OFFSET(gpmc_wpn),      MODE(0) | NO_PULL                 },
  {OFFSET(gpmc_csn0),     MODE(0) | NO_PULL                 },
  {OFFSET(gpmc_wen),      MODE(0) | PULLED_DOWN             },
  {OFFSET(gpmc_oen_ren),  MODE(0) | PULLED_DOWN             },
  {OFFSET(gpmc_advn_ale), MODE(0) | PULLED_DOWN             },
  {OFFSET(gpmc_be0n_cle), MODE(0) | PULLED_DOWN             },
  {-1},
};

static struct module_pin_mux lcdc_pin_mux[] = {
  {OFFSET(lcd_vsync),      MODE(0) | PULLED_DOWN },
  {OFFSET(lcd_hsync),      MODE(0) | PULLED_DOWN },
  {OFFSET(lcd_pclk),       MODE(0) | PULLED_DOWN },
  {OFFSET(lcd_ac_bias_en), MODE(0) | PULLED_DOWN },
  {OFFSET(lcd_data0),      MODE(0) | NO_PULL     },
  {OFFSET(lcd_data1),      MODE(0) | NO_PULL     },
  {OFFSET(lcd_data2),      MODE(0) | NO_PULL     },
  {OFFSET(lcd_data3),      MODE(0) | NO_PULL     },
  {OFFSET(lcd_data4),      MODE(0) | NO_PULL     },
  {OFFSET(lcd_data5),      MODE(0) | NO_PULL     },
  {OFFSET(lcd_data6),      MODE(0) | NO_PULL     },
  {OFFSET(lcd_data7),      MODE(0) | NO_PULL     },
  {OFFSET(lcd_data8),      MODE(0) | NO_PULL     },
  {OFFSET(lcd_data9),      MODE(0) | NO_PULL     },
  {OFFSET(lcd_data10),     MODE(0) | NO_PULL     },
  {OFFSET(lcd_data11),     MODE(0) | NO_PULL     },
  {OFFSET(lcd_data12),     MODE(0) | NO_PULL     },
  {OFFSET(lcd_data13),     MODE(0) | NO_PULL     },
  {OFFSET(lcd_data14),     MODE(0) | NO_PULL     },
  {OFFSET(lcd_data15),     MODE(0) | NO_PULL     },
  {-1},
};

void set_uart_mux_conf(void)
{
	configure_module_pin_mux(uart0_pin_mux);
}

void set_mux_conf_regs(void)
{
   configure_module_pin_mux(i2c0_pin_mux);
   configure_module_pin_mux(mii1_pin_mux);
   configure_module_pin_mux(lcdc_pin_mux);   
   configure_module_pin_mux(spi0_pin_mux);
   configure_module_pin_mux(nand_pin_mux);
}

const struct dpll_params *get_dpll_ddr_params(void)
{
	int ind = get_sys_clk_index();

   return &dpll_ddr3_400MHz[ind];
}

const struct dpll_params *get_dpll_mpu_params(void)
{
	int ind = get_sys_clk_index();
	int freq = am335x_get_efuse_mpu_max_freq(cdev);

	freq = MPUPLL_M_1000;

	switch (freq) {
	case MPUPLL_M_1000:
		return &dpll_mpu_opp[ind][5];
	case MPUPLL_M_800:
		return &dpll_mpu_opp[ind][4];
	case MPUPLL_M_720:
		return &dpll_mpu_opp[ind][3];
	case MPUPLL_M_600:
		return &dpll_mpu_opp[ind][2];
	case MPUPLL_M_500:
		return &dpll_mpu_opp100;
	case MPUPLL_M_300:
		return &dpll_mpu_opp[ind][0];
	}

	return &dpll_mpu_opp[ind][0];
}

void scale_vcores(void)
{
	int freq, usb_cur_lim, mpu_vdd;

	if (power_tps65217_init(0))
		return;

	freq = am335x_get_efuse_mpu_max_freq(cdev);
	
	//freq = MPUPLL_M_1000;

	switch (freq) {
	case MPUPLL_M_1000:
		mpu_vdd = TPS65217_DCDC_VOLT_SEL_1325MV;
		usb_cur_lim = TPS65217_USB_INPUT_CUR_LIMIT_1800MA;
		break;
	case MPUPLL_M_800:
		mpu_vdd = TPS65217_DCDC_VOLT_SEL_1275MV;
		usb_cur_lim = TPS65217_USB_INPUT_CUR_LIMIT_1300MA;
		break;
	case MPUPLL_M_720:
		mpu_vdd = TPS65217_DCDC_VOLT_SEL_1200MV;
		usb_cur_lim = TPS65217_USB_INPUT_CUR_LIMIT_1300MA;
		break;
	case MPUPLL_M_600:
	case MPUPLL_M_500:
	case MPUPLL_M_300:
	default:
		mpu_vdd = TPS65217_DCDC_VOLT_SEL_1100MV;
		usb_cur_lim = TPS65217_USB_INPUT_CUR_LIMIT_1300MA;
		break;
	}

	if (tps65217_reg_write(TPS65217_PROT_LEVEL_NONE,
			       TPS65217_POWER_PATH,
			       usb_cur_lim,
			       TPS65217_USB_INPUT_CUR_LIMIT_MASK))
		puts("tps65217_reg_write failure\n");

	/* Set DCDC2 (MPU) voltage */
	if (tps65217_voltage_update(TPS65217_DEFDCDC2, mpu_vdd)) {
		puts("tps65217_voltage_update failure\n");
		return;
	}
}

static const struct ddr_data ddr3_beagleblack_data = {
	.datardsratio0 = MT41K256M16HA125E_RD_DQS,
	.datawdsratio0 = MT41K256M16HA125E_WR_DQS,
	.datafwsratio0 = MT41K256M16HA125E_PHY_FIFO_WE,
	.datawrsratio0 = MT41K256M16HA125E_PHY_WR_DATA,
};

static const struct cmd_control ddr3_beagleblack_cmd_ctrl_data = {
	.cmd0csratio = MT41K256M16HA125E_RATIO,
	.cmd0iclkout = MT41K256M16HA125E_INVERT_CLKOUT,

	.cmd1csratio = MT41K256M16HA125E_RATIO,
	.cmd1iclkout = MT41K256M16HA125E_INVERT_CLKOUT,

	.cmd2csratio = MT41K256M16HA125E_RATIO,
	.cmd2iclkout = MT41K256M16HA125E_INVERT_CLKOUT,
};

static struct emif_regs ddr3_beagleblack_emif_reg_data = {
	.sdram_config = MT41K256M16HA125E_EMIF_SDCFG,
	.ref_ctrl = MT41K256M16HA125E_EMIF_SDREF,
	.sdram_tim1 = MT41K256M16HA125E_EMIF_TIM1,
	.sdram_tim2 = MT41K256M16HA125E_EMIF_TIM2,
	.sdram_tim3 = MT41K256M16HA125E_EMIF_TIM3,
	.ocp_config = 0x00141414,
	.zq_config = MT41K256M16HA125E_ZQ_CFG,
	.emif_ddr_phy_ctlr_1 = MT41K256M16HA125E_EMIF_READ_LATENCY,
};

const struct ctrl_ioregs ioregs_bonelt = {
	.cm0ioctl		= MT41K256M16HA125E_IOCTRL_VALUE,
	.cm1ioctl		= MT41K256M16HA125E_IOCTRL_VALUE,
	.cm2ioctl		= MT41K256M16HA125E_IOCTRL_VALUE,
	.dt0ioctl		= MT41K256M16HA125E_IOCTRL_VALUE,
	.dt1ioctl		= MT41K256M16HA125E_IOCTRL_VALUE,
};

void sdram_init(void)
{

// printf("FDT %p gd %p\n", gd->fdt_blob, gd);
 printf("%s: gd flags = %08lx\n", __func__, gd->flags); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// printf("%s: ram_base = %08lx\n", __func__, gd->ram_base); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// printf("%s: ram_top = %08lx\n", __func__, gd->ram_top); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// printf("%s: relocaddr = %08lx\n", __func__, gd->relocaddr); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// printf("%s: ram_size = %08lx\n", __func__, gd->ram_size); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// printf("%s: start_addr_sp = %08lx\n", __func__, gd->start_addr_sp); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// printf("%s: malloc_base = %08lx\n", __func__, gd->malloc_base); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// printf("%s: malloc_limit = %08lx\n", __func__, gd->malloc_limit); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// printf("%s: malloc_ptr = %08lx\n", __func__, gd->malloc_ptr); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


   config_ddr(400, &ioregs_bonelt,
      &ddr3_beagleblack_data,
      &ddr3_beagleblack_cmd_ctrl_data,
      &ddr3_beagleblack_emif_reg_data, 0);
}


void am33xx_spl_board_init(void)
{
//   ulong addr;
//   int ret;

 printf("%s: P_1\n", __func__); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   dram_init_banksize();

// printf("FDT %p gd %p\n", gd->fdt_blob, gd);
// printf("%s: flags = %08lx\n", __func__, gd->flags); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// printf("%s: ram_base = %08lx\n", __func__, gd->ram_base); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// printf("%s: ram_top = %08lx\n", __func__, gd->ram_top); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// printf("%s: relocaddr = %08lx\n", __func__, gd->relocaddr); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// printf("%s: ram_size = %08lx\n", __func__, gd->ram_size); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// printf("%s: start_addr_sp = %08lx\n", __func__, gd->start_addr_sp); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// printf("%s: malloc_base = %08lx\n", __func__, gd->malloc_base); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// printf("%s: malloc_limit = %08lx\n", __func__, gd->malloc_limit); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// printf("%s: malloc_ptr = %08lx\n", __func__, gd->malloc_ptr); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


//   gd->ram_base = CFG_SYS_SDRAM_BASE;
//   gd->ram_top = gd->ram_base + gd->ram_size;
//   gd->relocaddr = gd->ram_top;

   /* Round memory pointer down to next 4 kB limit */
//   gd->relocaddr &= ~(4096 - 1);  

//   arch_reserve_mmu();

//   gd->flags |= GD_FLG_RELOC;

//   ret = dm_init_and_scan(false);

// printf("%s: P_2\n", __func__); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

//   enable_caches();

// printf("%s: P_3\n", __func__); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

//   ret = stdio_init_tables();
//   ret = stdio_add_devices();

//   gd->flags &= ~GD_FLG_RELOC;


//   u32 regval;
//   // Output high on GPIO_1_17
//   regval=__raw_readl(0x4804C000 + 0x138);
//   __raw_writel((regval | 0x00020000), (0x4804C000 + 0x13C));

}

#if !CONFIG_IS_ENABLED(OF_CONTROL)

/*
 * Node /ocp/interconnect@48000000/segment@0/target-module@30000/spi@0/is25lp040e@0 index 22
 * driver jedec_spi_nor parent None
 */
//static struct dtd_jedec_spi_nor dtv_is25lp040e_at_0 = {
//	.reg			= {0x0},
//	.spi_max_frequency	= 0x16e3600,
//};
//U_BOOT_DRVINFO(is25lp040e_at_0) = {
//	.name		= "jedec_spi_nor",
//	.plat		= &dtv_is25lp040e_at_0,
//	.plat_size	= sizeof(dtv_is25lp040e_at_0),
//	.parent_idx	= 32,
//};

#if CONFIG_IS_ENABLED(NET)
/* CPSW plat */
struct cpsw_slave_data slave_data[] = {
	{
		.slave_reg_ofs  = CPSW_SLAVE0_OFFSET,
		.sliver_reg_ofs = CPSW_SLIVER0_OFFSET,
		.phy_addr       = 0,
	},
	{
		.slave_reg_ofs  = CPSW_SLAVE1_OFFSET,
		.sliver_reg_ofs = CPSW_SLIVER1_OFFSET,
		.phy_addr       = 1,
	},
};

struct cpsw_platform_data am335_eth_data = {
	.cpsw_base		= CPSW_BASE,
	.version		= CPSW_CTRL_VERSION_2,
	.bd_ram_ofs		= CPSW_BD_OFFSET,
	.ale_reg_ofs		= CPSW_ALE_OFFSET,
	.cpdma_reg_ofs		= CPSW_CPDMA_OFFSET,
	.mdio_div		= CPSW_MDIO_DIV,
	.host_port_reg_ofs	= CPSW_HOST_PORT_OFFSET,
	.channels		= 8,
	.slaves			= 2,
	.slave_data		= slave_data,
	.ale_entries		= 1024,
	.mac_control		= 0x20,
	.active_slave		= 0,
	.mdio_base		= 0x4a101000,
	.gmii_sel		= 0x44e10650,
	.phy_sel_compat		= "ti,am3352-cpsw-phy-sel",
	.syscon_addr		= 0x44e10630,
	.macid_sel_compat	= "cpsw,am33xx",
};

struct eth_pdata cpsw_pdata = {
	.iobase = 0x4a100000,
	.phy_interface = 0,
	.priv_pdata = &am335_eth_data,
};

U_BOOT_DRVINFO(am335x_eth) = {
	.name = "eth_cpsw",
	.plat = &cpsw_pdata,
};
#endif
#endif
#endif  /*  !CONFIG_IS_ENABLED(SKIP_LOWLEVEL_INIT)  */


#ifdef CONFIG_DEBUG_UART_BOARD_INIT
void board_debug_uart_init(void)
{

//   setup_early_clocks();
//   set_uart_mux_conf();

/* For debugging, setup the gpio that user LEDs are connected to plus GPIO1_17   */
   __raw_writel(0x17, (0x44E10000 + 0x844));  // control module: GPIO1_17/gpmc_a1 (routed to P9:23)
   __raw_writel(0x17, (0x44E10000 + 0x854));  // control module: GPIO1_21/gpmc_a5 USR0 (D2)
   __raw_writel(0x17, (0x44E10000 + 0x858));  // control module: GPIO1_22/gpmc_a6 USR1 (D3)
   __raw_writel(0x17, (0x44E10000 + 0x85C));  // control module: GPIO1_23/gpmc_a7 USR2 (D3)
   __raw_writel(0x17, (0x44E10000 + 0x860));  // control module: GPIO1_24/gpmc_a8 USR3 (D4)

   __raw_writel(0x0, (0x4804C000 + 0x130));   // GPIO1 CTRL: enable
   
   u32 regval=0;
   regval=__raw_readl(0x4804C000 + 0x134);  // GPIO_OE
   __raw_writel((regval & 0xFE1DFFFF), (0x4804C000 + 0x134));  // set those 5 gpios as outputs

   // Turn on USER LED0: as the earliest sign of life
   regval=__raw_readl(0x4804C000 + 0x138);
   __raw_writel((regval | 0x00200000), (0x4804C000 + 0x13C));

}
#endif

/*
 * Basic board specific setup.  Pinmux has been handled already.
 */
int board_init(void)
{
	gd->bd->bi_boot_params = CFG_SYS_SDRAM_BASE + 0x100;
#if defined(CONFIG_NOR) || defined(CONFIG_MTD_RAW_NAND)
	gpmc_init();
#endif
	return 0;
}

#ifdef CONFIG_BOARD_LATE_INIT
int board_late_init(void)
{
#if !defined(CONFIG_SPL_BUILD)
	uint8_t mac_addr[6];
	uint32_t mac_hi, mac_lo;

	/* try reading mac address from efuse */
	mac_lo = readl(&cdev->macid0l);
	mac_hi = readl(&cdev->macid0h);
	mac_addr[0] = mac_hi & 0xFF;
	mac_addr[1] = (mac_hi & 0xFF00) >> 8;
	mac_addr[2] = (mac_hi & 0xFF0000) >> 16;
	mac_addr[3] = (mac_hi & 0xFF000000) >> 24;
	mac_addr[4] = mac_lo & 0xFF;
	mac_addr[5] = (mac_lo & 0xFF00) >> 8;

	if (!env_get("ethaddr")) {
		printf("<ethaddr> not set. Validating first E-fuse MAC\n");

		if (is_valid_ethaddr(mac_addr))
			eth_env_set_enetaddr("ethaddr", mac_addr);
	}

	mac_lo = readl(&cdev->macid1l);
	mac_hi = readl(&cdev->macid1h);
	mac_addr[0] = mac_hi & 0xFF;
	mac_addr[1] = (mac_hi & 0xFF00) >> 8;
	mac_addr[2] = (mac_hi & 0xFF0000) >> 16;
	mac_addr[3] = (mac_hi & 0xFF000000) >> 24;
	mac_addr[4] = mac_lo & 0xFF;
	mac_addr[5] = (mac_lo & 0xFF00) >> 8;

	if (!env_get("eth1addr")) {
		if (is_valid_ethaddr(mac_addr))
			eth_env_set_enetaddr("eth1addr", mac_addr);
	}
#endif

	return 0;
}
#endif

#if defined(CONFIG_OF_BOARD_SETUP) && defined(CONFIG_OF_CONTROL) 

int ft_board_setup(void *blob, struct bd_info *bd)
{
	int __maybe_unused res;


 printf("%s: P_1\n", __func__); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

//	if (IS_ENABLED(CONFIG_FDT_SIMPLEFB))
//		fdt_simplefb_enable_and_mem_rsv(blob);

 printf("%s: P_2\n", __func__); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


 u32 regval;
 // Output high on GPIO_1_17
 regval=__raw_readl(0x4804C000 + 0x138);
 __raw_writel((regval | 0x00020000), (0x4804C000 + 0x13C));


	return 0;
}
#endif

