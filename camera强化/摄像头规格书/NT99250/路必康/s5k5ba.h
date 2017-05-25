/* linux/drivers/media/video/s5k5ba.h
 *
 * Copyright (c) 2010 Samsung Electronics Co., Ltd.
 * 		http://www.samsung.com/
 *
 * Driver for S5K5BA (UXGA camera) from Samsung Electronics
 * 1/4" 2.0Mp CMOS Image Sensor SoC with an Embedded Image Processor
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#define S5K5BA_COMPLETE
#ifndef __S5K5BA_H__
#define __S5K5BA_H__
#if 0
struct s5k5ba_reg {
	unsigned short addr;
	unsigned short val;
};
#else
struct s5k5ba_reg {
	unsigned char addr;
	unsigned char val;
};
#endif

struct s5k5ba_regset_type {
	unsigned char *regset;
	int len;
};

/*
 * Macro
 */
#define REGSET_LENGTH(x)	(sizeof(x)/sizeof(s5k5ba_reg))

/*
 * User defined commands
 */
/* S/W defined features for tune */
#define REG_DELAY	0xFF00	/* in ms */
#define REG_CMD		0xFFFF	/* Followed by command */

/* Following order should not be changed */
enum image_size_s5k5ba {
	/* This SoC supports upto UXGA (1600*1200) */
#if 0
	QQVGA,	/* 160*120*/
	QCIF,	/* 176*144 */
	QVGA,	/* 320*240 */
	CIF,	/* 352*288 */
#endif	
	VGA,	/* 640*480 */
#if 0
	SVGA,	/* 800*600 */

	HD720P,	/* 1280*720 */
	SXGA,	/* 1280*1024 */
	UXGA,	/* 1600*1200 */
#endif
};

/*
 * Following values describe controls of camera
 * in user aspect and must be match with index of s5k5ba_regset[]
 * These values indicates each controls and should be used
 * to control each control
 */
enum s5k5ba_control {
	S5K5BA_INIT,
	S5K5BA_EV,
	S5K5BA_AWB,
	S5K5BA_MWB,
	S5K5BA_EFFECT,
	S5K5BA_CONTRAST,
	S5K5BA_SATURATION,
	S5K5BA_SHARPNESS,
};

#define S5K5BA_REGSET(x)	{	\
	.regset = x,			\
	.len = sizeof(x)/sizeof(s5k5ba_reg),}


/*
 * User tuned register setting values
 */

 #if 1
static unsigned short s5k5ba_init0[][2] = 
{
	{0xffff, 0xff},
};
 
 static unsigned short s5k5ba_init_reg[][2]={
	{0x3024,0x02}, //TG	 
	{0x32F0,0x01},//0:UYVY  2:VYUY  1:YUYV   3:YVYU
	{0x301e,0x54},
	{0x301f,0x48},
	//GammaHDR 
	{0x3270,0x00},
	{0x3271,0x04},
	{0x3272,0x0E},
	{0x3273,0x28},
	{0x3274,0x3F},
	{0x3275,0x50},
	{0x3276,0x6E},
	{0x3277,0x88},
	{0x3278,0xA0},
	{0x3279,0xB3},
	{0x327A,0xD2},
	{0x327B,0xE8},
	{0x327C,0xF5},
	{0x327D,0xFF},
	{0x327E,0xFF},

	//CC ++++
	{0x3302,0x00},
	{0x3303,0x2E},
	{0x3304,0x00},
	{0x3305,0xB7},
	{0x3306,0x00},
	{0x3307,0x1A},
	{0x3308,0x07},
	{0x3309,0xE7},
	{0x330A,0x07},
	{0x330B,0x44},
	{0x330C,0x00},
	{0x330D,0xD6},
	{0x330E,0x01},
	{0x330F,0x01},
	{0x3310,0x07},
	{0x3311,0x1A},
	{0x3312,0x07},
	{0x3313,0xE5},
	/*//CC_MS
	{0x3302,0x00},
	{0x3303,0x25},
	{0x3304,0x00},
	{0x3305,0xB7},
	{0x3306,0x00},
	{0x3307,0x23},
	{0x3308,0x07},
	{0x3309,0xFC},
	{0x330A,0x06},
	{0x330B,0xB5},
	{0x330C,0x01},
	{0x330D,0x4F},
	{0x330E,0x01},
	{0x330F,0x2A},
	{0x3310,0x06},
	{0x3311,0xE7},
	{0x3312,0x07},
	{0x3313,0xEF},*/

	//LSC_+++++
	{0x3250,0x01},  
	{0x3251,0x87}, 
	{0x3252,0x01}, 
	{0x3253,0x88}, 
	{0x3254,0x01}, 
	{0x3255,0x89}, 
	{0x3256,0x01}, 
	{0x3257,0x33}, 
	{0x3258,0x01}, 
	{0x3259,0x34}, 
	{0x325A,0x01}, 
	{0x325B,0x35}, 
	{0x325C,0x00}, 
	{0x325D,0x00}, 
	{0x325E,0x00}, 
	{0x325F,0x00}, 
	{0x3260,0x00}, 
	{0x3261,0x00}, 
	{0x3262,0x09}, 
	{0x3263,0x08}, 
	{0x3264,0x08}, 
	{0x3265,0x17}, 
	{0x3266,0x00},  
	{0x3200,0x3e},
	//LSC_----  
	{0x3102,0x0b},
	{0x3103,0x46},
	{0x3105,0x33},
	{0x3107,0x32},
	{0x310A,0x03},
	{0x310B,0x18},
	{0x310f,0x08},
	{0x3110,0x03},
	{0x3113,0x0F},
	{0x3119,0x17},
	{0x3114,0x03},
	{0x3117,0x03},
	{0x3118,0x01},
	{0x3380,0x03},

	{0x3044,0x02},
	{0x3045,0xd0},
	{0x3046,0x02},
	{0x3047,0xd0},
	{0x3048,0x02},
	{0x3049,0xd0},
	{0x304a,0x02},
	{0x304b,0xd0},
	{0x303e,0x02},
	{0x303f,0x2b},
	{0x3052,0x80},
	{0x3059,0x10},
	{0x305a,0x28},
	{0x305b,0x20},
	{0x305c,0x04},
	{0x305d,0x28},
	{0x305e,0x04},
	{0x305f,0x52},
	{0x3058,0x01},

	{0x3080,0x80},
	{0x3081,0x80},
	{0x3082,0x80},
	{0x3083,0x40},
	{0x3084,0x80},
	{0x3085,0x40},


	{0x32BB,0x0b},
	{0x32bd,0x05},
	{0x32be,0x05},
	{0x32cd,0x01},
	{0x32d3,0x13},
	{0x32d7,0x82},
	{0x32d8,0x3F},
	{0x32d9,0x10},
	{0x32c5,0x1f},
	/*
	{0x32b0,0x00},
	{0x32b1,0x90},
	{0x32B2,0x00},
	{0x32B3,0xc8},
	{0x32B4,0x00},
	{0x32B5,0x97},
	{0x32B6,0x02},
	{0x32B7,0x58},
	{0x32B8,0x01},
	{0x32B9,0xc3},*/

	{0x329C,0x4b},
	{0x32bf,0x52},
	{0x32c0,0x10},
	{0x3200,0x3e},

	{0x32b0,0x02},
	{0x32b1,0xc0},

	{0x3290,0x01},  
	{0x3291,0x68},
	{0x3296,0x01},
	{0x3297,0x75},  
	{0x32A9,0x11},
	{0x32AA,0x01},  
	{0x329b,0x01},
	{0x32a2,0x60},
	{0x32a4,0xa0},
	{0x32a6,0x60},
	{0x32a8,0xa0},  //awb init ----
	{0x3012,0x01}, 
	{0x3013,0x80},
	{0x301d,0x08}, 
	{0x3201,0x7f}, 

	{0x32f6,0x0c},
	{0x306d,0x01}, //pclk 

	//[640X480]==========================================
	//edge & denoise +++
	{0x3300,0x30},
	{0x3301,0x80},
	{0x3320,0x28},
	{0x3331,0x04},
	{0x3332,0x40},
	{0x3339,0x10},
	{0x333a,0x1a},
	//edge & denoise ---
	{0x32e0,0x02}, 
	{0x32e1,0x80}, 
	{0x32e2,0x01}, 
	{0x32e3,0xe0}, 
	{0x32e4,0x01}, 
	{0x32e5,0x81}, 
	{0x32e6,0x00}, 
	{0x32e7,0x40}, 
	{0x301e,0x00}, //pll
	{0x301f,0x20}, //pll
	{0x3022,0x25}, 
	{0x3023,0x64}, 
	{0x3002,0x00}, 
	{0x3003,0x04}, 
	{0x3004,0x00}, 
	{0x3005,0x04}, 
	{0x3006,0x06}, 
	{0x3007,0x43}, 
	{0x3008,0x04}, 
	{0x3009,0xb3}, 
	{0x300a,0x09}, 
	{0x300b,0x91}, 
	{0x300c,0x02}, 
	{0x300d,0x91}, 
	{0x300e,0x06}, 
	{0x300f,0x40}, 
	{0x3010,0x02}, 
	{0x3011,0x58}, 
	{0x32bb,0x0b}, 
	{0x32bc,0x30}, 
	{0x32c1,0x22},//0x23},//0x25}, 
	{0x32c2,0xae},//14fps//0xd4},//10fps//0x5c},  //7.14fps @ 48M 
	{0x32c8,0x62}, 
	{0x32c9,0x52}, 
	{0x32c4,0x00}, 
	//{0x3201,0x7f}, 
	{0x3021,0x06}, 
	{0x3060,0x01}, 
	 
	{0xffff,0xff},

 };
 #endif
 
 #define S5K5BA_INIT_REGS	\
	(sizeof(s5k5ba_init_reg) / sizeof(s5k5ba_init_reg[0]))//modify by xu_bin 110418
#define S5K5BA_INIT_REGS1	\
	(sizeof(s5k5ba_init0) / sizeof(s5k5ba_init0[0]))
/*
 * EV bias
 */

static const struct s5k5ba_reg s5k5ba_ev_m6[] = {
};

static const struct s5k5ba_reg s5k5ba_ev_m5[] = {
};

static const struct s5k5ba_reg s5k5ba_ev_m4[] = {
};

static const struct s5k5ba_reg s5k5ba_ev_m3[] = {
};

static const struct s5k5ba_reg s5k5ba_ev_m2[] = {
};

static const struct s5k5ba_reg s5k5ba_ev_m1[] = {
};

static const struct s5k5ba_reg s5k5ba_ev_default[] = {
};

static const struct s5k5ba_reg s5k5ba_ev_p1[] = {
};

static const struct s5k5ba_reg s5k5ba_ev_p2[] = {
};

static const struct s5k5ba_reg s5k5ba_ev_p3[] = {
};

static const struct s5k5ba_reg s5k5ba_ev_p4[] = {
};

static const struct s5k5ba_reg s5k5ba_ev_p5[] = {
};

static const struct s5k5ba_reg s5k5ba_ev_p6[] = {
};

#ifdef S5K5BA_COMPLETE
/* Order of this array should be following the querymenu data */
static const unsigned char *s5k5ba_regs_ev_bias[] = {
	(unsigned char *)s5k5ba_ev_m6, (unsigned char *)s5k5ba_ev_m5,
	(unsigned char *)s5k5ba_ev_m4, (unsigned char *)s5k5ba_ev_m3,
	(unsigned char *)s5k5ba_ev_m2, (unsigned char *)s5k5ba_ev_m1,
	(unsigned char *)s5k5ba_ev_default, (unsigned char *)s5k5ba_ev_p1,
	(unsigned char *)s5k5ba_ev_p2, (unsigned char *)s5k5ba_ev_p3,
	(unsigned char *)s5k5ba_ev_p4, (unsigned char *)s5k5ba_ev_p5,
	(unsigned char *)s5k5ba_ev_p6,
};

/*
 * Auto White Balance configure
 */
static const struct s5k5ba_reg s5k5ba_awb_off[] = {
};

static const struct s5k5ba_reg s5k5ba_awb_on[] = {
};

static const unsigned char *s5k5ba_regs_awb_enable[] = {
	(unsigned char *)s5k5ba_awb_off,
	(unsigned char *)s5k5ba_awb_on,
};

/*
 * Manual White Balance (presets)
 */
static const struct s5k5ba_reg s5k5ba_wb_tungsten[] = {

};

static const struct s5k5ba_reg s5k5ba_wb_fluorescent[] = {

};

static const struct s5k5ba_reg s5k5ba_wb_sunny[] = {

};

static const struct s5k5ba_reg s5k5ba_wb_cloudy[] = {

};

/* Order of this array should be following the querymenu data */
static const unsigned char *s5k5ba_regs_wb_preset[] = {
	(unsigned char *)s5k5ba_wb_tungsten,
	(unsigned char *)s5k5ba_wb_fluorescent,
	(unsigned char *)s5k5ba_wb_sunny,
	(unsigned char *)s5k5ba_wb_cloudy,
};

/*
 * Color Effect (COLORFX)
 */
static const struct s5k5ba_reg s5k5ba_color_sepia[] = {
};

static const struct s5k5ba_reg s5k5ba_color_aqua[] = {
};

static const struct s5k5ba_reg s5k5ba_color_monochrome[] = {
};

static const struct s5k5ba_reg s5k5ba_color_negative[] = {
};

static const struct s5k5ba_reg s5k5ba_color_sketch[] = {
};

/* Order of this array should be following the querymenu data */
static const unsigned char *s5k5ba_regs_color_effect[] = {
	(unsigned char *)s5k5ba_color_sepia,
	(unsigned char *)s5k5ba_color_aqua,
	(unsigned char *)s5k5ba_color_monochrome,
	(unsigned char *)s5k5ba_color_negative,
	(unsigned char *)s5k5ba_color_sketch,
};

/*
 * Contrast bias
 */
static const struct s5k5ba_reg s5k5ba_contrast_m2[] = {
};

static const struct s5k5ba_reg s5k5ba_contrast_m1[] = {
};

static const struct s5k5ba_reg s5k5ba_contrast_default[] = {
};

static const struct s5k5ba_reg s5k5ba_contrast_p1[] = {
};

static const struct s5k5ba_reg s5k5ba_contrast_p2[] = {
};

static const unsigned char *s5k5ba_regs_contrast_bias[] = {
	(unsigned char *)s5k5ba_contrast_m2,
	(unsigned char *)s5k5ba_contrast_m1,
	(unsigned char *)s5k5ba_contrast_default,
	(unsigned char *)s5k5ba_contrast_p1,
	(unsigned char *)s5k5ba_contrast_p2,
};

/*
 * Saturation bias
 */
static const struct s5k5ba_reg s5k5ba_saturation_m2[] = {
};

static const struct s5k5ba_reg s5k5ba_saturation_m1[] = {
};

static const struct s5k5ba_reg s5k5ba_saturation_default[] = {
};

static const struct s5k5ba_reg s5k5ba_saturation_p1[] = {
};

static const struct s5k5ba_reg s5k5ba_saturation_p2[] = {
};

static const unsigned char *s5k5ba_regs_saturation_bias[] = {
	(unsigned char *)s5k5ba_saturation_m2,
	(unsigned char *)s5k5ba_saturation_m1,
	(unsigned char *)s5k5ba_saturation_default,
	(unsigned char *)s5k5ba_saturation_p1,
	(unsigned char *)s5k5ba_saturation_p2,
};

/*
 * Sharpness bias
 */
static const struct s5k5ba_reg s5k5ba_sharpness_m2[] = {
};

static const struct s5k5ba_reg s5k5ba_sharpness_m1[] = {
};

static const struct s5k5ba_reg s5k5ba_sharpness_default[] = {
};

static const struct s5k5ba_reg s5k5ba_sharpness_p1[] = {
};

static const struct s5k5ba_reg s5k5ba_sharpness_p2[] = {
};

static const unsigned char *s5k5ba_regs_sharpness_bias[] = {
	(unsigned char *)s5k5ba_sharpness_m2,
	(unsigned char *)s5k5ba_sharpness_m1,
	(unsigned char *)s5k5ba_sharpness_default,
	(unsigned char *)s5k5ba_sharpness_p1,
	(unsigned char *)s5k5ba_sharpness_p2,
};
#endif /* S5K5BA_COMPLETE */

#endif




