/*
 * A V4L2 driver for  NT99250 cameras.
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/videodev2.h>
#include <media/v4l2-device.h>
#include <media/v4l2-chip-ident.h>
#include <media/v4l2-i2c-drv.h>
#include <linux/regulator/consumer.h>

MODULE_AUTHOR("yj");
MODULE_DESCRIPTION("A low-level driver for nt99250 sensors");
MODULE_LICENSE("GPL");

static int debug = 0;
module_param(debug, bool, 0644);
MODULE_PARM_DESC(debug, "Debug level (0-1)");

/*
 * Basic window sizes.  These probably belong somewhere more globally
 * useful.
 */
#define VGA_WIDTH	640
#define VGA_HEIGHT	480
#define QVGA_WIDTH	320
#define QVGA_HEIGHT	240
#define CIF_WIDTH	352
#define CIF_HEIGHT	288
#define QCIF_WIDTH	176
#define	QCIF_HEIGHT	144

/*
 * Our nominal (default) frame rate.
 */
#define NT99250_FRAME_RATE 30

#define NT99250_I2C_ADDR 0x6C//0x60

#define RESET 0x00
#define CMATRIX_LEN 6

/* Registers */
#define REG_MIDH	0x3000	/* Manuf. ID high */
#define REG_MIDL	0x3001	/* Manuf. ID low */
#define REG_MVFP	0x3022	/* Mirror / vflip */
//#define REG_BRIGHT	0x5589	/* Brightness */
//#define REG_CONTRAS	0x5588	/* Contrast control */
#define REG_RESET   0X3021
//#define REG_WIDTH_HIGH  0x3804 //set resolution
//#define REG_WIDTH_LOW  0x3805
//#define REG_HIGH_HIGH  0x3806
//#define REG_HIGH_LOW   0x3807
//#define REG_CAMIFCTL	0x4100 //source select
//#define REG_FORMATCTL	0x4300 //output format 

struct regval_list {
	unsigned short reg_num;
	unsigned char value;
};

static int iInitTag = 0;

/* init 640x480 */
static struct regval_list nt99250_init_data[] = {
	{0x3024,0x02}, //TG     
	{0x32F0,0x01},//0:UYVY	2:VYUY	1:YUYV	 3:YVYU
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
	{0x306d, 0x01}, //pclk 
	{0, 0}
};

/* 1600X1200 UXGA */
static struct regval_list nt99250_uxga[] = {
	//[1600X1200]=====================================
	//edge & denoise +++
	{0x3300,0x30},
	{0x3301,0x80},
	{0x3320,0x28},
	{0x3331,0x04},
	{0x3332,0x40},
	{0x3339,0x10},
	{0x333a,0x1a},
	//edge & denoise ---
	{0x32e0,0x06},
	{0x32e1,0x40},
	{0x32e2,0x04},
	{0x32e3,0xb0},
	{0x32e4,0x00},
	{0x32e5,0x00},
	{0x32e6,0x00},
	{0x32e7,0x00},
	{0x301e,0x00},
	{0x301f,0x20},
	{0x3022,0x25},
	{0x3023,0x24},
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
	{0x300c,0x04},
	{0x300d,0xc8},
	{0x300e,0x06},
	{0x300f,0x40},
	{0x3010,0x04},
	{0x3011,0xb0},
	{0x32bb,0x0b},
	{0x32bc,0x30},
	{0x32c1,0x25},
	{0x32c2,0x5c},
	{0x32c8,0x62},
	{0x32c9,0x52},
	{0x32c4,0x00},

	{0x3021,0x06}, 
	{0x3060,0x01},
	{0x0, 0x0}, 
};

/* 1280X960  */
static struct regval_list nt99250_1280_960[] = {
	//[1280X960]================================================
	//edge & denoise +++
	{0x3300,0x30},
	{0x3301,0x80},
	{0x3320,0x28},
	{0x3331,0x04},
	{0x3332,0x40},
	{0x3339,0x10},
	{0x333a,0x1a},
	//edge & denoise ---
	{0x32e0,0x05}, 
	{0x32e1,0x00}, 
	{0x32e2,0x03}, 
	{0x32e3,0xc0}, 
	{0x32e4,0x00}, 
	{0x32e5,0x40}, 
	{0x32e6,0x00}, 
	{0x32e7,0x40}, 
	{0x301e,0x00}, 
	{0x301f,0x20}, 
	{0x3022,0x25}, 
	{0x3023,0x24}, 
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
	{0x300c,0x04}, 
	{0x300d,0xc8}, 
	{0x300e,0x06}, 
	{0x300f,0x40}, 
	{0x3010,0x04}, 
	{0x3011,0xb0}, 
	{0x32bb,0x0b}, 
	{0x32bc,0x30}, 
	{0x32c1,0x25}, 
	{0x32c2,0x5c}, 
	{0x32c8,0x62}, 
	{0x32c9,0x52}, 
	{0x32c4,0x00}, 
	
	{0x3021,0x06}, 
	{0x3060,0x01},
	{0x0, 0x0}, 
};

static struct regval_list nt99250_1024_768[] = {
	//[1024X768]=============================================
	//edge & denoise +++
	{0x3300,0x30},
	{0x3301,0x80},
	{0x3320,0x28},
	{0x3331,0x04},
	{0x3332,0x40},
	{0x3339,0x10},
	{0x333a,0x1a},
	//edge & denoise ---
	{0x32e0,0x04}, 
	{0x32e1,0x00}, 
	{0x32e2,0x03}, 
	{0x32e3,0x00}, 
	{0x32e4,0x00}, 
	{0x32e5,0x90}, 
	{0x32e6,0x00}, 
	{0x32e7,0x90}, 
	{0x301e,0x00}, 
	{0x301f,0x20}, 
	{0x3022,0x25}, 
	{0x3023,0x24}, 
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
	{0x300c,0x04}, 
	{0x300d,0xc8}, 
	{0x300e,0x06}, 
	{0x300f,0x40}, 
	{0x3010,0x04}, 
	{0x3011,0xb0}, 
	{0x32bb,0x0b}, 
	{0x32bc,0x30}, 
	{0x32c1,0x25}, 
	{0x32c2,0x5c}, 
	{0x32c8,0x62}, 
	{0x32c9,0x52}, 
	{0x32c4,0x00}, 
	{0x3021,0x06}, 
	{0x3060,0x01}, 
		{0, 0}
};

static struct regval_list nt99250_vga[] = {
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
		{0, 0}
};
/* 800X600 SVGA*/
static struct regval_list nt99250_svga[] = {
 //[800X600]======================================
 //edge & denoise +++
 {0x3300,0x30},
 {0x3301,0x80},
 {0x3320,0x28},
 {0x3331,0x04},
 {0x3332,0x40},
 {0x3339,0x10},
 {0x333a,0x1a},
 //edge & denoise ---
 {0x32e0,0x03}, 
 {0x32e1,0x20}, 
 {0x32e2,0x02}, 
 {0x32e3,0x58}, 
 {0x32e4,0x01}, 
 {0x32e5,0x00}, 
 {0x32e6,0x00}, 
 {0x32e7,0x00}, 
 {0x301e,0x00}, 
 {0x301f,0x20}, 
 {0x3022,0x25}, 
 {0x3023,0x64}, 
 {0x3002,0x00}, 
 {0x3003,0x02}, 
 {0x3004,0x00}, 
 {0x3005,0x02}, 
 {0x3006,0x06}, 
 {0x3007,0x41}, 
 {0x3008,0x04}, 
 {0x3009,0xb1}, 
 {0x300a,0x09}, 
 {0x300b,0x91}, 
 {0x300c,0x02}, 
 {0x300d,0x8d}, 
 {0x300e,0x06}, 
 {0x300f,0x40}, 
 {0x3010,0x02}, 
 {0x3011,0x58}, 
 {0x32bb,0x0b}, 
 {0x32bc,0x30}, 
 {0x32c1,0x25}, 
 {0x32c2,0x5c}, 
 {0x32c8,0x62}, 
 {0x32c9,0x52}, 
 {0x32c4,0x00}, 
 {0x3021,0x06}, 
 {0x3060,0x01},
  {0, 0}
};

/* 352X288 CIF */
static struct regval_list nt99250_cif[] = {
	//[352X288]=====================================
	//edge & denoise +++
	{0x3300,0x30},
	{0x3301,0x80},
	{0x3320,0x28},
	{0x3331,0x04},
	{0x3332,0x40},
	{0x3339,0x10},
	{0x333a,0x1a},
	//edge & denoise ---
	{0x301e,0x00},
	{0x301f,0x20},
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
	{0x300d,0x8d},
	{0x300e,0x06},
	{0x300f,0x40},
	{0x3010,0x02},
	{0x3011,0x58},
	{0x32bb,0x0b},
	{0x32bc,0x30},
	{0x32c1,0x25},
	{0x32c2,0x5c},
	{0x32c8,0x62},
	{0x32c9,0x52},
	{0x32c4,0x00},

	{0x3021,0x06}, 
	{0x3060,0x01},
		{0, 0}
};

/*
 * Information we maintain about a known sensor.
 */
struct nt99250_format_struct;  /* coming later */
struct nt99250_info {
	struct v4l2_subdev sd;
	struct nt99250_format_struct *fmt;  /* Current format */
	unsigned char sat;		/* Saturation value */
	int hue;			/* Hue value */
};

static inline struct nt99250_info *to_state(struct v4l2_subdev *sd)
{
	return container_of(sd, struct nt99250_info, sd);
}

/*
 * Here we'll try to encapsulate the changes for just the output
 * video format.
 *
 * RGB656 and YUV422 come from OV; RGB444 is homebrewed.
 *
 */

static struct regval_list nt99250_fmt_yuv422[] = {
	
};

static struct regval_list nt99250_fmt_rgb565[] = {
	
};

static struct regval_list nt99250_fmt_rgb444[] = {
	
};

static struct regval_list nt99250_fmt_raw[] = {
	
};

#define COLOR_TEMPERATURE_CLOUDY_DN  6500
#define COLOR_TEMPERATURE_CLOUDY_UP    8000
#define COLOR_TEMPERATURE_CLEARDAY_DN  5000
#define COLOR_TEMPERATURE_CLEARDAY_UP    6500
#define COLOR_TEMPERATURE_OFFICE_DN     3500
#define COLOR_TEMPERATURE_OFFICE_UP     5000
#define COLOR_TEMPERATURE_HOME_DN       2500
#define COLOR_TEMPERATURE_HOME_UP       3500

static  struct regval_list nt99250_WhiteB_Auto[]=
{
    {0x3201, 0x7f},  //AWB auto, bit[1]:0,auto
    {0x0000, 0x00}
};
/* Cloudy Colour Temperature : 6500K - 8000K  */
static  struct regval_list nt99250_WhiteB_Cloudy[]=
{
    {0x3201, 0x6f},
    {0x3290, 0x01},
    {0x3291, 0x48},
    {0x3296, 0x01},
    {0x3297, 0x58},
    
    {0x0000, 0x00}
};
/* ClearDay Colour Temperature : 5000K - 6500K  */
static  struct regval_list nt99250_WhiteB_ClearDay[]=
{
    //Sunny
    {0x3201, 0x6f},
    {0x3290, 0x01},
    {0x3291, 0x38},
    {0x3296, 0x01},
    {0x3297, 0x68},
    
    {0x0000, 0x00}

};
/* Office Colour Temperature : 3500K - 5000K  */
static  struct regval_list nt99250_WhiteB_TungstenLamp1[]=
{
    //Office
    {0x3201, 0x6f},
    {0x3290, 0x01},
    {0x3291, 0x24},
    {0x3296, 0x01},
    {0x3297, 0x78},

    {0x0000, 0x00}

};
/* Home Colour Temperature : 2500K - 3500K  */
static  struct regval_list nt99250_WhiteB_TungstenLamp2[]=
{
    //Home
    {0x3201, 0x6f},
    {0x3290, 0x01},
    {0x3291, 0x30},
    {0x3296, 0x01},
    {0x3297, 0x70},

    {0x0000, 0x00}
};

static  struct regval_list nt99250_Brightness0[]=
{
    // Brightness -2
  
    {0x0000, 0x00}
};

static  struct regval_list nt99250_Brightness1[]=
{
    // Brightness -1
 
    {0x0000, 0x00}
};

static  struct regval_list nt99250_Brightness2[]=
{
    //  Brightness 0
    {0x0000, 0x00}
};

static  struct regval_list nt99250_Brightness3[]=
{
    // Brightness +1
    {0x0000, 0x00}
};

static  struct regval_list nt99250_Brightness4[]=
{
    //  Brightness +2
    {0x0000, 0x00}
};

static  struct regval_list nt99250_Brightness5[]=
{
    //  Brightness +3
    {0x0000, 0x00}
};

static  struct regval_list nt99250_Effect_Normal[] =
{
    {0x32f1, 0x00},
    {0x32f6, 0x0c},
    
    {0x0000, 0x00}
};

static  struct regval_list nt99250_Effect_WandB[] =
{
    {0x32f1, 0x01},
    {0x32f6, 0x0c},
    
    {0x0000, 0x00}
};

static  struct regval_list nt99250_Effect_Sepia[] =
{
    {0x32f1, 0x02},
    {0x32f6, 0x0c},
    
    {0x0000, 0x00}
};

static  struct regval_list nt99250_Effect_Negative[] =
{
    //Negative
    {0x32f1, 0x03},
    {0x32f6, 0x0c},
    
    {0x0000, 0x00}
};
static  struct regval_list nt99250_Effect_Bluish[] =
{
    // Bluish
    {0x32f1, 0x08},
    {0x32f4, 0xf0},
    {0x32f5, 0x80},
    {0x32f6, 0x0c},
    
    {0x0000, 0x00}
};

static  struct regval_list nt99250_Effect_Green[] =
{
    //  Greenish
    {0x32f1, 0x05},
    {0x32f4, 0x60},
    {0x32f5, 0x20},
    {0x32f6, 0x0c},
    
    {0x0000, 0x00}
};
static  struct regval_list nt99250_Exposure0[]=
{
    //-3
    {0x32f2, 0x38},
    {0x0000, 0x00}
};

static  struct regval_list nt99250_Exposure1[]=
{
    //-2
    {0x32f2, 0x50},
    {0x0000, 0x00}
};

static  struct regval_list nt99250_Exposure2[]=
{
    //-0.3EV
    {0x32f2, 0x68},
    {0x0000, 0x00}
};

static  struct regval_list nt99250_Exposure3[]=
{
    //default
    {0x32f2, 0x80},
    {0x0000, 0x00}
};

static  struct regval_list nt99250_Exposure4[]=
{
    // 1
    {0x32f2, 0x98},
    {0x0000, 0x00}
};

static  struct regval_list nt99250_Exposure5[]=
{
    // 2
    {0x32f2, 0xb0},
    {0x0000, 0x00}
};

static  struct regval_list nt99250_Exposure6[]=
{
    // 3
    {0x32f2, 0xc8},
    {0x0000, 0x00}
};

static  struct regval_list nt99250_Saturation0[]=
{
    
    {0x0000, 0x00}
};

static  struct regval_list nt99250_Saturation1[]=
{
    
    {0x0000, 0x00}
};

static  struct regval_list nt99250_Saturation2[]=
{
    
    {0x0000, 0x00}
};


static  struct regval_list nt99250_Contrast0[]=
{
    //Contrast -3
    
    {0x0000, 0x00}
};

static  struct regval_list nt99250_Contrast1[]=
{
    //Contrast -2
    
    {0x0000, 0x00}
};

static  struct regval_list nt99250_Contrast2[]=
{
    // Contrast -1
    
    {0x0000, 0x00}
};

static  struct regval_list nt99250_Contrast3[]=
{
    //Contrast 0
    
    {0x0000, 0x00}
};

static  struct regval_list nt99250_Contrast4[]=
{
    //Contrast +1
    
    {0x0000, 0x00}
};


static  struct regval_list nt99250_Contrast5[]=
{
    //Contrast +2
    
    {0x0000, 0x00}
};

static  struct regval_list nt99250_Contrast6[]=
{
    //Contrast +3
    
    {0x0000, 0x00}
};

static  struct regval_list nt99250_MirrorOn[]=
{
    {0x3022, 0x25},	

    {0x0000, 0x00}
};

static  struct regval_list nt99250_MirrorOff[]=
{
    {0x3022, 0x24},
    	
    {0x0000, 0x00}
};

static  struct regval_list nt99250_FlipOn[]=
{
    {0x3022, 0x27},
    	
    {0x0000, 0x00}
};

static  struct regval_list nt99250_FlipOff[]=
{
    {0x3022, 0x26},
    	
    {0x0000, 0x00}
};

static  struct regval_list nt99250_SceneAuto[] =
{
    {0x32c1, 0x25},
    {0x32c2, 0xa0},
    
    {0x0000, 0x00}
};

static  struct regval_list nt99250_SceneNight[] =
{
    //30fps ~ 5fps night mode for 60/50Hz light environment, 24Mhz clock input,24Mzh pclk
    {0x32c1, 0x29},
    {0x32c2, 0x60},
    
    {0x0000, 0x00}
};


static struct regval_list nt99250_Zoom0[] =
{
    {0x0, 0x0},
};

static struct regval_list nt99250_Zoom1[] =
{
     {0x0, 0x0},
};

static struct regval_list nt99250_Zoom2[] =
{
    {0x0, 0x0},
};


static struct regval_list nt99250_Zoom3[] =
{
    {0x0, 0x0},
};


static struct regval_list *nt99250_ExposureSeqe[] = {nt99250_Exposure0, nt99250_Exposure1, nt99250_Exposure2, nt99250_Exposure3,
    nt99250_Exposure4, nt99250_Exposure5,nt99250_Exposure6,NULL,
};

static struct regval_list *nt99250_EffectSeqe[] = {nt99250_Effect_Normal, nt99250_Effect_WandB, nt99250_Effect_Negative,nt99250_Effect_Sepia,
    nt99250_Effect_Green, nt99250_Effect_Bluish, NULL,
};

static struct regval_list *nt99250_WhiteBalanceSeqe[] = {nt99250_WhiteB_Auto, nt99250_WhiteB_TungstenLamp1,nt99250_WhiteB_TungstenLamp2,
    nt99250_WhiteB_ClearDay, nt99250_WhiteB_Cloudy,NULL,
};

static struct regval_list *nt99250_BrightnessSeqe[] = {nt99250_Brightness0, nt99250_Brightness1, nt99250_Brightness2, nt99250_Brightness3,
    nt99250_Brightness4, nt99250_Brightness5,NULL,
};

static struct regval_list *nt99250_ContrastSeqe[] = {nt99250_Contrast0, nt99250_Contrast1, nt99250_Contrast2, nt99250_Contrast3,
    nt99250_Contrast4, nt99250_Contrast5, nt99250_Contrast6, NULL,
};

static struct regval_list *nt99250_SaturationSeqe[] = {nt99250_Saturation0, nt99250_Saturation1, nt99250_Saturation2, NULL,};

static struct regval_list *nt99250_MirrorSeqe[] = {nt99250_MirrorOff, nt99250_MirrorOn,NULL,};

static struct regval_list *nt99250_FlipSeqe[] = {nt99250_FlipOff, nt99250_FlipOn,NULL,};

static struct regval_list *nt99250_SceneSeqe[] = {nt99250_SceneAuto, nt99250_SceneNight,NULL,};

static struct regval_list *nt99250_ZoomSeqe[] = {nt99250_Zoom0, nt99250_Zoom1, nt99250_Zoom2, nt99250_Zoom3, NULL,};


/*
 * Low-level register I/O.
 */

static int nt99250_read(struct v4l2_subdev *sd, unsigned short reg,
		unsigned char *value)
{
	struct i2c_client *client = v4l2_get_subdevdata(sd);

	int err = -1;
	u8 buf[2] = {0};
	u8 ucData[2] = {0};
	int ret = -1;
	int iCnt = 2;
	
	
	buf[0] = reg >> 8; 
	buf[1] = reg & 0xFF;
	
	struct i2c_msg msg[2] = { 
				  { .addr = client->addr,
				    .flags = 0 /*| I2C_M_IGNORE_NAK*/,
				    .buf = buf, .len = 2 },//write reg addr msg,write two byte reg addr.
				  { .addr = client->addr,
				    .flags = I2C_M_RD /*| I2C_M_IGNORE_NAK*/,
				    .buf = ucData, .len = 1 } //read msg ,if read two byte ,then len=2
				  };

    while ((iCnt--) && (err < 0)) 
    {                       
        err = i2c_transfer(client->adapter, msg, 2);
        if (err  >= 0) {
            *value = ucData[0];
            return 0;
        } else {
            udelay(100);
            printk("i2c read error %d\n", err);
        }
    }
	return 0;

}

static int nt99250_write(struct v4l2_subdev *sd, unsigned short reg,
		unsigned char value)
{
	struct i2c_client *client = v4l2_get_subdevdata(sd);
	int err = -1;
	int iCnt = 2;
	u8 buf[3] = {0};
	struct i2c_msg msg[1] = {{0}};

	while( (iCnt-- > 0) && err < 0)
	{
		buf[0] = reg >> 8; 
		buf[1] = reg & 0xFF;
		buf[2] = value;
		msg->addr = client->addr;
		msg->flags = 0 /*| I2C_M_IGNORE_NAK*/;
		msg->buf = buf;
		msg->len = sizeof(buf);
		err = i2c_transfer(client->adapter, msg, 1);
		if(err >= 0)
		{
			break;
		}else
		{
			printk("send error %d\n", err);
			udelay(10);
		}
	}
	
	return err;
}

/*
 * Write a list of register settings; ff/ff stops the process.
 */
static int nt99250_write_array(struct v4l2_subdev *sd, struct regval_list *vals)
{
	while (vals->reg_num != 0 ) {
		int ret = nt99250_write(sd, vals->reg_num, vals->value);
		if (ret < 0)
			return ret;
		vals++;
	}
	printk("write success \n");
	return 0;
}

/*
 * reset the sensor.
 */
static int nt99250_reset(struct v4l2_subdev *sd, u32 val)
{
	nt99250_write(sd, REG_RESET, 0x00);
	printk("standby \n");
	return 0;
}

static int nt99250_init(struct v4l2_subdev *sd, u32 val)
{
	int iRet = 0;
	printk("%s \n", __func__);
	iRet = nt99250_write_array(sd, nt99250_init_data);
	nt99250_write_array(sd,nt99250_vga );
	return iRet;
}

static int nt99250_detect(struct v4l2_subdev *sd)
{
	unsigned char v = 0;
	int ret;
	unsigned short pid = 0;
	ret = nt99250_read(sd, REG_MIDH, &v);
	printk("var high 0x%x\n", v);
	if (ret < 0)
	{
		printk("ov read error \n");
		return ret;
	}
	pid |= (v << 8);
	v = 0;
	ret = nt99250_read(sd, REG_MIDL, &v);
	printk("var low 0x%x\n", v);
	if (ret < 0)
		return ret;
	pid |= (v & 0xff);
	
	if (pid != 0x2501 && pid != 0x2500)
    {
        printk("error: devicr mismatched \n");
        ret = -ENODEV;
		return -1;
    }

	ret = nt99250_init(sd, 0);
	if (ret < 0)
	{
		printk("nt99250 init failed \n");
		return ret;
	}
	return 0;
}

/*
 * Store information about the video data format.  The color matrix
 * is deeply tied into the format, so keep the relevant values here.
 * The magic matrix nubmers come from OmniVision.
 */
static struct nt99250_format_struct {
	__u8 *desc;
	__u32 pixelformat;
	struct regval_list *regs;
	int cmatrix[CMATRIX_LEN];
	int bpp;   /* Bytes per pixel */
} nt99250_formats[] = {
	{
		.desc		= "YUYV 4:2:2",
		.pixelformat	= V4L2_PIX_FMT_YUYV,
		.regs 		= nt99250_fmt_yuv422,
		.cmatrix	= { 128, -128, 0, -34, -94, 128 },
		.bpp		= 2,
	},
	{
		.desc		= "RGB 444",
		.pixelformat	= V4L2_PIX_FMT_RGB444,
		.regs		= nt99250_fmt_rgb444,
		.cmatrix	= { 179, -179, 0, -61, -176, 228 },
		.bpp		= 2,
	},
	{
		.desc		= "RGB 565",
		.pixelformat	= V4L2_PIX_FMT_RGB565,
		.regs		= nt99250_fmt_rgb565,
		.cmatrix	= { 179, -179, 0, -61, -176, 228 },
		.bpp		= 2,
	},
	{
		.desc		= "Raw RGB Bayer",
		.pixelformat	= V4L2_PIX_FMT_SBGGR8,
		.regs 		= nt99250_fmt_raw,
		.cmatrix	= { 0, 0, 0, 0, 0, 0 },
		.bpp		= 1
	},
};
#define N_NT99250_FMTS ARRAY_SIZE(nt99250_formats)

static struct nt99250_win_size {
	int	width;
	int	height;
	int	hstart;		/* Start/stop values for the camera.  Note */
	int	hstop;		/* that they do not always make complete */
	int	vstart;		/* sense to humans, but evidently the sensor */
	int	vstop;		/* will do the right thing... */
	struct regval_list *regs; /* Regs to tweak */
/* h/vref stuff */
} nt99250_win_sizes[] = {
	/* VGA */
	{
		.width		= VGA_WIDTH,
		.height		= VGA_HEIGHT,
		.hstart		= 158,		/* These values from */
		.hstop		=  14,		/* Omnivision */
		.vstart		=  10,
		.vstop		= 490,
		.regs 		= NULL,
	},
	/* CIF */
	{
		.width		= CIF_WIDTH,
		.height		= CIF_HEIGHT,
		.hstart		= 170,		/* Empirically determined */
		.hstop		=  90,
		.vstart		=  14,
		.vstop		= 494,
		.regs 		= NULL,
	},
	/* QVGA */
	{
		.width		= QVGA_WIDTH,
		.height		= QVGA_HEIGHT,
		.hstart		= 164,		/* Empirically determined */
		.hstop		=  20,
		.vstart		=  14,
		.vstop		= 494,
		.regs 		= NULL,
	},
	/* QCIF */
	{
		.width		= QCIF_WIDTH,
		.height		= QCIF_HEIGHT,
		.hstart		= 456,		/* Empirically determined */
		.hstop		=  24,
		.vstart		=  14,
		.vstop		= 494,
		.regs 		= NULL,
	},
};

#define N_WIN_SIZES (ARRAY_SIZE(nt99250_win_sizes))

/*
 * Store a set of start/stop values into the camera.
 */
static int nt99250_set_hw(struct v4l2_subdev *sd, int hstart, int hstop,
		int vstart, int vstop)
{
	int ret;
	return 0;
}

static int nt99250_enum_fmt(struct v4l2_subdev *sd, struct v4l2_fmtdesc *fmt)
{
	struct nt99250_format_struct *ofmt;

	if (fmt->index >= N_NT99250_FMTS)
		return -EINVAL;

	ofmt = nt99250_formats + fmt->index;
	fmt->flags = 0;
	strcpy(fmt->description, ofmt->desc);
	fmt->pixelformat = ofmt->pixelformat;
	return 0;
}

static int nt99250_try_fmt_internal(struct v4l2_subdev *sd,
		struct v4l2_format *fmt,
		struct nt99250_format_struct **ret_fmt,
		struct nt99250_win_size **ret_wsize)
{
	int index;
	struct nt99250_win_size *wsize;
	struct v4l2_pix_format *pix = &fmt->fmt.pix;

	for (index = 0; index < N_NT99250_FMTS; index++)
		if (nt99250_formats[index].pixelformat == pix->pixelformat)
			break;
	if (index >= N_NT99250_FMTS) {
		/* default to first format */
		index = 0;
		pix->pixelformat = nt99250_formats[0].pixelformat;
	}
	if (ret_fmt != NULL)
		*ret_fmt = nt99250_formats + index;
	/*
	 * Fields: the OV devices claim to be progressive.
	 */
	pix->field = V4L2_FIELD_NONE;
	/*
	 * Round requested image size down to the nearest
	 * we support, but not below the smallest.
	 */
	for (wsize = nt99250_win_sizes; wsize < nt99250_win_sizes + N_WIN_SIZES;
	     wsize++)
		if (pix->width >= wsize->width && pix->height >= wsize->height)
			break;
	if (wsize >= nt99250_win_sizes + N_WIN_SIZES)
		wsize--;   /* Take the smallest one */
	if (ret_wsize != NULL)
		*ret_wsize = wsize;
	/*
	 * Note the size we'll actually handle.
	 */
	pix->width = wsize->width;
	pix->height = wsize->height;
	pix->bytesperline = pix->width*nt99250_formats[index].bpp;
	pix->sizeimage = pix->height*pix->bytesperline;
	
	return 0;
}

static int nt99250_try_fmt(struct v4l2_subdev *sd, struct v4l2_format *fmt)
{
	return nt99250_try_fmt_internal(sd, fmt, NULL, NULL);
}

/*
 * Set a format.
 */
static int nt99250_s_fmt(struct v4l2_subdev *sd, struct v4l2_format *fmt)
{
	int ret;
	int iPreview = 0;
	unsigned short	AE_reg,REG_SCALER, AGC_reg;	
	unsigned char	temp_reg12,temp_reg13;	
	unsigned short  shutter;		

	printk("%s called  %d,%d\n", __FUNCTION__  ,fmt->fmt.pix.width,fmt->fmt.pix.height);
	if(1 == iInitTag)
	{
		nt99250_init(sd, 1);
	}
	// turn on SCALER	for preview
	nt99250_read(sd ,0x3201, &REG_SCALER);	
	nt99250_write(sd, 0x3201, (REG_SCALER|0x40) );	
	/*
	// turn on AE	for preview
	nt99250_read(sd ,0x3201, &AE_reg);	
	nt99250_write(sd, 0x3201, (AE_reg|0x20) );	
	// turn on AGC   for preview
	nt99250_read(sd ,0x32bb, &AGC_reg);	
	nt99250_write(sd, 0x32bb, (AGC_reg|0x01) );*/
	
#if 1
	if( 1600 == fmt->fmt.pix.width && 1200 == fmt->fmt.pix.height)
	{
		nt99250_write_array(sd,nt99250_uxga );
		// turn off  SCALER	 for capture
		nt99250_read(sd ,0x3201, &REG_SCALER);	
		nt99250_write(sd, 0x3201, (REG_SCALER|0x40) );	
		printk("write uxga \n");
	}
	else if( 1280 == fmt->fmt.pix.width && (960 == fmt->fmt.pix.height
		 || 720 == fmt->fmt.pix.height))
		{		
		nt99250_write_array(sd,nt99250_1280_960 );
		printk("write 1280 x 960 \n");
		}	
	else if( 1024 == fmt->fmt.pix.width && 768 == fmt->fmt.pix.height)
		{
		nt99250_write_array(sd,nt99250_1024_768 );
		printk("write 1024 x 768 \n");
		}
	else if( 800 == fmt->fmt.pix.width && 600 == fmt->fmt.pix.height)
	{
		nt99250_write_array(sd,nt99250_svga );
		printk("write svga \n");
	}else if( 640 == fmt->fmt.pix.width && 480 == fmt->fmt.pix.height)
	{
		nt99250_write_array(sd,nt99250_vga );
		iPreview = 1;
		printk("write vga \n");
	}else if( 352 == fmt->fmt.pix.width && 288 == fmt->fmt.pix.height)
	{
		nt99250_write_array(sd,nt99250_cif );
		printk("write cif \n");
	}
#endif	
#if 0
	if(1 == iPreview)
	{
		// turn on AE	for preview
		nt99250_read(sd ,0x3201, &AE_reg);	
		nt99250_write(sd, 0x3201, (AE_reg|0x20) );	
		// turn on AGC	 for preview
		nt99250_read(sd ,0x32bb, &AGC_reg); 
		nt99250_write(sd, 0x32bb, (AGC_reg|0x01) );
	}
	else			
	{	
		
		nt99250_read(sd, 0x3012, &temp_reg12); 
		nt99250_read(sd, 0x3013, &temp_reg13); 
		shutter = (temp_reg13 & 0x00FF) | (temp_reg12 << 8); 

		// turn off AE 
		nt99250_read(sd ,0x3201, &AE_reg); 
		nt99250_write(sd, 0x3201, (AE_reg&(~0x20)) ); 
		// turn off AGC 
		nt99250_read(sd ,0x32bb, &AGC_reg); 
		nt99250_write(sd, 0x32bb, (AGC_reg&(~0x01)) );
		if ((fmt->fmt.pix.width <= 640) && (fmt->fmt.pix.height <= 480))   
		{  
			shutter = shutter*1984/1024; 
		}  
		else if ((fmt->fmt.pix.width == 800) && (fmt->fmt.pix.height == 600))   
		{  
			shutter = shutter*1984/1184; 
		}  
		else if ((fmt->fmt.pix.width == 1280) && (fmt->fmt.pix.height == 1024)) 
		{  
			shutter = shutter*1984/1664; 
		}  
		else if ((fmt->fmt.pix.width == 1280) && (fmt->fmt.pix.height == 960)) 
		{  
			shutter = shutter*1984/1664; 
		}  
		else if ((fmt->fmt.pix.width == 1600) && (fmt->fmt.pix.height == 1200)) 
		{  
			shutter = shutter*1984/1984; 
		}  
		if (shutter < 1) 
		{  
			shutter = 1; 
		} 
		nt99250_write(sd, 0x3012, sizeof((shutter >> 8) & 0xff) ); 
		nt99250_write(sd, 0x3013, sizeof(shutter & 0xFF) );
		
		printk("%s called 0x3012 \n", __FUNCTION__);
	}
	#endif
	mdelay(250);

	return 0;
}

/*
 * Implement G/S_PARM.  There is a "high quality" mode we could try
 * to do someday; for now, we just do the frame rate tweak.
 */
static int nt99250_g_parm(struct v4l2_subdev *sd, struct v4l2_streamparm *parms)
{
	struct v4l2_captureparm *cp = &parms->parm.capture;
	unsigned char clkrc;
	int ret;

	if (parms->type != V4L2_BUF_TYPE_VIDEO_CAPTURE)
		return -EINVAL;
	//ret = nt99250_read(sd, REG_CLKRC, &clkrc);
	//if (ret < 0)
	//	return ret;
	memset(cp, 0, sizeof(struct v4l2_captureparm));
	cp->capability = V4L2_CAP_TIMEPERFRAME;
	cp->timeperframe.numerator = 1;
	cp->timeperframe.denominator = NT99250_FRAME_RATE;
	return 0;
}

static int nt99250_s_parm(struct v4l2_subdev *sd, struct v4l2_streamparm *parms)
{
	struct v4l2_captureparm *cp = &parms->parm.capture;
	struct v4l2_fract *tpf = &cp->timeperframe;
	unsigned char clkrc;
	int ret, div;
	//printk("%s , %d\n", __FUNCTION__, __LINE__);
	if (parms->type != V4L2_BUF_TYPE_VIDEO_CAPTURE)
		return -EINVAL;
	
	return 0;
}

static int nt99250_s_sat(struct v4l2_subdev *sd, int value)
{
	struct nt99250_info *info = to_state(sd);
	printk("%s, %d \n", __FUNCTION__, value);
	if(value < 0 || value >= sizeof(nt99250_SaturationSeqe)/ sizeof(struct regval_list *))
	{
		return -1;
	}
	nt99250_write_array( sd, nt99250_SaturationSeqe[value]);
	info->sat = value;
	
	return 0;
}

static int nt99250_g_sat(struct v4l2_subdev *sd, __s32 *value)
{
	struct nt99250_info *info = to_state(sd);

	*value = info->sat;
	return 0;
}

/*
 * Some weird registers seem to store values in a sign/magnitude format!
 */
static unsigned char nt99250_sm_to_abs(unsigned char v)
{
	if ((v & 0x80) == 0)
		return v + 128;
	return 128 - (v & 0x7f);
}

static unsigned char nt99250_abs_to_sm(unsigned char v)
{
	if (v > 127)
		return v & 0x7f;
	return (128 - v) | 0x80;
}

static int nt99250_s_brightness(struct v4l2_subdev *sd, int value)
{	
#if 0
	printk("%s, %d \n", __FUNCTION__, value);

	if(value < 0 || value >= sizeof(nt99250_BrightnessSeqe)/ sizeof(struct regval_list *))
	{
		return -1;
	}

	nt99250_write_array( sd, nt99250_BrightnessSeqe[value]);
#endif	
	return 0;
}

static int nt99250_g_brightness(struct v4l2_subdev *sd, __s32 *value)
{
	return 0;
}

static int nt99250_s_contrast(struct v4l2_subdev *sd, int value)
{
	printk("%s, %d \n", __FUNCTION__, value);
	if(value < 0 || value >= sizeof(nt99250_ContrastSeqe)/ sizeof(struct regval_list *))
	{
		return -1;
	}

	nt99250_write_array( sd, nt99250_ContrastSeqe[value]);
}

static int nt99250_g_contrast(struct v4l2_subdev *sd, __s32 *value)
{
	unsigned char v = 0;
	*value = v;
	return 0;
}

static int nt99250_g_hflip(struct v4l2_subdev *sd, __s32 *value)
{
	int ret;
	unsigned char v = 0;

	ret = nt99250_read(sd, REG_MVFP, &v);
	*value = v;
	return ret;
}

static int nt99250_s_hflip(struct v4l2_subdev *sd, int value)
{
	printk("%s, %d \n", __FUNCTION__, value);
	if(value < 0 || value >= sizeof(nt99250_MirrorSeqe)/ sizeof(struct regval_list *))
	{
		return -1;
	}
	return nt99250_write_array( sd, nt99250_MirrorSeqe[value]);
}

static int nt99250_g_vflip(struct v4l2_subdev *sd, __s32 *value)
{
	int ret;
	unsigned char v = 0;
	*value = v ;
	return ret;
}

static int nt99250_s_vflip(struct v4l2_subdev *sd, int value)
{
	printk("%s, %d \n", __FUNCTION__, value);
	if(value < 0 || value >= sizeof(nt99250_FlipSeqe)/ sizeof(struct regval_list *))
	{
		return -1;
	}
	return nt99250_write_array( sd, nt99250_FlipSeqe[value]);
}

static int nt99250_s_white_balance(struct v4l2_subdev *sd, int value)
{
	printk("%s, %d \n", __FUNCTION__, value);
	if(value < 0 || value >= sizeof(nt99250_WhiteBalanceSeqe)/ sizeof(struct regval_list *))
	{
		return -1;
	}
	return nt99250_write_array( sd, nt99250_WhiteBalanceSeqe[value ]);
}

static int nt99250_s_exposure(struct v4l2_subdev *sd, int value)
{
	printk("%s, %d \n", __FUNCTION__, value);
	if(value < 0 || value >= sizeof(nt99250_ExposureSeqe)/ sizeof(struct regval_list *))
	{
		return -1;
	}
	return nt99250_write_array( sd, nt99250_ExposureSeqe[value]);
}

static int nt99250_s_zoom(struct v4l2_subdev *sd, int value)
{
	printk("%s, %d \n", __FUNCTION__, value);
	if(value < 0 || value >= sizeof(nt99250_ZoomSeqe)/ sizeof(struct regval_list *))
	{
		return -1;
	}
	return nt99250_write_array( sd, nt99250_ZoomSeqe[value]);
}

static int nt99250_s_effect(struct v4l2_subdev *sd, int value)
{
	printk("%s, %d \n", __FUNCTION__, value);
	if(value < 0 || value >= sizeof(nt99250_EffectSeqe)/ sizeof(struct regval_list *))
	{
		return -1;
	}
	return nt99250_write_array( sd, nt99250_EffectSeqe[value]);
}

static int nt99250_s_scene(struct v4l2_subdev *sd, int value)
{
	printk("%s, %d \n", __FUNCTION__, value);
	if(value < 0 || value >= sizeof(nt99250_SceneSeqe)/ sizeof(struct regval_list *))
	{
		return -1;
	}
	return nt99250_write_array( sd, nt99250_SceneSeqe[value]);
}


static int nt99250_queryctrl(struct v4l2_subdev *sd,
		struct v4l2_queryctrl *qc)
{
	printk("%s \n", __FUNCTION__);
	/* Fill in min, max, step and default value for these controls. */
	switch (qc->id) {
	case V4L2_CID_BRIGHTNESS:
		return v4l2_ctrl_query_fill(qc, -3, 2, 1, 0);
	case V4L2_CID_CONTRAST:
		return v4l2_ctrl_query_fill(qc, -3, 3, 1, 0);
	case V4L2_CID_VFLIP:
	case V4L2_CID_HFLIP:
		return v4l2_ctrl_query_fill(qc, 0, 1, 1, 1);
	case V4L2_CID_SATURATION:
		return v4l2_ctrl_query_fill(qc, 0, 2, 1, 0);
	case V4L2_CID_DO_WHITE_BALANCE:
		return v4l2_ctrl_query_fill(qc, 0, 4, 1, 0);
	case V4L2_CID_EXPOSURE:
		return v4l2_ctrl_query_fill(qc, 0, 6, 1, 0);
	case V4L2_CID_ZOOM_RELATIVE:
		return v4l2_ctrl_query_fill(qc, -1, 1, 1, 0);
	case V4L2_CID_ZOOM_ABSOLUTE:
		return v4l2_ctrl_query_fill(qc, 0, 3, 1, 0);
	case V4L2_CID_FOCUS_RELATIVE:
		return v4l2_ctrl_query_fill(qc, -1, 1, 1, 0);
	case V4L2_CID_FOCUS_ABSOLUTE:
		return v4l2_ctrl_query_fill(qc, 0, 255, 1, 125);
	case V4L2_CID_SCENE:
		return v4l2_ctrl_query_fill(qc, 0, 1, 1, 0);
	case V4L2_CID_FLASH:
		return v4l2_ctrl_query_fill(qc, 0, 3, 1, 0);
	case V4L2_CID_EFFECT:
		return v4l2_ctrl_query_fill(qc, 0, 5, 1, 0);
	}
	return -EINVAL;
}

static int nt99250_g_ctrl(struct v4l2_subdev *sd, struct v4l2_control *ctrl)
{
	switch (ctrl->id) {
	case V4L2_CID_BRIGHTNESS:
		return nt99250_g_brightness(sd, &ctrl->value);
	case V4L2_CID_CONTRAST:
		return nt99250_g_contrast(sd, &ctrl->value);
	case V4L2_CID_SATURATION:
		return nt99250_g_sat(sd, &ctrl->value);
	case V4L2_CID_VFLIP:
		return nt99250_g_vflip(sd, &ctrl->value);
	case V4L2_CID_HFLIP:
		return nt99250_g_hflip(sd, &ctrl->value);
	}
	return -EINVAL;
}

static int nt99250_s_ctrl(struct v4l2_subdev *sd, struct v4l2_control *ctrl)
{
	switch (ctrl->id) {
	case V4L2_CID_BRIGHTNESS:
		return nt99250_s_brightness(sd, ctrl->value);
	case V4L2_CID_CONTRAST:
		return nt99250_s_contrast(sd, ctrl->value);
	case V4L2_CID_SATURATION:
		return nt99250_s_sat(sd, ctrl->value);
	case V4L2_CID_VFLIP:
		return nt99250_s_vflip(sd, ctrl->value);
	case V4L2_CID_HFLIP:
		return nt99250_s_hflip(sd, ctrl->value);
	case V4L2_CID_AUTO_WHITE_BALANCE:
	case V4L2_CID_DO_WHITE_BALANCE:
		return nt99250_s_white_balance(sd, ctrl->value);
	case V4L2_CID_EXPOSURE:
		return nt99250_s_exposure(sd, ctrl->value);
	case V4L2_CID_COLORFX://V4L2_CID_EFFECT:
			return nt99250_s_effect(sd, ctrl->value);
	}
	return -EINVAL;
}

static int nt99250_s_ext_ctrl(struct v4l2_subdev *sd, struct v4l2_control *ctrl)
{
	switch (ctrl->id) {
		case V4L2_CID_ZOOM_RELATIVE:
		return nt99250_s_zoom(sd, ctrl->value);
		case V4L2_CID_ZOOM_ABSOLUTE:
			return 0;
		case V4L2_CID_FOCUS_RELATIVE:
			return 0;
		case V4L2_CID_FOCUS_ABSOLUTE:
			return 0;
		case V4L2_CID_SCENE:
			return nt99250_s_scene(sd, ctrl->value);
		case V4L2_CID_FLASH:
			return 0;
	}
	return -EINVAL;
}

static int nt99250_g_chip_ident(struct v4l2_subdev *sd,
		struct v4l2_dbg_chip_ident *chip)
{
	struct i2c_client *client = v4l2_get_subdevdata(sd);

	return v4l2_chip_ident_i2c_client(client, chip, V4L2_IDENT_NT99250, 0);
}

#ifdef CONFIG_VIDEO_ADV_DEBUG
static int nt99250_g_register(struct v4l2_subdev *sd, struct v4l2_dbg_register *reg)
{
	struct i2c_client *client = v4l2_get_subdevdata(sd);
	unsigned char val = 0;
	int ret;

	if (!v4l2_chip_match_i2c_client(client, &reg->match))
		return -EINVAL;
	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;
	ret = nt99250_read(sd, reg->reg & 0xff, &val);
	reg->val = val;
	reg->size = 1;
	return ret;
}

static int nt99250_s_register(struct v4l2_subdev *sd, struct v4l2_dbg_register *reg)
{
	struct i2c_client *client = v4l2_get_subdevdata(sd);

	if (!v4l2_chip_match_i2c_client(client, &reg->match))
		return -EINVAL;
	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;
	nt99250_write(sd, reg->reg & 0xff, reg->val & 0xff);
	return 0;
}
#endif

/* ----------------------------------------------------------------------- */

static const struct v4l2_subdev_core_ops nt99250_core_ops = {
	.g_chip_ident = nt99250_g_chip_ident,
	.g_ctrl = nt99250_g_ctrl,
	.s_ctrl = nt99250_s_ctrl,
	.s_ext_ctrls = nt99250_s_ext_ctrl,
	.queryctrl = nt99250_queryctrl,
	.reset = nt99250_reset,
	.init = nt99250_init,
#ifdef CONFIG_VIDEO_ADV_DEBUG
	.g_register = nt99250_g_register,
	.s_register = nt99250_s_register,
#endif
};

static const struct v4l2_subdev_video_ops nt99250_video_ops = {
	.enum_fmt = nt99250_enum_fmt,
	.try_fmt = nt99250_try_fmt,
	.s_fmt = nt99250_s_fmt,
	.s_parm = nt99250_s_parm,
	.g_parm = nt99250_g_parm,
};

static const struct v4l2_subdev_ops nt99250_ops = {
	.core = &nt99250_core_ops,
	.video = &nt99250_video_ops,
};

/* ----------------------------------------------------------------------- */
static struct regulator *cam_regulator;
static int nt99250_probe(struct i2c_client *client,
			const struct i2c_device_id *id)
{
	struct v4l2_subdev *sd;
	struct nt99250_info *info;
	int ret;
	unsigned char ucVal = 0;

	info = kzalloc(sizeof(struct nt99250_info), GFP_KERNEL);
	if (info == NULL)
		return -ENOMEM;
	sd = &info->sd;
	v4l2_i2c_subdev_init(sd, client, &nt99250_ops);
	
	nt99250_write(sd, REG_RESET, 0x00);
	ret = nt99250_detect(sd);
	
	if (ret) {
		v4l_dbg(1, debug, client,
			"chip found @ 0x%x (%s) is not an nt99250 chip.\n",
			client->addr << 1, client->adapter->name);
		printk("detect failed \n");
		kfree(info);
		return ret;
	}

	printk("chip found @ 0x%02x (%s)\n",
			client->addr << 1, client->adapter->name);

	info->fmt = &nt99250_formats[0];
	info->sat = 128;	/* Review this */

	return 0;
}


static int nt99250_remove(struct i2c_client *client)
{
	struct v4l2_subdev *sd = i2c_get_clientdata(client);
	v4l2_device_unregister_subdev(sd);
	kfree(to_state(sd));
	return 0;
}

static int nt99250_suspend(struct i2c_client *client, pm_message_t state)
{
	iInitTag = 0;
	return 0;
}

static int nt99250_resume(struct i2c_client *client)
{
	struct v4l2_subdev *sd = i2c_get_clientdata(client);
	iInitTag = 1;
	return 0;
}

static const struct i2c_device_id nt99250_id[] = {
	{ "nt99250", 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, nt99250_id);

static struct v4l2_i2c_driver_data v4l2_i2c_data = {
	.name = "nt99250",
	.probe = nt99250_probe,
	.remove = nt99250_remove,
	.suspend = nt99250_suspend,
	.resume = nt99250_resume,
	.id_table = nt99250_id,
};
