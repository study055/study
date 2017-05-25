/* linux/arch/arm/mach-s5pv210/mach-smdkc110.c
 *
 * Copyright (c) 2010 Samsung Electronics Co., Ltd.
 *		http://www.samsung.com/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/init.h>
#include <linux/serial_core.h>
#include <linux/gpio.h>
#include <linux/videodev2.h>
#include <linux/i2c.h>
#include <linux/regulator/max8698.h>
#include <linux/clk.h>
#include <linux/delay.h>
#include <linux/usb/ch9.h>
#include <linux/pwm_backlight.h>
#include <linux/spi/spi.h>

#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/setup.h>
#include <asm/mach-types.h>

#include <mach/map.h>
#include <mach/regs-clock.h>
#include <mach/regs-mem.h>
#include <mach/regs-gpio.h>
#include <mach/gpio-bank.h>
#include <mach/ts.h>
#include <mach/adc.h>

#include <media/s5k3ba_platform.h>
#include <media/s5k4ba_platform.h>
#include <media/s5k4ea_platform.h>
#include <media/s5k6aa_platform.h>
#include <media/ov7670_platform.h>
#include <media/bf3703_platform.h>

#include <plat/regs-serial.h>
#include <plat/s5pv210.h>
#include <plat/devs.h>
#include <plat/cpu.h>
#include <plat/fb.h>
#include <plat/gpio-cfg.h>
#include <plat/iic.h>
#include <plat/spi.h>
#include <plat/fimc.h>
//#include <plat/jpeg.h>  //liao
#include <plat/csis.h>
#include <plat/mfc.h>
#include <plat/sdhci.h>
#include <plat/ide.h>
#include <plat/regs-otg.h>
#include <plat/clock.h>
#include <mach/gpio-bank-b.h>
//add by jhuang
#include <media/GT2005_platform.h>
#include <media/gc0308_platform.h>//add by jhuang

/*
#ifdef CONFIG_ANDROID_PMEM
#include <linux/android_pmem.h>
#include <plat/media.h>
#endif
// added   2011.02.24 
#include <plat/gpio-cfg.h>
#include <linux/gpio.h>
#include <plat/regs-rtc.h>
#include <plat/map-base.h>
#include <linux/io.h>
//-----
*/
#ifdef CONFIG_I2C_GPIO
#include <linux/i2c-gpio.h>
#include <mach/gpio.h>
//#include <plat/gpio-cfg.h>
#endif
 //[david.modify] 2010-12-21 20:26
#include <kernel_david.h>
#include <linux/delay.h>
extern int act8937_write(unsigned int adr, unsigned int val);
extern int act8937_read(unsigned int adr);

#if defined(CONFIG_PM)
#include <plat/pm.h>
#endif

#ifdef CONFIG_SENSORS_MMC31XX
#include <linux/mmc31xx.h>
#endif

/* Following are default values for UCON, ULCON and UFCON UART registers */
#define S5PV210_UCON_DEFAULT	(S3C2410_UCON_TXILEVEL |	\
				 S3C2410_UCON_RXILEVEL |	\
				 S3C2410_UCON_TXIRQMODE |	\
				 S3C2410_UCON_RXIRQMODE |	\
				 S3C2410_UCON_RXFIFO_TOI |	\
				 S3C2443_UCON_RXERR_IRQEN)

#define S5PV210_ULCON_DEFAULT	S3C2410_LCON_CS8

#define S5PV210_UFCON_DEFAULT	(S3C2410_UFCON_FIFOMODE |	\
				 S5PV210_UFCON_TXTRIG4 |	\
				 S5PV210_UFCON_RXTRIG4)

extern void s5pv210_reserve_bootmem(void);
extern void s3c_sdhci_set_platdata(void);

static struct s3c2410_uartcfg smdkv210_uartcfgs[] __initdata = {
	[0] = {
		.hwport		= 0,
		.flags		= 0,
		.ucon		= S5PV210_UCON_DEFAULT,
		.ulcon		= S5PV210_ULCON_DEFAULT,
		.ufcon		= S5PV210_UFCON_DEFAULT,
	},
	[1] = {
		.hwport		= 1,
		.flags		= 0,
		.ucon		= S5PV210_UCON_DEFAULT,
		.ulcon		= S5PV210_ULCON_DEFAULT,
		.ufcon		= S5PV210_UFCON_DEFAULT,
	},
	[2] = {
		.hwport		= 2,
		.flags		= 0,
		.ucon		= S5PV210_UCON_DEFAULT,
		.ulcon		= S5PV210_ULCON_DEFAULT,
		.ufcon		= S5PV210_UFCON_DEFAULT,
	},
	[3] = {
		.hwport		= 3,
		.flags		= 0,
		.ucon		= S5PV210_UCON_DEFAULT,
		.ulcon		= S5PV210_ULCON_DEFAULT,
		.ufcon		= S5PV210_UFCON_DEFAULT,
	},
};

/* PMIC */
#ifdef CONFIG_REGULATOR_MAX8698
static struct regulator_consumer_supply buck1_consumers[] = {
	{
		.supply		= "vddarm",
	},
};

static struct regulator_init_data max8698_buck1_data = {
	.constraints	= {
		.name		= "VCC_ARM",
		.min_uV		=  750000,
		.max_uV		= 1500000,
		.always_on	= 1,
		.valid_ops_mask	= REGULATOR_CHANGE_VOLTAGE,
		.state_mem	= {
			.uV	= 1250000,
			.mode	= REGULATOR_MODE_NORMAL,
			.enabled = 0,
		},
	},
	.num_consumer_supplies	= ARRAY_SIZE(buck1_consumers),
	.consumer_supplies	= buck1_consumers,
};

static struct regulator_consumer_supply buck2_consumers[] = {
	{
		.supply		= "vddint",
	},
};

static struct regulator_init_data max8698_buck2_data = {
	.constraints	= {
		.name		= "VCC_INTERNAL",
		.min_uV		= 950000,
		.max_uV		= 1200000,
		.always_on	= 1,
		.valid_ops_mask	= REGULATOR_CHANGE_VOLTAGE,
		.state_mem	= {
			.uV	= 1100000,
			.mode	= REGULATOR_MODE_NORMAL,
			.enabled = 0,
		},
	},
	.num_consumer_supplies	= ARRAY_SIZE(buck2_consumers),
	.consumer_supplies	= buck2_consumers,
};

static struct regulator_init_data max8698_buck3_data = {
	.constraints	= {
		.name		= "VCC_MEM",
		.min_uV		= 1800000,
		.max_uV		= 1800000,
		.apply_uV	= 1,
		.state_mem	= {
			.uV	= 1800000,
			.mode	= REGULATOR_MODE_NORMAL,
			.enabled = 1,
		},
		/* .initial_state	= PM_SUSPEND_MEM, */
	},
};

static struct regulator_init_data max8698_ldo2_data = {
	.constraints	= {
		.name		= "VALIVE_1.1V",
		.min_uV		= 1100000,
		.max_uV		= 1100000,
		.apply_uV	= 1,
		.always_on	= 1,
		.state_mem	= {
			.uV	= 1100000,
			.mode	= REGULATOR_MODE_NORMAL,
			.enabled = 1,
		},
	},
};

static struct regulator_consumer_supply ldo3_consumers[] = {
	{
		.supply		= "vddusb_dig",
	},
};

static struct regulator_init_data max8698_ldo3_data = {
	.constraints	= {
		.name		= "VUOTG_D_1.1V/VUHOST_D_1.1V",
		.min_uV		= 1100000,
		.max_uV		= 1100000,
		.apply_uV	= 1,
		.boot_on	= 1,
		.valid_ops_mask	= REGULATOR_CHANGE_STATUS,
		.state_mem = {
			.uV		= 1100000,
			.mode		= REGULATOR_MODE_NORMAL,
			.enabled	= 1,	/* LDO3 should be OFF in sleep mode */
		},
	},
	.num_consumer_supplies	= ARRAY_SIZE(ldo3_consumers),
	.consumer_supplies	= ldo3_consumers,
};

static struct regulator_consumer_supply ldo4_consumers[] = {
	{
		.supply		= "vddmipi",
	},
};

static struct regulator_init_data max8698_ldo4_data = {
	.constraints	= {
		.name		= "V_MIPI_1.8V",
		.min_uV		= 1800000,
		.max_uV		= 1800000,
		.apply_uV	= 1,
		.boot_on	= 1,
		.valid_ops_mask	= REGULATOR_CHANGE_STATUS,
		.state_mem = {
			.uV		= 1800000,
			.mode		= REGULATOR_MODE_NORMAL,
			.enabled	= 0,	/* LDO4 should be OFF in sleep mode */
		},
	},
	.num_consumer_supplies	= ARRAY_SIZE(ldo4_consumers),
	.consumer_supplies	= ldo4_consumers,
};

static struct regulator_init_data max8698_ldo5_data = {
	.constraints	= {
		.name		= "VMMC_2.8V/VEXT_2.8V",
		.min_uV		= 2800000,
		.max_uV		= 2800000,
		.apply_uV	= 1,
		.state_mem = {
			.uV		= 2800000,
			.mode		= REGULATOR_MODE_NORMAL,
			.enabled	= 1,
		},
	},
};

static struct regulator_consumer_supply ldo6_consumers[] = {
	{
		.supply		= "vddlcd",
	},
};

static struct regulator_init_data max8698_ldo6_data = {
	.constraints	= {
		.name		= "VCC_2.6V",
		.min_uV		= 2600000,
		.max_uV		= 2600000,
		.apply_uV	= 1,
		.boot_on	= 1,
		.valid_ops_mask	= REGULATOR_CHANGE_STATUS,
		.state_mem = {
			.uV		= 2600000,
			.mode		= REGULATOR_MODE_NORMAL,
			.enabled	= 0,
		},
	},
	.num_consumer_supplies	= ARRAY_SIZE(ldo6_consumers),
	.consumer_supplies	= ldo6_consumers,
};

static struct regulator_consumer_supply ldo7_consumers[] = {
	{
		.supply		= "vddmodem",
	},
};

static struct regulator_init_data max8698_ldo7_data = {
	.constraints	= {
		.name		= "VDAC_2.8V",
		.min_uV		= 2800000,
		.max_uV		= 2800000,
		.apply_uV	= 1,
		.boot_on	= 1,
		.valid_ops_mask	= REGULATOR_CHANGE_STATUS,
		.state_mem = {
			.uV		= 2800000,
			.mode		= REGULATOR_MODE_NORMAL,
			.enabled	= 1,
		},
	},
	.num_consumer_supplies	= ARRAY_SIZE(ldo7_consumers),
	.consumer_supplies	= ldo7_consumers,
};

static struct regulator_consumer_supply ldo8_consumers[] = {
	{
		.supply		= "vddusb_anlg",
	},
};

static struct regulator_init_data max8698_ldo8_data = {
	.constraints	= {
		.name		= "VUOTG_A_3.3V/VUHOST_A_3.3V",
		.min_uV		= 3300000,
		.max_uV		= 3300000,
		.apply_uV	= 1,
		.boot_on	= 1,
		.valid_ops_mask	= REGULATOR_CHANGE_STATUS,
		.state_mem = {
			.uV		= 0,
			.mode		= REGULATOR_MODE_NORMAL,
			.enabled	= 1,	/* LDO8 should be OFF in sleep mode */
		},
	},
	.num_consumer_supplies	= ARRAY_SIZE(ldo8_consumers),
	.consumer_supplies	= ldo8_consumers,
};

static struct regulator_init_data max8698_ldo9_data = {
	.constraints	= {
		.name		= "{VADC/VSYS/VKEY}_2.8V",
		.min_uV		= 3000000,
		.max_uV		= 3000000,
		.apply_uV	= 1,
		.state_mem = {
			.uV		= 3000000,
			.mode		= REGULATOR_MODE_NORMAL,
			.enabled	= 1,
		},
	},
};

static struct max8698_subdev_data smdkc110_regulators[] = {
	{ MAX8698_LDO2, &max8698_ldo2_data },
	{ MAX8698_LDO3, &max8698_ldo3_data },
	{ MAX8698_LDO4, &max8698_ldo4_data },
	{ MAX8698_LDO5, &max8698_ldo5_data },
	{ MAX8698_LDO6, &max8698_ldo6_data },
	{ MAX8698_LDO7, &max8698_ldo7_data },
	{ MAX8698_LDO8, &max8698_ldo8_data },
	{ MAX8698_LDO9, &max8698_ldo9_data },
	{ MAX8698_BUCK1, &max8698_buck1_data },
	{ MAX8698_BUCK2, &max8698_buck2_data },
	{ MAX8698_BUCK3, &max8698_buck3_data },
};

/* 800MHz default voltage */
#if 0
static struct max8698_platform_data max8698_platform_data_0 = {
	.num_regulators	= ARRAY_SIZE(smdkc110_regulators),
	.regulators	= smdkc110_regulators,

	.set1		= S5PV210_GPH1(6),
	.set2		= S5PV210_GPH1(7),
	.set3		= S5PV210_GPH0(4),

	.dvsarm1	= 0x8,
	.dvsarm2	= 0x6,
	.dvsarm3	= 0x5,
	.dvsarm4	= 0x5,

	.dvsint1	= 0x9,
	.dvsint2	= 0x5,
};
#endif
/* 1Ghz default voltage */
static struct max8698_platform_data max8698_platform_data = {
	.num_regulators = ARRAY_SIZE(smdkc110_regulators),
	.regulators     = smdkc110_regulators,

	.set1           = S5PV210_GPH1(6),
	.set2           = S5PV210_GPH1(7),
	.set3           = S5PV210_GPH0(4),
	.dvsarm1        = 0xa,  // 1.25v
	.dvsarm2        = 0x9,  // 1.20V
	.dvsarm3        = 0x6,  // 1.05V
	.dvsarm4        = 0x4,  // 0.95V

	.dvsint1        = 0x7,  // 1.10v
	.dvsint2        = 0x5,  // 1.00V
};
#endif

#ifdef CONFIG_TOUCHSCREEN_S3C
static struct s3c_ts_mach_info s3c_ts_platform __initdata = {
	.delay                  = 10000,
	.presc                  = 49,
	.oversampling_shift     = 2,
	.resol_bit              = 12,
	.s3c_adc_con            = ADC_TYPE_2,
};
#endif
#if 0
#ifdef CONFIG_TOUCHSCREEN_SIS_I2C
static struct sis_i2c_mach_info sis_i2c_platform __initdata= {
	.delay                  = 10000,
	.presc                  = 49,
	.oversampling_shift     = 2,
	.resol_bit              = 12,
	.s3c_adc_con            = ADC_TYPE_2,
};
#endif
#endif
#ifdef CONFIG_S5PV210_ADC
static struct s3c_adc_mach_info s3c_adc_platform __initdata = {
	/* s5pc100 supports 12-bit resolution */
	.delay  = 10000,
	.presc  = 49,
	.resolution = 12,
};
#endif

#ifdef CONFIG_VIDEO_FIMC
/*
 * External camera reset
 * Because the most of cameras take i2c bus signal, so that
 * you have to reset at the boot time for other i2c slave devices.
 * This function also called at fimc_init_camera()
 * Do optimization for cameras on your platform.
*/
static int smdkv210_cam0_power(int onoff)
{
	int err;
	printk("%s\n",__func__);
	/* Camera A */
#if 0
	//power
	err = gpio_request(S5PV210_GPB(5), "GPB");
	if (err)
		printk(KERN_ERR "#### failed to request GPB5 for CAM Forward..\n");

	s3c_gpio_setpull(S5PV210_GPB(5), S3C_GPIO_PULL_NONE);
	gpio_direction_output(S5PV210_GPB(5), 1);
	gpio_free(S5PV210_GPB(5));
/*
	err = gpio_request(S5PV210_GPB(4), "GPB");
	if (err)
		printk(KERN_ERR "#### failed to request GPB4 for CAM Back..\n");

	s3c_gpio_setpull(S5PV210_GPB(4), S3C_GPIO_PULL_NONE);
	gpio_direction_output(S5PV210_GPB(4), 0);
	gpio_free(S5PV210_GPB(4));
	udelay(50);
*/
	//reset	
	err = gpio_request(S5PV210_GPE1(4), "GPE1");
	if (err)
		printk(KERN_ERR "#### failed to request GPE1_4 for CAM ALL\n");

	s3c_gpio_setpull(S5PV210_GPE1(4), S3C_GPIO_PULL_NONE);
	gpio_direction_output(S5PV210_GPE1(4), 0);
	mdelay(100);
	gpio_direction_output(S5PV210_GPE1(4), 1);
	gpio_free(S5PV210_GPE1(4));
	printk("%s end\n",__func__);
	
#endif

	//print info
//{
	//printk( "GPB CON:0x%x DATA:0x%x\n",
	//	__raw_readl(S5PV210_GPB_BASE + 0x00),__raw_readl(S5PV210_GPB_BASE + 0x04));
	//printk( "GPE1 CON:0x%x DATA:0x%x\n",
	//	__raw_readl(S5PV210_GPE1_BASE + 0x00),__raw_readl(S5PV210_GPE1_BASE + 0x04));

//}

{
	#if 1
	unsigned int tempvalue=0;
      	tempvalue = readl(S5P_CLK_DIV1);	
	tempvalue = (tempvalue &0xffff0fff);	
	writel(tempvalue,S5P_CLK_DIV1);	
	tempvalue = readl(S5P_CLK_SRC1);	
	tempvalue = (tempvalue &0xffff0fff)|0x00001000;	
	writel(tempvalue,S5P_CLK_SRC1);	
	#endif
	//printk( "S5P_CLK_DIV1:0x%x S5P_CLK_SRC1:0x%x\n",
	//__raw_readl(S5P_CLK_DIV1),__raw_readl(S5P_CLK_SRC1));

}

#if 0
	err = gpio_request(S5PV210_GPH0(2), "GPH0");
	if (err)
		printk(KERN_ERR "#### failed to request GPH0 for CAM_2V8\n");

	s3c_gpio_setpull(S5PV210_GPH0(2), S3C_GPIO_PULL_NONE);
	gpio_direction_output(S5PV210_GPH0(2), 0);
	gpio_direction_output(S5PV210_GPH0(2), 1);
	gpio_free(S5PV210_GPH0(2));
#endif
	return 0;
}

static int smdkv210_cam1_power(int onoff)
{
	int err;

	/* S/W workaround for the SMDK_CAM4_type board
	 * When SMDK_CAM4 type module is initialized at power reset,
	 * it needs the cam_mclk.
	 *
	 * Now cam_mclk is set as below, need only set the gpio mux.
	 * CAM_SRC1 = 0x0006000, CLK_DIV1 = 0x00070400.
	 * cam_mclk source is SCLKMPLL, and divider value is 8.
	*/
	#if 0

	/* gpio mux select the cam_mclk */
	err = gpio_request(S5PV210_GPJ1(4), "GPJ1");
	if (err)
		printk(KERN_ERR "#### failed to request GPJ1 for CAM_2V8\n");

	s3c_gpio_setpull(S5PV210_GPJ1(4), S3C_GPIO_PULL_NONE);
	s3c_gpio_cfgpin(S5PV210_GPJ1(4), (0x3<<16));

	/* Camera B */
	err = gpio_request(S5PV210_GPH0(3), "GPH0");
	if (err)
		printk(KERN_ERR "#### failed to request GPH0 for CAM_2V8\n");

	s3c_gpio_setpull(S5PV210_GPH0(3), S3C_GPIO_PULL_NONE);
	gpio_direction_output(S5PV210_GPH0(3), 0);
	gpio_direction_output(S5PV210_GPH0(3), 1);

	udelay(1000);

	gpio_free(S5PV210_GPJ1(4));
	gpio_free(S5PV210_GPH0(3));
	#endif
	return 0;
}

static int smdkv210_mipi_cam_reset(void)
{
	int err;

	err = gpio_request(S5PV210_GPH0(3), "GPH0");
	if (err)
		printk(KERN_ERR "#### failed to reset(GPH0) for MIPI CAM\n");

	s3c_gpio_setpull(S5PV210_GPH0(3), S3C_GPIO_PULL_NONE);
	gpio_direction_output(S5PV210_GPH0(3), 0);
	gpio_direction_output(S5PV210_GPH0(3), 1);
	gpio_free(S5PV210_GPH0(3));

	return 0;
}

static int smdkv210_mipi_cam_power(int onoff)
{
	int err;

	/* added for V210 CAM power */
	err = gpio_request(S5PV210_GPH1(2), "GPH1");
	if (err)
		printk(KERN_ERR "#### failed to request(GPH1)for CAM_2V8\n");

	s3c_gpio_setpull(S5PV210_GPH1(2), S3C_GPIO_PULL_NONE);
	gpio_direction_output(S5PV210_GPH1(2), onoff);
	gpio_free(S5PV210_GPH1(2));

	return 0;
}
//add by jhuang for GT2005
#if 1//add  by jhuang

static void set_cam2005_main_power(int onoff)

{
	/*TODO  camera pm. 1. Power always on, sensor powersaving; 2. external power off  */
	unsigned int gpio;
	//struct regulator *cam_reg = regulators[0];
	int err;
	
	printk("Set cam back state:%s\n",onoff?("on"):("off"));
	printk("set_cam_back_power start\n");
	
	printk("jhuang:%s,%d",__func__,__LINE__);
	//if(IS_ERR(cam_reg))
	//{
	//	chgdbg("set_cam_front_power:regulator, err\n");
	//	return;
	//}

		
		//power
	if(onoff)
	{
		//PWREN CAM_B
	
		err = gpio_request(S5PV210_GPB(2), "GPB");
		if (err)
			printk(KERN_ERR "#### failed to request GPB2 for CAM\n");
		s3c_gpio_cfgpin(S5PV210_GPB(2),S3C_GPIO_OUTPUT);
		s3c_gpio_setpull(S5PV210_GPB(2), S3C_GPIO_PULL_UP);
		gpio_direction_output(S5PV210_GPB(2), 1);
		gpio_free(S5PV210_GPB(2));

		//RESET
			err = gpio_request(S5PV210_GPE1(4), "GPE1");
			if (err)
				printk(KERN_ERR "#### failed to request GPE1_4 for CAM\n");
			s3c_gpio_setpull(S5PV210_GPE1(4), S3C_GPIO_PULL_NONE);
			gpio_direction_output(S5PV210_GPE1(4),0);
			mdelay(100);
			gpio_direction_output(S5PV210_GPE1(4), 1);
			gpio_free(S5PV210_GPE1(4));


		//mdelay(300);
		//PWDN CAM_back
			err = gpio_request(S5PV210_GPB(4), "GPB");
			if (err)
				printk(KERN_ERR "#### failed to request GPB4 for CAM\n");
			s3c_gpio_cfgpin(S5PV210_GPB(4),S3C_GPIO_OUTPUT);
			s3c_gpio_setpull(S5PV210_GPB(4), S3C_GPIO_PULL_UP);
			gpio_direction_output(S5PV210_GPB(4), 0);
			mdelay(100);
			gpio_direction_output(S5PV210_GPB(4), 1);
			gpio_free(S5PV210_GPB(4));

				//mdelay(200);

		mdelay(100);

//david   JHUANG
		unsigned int tmp;
		tmp = __raw_readl(S5PV210_GPE0DRV);
		printk("S5PV210_GPE0DRV=%x", tmp);
		tmp = 0xFFFF;
		__raw_writel(tmp, (S5PV210_GPE0DRV));		
		printk("S5PV210_GPE0DRV=%x", tmp);

	tmp = __raw_readl(S5PV210_GPE1DRV);
	printk("S5PV210_GPE1DRV=%x", tmp);
	tmp = 0xFFFF;
	__raw_writel(tmp, (S5PV210_GPE1DRV));		
	printk("S5PV210_GPE1DRV=%x", tmp);


				


	}
	else	
	{	
		mdelay(200);
		//pwdn CAM_B
		err = gpio_request(S5PV210_GPB(4), "GPB");
		if (err)
			printk(KERN_ERR "#### failed to request GPB4 for CAM\n");

		s3c_gpio_setpull(S5PV210_GPB(4), S3C_GPIO_PULL_NONE);
		//mdelay(100);
		gpio_direction_output(S5PV210_GPB(4), 0);
		gpio_free(S5PV210_GPB(4));
		mdelay(200);

		
		//RESET
		err = gpio_request(S5PV210_GPE1(4), "GPE1");
		if (err)
			printk(KERN_ERR "#### failed to request GPE1_4 for CAM\n");
		s3c_gpio_setpull(S5PV210_GPE1(4), S3C_GPIO_PULL_NONE);
		//gpio_direction_output(S5PV210_GPE1(4),1);
		//mdelay(100);
		gpio_direction_output(S5PV210_GPE1(4), 0);
		gpio_free(S5PV210_GPE1(4));

		mdelay(200);

		//CAM_PWREN  CAM_B
		err = gpio_request(S5PV210_GPB(2), "GPB");
		if (err)
			printk(KERN_ERR "#### failed to request GPB2 for CAM\n");
		s3c_gpio_setpull(S5PV210_GPB(2), S3C_GPIO_PULL_NONE);
		gpio_direction_output(S5PV210_GPB(2), 0);
		gpio_free(S5PV210_GPB(2));
		mdelay(100);

	}
	return;
}
#endif
#if 1 //modify by jhuang
//static void set_cam_front_power(device_pm_state val, struct regulator ** regulators)
//static void set_cam0308_main_power(device_pm_state val, struct regulator ** regulators)
static void set_cam0308_main_power(int onoff)

{
	/*TODO  camera pm. 1. Power always on, sensor powersaving; 2. external power off  */
	unsigned int gpio;
	//struct regulator *cam_reg = regulators[0];
	int err;
	
	//chgdbg("Set cam front state:%s\n",onoff?("on"):("off"));
	printk("set_cam_front_power start\n");
	
	printk("jhuang:%s,%d",__func__,__LINE__);
	//if(IS_ERR(cam_reg))
	//{
	//	chgdbg("set_cam_front_power:regulator, err\n");
	//	return;
	//}

		
		//power
	if(onoff)
	{
		//PWREN CAM_F
	
		err = gpio_request(S5PV210_GPD0(1), "GPD0");
		if (err)
			printk(KERN_ERR "#### failed to request GPB2 for CAM\n");
		s3c_gpio_cfgpin(S5PV210_GPD0(1),S3C_GPIO_OUTPUT);
		s3c_gpio_setpull(S5PV210_GPD0(1), S3C_GPIO_PULL_UP);
		gpio_direction_output(S5PV210_GPD0(1), 1);
		gpio_free(S5PV210_GPD0(1));


		//mdelay(300);
		//PWDN  CAM_F
		err = gpio_request(S5PV210_GPB(5), "GPB");
		if (err)
			printk(KERN_ERR "#### failed to request GPB4 for CAM\n");
		s3c_gpio_cfgpin(S5PV210_GPB(5),S3C_GPIO_INPUT);
		s3c_gpio_setpull(S5PV210_GPB(5), S3C_GPIO_PULL_UP);
		gpio_direction_output(S5PV210_GPB(5), 1);
		mdelay(100);
		gpio_direction_output(S5PV210_GPB(5), 0);
		gpio_free(S5PV210_GPB(5));
				//mdelay(200);
		//RESET
		err = gpio_request(S5PV210_GPE1(4), "GPE1");
		if (err)
			printk(KERN_ERR "#### failed to request GPE1_4 for CAM\n");
		s3c_gpio_setpull(S5PV210_GPE1(4), S3C_GPIO_PULL_NONE);
		gpio_direction_output(S5PV210_GPE1(4),0);
		mdelay(100);
		gpio_direction_output(S5PV210_GPE1(4), 1);
		gpio_free(S5PV210_GPE1(4));
		mdelay(100);

//DAVID&JHUANG
		unsigned int tmp;
		tmp = __raw_readl(S5PV210_GPE0DRV);
		printk("S5PV210_GPE0DRV=%x", tmp);
		tmp = 0xFFFF;
		__raw_writel(tmp, (S5PV210_GPE0DRV));		
		printk("S5PV210_GPE0DRV=%x", tmp);

	tmp = __raw_readl(S5PV210_GPE1DRV);
	printk("S5PV210_GPE1DRV=%x", tmp);
	tmp = 0xFFFF;
	__raw_writel(tmp, (S5PV210_GPE1DRV));		
	printk("S5PV210_GPE1DRV=%x", tmp);



	}
	else	
	{	
		mdelay(200);
		//pwdn CAM_F
		err = gpio_request(S5PV210_GPB(5), "GPB");
		if (err)
			printk(KERN_ERR "#### failed to request GPB4 for CAM\n");

		s3c_gpio_setpull(S5PV210_GPB(5), S3C_GPIO_PULL_NONE);
		//mdelay(100);
		gpio_direction_output(S5PV210_GPB(5), 1);
		gpio_free(S5PV210_GPB(5));
		mdelay(200);

		
		//RESET
		err = gpio_request(S5PV210_GPE1(4), "GPE1");
		if (err)
			printk(KERN_ERR "#### failed to request GPE1_4 for CAM\n");
		s3c_gpio_setpull(S5PV210_GPE1(4), S3C_GPIO_PULL_NONE);
		//gpio_direction_output(S5PV210_GPE1(4),1);
		mdelay(100);
		gpio_direction_output(S5PV210_GPE1(4), 0);
		gpio_free(S5PV210_GPE1(4));


		//CAM_PWREN  CAM_F
		err = gpio_request(S5PV210_GPD0(1), "GPD0");
		if (err)
			printk(KERN_ERR "#### failed to request GPB2 for CAM\n");

		s3c_gpio_setpull(S5PV210_GPD0(1), S3C_GPIO_PULL_NONE);
		gpio_direction_output(S5PV210_GPD0(1), 0);
		gpio_free(S5PV210_GPD0(1));
		mdelay(100);

	}
	return;
}
#endif

#if 1
static int smdkv210_bf3703_power(int onoff)
{
	int err;
	printk(">>>>>%s\n",__func__);
	/* Camera A */
	if(onoff)
	{

	err = gpio_request(S5PV210_GPD0(1), "GPB");
	if (err)
		printk(KERN_ERR "#### failed to request GPB5 for CAM Forward..\n");
	s3c_gpio_cfgpin(S5PV210_GPD0(1),S3C_GPIO_OUTPUT);
	s3c_gpio_setpull(S5PV210_GPD0(1), S3C_GPIO_PULL_NONE);
	gpio_set_value(S5PV210_GPD0(1), 1);
	gpio_free(S5PV210_GPD0(1));
	mdelay(300);
	
	// power down
	err = gpio_request(S5PV210_GPB(5),"GPB_5");
	if(err)
			printk(KERN_ERR "#### failed to request GPB_5 for CAM0\n");
	s3c_gpio_cfgpin(S5PV210_GPB(5),S3C_GPIO_OUTPUT);
	s3c_gpio_setpull(S5PV210_GPB(5), S3C_GPIO_PULL_NONE);
	gpio_set_value(S5PV210_GPB(5), 0);
	gpio_free(S5PV210_GPB(5));


	err = gpio_request(S5PV210_GPB(2), "GPB");
	if (err)
		printk(KERN_ERR "#### failed to request GPB2 for CAM Back..\n");
	s3c_gpio_setpull(S5PV210_GPB(2), S3C_GPIO_PULL_DOWN);
	s3c_gpio_cfgpin(S5PV210_GPB(2),S3C_GPIO_INPUT);
	gpio_free(S5PV210_GPB(2));

	err = gpio_request(S5PV210_GPB(4), "GPB");
	if (err)
		printk(KERN_ERR "#### failed to request GPB4 for CAM Back..\n");
	s3c_gpio_setpull(S5PV210_GPB(4), S3C_GPIO_PULL_DOWN);
	s3c_gpio_cfgpin(S5PV210_GPB(4),S3C_GPIO_INPUT);
	gpio_free(S5PV210_GPB(4));
	
		//reset	
#if 0
		err = gpio_request(S5PV210_GPE1(4), "GPE1");
		if (err)
			printk(KERN_ERR "#### failed to request GPE1_4 for CAM\n");

		s3c_gpio_setpull(S5PV210_GPE1(4), S3C_GPIO_PULL_NONE);
		gpio_direction_output(S5PV210_GPE1(4), 0);
		mdelay(50);
		gpio_direction_output(S5PV210_GPE1(4), 1);
		gpio_free(S5PV210_GPE1(4));
#endif
	}
	else	
	{	
#if 0

		err = gpio_request(S5PV210_GPE1(4), "GPE1");
		if (err)
			printk(KERN_ERR "#### failed to request GPE1_4 for CAM ALL\n");

		s3c_gpio_setpull(S5PV210_GPE1(4), S3C_GPIO_PULL_NONE);
		gpio_direction_output(S5PV210_GPE1(4), 0);
		gpio_free(S5PV210_GPE1(4));
		
		//CAM_PWREN
		err = gpio_request(S5PV210_GPB(4), "GPB");
		if (err)
			printk(KERN_ERR "#### failed to request GPB3 for CAM\n");

		s3c_gpio_setpull(S5PV210_GPB(4), S3C_GPIO_PULL_NONE);
		gpio_direction_output(S5PV210_GPB(4), 0);
		gpio_free(S5PV210_GPB(4));

		
		err = gpio_request(S5PV210_GPD0(1), "GPB");
		if (err)
			printk(KERN_ERR "#### failed to request GPB5 for CAM Forward..\n");
		s3c_gpio_cfgpin(S5PV210_GPD0(1),S3C_GPIO_OUTPUT);
		s3c_gpio_setpull(S5PV210_GPD0(1), S3C_GPIO_PULL_NONE);
		gpio_direction_output(S5PV210_GPD0(1), 0);
		gpio_free(S5PV210_GPD0(1));
#else
		//modify by liao 2011-08-02
		// power down
		err = gpio_request(S5PV210_GPB(5),"GPB_5");
		if(err)
			printk(KERN_ERR "#### failed to request GPB_5 for CAM0\n");
		s3c_gpio_cfgpin(S5PV210_GPB(5),S3C_GPIO_OUTPUT);
		s3c_gpio_setpull(S5PV210_GPB(5), S3C_GPIO_PULL_NONE);
		gpio_direction_output(S5PV210_GPB(5), 1);
		gpio_free(S5PV210_GPB(5));
#endif	


	}
	return 0;
}
#endif

/*
 * Guide for Camera Configuration for smdkv210
 * ITU channel must be set as A or B
 * ITU CAM CH A: S5K3BA only
 * ITU CAM CH B: one of S5K3BA and S5K4BA
 * MIPI: one of S5K4EA and S5K6AA
 *
 * NOTE1: if the S5K4EA is enabled, all other cameras must be disabled
 * NOTE2: currently, only 1 MIPI camera must be enabled
 * NOTE3: it is possible to use both one ITU cam and
 * 	  one MIPI cam except for S5K4EA case
 *
*/
#undef CAM_ITU_CH_A
#undef WRITEBACK_ENABLED
//delete by jhuang
#if 0
#ifdef CONFIG_VIDEO_S5K4EA
#define S5K4EA_ENABLED
/* undef : 3BA, 4BA, 6AA */
#else
#ifdef CONFIG_VIDEO_S5K6AA
#define S5K6AA_ENABLED
/* undef : 4EA */
#else
#ifdef CONFIG_VIDEO_S5K3BA
#define S5K3BA_ENABLED

#else
#ifdef  CONFIG_VIDEO_OV7670
#define OV7670_ENABLED

#else
//#ifdef CONFIG_VIDEO_BF3703//by lfj 110216,for byd sensor
//#define BF3703_ENABLED

//#else
#ifdef CONFIG_VIDEO_S5K4BA
#define S5K4BA_ENABLED

#elif CONFIG_VIDEO_S5K5BA
#define S5K5BA_ENABLED

/* undef : 3BA */
#endif
#endif
#endif
#endif
#endif
#endif
/* External camera module setting */
/* 2 ITU Cameras */
#ifdef S5K3BA_ENABLED
static struct s5k3ba_platform_data s5k3ba_plat = {
	.default_width = 640,
	.default_height = 480,
	.pixelformat = V4L2_PIX_FMT_VYUY,
	.freq = 24000000,
	.is_mipi = 0,
};

static struct i2c_board_info  s5k3ba_i2c_info = {
	I2C_BOARD_INFO("S5K3BA", 0x2d),
	.platform_data = &s5k3ba_plat,
};

static struct s3c_platform_camera s5k3ba = {
#ifdef CAM_ITU_CH_A
	.id		= CAMERA_PAR_A,
#else
	.id		= CAMERA_PAR_B,
#endif
	.type		= CAM_TYPE_ITU,
	.fmt		= ITU_601_YCBCR422_8BIT,
	.order422	= CAM_ORDER422_8BIT_CRYCBY,
	.i2c_busnum	= 0,
	.info		= &s5k3ba_i2c_info,
	.pixelformat	= V4L2_PIX_FMT_VYUY,
	.srclk_name	= "mout_epll",
	.clk_name	= "sclk_cam1",
	.clk_rate	= 24000000,
	.line_length	= 1920,
	.width		= 640,
	.height		= 480,
	.window		= {
		.left	= 0,
		.top	= 0,
		.width	= 640,
		.height	= 480,
	},

	/* Polarity */
	.inv_pclk	= 0,
	.inv_vsync	= 1,
	.inv_href	= 0,
	.inv_hsync	= 0,

	.initialized	= 0,
#ifdef CAM_ITU_CH_A
	.cam_power	= smdkv210_cam0_power,
#else
	.cam_power	= smdkv210_cam1_power,
#endif
};
#endif

#ifdef S5K4BA_ENABLED
static struct s5k4ba_platform_data s5k4ba_plat = {
	.default_width = 800,
	.default_height = 600,
	.pixelformat = V4L2_PIX_FMT_UYVY,
	.freq = 44000000,
	.is_mipi = 0,
};

static struct i2c_board_info  s5k4ba_i2c_info = {
	I2C_BOARD_INFO("S5K4BA", 0x2d),
	.platform_data = &s5k4ba_plat,
};

static struct s3c_platform_camera s5k4ba = {
	.id		= CAMERA_PAR_B,
	.type		= CAM_TYPE_ITU,
	.fmt		= ITU_601_YCBCR422_8BIT,
	.order422	= CAM_ORDER422_8BIT_CBYCRY,
	.i2c_busnum	= 0,
	.info		= &s5k4ba_i2c_info,
	.pixelformat	= V4L2_PIX_FMT_UYVY,
	.srclk_name	= "mout_mpll",
	.clk_name	= "sclk_cam1",
	.clk_rate	= 44000000,
	.line_length	= 1920,
	.width		= 800,
	.height		= 600,
	.window		= {
		.left	= 0,
		.top	= 0,
		.width	= 800,
		.height	= 600,
	},

	/* Polarity */
	.inv_pclk	= 0,
	.inv_vsync	= 1,
	.inv_href	= 0,
	.inv_hsync	= 0,

	.initialized	= 0,
#ifdef CAM_ITU_CH_A
	.cam_power	= smdkv210_cam0_power,
#else
	.cam_power	= smdkv210_cam1_power,
#endif
};
#endif

// added by liao:
#ifdef S5K5BA_ENABLED
static struct s5k4ba_platform_data s5k5ba_plat = {
	.default_width = 800,
	.default_height = 600,
	.pixelformat = V4L2_PIX_FMT_UYVY,
	.freq = 24000000,  //36M
	.is_mipi = 0,
};

static struct i2c_board_info  s5k5ba_i2c_info = {
	I2C_BOARD_INFO("S5K5BA", 0x2d),
	.platform_data = &s5k5ba_plat,
};

static struct s3c_platform_camera s5k5ba = {
	.id		= CAMERA_PAR_A,
	.type		= CAM_TYPE_ITU,
	.fmt		= ITU_601_YCBCR422_8BIT,
	.order422	= CAM_ORDER422_8BIT_CBYCRY,
	.i2c_busnum	= 1,
	.info		= &s5k5ba_i2c_info,
	.pixelformat	= V4L2_PIX_FMT_UYVY,
	.srclk_name	= "mout_mpll",
	.clk_name	= "sclk_cam0",
	.clk_rate	= 24000000,
	.line_length	= 1920,
	.width		= 800,
	.height		= 600,
	.window		= {
		.left	= 0,
		.top	= 0,
		.width	= 800,
		.height	= 600,
	},

	/* Polarity */
	.inv_pclk	= 1,
	.inv_vsync	= 1,
	.inv_href	= 0,
	.inv_hsync	= 1,

	.initialized	= 0,
//#ifdef CAM_ITU_CH_A
//	.cam_power	= smdkv210_cam0_power,
//#else
//	.cam_power	= smdkv210_cam1_power,
//#endif
};
#endif


#ifdef OV7670_ENABLED
static struct ov7670_platform_data ov7670_plat = {
	.default_width = 640,
	.default_height = 480,
	.pixelformat = V4L2_PIX_FMT_YUYV,
	.freq = 24000000,
	.is_mipi = 0,
};

static struct i2c_board_info  ov7670_i2c_info = {
	I2C_BOARD_INFO("ov7670", 0x21),
	.platform_data = &ov7670_plat,
};

static struct s3c_platform_camera ov7670 = {
	.id		= CAMERA_PAR_A,
	.type		= CAM_TYPE_ITU,
	.fmt		= ITU_601_YCBCR422_8BIT,
	.order422	= CAM_ORDER422_8BIT_YCBYCR,
	.i2c_busnum	= 3,
	.info		= &ov7670_i2c_info,
	.pixelformat	= V4L2_PIX_FMT_YUYV,
	.srclk_name	= "xusbxti",//"mout_mpll",
	.clk_name	= "sclk_cam0",
	.clk_rate	= 24000000,
	.line_length	= 640,
	.width		= 640,
	.height		= 480,
	.window		= {
		.left	= 0,
		.top	= 0,
		.width	= 640,
		.height	= 480,
	},

	/* Polarity */
	.inv_pclk	= 0,
	.inv_vsync	= 0,
	.inv_href	= 0,
	.inv_hsync	= 0,

	.initialized	= 0,
#if 1//def CAM_ITU_CH_A
	//.cam_power	= smdkv210_cam0_power,
#else
	.cam_power	= smdkv210_cam1_power,
#endif
};
#endif
#ifdef CONFIG_VIDEO_BF3703// BF3703_ENABLED
/*BYD 3703 is the replacement of ov7670.The driver ported from 7670, 
only changed the register default settings, and only settings for YUV*/
static struct bf3703_platform_data bf3703_plat = {
	.default_width = 640,
	.default_height = 480,
	.pixelformat	= V4L2_PIX_FMT_UYVY,
	.freq = 24000000,
	.is_mipi = 0,
};

static struct i2c_board_info  bf3703_i2c_info = {
	I2C_BOARD_INFO("bf3703", 0x6E),
	.platform_data = &bf3703_plat,
};

static struct s3c_platform_camera bf3703 = {
	.id		= CAMERA_PAR_B,
	.type		= CAM_TYPE_ITU,
	.fmt		= ITU_601_YCBCR422_8BIT,
	.order422	= CAM_ORDER422_8BIT_CBYCRY,
#ifdef CONFIG_I2C_GPIO
	.i2c_busnum	= 3, //3, //uses gpio emulated method
#else
	.i2c_busnum	= 1,
#endif
	.info		= &bf3703_i2c_info,
	.pixelformat	= V4L2_PIX_FMT_UYVY,
	.srclk_name	= "mout_mpll",//"mout_mpll",
	.clk_name	= "sclk_cam1",
	.clk_rate	= 24000000,
	.line_length	= 640,
	.width		= 640,
	.height		= 480,
	.window		= {
		.left	= 0,
		.top	= 0,
		.width	= 640,
		.height	= 480,
	},

	/* Polarity */
	.inv_pclk	= 1,//for byd //0,
	.inv_vsync = 1,//for byd//0,
	.inv_href	= 0,
	.inv_hsync = 0,

	.initialized = 0,
	.cam_power	= smdkv210_bf3703_power,  

};
#endif

//add by jhuang 
#ifdef CONFIG_VIDEO_GT2005
static struct GT2005_platform_data GT2005_plat = {
	.default_width = 640,
	.default_height = 480,
	.pixelformat = V4L2_PIX_FMT_UYVY,   //modify by jhuang
	.freq = 24000000,
	.is_mipi = 0,
};

static struct i2c_board_info GT2005_i2c_info = {
	I2C_BOARD_INFO("GT2005", 0x78 >> 1),
	.platform_data = &GT2005_plat,
};

static struct s3c_platform_camera GT2005 = {
//#ifdef CAM_ITU_CH_A
	.id		= CAMERA_PAR_A,
//#else
//	.id		= CAMERA_PAR_B,
//#endif
	.type		= CAM_TYPE_ITU,
	.fmt		= ITU_601_YCBCR422_8BIT,
	.order422	= CAM_ORDER422_8BIT_CBYCRY,
	.i2c_busnum	= 3,
	.info		= &GT2005_i2c_info,
	.pixelformat	= V4L2_PIX_FMT_UYVY,//modify by jhuang
	
	.srclk_name	= "mout_mpll",
	.clk_name	= "sclk_cam1",
	.clk_rate	= 24000000,             /* 24MHz */
	.line_length	= 640,              /* 640*480 */
	/* default resol for preview kind of thing */
	.width		= 640,
	.height		= 480,
	.window		= {
		.left   = 16,
		.top    = 0,
		.width  = 640 - 16,
		.height = 480,
	},

	/* Polarity */
	.inv_pclk	= 0,
	.inv_vsync	= 1,
	.inv_href	= 0,
	.inv_hsync	= 1,

	.initialized	= 0,
	.cam_power=set_cam2005_main_power
};
#endif
#ifdef CONFIG_VIDEO_GC0308// GC0308_ENABLED
static struct gc0308_platform_data gc0308_plat = {
	.default_width = 640,
	.default_height = 480,
	.pixelformat = V4L2_PIX_FMT_YUYV,
	.freq = 24000000,
	.is_mipi = 0,
};

static struct i2c_board_info gc0308_i2c_info = {
	I2C_BOARD_INFO("GC0308", 0x42 >> 1),
	.platform_data = &gc0308_plat,
};

static struct s3c_platform_camera gc0308 = {
//#ifdef CAM_ITU_CH_A
	.id		= CAMERA_PAR_B,
//#else
	//.id		= CAMERA_PAR_B,
//#endif
	.type		= CAM_TYPE_ITU,
	.fmt		= ITU_601_YCBCR422_8BIT,
	.order422	= CAM_ORDER422_8BIT_CBYCRY,
#ifdef CONFIG_I2C_GPIO
		.i2c_busnum = 3, //3, //uses gpio emulated method
#else
		.i2c_busnum = 1,
#endif
	.info		= &gc0308_i2c_info,
	.pixelformat	= V4L2_PIX_FMT_YUYV,
	.srclk_name	= "mout_mpll",
	.clk_name	= "sclk_cam1",
	.clk_rate	= 24000000,             /* 24MHz */
	.line_length	= 640,              /* 640*480 */
	/* default resol for preview kind of thing */
	.width		= 640,
	.height		= 480,
	.window		= {
		.left   = 16,
		.top    = 0,
		.width  = 640 - 16,
		.height = 480,
	},

	/* Polarity */
	.inv_pclk	= 0,
	.inv_vsync	= 1,
	.inv_href	= 0,
	.inv_hsync	= 0,

	.initialized	= 0,
	.cam_power	= set_cam0308_main_power,
};
#endif



/* 2 MIPI Cameras */
#ifdef S5K4EA_ENABLED
static struct s5k4ea_platform_data s5k4ea_plat = {
	.default_width = 1920,
	.default_height = 1080,
	.pixelformat = V4L2_PIX_FMT_UYVY,
	.freq = 24000000,
	.is_mipi = 1,
};

static struct i2c_board_info  s5k4ea_i2c_info = {
	I2C_BOARD_INFO("S5K4EA", 0x2d),
	.platform_data = &s5k4ea_plat,
};

static struct s3c_platform_camera s5k4ea = {
	.id		= CAMERA_CSI_C,
	.type		= CAM_TYPE_MIPI,
	.fmt		= MIPI_CSI_YCBCR422_8BIT,
	.order422	= CAM_ORDER422_8BIT_CBYCRY,
	.i2c_busnum	= 0,
	.info		= &s5k4ea_i2c_info,
	.pixelformat	= V4L2_PIX_FMT_UYVY,
	.srclk_name	= "mout_mpll",
	.clk_name	= "sclk_cam0",
	.clk_rate	= 48000000,
	.line_length	= 1920,
	.width		= 1920,
	.height		= 1080,
	.window		= {
		.left	= 0,
		.top	= 0,
		.width	= 1920,
		.height	= 1080,
	},

	.mipi_lanes	= 2,
	.mipi_settle	= 12,
	.mipi_align	= 32,

	/* Polarity */
	.inv_pclk	= 0,
	.inv_vsync	= 1,
	.inv_href	= 0,
	.inv_hsync	= 0,

	.initialized	= 0,
	.cam_power	= smdkv210_mipi_cam_power,
};
#endif

#ifdef S5K6AA_ENABLED
static struct s5k6aa_platform_data s5k6aa_plat = {
	.default_width = 640,
	.default_height = 480,
	.pixelformat = V4L2_PIX_FMT_UYVY,
	.freq = 24000000,
	.is_mipi = 1,
};

static struct i2c_board_info  s5k6aa_i2c_info = {
	I2C_BOARD_INFO("S5K6AA", 0x3c),
	.platform_data = &s5k6aa_plat,
};

static struct s3c_platform_camera s5k6aa = {
	.id		= CAMERA_CSI_C,
	.type		= CAM_TYPE_MIPI,
	.fmt		= MIPI_CSI_YCBCR422_8BIT,
	.order422	= CAM_ORDER422_8BIT_CBYCRY,
	.i2c_busnum	= 0,
	.info		= &s5k6aa_i2c_info,
	.pixelformat	= V4L2_PIX_FMT_UYVY,
	.srclk_name	= "xusbxti",
	.clk_name	= "sclk_cam",
	.clk_rate	= 24000000,
	.line_length	= 1920,
	/* default resol for preview kind of thing */
	.width		= 640,
	.height		= 480,
	.window		= {
		.left	= 0,
		.top	= 0,
		.width	= 640,
		.height	= 480,
	},

	.mipi_lanes	= 1,
	.mipi_settle	= 6,
	.mipi_align	= 32,

	/* Polarity */
	.inv_pclk	= 0,
	.inv_vsync	= 1,
	.inv_href	= 0,
	.inv_hsync	= 0,

	.initialized	= 0,
	.cam_power	= smdkv210_mipi_cam_power,
};
#endif

#ifdef WRITEBACK_ENABLED
static struct i2c_board_info  writeback_i2c_info = {
	I2C_BOARD_INFO("WriteBack", 0x0),
};

static struct s3c_platform_camera writeback = {
	.id		= CAMERA_WB,
	.fmt		= ITU_601_YCBCR422_8BIT,
	.order422	= CAM_ORDER422_8BIT_CBYCRY,
	.i2c_busnum	= 0,
	.info		= &writeback_i2c_info,
	.pixelformat	= V4L2_PIX_FMT_YUV444,
	.line_length	= 800,
	.width		= 480,
	.height		= 800,
	.window		= {
		.left	= 0,
		.top	= 0,
		.width	= 480,
		.height	= 800,
	},

	.initialized	= 0,
};
#endif

/* Interface setting */
static struct s3c_platform_fimc fimc_plat = {
	.srclk_name	= "mout_mpll",
	.clk_name	= "fimc",
	.clk_rate	= 166000000,
#if defined(S5K4EA_ENABLED) || defined(S5K6AA_ENABLED)
	.default_cam	= CAMERA_CSI_C,
#else

#ifdef WRITEBACK_ENABLED
	.default_cam	= CAMERA_WB,
#elif 1//CAM_ITU_CH_A
	.default_cam	= CAMERA_PAR_A,
#else
	.default_cam	= CAMERA_PAR_B,
#endif

#endif
	.camera		= {
#ifdef S5K3BA_ENABLED
		&s5k3ba,
#endif
#ifdef S5K4BA_ENABLED
		&s5k4ba,
#endif
#ifdef S5K5BA_ENABLED
		&s5k5ba,
#endif
#ifdef OV7670_ENABLED
		&ov7670,
#endif
#ifdef CONFIG_VIDEO_BF3703//BF3703_ENABLED
		&bf3703,
#endif
#ifdef S5K4EA_ENABLED
		&s5k4ea,
#endif
#ifdef S5K6AA_ENABLED
		&s5k6aa,
#endif 
#ifdef  CONFIG_VIDEO_GT2005//add by jhuang
		&GT2005,
#endif
#ifdef CONFIG_VIDEO_GC0308  //add by jhuang
		&gc0308,
#endif

#ifdef WRITEBACK_ENABLED
		&writeback,
#endif
	},
	.hw_ver		= 0x43,
};
#endif
/*
#ifdef CONFIG_VIDEO_JPEG_V2
static struct s3c_platform_jpeg jpeg_plat __initdata = {
	.max_main_width		= 800,
	.max_main_height	= 600,
	.max_thumb_width	= 320,
	.max_thumb_height	= 240,
};
#endif
*/

#if defined(CONFIG_SPI_CNTRLR_0) || defined(CONFIG_SPI_CNTRLR_1) || defined(CONFIG_SPI_CNTRLR_2)
static void s3c_cs_suspend(int pin, pm_message_t pm)
{
        /* Whatever need to be done */
}

static void s3c_cs_resume(int pin)
{
        /* Whatever need to be done */
}

static void s3c_cs_set(int pin, int lvl)
{
        if(lvl == CS_HIGH)
           s3c_gpio_setpin(pin, 1);
        else
           s3c_gpio_setpin(pin, 0);
}
static void s3c_cs_config(int pin, int mode, int pull)
{
        s3c_gpio_cfgpin(pin, mode);

        if(pull == CS_HIGH){
           s3c_gpio_setpull(pin, S3C_GPIO_PULL_UP);
		   s3c_gpio_setpin(pin, 0);
		}
        else{
           s3c_gpio_setpull(pin, S3C_GPIO_PULL_DOWN);
		   s3c_gpio_setpin(pin, 1);
		}
}
#endif
#if 1   //del by liao fro camera
#if defined(CONFIG_SPI_CNTRLR_0)
static struct s3c_spi_pdata s3c_slv_pdata_0[] __initdata = {
        [0] = { /* Slave-0 */
                .cs_level     = CS_FLOAT,
                .cs_pin       = S5PV210_GPB(1),
                .cs_mode      = S5PV210_GPB_OUTPUT(1),
                .cs_set       = s3c_cs_set,
                .cs_config    = s3c_cs_config,
                .cs_suspend   = s3c_cs_suspend,
                .cs_resume    = s3c_cs_resume,
        },
        #if 0
        [1] = { /* Slave-1 */
                .cs_level     = CS_FLOAT,
                .cs_pin       = S5PV210_GPA1(1),
                .cs_mode      = S5PV210_GPA1_OUTPUT(1),
                .cs_set       = s3c_cs_set,
                .cs_config    = s3c_cs_config,
                .cs_suspend   = s3c_cs_suspend,
                .cs_resume    = s3c_cs_resume,
        },
        #endif
};
#endif

#if defined(CONFIG_SPI_CNTRLR_1)
static struct s3c_spi_pdata s3c_slv_pdata_1[] __initdata = {
        [0] = { /* Slave-0 */
                .cs_level     = CS_FLOAT,
                .cs_pin       = S5PV210_GPB(2),
                .cs_mode      = S5PV210_GPB_OUTPUT(2),
                .cs_set       = s3c_cs_set,
                .cs_config    = s3c_cs_config,
                .cs_suspend   = s3c_cs_suspend,
                .cs_resume    = s3c_cs_resume,
        },
		#if 0
        [1] = { /* Slave-1 */
                .cs_level     = CS_FLOAT,
                .cs_pin       = S5PV210_GPA1(3),
                .cs_mode      = S5PV210_GPA1_OUTPUT(3),
                .cs_set       = s3c_cs_set,
                .cs_config    = s3c_cs_config,
                .cs_suspend   = s3c_cs_suspend,
                .cs_resume    = s3c_cs_resume,
        },
		#endif
};
#endif
#endif
static struct spi_board_info s3c_spi_devs[] __initdata = {
#if defined(CONFIG_SPI_CNTRLR_0)
        [0] = {
                .modalias        = "spidev", /* Test Interface */
                .mode            = SPI_MODE_0,  /* CPOL=0, CPHA=0 */
                .max_speed_hz    = 100000,
                /* Connected to SPI-0 as 1st Slave */
                .bus_num         = 0,
                .irq             = IRQ_SPI0,
                .chip_select     = 0,
        },
		#if 0
        [1] = {
                .modalias        = "spidev", /* Test Interface */
                .mode            = SPI_MODE_0,  /* CPOL=0, CPHA=0 */
                .max_speed_hz    = 100000,
                /* Connected to SPI-0 as 2nd Slave */
                .bus_num         = 0,
                .irq             = IRQ_SPI0,
                .chip_select     = 1,
        },
		#endif
#endif

#if defined(CONFIG_SPI_CNTRLR_1)
        [1] = {
                .modalias        = "spidev", /* Test Interface */
                .mode            = SPI_MODE_0,  /* CPOL=0, CPHA=0 */
                .max_speed_hz    = 100000,
                /* Connected to SPI-1 as 1st Slave */
                .bus_num         = 1,
                .irq             = IRQ_SPI1,
                .chip_select     = 0,
        },
		#if 0
        [3] = {
                .modalias        = "spidev", /* Test Interface */
                .mode            = SPI_MODE_0 | SPI_CS_HIGH,    /* CPOL=0, CPHA=0 & CS is Active High */
                .max_speed_hz    = 100000,
                /* Connected to SPI-1 as 3rd Slave */
                .bus_num         = 1,
                .irq             = IRQ_SPI1,
                .chip_select     = 1,
        },
		#endif
#endif
};

/*
  lcd brightness :
  in android froyo 0~255
*/
#if defined(CONFIG_HAVE_PWM)
static struct platform_pwm_backlight_data smdk_backlight_data = {
	.pwm_id  = 0,
	.max_brightness = 425,//510,//255,  //modify by liao,to down brightness level.
	.dft_brightness = 140,//40,//20
	.pwm_period_ns  = 10000,//1000000/*78770*/,
};

static struct platform_device smdk_backlight_device = {
	.name      = "pwm-backlight",
	.id        = -1,
	.dev        = {
		.parent = &s3c_device_timer[0].dev,
		.platform_data = &smdk_backlight_data,
	},
};
static void __init smdk_backlight_register(void)
{
	int ret = platform_device_register(&smdk_backlight_device);
	if (ret)
		printk(KERN_ERR "smdk: failed to register backlight device: %d\n", ret);
}
#endif

#if defined(CONFIG_BLK_DEV_IDE_S3C)
static struct s3c_ide_platdata smdkv210_ide_pdata __initdata = {
	.setup_gpio     = s3c_ide_setup_gpio,
};
#endif

#ifdef CONFIG_REGULATOR_ACT8937
extern struct act8937_platform_data act8937_platform_data;  /*defined in plat-regulators.c*/
#endif
/* I2C0 */
static struct i2c_board_info i2c_devs0[] __initdata = {
//jeff, T34 pmic
#ifdef CONFIG_REGULATOR_ACT8937
	{
		I2C_BOARD_INFO("act8937", 0x5B),
		.platform_data = &act8937_platform_data,
	},
#endif
};

/* I2C1 */
/*  
static struct i2c_board_info i2c_devs1[] __initdata = {

#ifdef CONFIG_VIDEO_TV20
	{
		I2C_BOARD_INFO("s5p_ddc", (0x74>>1)),
	},
#endif

#ifdef CONFIG_SENSORS_MMC31XX
	{
		I2C_BOARD_INFO(MMC31XX_I2C_NAME, MMC31XX_I2C_ADDR),
	},
#endif

#ifdef CONFIG_BOSCH_BMA150
	{
		I2C_BOARD_INFO("bma150", (0x38)),
	},
#endif

};
*/

/* I2C2 */
/*
static struct i2c_board_info i2c_devs2[] __initdata = {

#ifdef CONFIG_TOUCHSCREEN_GOODIX
	{
		I2C_BOARD_INFO("Goodix-TS",(0xAA>>1)),
	},
#endif
#ifdef CONFIG_TOUCHSCREEN_KEYPAD_TOUCH_KEY
	{
		I2C_BOARD_INFO("touch_key",(0x2b)),
	},
#endif

};
*/

#ifdef CONFIG_I2C_GPIO  //modify by xu_bin 110507
// 20110622

static struct i2c_gpio_platform_data gpio_i2c3_data  = {

	.sda_pin = S5PV210_GPD1(2),	
	.scl_pin = S5PV210_GPD1(3),		
	.udelay =1, //50,  modify by xu_bin 110509 for sda delay 
};

struct platform_device s3c_device_gpio_i2c3 = {	
	.name	= "i2c-gpio",	
	.id		  = 3,	
	.dev = { .platform_data = &gpio_i2c3_data },
};
static struct i2c_board_info i2c_devs3[] __initdata = {	

	{
		I2C_BOARD_INFO("bma150", (0x38)),  
	},
	
	{
		I2C_BOARD_INFO(MMC31XX_I2C_NAME, MMC31XX_I2C_ADDR),
	},
#ifdef CONFIG_VIDEO_TV20
	{
		I2C_BOARD_INFO("s5p_ddc", (0x74>>1)),
	},
#endif
};

static struct i2c_gpio_platform_data gpio_i2c4_data  = {

	.sda_pin =  S5PV210_GPD1(4),
	.scl_pin  =  S5PV210_GPD1(5),		
	.udelay =1, //50,  modify by xu_bin 110509 for sda delay 
};

struct platform_device s3c_device_gpio_i2c4 = {	
	.name	= "i2c-gpio",	
	.id		  = 4,	
	.dev = { .platform_data = &gpio_i2c4_data },
};
static struct i2c_board_info i2c_devs4[] __initdata = {	
//add by jhuang for GT819
#ifdef CONFIG_GOODIX_TOUCHSCREEN_GT819
{
	I2C_BOARD_INFO("Goodix-TS",(0xb8>>1)),

},
#endif

#ifdef CONFIG_TOUCHSCREEN_GOODIX
	{
		I2C_BOARD_INFO("Goodix-TS",(0xAA>>1)),
	},
#endif
#ifdef CONFIG_TOUCHSCREEN_KEYPAD_TOUCH_KEY
	{
		I2C_BOARD_INFO("touch_key",(0x2b)),
	},
#endif

};

#endif

#ifdef CONFIG_DM9000
static void __init smdkv210_dm9000_set(void)
{
	unsigned int tmp;

	tmp = ((0<<28)|(0<<24)|(5<<16)|(0<<12)|(0<<8)|(0<<4)|(0<<0));
	__raw_writel(tmp, (S5P_SROM_BW+0x18));

	tmp = __raw_readl(S5P_SROM_BW);
	tmp &= ~(0xf << 20);

#ifdef CONFIG_DM9000_16BIT
	tmp |= (0x1 << 20);
#else
	tmp |= (0x2 << 20);
#endif
	__raw_writel(tmp, S5P_SROM_BW);

	tmp = __raw_readl(S5PV210_MP01CON);
	tmp &= ~(0xf << 20);
	tmp |= (2 << 20);

	__raw_writel(tmp, S5PV210_MP01CON);
}
#endif
/*   //delete by liao  for android mem error
#ifdef CONFIG_ANDROID_PMEM
static struct android_pmem_platform_data pmem_pdata = {
	.name = "pmem",
	.no_allocator = 1,
	.cached = 1,
	.start = 0, // will be set during proving pmem driver.
	.size = 0 // will be set during proving pmem driver.
};

static struct android_pmem_platform_data pmem_gpu1_pdata = {
   .name = "pmem_gpu1",
   .no_allocator = 1,
   .cached = 1,
   .buffered = 1,
   .start = 0,
   .size = 0,
};

static struct android_pmem_platform_data pmem_adsp_pdata = {
   .name = "pmem_adsp",
   .no_allocator = 1,
   .cached = 1,
   .buffered = 1,
   .start = 0,
   .size = 0,
};

static struct platform_device pmem_device = {
   .name = "android_pmem",
   .id = 0,
   .dev = { .platform_data = &pmem_pdata },
};

static struct platform_device pmem_gpu1_device = {
	.name = "android_pmem",
	.id = 1,
	.dev = { .platform_data = &pmem_gpu1_pdata },
};

static struct platform_device pmem_adsp_device = {
	.name = "android_pmem",
	.id = 2,
	.dev = { .platform_data = &pmem_adsp_pdata },
};

static void __init android_pmem_set_platdata(void)
{
	pmem_pdata.start = (u32)s3c_get_media_memory_bank(S3C_MDEV_PMEM, 0);
	pmem_pdata.size = (u32)s3c_get_media_memsize_bank(S3C_MDEV_PMEM, 0);

	pmem_gpu1_pdata.start = (u32)s3c_get_media_memory_bank(S3C_MDEV_PMEM_GPU1, 0);
	pmem_gpu1_pdata.size = (u32)s3c_get_media_memsize_bank(S3C_MDEV_PMEM_GPU1, 0);

	pmem_adsp_pdata.start = (u32)s3c_get_media_memory_bank(S3C_MDEV_PMEM_ADSP, 0);
	pmem_adsp_pdata.size = (u32)s3c_get_media_memsize_bank(S3C_MDEV_PMEM_ADSP, 0);
}
#endif
*/
struct platform_device sec_device_battery = {
	.name	= "sec-fake-battery",
	.id		= -1,
};

static struct platform_device device_pm_dev = {
	.name		= "device-pm",
	.id		= -1,
};
extern struct gpio_regulator_platform_data v210_gpio_regs_platform_data;
static struct platform_device v210_gpio_regulators_dev = {
	.name		= "gpio-regulators",
	.id		= -1,
	.dev = { .platform_data = &v210_gpio_regs_platform_data },
};
//davied add for bt_power control
#ifdef CONFIG_BT_POWER
static struct platform_device bt_power_device = {
	.name = "bt_power",
};
extern int bluetooth_power(int on);
static void __init bt_power_init(void)
{
	bt_power_device.dev.platform_data = &bluetooth_power;		
}
#endif
//--
static struct platform_device *smdkv210_devices[] __initdata = {
	/* last enable to support regulator on/off in device driver */
	&s3c_device_i2c0,
	//&s3c_device_i2c1,
	//&s3c_device_i2c2,
	&v210_gpio_regulators_dev,
	&device_pm_dev,
#ifdef  CONFIG_I2C_GPIO  //modify by xu_bin 110507
	&s3c_device_gpio_i2c3,
	&s3c_device_gpio_i2c4,
#endif
#ifdef CONFIG_MTD_ONENAND
	&s3c_device_onenand,
#endif
#ifdef CONFIG_MTD_NAND
	&s3c_device_nand,
#endif
#ifdef CONFIG_S3C_DEV_HSMMC
	&s3c_device_hsmmc0,
#endif
#ifdef CONFIG_S3C_DEV_HSMMC1
	&s3c_device_hsmmc1,
#endif
#ifdef CONFIG_S3C_DEV_HSMMC2
	&s3c_device_hsmmc2,
#endif
#ifdef CONFIG_S3C_DEV_HSMMC3
	&s3c_device_hsmmc3,
#endif
#ifdef CONFIG_FB_S3C
	&s3c_device_fb,
#endif
	&s3c_device_keypad,
#ifdef CONFIG_TOUCHSCREEN_S3C
	&s3c_device_ts,
#endif
#ifdef CONFIG_S5PV210_ADC
	&s3c_device_adc,
#endif
#ifdef CONFIG_DM9000
	&s5p_device_dm9000,
#endif
#ifdef CONFIG_S3C2410_WATCHDOG
	&s3c_device_wdt,
#endif
#if defined(CONFIG_BLK_DEV_IDE_S3C)
	&s3c_device_cfcon,
#endif
#ifdef CONFIG_RTC_DRV_S3C
	&s5p_device_rtc,
#endif
#ifdef CONFIG_HAVE_PWM
	&s3c_device_timer[0],
	&s3c_device_timer[1],
	&s3c_device_timer[2],
	&s3c_device_timer[3],
#endif

#ifdef CONFIG_SND_S3C24XX_SOC
	&s3c64xx_device_iis0,
#endif
#ifdef CONFIG_SND_S3C_SOC_AC97
	&s3c64xx_device_ac97,
#endif
#ifdef CONFIG_SND_S3C_SOC_PCM
	&s3c64xx_device_pcm1,
#endif
#ifdef CONFIG_SND_S5P_SOC_SPDIF
	&s5p_device_spdif,
#endif
#ifdef CONFIG_VIDEO_FIMC
	&s3c_device_fimc0,
	&s3c_device_fimc1,
	&s3c_device_fimc2,
	&s3c_device_csis,
	&s3c_device_ipc,
#endif
#ifdef CONFIG_VIDEO_MFC50
	&s3c_device_mfc,
#endif

#ifdef CONFIG_VIDEO_JPEG_V2
	&s3c_device_jpeg,
#endif

#ifdef CONFIG_VIDEO_ROTATOR
	&s5p_device_rotator,
#endif

#ifdef CONFIG_USB
	&s3c_device_usb_ehci,
	&s3c_device_usb_ohci,
#endif
#ifdef CONFIG_USB_GADGET
	&s3c_device_usbgadget,
#endif
#ifdef CONFIG_USB_ANDROID
	&s3c_device_android_usb,
	&s3c_device_usb_mass_storage,
#endif



#ifdef CONFIG_SPI_CNTRLR_0
        &s3c_device_spi0,
#endif
#ifdef CONFIG_SPI_CNTRLR_1
        &s3c_device_spi1,
#endif

#ifdef CONFIG_VIDEO_TV20
	&s5p_device_tvout,
	&s5p_device_cec,
	&s5p_device_hpd,
#endif
/*   //delete by liao for android mem
#ifdef CONFIG_ANDROID_PMEM
	&pmem_device,
	&pmem_gpu1_device,
	&pmem_adsp_device,
#endif
*/
	&sec_device_battery,
#ifdef CONFIG_VIDEO_G2D
	&s5p_device_g2d,
#endif
#ifdef CONFIG_BT_POWER
	&bt_power_device,
#endif
#if CONFIG_RECOVERY
	&s5p_device_recovery,
#endif
};
static void __init smdkv210_fixup(struct machine_desc *desc,
                                       struct tag *tags, char **cmdline,
                                       struct meminfo *mi)
{
	DPNOK(desc);
#if CONFIG_SMDKC110_D_TYPE
       mi->bank[0].start = 0x30000000;
       mi->bank[0].size = 80 * SZ_1M;
       mi->bank[0].node = 0;
#else if CONFIG_SMDKC110_N_TYPE
       mi->bank[0].start = 0x30000000;
       mi->bank[0].size = 256 * SZ_1M;
       mi->bank[0].node = 0;
#endif
       mi->bank[1].start = 0x40000000;
       mi->bank[1].size = 256 * SZ_1M;
       mi->bank[1].node = 1;
#if CONFIG_SMDKC110_D_TYPE
	mi->bank[2].start = 0x50000000;  //added by liao for mem
	mi->bank[2].size = 128 * SZ_1M;
	mi->bank[2].node = 2;	
        mi->nr_banks = 3;
#else if CONFIG_SMDKC110_N_TYPE
	mi->nr_banks = 2;
#endif
	DPNOK(  mi->nr_banks );	   
}

static void __init smdkv210_map_io(void)
{ //[david.modify] 2010-12-21 20:40
	DPNOK(S5P_VA_CHIPID);

	s5p_init_io(NULL, 0, S5P_VA_CHIPID);
	s3c24xx_init_clocks(24000000);
	s3c24xx_init_uarts(smdkv210_uartcfgs, ARRAY_SIZE(smdkv210_uartcfgs));
	s5pv210_reserve_bootmem();

#ifdef CONFIG_MTD_ONENAND
	s3c_device_onenand.name = "s5pc110-onenand";
#endif
#ifdef CONFIG_MTD_NAND
	s3c_device_nand.name = "s5pv210-nand";
#endif
	DPNOK(0);
}

#ifdef CONFIG_S3C_SAMSUNG_PMEM
static void __init s3c_pmem_set_platdata(void)
{
	DPNOK(S3C_MDEV_PMEM);
	pmem_pdata.start = s3c_get_media_memory_bank(S3C_MDEV_PMEM, 1);
	pmem_pdata.size = s3c_get_media_memsize_bank(S3C_MDEV_PMEM, 1);
}
#endif

#ifdef CONFIG_FB_S3C_LTE480WV
static struct s3c_platform_fb lte480wv_fb_data __initdata = {
	.hw_ver	= 0x62,
	.nr_wins = 5,
	.default_win = CONFIG_FB_S3C_DEFAULT_WINDOW,
	.swap = FB_SWAP_WORD | FB_SWAP_HWORD,
	.lcd_on = s3cfb_lcd_on,
};
#endif
/* this function are used to detect s5pc110 chip version temporally */

int s5pc110_version ;

void _hw_version_check(void)
{
	void __iomem * phy_address ;
	int temp; 

	phy_address = ioremap (0x40,1);

	temp = __raw_readl(phy_address);


	if (temp == 0xE59F010C)
	{
		s5pc110_version = 0;
	}
	else
	{
		s5pc110_version=1 ;
	}
	printk("S5PC110 Hardware version : EVT%d \n",s5pc110_version);
	
	iounmap(phy_address);
}

/* Temporally used
 * return value 0 -> EVT 0
 * value 1 -> evt 1
 */

int hw_version_check(void)
{
	return s5pc110_version ;
}
EXPORT_SYMBOL(hw_version_check);

static void smdkv210_power_off(void)
{
#if 1//ZhangMing for poweroff fail when in charge
	//act8937_write(0x22,act8937_read(0x22)&(~(0x01<<7)));
	//act8937_write(0x55,act8937_read(0x55)&(~(0x01<<7)));
	/* PS_HOLD --> Output Low */
	//printk(KERN_EMERG "%s : setting GPIO_PDA_PS_HOLD low.\n", __func__);
	/* PS_HOLD output High --> Low  PS_HOLD_CONTROL, R/W, 0xE010_E81C */
	//writel(readl(S5P_PSHOLD_CONTROL) & 0xFFFFFEFF, S5P_PSHOLD_CONTROL);
	act8937_write(0x22,act8937_read(0x22)&(~(0x01<<7)));
	act8937_write(0x55,act8937_read(0x55)&(~(0x01<<7)));

	/* PS_HOLD --> Output Low */
	printk(KERN_EMERG "%s : setting GPIO_PDA_PS_HOLD low.\n", __func__);
	/* PS_HOLD output High --> Low  PS_HOLD_CONTROL, R/W, 0xE010_E81C */

	writel(0x201, S5P_PSHOLD_CONTROL);

	printk(KERN_EMERG "%s : after setting GPIO_PDA_PS_HOLD low.\n", __func__);

	{
		void __iomem *map_reg;
		
		printk(KERN_EMERG "%s : acin present, softreset to enter uboot.\n", __func__);
		
		map_reg = ioremap((unsigned long)0xE0102000, 0x20);
		writel(0x01, map_reg);
	}
#else
	/*belowing according to uboot settings*/
	writel(readl(S5PV210_GPC0DAT) & (~(1<<4)), S5PV210_GPC0DAT);
	writel(readl(S5PV210_GPC0CONPDN) & (~0x300), S5PV210_GPC0CONPDN);

	writel(readl(S5PV210_GPC1DAT) & (~(1<<0)), S5PV210_GPC1DAT);
	writel(readl(S5PV210_GPC1CONPDN) & (~0x3), S5PV210_GPC1CONPDN);

	writel(readl(S5PV210_GPC0PUD) & (~0x300), S5PV210_GPC0PUD);
	writel(readl(S5PV210_GPC0CON) & (~0xF0000) | 0x10000, S5PV210_GPC0CON);
	writel(readl(S5PV210_GPC0PUDPDN) & (~0x300), S5PV210_GPC0PUDPDN);

	writel(readl(S5PV210_GPC1PUD) & (~0x3), S5PV210_GPC1PUD);
	writel(readl(S5PV210_GPC1CON) & (~0xF) | 0x1, S5PV210_GPC1CON);
	writel(readl(S5PV210_GPC1PUDPDN) & (~0x3), S5PV210_GPC1PUDPDN);

	act8937_write(0x61, act8937_read(0x61) & (~0x80));
	act8937_write(0x65, act8937_read(0x65) & (~0x80));

	printk(KERN_EMERG "%s : setting GPIO_PDA_PS_HOLD low .\n", __func__);

	writel(0x201, S5P_PSHOLD_CONTROL);

	{
		void __iomem *map_reg;
		
		printk(KERN_EMERG "%s : acin present, softreset to enter uboot.\n", __func__);
		
		map_reg = ioremap((unsigned long)0xE0102000, 0x20);
		writel(0x01, map_reg);
	}
	
#endif	

	while(1);

	printk(KERN_EMERG "%s : should not reach here!\n", __func__);
}

#ifdef CONFIG_VIDEO_TV20
void s3c_set_qos()
{
	/* VP QoS */
	__raw_writel(0x00400001, S5P_VA_DMC0 + 0xC8);
	__raw_writel(0x387F0022, S5P_VA_DMC0 + 0xCC);
	/* MIXER QoS */
	__raw_writel(0x00400001, S5P_VA_DMC0 + 0xD0);
	__raw_writel(0x3FFF0062, S5P_VA_DMC0 + 0xD4);
	/* LCD1 QoS */
	__raw_writel(0x00800001, S5P_VA_DMC1 + 0x90);
	__raw_writel(0x3FFF005B, S5P_VA_DMC1 + 0x94);
	/* LCD2 QoS */
	__raw_writel(0x00800001, S5P_VA_DMC1 + 0x98);
	__raw_writel(0x3FFF015B, S5P_VA_DMC1 + 0x9C);
	/* VP QoS */
	__raw_writel(0x00400001, S5P_VA_DMC1 + 0xC8);
	__raw_writel(0x387F002B, S5P_VA_DMC1 + 0xCC);
	/* DRAM Controller QoS */
	__raw_writel((__raw_readl(S5P_VA_DMC0)&~(0xFFF<<16)|(0x100<<16)),
			S5P_VA_DMC0 + 0x0);
	__raw_writel((__raw_readl(S5P_VA_DMC1)&~(0xFFF<<16)|(0x100<<16)),
			S5P_VA_DMC1 + 0x0);
	/* BUS QoS AXI_DSYS Control */
	__raw_writel(0x00000007, S5P_VA_BUS_AXI_DSYS + 0x400);
	__raw_writel(0x00000007, S5P_VA_BUS_AXI_DSYS + 0x420);
	__raw_writel(0x00000030, S5P_VA_BUS_AXI_DSYS + 0x404);
	__raw_writel(0x00000030, S5P_VA_BUS_AXI_DSYS + 0x424);
}
#endif
//jeff
//reset GPH2, GPH3 CON, because these registers is config to keypad in uboot 
void s3c_setup_keypad_gpio_reset(void)
{
	DPNOK(0);
	unsigned int gpio;
	unsigned int end;
	
	//end = S5PV210_GPH3(8);
	end = S5PV210_GPH3(2);  //LIAO 
	/* Set all the necessary GPH2 pins to special-function 0 */
	for (gpio = S5PV210_GPH3(0); gpio < end; gpio++) {
		s3c_gpio_cfgpin(gpio, S3C_GPIO_SFN(0));
		s3c_gpio_setpull(gpio, S3C_GPIO_PULL_NONE);
	}

	//end = S5PV210_GPH2(8);
	end = S5PV210_GPH2(3);  //LIAO
	/* Set all the necessary GPK pins to special-function 0 */
	for (gpio = S5PV210_GPH2(0); gpio < end; gpio++) {
		s3c_gpio_cfgpin(gpio, S3C_GPIO_SFN(0));
		s3c_gpio_setpull(gpio, S3C_GPIO_PULL_NONE);
	}
}

// ++++++2011.05.06  SET some gpio idle  mode
/*platform related*/
//S5PV210_GPI(5),S5PV210_GPI(6),
//SET 3G modem UART Idle

/*
			S5PV210_MP01(0),S5PV210_MP01(1),
			S5PV210_MP01(2),S5PV210_MP01(3),
			S5PV210_MP01(6),S5PV210_MP01(7),
			S5PV210_MP02(0),S5PV210_MP02(1),
			S5PV210_MP02(3),
			S5PV210_MP03(0),S5PV210_MP03(1),
			S5PV210_MP03(2),S5PV210_MP03(3),
			S5PV210_MP03(6),S5PV210_MP03(7),
			S5PV210_MP04(0),S5PV210_MP04(1),
			S5PV210_MP04(2),S5PV210_MP04(3),
			S5PV210_MP04(4),S5PV210_MP04(5),
			S5PV210_MP04(6),S5PV210_MP04(7),
			S5PV210_MP05(0),S5PV210_MP05(1),
			S5PV210_MP05(2),S5PV210_MP05(3),
			S5PV210_MP05(4),S5PV210_MP05(5),
			S5PV210_MP05(6),S5PV210_MP05(7),
			S5PV210_MP06(0),S5PV210_MP06(1),
			S5PV210_MP06(2),S5PV210_MP06(3),
			S5PV210_MP06(4),S5PV210_MP06(5),
			S5PV210_MP06(6),S5PV210_MP06(7),
			S5PV210_MP07(0),S5PV210_MP07(1),
			S5PV210_MP07(2),S5PV210_MP07(3),
			S5PV210_MP07(4),S5PV210_MP07(5),
			S5PV210_MP07(6),S5PV210_MP07(7),
*/
//S5PV210_GPH0(1), -->SET CHAGER LEVEL  500mA
void s3c_setup_idle_gpio(void)
{
	printk(">>>>>S3c_setup_idle_gpio....\n");
	unsigned int NC_GPIO[]={
			S5PV210_GPA0(4),S5PV210_GPA0(5),  
			S5PV210_GPA0(6),S5PV210_GPA0(7),
			S5PV210_GPB(0),S5PV210_GPB(1),
			S5PV210_GPB(3),S5PV210_GPB(7),
			S5PV210_GPC0(1),S5PV210_GPC0(2),
			S5PV210_GPC1(0),S5PV210_GPC1(1),
			S5PV210_GPH2(6),S5PV210_GPH2(7),
			S5PV210_GPH3(3),S5PV210_GPH3(4),
			S5PV210_GPI(5),S5PV210_GPI(6),
			S5PV210_ETC0(0),S5PV210_ETC0(1),
			S5PV210_ETC0(2),S5PV210_ETC0(3),
			S5PV210_ETC0(4),S5PV210_ETC0(5),
			S5PV210_ETC4(3),S5PV210_ETC2(2),
			S5PV210_ETC2(3),S5PV210_GPH0(1),
			S5PV210_GPH3(7),S5PV210_GPG0(2),
			S5PV210_GPG3(2),S5PV210_GPJ0(0),
			S5PV210_GPJ0(1),S5PV210_GPJ0(2),
			S5PV210_GPJ0(3),S5PV210_GPJ0(4),
			S5PV210_GPJ0(5),S5PV210_GPJ0(6),
			S5PV210_GPJ0(7),S5PV210_GPJ1(0),
			S5PV210_GPJ1(1),S5PV210_GPJ1(2),
			S5PV210_GPJ1(3),S5PV210_GPJ1(4),
			S5PV210_GPJ1(5),S5PV210_GPJ2(0),
			S5PV210_GPJ2(1),S5PV210_GPJ2(2),
			S5PV210_GPJ2(3),S5PV210_GPJ2(4),
			S5PV210_GPJ2(5),S5PV210_GPJ2(6),
			S5PV210_GPJ2(7),S5PV210_GPJ3(0),
			S5PV210_GPJ3(1),S5PV210_GPJ3(2),
			S5PV210_GPJ3(3),S5PV210_GPJ3(4),
			S5PV210_GPJ3(5),S5PV210_GPJ3(6),
			S5PV210_GPJ3(7),S5PV210_GPJ4(0),
			S5PV210_GPJ4(1),S5PV210_GPJ4(2),
			S5PV210_GPJ4(3),S5PV210_GPJ4(4)};
	unsigned int i, num=0;

	num = sizeof(NC_GPIO)/sizeof(unsigned int);
	for (i = 0; i < num; i++) {
		s3c_gpio_cfgpin(NC_GPIO[i], S3C_GPIO_SFN(0));
		s3c_gpio_setpull(NC_GPIO[i], S3C_GPIO_PULL_DOWN);
	}
}
// ------- 
static void __init smdkv210_machine_init(void)
{

	DPNOK(0);
	/* Find out S5PC110 chip version */
	_hw_version_check();

	/* OneNAND */
#ifdef CONFIG_MTD_ONENAND
	//s3c_device_onenand.dev.platform_data = &s5p_onenand_data;
#endif
#ifdef CONFIG_MTD_NAND
	//s3c_device_nand.dev.platform_data = &s5p_nand_data;
#endif

#ifdef CONFIG_DM9000
	smdkv210_dm9000_set();
#endif
/*
#ifdef CONFIG_ANDROID_PMEM
	android_pmem_set_platdata();
#endif
*/
	/* i2c */
	s3c_i2c0_set_platdata(NULL);
	//s3c_i2c1_set_platdata(NULL);
	//s3c_i2c2_set_platdata(NULL);
	i2c_register_board_info(0, i2c_devs0, ARRAY_SIZE(i2c_devs0));
	//i2c_register_board_info(1, i2c_devs1, ARRAY_SIZE(i2c_devs1));
	//i2c_register_board_info(2, i2c_devs2, ARRAY_SIZE(i2c_devs2));

#ifdef CONFIG_I2C_GPIO  //modify by xu_bin 110507
       i2c_register_board_info(3, i2c_devs3, ARRAY_SIZE(i2c_devs3));
       i2c_register_board_info(4, i2c_devs4, ARRAY_SIZE(i2c_devs4)); //liao 
#endif
	/* to support system shut down */
	pm_power_off = smdkv210_power_off;
#if defined(CONFIG_SPI_CNTRLR_0)
        s3cspi_set_slaves(BUSNUM(0), ARRAY_SIZE(s3c_slv_pdata_0), s3c_slv_pdata_0);
#endif
#if defined(CONFIG_SPI_CNTRLR_1)
        s3cspi_set_slaves(BUSNUM(1), ARRAY_SIZE(s3c_slv_pdata_1), s3c_slv_pdata_1);
#endif
#if defined(CONFIG_SPI_CNTRLR_2)
        s3cspi_set_slaves(BUSNUM(2), ARRAY_SIZE(s3c_slv_pdata_2), s3c_slv_pdata_2);
#endif
        spi_register_board_info(s3c_spi_devs, ARRAY_SIZE(s3c_spi_devs));

#ifdef CONFIG_FB_S3C_LTE480WV
	s3cfb_set_platdata(&lte480wv_fb_data);
#endif

#if defined(CONFIG_BLK_DEV_IDE_S3C)
	s3c_ide_set_platdata(&smdkv210_ide_pdata);
#endif

#if defined(CONFIG_TOUCHSCREEN_S3C)
	s3c_ts_set_platdata(&s3c_ts_platform);
#endif

#if defined(CONFIG_S5PV210_ADC)
	s3c_adc_set_platdata(&s3c_adc_platform);
#endif

#if defined(CONFIG_PM)
	s3c_pm_init();
#endif
#ifdef CONFIG_VIDEO_FIMC
	/* fimc */
	s3c_fimc0_set_platdata(&fimc_plat);
	s3c_fimc1_set_platdata(&fimc_plat);
	s3c_fimc2_set_platdata(&fimc_plat);
	s3c_csis_set_platdata(NULL);
	smdkv210_cam0_power(1);  //main for camera CLk
//	smdkv210_cam1_power(1);
//	smdkv210_mipi_cam_reset();
#endif
//#ifdef CONFIG_VIDEO_JPEG_V2
//	s3c_jpeg_set_platdata(&jpeg_plat);
//#endif
#ifdef CONFIG_VIDEO_MFC50
	/* mfc */
	s3c_mfc_set_platdata(NULL);
#endif

#ifdef CONFIG_VIDEO_TV20
	s3c_set_qos();
#endif

#ifdef CONFIG_S3C_DEV_HSMMC
	s5pv210_default_sdhci0();
#endif
#ifdef CONFIG_S3C_DEV_HSMMC1
	s5pv210_default_sdhci1();
#endif
#ifdef CONFIG_S3C_DEV_HSMMC2
	s5pv210_default_sdhci2();
#endif
#ifdef CONFIG_S3C_DEV_HSMMC3
	s5pv210_default_sdhci3();
#endif
#ifdef CONFIG_S5PV210_SETUP_SDHCI
	s3c_sdhci_set_platdata();
#endif
//davied add for bt
#ifdef CONFIG_BT_POWER
	bt_power_init();
#endif
//--
	platform_add_devices(smdkv210_devices, ARRAY_SIZE(smdkv210_devices));
#if defined(CONFIG_HAVE_PWM)
	smdk_backlight_register();
#endif
	//jeff	
	s3c_setup_keypad_gpio_reset();
	s3c_setup_idle_gpio();  // ++++   2011.05.06
}

#ifdef CONFIG_USB_SUPPORT
/* Initializes OTG Phy. */
void otg_phy_init(void)
{
	__raw_writel(__raw_readl(S5P_USB_PHY_CONTROL)
		|(0x1<<0), S5P_USB_PHY_CONTROL); /*USB PHY0 Enable */
	__raw_writel((__raw_readl(S3C_USBOTG_PHYPWR)
		&~(0x3<<3)&~(0x1<<0))|(0x1<<5), S3C_USBOTG_PHYPWR);
	__raw_writel((__raw_readl(S3C_USBOTG_PHYCLK)
		&~(0x5<<2))|(0x3<<0), S3C_USBOTG_PHYCLK);
	__raw_writel((__raw_readl(S3C_USBOTG_RSTCON)
		&~(0x3<<1))|(0x1<<0), S3C_USBOTG_RSTCON);
	udelay(10);
	__raw_writel(__raw_readl(S3C_USBOTG_RSTCON)
		&~(0x7<<0), S3C_USBOTG_RSTCON);
	udelay(10);
}
EXPORT_SYMBOL(otg_phy_init);

/* USB Control request data struct must be located here for DMA transfer */
//struct usb_ctrlrequest usb_ctrl __attribute__((aligned(8)));

/* OTG PHY Power Off */
void otg_phy_off(void)
{
	__raw_writel(__raw_readl(S3C_USBOTG_PHYPWR)
		|(0x3<<3), S3C_USBOTG_PHYPWR);
	__raw_writel(__raw_readl(S5P_USB_PHY_CONTROL)
		&~(1<<0), S5P_USB_PHY_CONTROL);
}
EXPORT_SYMBOL(otg_phy_off);

void usb_host_phy_init(void)
{
	struct clk *otg_clk;

	otg_clk = clk_get(NULL, "usbotg");
	clk_enable(otg_clk);

	if (readl(S5P_USB_PHY_CONTROL) & (0x1<<1))
		return;

	__raw_writel(__raw_readl(S5P_USB_PHY_CONTROL)
		|(0x1<<1), S5P_USB_PHY_CONTROL);
	__raw_writel((__raw_readl(S3C_USBOTG_PHYPWR)
		&~(0x1<<7)&~(0x1<<6))|(0x1<<8)|(0x1<<5), S3C_USBOTG_PHYPWR);
	__raw_writel((__raw_readl(S3C_USBOTG_PHYCLK)
		&~(0x1<<7))|(0x3<<0), S3C_USBOTG_PHYCLK);
	__raw_writel((__raw_readl(S3C_USBOTG_RSTCON))
		|(0x1<<4)|(0x1<<3), S3C_USBOTG_RSTCON);
	__raw_writel(__raw_readl(S3C_USBOTG_RSTCON)
		&~(0x1<<4)&~(0x1<<3), S3C_USBOTG_RSTCON);
}
EXPORT_SYMBOL(usb_host_phy_init);

void usb_host_phy_off(void)
{
	__raw_writel(__raw_readl(S3C_USBOTG_PHYPWR)
		|(0x1<<7)|(0x1<<6), S3C_USBOTG_PHYPWR);
	__raw_writel(__raw_readl(S5P_USB_PHY_CONTROL)
		&~(1<<1), S5P_USB_PHY_CONTROL);
}
EXPORT_SYMBOL(usb_host_phy_off);
#endif

#if defined(CONFIG_KEYPAD_S3C) || defined(CONFIG_KEYPAD_S3C_MODULE)
#if defined(CONFIG_KEYPAD_S3C_MSM)
void s3c_setup_keypad_cfg_gpio(void)
{
	unsigned int gpio;
	unsigned int end;


	DPNOK(0);
	/* gpio setting for KP_COL0 */
	s3c_gpio_cfgpin(S5PV210_GPJ1(5), S3C_GPIO_SFN(3));
	s3c_gpio_setpull(S5PV210_GPJ1(5), S3C_GPIO_PULL_NONE);

	/* gpio setting for KP_COL1 ~ KP_COL7 and KP_ROW0 */
	end = S5PV210_GPJ2(8);
	for (gpio = S5PV210_GPJ2(0); gpio < end; gpio++) {
		s3c_gpio_cfgpin(gpio, S3C_GPIO_SFN(3));
		s3c_gpio_setpull(gpio, S3C_GPIO_PULL_NONE);
	}

	/* gpio setting for KP_ROW1 ~ KP_ROW8 */
	end = S5PV210_GPJ3(8);
	for (gpio = S5PV210_GPJ3(0); gpio < end; gpio++) {
		s3c_gpio_cfgpin(gpio, S3C_GPIO_SFN(3));
		s3c_gpio_setpull(gpio, S3C_GPIO_PULL_NONE);
	}

	/* gpio setting for KP_ROW9 ~ KP_ROW13 */
	end = S5PV210_GPJ4(5);
	for (gpio = S5PV210_GPJ4(0); gpio < end; gpio++) {
		s3c_gpio_cfgpin(gpio, S3C_GPIO_SFN(3));
		s3c_gpio_setpull(gpio, S3C_GPIO_PULL_NONE);
	}
}
#else
void s3c_setup_keypad_cfg_gpio(int rows, int columns)
{
	unsigned int gpio;
	unsigned int end;

	//printk("####>>>>> key _ rows =%d\n",rows);
	//printk("####>>>>> key _ columns = %d\n",columns);
	end = S5PV210_GPH3(rows);

	/* Set all the necessary GPH2 pins to special-function 0 */
	for (gpio = S5PV210_GPH3(0); gpio < end; gpio++) {
		s3c_gpio_cfgpin(gpio, S3C_GPIO_SFN(3));
		s3c_gpio_setpull(gpio, S3C_GPIO_PULL_UP);
	}

	end = S5PV210_GPH2(columns);

	/* Set all the necessary GPK pins to special-function 0 */
	for (gpio = S5PV210_GPH2(0); gpio < end; gpio++) {
		s3c_gpio_cfgpin(gpio, S3C_GPIO_SFN(3));
		s3c_gpio_setpull(gpio, S3C_GPIO_PULL_NONE);
	}
}
#endif /* if defined(CONFIG_KEYPAD_S3C_MSM)*/
EXPORT_SYMBOL(s3c_setup_keypad_cfg_gpio);
#endif

MACHINE_START(SMDKC110, "SMDKC110")
	/* Maintainer: Kukjin Kim <kgene.kim@samsung.com> */
	.phys_io	= S3C_PA_UART & 0xfff00000,
	.io_pg_offst	= (((u32)S3C_VA_UART) >> 18) & 0xfffc,
	.boot_params	= S5P_PA_SDRAM + 0x100,
	.fixup		= smdkv210_fixup,
	.init_irq	= s5pv210_init_irq,
	.map_io		= smdkv210_map_io,
	.init_machine	= smdkv210_machine_init,
	.timer		= &s5p_systimer,
MACHINE_END
