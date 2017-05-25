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
	VGA,	/* 640*480 */
#endif
	SVGA,	/* 800*600 */
#if 0
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
	{0xFCFC,0xD000},
	{0x0028,0xD000},
	{0x002A,0x1030},
	{0x0F12,0x0000},                 
	{0x002A,0x0014},
	{0x0F12,0x0001},//p100 //d1 //Delay // Start of Trap and Patch    1103 TnP 
	{0xffff, 0xffff},
};
 
 static unsigned short s5k5ba_init_reg[][2]={
{0xFCFC,0xD000},                                                     
{0x0028,0x7000},
{0x002A,0x1668},
{0x0F12,0xB5FE},    // 70001668 
{0x0F12,0x0007},    // 7000166A 
{0x0F12,0x683C},    // 7000166C 
{0x0F12,0x687E},    // 7000166E 
{0x0F12,0x1DA5},    // 70001670 
{0x0F12,0x88A0},    // 70001672 
{0x0F12,0x2800},    // 70001674 
{0x0F12,0xD00B},    // 70001676 
{0x0F12,0x88A8},    // 70001678 
{0x0F12,0x2800},    // 7000167A 
{0x0F12,0xD008},    // 7000167C 
{0x0F12,0x8820},    // 7000167E 
{0x0F12,0x8829},    // 70001680 
{0x0F12,0x4288},    // 70001682 
{0x0F12,0xD301},    // 70001684 
{0x0F12,0x1A40},    // 70001686 
{0x0F12,0xE000},    // 70001688 
{0x0F12,0x1A08},    // 7000168A 
{0x0F12,0x9001},    // 7000168C 
{0x0F12,0xE001},    // 7000168E 
{0x0F12,0x2019},    // 70001690 
{0x0F12,0x9001},    // 70001692 
{0x0F12,0x4916},    // 70001694 
{0x0F12,0x466B},    // 70001696 
{0x0F12,0x8A48},    // 70001698 
{0x0F12,0x8118},    // 7000169A 
{0x0F12,0x8A88},    // 7000169C 
{0x0F12,0x8158},    // 7000169E 
{0x0F12,0x4814},    // 700016A0 
{0x0F12,0x8940},    // 700016A2 
{0x0F12,0x0040},    // 700016A4 
{0x0F12,0x2103},    // 700016A6 
{0x0F12,0xF000},    // 700016A8 
{0x0F12,0xF826},    // 700016AA 
{0x0F12,0x88A1},    // 700016AC 
{0x0F12,0x4288},    // 700016AE 
{0x0F12,0xD908},    // 700016B0 
{0x0F12,0x8828},    // 700016B2 
{0x0F12,0x8030},    // 700016B4 
{0x0F12,0x8868},    // 700016B6 
{0x0F12,0x8070},    // 700016B8 
{0x0F12,0x88A8},    // 700016BA 
{0x0F12,0x6038},    // 700016BC 
{0x0F12,0xBCFE},    // 700016BE 
{0x0F12,0xBC08},    // 700016C0 
{0x0F12,0x4718},    // 700016C2 
{0x0F12,0x88A9},    // 700016C4 
{0x0F12,0x4288},    // 700016C6 
{0x0F12,0xD906},    // 700016C8 
{0x0F12,0x8820},    // 700016CA 
{0x0F12,0x8030},    // 700016CC 
{0x0F12,0x8860},    // 700016CE 
{0x0F12,0x8070},    // 700016D0 
{0x0F12,0x88A0},    // 700016D2 
{0x0F12,0x6038},    // 700016D4 
{0x0F12,0xE7F2},    // 700016D6 
{0x0F12,0x9801},    // 700016D8 
{0x0F12,0xA902},    // 700016DA 
{0x0F12,0xF000},    // 700016DC 
{0x0F12,0xF812},    // 700016DE 
{0x0F12,0x0033},    // 700016E0 
{0x0F12,0x0029},    // 700016E2 
{0x0F12,0x9A02},    // 700016E4 
{0x0F12,0x0020},    // 700016E6 
{0x0F12,0xF000},    // 700016E8 
{0x0F12,0xF814},    // 700016EA 
{0x0F12,0x6038},    // 700016EC 
{0x0F12,0xE7E6},    // 700016EE 
{0x0F12,0x1A28},    // 700016F0 
{0x0F12,0x7000},    // 700016F2 
{0x0F12,0x0D64},    // 700016F4 
{0x0F12,0x7000},    // 700016F6 
{0x0F12,0x4778},    // 700016F8 
{0x0F12,0x46C0},    // 700016FA 
{0x0F12,0xF004},    // 700016FC 
{0x0F12,0xE51F},    // 700016FE 
{0x0F12,0xA464},    // 70001700 
{0x0F12,0x0000},    // 70001702 
{0x0F12,0x4778},    // 70001704 
{0x0F12,0x46C0},    // 70001706 
{0x0F12,0xC000},    // 70001708 
{0x0F12,0xE59F},    // 7000170A 
{0x0F12,0xFF1C},    // 7000170C 
{0x0F12,0xE12F},    // 7000170E 
{0x0F12,0x6009},    // 70001710 
{0x0F12,0x0000},    // 70001712 
{0x0F12,0x4778},    // 70001714 
{0x0F12,0x46C0},    // 70001716 
{0x0F12,0xC000},    // 70001718 
{0x0F12,0xE59F},    // 7000171A 
{0x0F12,0xFF1C},    // 7000171C 
{0x0F12,0xE12F},    // 7000171E 
{0x0F12,0x622F},    // 70001720 
{0x0F12,0x0000},    // 70001722 
{0x002A,0x2080},
{0x0F12,0xB510},    // 70002080 
{0x0F12,0xF000},    // 70002082 
{0x0F12,0xF8F4},    // 70002084 
{0x0F12,0xBC10},    // 70002086 
{0x0F12,0xBC08},    // 70002088 
{0x0F12,0x4718},    // 7000208A 
{0x0F12,0xB5F0},    // 7000208C 
{0x0F12,0xB08B},    // 7000208E 
{0x0F12,0x0006},    // 70002090 
{0x0F12,0x2000},    // 70002092 
{0x0F12,0x9004},    // 70002094 
{0x0F12,0x6835},    // 70002096 
{0x0F12,0x6874},    // 70002098 
{0x0F12,0x68B0},    // 7000209A 
{0x0F12,0x900A},    // 7000209C 
{0x0F12,0x68F0},    // 7000209E 
{0x0F12,0x9009},    // 700020A0 
{0x0F12,0x4F7D},    // 700020A2 
{0x0F12,0x8979},    // 700020A4 
{0x0F12,0x084A},    // 700020A6 
{0x0F12,0x88A8},    // 700020A8 
{0x0F12,0x88A3},    // 700020AA 
{0x0F12,0x4298},    // 700020AC 
{0x0F12,0xD300},    // 700020AE 
{0x0F12,0x0018},    // 700020B0 
{0x0F12,0xF000},    // 700020B2 
{0x0F12,0xF907},    // 700020B4 
{0x0F12,0x9007},    // 700020B6 
{0x0F12,0x0021},    // 700020B8 
{0x0F12,0x0028},    // 700020BA 
{0x0F12,0xAA04},    // 700020BC 
{0x0F12,0xF000},    // 700020BE 
{0x0F12,0xF909},    // 700020C0 
{0x0F12,0x9006},    // 700020C2 
{0x0F12,0x88A8},    // 700020C4 
{0x0F12,0x2800},    // 700020C6 
{0x0F12,0xD102},    // 700020C8 
{0x0F12,0x27FF},    // 700020CA 
{0x0F12,0x1C7F},    // 700020CC 
{0x0F12,0xE047},    // 700020CE 
{0x0F12,0x88A0},    // 700020D0 
{0x0F12,0x2800},    // 700020D2 
{0x0F12,0xD101},    // 700020D4 
{0x0F12,0x2700},    // 700020D6 
{0x0F12,0xE042},    // 700020D8 
{0x0F12,0x8820},    // 700020DA 
{0x0F12,0x466B},    // 700020DC 
{0x0F12,0x8198},    // 700020DE 
{0x0F12,0x8860},    // 700020E0 
{0x0F12,0x81D8},    // 700020E2 
{0x0F12,0x8828},    // 700020E4 
{0x0F12,0x8118},    // 700020E6 
{0x0F12,0x8868},    // 700020E8 
{0x0F12,0x8158},    // 700020EA 
{0x0F12,0xA802},    // 700020EC 
{0x0F12,0xC803},    // 700020EE 
{0x0F12,0xF000},    // 700020F0 
{0x0F12,0xF8F8},    // 700020F2 
{0x0F12,0x9008},    // 700020F4 
{0x0F12,0x8ABA},    // 700020F6 
{0x0F12,0x9808},    // 700020F8 
{0x0F12,0x466B},    // 700020FA 
{0x0F12,0x4342},    // 700020FC 
{0x0F12,0x9202},    // 700020FE 
{0x0F12,0x8820},    // 70002100 
{0x0F12,0x8198},    // 70002102 
{0x0F12,0x8860},    // 70002104 
{0x0F12,0x81D8},    // 70002106 
{0x0F12,0x980A},    // 70002108 
{0x0F12,0x9903},    // 7000210A 
{0x0F12,0xF000},    // 7000210C 
{0x0F12,0xF8EA},    // 7000210E 
{0x0F12,0x9A02},    // 70002110 
{0x0F12,0x17D1},    // 70002112 
{0x0F12,0x0E09},    // 70002114 
{0x0F12,0x1889},    // 70002116 
{0x0F12,0x1209},    // 70002118 
{0x0F12,0x4288},    // 7000211A 
{0x0F12,0xDD1F},    // 7000211C 
{0x0F12,0x8820},    // 7000211E 
{0x0F12,0x466B},    // 70002120 
{0x0F12,0x8198},    // 70002122 
{0x0F12,0x8860},    // 70002124 
{0x0F12,0x81D8},    // 70002126 
{0x0F12,0x980A},    // 70002128 
{0x0F12,0x9903},    // 7000212A 
{0x0F12,0xF000},    // 7000212C 
{0x0F12,0xF8DA},    // 7000212E 
{0x0F12,0x9001},    // 70002130 
{0x0F12,0x8828},    // 70002132 
{0x0F12,0x466B},    // 70002134 
{0x0F12,0x8118},    // 70002136 
{0x0F12,0x8868},    // 70002138 
{0x0F12,0x8158},    // 7000213A 
{0x0F12,0x980A},    // 7000213C 
{0x0F12,0x9902},    // 7000213E 
{0x0F12,0xF000},    // 70002140 
{0x0F12,0xF8D0},    // 70002142 
{0x0F12,0x8AB9},    // 70002144 
{0x0F12,0x9A08},    // 70002146 
{0x0F12,0x4351},    // 70002148 
{0x0F12,0x17CA},    // 7000214A 
{0x0F12,0x0E12},    // 7000214C 
{0x0F12,0x1851},    // 7000214E 
{0x0F12,0x120A},    // 70002150 
{0x0F12,0x9901},    // 70002152 
{0x0F12,0xF000},    // 70002154 
{0x0F12,0xF8B6},    // 70002156 
{0x0F12,0x0407},    // 70002158 
{0x0F12,0x0C3F},    // 7000215A 
{0x0F12,0xE000},    // 7000215C 
{0x0F12,0x2700},    // 7000215E 
{0x0F12,0x8820},    // 70002160 
{0x0F12,0x466B},    // 70002162 
{0x0F12,0xAA05},    // 70002164 
{0x0F12,0x8198},    // 70002166 
{0x0F12,0x8860},    // 70002168 
{0x0F12,0x81D8},    // 7000216A 
{0x0F12,0x8828},    // 7000216C 
{0x0F12,0x8118},    // 7000216E 
{0x0F12,0x8868},    // 70002170 
{0x0F12,0x8158},    // 70002172 
{0x0F12,0xA802},    // 70002174 
{0x0F12,0xC803},    // 70002176 
{0x0F12,0x003B},    // 70002178 
{0x0F12,0xF000},    // 7000217A 
{0x0F12,0xF8BB},    // 7000217C 
{0x0F12,0x88A1},    // 7000217E 
{0x0F12,0x88A8},    // 70002180 
{0x0F12,0x003A},    // 70002182 
{0x0F12,0xF000},    // 70002184 
{0x0F12,0xF8BE},    // 70002186 
{0x0F12,0x0004},    // 70002188 
{0x0F12,0xA804},    // 7000218A 
{0x0F12,0xC803},    // 7000218C 
{0x0F12,0x9A09},    // 7000218E 
{0x0F12,0x9B07},    // 70002190 
{0x0F12,0xF000},    // 70002192 
{0x0F12,0xF8AF},    // 70002194 
{0x0F12,0xA806},    // 70002196 
{0x0F12,0xC805},    // 70002198 
{0x0F12,0x0021},    // 7000219A 
{0x0F12,0xF000},    // 7000219C 
{0x0F12,0xF8B2},    // 7000219E 
{0x0F12,0x6030},    // 700021A0 
{0x0F12,0xB00B},    // 700021A2 
{0x0F12,0xBCF0},    // 700021A4 
{0x0F12,0xBC08},    // 700021A6 
{0x0F12,0x4718},    // 700021A8 
{0x0F12,0xB5F1},    // 700021AA 
{0x0F12,0x9900},    // 700021AC 
{0x0F12,0x680C},    // 700021AE 
{0x0F12,0x493A},    // 700021B0 
{0x0F12,0x694B},    // 700021B2 
{0x0F12,0x698A},    // 700021B4 
{0x0F12,0x4694},    // 700021B6 
{0x0F12,0x69CD},    // 700021B8 
{0x0F12,0x6A0E},    // 700021BA 
{0x0F12,0x4F38},    // 700021BC 
{0x0F12,0x42BC},    // 700021BE 
{0x0F12,0xD800},    // 700021C0 
{0x0F12,0x0027},    // 700021C2 
{0x0F12,0x4937},    // 700021C4 
{0x0F12,0x6B89},    // 700021C6 
{0x0F12,0x0409},    // 700021C8 
{0x0F12,0x0C09},    // 700021CA 
{0x0F12,0x4A35},    // 700021CC 
{0x0F12,0x1E92},    // 700021CE 
{0x0F12,0x6BD2},    // 700021D0 
{0x0F12,0x0412},    // 700021D2 
{0x0F12,0x0C12},    // 700021D4 
{0x0F12,0x429F},    // 700021D6 
{0x0F12,0xD801},    // 700021D8 
{0x0F12,0x0020},    // 700021DA 
{0x0F12,0xE031},    // 700021DC 
{0x0F12,0x001F},    // 700021DE 
{0x0F12,0x434F},    // 700021E0 
{0x0F12,0x0A3F},    // 700021E2 
{0x0F12,0x42A7},    // 700021E4 
{0x0F12,0xD301},    // 700021E6 
{0x0F12,0x0018},    // 700021E8 
{0x0F12,0xE02A},    // 700021EA 
{0x0F12,0x002B},    // 700021EC 
{0x0F12,0x434B},    // 700021EE 
{0x0F12,0x0A1B},    // 700021F0 
{0x0F12,0x42A3},    // 700021F2 
{0x0F12,0xD303},    // 700021F4 
{0x0F12,0x0220},    // 700021F6 
{0x0F12,0xF000},    // 700021F8 
{0x0F12,0xF88C},    // 700021FA 
{0x0F12,0xE021},    // 700021FC 
{0x0F12,0x0029},    // 700021FE 
{0x0F12,0x4351},    // 70002200 
{0x0F12,0x0A09},    // 70002202 
{0x0F12,0x42A1},    // 70002204 
{0x0F12,0xD301},    // 70002206 
{0x0F12,0x0028},    // 70002208 
{0x0F12,0xE01A},    // 7000220A 
{0x0F12,0x0031},    // 7000220C 
{0x0F12,0x4351},    // 7000220E 
{0x0F12,0x0A09},    // 70002210 
{0x0F12,0x42A1},    // 70002212 
{0x0F12,0xD304},    // 70002214 
{0x0F12,0x0220},    // 70002216 
{0x0F12,0x0011},    // 70002218 
{0x0F12,0xF000},    // 7000221A 
{0x0F12,0xF87B},    // 7000221C 
{0x0F12,0xE010},    // 7000221E 
{0x0F12,0x491E},    // 70002220 
{0x0F12,0x8C89},    // 70002222 
{0x0F12,0x000A},    // 70002224 
{0x0F12,0x4372},    // 70002226 
{0x0F12,0x0A12},    // 70002228 
{0x0F12,0x42A2},    // 7000222A 
{0x0F12,0xD301},    // 7000222C 
{0x0F12,0x0030},    // 7000222E 
{0x0F12,0xE007},    // 70002230 
{0x0F12,0x4662},    // 70002232 
{0x0F12,0x434A},    // 70002234 
{0x0F12,0x0A12},    // 70002236 
{0x0F12,0x42A2},    // 70002238 
{0x0F12,0xD302},    // 7000223A 
{0x0F12,0x0220},    // 7000223C 
{0x0F12,0xF000},    // 7000223E 
{0x0F12,0xF869},    // 70002240 
{0x0F12,0x4B16},    // 70002242 
{0x0F12,0x4D18},    // 70002244 
{0x0F12,0x8D99},    // 70002246 
{0x0F12,0x1FCA},    // 70002248 
{0x0F12,0x3AF9},    // 7000224A 
{0x0F12,0xD00A},    // 7000224C 
{0x0F12,0x2001},    // 7000224E 
{0x0F12,0x0240},    // 70002250 
{0x0F12,0x8468},    // 70002252 
{0x0F12,0x0220},    // 70002254 
{0x0F12,0xF000},    // 70002256 
{0x0F12,0xF85D},    // 70002258 
{0x0F12,0x9900},    // 7000225A 
{0x0F12,0x6008},    // 7000225C 
{0x0F12,0xBCF8},    // 7000225E 
{0x0F12,0xBC08},    // 70002260 
{0x0F12,0x4718},    // 70002262 
{0x0F12,0x8D19},    // 70002264 
{0x0F12,0x8469},    // 70002266 
{0x0F12,0x9900},    // 70002268 
{0x0F12,0x6008},    // 7000226A 
{0x0F12,0xE7F7},    // 7000226C 
{0x0F12,0xB570},    // 7000226E 
{0x0F12,0x2200},    // 70002270 
{0x0F12,0x490E},    // 70002272 
{0x0F12,0x480E},    // 70002274 
{0x0F12,0x2401},    // 70002276 
{0x0F12,0xF000},    // 70002278 
{0x0F12,0xF852},    // 7000227A 
{0x0F12,0x0022},    // 7000227C 
{0x0F12,0x490D},    // 7000227E 
{0x0F12,0x480D},    // 70002280 
{0x0F12,0x2502},    // 70002282 
{0x0F12,0xF000},    // 70002284 
{0x0F12,0xF84C},    // 70002286 
{0x0F12,0x490C},    // 70002288 
{0x0F12,0x480D},    // 7000228A 
{0x0F12,0x002A},    // 7000228C 
{0x0F12,0xF000},    // 7000228E 
{0x0F12,0xF847},    // 70002290 
{0x0F12,0xBC70},    // 70002292 
{0x0F12,0xBC08},    // 70002294 
{0x0F12,0x4718},    // 70002296 
{0x0F12,0x0D64},    // 70002298 
{0x0F12,0x7000},    // 7000229A 
{0x0F12,0x0470},    // 7000229C 
{0x0F12,0x7000},    // 7000229E 
{0x0F12,0xA120},    // 700022A0 
{0x0F12,0x0007},    // 700022A2 
{0x0F12,0x0402},    // 700022A4 
{0x0F12,0x7000},    // 700022A6 
{0x0F12,0x14A0},    // 700022A8 
{0x0F12,0x7000},    // 700022AA 
{0x0F12,0x208D},    // 700022AC 
{0x0F12,0x7000},    // 700022AE 
{0x0F12,0x622F},    // 700022B0 
{0x0F12,0x0000},    // 700022B2 
{0x0F12,0x1669},    // 700022B4 
{0x0F12,0x7000},    // 700022B6 
{0x0F12,0x6445},    // 700022B8 
{0x0F12,0x0000},    // 700022BA 
{0x0F12,0x21AB},    // 700022BC 
{0x0F12,0x7000},    // 700022BE 
{0x0F12,0x2AA9},    // 700022C0 
{0x0F12,0x0000},    // 700022C2 
{0x0F12,0x4778},    // 700022C4 
{0x0F12,0x46C0},    // 700022C6 
{0x0F12,0xC000},    // 700022C8 
{0x0F12,0xE59F},    // 700022CA 
{0x0F12,0xFF1C},    // 700022CC 
{0x0F12,0xE12F},    // 700022CE 
{0x0F12,0x5F49},    // 700022D0 
{0x0F12,0x0000},    // 700022D2 
{0x0F12,0x4778},    // 700022D4 
{0x0F12,0x46C0},    // 700022D6 
{0x0F12,0xC000},    // 700022D8 
{0x0F12,0xE59F},    // 700022DA 
{0x0F12,0xFF1C},    // 700022DC 
{0x0F12,0xE12F},    // 700022DE 
{0x0F12,0x5FC7},    // 700022E0 
{0x0F12,0x0000},    // 700022E2 
{0x0F12,0x4778},    // 700022E4 
{0x0F12,0x46C0},    // 700022E6 
{0x0F12,0xC000},    // 700022E8 
{0x0F12,0xE59F},    // 700022EA 
{0x0F12,0xFF1C},    // 700022EC 
{0x0F12,0xE12F},    // 700022EE 
{0x0F12,0x5457},    // 700022F0 
{0x0F12,0x0000},    // 700022F2 
{0x0F12,0x4778},    // 700022F4 
{0x0F12,0x46C0},    // 700022F6 
{0x0F12,0xC000},    // 700022F8 
{0x0F12,0xE59F},    // 700022FA 
{0x0F12,0xFF1C},    // 700022FC 
{0x0F12,0xE12F},    // 700022FE 
{0x0F12,0x5FA3},    // 70002300 
{0x0F12,0x0000},    // 70002302 
{0x0F12,0x4778},    // 70002304 
{0x0F12,0x46C0},    // 70002306 
{0x0F12,0xC000},    // 70002308 
{0x0F12,0xE59F},    // 7000230A 
{0x0F12,0xFF1C},    // 7000230C 
{0x0F12,0xE12F},    // 7000230E 
{0x0F12,0x51F9},    // 70002310 
{0x0F12,0x0000},    // 70002312 
{0x0F12,0x4778},    // 70002314 
{0x0F12,0x46C0},    // 70002316 
{0x0F12,0xF004},    // 70002318 
{0x0F12,0xE51F},    // 7000231A 
{0x0F12,0xA464},    // 7000231C 
{0x0F12,0x0000},    // 7000231E 
{0x0F12,0x4778},    // 70002320 
{0x0F12,0x46C0},    // 70002322 
{0x0F12,0xC000},    // 70002324 
{0x0F12,0xE59F},    // 70002326 
{0x0F12,0xFF1C},    // 70002328 
{0x0F12,0xE12F},    // 7000232A 
{0x0F12,0xA007},    // 7000232C 
{0x0F12,0x0000},    // 7000232E 
{0x0F12,0x6546},    // 70002330 
{0x0F12,0x2062},    // 70002332 
{0x0F12,0x3120},    // 70002334 
{0x0F12,0x3220},    // 70002336 
{0x0F12,0x3130},    // 70002338 
{0x0F12,0x0030},    // 7000233A 
{0x0F12,0xE010},    // 7000233C 
{0x0F12,0x0208},    // 7000233E 
{0x0F12,0x0058},    // 70002340 
{0x0F12,0x0000},    // 70002342 // End of Trap and Patch (Last : 70002342h)// Total Size 896 (0x0380)
{0x0028,0xD000},
{0x002A,0x1000},
{0x0F12,0x0001}, 
{0x0028,0x7000},
{0x002A,0x1662},
{0x0F12,0x03B0},
{0x0F12,0x03B0}, 
{0x0028,0x7000},
{0x002A,0x1658},
{0x0F12,0x9C40},
{0x0F12,0x0000},
{0x0F12,0x9C40},
{0x0F12,0x0000},
{0x0028,0x7000},
{0x002A,0x0ADC},
{0x0F12,0x0AF0}, //setot_uOnlineClocksDiv40                                                                         
{0x002A,0x0AE2},
{0x0F12,0x222E}, //setot_usSetRomWaitStateThreshold4KHz                                                             
{0x002A,0x0B94},
{0x0F12,0x0580}, //awbb_GainsInit_0_:R                                                                              
{0x0F12,0x0400}, //awbb_GainsInit_1_:G                                                                              
{0x0F12,0x05F0}, //awbb_GainsInit_2_:B                                                                              
{0x002A,0x04A0},
{0x0F12,0x8000}, //lt_uLeiInit:AE start                                                                             
{0x002A,0x049A},
{0x0F12,0x00fa}, //lt_uMinExp   0.5ms·Î º¯°æ                          
{0x0028,0xD000},
{0x002A,0xF106},
{0x0F12,0x0001}, // L-OB non sub-sampling: revised by Ana 080128                                                     
{0x002A,0xF206},                                                
{0x0F12,0x0001}, // F-OB non sub-sampling: revised by Ana 080128                                                     							//	{0x/W,0xRI,0xTE,0x	,}D000F260	0000	   tgr_auto_exp (shutter on off:0b shutter off): revised by Ana 080112     
{0x002A,0xC202},
{0x0F12,0x0700}, // tgr_coarse_integration_time[15:0]: revised by Ana 080115                                        
{0x002A,0xF260},
{0x0F12,0x0001}, //TGR_AUTO EXP OFF                                                                                 
{0x002A,0xF414},
{0x0F12,0x0030}, // aig_adc_sat[7:0] (aig_adc_sat[7:4]+1)*33mV + 690mV                                                                                                                     
{0x002A,0xC204},                                                      
{0x0F12,0x0100}, // tgr_analogue_code_global[12:0] Analog gain X8                                                   
{0x002A,0xF402},                                                      
{0x0F12,0x0092}, // aig_offset[7:0] : revised by Ana 080425                                                         
{0x0F12,0x007F}, // aig_offset2[7:0]: revised by Ana 080425                                                         
{0x002A,0xF700},
{0x0F12,0x0040}, // bpradlc_control[7:0]: revised by Ana 080529                                                     
{0x0F12,0x0000}, // bpradlc_f_adlc_tune_gr[7:0]: revised by Tg 080529                                               
{0x0F12,0x0000}, // bpradlc_f_adlc_tune_gb[7:0]: revised by TG 080529                                               
{0x0F12,0x0000}, // bpradlc_f_adlc_tune_b[7:0]: revised by TG 080529                                                
{0x0F12,0x0000}, // bpradlc_f_adlc_tune_total[7:0]: revised by TG 080529                                            
{0x0F12,0x0040}, // bpradlc_line_adlc_tune_r[7:0]: revised by Ana 080425                                            
{0x0F12,0x0040}, // bpradlc_line_adlc_tune_gr[7:0]: revised by Ana 080425                                           
{0x0F12,0x0040}, // bpradlc_line_adlc_tune_gb[7:0]: revised by Ana 080425                                           
{0x0F12,0x0040}, // bpradlc_line_adlc_tune_b[7:0]: revised by Ana 080425                                            
{0x0F12,0x0040}, // bpradlc_line_adlc_tune_total[7:0]: revised by Ana 080425                                        
{0x0F12,0x0001}, // bpradlc_adlc_en[7:0]: revised by TG 080529                                                                         
{0x0F12,0x0015}, // bpradlc_adlc_option[7:0]: revised by TG 080425                                                                          
{0x0F12,0x0001}, // bpradlc_adlc_bpr_en[7:0] adlc_bpr_enable: revised by Ana 080112                                 
{0x0F12,0x0040}, // bpradlc_adlc_bpr_thresh[7:0]: 080425                                                            
{0x002A,0xF48A},
{0x0F12,0x0048}, // aig_ld_ptr7[9:0] line OB start addr(04h~48h): revised by Ana 080911                             
{0x002A,0xF10A},
{0x0F12,0x008B}, // tgr_h_desc0_addr_end[10:0] line OB end addr(47h~8Bh): revised by Ana 080911                                  //     line OB start - line OB end = 67d                                                   
{0x002A,0xF900},
{0x0F12,0x0067}, // cfpn_config: revised by Ana 080425                                                                               //     cfpn_ref_gain[9:7],cpfn_ref_lines[6:5],cfpn_hbinning[4],cfpn_output_direct[3],      
{0x002A,0xF406},
{0x0F12,0x0092}, // aig_cfpn_ref_offset[7:0] : revised by Ana 080425                                                
{0x0F12,0x007F}, // aig_cfpn_ref_offset2[7:0]: revised by Ana 080425                                                
{0x0F12,0x0003}, // aig_cfpn_ref_gain[7:0]: revised by Ana 080425                                                   
{0x0F12,0x0003}, // aig_ld_ctrl[1:0] aig_ldb_en[1], aig_ld_en[0]                                                    
{0x0F12,0x0003}, // aig_row_id_ctrl[1:0]                                                                            
{0x002A,0xF442},                                                     
{0x0F12,0x0000}, // aig_vavg[0]                                                                                     
{0x0F12,0x0000}, // aig_havg[0]=1b @H-avg mode: revised by Ana 080116                                               
{0x002A,0xF448},                                                     
{0x0F12,0x0000}, // aig_sl_off[0]                                                                                   
{0x002A,0xF456},                                                     
{0x0F12,0x0001}, // aig_blst_en[0]                                                                                  
{0x0F12,0x0010}, // aig_blst_en_cintr[15:0]                                                                         
{0x0F12,0x0000}, // aig_dshut_en[0]                                                                                 
{0x002A,0xF41A},
{0x0F12,0x00FF}, // aig_comp_bias[7:0] aig_comp2_bias[7:4], aig_comp1_bias[3:0]: revised by Ana 081013              
{0x0F12,0x0003}, // aig_pix_bias[3:0]                                                                                                                                                                                           
{0x002A,0xF420},                                                                                            
{0x0F12,0x0030}, // aig_clp_lvl[7:0]: revised by Ana 080910(refer to 6AA)                                           
{0x002A,0xF410},                                                                                            
{0x0F12,0x0001}, // aig_clp_sl_ctrl[0]                                                                                                                                                                                           
{0x0F12,0x0000}, // aig_cds_test[7:0], aig_cds_test[1]=1b @H-avg mode current save: revised by Ana 080116           
{0x002A,0xF416},                                                                                            
{0x0F12,0x0001}, // aig_rmp_option[7:0]                                                                             
{0x002A,0xF424},                                                                                            
{0x0F12,0x0000}, // aig_ref_option[7:0], aig_ref_option[0]=1b @H-avg mode odd cds off: revised by Ana 080116        
{0x002A,0xF422},                                                                                            
{0x0F12,0x0000}, // aig_monit[7:0]                                                                                                                                                                                              
{0x002A,0xF41E},                                                                                            
{0x0F12,0x0000}, // aig_pd_pix[0]                                                                                   
{0x002A,0xF428},                                                                                            
{0x0F12,0x0000}, // aig_pd_cp[1:0] aig_pd_ncp[1], aig_pd_cp[0]                                                      
{0x0F12,0x0000}, // aig_pd_reg_pix[0]                                                                               
{0x0F12,0x0000}, // aig_pd_reg_rg[0]                                                                                
{0x002A,0xF430},                                                                                            
{0x0F12,0x0000}, // aig_pd_reg_tgsl[0]                                                                              
{0x0F12,0x0000}, // aig_pd_reg_ntg[0]                                                                                                                                                                                          
{0x0F12,0x0008}, // aig_rosc_tune_cp[3:0]: revised by Ana 080418                                                    
{0x0F12,0x0005}, // aig_rosc_tune_ncp[3:0]                                                                          
{0x0F12,0x000F}, // aig_cp_capa[3:0]                                                                                
{0x0F12,0x0001}, // aig_reg_tune_pix[7:0]                                                                           
{0x0F12,0x0040}, // aig_reg_tune_rg[7:0]                                                                            
{0x0F12,0x0040}, // aig_reg_tune_tgsl[7:0]                                                                          
{0x0F12,0x0010}, // aig_reg_tune_ntg[7:0]                                                                           
{0x002A,0xF4D6},
{0x0F12,0x0090}, //                                                                                                                                                                                                                                       
{0x0F12,0x0000}, // aig_vreg_sel[7:0] [9]h_test, [1]reg_sw_stby, [0]aig_reg_sel                                                                                                                
{0x002A,0xF47C},                                                               
{0x0F12,0x000C}, // aig_ld_ptr0[4:0]                                                                            
{0x0F12,0x0000}, // aig_ld_ptr1[8:0]: revised by Ana 081010                                                     
{0x002A,0xF49A},                                                               
{0x0F12,0x0008}, // aig_sla_ptr0[3:0]: revised by Ana 080911                                                    
{0x0F12,0x0000}, // aig_sla_ptr1[8:0]: revised by Ana 081010                                                    
{0x002A,0xF4A2},                                                               
{0x0F12,0x0008}, // aig_slb_ptr0[7:0]: revised by Ana 080911                                                    
{0x0F12,0x0000}, // aig_slb_ptr1[8:0]: revised by Ana 081010                                                    
{0x002A,0xF4B2},                                                               
{0x0F12,0x0013}, // aig_rxa_ptr0[6:0]: revised by Ana 081016 for CFPN                                           
{0x0F12,0x0000}, // aig_rxa_ptr1[9:0]: revised by Ana 081010                                                    
{0x0F12,0x0013}, // aig_rxb_ptr0[6:0]: revised by Ana 081016 for CFPN                                           
{0x0F12,0x0000}, // aig_rxb_ptr1[9:0]: revised by Ana 081010                                                    
{0x002A,0xF4AA},                                                               
{0x0F12,0x009B}, // aig_txa_ptr0[8:0]: revised by Ana 081016 for CFPN                                           
{0x0F12,0x00FB}, // aig_txa_ptr1[8:0]: revised by Ana 081016 for CFPN                                           
{0x0F12,0x009B}, // aig_txb_ptr0[9:0]: revised by Ana 081016 for CFPN                                           
{0x0F12,0x00FB}, // aig_txb_ptr1[9:0]: revised by Ana 081016 for CFPN                                           
{0x002A,0xF474},                                                               
{0x0F12,0x0017}, // aig_s3_ptr0[6:0]: revised by Ana 081016 for CFPN                                             
{0x0F12,0x005F}, // aig_s3_ptr1[8:0]: revised by Ana 081016 for CFPN                                             
{0x0F12,0x0017}, // aig_s4_ptr0[6:0]: revised by Ana 081016 for CFPN                                             
{0x0F12,0x008F}, // aig_s4_ptr1[8:0]: revised by Ana 081016 for CFPN                                                                                                                            
{0x002A,0xF48C},                                                               
{0x0F12,0x0017}, // aig_clp_sl_ptr0[6:0]: revised by Ana 081016 for CFPN                                         
{0x0F12,0x009B}, // aig_clp_sl_ptr1[8:0]: revised by Ana 081016 for CFPN                                         
{0x002A,0xF4C8},                                                               
{0x0F12,0x0163}, // aig_off_en_ptr0[9:0]: revised by Ana 081016 for CFPN                                         
{0x0F12,0x0193}, // aig_rmp_en_ptr0[9:0]: revised by Ana 081016 for CFPN                                         
{0x002A,0xF490},                                                               
{0x0F12,0x0191}, // aig_comp_en_ptr0[9:0]: revised by Ana 081016 for CFPN                                                                                                                        
{0x002A,0xF418},                                                               
{0x0F12,0x0083}, // aig_dbs_option[7:0]: revised by Ana 081010                                                                                                                                   
{0x002A,0xF454},                                                               
{0x0F12,0x0001}, // aig_power_save[0]: revised by Ana 081229                           
{0x002A,0xF702},
{0x0F12,0x0081},
{0x002A,0xF4D2},
{0x0F12,0x0000},
{0x0028,0x7000},
{0x002A,0x1176},
{0x0F12,0x0020}, // fpn_GainInput[0]                                                                                
{0x0F12,0x0040}, // fpn_GainInput[1]                                                                                
{0x0F12,0x0080}, // fpn_GainInput[2]                                                                                
{0x0F12,0x0100}, // fpn_GainInput[3]                                                                                
{0x0F12,0x0014}, // fpn_GainOutput[0]                                                                               
{0x0F12,0x000A}, // fpn_GainOutput[1]                                                                               
{0x0F12,0x0008}, // fpn_GainOutput[2]                                                                               
{0x0F12,0x0004}, // fpn_GainOutput[3]                                                                                                 // 	CFPN Canceller                                                                       
{0x002A,0x116C},
{0x0F12,0x0000}, // REF Gain                                                                                        
{0x0F12,0x0000}, // OvflProtect                                                                                     
{0x0F12,0x0000}, // bypassThrough                                                                                   
{0x0F12,0x0000}, // bypass                                                                                          
{0x0F12,0x0002}, // fpn.CollectMethod           0 : Center	1 : Right	2: LEFT                              
{0x002A,0x0AE8},
{0x0F12,0x0000}, // setot_bSendFpnToISP = ??                                                                                          // sensor aig table setting   sunkyu start.  	                                              
{0x002A,0x10EE}, 
{0x0F12,0x0000}, // senHal_SenRegsModes3_usRegCount   0 value --> not use aig table 3                                                  // below register is needed for caculating forbidden and non-linear area.                                     // because senHal_SenRegsModes3_usRegCount is 0, below value does not write to HW.          
{0x002A,0x10F2},
{0x0F12,0x0000}, //senHal_SenRegsModes3_pSenModesRegsArray3[0]	real register 0xD000F45A                             
{0x002A,0x1152},                                                                          
{0x0F12,0x0030}, //senHal_SenRegsModes3_pSenModesRegsArray3[48] real register 0xD000F4BA                            
{0x0F12,0x0028}, //senHal_SenRegsModes3_pSenModesRegsArray3[49] real register 0xD000F4BC                            
{0x0F12,0x0030}, //senHal_SenRegsModes3_pSenModesRegsArray3[50] real register 0xD000F4BE                            
{0x002A,0x1148},                                                                          
{0x0F12,0x00FB}, //senHal_SenRegsModes3_pSenModesRegsArray3[43] real register 0xD000F4B0                            
{0x002A,0x1144},                                                                          
{0x0F12,0x00FB}, //senHal_SenRegsModes3_pSenModesRegsArray3[41] real register 0xD000F4AC                                              // this register is needed to cac forbidden area, this register have to be large for forbidden.     
{0x002A,0x1150},
{0x0F12,0x01F4}, //senHal_SenRegsModes3_pSenModesRegsArray3[47] real register 0xD000F4B8                                            //   sensor aig table setting  sunkyu end                                                
{0x002A,0x1084},
{0x0F12,0x0000}, //senHal_bUseAnalogBinning                                                                        
{0x0F12,0x0000}, //senHal_bUseAnalogVerAvg                                                                         
{0x002A,0x0F4C},
{0x0F12,0x003A}, //TVAR_ae_BrAve                                                                                   
{0x002A,0x0478},
{0x0F12,0x0114},
{0x0F12,0x00EB}, //ae boundary                                                                                                                                                             
{0x002A,0x0484},                                                                          
{0x0F12,0x410A},	//8214	//uMaxExp1                         
{0x0F12,0x0000},                                                                          
{0x002A,0x048C},                                                                          
{0x0F12,0x8214},	//045A	//uMaxExp2                                 
{0x0F12,0x0000},	//0001                                                                          
{0x0F12,0xA122},	//545A	//uMaxExp3                                 
{0x0F12,0x0000},	//0001                                                                          
{0x002A,0x0488},                                                                          
{0x0F12,0xf424},	//C4B4	//E45A	//uMaxExp4                         
{0x0F12,0x0000},	//0004	//0001                                                                          
{0x002A,0x043A},                                                                          
{0x0F12,0x01B0},	//lt_uMaxAnGain0          
{0x0F12,0x01C0},	//lt_uMaxAnGain0_1        
{0x002A,0x0494},                                                                          
{0x0F12,0x02B0},	//lt_uMaxAnGain1      
{0x0F12,0x0580},	//lt_uMaxAnGain2  
{0x0f12,0x0100},    
{0x002A,0x0F52},
{0x0F12,0x000F}, //ae_StatMode                   // Flicker                                                                                
{0x002A,0x0B2E},
{0x0F12,0x0001}, // AFC_Default60Hz	 Auto Flicker 60Hz start 0: Auto Flicker 50Hz start                         
{0x002A,0x03F8},
{0x0F12,0x007F}, // REG_TC_DBG_AutoAlgEnBits	                  default : 007F                                                              
{0x002A,0x01B8},
{0x0F12,0x5DC0}, //REG_TC_IPRM_InClockLSBs            Min CLK                                                      
{0x0F12,0x0000}, //REG_TC_IPRM_InClockMSBs                                                                                                          
{0x002A,0x01CC},
{0x0F12,0x1770}, //24MHz REG_TC_IPRM_OpClk4KHz_0             System CLK                                                                      
{0x0F12,0x2328}, //36MHz REG_TC_IPRM_MinOutRate4KHz_0                  P CLK                                       
{0x0F12,0x251C}, //38MHz REG_TC_IPRM_MaxOutRate4KHz_0                                                              
{0x002A,0x01D2},
{0x0F12,0x0bb8}, //12MHz REG_TC_IPRM_OpClk4KHz_1                                                   
{0x0F12,0x1676}, //REG_TC_IPRM_MinOutRate4KHz_1                                       
{0x0F12,0x18a6}, //REG_TC_IPRM_MaxOutRate4KHz_1                                       
{0x002A,0x01C6},
{0x0F12,0x0003}, //REG_TC_IPRM_UseNPviClocks                                                             
{0x0F12,0x0000}, //REG_TC_IPRM_UseNMipiClocks                                                            
{0x002A,0x01C0},                               
{0x0F12,0x0000}, //EG_TC_IPRM_ValidVActiveLow                                                            
{0x002A,0x01DE},                               
{0x0F12,0x0001}, //REG_TC_IPRM_UseRegsAPI                                                                
{0x0F12,0x0001}, //REG_TC_IPRM_InitParamsUpdated                                                         
{0x002A,0x1084},                               
{0x0F12,0x0001}, //senHal_bUseAnalogBinning		Avg Sub-Sampling	                          
{0x002A,0x10B6},                               
{0x0F12,0x0048}, //senHal_uMinColsAddAnalogBin                                                           
{0x002A,0x0ADC},
{0x0F12,0x0A28}, //default : 0AF0, setot_uOnlineClockDiv40                                                                                                                    
{0x002A,0x0242},
{0x0F12,0x0320}, //REG_0TC_PCFG_usWidth                                                         
{0x0F12,0x0258}, //REG_0TC_PCFG_usHeight                                                        
{0x0F12,0x0005}, //REG_0TC_PCFG_Format                                                          
{0x0F12,0x251C}, //REG_0TC_PCFG_usMaxOut4KHzRate                                                
{0x0F12,0x2328}, //REG_0TC_PCFG_usMinOut4KHzRate 
{0x0F12,0x0072}, //REG_0TC_PCFG_PVIMask          
{0x0F12,0x0000}, //REG_0TC_PCFG_uClockInd        
{0x0F12,0x0000}, //REG_0TC_PCFG_usFrTimeType         
{0x0F12,0x0000}, //REG_0TC_PCFG_FrRateQualityType    
{0x0F12,0x0535}, //REG_0TC_PCFG_usMaxFrTimeMsecMult10
{0x0F12,0x029A}, //REG_0TC_PCFG_usMinFrTimeMsecMult10
{0x002A,0x0288},                               
{0x0F12,0x0000}, //REG_0TC_PCFG_uPrevMirror            
{0x0F12,0x0000}, //REG_0TC_PCFG_uCaptureMirror   
{0x002A,0x0268},
{0x0F12,0x0320}, //REG_1TC_PCFG_usWidth                                                         
{0x0F12,0x0258}, //REG_1TC_PCFG_usHeight                                                        
{0x0F12,0x0005}, //REG_1TC_PCFG_Format                                                          
{0x0F12,0x251C}, //REG_1TC_PCFG_usMaxOut4KHzRate                                                
{0x0F12,0x2328}, //REG_1TC_PCFG_usMinOut4KHzRate 
{0x0F12,0x0052}, //REG_1TC_PCFG_PVIMask          
{0x0F12,0x0000}, //REG_1TC_PCFG_uClockInd        
{0x0F12,0x0000}, //REG_1TC_PCFG_usFrTimeType         
{0x0F12,0x0000}, //REG_1TC_PCFG_FrRateQualityType    
{0x0F12,0x029A}, //REG_1TC_PCFG_usMaxFrTimeMsecMult10
{0x0F12,0x029A}, //REG_1TC_PCFG_usMinFrTimeMsecMult10
{0x002A,0x0288},                               
{0x0F12,0x0000}, //REG_1TC_PCFG_uPrevMirror            
{0x0F12,0x0000}, //REG_1TC_PCFG_uCaptureMirror 
{0x002A,0x028E},
{0x0F12,0x0320}, //REG_2TC_PCFG_usWidth                                                          
{0x0F12,0x0258}, //REG_2TC_PCFG_usHeight                                                         
{0x0F12,0x0005}, //REG_2TC_PCFG_Format                                                           
{0x0F12,0x251C}, //REG_2TC_PCFG_usMaxOut4KHzRate                                                 
{0x0F12,0x2328}, //REG_2TC_PCFG_usMinOut4KHzRate 
{0x0F12,0x0052}, //REG_2TC_PCFG_PVIMask          
{0x0F12,0x0000}, //REG_2TC_PCFG_uClockInd        
{0x0F12,0x0000}, //REG_2TC_PCFG_usFrTimeType         
{0x0F12,0x0000}, //REG_2TC_PCFG_FrRateQualityType    
{0x0F12,0x07D0}, //REG_2TC_PCFG_usMaxFrTimeMsecMult10
{0x0F12,0x029A}, //REG_2TC_PCFG_usMinFrTimeMsecMult10
{0x002A,0x02AE},                               
{0x0F12,0x0000}, //REG_2TC_PCFG_uPrevMirror            
{0x0F12,0x0000}, //REG_2TC_PCFG_uCaptureMirror  
{0x002A,0x02B4},
{0x0F12,0x0320}, //REG_2TC_PCFG_usWidth                                                          
{0x0F12,0x0258}, //REG_2TC_PCFG_usHeight                                                         
{0x0F12,0x0005}, //REG_2TC_PCFG_Format                                                           
{0x0F12,0x251C}, //REG_2TC_PCFG_usMaxOut4KHzRate                                                 
{0x0F12,0x2328}, //REG_2TC_PCFG_usMinOut4KHzRate 
{0x0F12,0x0052}, //REG_2TC_PCFG_PVIMask          
{0x0F12,0x0000}, //REG_2TC_PCFG_uClockInd        
{0x0F12,0x0000}, //REG_2TC_PCFG_usFrTimeType         
{0x0F12,0x0000}, //REG_2TC_PCFG_FrRateQualityType    
{0x0F12,0x186A},	//07D0 //REG_2TC_PCFG_usMaxFrTimeMsecMult10
{0x0F12,0x029A}, //REG_2TC_PCFG_usMinFrTimeMsecMult10
{0x002A,0x02D4},                               
{0x0F12,0x0000}, //REG_2TC_PCFG_uPrevMirror            
{0x0F12,0x0000}, //REG_2TC_PCFG_uCaptureMirror                                                                       
{0x002A,0x021C},
{0x0F12,0x0000}, //REG_TC_GP_ActivePrevConfig                                                        
{0x002A,0x0220},                               
{0x0F12,0x0001}, //REG_TC_GP_PrevOpenAfterChange                                                     
{0x002A,0x01F8},                               
{0x0F12,0x0001}, //REG_TC_GP_NewConfigSync                                                           
{0x002A,0x021E},                               
{0x0F12,0x0001}, //REG_TC_GP_PrevConfigChanged                                                                                                                           
{0x002A,0x0302},
{0x0F12,0x0001}, //REG_1TC_CCFG_uCaptureMode 
{0x0F12,0x0640}, //REG_1TC_CCFG_usWidth                      	
{0x0F12,0x04B0}, //REG_1TC_CCFG_usHeight                     	
{0x0F12,0x0005}, //REG_1TC_CCFG_Format                       	
{0x0F12,0x251C}, //REG_1TC_CCFG_usMaxOut4KHzRate             	
{0x0F12,0x2328}, //REG_1TC_CCFG_usMinOut4KHzRate             	
{0x0F12,0x0052}, //REG_1TC_CCFG_PVIMask                      	
{0x0F12,0x0000}, //REG_1TC_CCFG_uClockInd                    	
{0x0F12,0x0000}, //REG_1TC_CCFG_usFrTimeType                 	
{0x0F12,0x0002}, //REG_1TC_CCFG_FrRateQualityType            	
{0x0F12,0x0535}, //REG_1TC_CCFG_usMaxFrTimeMsecMult10        	
{0x0F12,0x0535}, //REG_1TC_CCFG_usMinFrTimeMsecMult10        	                                                       
{0x002A,0x0324},
{0x0F12,0x0001}, //REG_1TC_CCFG_uCaptureMode 
{0x0F12,0x0640}, //REG_1TC_CCFG_usWidth                      	
{0x0F12,0x04B0}, //REG_1TC_CCFG_usHeight                     	
{0x0F12,0x0005}, //REG_1TC_CCFG_Format                       	
{0x0F12,0x18a6}, //REG_1TC_CCFG_usMaxOut4KHzRate             	
{0x0F12,0x1676}, //REG_1TC_CCFG_usMinOut4KHzRate             	
{0x0F12,0x0052}, //REG_1TC_CCFG_PVIMask                      	
{0x0F12,0x0001}, //REG_1TC_CCFG_uClockInd                    	
{0x0F12,0x0000}, //REG_1TC_CCFG_usFrTimeType                 	
{0x0F12,0x0002}, //REG_1TC_CCFG_FrRateQualityType            	
{0x0F12,0x1388}, //REG_1TC_CCFG_usMaxFrTimeMsecMult10        	
{0x0F12,0x1388}, //REG_1TC_CCFG_usMinFrTimeMsecMult10        	                  
{0x002A,0x0346},
{0x0F12,0x0001}, //REG_2TC_CCFG_uCaptureMode 
{0x0F12,0x0640}, //REG_2TC_CCFG_usWidth                      	
{0x0F12,0x04B0}, //REG_2TC_CCFG_usHeight                     	
{0x0F12,0x0005}, //REG_2TC_CCFG_Format                       	
{0x0F12,0x18a6}, //REG_2TC_CCFG_usMaxOut4KHzRate             	
{0x0F12,0x1676}, //REG_2TC_CCFG_usMinOut4KHzRate             	
{0x0F12,0x0052}, //REG_2TC_CCFG_PVIMask                      	
{0x0F12,0x0001}, //REG_2TC_CCFG_uClockInd                    	
{0x0F12,0x0000}, //REG_2TC_CCFG_usFrTimeType                 	
{0x0F12,0x0002}, //REG_2TC_CCFG_FrRateQualityType            	
{0x0F12,0x2710}, //186A //REG_2TC_CCFG_usMaxFrTimeMsecMult10        	
{0x0F12,0x2710}, //186A //REG_2TC_CCFG_usMinFrTimeMsecMult10        	                                                                         
{0x002A,0x0780},
{0x0F12,0x0000}, // msm_uOffsetNoBin[0][0]                                                                  
{0x0F12,0x0000}, // msm_uOffsetNoBin[0][1]                                                                  
{0x0F12,0x0000}, // msm_uOffsetNoBin[1][0]                                                                  
{0x0F12,0x0000}, // msm_uOffsetNoBin[1][1]                                                                  
{0x0F12,0x0000}, // msm_uOffsetNoBin[2][0]                                                                  
{0x0F12,0x0000}, // msm_uOffsetNoBin[2][1]                                                                  
{0x0F12,0x0000}, // msm_uOffsetNoBin[3][0]                                                                  
{0x0F12,0x0000}, // msm_uOffsetNoBin[3][1]                                                                  
{0x002A,0x0798},
{0x0F12,0x0000}, //msm_uOffsetBin[0][0]                                                                     
{0x0F12,0x0000}, //msm_uOffsetBin[0][1]                                                                     
{0x0F12,0x0000}, //msm_uOffsetBin[1][0]                                                                     
{0x0F12,0x0000}, //msm_uOffsetBin[1][1]                                                                     
{0x002A,0x07C0},
{0x0F12,0x0004}, //msm_NonLinearOfsOutput[2]                                                                
{0x0F12,0x0004}, //msm_NonLinearOfsOutput[3]                                                                
{0x002A,0x0B94},
{0x0F12,0x0580}, //awbb_GainsInit_0_:R                                                                      
{0x0F12,0x0400}, //awbb_GainsInit_1_:G                                                                      
{0x0F12,0x05F0}, //awbb_GainsInit_2_:B                                                                      
{0x002A,0x04A0},
{0x0F12,0x8000}, //lt_uLeiInit:AE start                                                                      					                                                                       
{0x002A,0x0F5A},//  AE Weight
{0x0F12,0x0101}, // ae_WeightTbl_16_0_                                                                      
{0x0F12,0x0101}, // ae_WeightTbl_16_1_                                                                      
{0x0F12,0x0101}, // ae_WeightTbl_16_2_                                                                      
{0x0F12,0x0101}, // ae_WeightTbl_16_3_7                                                                     
{0x0F12,0x0101}, // ae_WeightTbl_16_4_                                                                      
{0x0F12,0x0101}, // ae_WeightTbl_16_5_                                                                      
{0x0F12,0x0101}, // ae_WeightTbl_16_6_                                                                      
{0x0F12,0x0101}, // ae_WeightTbl_16_7_                                                                      
{0x0F12,0x0101}, // ae_WeightTbl_16_8_                                                                      
{0x0F12,0x0202}, // ae_WeightTbl_16_9_                                                                      
{0x0F12,0x0202}, // ae_WeightTbl_16_10                                                                      
{0x0F12,0x0101}, // ae_WeightTbl_16_11                                                                      
{0x0F12,0x0101}, // ae_WeightTbl_16_12                                                                      
{0x0F12,0x0202}, // ae_WeightTbl_16_13                                                                      
{0x0F12,0x0202}, // ae_WeightTbl_16_14                                                                      
{0x0F12,0x0101}, // ae_WeightTbl_16_15                                                                      
{0x0F12,0x0201}, // ae_WeightTbl_16_16                                                                      
{0x0F12,0x0303}, // ae_WeightTbl_16_17                                                                      
{0x0F12,0x0303}, // ae_WeightTbl_16_18                                                                      
{0x0F12,0x0102}, // ae_WeightTbl_16_19                                                                      
{0x0F12,0x0201}, // ae_WeightTbl_16_20                                                                      
{0x0F12,0x0303}, // ae_WeightTbl_16_21                                                                      
{0x0F12,0x0303}, // ae_WeightTbl_16_22                                                                      
{0x0F12,0x0102}, // ae_WeightTbl_16_23                                                                      
{0x0F12,0x0101}, // ae_WeightTbl_16_24                                                                      
{0x0F12,0x0202}, // ae_WeightTbl_16_25                                                                      
{0x0F12,0x0202}, // ae_WeightTbl_16_26                                                                      
{0x0F12,0x0101}, // ae_WeightTbl_16_27                                                                      
{0x0F12,0x0101}, // ae_WeightTbl_16_28                                                                      
{0x0F12,0x0101}, // ae_WeightTbl_16_29                                                                      
{0x0F12,0x0101}, // ae_WeightTbl_16_30                                                                      
{0x0F12,0x0101}, // ae_WeightTbl_16_31                                                                                                                                                          
{0x002A,0x0704},
{0x0F12,0x00B3}, //TVAR_ash_AwbAshCord_0_                                                                   
{0x0F12,0x00E5}, //TVAR_ash_AwbAshCord_1_                                                                   
{0x0F12,0x0120}, //TVAR_ash_AwbAshCord_2_                                                                   
{0x0F12,0x0136}, //TVAR_ash_AwbAshCord_3_                                                                   
{0x0F12,0x0180}, //TVAR_ash_AwbAshCord_4_                                                                   
{0x0F12,0x01B0}, //TVAR_ash_AwbAshCord_5_                                                                   
{0x0F12,0x0200}, //TVAR_ash_AwbAshCord_6_                                                                                                                 
{0x002A,0x06F2},
{0x0F12,0x00B3}, //SARR_AwbCcmCord_0_	Hor                                                                  
{0x0F12,0x00E5}, //SARR_AwbCcmCord_1_	IncaA                                                                
{0x0F12,0x0120}, //SARR_AwbCcmCord_2_	WW                                                                   
{0x0F12,0x0136}, //SARR_AwbCcmCord_3_	CW                                                                   
{0x0F12,0x0180}, //SARR_AwbCcmCord_4_	D50                                                                  
{0x0F12,0x0190}, //SARR_AwbCcmCord_5_	D65                                                                                                                      
{0x002A,0x103E},
{0x0F12,0x0000}, //SARR_IllumType_0_                                                                        
{0x0F12,0x0009}, //SARR_IllumType_1_                                                                        
{0x0F12,0x0018}, //SARR_IllumType_2_                                                                        
{0x0F12,0x0032}, //SARR_IllumType_3_                                                                        
{0x0F12,0x004A}, //SARR_IllumType_4_                                                                        
{0x0F12,0x0051}, //SARR_IllumType_5_                                                                        
{0x0F12,0x0056}, //SARR_IllumType_6_                                                                        
{0x0F12,0x010C}, //SARe_2_R_IllumTypeF_0_                                                                   
{0x0F12,0x010C}, //SARe_3_R_IllumTypeF_1_                                                                   
{0x0F12,0x0109}, //SARe_4_R_IllumTypeF_2_                                                                   
{0x0F12,0x0105}, //SARe_5_R_IllumTypeF_3_                                                                   
{0x0F12,0x0102}, //SARe_6_R_IllumTypeF_4_                                                                   
{0x0F12,0x00FB}, //SARR_IllumTypeF_5_                                                                       
{0x0F12,0x00F8}, //SARR_IllumTypeF_6_                                                                       					// TVAR_ash_GASAlpha(Indoor)                                                       
{0x002A,0x0712}, 
{0x0F12,0x0100}, //TVAR_ash_GASAlpha[0]                                                                     
{0x0F12,0x0100}, //TVAR_ash_GASAlpha[1]                                                                     
{0x0F12,0x0100}, //TVAR_ash_GASAlpha[2]                                                                     
{0x0F12,0x0100}, //TVAR_ash_GASAlpha[3]                                                                     
{0x0F12,0x0100}, //TVAR_ash_GASAlpha[4]                                                                     
{0x0F12,0x0100}, //TVAR_ash_GASAlpha[5]                                                                     
{0x0F12,0x0100}, //TVAR_ash_GASAlpha[6]                                                                     
{0x0F12,0x0100}, //TVAR_ash_GASAlpha[7]                                                                     
{0x0F12,0x0100}, //TVAR_ash_GASAlpha[8]                                                                     
{0x0F12,0x0100}, //TVAR_ash_GASAlpha[9]                                                                     
{0x0F12,0x0100}, //TVAR_ash_GASAlpha[10]                                                                    
{0x0F12,0x0100}, //TVAR_ash_GASAlpha[11]                                                                    
{0x0F12,0x0100}, //TVAR_ash_GASAlpha[12]                                                                    
{0x0F12,0x0100}, //TVAR_ash_GASAlpha[13]                                                                    
{0x0F12,0x0100}, //TVAR_ash_GASAlpha[14]                                                                    
{0x0F12,0x0100}, //TVAR_ash_GASAlpha[15]                                                                    
{0x0F12,0x0100}, //TVAR_ash_GASAlpha[16]                                                                    
{0x0F12,0x0100}, //TVAR_ash_GASAlpha[17]                                                                    
{0x0F12,0x0100}, //TVAR_ash_GASAlpha[18]                                                                    
{0x0F12,0x0100}, //TVAR_ash_GASAlpha[19]                                                                    
{0x0F12,0x0100}, //TVAR_ash_GASAlpha[20]                                                                    
{0x0F12,0x0100}, //TVAR_ash_GASAlpha[21]                                                                    
{0x0F12,0x0100}, //TVAR_ash_GASAlpha[22]                                                                    
{0x0F12,0x0100}, //TVAR_ash_GASAlpha[23]                                                                    
{0x0F12,0x0100}, //TVAR_ash_GASAlpha[24]                                                                    
{0x0F12,0x0100}, //TVAR_ash_GASAlpha[25]                                                                    
{0x0F12,0x0100}, //TVAR_ash_GASAlpha[26]                                                                    
{0x0F12,0x0100}, //TVAR_ash_GASAlpha[27]                                                                                                                        
{0x0F12,0x0100}, //TVAR_ash_GASOutdoorAlpha_0_                                                              
{0x0F12,0x0100}, //TVAR_ash_GASOutdoorAlpha_1_                                                              
{0x0F12,0x0100}, //TVAR_ash_GASOutdoorAlpha_2_                                                              
{0x0F12,0x0100}, //TVAR_ash_GASOutdoorAlpha_3_                                                                                                                        
{0x002A,0x0754},
{0x0F12,0x2388}, //TVAR_ash_pGAS                                                                            
{0x0F12,0x7000}, //TVAR_ash_pGAS                                                                                                                       
{0x002A,0x2388},
{0x0F12,0x01CC}, //TVAR_ash_pGAS[0]                                                                         
{0x0F12,0x0178}, //TVAR_ash_pGAS[1]                                                                         
{0x0F12,0x013B}, //TVAR_ash_pGAS[2]                                                                         
{0x0F12,0x0108}, //TVAR_ash_pGAS[3]                                                                         
{0x0F12,0x00E4}, //TVAR_ash_pGAS[4]                                                                         
{0x0F12,0x00CC}, //TVAR_ash_pGAS[5]                                                                         
{0x0F12,0x00C5}, //TVAR_ash_pGAS[6]                                                                         
{0x0F12,0x00CF}, //TVAR_ash_pGAS[7]                                                                         
{0x0F12,0x00E8}, //TVAR_ash_pGAS[8]                                                                         
{0x0F12,0x0111}, //TVAR_ash_pGAS[9]                                                                         
{0x0F12,0x0142}, //TVAR_ash_pGAS[10]                                                                        
{0x0F12,0x0183}, //TVAR_ash_pGAS[11]                                                                        
{0x0F12,0x01D9}, //TVAR_ash_pGAS[12]                                                                        
{0x0F12,0x0184}, //TVAR_ash_pGAS[13]                                                                        
{0x0F12,0x0142}, //TVAR_ash_pGAS[14]                                                                        
{0x0F12,0x0101}, //TVAR_ash_pGAS[15]                                                                        
{0x0F12,0x00CF}, //TVAR_ash_pGAS[16]                                                                        
{0x0F12,0x00A7}, //TVAR_ash_pGAS[17]                                                                        
{0x0F12,0x0090}, //TVAR_ash_pGAS[18]                                                                        
{0x0F12,0x0088}, //TVAR_ash_pGAS[19]                                                                        
{0x0F12,0x0092}, //TVAR_ash_pGAS[20]                                                                        
{0x0F12,0x00AC}, //TVAR_ash_pGAS[21]                                                                        
{0x0F12,0x00D5}, //TVAR_ash_pGAS[22]                                                                        
{0x0F12,0x010D}, //TVAR_ash_pGAS[23]                                                                        
{0x0F12,0x014E}, //TVAR_ash_pGAS[24]                                                                        
{0x0F12,0x0190}, //TVAR_ash_pGAS[25]                                                                        
{0x0F12,0x014E}, //TVAR_ash_pGAS[26]                                                                        
{0x0F12,0x010D}, //TVAR_ash_pGAS[27]                                                                        
{0x0F12,0x00CA}, //TVAR_ash_pGAS[28]                                                                        
{0x0F12,0x0094}, //TVAR_ash_pGAS[29]                                                                        
{0x0F12,0x006D}, //TVAR_ash_pGAS[30]                                                                        
{0x0F12,0x0054}, //TVAR_ash_pGAS[31]                                                                        
{0x0F12,0x004E}, //TVAR_ash_pGAS[32]                                                                        
{0x0F12,0x0058}, //TVAR_ash_pGAS[33]                                                                        
{0x0F12,0x0073}, //TVAR_ash_pGAS[34]                                                                        
{0x0F12,0x009D}, //TVAR_ash_pGAS[35]                                                                        
{0x0F12,0x00D5}, //TVAR_ash_pGAS[36]                                                                        
{0x0F12,0x011D}, //TVAR_ash_pGAS[37]                                                                        
{0x0F12,0x015A}, //TVAR_ash_pGAS[38]                                                                        
{0x0F12,0x0129}, //TVAR_ash_pGAS[39]                                                                        
{0x0F12,0x00E5}, //TVAR_ash_pGAS[40]                                                                        
{0x0F12,0x00A1}, //TVAR_ash_pGAS[41]                                                                        
{0x0F12,0x006B}, //TVAR_ash_pGAS[42]                                                                        
{0x0F12,0x0042}, //TVAR_ash_pGAS[43]                                                                        
{0x0F12,0x002A}, //TVAR_ash_pGAS[44]                                                                        
{0x0F12,0x0022}, //TVAR_ash_pGAS[45]                                                                        
{0x0F12,0x002D}, //TVAR_ash_pGAS[46]                                                                        
{0x0F12,0x0049}, //TVAR_ash_pGAS[47]                                                                        
{0x0F12,0x0075}, //TVAR_ash_pGAS[48]                                                                        
{0x0F12,0x00AF}, //TVAR_ash_pGAS[49]                                                                        
{0x0F12,0x00F8}, //TVAR_ash_pGAS[50]                                                                        
{0x0F12,0x013A}, //TVAR_ash_pGAS[51]                                                                        
{0x0F12,0x0113}, //TVAR_ash_pGAS[52]                                                                        
{0x0F12,0x00CE}, //TVAR_ash_pGAS[53]                                                                        
{0x0F12,0x0088}, //TVAR_ash_pGAS[54]                                                                        
{0x0F12,0x0052}, //TVAR_ash_pGAS[55]                                                                        
{0x0F12,0x002A}, //TVAR_ash_pGAS[56]                                                                        
{0x0F12,0x0010}, //TVAR_ash_pGAS[57]                                                                        
{0x0F12,0x0009}, //TVAR_ash_pGAS[58]                                                                        
{0x0F12,0x0015}, //TVAR_ash_pGAS[59]                                                                        
{0x0F12,0x0032}, //TVAR_ash_pGAS[60]                                                                        
{0x0F12,0x005E}, //TVAR_ash_pGAS[61]                                                                        
{0x0F12,0x0098}, //TVAR_ash_pGAS[62]                                                                        
{0x0F12,0x00E3}, //TVAR_ash_pGAS[63]                                                                        
{0x0F12,0x0128}, //TVAR_ash_pGAS[64]                                                                        
{0x0F12,0x010A}, //TVAR_ash_pGAS[65]                                                                        
{0x0F12,0x00C4}, //TVAR_ash_pGAS[66]                                                                        
{0x0F12,0x0080}, //TVAR_ash_pGAS[67]                                                                        
{0x0F12,0x0049}, //TVAR_ash_pGAS[68]                                                                        
{0x0F12,0x0020}, //TVAR_ash_pGAS[69]                                                                        
{0x0F12,0x0008}, //TVAR_ash_pGAS[70]                                                                        
{0x0F12,0x0000}, //TVAR_ash_pGAS[71]                                                                        
{0x0F12,0x000D}, //TVAR_ash_pGAS[72]                                                                        
{0x0F12,0x002A}, //TVAR_ash_pGAS[73]                                                                        
{0x0F12,0x0058}, //TVAR_ash_pGAS[74]                                                                        
{0x0F12,0x0093}, //TVAR_ash_pGAS[75]                                                                        
{0x0F12,0x00DD}, //TVAR_ash_pGAS[76]                                                                        
{0x0F12,0x0123}, //TVAR_ash_pGAS[77]                                                                        
{0x0F12,0x010D}, //TVAR_ash_pGAS[78]                                                                        
{0x0F12,0x00CA}, //TVAR_ash_pGAS[79]                                                                        
{0x0F12,0x0085}, //TVAR_ash_pGAS[80]                                                                        
{0x0F12,0x004E}, //TVAR_ash_pGAS[81]                                                                        
{0x0F12,0x0026}, //TVAR_ash_pGAS[82]                                                                        
{0x0F12,0x000E}, //TVAR_ash_pGAS[83]                                                                        
{0x0F12,0x0007}, //TVAR_ash_pGAS[84]                                                                        
{0x0F12,0x0014}, //TVAR_ash_pGAS[85]                                                                        
{0x0F12,0x0032}, //TVAR_ash_pGAS[86]                                                                        
{0x0F12,0x0061}, //TVAR_ash_pGAS[87]                                                                        
{0x0F12,0x009C}, //TVAR_ash_pGAS[88]                                                                        
{0x0F12,0x00E8}, //TVAR_ash_pGAS[89]                                                                        
{0x0F12,0x012F}, //TVAR_ash_pGAS[90]                                                                        
{0x0F12,0x0121}, //TVAR_ash_pGAS[91]                                                                        
{0x0F12,0x00DE}, //TVAR_ash_pGAS[92]                                                                        
{0x0F12,0x009A}, //TVAR_ash_pGAS[93]                                                                        
{0x0F12,0x0063}, //TVAR_ash_pGAS[94]                                                                        
{0x0F12,0x003B}, //TVAR_ash_pGAS[95]                                                                        
{0x0F12,0x0024}, //TVAR_ash_pGAS[96]                                                                        
{0x0F12,0x001D}, //TVAR_ash_pGAS[97]                                                                        
{0x0F12,0x002B}, //TVAR_ash_pGAS[98]                                                                        
{0x0F12,0x0049}, //TVAR_ash_pGAS[99]                                                                        
{0x0F12,0x0079}, //TVAR_ash_pGAS[100]                                                                       
{0x0F12,0x00B5}, //TVAR_ash_pGAS[101]                                                                       
{0x0F12,0x0100}, //TVAR_ash_pGAS[102]                                                                       
{0x0F12,0x0145}, //TVAR_ash_pGAS[103]                                                                       
{0x0F12,0x013F}, //TVAR_ash_pGAS[104]                                                                       
{0x0F12,0x0101}, //TVAR_ash_pGAS[105]                                                                       
{0x0F12,0x00BE}, //TVAR_ash_pGAS[106]                                                                       
{0x0F12,0x0087}, //TVAR_ash_pGAS[107]                                                                       
{0x0F12,0x005F}, //TVAR_ash_pGAS[108]                                                                       
{0x0F12,0x0048}, //TVAR_ash_pGAS[109]                                                                       
{0x0F12,0x0043}, //TVAR_ash_pGAS[110]                                                                       
{0x0F12,0x0051}, //TVAR_ash_pGAS[111]                                                                       
{0x0F12,0x0070}, //TVAR_ash_pGAS[112]                                                                       
{0x0F12,0x00A0}, //TVAR_ash_pGAS[113]                                                                       
{0x0F12,0x00DF}, //TVAR_ash_pGAS[114]                                                                       
{0x0F12,0x0126}, //TVAR_ash_pGAS[115]                                                                       
{0x0F12,0x0168}, //TVAR_ash_pGAS[116]                                                                       
{0x0F12,0x016D}, //TVAR_ash_pGAS[117]                                                                       
{0x0F12,0x012F}, //TVAR_ash_pGAS[118]                                                                       
{0x0F12,0x00EF}, //TVAR_ash_pGAS[119]                                                                       
{0x0F12,0x00BA}, //TVAR_ash_pGAS[120]                                                                       
{0x0F12,0x0093}, //TVAR_ash_pGAS[121]                                                                       
{0x0F12,0x007E}, //TVAR_ash_pGAS[122]                                                                       
{0x0F12,0x0079}, //TVAR_ash_pGAS[123]                                                                       
{0x0F12,0x0087}, //TVAR_ash_pGAS[124]                                                                       
{0x0F12,0x00A6}, //TVAR_ash_pGAS[125]                                                                       
{0x0F12,0x00D7}, //TVAR_ash_pGAS[126]                                                                       
{0x0F12,0x0114}, //TVAR_ash_pGAS[127]                                                                       
{0x0F12,0x0158}, //TVAR_ash_pGAS[128]                                                                       
{0x0F12,0x0199}, //TVAR_ash_pGAS[129]                                                                       
{0x0F12,0x01A6}, //TVAR_ash_pGAS[130]                                                                       
{0x0F12,0x015E}, //TVAR_ash_pGAS[131]                                                                       
{0x0F12,0x0122}, //TVAR_ash_pGAS[132]                                                                       
{0x0F12,0x00F1}, //TVAR_ash_pGAS[133]                                                                       
{0x0F12,0x00CB}, //TVAR_ash_pGAS[134]                                                                       
{0x0F12,0x00B6}, //TVAR_ash_pGAS[135]                                                                       
{0x0F12,0x00B2}, //TVAR_ash_pGAS[136]                                                                       
{0x0F12,0x00C0}, //TVAR_ash_pGAS[137]                                                                       
{0x0F12,0x00DF}, //TVAR_ash_pGAS[138]                                                                       
{0x0F12,0x010D}, //TVAR_ash_pGAS[139]                                                                       
{0x0F12,0x0145}, //TVAR_ash_pGAS[140]                                                                       
{0x0F12,0x0188}, //TVAR_ash_pGAS[141]                                                                       
{0x0F12,0x01DF}, //TVAR_ash_pGAS[142]                                                                       
{0x0F12,0x016C}, //TVAR_ash_pGAS[143]                                                                       
{0x0F12,0x0127}, //TVAR_ash_pGAS[144]                                                                       
{0x0F12,0x00F2}, //TVAR_ash_pGAS[145]                                                                       
{0x0F12,0x00CA}, //TVAR_ash_pGAS[146]                                                                       
{0x0F12,0x00AC}, //TVAR_ash_pGAS[147]                                                                       
{0x0F12,0x009B}, //TVAR_ash_pGAS[148]                                                                       
{0x0F12,0x0096}, //TVAR_ash_pGAS[149]                                                                       
{0x0F12,0x009C}, //TVAR_ash_pGAS[150]                                                                       
{0x0F12,0x00AE}, //TVAR_ash_pGAS[151]                                                                       
{0x0F12,0x00CC}, //TVAR_ash_pGAS[152]                                                                       
{0x0F12,0x00F4}, //TVAR_ash_pGAS[153]                                                                       
{0x0F12,0x012D}, //TVAR_ash_pGAS[154]                                                                       
{0x0F12,0x0179}, //TVAR_ash_pGAS[155]                                                                       
{0x0F12,0x0130}, //TVAR_ash_pGAS[156]                                                                       
{0x0F12,0x00F6}, //TVAR_ash_pGAS[157]                                                                       
{0x0F12,0x00C4}, //TVAR_ash_pGAS[158]                                                                       
{0x0F12,0x0099}, //TVAR_ash_pGAS[159]                                                                       
{0x0F12,0x007C}, //TVAR_ash_pGAS[160]                                                                       
{0x0F12,0x006C}, //TVAR_ash_pGAS[161]                                                                       
{0x0F12,0x0067}, //TVAR_ash_pGAS[162]                                                                       
{0x0F12,0x006E}, //TVAR_ash_pGAS[163]                                                                       
{0x0F12,0x007F}, //TVAR_ash_pGAS[164]                                                                       
{0x0F12,0x009E}, //TVAR_ash_pGAS[165]                                                                       
{0x0F12,0x00C8}, //TVAR_ash_pGAS[166]                                                                       
{0x0F12,0x0100}, //TVAR_ash_pGAS[167]                                                                       
{0x0F12,0x0138}, //TVAR_ash_pGAS[168]                                                                       
{0x0F12,0x0107}, //TVAR_ash_pGAS[169]                                                                       
{0x0F12,0x00CF}, //TVAR_ash_pGAS[170]                                                                       
{0x0F12,0x0097}, //TVAR_ash_pGAS[171]                                                                       
{0x0F12,0x006D}, //TVAR_ash_pGAS[172]                                                                       
{0x0F12,0x0050}, //TVAR_ash_pGAS[173]                                                                       
{0x0F12,0x0040}, //TVAR_ash_pGAS[174]                                                                       
{0x0F12,0x003B}, //TVAR_ash_pGAS[175]                                                                       
{0x0F12,0x0042}, //TVAR_ash_pGAS[176]                                                                       
{0x0F12,0x0055}, //TVAR_ash_pGAS[177]                                                                       
{0x0F12,0x0074}, //TVAR_ash_pGAS[178]                                                                       
{0x0F12,0x009F}, //TVAR_ash_pGAS[179]                                                                       
{0x0F12,0x00D9}, //TVAR_ash_pGAS[180]                                                                       
{0x0F12,0x0110}, //TVAR_ash_pGAS[181]                                                                       
{0x0F12,0x00E9}, //TVAR_ash_pGAS[182]                                                                       
{0x0F12,0x00AE}, //TVAR_ash_pGAS[183]                                                                       
{0x0F12,0x0077}, //TVAR_ash_pGAS[184]                                                                       
{0x0F12,0x004D}, //TVAR_ash_pGAS[185]                                                                       
{0x0F12,0x002F}, //TVAR_ash_pGAS[186]                                                                       
{0x0F12,0x001F}, //TVAR_ash_pGAS[187]                                                                       
{0x0F12,0x001A}, //TVAR_ash_pGAS[188]                                                                       
{0x0F12,0x0022}, //TVAR_ash_pGAS[189]                                                                       
{0x0F12,0x0036}, //TVAR_ash_pGAS[190]                                                                       
{0x0F12,0x0055}, //TVAR_ash_pGAS[191]                                                                       
{0x0F12,0x0081}, //TVAR_ash_pGAS[192]                                                                       
{0x0F12,0x00BC}, //TVAR_ash_pGAS[193]                                                                       
{0x0F12,0x00F5}, //TVAR_ash_pGAS[194]                                                                       
{0x0F12,0x00D8}, //TVAR_ash_pGAS[195]                                                                       
{0x0F12,0x009C}, //TVAR_ash_pGAS[196]                                                                       
{0x0F12,0x0064}, //TVAR_ash_pGAS[197]                                                                       
{0x0F12,0x003A}, //TVAR_ash_pGAS[198]                                                                       
{0x0F12,0x001C}, //TVAR_ash_pGAS[199]                                                                       
{0x0F12,0x000B}, //TVAR_ash_pGAS[200]                                                                       
{0x0F12,0x0006}, //TVAR_ash_pGAS[201]                                                                       
{0x0F12,0x000F}, //TVAR_ash_pGAS[202]                                                                       
{0x0F12,0x0024}, //TVAR_ash_pGAS[203]                                                                       
{0x0F12,0x0044}, //TVAR_ash_pGAS[204]                                                                       
{0x0F12,0x0070}, //TVAR_ash_pGAS[205]                                                                       
{0x0F12,0x00AD}, //TVAR_ash_pGAS[206]                                                                       
{0x0F12,0x00E6}, //TVAR_ash_pGAS[207]                                                                       
{0x0F12,0x00D0}, //TVAR_ash_pGAS[208]                                                                       
{0x0F12,0x0095}, //TVAR_ash_pGAS[209]                                                                       
{0x0F12,0x005D}, //TVAR_ash_pGAS[210]                                                                       
{0x0F12,0x0033}, //TVAR_ash_pGAS[211]                                                                       
{0x0F12,0x0015}, //TVAR_ash_pGAS[212]                                                                       
{0x0F12,0x0005}, //TVAR_ash_pGAS[213]                                                                       
{0x0F12,0x0000}, //TVAR_ash_pGAS[214]                                                                       
{0x0F12,0x0009}, //TVAR_ash_pGAS[215]                                                                       
{0x0F12,0x001E}, //TVAR_ash_pGAS[216]                                                                       
{0x0F12,0x0041}, //TVAR_ash_pGAS[217]                                                                       
{0x0F12,0x006D}, //TVAR_ash_pGAS[218]                                                                       
{0x0F12,0x00AA}, //TVAR_ash_pGAS[219]                                                                       
{0x0F12,0x00E4}, //TVAR_ash_pGAS[220]                                                                       
{0x0F12,0x00D6}, //TVAR_ash_pGAS[221]                                                                       
{0x0F12,0x009A}, //TVAR_ash_pGAS[222]                                                                       
{0x0F12,0x0062}, //TVAR_ash_pGAS[223]                                                                       
{0x0F12,0x0038}, //TVAR_ash_pGAS[224]                                                                       
{0x0F12,0x001B}, //TVAR_ash_pGAS[225]                                                                       
{0x0F12,0x000A}, //TVAR_ash_pGAS[226]                                                                       
{0x0F12,0x0006}, //TVAR_ash_pGAS[227]                                                                       
{0x0F12,0x0010}, //TVAR_ash_pGAS[228]                                                                       
{0x0F12,0x0026}, //TVAR_ash_pGAS[229]                                                                       
{0x0F12,0x0049}, //TVAR_ash_pGAS[230]                                                                       
{0x0F12,0x0076}, //TVAR_ash_pGAS[231]                                                                       
{0x0F12,0x00B4}, //TVAR_ash_pGAS[232]                                                                       
{0x0F12,0x00ED}, //TVAR_ash_pGAS[233]                                                                       
{0x0F12,0x00E4}, //TVAR_ash_pGAS[234]                                                                       
{0x0F12,0x00AB}, //TVAR_ash_pGAS[235]                                                                       
{0x0F12,0x0072}, //TVAR_ash_pGAS[236]                                                                       
{0x0F12,0x0049}, //TVAR_ash_pGAS[237]                                                                       
{0x0F12,0x002C}, //TVAR_ash_pGAS[238]                                                                       
{0x0F12,0x001C}, //TVAR_ash_pGAS[239]                                                                       
{0x0F12,0x0019}, //TVAR_ash_pGAS[240]                                                                       
{0x0F12,0x0023}, //TVAR_ash_pGAS[241]                                                                       
{0x0F12,0x003A}, //TVAR_ash_pGAS[242]                                                                       
{0x0F12,0x005D}, //TVAR_ash_pGAS[243]                                                                       
{0x0F12,0x008B}, //TVAR_ash_pGAS[244]                                                                       
{0x0F12,0x00C8}, //TVAR_ash_pGAS[245]                                                                       
{0x0F12,0x00FF}, //TVAR_ash_pGAS[246]                                                                       
{0x0F12,0x00FF}, //TVAR_ash_pGAS[247]                                                                       
{0x0F12,0x00C8}, //TVAR_ash_pGAS[248]                                                                       
{0x0F12,0x0090}, //TVAR_ash_pGAS[249]                                                                       
{0x0F12,0x0066}, //TVAR_ash_pGAS[250]                                                                       
{0x0F12,0x004A}, //TVAR_ash_pGAS[251]                                                                       
{0x0F12,0x003A}, //TVAR_ash_pGAS[252]                                                                       
{0x0F12,0x0038}, //TVAR_ash_pGAS[253]                                                                       
{0x0F12,0x0042}, //TVAR_ash_pGAS[254]                                                                       
{0x0F12,0x0059}, //TVAR_ash_pGAS[255]                                                                       
{0x0F12,0x007C}, //TVAR_ash_pGAS[256]                                                                       
{0x0F12,0x00AB}, //TVAR_ash_pGAS[257]                                                                       
{0x0F12,0x00E7}, //TVAR_ash_pGAS[258]                                                                       
{0x0F12,0x011D}, //TVAR_ash_pGAS[259]                                                                       
{0x0F12,0x0123}, //TVAR_ash_pGAS[260]                                                                       
{0x0F12,0x00EC}, //TVAR_ash_pGAS[261]                                                                       
{0x0F12,0x00B7}, //TVAR_ash_pGAS[262]                                                                       
{0x0F12,0x008E}, //TVAR_ash_pGAS[263]                                                                       
{0x0F12,0x0073}, //TVAR_ash_pGAS[264]                                                                       
{0x0F12,0x0064}, //TVAR_ash_pGAS[265]                                                                       
{0x0F12,0x0062}, //TVAR_ash_pGAS[266]                                                                       
{0x0F12,0x006D}, //TVAR_ash_pGAS[267]                                                                       
{0x0F12,0x0083}, //TVAR_ash_pGAS[268]                                                                       
{0x0F12,0x00A7}, //TVAR_ash_pGAS[269]                                                                       
{0x0F12,0x00D4}, //TVAR_ash_pGAS[270]                                                                       
{0x0F12,0x010B}, //TVAR_ash_pGAS[271]                                                                       
{0x0F12,0x0144}, //TVAR_ash_pGAS[272]                                                                       
{0x0F12,0x0156}, //TVAR_ash_pGAS[273]                                                                       
{0x0F12,0x0114}, //TVAR_ash_pGAS[274]                                                                       
{0x0F12,0x00E2}, //TVAR_ash_pGAS[275]                                                                       
{0x0F12,0x00BB}, //TVAR_ash_pGAS[276]                                                                       
{0x0F12,0x009F}, //TVAR_ash_pGAS[277]                                                                       
{0x0F12,0x0090}, //TVAR_ash_pGAS[278]                                                                       
{0x0F12,0x008E}, //TVAR_ash_pGAS[279]                                                                       
{0x0F12,0x0099}, //TVAR_ash_pGAS[280]                                                                       
{0x0F12,0x00B0}, //TVAR_ash_pGAS[281]                                                                       
{0x0F12,0x00D2}, //TVAR_ash_pGAS[282]                                                                       
{0x0F12,0x00FE}, //TVAR_ash_pGAS[283]                                                                       
{0x0F12,0x0133}, //TVAR_ash_pGAS[284]                                                                       
{0x0F12,0x017D}, //TVAR_ash_pGAS[285]                                                                       
{0x0F12,0x0174}, //TVAR_ash_pGAS[286]                                                                       
{0x0F12,0x012A}, //TVAR_ash_pGAS[287]                                                                       
{0x0F12,0x00F6}, //TVAR_ash_pGAS[288]                                                                       
{0x0F12,0x00CC}, //TVAR_ash_pGAS[289]                                                                       
{0x0F12,0x00AD}, //TVAR_ash_pGAS[290]                                                                       
{0x0F12,0x009C}, //TVAR_ash_pGAS[291]                                                                       
{0x0F12,0x0099}, //TVAR_ash_pGAS[292]                                                                       
{0x0F12,0x00A4}, //TVAR_ash_pGAS[293]                                                                       
{0x0F12,0x00BC}, //TVAR_ash_pGAS[294]                                                                       
{0x0F12,0x00E0}, //TVAR_ash_pGAS[295]                                                                       
{0x0F12,0x010E}, //TVAR_ash_pGAS[296]                                                                       
{0x0F12,0x0147}, //TVAR_ash_pGAS[297]                                                                       
{0x0F12,0x0193}, //TVAR_ash_pGAS[298]                                                                       
{0x0F12,0x013A}, //TVAR_ash_pGAS[299]                                                                       
{0x0F12,0x00FE}, //TVAR_ash_pGAS[300]                                                                       
{0x0F12,0x00C9}, //TVAR_ash_pGAS[301]                                                                       
{0x0F12,0x009E}, //TVAR_ash_pGAS[302]                                                                       
{0x0F12,0x007E}, //TVAR_ash_pGAS[303]                                                                       
{0x0F12,0x006E}, //TVAR_ash_pGAS[304]                                                                       
{0x0F12,0x006B}, //TVAR_ash_pGAS[305]                                                                       
{0x0F12,0x0075}, //TVAR_ash_pGAS[306]                                                                       
{0x0F12,0x008D}, //TVAR_ash_pGAS[307]                                                                       
{0x0F12,0x00B2}, //TVAR_ash_pGAS[308]                                                                       
{0x0F12,0x00E0}, //TVAR_ash_pGAS[309]                                                                       
{0x0F12,0x011B}, //TVAR_ash_pGAS[310]                                                                       
{0x0F12,0x0152}, //TVAR_ash_pGAS[311]                                                                       
{0x0F12,0x0112}, //TVAR_ash_pGAS[312]                                                                       
{0x0F12,0x00D8}, //TVAR_ash_pGAS[313]                                                                       
{0x0F12,0x009F}, //TVAR_ash_pGAS[314]                                                                       
{0x0F12,0x0073}, //TVAR_ash_pGAS[315]                                                                       
{0x0F12,0x0054}, //TVAR_ash_pGAS[316]                                                                       
{0x0F12,0x0042}, //TVAR_ash_pGAS[317]                                                                       
{0x0F12,0x003F}, //TVAR_ash_pGAS[318]                                                                       
{0x0F12,0x0049}, //TVAR_ash_pGAS[319]                                                                       
{0x0F12,0x0061}, //TVAR_ash_pGAS[320]                                                                       
{0x0F12,0x0085}, //TVAR_ash_pGAS[321]                                                                       
{0x0F12,0x00B5}, //TVAR_ash_pGAS[322]                                                                       
{0x0F12,0x00F2}, //TVAR_ash_pGAS[323]                                                                       
{0x0F12,0x0128}, //TVAR_ash_pGAS[324]                                                                       
{0x0F12,0x00F5}, //TVAR_ash_pGAS[325]                                                                       
{0x0F12,0x00BA}, //TVAR_ash_pGAS[326]                                                                       
{0x0F12,0x0080}, //TVAR_ash_pGAS[327]                                                                       
{0x0F12,0x0054}, //TVAR_ash_pGAS[328]                                                                       
{0x0F12,0x0034}, //TVAR_ash_pGAS[329]                                                                       
{0x0F12,0x0022}, //TVAR_ash_pGAS[330]                                                                       
{0x0F12,0x001D}, //TVAR_ash_pGAS[331]                                                                       
{0x0F12,0x0027}, //TVAR_ash_pGAS[332]                                                                       
{0x0F12,0x003F}, //TVAR_ash_pGAS[333]                                                                       
{0x0F12,0x0064}, //TVAR_ash_pGAS[334]                                                                       
{0x0F12,0x0092}, //TVAR_ash_pGAS[335]                                                                       
{0x0F12,0x00CF}, //TVAR_ash_pGAS[336]                                                                       
{0x0F12,0x0109}, //TVAR_ash_pGAS[337]                                                                       
{0x0F12,0x00E4}, //TVAR_ash_pGAS[338]                                                                       
{0x0F12,0x00A8}, //TVAR_ash_pGAS[339]                                                                       
{0x0F12,0x006E}, //TVAR_ash_pGAS[340]                                                                       
{0x0F12,0x0041}, //TVAR_ash_pGAS[341]                                                                       
{0x0F12,0x0021}, //TVAR_ash_pGAS[342]                                                                       
{0x0F12,0x000E}, //TVAR_ash_pGAS[343]                                                                       
{0x0F12,0x0008}, //TVAR_ash_pGAS[344]                                                                       
{0x0F12,0x0012}, //TVAR_ash_pGAS[345]                                                                       
{0x0F12,0x0029}, //TVAR_ash_pGAS[346]                                                                       
{0x0F12,0x004D}, //TVAR_ash_pGAS[347]                                                                       
{0x0F12,0x007C}, //TVAR_ash_pGAS[348]                                                                       
{0x0F12,0x00B8}, //TVAR_ash_pGAS[349]                                                                       
{0x0F12,0x00F3}, //TVAR_ash_pGAS[350]                                                                       
{0x0F12,0x00DF}, //TVAR_ash_pGAS[351]                                                                       
{0x0F12,0x00A2}, //TVAR_ash_pGAS[352]                                                                       
{0x0F12,0x0068}, //TVAR_ash_pGAS[353]                                                                       
{0x0F12,0x003B}, //TVAR_ash_pGAS[354]                                                                       
{0x0F12,0x001A}, //TVAR_ash_pGAS[355]                                                                       
{0x0F12,0x0006}, //TVAR_ash_pGAS[356]                                                                       
{0x0F12,0x0000}, //TVAR_ash_pGAS[357]                                                                       
{0x0F12,0x0009}, //TVAR_ash_pGAS[358]                                                                       
{0x0F12,0x001F}, //TVAR_ash_pGAS[359]                                                                       
{0x0F12,0x0042}, //TVAR_ash_pGAS[360]                                                                       
{0x0F12,0x0071}, //TVAR_ash_pGAS[361]                                                                       
{0x0F12,0x00AE}, //TVAR_ash_pGAS[362]                                                                       
{0x0F12,0x00E9}, //TVAR_ash_pGAS[363]                                                                       
{0x0F12,0x00E4}, //TVAR_ash_pGAS[364]                                                                       
{0x0F12,0x00A7}, //TVAR_ash_pGAS[365]                                                                       
{0x0F12,0x006C}, //TVAR_ash_pGAS[366]                                                                       
{0x0F12,0x003F}, //TVAR_ash_pGAS[367]                                                                       
{0x0F12,0x001E}, //TVAR_ash_pGAS[368]                                                                       
{0x0F12,0x000B}, //TVAR_ash_pGAS[369]                                                                       
{0x0F12,0x0004}, //TVAR_ash_pGAS[370]                                                                       
{0x0F12,0x000D}, //TVAR_ash_pGAS[371]                                                                       
{0x0F12,0x0022}, //TVAR_ash_pGAS[372]                                                                       
{0x0F12,0x0044}, //TVAR_ash_pGAS[373]                                                                       
{0x0F12,0x0072}, //TVAR_ash_pGAS[374]                                                                       
{0x0F12,0x00AE}, //TVAR_ash_pGAS[375]                                                                       
{0x0F12,0x00EA}, //TVAR_ash_pGAS[376]                                                                       
{0x0F12,0x00F5}, //TVAR_ash_pGAS[377]                                                                       
{0x0F12,0x00B9}, //TVAR_ash_pGAS[378]                                                                       
{0x0F12,0x007D}, //TVAR_ash_pGAS[379]                                                                       
{0x0F12,0x0051}, //TVAR_ash_pGAS[380]                                                                       
{0x0F12,0x002F}, //TVAR_ash_pGAS[381]                                                                       
{0x0F12,0x001C}, //TVAR_ash_pGAS[382]                                                                       
{0x0F12,0x0015}, //TVAR_ash_pGAS[383]                                                                       
{0x0F12,0x001D}, //TVAR_ash_pGAS[384]                                                                       
{0x0F12,0x0031}, //TVAR_ash_pGAS[385]                                                                       
{0x0F12,0x0053}, //TVAR_ash_pGAS[386]                                                                       
{0x0F12,0x0080}, //TVAR_ash_pGAS[387]                                                                       
{0x0F12,0x00BC}, //TVAR_ash_pGAS[388]                                                                       
{0x0F12,0x00F7}, //TVAR_ash_pGAS[389]                                                                       
{0x0F12,0x0111}, //TVAR_ash_pGAS[390]                                                                       
{0x0F12,0x00D6}, //TVAR_ash_pGAS[391]                                                                       
{0x0F12,0x009C}, //TVAR_ash_pGAS[392]                                                                       
{0x0F12,0x006F}, //TVAR_ash_pGAS[393]                                                                       
{0x0F12,0x004E}, //TVAR_ash_pGAS[394]                                                                       
{0x0F12,0x003A}, //TVAR_ash_pGAS[395]                                                                       
{0x0F12,0x0033}, //TVAR_ash_pGAS[396]                                                                       
{0x0F12,0x003A}, //TVAR_ash_pGAS[397]                                                                       
{0x0F12,0x004E}, //TVAR_ash_pGAS[398]                                                                       
{0x0F12,0x006E}, //TVAR_ash_pGAS[399]                                                                       
{0x0F12,0x009B}, //TVAR_ash_pGAS[400]                                                                       
{0x0F12,0x00D5}, //TVAR_ash_pGAS[401]                                                                       
{0x0F12,0x010F}, //TVAR_ash_pGAS[402]                                                                       
{0x0F12,0x0139}, //TVAR_ash_pGAS[403]                                                                       
{0x0F12,0x00FD}, //TVAR_ash_pGAS[404]                                                                       
{0x0F12,0x00C6}, //TVAR_ash_pGAS[405]                                                                       
{0x0F12,0x0098}, //TVAR_ash_pGAS[406]                                                                       
{0x0F12,0x0077}, //TVAR_ash_pGAS[407]                                                                       
{0x0F12,0x0064}, //TVAR_ash_pGAS[408]                                                                       
{0x0F12,0x005D}, //TVAR_ash_pGAS[409]                                                                       
{0x0F12,0x0064}, //TVAR_ash_pGAS[410]                                                                       
{0x0F12,0x0076}, //TVAR_ash_pGAS[411]                                                                       
{0x0F12,0x0095}, //TVAR_ash_pGAS[412]                                                                       
{0x0F12,0x00C2}, //TVAR_ash_pGAS[413]                                                                       
{0x0F12,0x00F8}, //TVAR_ash_pGAS[414]                                                                       
{0x0F12,0x0135}, //TVAR_ash_pGAS[415]                                                                       
{0x0F12,0x016C}, //TVAR_ash_pGAS[416]                                                                       
{0x0F12,0x0128}, //TVAR_ash_pGAS[417]                                                                       
{0x0F12,0x00F2}, //TVAR_ash_pGAS[418]                                                                       
{0x0F12,0x00C7}, //TVAR_ash_pGAS[419]                                                                       
{0x0F12,0x00A4}, //TVAR_ash_pGAS[420]                                                                       
{0x0F12,0x0092}, //TVAR_ash_pGAS[421]                                                                       
{0x0F12,0x008A}, //TVAR_ash_pGAS[422]                                                                       
{0x0F12,0x008F}, //TVAR_ash_pGAS[423]                                                                       
{0x0F12,0x00A3}, //TVAR_ash_pGAS[424]                                                                       
{0x0F12,0x00C0}, //TVAR_ash_pGAS[425]                                                                       
{0x0F12,0x00EA}, //TVAR_ash_pGAS[426]                                                                       
{0x0F12,0x0121}, //TVAR_ash_pGAS[427]                                                                       
{0x0F12,0x016F}, //TVAR_ash_pGAS[428]                                                                       
{0x0F12,0x0123}, //TVAR_ash_pGAS[429]                                                                       
{0x0F12,0x00E7}, //TVAR_ash_pGAS[430]                                                                       
{0x0F12,0x00BD}, //TVAR_ash_pGAS[431]                                                                       
{0x0F12,0x009C}, //TVAR_ash_pGAS[432]                                                                       
{0x0F12,0x0087}, //TVAR_ash_pGAS[433]                                                                       
{0x0F12,0x007C}, //TVAR_ash_pGAS[434]                                                                       
{0x0F12,0x007B}, //TVAR_ash_pGAS[435]                                                                       
{0x0F12,0x0086}, //TVAR_ash_pGAS[436]                                                                       
{0x0F12,0x0099}, //TVAR_ash_pGAS[437]                                                                       
{0x0F12,0x00B7}, //TVAR_ash_pGAS[438]                                                                       
{0x0F12,0x00DC}, //TVAR_ash_pGAS[439]                                                                       
{0x0F12,0x010E}, //TVAR_ash_pGAS[440]                                                                       
{0x0F12,0x014A}, //TVAR_ash_pGAS[441]                                                                       
{0x0F12,0x00F1}, //TVAR_ash_pGAS[442]                                                                       
{0x0F12,0x00C1}, //TVAR_ash_pGAS[443]                                                                       
{0x0F12,0x0096}, //TVAR_ash_pGAS[444]                                                                       
{0x0F12,0x0077}, //TVAR_ash_pGAS[445]                                                                       
{0x0F12,0x0062}, //TVAR_ash_pGAS[446]                                                                       
{0x0F12,0x0058}, //TVAR_ash_pGAS[447]                                                                       
{0x0F12,0x0057}, //TVAR_ash_pGAS[448]                                                                       
{0x0F12,0x0061}, //TVAR_ash_pGAS[449]                                                                       
{0x0F12,0x0074}, //TVAR_ash_pGAS[450]                                                                       
{0x0F12,0x0090}, //TVAR_ash_pGAS[451]                                                                       
{0x0F12,0x00B7}, //TVAR_ash_pGAS[452]                                                                       
{0x0F12,0x00E7}, //TVAR_ash_pGAS[453]                                                                       
{0x0F12,0x0113}, //TVAR_ash_pGAS[454]                                                                       
{0x0F12,0x00CB}, //TVAR_ash_pGAS[455]                                                                       
{0x0F12,0x009D}, //TVAR_ash_pGAS[456]                                                                       
{0x0F12,0x0071}, //TVAR_ash_pGAS[457]                                                                       
{0x0F12,0x0052}, //TVAR_ash_pGAS[458]                                                                       
{0x0F12,0x0040}, //TVAR_ash_pGAS[459]                                                                       
{0x0F12,0x0035}, //TVAR_ash_pGAS[460]                                                                       
{0x0F12,0x0034}, //TVAR_ash_pGAS[461]                                                                       
{0x0F12,0x003D}, //TVAR_ash_pGAS[462]                                                                       
{0x0F12,0x004F}, //TVAR_ash_pGAS[463]                                                                       
{0x0F12,0x006B}, //TVAR_ash_pGAS[464]                                                                       
{0x0F12,0x0090}, //TVAR_ash_pGAS[465]                                                                       
{0x0F12,0x00C2}, //TVAR_ash_pGAS[466]                                                                       
{0x0F12,0x00EC}, //TVAR_ash_pGAS[467]                                                                       
{0x0F12,0x00B0}, //TVAR_ash_pGAS[468]                                                                       
{0x0F12,0x0082}, //TVAR_ash_pGAS[469]                                                                       
{0x0F12,0x0057}, //TVAR_ash_pGAS[470]                                                                       
{0x0F12,0x003A}, //TVAR_ash_pGAS[471]                                                                       
{0x0F12,0x0026}, //TVAR_ash_pGAS[472]                                                                       
{0x0F12,0x001B}, //TVAR_ash_pGAS[473]                                                                       
{0x0F12,0x0019}, //TVAR_ash_pGAS[474]                                                                       
{0x0F12,0x0021}, //TVAR_ash_pGAS[475]                                                                       
{0x0F12,0x0033}, //TVAR_ash_pGAS[476]                                                                       
{0x0F12,0x004F}, //TVAR_ash_pGAS[477]                                                                       
{0x0F12,0x0072}, //TVAR_ash_pGAS[478]                                                                       
{0x0F12,0x00A2}, //TVAR_ash_pGAS[479]                                                                       
{0x0F12,0x00CF}, //TVAR_ash_pGAS[480]                                                                       
{0x0F12,0x009F}, //TVAR_ash_pGAS[481]                                                                       
{0x0F12,0x0072}, //TVAR_ash_pGAS[482]                                                                       
{0x0F12,0x0047}, //TVAR_ash_pGAS[483]                                                                       
{0x0F12,0x002A}, //TVAR_ash_pGAS[484]                                                                       
{0x0F12,0x0016}, //TVAR_ash_pGAS[485]                                                                       
{0x0F12,0x000A}, //TVAR_ash_pGAS[486]                                                                       
{0x0F12,0x0008}, //TVAR_ash_pGAS[487]                                                                       
{0x0F12,0x000F}, //TVAR_ash_pGAS[488]                                                                       
{0x0F12,0x0021}, //TVAR_ash_pGAS[489]                                                                       
{0x0F12,0x003A}, //TVAR_ash_pGAS[490]                                                                       
{0x0F12,0x005C}, //TVAR_ash_pGAS[491]                                                                       
{0x0F12,0x008C}, //TVAR_ash_pGAS[492]                                                                       
{0x0F12,0x00BB}, //TVAR_ash_pGAS[493]                                                                       
{0x0F12,0x009A}, //TVAR_ash_pGAS[494]                                                                       
{0x0F12,0x006C}, //TVAR_ash_pGAS[495]                                                                       
{0x0F12,0x0042}, //TVAR_ash_pGAS[496]                                                                       
{0x0F12,0x0024}, //TVAR_ash_pGAS[497]                                                                       
{0x0F12,0x0010}, //TVAR_ash_pGAS[498]                                                                       
{0x0F12,0x0004}, //TVAR_ash_pGAS[499]                                                                       
{0x0F12,0x0000}, //TVAR_ash_pGAS[500]                                                                       
{0x0F12,0x0007}, //TVAR_ash_pGAS[501]                                                                       
{0x0F12,0x0018}, //TVAR_ash_pGAS[502]                                                                       
{0x0F12,0x0030}, //TVAR_ash_pGAS[503]                                                                       
{0x0F12,0x0050}, //TVAR_ash_pGAS[504]                                                                       
{0x0F12,0x0080}, //TVAR_ash_pGAS[505]                                                                       
{0x0F12,0x00AF}, //TVAR_ash_pGAS[506]                                                                       
{0x0F12,0x009F}, //TVAR_ash_pGAS[507]                                                                       
{0x0F12,0x0071}, //TVAR_ash_pGAS[508]                                                                       
{0x0F12,0x0046}, //TVAR_ash_pGAS[509]                                                                       
{0x0F12,0x0028}, //TVAR_ash_pGAS[510]                                                                       
{0x0F12,0x0014}, //TVAR_ash_pGAS[511]                                                                       
{0x0F12,0x0006}, //TVAR_ash_pGAS[512]                                                                       
{0x0F12,0x0003}, //TVAR_ash_pGAS[513]                                                                       
{0x0F12,0x0009}, //TVAR_ash_pGAS[514]                                                                       
{0x0F12,0x0019}, //TVAR_ash_pGAS[515]                                                                       
{0x0F12,0x0030}, //TVAR_ash_pGAS[516]                                                                       
{0x0F12,0x0051}, //TVAR_ash_pGAS[517]                                                                       
{0x0F12,0x0080}, //TVAR_ash_pGAS[518]                                                                       
{0x0F12,0x00B0}, //TVAR_ash_pGAS[519]                                                                       
{0x0F12,0x00AD}, //TVAR_ash_pGAS[520]                                                                       
{0x0F12,0x0080}, //TVAR_ash_pGAS[521]                                                                       
{0x0F12,0x0055}, //TVAR_ash_pGAS[522]                                                                       
{0x0F12,0x0036}, //TVAR_ash_pGAS[523]                                                                       
{0x0F12,0x0021}, //TVAR_ash_pGAS[524]                                                                       
{0x0F12,0x0015}, //TVAR_ash_pGAS[525]                                                                       
{0x0F12,0x0010}, //TVAR_ash_pGAS[526]                                                                       
{0x0F12,0x0016}, //TVAR_ash_pGAS[527]                                                                       
{0x0F12,0x0024}, //TVAR_ash_pGAS[528]                                                                       
{0x0F12,0x003A}, //TVAR_ash_pGAS[529]                                                                       
{0x0F12,0x005B}, //TVAR_ash_pGAS[530]                                                                       
{0x0F12,0x008B}, //TVAR_ash_pGAS[531]                                                                       
{0x0F12,0x00BA}, //TVAR_ash_pGAS[532]                                                                       
{0x0F12,0x00C8}, //TVAR_ash_pGAS[533]                                                                       
{0x0F12,0x0099}, //TVAR_ash_pGAS[534]                                                                       
{0x0F12,0x006E}, //TVAR_ash_pGAS[535]                                                                       
{0x0F12,0x004E}, //TVAR_ash_pGAS[536]                                                                       
{0x0F12,0x003A}, //TVAR_ash_pGAS[537]                                                                       
{0x0F12,0x002D}, //TVAR_ash_pGAS[538]                                                                       
{0x0F12,0x002A}, //TVAR_ash_pGAS[539]                                                                       
{0x0F12,0x002E}, //TVAR_ash_pGAS[540]                                                                       
{0x0F12,0x003B}, //TVAR_ash_pGAS[541]                                                                       
{0x0F12,0x0051}, //TVAR_ash_pGAS[542]                                                                       
{0x0F12,0x0072}, //TVAR_ash_pGAS[543]                                                                       
{0x0F12,0x00A1}, //TVAR_ash_pGAS[544]                                                                       
{0x0F12,0x00D2}, //TVAR_ash_pGAS[545]                                                                       
{0x0F12,0x00EC}, //TVAR_ash_pGAS[546]                                                                       
{0x0F12,0x00BE}, //TVAR_ash_pGAS[547]                                                                       
{0x0F12,0x0092}, //TVAR_ash_pGAS[548]                                                                       
{0x0F12,0x0072}, //TVAR_ash_pGAS[549]                                                                       
{0x0F12,0x005C}, //TVAR_ash_pGAS[550]                                                                       
{0x0F12,0x0050}, //TVAR_ash_pGAS[551]                                                                       
{0x0F12,0x004D}, //TVAR_ash_pGAS[552]                                                                       
{0x0F12,0x0050}, //TVAR_ash_pGAS[553]                                                                       
{0x0F12,0x005D}, //TVAR_ash_pGAS[554]                                                                       
{0x0F12,0x0073}, //TVAR_ash_pGAS[555]                                                                       
{0x0F12,0x0094}, //TVAR_ash_pGAS[556]                                                                       
{0x0F12,0x00C2}, //TVAR_ash_pGAS[557]                                                                       
{0x0F12,0x00F4}, //TVAR_ash_pGAS[558]                                                                       
{0x0F12,0x011A}, //TVAR_ash_pGAS[559]                                                                       
{0x0F12,0x00E3}, //TVAR_ash_pGAS[560]                                                                       
{0x0F12,0x00B7}, //TVAR_ash_pGAS[561]                                                                       
{0x0F12,0x0097}, //TVAR_ash_pGAS[562]                                                                       
{0x0F12,0x0081}, //TVAR_ash_pGAS[563]                                                                       
{0x0F12,0x0075}, //TVAR_ash_pGAS[564]                                                                       
{0x0F12,0x006F}, //TVAR_ash_pGAS[565]                                                                       
{0x0F12,0x0074}, //TVAR_ash_pGAS[566]                                                                       
{0x0F12,0x0081}, //TVAR_ash_pGAS[567]                                                                       
{0x0F12,0x0097}, //TVAR_ash_pGAS[568]                                                                       
{0x0F12,0x00B8}, //TVAR_ash_pGAS[569]                                                                       
{0x0F12,0x00E7}, //TVAR_ash_pGAS[570]                                                                       
{0x0F12,0x0127}, //TVAR_ash_pGAS[571]                                                                                                                                                         
{0x002A,0x04CC},
{0x0F12,0x0000}, //SARR_usGammaLutRGBIndoor[0][0]                                                           
{0x0F12,0x0002}, //SARR_usGammaLutRGBIndoor[0][1]                                                           
{0x0F12,0x0008}, //SARR_usGammaLutRGBIndoor[0][2]                                                           
{0x0F12,0x0018}, //SARR_usGammaLutRGBIndoor[0][3]                                                           
{0x0F12,0x005A}, //SARR_usGammaLutRGBIndoor[0][4]                                                           
{0x0F12,0x00DF}, //SARR_usGammaLutRGBIndoor[0][5]                                                           
{0x0F12,0x013F}, //SARR_usGammaLutRGBIndoor[0][6]                                                           
{0x0F12,0x0186}, //SARR_usGammaLutRGBIndoor[0][7]                                                           
{0x0F12,0x01E6}, //SARR_usGammaLutRGBIndoor[0][8]                                                           
{0x0F12,0x0236}, //SARR_usGammaLutRGBIndoor[0][9]                                                           
{0x0F12,0x02BA}, //SARR_usGammaLutRGBIndoor[0][10]                                                          
{0x0F12,0x032A}, //SARR_usGammaLutRGBIndoor[0][11]                                                          
{0x0F12,0x0385}, //SARR_usGammaLutRGBIndoor[0][12]                                                          
{0x0F12,0x03C2}, //SARR_usGammaLutRGBIndoor[0][13]                                                          
{0x0F12,0x03EA}, //SARR_usGammaLutRGBIndoor[0][14]                                                          
{0x0F12,0x03FF}, //SARR_usGammaLutRGBIndoor[0][15]                                                          
{0x0F12,0x0000}, //SARR_usGammaLutRGBIndoor[1][0]                                                           
{0x0F12,0x0002}, //SARR_usGammaLutRGBIndoor[1][1]                                                           
{0x0F12,0x0008}, //SARR_usGammaLutRGBIndoor[1][2]                                                           
{0x0F12,0x0018}, //SARR_usGammaLutRGBIndoor[1][3]                                                           
{0x0F12,0x005A}, //SARR_usGammaLutRGBIndoor[1][4]                                                           
{0x0F12,0x00DF}, //SARR_usGammaLutRGBIndoor[1][5]                                                           
{0x0F12,0x013F}, //SARR_usGammaLutRGBIndoor[1][6]                                                           
{0x0F12,0x0186}, //SARR_usGammaLutRGBIndoor[1][7]                                                           
{0x0F12,0x01E6}, //SARR_usGammaLutRGBIndoor[1][8]                                                           
{0x0F12,0x0236}, //SARR_usGammaLutRGBIndoor[1][9]                                                           
{0x0F12,0x02BA}, //SARR_usGammaLutRGBIndoor[1][10]                                                          
{0x0F12,0x032A}, //SARR_usGammaLutRGBIndoor[1][11]                                                          
{0x0F12,0x0385}, //SARR_usGammaLutRGBIndoor[1][12]                                                          
{0x0F12,0x03C2}, //SARR_usGammaLutRGBIndoor[1][13]                                                          
{0x0F12,0x03EA}, //SARR_usGammaLutRGBIndoor[1][14]                                                          
{0x0F12,0x03FF}, //SARR_usGammaLutRGBIndoor[1][15]                                                          
{0x0F12,0x0000}, //SARR_usGammaLutRGBIndoor[2][0]                                                           
{0x0F12,0x0002}, //SARR_usGammaLutRGBIndoor[2][1]                                                           
{0x0F12,0x0008}, //SARR_usGammaLutRGBIndoor[2][2]                                                           
{0x0F12,0x0018}, //SARR_usGammaLutRGBIndoor[2][3]                                                           
{0x0F12,0x005A}, //SARR_usGammaLutRGBIndoor[2][4]                                                           
{0x0F12,0x00DF}, //SARR_usGammaLutRGBIndoor[2][5]                                                           
{0x0F12,0x013F}, //SARR_usGammaLutRGBIndoor[2][6]                                                           
{0x0F12,0x0186}, //SARR_usGammaLutRGBIndoor[2][7]                                                           
{0x0F12,0x01E6}, //SARR_usGammaLutRGBIndoor[2][8]                                                           
{0x0F12,0x0236}, //SARR_usGammaLutRGBIndoor[2][9]                                                           
{0x0F12,0x02BA}, //SARR_usGammaLutRGBIndoor[2][10]                                                          
{0x0F12,0x032A}, //SARR_usGammaLutRGBIndoor[2][11]                                                          
{0x0F12,0x0385}, //SARR_usGammaLutRGBIndoor[2][12]                                                          
{0x0F12,0x03C2}, //SARR_usGammaLutRGBIndoor[2][13]                                                          
{0x0F12,0x03EA}, //SARR_usGammaLutRGBIndoor[2][14]                                                          
{0x0F12,0x03FF}, //SARR_usGammaLutRGBIndoor[2][15]                                                                                                                                                                                                                                                                                                             
{0x002A,0x0DA6},
{0x0F12,0x0000}, // awbb_LowBr_NBzone	                                                                     
{0x0F12,0x0000}, // awbb_LowBr0_NBzone                                                                      
{0x002A,0x0E8C},
{0x0F12,0x0000}, // awbb_LowBr0_PatchNumZone                                                                
{0x002A,0x0D6C},
{0x0F12,0x0040}, // awbb_YMedMoveToYAv                                                                                                                                     
{0x002A,0x0B9C},
{0x0F12,0x038F}, //awbb_IndoorGrZones_m_BGrid_0__m_left                                                     
{0x0F12,0x039B}, //awbb_IndoorGrZones_m_BGrid_0__m_right                                                    
{0x0F12,0x0373}, //awbb_IndoorGrZones_m_BGrid_1__m_left                                                     
{0x0F12,0x03B0}, //awbb_IndoorGrZones_m_BGrid_1__m_right                                                    
{0x0F12,0x0352}, //awbb_IndoorGrZones_m_BGrid_2__m_left                                                     
{0x0F12,0x03B7}, //awbb_IndoorGrZones_m_BGrid_2__m_right                                                    
{0x0F12,0x0334}, //awbb_IndoorGrZones_m_BGrid_3__m_left                                                     
{0x0F12,0x03B5}, //awbb_IndoorGrZones_m_BGrid_3__m_right                                                    
{0x0F12,0x0318}, //awbb_IndoorGrZones_m_BGrid_4__m_left                                                     
{0x0F12,0x03B0}, //awbb_IndoorGrZones_m_BGrid_4__m_right                                                    
{0x0F12,0x02FF}, //awbb_IndoorGrZones_m_BGrid_5__m_left                                                     
{0x0F12,0x038D}, //awbb_IndoorGrZones_m_BGrid_5__m_right                                                    
{0x0F12,0x02E7}, //awbb_IndoorGrZones_m_BGrid_6__m_left                                                     
{0x0F12,0x0372}, //awbb_IndoorGrZones_m_BGrid_6__m_right                                                    
{0x0F12,0x02D0}, //awbb_IndoorGrZones_m_BGrid_7__m_left                                                     
{0x0F12,0x035D}, //awbb_IndoorGrZones_m_BGrid_7__m_right                                                    
{0x0F12,0x02B5}, //awbb_IndoorGrZones_m_BGrid_8__m_left                                                     
{0x0F12,0x0345}, //awbb_IndoorGrZones_m_BGrid_8__m_right                                                    
{0x0F12,0x02A1}, //awbb_IndoorGrZones_m_BGrid_9__m_left                                                     
{0x0F12,0x0331}, //awbb_IndoorGrZones_m_BGrid_9__m_right                                                    
{0x0F12,0x028B}, //awbb_IndoorGrZones_m_BGrid_10__m_left                                                    
{0x0F12,0x031E}, //awbb_IndoorGrZones_m_BGrid_10__m_right                                                   
{0x0F12,0x0273}, //awbb_IndoorGrZones_m_BGrid_11__m_left                                                    
{0x0F12,0x0309}, //awbb_IndoorGrZones_m_BGrid_11__m_right                                                   
{0x0F12,0x025F}, //awbb_IndoorGrZones_m_BGrid_12__m_left                                                    
{0x0F12,0x02F5}, //awbb_IndoorGrZones_m_BGrid_12__m_right                                                   
{0x0F12,0x0250}, //awbb_IndoorGrZones_m_BGrid_13__m_left                                                    
{0x0F12,0x02DB}, //awbb_IndoorGrZones_m_BGrid_13__m_right                                                   
{0x0F12,0x0241}, //awbb_IndoorGrZones_m_BGrid_14__m_left                                                    
{0x0F12,0x02C7}, //awbb_IndoorGrZones_m_BGrid_14__m_right                                                   
{0x0F12,0x0233}, //awbb_IndoorGrZones_m_BGrid_15__m_left                                                    
{0x0F12,0x02B9}, //awbb_IndoorGrZones_m_BGrid_15__m_right                                                   
{0x0F12,0x0223}, //awbb_IndoorGrZones_m_BGrid_16__m_left                                                    
{0x0F12,0x02AB}, //awbb_IndoorGrZones_m_BGrid_16__m_right                                                   
{0x0F12,0x0217}, //awbb_IndoorGrZones_m_BGrid_17__m_left                                                    
{0x0F12,0x02A2}, //awbb_IndoorGrZones_m_BGrid_17__m_right                                                   
{0x0F12,0x0207}, //awbb_IndoorGrZones_m_BGrid_18__m_left                                                    
{0x0F12,0x0294}, //awbb_IndoorGrZones_m_BGrid_18__m_right                                                   
{0x0F12,0x01FA}, //awbb_IndoorGrZones_m_BGrid_19__m_left                                                    
{0x0F12,0x0289}, //awbb_IndoorGrZones_m_BGrid_19__m_right                                                   
{0x0F12,0x01EA}, //awbb_IndoorGrZones_m_BGrid_20__m_left                                                    
{0x0F12,0x0281}, //awbb_IndoorGrZones_m_BGrid_20__m_right                                                   
{0x0F12,0x01DD}, //awbb_IndoorGrZones_m_BGrid_21__m_left                                                    
{0x0F12,0x027B}, //awbb_IndoorGrZones_m_BGrid_21__m_right                                                   
{0x0F12,0x01D0}, //awbb_IndoorGrZones_m_BGrid_22__m_left                                                    
{0x0F12,0x0273}, //awbb_IndoorGrZones_m_BGrid_22__m_right                                                   
{0x0F12,0x01C3}, //awbb_IndoorGrZones_m_BGrid_23__m_left                                                    
{0x0F12,0x026A}, //awbb_IndoorGrZones_m_BGrid_23__m_right                                                   
{0x0F12,0x01B6}, //awbb_IndoorGrZones_m_BGrid_24__m_left                                                    
{0x0F12,0x0265}, //awbb_IndoorGrZones_m_BGrid_24__m_right                                                   
{0x0F12,0x01AB}, //awbb_IndoorGrZones_m_BGrid_25__m_left                                                    
{0x0F12,0x025B}, //awbb_IndoorGrZones_m_BGrid_25__m_right                                                   
{0x0F12,0x01A1}, //awbb_IndoorGrZones_m_BGrid_26__m_left                                                    
{0x0F12,0x0254}, //awbb_IndoorGrZones_m_BGrid_26__m_right                                                   
{0x0F12,0x0198}, //awbb_IndoorGrZones_m_BGrid_27__m_left                                                    
{0x0F12,0x024B}, //awbb_IndoorGrZones_m_BGrid_27__m_right                                                   
{0x0F12,0x0192}, //awbb_IndoorGrZones_m_BGrid_28__m_left                                                    
{0x0F12,0x0242}, //awbb_IndoorGrZones_m_BGrid_28__m_right                                                   
{0x0F12,0x0191}, //awbb_IndoorGrZones_m_BGrid_29__m_left                                                    
{0x0F12,0x023A}, //awbb_IndoorGrZones_m_BGrid_29__m_right                                                   
{0x0F12,0x0192}, //awbb_IndoorGrZones_m_BGrid_30__m_left                                                    
{0x0F12,0x0222}, //awbb_IndoorGrZones_m_BGrid_30__m_right                                                   
{0x0F12,0x01C5}, //awbb_IndoorGrZones_m_BGrid_31__m_left                                                    
{0x0F12,0x01DF}, //awbb_IndoorGrZones_m_BGrid_31__m_right                                                   
{0x0F12,0x0000}, //awbb_IndoorGrZones_m_BGrid_32__m_left                                                    
{0x0F12,0x0000}, //awbb_IndoorGrZones_m_BGrid_32__m_right                                                   
{0x0F12,0x0000}, //awbb_IndoorGrZones_m_BGrid_33__m_left                                                    
{0x0F12,0x0000}, //awbb_IndoorGrZones_m_BGrid_33__m_right                                                                                          
{0x002A,0x0C3C},
{0x0F12,0x0004}, //awbb_IndoorGrZones_m_GridStep                                                            
{0x0F12,0x0000},                              
{0x0F12,0x0022}, //awbb_IndoorGrZones_m_GridSz                                                              
{0x0F12,0x0000},                              
{0x0F12,0x010F}, //awbb_IndoorGrZones_m_Boffs   																												  //
{0x0F12,0x0000},                              
{0x0F12,0x0020}, //awbb_IndoorGrZones_y_low     																													//
{0x0F12,0x0000},                              
{0x002A,0x0C50},                              
{0x0F12,0x00E0}, //awbb_IndoorGrZones_y_high  																												    //
{0x0F12,0x0000},                                                             
{0x0F12,0x0264}, //awbb_OutdoorGrZones_m_BGrid_0__m_left                                                    
{0x0F12,0x0279}, //awbb_OutdoorGrZones_m_BGrid_0__m_right                                                   
{0x0F12,0x0250}, //awbb_OutdoorGrZones_m_BGrid_1__m_left                                                    
{0x0F12,0x0287}, //awbb_OutdoorGrZones_m_BGrid_1__m_right                                                   
{0x0F12,0x0244}, //awbb_OutdoorGrZones_m_BGrid_2__m_left                                                    
{0x0F12,0x0287}, //awbb_OutdoorGrZones_m_BGrid_2__m_right                                                   
{0x0F12,0x0235}, //awbb_OutdoorGrZones_m_BGrid_3__m_left                                                    
{0x0F12,0x0289}, //awbb_OutdoorGrZones_m_BGrid_3__m_right                                                   
{0x0F12,0x0225}, //awbb_OutdoorGrZones_m_BGrid_4__m_left                                                    
{0x0F12,0x0287}, //awbb_OutdoorGrZones_m_BGrid_4__m_right                                                   
{0x0F12,0x0213}, //awbb_OutdoorGrZones_m_BGrid_5__m_left                                                    
{0x0F12,0x0286}, //awbb_OutdoorGrZones_m_BGrid_5__m_right                                                   
{0x0F12,0x0202}, //awbb_OutdoorGrZones_m_BGrid_6__m_left                                                    
{0x0F12,0x027A}, //awbb_OutdoorGrZones_m_BGrid_6__m_right                                                   
{0x0F12,0x01F3}, //awbb_OutdoorGrZones_m_BGrid_7__m_left                                                    
{0x0F12,0x0272}, //awbb_OutdoorGrZones_m_BGrid_7__m_right                                                   
{0x0F12,0x01E9}, //awbb_OutdoorGrZones_m_BGrid_8__m_left                                                    
{0x0F12,0x0269}, //awbb_OutdoorGrZones_m_BGrid_8__m_right                                                   
{0x0F12,0x01E2}, //awbb_OutdoorGrZones_m_BGrid_9__m_left                                                    
{0x0F12,0x0263}, //awbb_OutdoorGrZones_m_BGrid_9__m_right                                                   
{0x0F12,0x01E0}, //awbb_OutdoorGrZones_m_BGrid_10__m_left                                                   
{0x0F12,0x025A}, //awbb_OutdoorGrZones_m_BGrid_10__m_right                                                  
{0x0F12,0x01E1}, //awbb_OutdoorGrZones_m_BGrid_11__m_left                                                   
{0x0F12,0x0256}, //awbb_OutdoorGrZones_m_BGrid_11__m_right                                                  
{0x0F12,0x01EE}, //awbb_OutdoorGrZones_m_BGrid_12__m_left                                                   
{0x0F12,0x0251}, //awbb_OutdoorGrZones_m_BGrid_12__m_right                                                  
{0x0F12,0x01F8}, //awbb_OutdoorGrZones_m_BGrid[26]                                                          
{0x0F12,0x024A}, //awbb_OutdoorGrZones_m_BGrid[27]                                                          
{0x0F12,0x020D}, //awbb_OutdoorGrZones_m_BGrid[28]                                                          
{0x0F12,0x0231}, //awbb_OutdoorGrZones_m_BGrid[29]                                                          
{0x0F12,0x0000}, //awbb_OutdoorGrZones_m_BGrid[30]                                                          
{0x0F12,0x0000}, //awbb_OutdoorGrZones_m_BGrid[31]                                                          
{0x0F12,0x0000}, //awbb_OutdoorGrZones_m_BGrid[32]                                                          
{0x0F12,0x0000}, //awbb_OutdoorGrZones_m_BGrid[33]                                                          //
{0x002A,0x0CB8},
{0x0F12,0x0004}, // awbb_OutdoorGrZones_m_GridStep         																							  //
{0x0F12,0x0000},
{0x0F12,0x0011}, // awbb_OutdoorGrZones_m_GridSz            																							//
{0x0F12,0x0000},
{0x0F12,0x01FF}, // awbb_OutdoorGrZones_m_Boffs             																							//
{0x0F12,0x0000},                            
{0x0F12,0x0020}, // awbb_OutdoorGrZones_y_low               																													//
{0x0F12,0x0000},                            
{0x002A,0x0CCC},                            
{0x0F12,0x00C0}, // awbb_OutdoorGrZones_y_high          																													    //
{0x0F12,0x0000},  				//  7-3. Low Br grey zone                    																												// 				 // param_  C4start	awbb_LowBrGrZones_m_BGrid 																												//
{0x0F12,0x031F}, // awbb_LowBrGrZones_m_BGrid_0__m_left  																															//
{0x0F12,0x0495}, // awbb_LowBrGrZones_m_BGrid_0__m_right 																															//
{0x0F12,0x02FC}, // awbb_LowBrGrZones_m_BGrid_1__m_left  																															//
{0x0F12,0x0495}, // awbb_LowBrGrZones_m_BGrid_1__m_right 																															//
{0x0F12,0x02D9}, // awbb_LowBrGrZones_m_BGrid_2__m_left  																															//
{0x0F12,0x0495}, // awbb_LowBrGrZones_m_BGrid_2__m_right 																															//
{0x0F12,0x02B6}, // awbb_LowBrGrZones_m_BGrid_3__m_left  																															//
{0x0F12,0x0495}, // awbb_LowBrGrZones_m_BGrid_3__m_right 																															//
{0x0F12,0x0293}, // awbb_LowBrGrZones_m_BGrid_4__m_left  																															//
{0x0F12,0x0495}, // awbb_LowBrGrZones_m_BGrid_4__m_right 																															//
{0x0F12,0x0270}, // awbb_LowBrGrZones_m_BGrid_5__m_left  																															//
{0x0F12,0x0495}, // awbb_LowBrGrZones_m_BGrid_5__m_right 																															//
{0x0F12,0x024E}, // awbb_LowBrGrZones_m_BGrid_6__m_left  																															//
{0x0F12,0x0495}, // awbb_LowBrGrZones_m_BGrid_6__m_right 																															//
{0x0F12,0x022B}, // awbb_LowBrGrZones_m_BGrid_7__m_left  																															//
{0x0F12,0x0495}, // awbb_LowBrGrZones_m_BGrid_7__m_right 																															//
{0x0F12,0x0208}, // awbb_LowBrGrZones_m_BGrid_8__m_left  																															//
{0x0F12,0x048A}, // awbb_LowBrGrZones_m_BGrid_8__m_right 																															//
{0x0F12,0x01E5}, // awbb_LowBrGrZones_m_BGrid_9__m_left  																															//
{0x0F12,0x0455}, // awbb_LowBrGrZones_m_BGrid_9__m_right 																															//
{0x0F12,0x01C2}, // awbb_LowBrGrZones_m_BGrid_10__m_left 																															//
{0x0F12,0x041F}, // awbb_LowBrGrZones_m_BGrid_10__m_right																															//
{0x0F12,0x019F}, // awbb_LowBrGrZones_m_BGrid_11__m_left 																															//
{0x0F12,0x03EA}, // awbb_LowBrGrZones_m_BGrid_11__m_right																															//
{0x0F12,0x017D}, // awbb_LowBrGrZones_m_BGrid_12__m_left 																															//
{0x0F12,0x03B4}, // awbb_LowBrGrZones_m_BGrid_12__m_right																															//
{0x0F12,0x015A}, // awbb_LowBrGrZones_m_BGrid_13__m_left 																															//
{0x0F12,0x037F}, // awbb_LowBrGrZones_m_BGrid_13__m_right																															//
{0x0F12,0x0137}, // awbb_LowBrGrZones_m_BGrid_14__m_left 																															//
{0x0F12,0x0349}, // awbb_LowBrGrZones_m_BGrid_14__m_right																															//
{0x0F12,0x0130}, // awbb_LowBrGrZones_m_BGrid_15__m_left 																															//
{0x0F12,0x0314}, // awbb_LowBrGrZones_m_BGrid_15__m_right																															//
{0x0F12,0x012F}, // awbb_LowBrGrZones_m_BGrid_16__m_left 																															//
{0x0F12,0x02DE}, // awbb_LowBrGrZones_m_BGrid_16__m_right																															//
{0x0F12,0x012F}, // awbb_LowBrGrZones_m_BGrid_17__m_left 																															//
{0x0F12,0x02B1}, // awbb_LowBrGrZones_m_BGrid_17__m_right																															//
{0x0F12,0x012E}, // awbb_LowBrGrZones_m_BGrid_18__m_left 																															//
{0x0F12,0x028B}, // awbb_LowBrGrZones_m_BGrid_18__m_right																															//
{0x0F12,0x012D}, // awbb_LowBrGrZones_m_BGrid_19__m_left 																															//
{0x0F12,0x0265}, // awbb_LowBrGrZones_m_BGrid_19__m_right																															//
{0x0F12,0x012C}, // awbb_LowBrGrZones_m_BGrid_20__m_left 																															//
{0x0F12,0x023F}, // awbb_LowBrGrZones_m_BGrid_20__m_right																															//
{0x0F12,0x012C}, // awbb_LowBrGrZones_m_BGrid_21__m_left 																															//
{0x0F12,0x0219}, // awbb_LowBrGrZones_m_BGrid_21__m_right																															//
{0x0F12,0x012B}, // awbb_LowBrGrZones_m_BGrid_22__m_left 																															//
{0x0F12,0x01F3}, // awbb_LowBrGrZones_m_BGrid_22__m_right																															//
{0x0F12,0x012A}, // awbb_LowBrGrZones_m_BGrid_23__m_left 																															//
{0x0F12,0x01CD}, // awbb_LowBrGrZones_m_BGrid_23__m_right																															//
{0x0F12,0x0000}, // awbb_LowBrGrZones_m_BGrid_24__m_left 																															//
{0x0F12,0x0000}, // awbb_LowBrGrZones_m_BGrid_24__m_right																															//																									  //
{0x0F12,0x0005}, 
{0x0F12,0x0000}, //awbb_LowBrGrZones_m_GridStep																																			  //
{0x0F12,0x0018},                             
{0x0F12,0x0000}, //awbb_LowBrGrZones_m_GridSz  																																				//
{0x0F12,0x00AF},                             
{0x0F12,0x0000}, //awbb_LowBrGrZones_m_Boffs  																																			  //
{0x0F12,0x0002},                             
{0x0F12,0x0000}, //A awbb_LowBrGrZones_y_low  																																			  //
{0x002A,0x0D48},                             
{0x0F12,0x00E0},                             
{0x0F12,0x0000}, //awbb_LowBrGrZones_y_high    																																				//                       																											  //
{0x0F12,0x032F},                             
{0x0F12,0x0000}, //awbb_CrclLowT_R_c          																																				//
{0x0F12,0x017A},                             
{0x0F12,0x0000}, //awbb_CrclLowT_B_c         																																				  //
{0x0F12,0x7300},                             
{0x0F12,0x0000}, //awbb_CrclLowT_Rad_c       																																				  //
{0x0F12,0x000A},                             
{0x0F12,0x0000}, //awbb_CrclLowT_y_low   																																				      //
{0x002A,0x0D60},                             
{0x0F12,0x00E0},                             
{0x0F12,0x0000}, //awbb_CrclLowT_y_high 																																				      //
{0x002A,0x0D82},                             
{0x0F12,0x0001},                             																																				  //																																									//
{0x002A,0x0D8E},
{0x0F12,0x0002}, // awbb_GridEnable 																																									//																													//
{0x002A,0x0DCE}, 
{0x0F12,0x0000}, //awbb_GridCorr_R_0__0_ 																																							//
{0x0F12,0x0000}, //awbb_GridCorr_R_0__1_ 																																							//
{0x0F12,0x0040}, //awbb_GridCorr_R_0__2_ 																																							//
{0x0F12,0x0000}, //awbb_GridCorr_R_0__3_ 																																							//
{0x0F12,0xFFF0}, //awbb_GridCorr_R_0__4_ 																																							//
{0x0F12,0x0020}, //awbb_GridCorr_R_0__5_ 																																							//
{0x0F12,0x0000}, //awbb_GridCorr_R_1__0_ 																																							//
{0x0F12,0x0000}, //awbb_GridCorr_R_1__1_ 																																							//
{0x0F12,0x0040}, //awbb_GridCorr_R_1__2_ 																																							//
{0x0F12,0x0000}, //awbb_GridCorr_R_1__3_ 																																							//
{0x0F12,0xFFF0}, //awbb_GridCorr_R_1__4_ 																																							//
{0x0F12,0x0020}, //awbb_GridCorr_R_1__5_ 																																							//
{0x0F12,0x0000}, //awbb_GridCorr_R_2__0_ 																																							//
{0x0F12,0x0000}, //awbb_GridCorr_R_2__1_ 																																							//
{0x0F12,0x0040}, //awbb_GridCorr_R_2__2_ 																																							//
{0x0F12,0x0000}, //awbb_GridCorr_R_2__3_ 																																							//
{0x0F12,0xFFF0}, //awbb_GridCorr_R_2__4_ 																																							//
{0x0F12,0x0020}, //awbb_GridCorr_R_2__5_ 																																							//
{0x0F12,0x0000}, //awbb_GridCorr_B_0__0_ 																																							//
{0x0F12,0xFFE0}, //awbb_GridCorr_B_0__1_ 																																							//
{0x0F12,0x0040}, //awbb_GridCorr_B_0__2_ 																																							//
{0x0F12,0xFFC0}, //awbb_GridCorr_B_0__3_ 																																							//
{0x0F12,0xFFB0}, //awbb_GridCorr_B_0__4_ 																																							//
{0x0F12,0xFED4}, //awbb_GridCorr_B_0__5_ 																																							//
{0x0F12,0x0000}, //awbb_GridCorr_B_1__0_ 																																							//
{0x0F12,0xFFE0}, //awbb_GridCorr_B_1__1_ 																																							//
{0x0F12,0x0040}, //awbb_GridCorr_B_1__2_ 																																							//
{0x0F12,0xFFC0}, //awbb_GridCorr_B_1__3_ 																																							//
{0x0F12,0xFFB0}, //awbb_GridCorr_B_1__4_ 																																							//
{0x0F12,0xFED4}, //awbb_GridCorr_B_1__5_ 																																							//
{0x0F12,0x0000}, //awbb_GridCorr_B_2__0_ 																																							//
{0x0F12,0xFFE0}, //awbb_GridCorr_B_2__1_ 																																							//
{0x0F12,0x0040}, //awbb_GridCorr_B_2__2_ 																																							//
{0x0F12,0xFFC0}, //awbb_GridCorr_B_2__3_ 																																							//
{0x0F12,0xFFB0}, //awbb_GridCorr_B_2__4_ 																																							//
{0x0F12,0xFED4}, //awbb_GridCorr_B_2__5_ 																																							//
{0x0F12,0x02C6}, //awbb_GridConst_1_0_ 																																								//
{0x0F12,0x0335}, //awbb_GridConst_1_1_ 																																								//
{0x0F12,0x03B3}, //awbb_GridConst_1_2_ 																																								//
{0x0F12,0x0FD7}, //awbb_GridConst_2_0  																																								//
{0x0F12,0x10C5}, //awbb_GridConst_2_1  																																								//
{0x0F12,0x116A}, //awbb_GridConst_2_2  																																								//
{0x0F12,0x117C}, //awbb_GridConst_2_3  																																								//
{0x0F12,0x11C2}, //awbb_GridConst_2_4  																																								//
{0x0F12,0x120B}, //awbb_GridConst_2_5  																																								//
{0x0F12,0x00B3}, //awbb_GridCoeff_R_1 																																								//
{0x0F12,0x00B7}, //awbb_GridCoeff_B_1 																																								//
{0x0F12,0x00D3}, //awbb_GridCoeff_R_2 																																								//
{0x0F12,0x0091}, //awbb_GridCoeff_B_2 																																								//																																							//
{0x002A,0x0D66},
{0x0F12,0x0133}, // awbb_IntcR																																												//
{0x0F12,0x010F}, // awbb_IntcB																																												//
{0x002A,0x0D74},
{0x0F12,0x052A}, // awbb_MvEq_RBthresh 																																								//
{0x002A,0x0DAE},
{0x0F12,0x0036}, //awbb_GamutWidthThr2 																																	  					  //
{0x0F12,0x001C}, //awbb_GamutHeightThr2																																	  					  //
{0x002A,0x0DAA},
{0x0F12,0x071A}, //awbb_GamutWidthThr1  																																							//
{0x0F12,0x03A4}, //awbb_GamutHeightThr1 																																							//
{0x002A,0x0D92},
{0x0F12,0x0BB8}, //awbb_SunnyBr		              																																			//
{0x0F12,0x0096}, //awbb_Sunny_NBzone	          																																			//
{0x002A,0x0E86},                               
{0x0F12,0x0216}, //awbb_OutdoorWP_r             																																			//
{0x0F12,0x029F}, //awbb_OutdoorWP_b             																																			//
{0x002A,0x0D96},                               
{0x0F12,0x0BB7}, //awbb_CloudyBr		          																																			  //
{0x0F12,0x0096}, //awbb_Cloudy_NBzone	        																																			  //
{0x002A,0x0DB2},                               
{0x0F12,0x00DA}, //awbb_CloudyRB              																																			  //
{0x002A,0x0D9A},                               
{0x0F12,0x000A}, //awbb_Cloudy_BdivRzone        																																			//
{0x002A,0x0DB4},                               
{0x0F12,0x0459}, //awbb_LowTempRB               																																			//
{0x002A,0x0DA4},                               
{0x0F12,0x000E}, //awbb_LowTemp_RBzone       																																				  //
{0x002A,0x0D64},                               
{0x0F12,0x0032}, //awbb_DarkBr		             																																				//
{0x002A,0x0DA6},                               
{0x0F12,0x001E}, //awbb_LowBr_NBzone	         																																				//
{0x002A,0x0D9C},                               
{0x0F12,0x001B}, //awbb_MacbethGamut_WidthZone  																																			//
{0x0F12,0x000E}, //awbb_MacbethGamut_HeightZone 																																			//
{0x0F12,0x0008}, //awbb_MacbethGamut_WidthZone2 																																			//
{0x0F12,0x0004}, //awbb_MacbethGamut_HeightZone2																																			//																														  //
{0x002A,0x0E30},
{0x0F12,0x0000}, //awbb_OutdoorFltrSz (outdoor WB moving average filtering)																						//																																		//
{0x002A,0x0D88},
{0x0F12,0x0001}, //awbb_Use_InvalidOutDoor																																						//																															//																															//
{0x002A,0x0C48},
{0x0F12,0x0020}, //awbb_IndoorGrZones_y_low  																																					//
{0x002A,0x0C50},
{0x0F12,0x00E0}, //awbb_IndoorGrZones_y_high 																																					//
{0x002A,0x0CC4},
{0x0F12,0x0020}, //awbb_OutdoorGrZones_y_low 																																					//
{0x002A,0x0CCC},
{0x0F12,0x00C0}, //awbb_OutdoorGrZones_y_high																																					//																																					//																																			//
{0x002A,0x0DC2},
{0x0F12,0x0030}, //awbb_GnCurPntImmunity 																																							//
{0x0F12,0x00C8}, //awbb_GnFarPntImmunity 																																							//
{0x0F12,0x012C}, //awbb_GnCurPntLongJump 																																							//
{0x0F12,0x0258}, //awbb_GainsMaxMove     																																							//
{0x0F12,0x0003}, //awbb_GnMinMatchToJump 																																							//																																							/																																			//
{0x002A,0x06D0},
{0x0F12,0x2800}, //TVAR_wbt_pBaseCcmsAddr[0] 																																					//
{0x0F12,0x7000}, 
{0x0F12,0x2824}, //TVAR_wbt_pBaseCcmsAddr[1]																																					//
{0x0F12,0x7000}, 
{0x0F12,0x2848}, //TVAR_wbt_pBaseCcmsAddr[2]																																					//
{0x0F12,0x7000}, 
{0x0F12,0x286C}, //TVAR_wbt_pBaseCcmsAddr[3] 																																					//
{0x0F12,0x7000}, 
{0x0F12,0x2890}, //TVAR_wbt_pBaseCcmsAddr[4]																																					//
{0x0F12,0x7000}, 
{0x0F12,0x28B4}, //TVAR_wbt_pBaseCcmsAddr[5] 																																					//
{0x0F12,0x7000},
{0x002A,0x06EC}, 
{0x0F12,0x28D8}, //TVAR_wbt_pOutdoorCcm      																																					//
{0x0F12,0x7000},																											//
{0x002A,0x2800},
{0x0F12,0x01E1},	//01FB
{0x0F12,0xFFC4},	//FF9C
{0x0F12,0xFFF8},	//FFFF
{0x0F12,0x0101},	//0137
{0x0F12,0x014C},	//0113
{0x0F12,0xFF55},	//FF6F
{0x0F12,0xFF5B},	//FF21
{0x0F12,0x0205},	//0194
{0x0F12,0xFF17},	//FF69
{0x0F12,0xFEFE},	//FF14
{0x0F12,0x01B6},	//0158
{0x0F12,0x0107},	//015D
{0x0F12,0xFFDB},	//FFF2
{0x0F12,0xFFDB},	//FFF1
{0x0F12,0x01D1},	//0179
{0x0F12,0x0163},	//017C
{0x0F12,0xFF9E},	//FFC3
{0x0F12,0x01B3},	//0197
{0x0F12,0x01E1},	//01FB
{0x0F12,0xFFC4},	//FF9C
{0x0F12,0xFFF8},	//FFFF
{0x0F12,0x0101},	//0137
{0x0F12,0x014C},	//0113
{0x0F12,0xFF55},	//FF6F
{0x0F12,0xFF5B},	//FF21
{0x0F12,0x0205},	//0194
{0x0F12,0xFF17},	//FF69
{0x0F12,0xFEFE},	//FF14
{0x0F12,0x01B6},	//0158
{0x0F12,0x0107},	//015D
{0x0F12,0xFFDB},	//FFF2
{0x0F12,0xFFDB},	//FFF1
{0x0F12,0x01D1},	//0179
{0x0F12,0x0163},	//017C
{0x0F12,0xFF9E},	//FFC3
{0x0F12,0x01B3},	//0197
{0x0F12,0x01E1},	//01FB
{0x0F12,0xFFC4},	//FF9C
{0x0F12,0xFFF8},	//FFFF
{0x0F12,0x0101},	//0137
{0x0F12,0x014C},	//0113
{0x0F12,0xFF55},	//FF6F
{0x0F12,0xFF5B},	//FF21
{0x0F12,0x0205},	//0194
{0x0F12,0xFF17},	//FF69
{0x0F12,0xFEFE},	//FF14
{0x0F12,0x01B6},	//0158
{0x0F12,0x0107},	//015D
{0x0F12,0xFFDB},	//FFF2
{0x0F12,0xFFDB},	//FFF1
{0x0F12,0x01D1},	//0179
{0x0F12,0x0163},	//017C
{0x0F12,0xFF9E},	//FFC3
{0x0F12,0x01B3},	//0197
{0x0F12,0x01FB},	//01FB
{0x0F12,0xFFA9},	//FF9C
{0x0F12,0xFFEA},	//FFFF
{0x0F12,0x0134},	//0137
{0x0F12,0x0133},	//0113
{0x0F12,0xFF5D},	//FF6F
{0x0F12,0xFE7A},	//FF21
{0x0F12,0x017D},	//0194
{0x0F12,0xFEED},	//FF69
{0x0F12,0xFF39},	//FF14
{0x0F12,0x01D6},	//0158
{0x0F12,0x00C4},	//015D
{0x0F12,0xFFCE},	//FFF2
{0x0F12,0xFFCD},	//FFF1
{0x0F12,0x01B7},	//0179
{0x0F12,0x0176},	//017C
{0x0F12,0xFFBD},	//FFC3
{0x0F12,0x0191},	//0197
{0x0F12,0x01FB},	//01FB
{0x0F12,0xFFA9},	//FF9C
{0x0F12,0xFFEA},	//FFFF
{0x0F12,0x0134},	//0137
{0x0F12,0x0133},	//0113
{0x0F12,0xFF5D},	//FF6F
{0x0F12,0xFE7A},	//FF21
{0x0F12,0x017D},	//0194
{0x0F12,0xFEED},	//FF69
{0x0F12,0xFF39},	//FF14
{0x0F12,0x01D6},	//0158
{0x0F12,0x00C4},	//015D
{0x0F12,0xFFCE},	//FFF2
{0x0F12,0xFFCD},	//FFF1
{0x0F12,0x01B7},	//0179
{0x0F12,0x0176},	//017C
{0x0F12,0xFFBD},	//FFC3
{0x0F12,0x0191},	//0197
{0x0F12,0x01D2},	//01D0   
{0x0F12,0xFFC2},	//FFB4   
{0x0F12,0xFFFC},	//000C   
{0x0F12,0x011E},	//0122   
{0x0F12,0x011D},	//0103   
{0x0F12,0xFF86},	//FF9B   
{0x0F12,0xFE78},	//FF33   
{0x0F12,0x017B},	//01C5   
{0x0F12,0xFEEB},	//FF33   
{0x0F12,0xFF38},	//FF16   
{0x0F12,0x01D5},	//015A   
{0x0F12,0x00C3},	//015F   
{0x0F12,0xFFCF},	//FFE0   
{0x0F12,0xFFCE},	//FFDF   
{0x0F12,0x01B8},	//0197   
{0x0F12,0x0178},	//0178   
{0x0F12,0xFFBF},	//FFBF   
{0x0F12,0x0193},	//0193   
{0x0F12,0x01E0}, // outdoor CCM 																																											//
{0x0F12,0xFFBF},
{0x0F12,0xFFFD},
{0x0F12,0x00F5},
{0x0F12,0x0139},
{0x0F12,0xFF74},
{0x0F12,0xFEEC},
{0x0F12,0x01FD},
{0x0F12,0xFF8E},
{0x0F12,0xFEFE},
{0x0F12,0x01B6},
{0x0F12,0x0107},
{0x0F12,0xFFDB},
{0x0F12,0xFFDB},
{0x0F12,0x01D1},
{0x0F12,0x0163},
{0x0F12,0xFF9E},
{0x0F12,0x01B3},																																											//
{0x002A,0x07EA},
{0x0F12,0x0000}, // afit_bUseNoiseInd 0 : NB 1: Noise Index 																													//																													//
{0x0F12,0x000A}, //SARR_uNormBrInDoor[0] 																																							//
{0x0F12,0x0019}, //SARR_uNormBrInDoor[1] 																																							//
{0x0F12,0x007D}, //SARR_uNormBrInDoor[2] 																																							//
{0x0F12,0x02BC}, //SARR_uNormBrInDoor[3] 																																							//
{0x0F12,0x07D0}, //SARR_uNormBrInDoor[4] 																																							//																											//
{0x0F12,0x000A}, //SARR_uNormBrOutDoor[0] 																																						//
{0x0F12,0x0019}, //SARR_uNormBrOutDoor[1] 																																						//
{0x0F12,0x007D}, //SARR_uNormBrOutDoor[2] 																																						//
{0x0F12,0x02BC}, //SARR_uNormBrOutDoor[3] 																																						//
{0x0F12,0x07D0}, //SARR_uNormBrOutDoor[4] 																																						//																																//
{0x002A,0x0814},
{0x0F12,0x082C}, // TVAR_afit_pBaseVals  																																							//
{0x0F12,0x7000}, // TVAR_afit_pBaseVals  																																																										//
{0x002A,0x082C},
{0x0F12,0x0000}, // BRIGHTNESS                                                                             			
{0x0F12,0x0000}, // CONTRAST                                                                               			
{0x0F12,0xFFC0}, // SATURATION                                                                             			
{0x0F12,0xFFE2}, // SHARP_BLUR                                                                             			
{0x0F12,0x0000}, // GLAMOUR                                                                                			
{0x0F12,0x03FF}, // Disparity_iSatSat                                                                      			
{0x0F12,0x03FF}, // Denoise1_iYDenThreshLow                                                                			
{0x0F12,0x0028}, // Denoise1_iYDenThreshLow_Bin                                                            			
{0x0F12,0x03FF}, // Denoise1_iYDenThreshHigh                                                               			
{0x0F12,0x00FF}, // Denoise1_iYDenThreshHigh_Bin                                                           			
{0x0F12,0x0002}, // Denoise1_iLowWWideThresh                                                               			
{0x0F12,0x000A}, // Denoise1_iHighWWideThresh                                                              			
{0x0F12,0x000A}, // Denoise1_iLowWideThresh                                                                			
{0x0F12,0x000A}, // Denoise1_iHighWideThresh                                                               			
{0x0F12,0x03FF}, // Denoise1_iSatSat                                                                       			
{0x0F12,0x03FF}, // Demosaic4_iHystGrayLow                                                                 			
{0x0F12,0x0000}, // Demosaic4_iHystGrayHigh                                                                			
{0x0F12,0x0344}, // UVDenoise_iYLowThresh                                                                  			
{0x0F12,0x033A}, // UVDenoise_iYHighThresh                                                                 			
{0x0F12,0x03FF}, // UVDenoise_iUVLowThresh                                                                 			
{0x0F12,0x03FF}, // UVDenoise_iUVHighThresh                                                                			
{0x0F12,0x0028}, // DSMix1_iLowLimit_Wide                                                                  			
{0x0F12,0x0032}, // DSMix1_iLowLimit_Wide_Bin                                                              			
{0x0F12,0x0014}, // DSMix1_iHighLimit_Wide                                                                 			
{0x0F12,0x0032}, // DSMix1_iHighLimit_Wide_Bin                                                             			
{0x0F12,0x0050}, // DSMix1_iLowLimit_Fine                                                                  			
{0x0F12,0x0032}, // DSMix1_iLowLimit_Fine_Bin                                                              			
{0x0F12,0x0046}, // DSMix1_iHighLimit_Fine                                                                 			
{0x0F12,0x0032}, // DSMix1_iHighLimit_Fine_Bin                                                             			
{0x0F12,0x0106}, // DSMix1_iRGBOffset                                                                      			
{0x0F12,0x006F}, // DSMix1_iDemClamp                                                                       			
{0x0F12,0x0C0F}, // "Disparity_iDispTH_LowDisparity_iDispTH_Low_Bin"     bin: desparity low  								
{0x0F12,0x0C0F}, // "Disparity_iDispTH_High Disparity_iDispTH_High_Bin"  bin: desparity high 								
{0x0F12,0x0303}, // "Despeckle_iCorrectionLevelColdDespeckle_iCorrectionLevelCold_Bin"                     			
{0x0F12,0x0303}, // Despeckle_iCorrectionLevelHotDespeckle_iCorrectionLevelHot_Bin                         			
{0x0F12,0x140A}, // "Despeckle_iColdThreshLowDespeckle_iColdThreshHigh"                                    			
{0x0F12,0x140A}, // "Despeckle_iHotThreshLowDespeckle_iHotThreshHigh"                                      			
{0x0F12,0x2828}, // "Denoise1_iLowMaxSlopeAllowedDenoise1_iHighMaxSlopeAllowed"                            			
{0x0F12,0x0606}, // "Denoise1_iLowSlopeThreshDenoise1_iHighSlopeThresh"                                    			
{0x0F12,0x023F}, // "Denoise1_iRadialPowerDenoise1_iRadialDivideShift"                                     			
{0x0F12,0x0480}, // "Denoise1_iRadialLimitDenoise1_iLWBNoise"                                              			
{0x0F12,0x000F}, // "Denoise1_iWideDenoise1_iWideWide"                                                     			
{0x0F12,0x030A}, // "Demosaic4_iHystGrayRangeUVDenoise_iYSupport"                                          			
{0x0F12,0x0003}, // "UVDenoise_iUVSupportDSMix1_iLowPower_Wide"                                            			
{0x0F12,0x0011}, // "DSMix1_iLowPower_Wide_BinDSMix1_iHighPower_Wide"                                      			
{0x0F12,0x0A0F}, // "DSMix1_iHighPower_Wide_BinDSMix1_iLowThresh_Wide"                                     			
{0x0F12,0x050A}, // "DSMix1_iHighThresh_WideDSMix1_iReduceNegativeWide"  ]negati fine         								
{0x0F12,0x0900}, // "DSMix1_iLowPower_FineDSMix1_iLowPower_Fine_Bin"     ]low fine            								
{0x0F12,0x0000}, // "DSMix1_iHighPower_FineDSMix1_iHighPower_Fine_Bin"   ]high fine           								
{0x0F12,0x980A}, // "DSMix1_iLowThresh_FineDSMix1_iHighThresh_Fine"      ]high low thres      								
{0x0F12,0x0005}, // "DSMix1_iReduceNegativeFineDSMix1_iRGBMultiplier"                                      			
{0x0F12,0x0000}, // "Mixer1_iNLowNoisePowerMixer1_iNLowNoisePower_Bin"                                     			
{0x0F12,0x0000}, // "Mixer1_iNVeryLowNoisePowerMixer1_iNVeryLowNoisePower_Bin"                             			
{0x0F12,0x0000}, // "Mixer1_iNHighNoisePowerMixer1_iNHighNoisePower_Bin"                                   			
{0x0F12,0x0000}, // "Mixer1_iWLowNoisePowerMixer1_iWVeryLowNoisePower"                                     			
{0x0F12,0x0A00}, // "Mixer1_iWHighNoisePowerMixer1_iWLowNoiseCeilGain"                                     			
{0x0F12,0x000A}, // "Mixer1_iWHighNoiseCeilGainMixer1_iWNoiseCeilGain"                                     			
{0x0F12,0x0180}, // "CCM_Oscar_iSaturationCCM_Oscar_bSaturation"                                           			
{0x0F12,0x0180}, // "RGBGamma2_iLinearityRGBGamma2_bLinearity"                                             			
{0x0F12,0x0100}, // "RGBGamma2_iDarkReduceRGBGamma2_bDarkReduce"                                           			
{0x0F12,0x6E14}, // "byr_gas2_iShadingPowerRGB2YUV_iRGBGain"                                               			
{0x0F12,0x0180}, // "RGB2YUV_iSaturationRGB2YUV_bGainOffset"                                               			
{0x0F12,0x000A}, //  RGB2YUV_iYOffset                                                                      			
{0x0F12,0x0000},// BRIGHTNESS                                                                           			
{0x0F12,0x0000},// CONTRAST                                                                             			
{0x0F12,0x0000},// SATURATION                                                                           			
{0x0F12,0x0000},// SHARP_BLUR                                                                           			
{0x0F12,0x0000},// GLAMOUR                                                                              			
{0x0F12,0x03FF},// Disparity_iSatSat                                                                    			
{0x0F12,0x0014},// Denoise1_iYDenThreshLow                                                              			
{0x0F12,0x000E},// Denoise1_iYDenThreshLow_Bin                                                          			
{0x0F12,0x0064},// Denoise1_iYDenThreshHigh                                                             			
{0x0F12,0x00FF},// Denoise1_iYDenThreshHigh_Bin                                                         			
{0x0F12,0x0002},// Denoise1_iLowWWideThresh                                                             			
{0x0F12,0x000A},// Denoise1_iHighWWideThresh                                                            			
{0x0F12,0x000A},// Denoise1_iLowWideThresh                                                              			
{0x0F12,0x000A},// Denoise1_iHighWideThresh                                                             			
{0x0F12,0x03FF},// Denoise1_iSatSat                                                                     			
{0x0F12,0x03FF},// Demosaic4_iHystGrayLow                                                               			
{0x0F12,0x0000},// Demosaic4_iHystGrayHigh                                                              			
{0x0F12,0x0114},// UVDenoise_iYLowThresh                                                                			
{0x0F12,0x020A},// UVDenoise_iYHighThresh                                                               			
{0x0F12,0x03FF},// UVDenoise_iUVLowThresh                                                               			
{0x0F12,0x03FF},// UVDenoise_iUVHighThresh                                                              			
{0x0F12,0x0028},// DSMix1_iLowLimit_Wide                                                                			
{0x0F12,0x0000},// DSMix1_iLowLimit_Wide_Bin                                                            			
{0x0F12,0x0014},// DSMix1_iHighLimit_Wide                                                               			
{0x0F12,0x0032},// DSMix1_iHighLimit_Wide_Bin                                                           			
{0x0F12,0x0050},// DSMix1_iLowLimit_Fine                                                                			
{0x0F12,0x0032},// DSMix1_iLowLimit_Fine_Bin                                                            			
{0x0F12,0x0046},// DSMix1_iHighLimit_Fine                                                               			
{0x0F12,0x0032},// DSMix1_iHighLimit_Fine_Bin                                                           			
{0x0F12,0x0106},// DSMix1_iRGBOffset                                                                    			
{0x0F12,0x006F},// DSMix1_iDemClamp                                                                     			
{0x0F12,0x050F},// "Disparity_iDispTH_LowDisparity_iDispTH_Low_Bin"    bin: desparity low 							
{0x0F12,0x0A0F},// "Disparity_iDispTH_High Disparity_iDispTH_High_Bin"                                  			
{0x0F12,0x0203},// "Despeckle_iCorrectionLevelColdDespeckle_iCorrectionLevelCold_Bin"                   			
{0x0F12,0x0203},// Despeckle_iCorrectionLevelHotDespeckle_iCorrectionLevelHot_Bin                       			
{0x0F12,0x140A},// "Despeckle_iColdThreshLowDespeckle_iColdThreshHigh"                                  			
{0x0F12,0x140A},// "Despeckle_iHotThreshLowDespeckle_iHotThreshHigh"                                    			
{0x0F12,0x2828},// "Denoise1_iLowMaxSlopeAllowedDenoise1_iHighMaxSlopeAllowed"                          			
{0x0F12,0x0606},// "Denoise1_iLowSlopeThreshDenoise1_iHighSlopeThresh"                                  			
{0x0F12,0x020A},// "Denoise1_iRadialPowerDenoise1_iRadialDivideShift"                                   			
{0x0F12,0x0480},// "Denoise1_iRadialLimitDenoise1_iLWBNoise"                                            			
{0x0F12,0x000F},// "Denoise1_iWideDenoise1_iWideWide"                                                   			
{0x0F12,0x0305},// "Demosaic4_iHystGrayRangeUVDenoise_iYSupport"                                        			
{0x0F12,0x2803},// "UVDenoise_iUVSupportDSMix1_iLowPower_Wide"                                          			
{0x0F12,0x2811},// "DSMix1_iLowPower_Wide_BinDSMix1_iHighPower_Wide"                                    			
{0x0F12,0x0A0F},// "DSMix1_iHighPower_Wide_BinDSMix1_iLowThresh_Wide"                                   			
{0x0F12,0x050A},// "DSMix1_iHighThresh_WideDSMix1_iReduceNegativeWide"                                  			
{0x0F12,0x101E},// "DSMix1_iLowPower_FineDSMix1_iLowPower_Fine_Bin"                                     			
{0x0F12,0x101E},// "DSMix1_iHighPower_FineDSMix1_iHighPower_Fine_Bin"                                   			
{0x0F12,0x980A},// "DSMix1_iLowThresh_FineDSMix1_iHighThresh_Fine"                                      			
{0x0F12,0x0005},// "DSMix1_iReduceNegativeFineDSMix1_iRGBMultiplier"                                    			
{0x0F12,0x0400},// "Mixer1_iNLowNoisePowerMixer1_iNLowNoisePower_Bin"                                   			
{0x0F12,0x0400},// "Mixer1_iNVeryLowNoisePowerMixer1_iNVeryLowNoisePower_Bin"                           			
{0x0F12,0x0000},// "Mixer1_iNHighNoisePowerMixer1_iNHighNoisePower_Bin"                                 			
{0x0F12,0x0000},// "Mixer1_iWLowNoisePowerMixer1_iWVeryLowNoisePower"                                   			
{0x0F12,0x0A00},// "Mixer1_iWHighNoisePowerMixer1_iWLowNoiseCeilGain"                                   			
{0x0F12,0x100A},// "Mixer1_iWHighNoiseCeilGainMixer1_iWNoiseCeilGain"                                   			
{0x0F12,0x0180},// "CCM_Oscar_iSaturationCCM_Oscar_bSaturation"                                         			
{0x0F12,0x0180},// "RGBGamma2_iLinearityRGBGamma2_bLinearity"                                           			
{0x0F12,0x0100},// "RGBGamma2_iDarkReduceRGBGamma2_bDarkReduce"                                         			
{0x0F12,0x8030},// "byr_gas2_iShadingPowerRGB2YUV_iRGBGain"                                             			
{0x0F12,0x0180},// "RGB2YUV_iSaturationRGB2YUV_bGainOffset"                                             			
{0x0F12,0x0000},//  RGB2YUV_iYOffset                                                                    			        
{0x0F12,0x0000},// BRIGHTNESS                                                         							
{0x0F12,0x0000},// CONTRAST                                                           							
{0x0F12,0x0000},// SATURATION                                                         							
{0x0F12,0x0000},// SHARP_BLUR                                                         							
{0x0F12,0x0000},// GLAMOUR                                                            							
{0x0F12,0x03FF},// Disparity_iSatSat                                                  							
{0x0F12,0x000C},// Denoise1_iYDenThreshLow                                            							
{0x0F12,0x0006},// Denoise1_iYDenThreshLow_Bin                                        							
{0x0F12,0x0060},// Denoise1_iYDenThreshHigh                                           							
{0x0F12,0x0050},// Denoise1_iYDenThreshHigh_Bin                                       							
{0x0F12,0x0002},// Denoise1_iLowWWideThresh                                           							
{0x0F12,0x000A},// Denoise1_iHighWWideThresh                                          							
{0x0F12,0x000A},// Denoise1_iLowWideThresh                                            							
{0x0F12,0x000A},// Denoise1_iHighWideThresh                                           							
{0x0F12,0x03FF},// Denoise1_iSatSat                                                   							
{0x0F12,0x03FF},// Demosaic4_iHystGrayLow                                             							
{0x0F12,0x0000},// Demosaic4_iHystGrayHigh                                            							
{0x0F12,0x0014},// UVDenoise_iYLowThresh                                              							
{0x0F12,0x000A},// UVDenoise_iYHighThresh                                             							
{0x0F12,0x03FF},// UVDenoise_iUVLowThresh                                             							
{0x0F12,0x03FF},// UVDenoise_iUVHighThresh                                            							
{0x0F12,0x0028},// DSMix1_iLowLimit_Wide                                              							
{0x0F12,0x0032},// DSMix1_iLowLimit_Wide_Bin                                          							
{0x0F12,0x0014},// DSMix1_iHighLimit_Wide                                             							
{0x0F12,0x0032},// DSMix1_iHighLimit_Wide_Bin                                         							
{0x0F12,0x0050},// DSMix1_iLowLimit_Fine                                              							
{0x0F12,0x0032},// DSMix1_iLowLimit_Fine_Bin                                          							
{0x0F12,0x0010},// DSMix1_iHighLimit_Fine                                             							
{0x0F12,0x0032},// DSMix1_iHighLimit_Fine_Bin                                         							
{0x0F12,0x0106},// DSMix1_iRGBOffset                                                  							
{0x0F12,0x006F},// DSMix1_iDemClamp                                                   							
{0x0F12,0x0202},// "Disparity_iDispTH_LowDisparity_iDispTH_Low_Bin"                   							
{0x0F12,0x0502},// "Disparity_iDispTH_High Disparity_iDispTH_High_Bin"                							
{0x0F12,0x0102},// "Despeckle_iCorrectionLevelColdDespeckle_iCorrectionLevelCold_Bin" 							
{0x0F12,0x0102},// Despeckle_iCorrectionLevelHotDespeckle_iCorrectionLevelHot_Bin     							
{0x0F12,0x140A},// "Despeckle_iColdThreshLowDespeckle_iColdThreshHigh"                							
{0x0F12,0x140A},// "Despeckle_iHotThreshLowDespeckle_iHotThreshHigh"                  							
{0x0F12,0x2828},// "Denoise1_iLowMaxSlopeAllowedDenoise1_iHighMaxSlopeAllowed"        							
{0x0F12,0x0606},// "Denoise1_iLowSlopeThreshDenoise1_iHighSlopeThresh"                							
{0x0F12,0x0205},// "Denoise1_iRadialPowerDenoise1_iRadialDivideShift"                 							
{0x0F12,0x0480},// "Denoise1_iRadialLimitDenoise1_iLWBNoise"                          							
{0x0F12,0x000F},// "Denoise1_iWideDenoise1_iWideWide"                                 							
{0x0F12,0x0005},// "Demosaic4_iHystGrayRangeUVDenoise_iYSupport"                      							
{0x0F12,0x2803},// "UVDenoise_iUVSupportDSMix1_iLowPower_Wide"                        							
{0x0F12,0x2811},// "DSMix1_iLowPower_Wide_BinDSMix1_iHighPower_Wide"                  							
{0x0F12,0x0A0F},// "DSMix1_iHighPower_Wide_BinDSMix1_iLowThresh_Wide"                 							
{0x0F12,0x050A},// "DSMix1_iHighThresh_WideDSMix1_iReduceNegativeWide"                							
{0x0F12,0x2020},// "DSMix1_iLowPower_FineDSMix1_iLowPower_Fine_Bin"                   							
{0x0F12,0x2020},// "DSMix1_iHighPower_FineDSMix1_iHighPower_Fine_Bin"                 							
{0x0F12,0x980A},// "DSMix1_iLowThresh_FineDSMix1_iHighThresh_Fine"                    							
{0x0F12,0x0007},// "DSMix1_iReduceNegativeFineDSMix1_iRGBMultiplier"                  							
{0x0F12,0x0403},// "Mixer1_iNLowNoisePowerMixer1_iNLowNoisePower_Bin"                 							
{0x0F12,0x0402},// "Mixer1_iNVeryLowNoisePowerMixer1_iNVeryLowNoisePower_Bin"         							
{0x0F12,0x0000},// "Mixer1_iNHighNoisePowerMixer1_iNHighNoisePower_Bin"               							
{0x0F12,0x0203},// "Mixer1_iWLowNoisePowerMixer1_iWVeryLowNoisePower"                 							
{0x0F12,0x0000},// "Mixer1_iWHighNoisePowerMixer1_iWLowNoiseCeilGain"                 							
{0x0F12,0x1006},// "Mixer1_iWHighNoiseCeilGainMixer1_iWNoiseCeilGain"                 							
{0x0F12,0x0180},// "CCM_Oscar_iSaturationCCM_Oscar_bSaturation"                       							
{0x0F12,0x0180},// "RGBGamma2_iLinearityRGBGamma2_bLinearity"                         							
{0x0F12,0x0100},// "RGBGamma2_iDarkReduceRGBGamma2_bDarkReduce"                       							
{0x0F12,0x803C},// "byr_gas2_iShadingPowerRGB2YUV_iRGBGain"                           							
{0x0F12,0x0180},// "RGB2YUV_iSaturationRGB2YUV_bGainOffset"                           							
{0x0F12,0x0000},//  RGB2YUV_iYOffset                                                  							
{0x0F12,0x0000},// BRIGHTNESS                                                         							
{0x0F12,0x0000},// CONTRAST                                                           							
{0x0F12,0x0000},// SATURATION                                                         							
{0x0F12,0x0000},// SHARP_BLUR                                                         							
{0x0F12,0x0000},// GLAMOUR                                                            							
{0x0F12,0x03FF},// Disparity_iSatSat                                                  							
{0x0F12,0x0006},// Denoise1_iYDenThreshLow                                            							
{0x0F12,0x0006},// Denoise1_iYDenThreshLow_Bin                                        							
{0x0F12,0x005A},// Denoise1_iYDenThreshHigh                                           							
{0x0F12,0x0050},// Denoise1_iYDenThreshHigh_Bin                                       							
{0x0F12,0x0002},// Denoise1_iLowWWideThresh                                           							
{0x0F12,0x000A},// Denoise1_iHighWWideThresh                                          							
{0x0F12,0x000A},// Denoise1_iLowWideThresh                                            							
{0x0F12,0x000A},// Denoise1_iHighWideThresh                                           							
{0x0F12,0x03FF},// Denoise1_iSatSat                                                   							
{0x0F12,0x03FF},// Demosaic4_iHystGrayLow                                             							
{0x0F12,0x0000},// Demosaic4_iHystGrayHigh                                            							
{0x0F12,0x0014},// UVDenoise_iYLowThresh                                              							
{0x0F12,0x000A},// UVDenoise_iYHighThresh                                             							
{0x0F12,0x03FF},// UVDenoise_iUVLowThresh                                             							
{0x0F12,0x03FF},// UVDenoise_iUVHighThresh                                            							
{0x0F12,0x0028},// DSMix1_iLowLimit_Wide                                              							
{0x0F12,0x0032},// DSMix1_iLowLimit_Wide_Bin                                          							
{0x0F12,0x0014},// DSMix1_iHighLimit_Wide                                             							
{0x0F12,0x0032},// DSMix1_iHighLimit_Wide_Bin                                         							
{0x0F12,0x0050},// DSMix1_iLowLimit_Fine                                              							
{0x0F12,0x0032},// DSMix1_iLowLimit_Fine_Bin                                          							
{0x0F12,0x0010},// DSMix1_iHighLimit_Fine                                             							
{0x0F12,0x0032},// DSMix1_iHighLimit_Fine_Bin                                         							
{0x0F12,0x0106},// DSMix1_iRGBOffset                                                  							
{0x0F12,0x006F},// DSMix1_iDemClamp                                                   							
{0x0F12,0x0202},// "Disparity_iDispTH_LowDisparity_iDispTH_Low_Bin"                   							
{0x0F12,0x0502},// "Disparity_iDispTH_High Disparity_iDispTH_High_Bin"                							
{0x0F12,0x0102},// "Despeckle_iCorrectionLevelColdDespeckle_iCorrectionLevelCold_Bin" 							
{0x0F12,0x0102},// Despeckle_iCorrectionLevelHotDespeckle_iCorrectionLevelHot_Bin     							
{0x0F12,0x140A},// "Despeckle_iColdThreshLowDespeckle_iColdThreshHigh"                							
{0x0F12,0x140A},// "Despeckle_iHotThreshLowDespeckle_iHotThreshHigh"                  							
{0x0F12,0x2828},// "Denoise1_iLowMaxSlopeAllowedDenoise1_iHighMaxSlopeAllowed"        							
{0x0F12,0x0606},// "Denoise1_iLowSlopeThreshDenoise1_iHighSlopeThresh"                							
{0x0F12,0x0205},// "Denoise1_iRadialPowerDenoise1_iRadialDivideShift"                 							
{0x0F12,0x0480},// "Denoise1_iRadialLimitDenoise1_iLWBNoise"                          							
{0x0F12,0x000F},// "Denoise1_iWideDenoise1_iWideWide"                                 							
{0x0F12,0x0005},// "Demosaic4_iHystGrayRangeUVDenoise_iYSupport"                      							
{0x0F12,0x2803},// "UVDenoise_iUVSupportDSMix1_iLowPower_Wide"                        							
{0x0F12,0x2811},// "DSMix1_iLowPower_Wide_BinDSMix1_iHighPower_Wide"                  							
{0x0F12,0x0A0F},// "DSMix1_iHighPower_Wide_BinDSMix1_iLowThresh_Wide"                 							
{0x0F12,0x050A},// "DSMix1_iHighThresh_WideDSMix1_iReduceNegativeWide"                							
{0x0F12,0x2020},// "DSMix1_iLowPower_FineDSMix1_iLowPower_Fine_Bin"                   							
{0x0F12,0x2020},// "DSMix1_iHighPower_FineDSMix1_iHighPower_Fine_Bin"                 							
{0x0F12,0x980A},// "DSMix1_iLowThresh_FineDSMix1_iHighThresh_Fine"                    							
{0x0F12,0x0007},// "DSMix1_iReduceNegativeFineDSMix1_iRGBMultiplier"                  							
{0x0F12,0x0403},// "Mixer1_iNLowNoisePowerMixer1_iNLowNoisePower_Bin"                 							
{0x0F12,0x0402},// "Mixer1_iNVeryLowNoisePowerMixer1_iNVeryLowNoisePower_Bin"         							
{0x0F12,0x0000},// "Mixer1_iNHighNoisePowerMixer1_iNHighNoisePower_Bin"               							
{0x0F12,0x0203},// "Mixer1_iWLowNoisePowerMixer1_iWVeryLowNoisePower"                 							
{0x0F12,0x0000},// "Mixer1_iWHighNoisePowerMixer1_iWLowNoiseCeilGain"                 							
{0x0F12,0x1006},// "Mixer1_iWHighNoiseCeilGainMixer1_iWNoiseCeilGain"                 							
{0x0F12,0x0180},// "CCM_Oscar_iSaturationCCM_Oscar_bSaturation"                       							
{0x0F12,0x0180},// "RGBGamma2_iLinearityRGBGamma2_bLinearity"                         							
{0x0F12,0x0100},// "RGBGamma2_iDarkReduceRGBGamma2_bDarkReduce"                       							
{0x0F12,0x803C},// "byr_gas2_iShadingPowerRGB2YUV_iRGBGain"                           							
{0x0F12,0x0180},// "RGB2YUV_iSaturationRGB2YUV_bGainOffset"                           							
{0x0F12,0x0000},//  RGB2YUV_iYOffset                                                  							
{0x0F12,0x0000},// BRIGHTNESS                                                         							
{0x0F12,0x000A},// CONTRAST                                                           							
{0x0F12,0x0000},// SATURATION                                                         							
{0x0F12,0x0000},// SHARP_BLUR                                                         							
{0x0F12,0x0000},// GLAMOUR                                                            							
{0x0F12,0x03FF},// Disparity_iSatSat                                                  							
{0x0F12,0x0006},// Denoise1_iYDenThreshLow                                            							
{0x0F12,0x0006},// Denoise1_iYDenThreshLow_Bin                                        							
{0x0F12,0x0050},// Denoise1_iYDenThreshHigh                                           							
{0x0F12,0x0050},// Denoise1_iYDenThreshHigh_Bin                                       							
{0x0F12,0x0002},// Denoise1_iLowWWideThresh                                           							
{0x0F12,0x000A},// Denoise1_iHighWWideThresh                                          							
{0x0F12,0x000A},// Denoise1_iLowWideThresh                                            							
{0x0F12,0x000A},// Denoise1_iHighWideThresh                                           							
{0x0F12,0x03FF},// Denoise1_iSatSat                                                   							
{0x0F12,0x03FF},// Demosaic4_iHystGrayLow                                             							
{0x0F12,0x0000},// Demosaic4_iHystGrayHigh                                            							
{0x0F12,0x0000},// UVDenoise_iYLowThresh                                              							
{0x0F12,0x0000},// UVDenoise_iYHighThresh                                             							
{0x0F12,0x03FF},// UVDenoise_iUVLowThresh                                             							
{0x0F12,0x03FF},// UVDenoise_iUVHighThresh                                            							
{0x0F12,0x0028},// DSMix1_iLowLimit_Wide                                              							
{0x0F12,0x0032},// DSMix1_iLowLimit_Wide_Bin                                          							
{0x0F12,0x0000},// DSMix1_iHighLimit_Wide                                             							
{0x0F12,0x0032},// DSMix1_iHighLimit_Wide_Bin                                         							
{0x0F12,0x0030},// DSMix1_iLowLimit_Fine                                              							
{0x0F12,0x0032},// DSMix1_iLowLimit_Fine_Bin                                          							
{0x0F12,0x0000},// DSMix1_iHighLimit_Fine                                             							
{0x0F12,0x0032},// DSMix1_iHighLimit_Fine_Bin                                         							
{0x0F12,0x0106},// DSMix1_iRGBOffset                                                  							
{0x0F12,0x006F},// DSMix1_iDemClamp                                                   							
{0x0F12,0x0202},// "Disparity_iDispTH_LowDisparity_iDispTH_Low_Bin"                   							
{0x0F12,0x0502},// "Disparity_iDispTH_High Disparity_iDispTH_High_Bin"                							
{0x0F12,0x0102},// "Despeckle_iCorrectionLevelColdDespeckle_iCorrectionLevelCold_Bin" 							
{0x0F12,0x0102},// Despeckle_iCorrectionLevelHotDespeckle_iCorrectionLevelHot_Bin     							
{0x0F12,0x140A},// "Despeckle_iColdThreshLowDespeckle_iColdThreshHigh"                							
{0x0F12,0x140A},// "Despeckle_iHotThreshLowDespeckle_iHotThreshHigh"                  							
{0x0F12,0x2828},// "Denoise1_iLowMaxSlopeAllowedDenoise1_iHighMaxSlopeAllowed"        							
{0x0F12,0x0606},// "Denoise1_iLowSlopeThreshDenoise1_iHighSlopeThresh"                							
{0x0F12,0x0205},// "Denoise1_iRadialPowerDenoise1_iRadialDivideShift"                 							
{0x0F12,0x0880},// "Denoise1_iRadialLimitDenoise1_iLWBNoise"                          							
{0x0F12,0x000F},// "Denoise1_iWideDenoise1_iWideWide"                                 							
{0x0F12,0x0005},// "Demosaic4_iHystGrayRangeUVDenoise_iYSupport"                      							
{0x0F12,0x2803},// "UVDenoise_iUVSupportDSMix1_iLowPower_Wide"                        							
{0x0F12,0x2811},// "DSMix1_iLowPower_Wide_BinDSMix1_iHighPower_Wide"                  							
{0x0F12,0x0A0F},// "DSMix1_iHighPower_Wide_BinDSMix1_iLowThresh_Wide"                 							
{0x0F12,0x050A},// "DSMix1_iHighThresh_WideDSMix1_iReduceNegativeWide"                							
{0x0F12,0x2020},// "DSMix1_iLowPower_FineDSMix1_iLowPower_Fine_Bin"                   							
{0x0F12,0x2020},// "DSMix1_iHighPower_FineDSMix1_iHighPower_Fine_Bin"                 							
{0x0F12,0x980A},// "DSMix1_iLowThresh_FineDSMix1_iHighThresh_Fine"                    							
{0x0F12,0x0007},// "DSMix1_iReduceNegativeFineDSMix1_iRGBMultiplier"                  							
{0x0F12,0x0408},// "Mixer1_iNLowNoisePowerMixer1_iNLowNoisePower_Bin"                 							
{0x0F12,0x0406},// "Mixer1_iNVeryLowNoisePowerMixer1_iNVeryLowNoisePower_Bin"         							
{0x0F12,0x0000},// "Mixer1_iNHighNoisePowerMixer1_iNHighNoisePower_Bin"               							
{0x0F12,0x0608},// "Mixer1_iWLowNoisePowerMixer1_iWVeryLowNoisePower"                 							
{0x0F12,0x0000},// "Mixer1_iWHighNoisePowerMixer1_iWLowNoiseCeilGain"                 							
{0x0F12,0x1006},// "Mixer1_iWHighNoiseCeilGainMixer1_iWNoiseCeilGain"                 							
{0x0F12,0x0180},// "CCM_Oscar_iSaturationCCM_Oscar_bSaturation"                       							
{0x0F12,0x0180},// "RGBGamma2_iLinearityRGBGamma2_bLinearity"                         							
{0x0F12,0x0100},// "RGBGamma2_iDarkReduceRGBGamma2_bDarkReduce"                       							
{0x0F12,0x8050},// "byr_gas2_iShadingPowerRGB2YUV_iRGBGain"                           							
{0x0F12,0x0140},// "RGB2YUV_iSaturationRGB2YUV_bGainOffset"                           							
{0x0F12,0x0000},//  RGB2YUV_iYOffset                                                  							
{0x0F12,0x00FF},// Denoise1_iUVDenThreshLow                         										
{0x0F12,0x00FF},// Denoise1_iUVDenThreshHigh                        										
{0x0F12,0x0800},// Denoise1_sensor_width                            										
{0x0F12,0x0600},// Denoise1_sensor_height                           										
{0x0F12,0x0000},// Denoise1_start_x                                 										
{0x0F12,0x0000},// Denoise1_start_y                                 										
{0x0F12,0x0000},// "Denoise1_iYDenSmoothDenoise1_iWSharp  "         										
{0x0F12,0x0300},// "Denoise1_iWWSharp Denoise1_iRadialTune  "       										
{0x0F12,0x0002},// "Denoise1_iOutputBrightnessDenoise1_binning_x  " 										
{0x0F12,0x0400},// "Denoise1_binning_yDemosaic4_iFDeriv  "          										
{0x0F12,0x0106},// "Demosaic4_iFDerivNeiDemosaic4_iSDeriv  "        										
{0x0F12,0x0005},// "Demosaic4_iSDerivNeiDemosaic4_iEnhancerG  "     										
{0x0F12,0x0000},// "Demosaic4_iEnhancerRBDemosaic4_iEnhancerV  "    										
{0x0F12,0x0703},// "Demosaic4_iDecisionThreshDemosaic4_iDesatThresh"										
{0x0F12,0x0000},//  Demosaic4_iBypassSelect                         										
{0x0F12,0xFFD6},
{0x0F12,0x53C1},
{0x0F12,0xE1FE},
{0x0F12,0x0001},
{0x002A,0x03FC},
{0x0F12,0x0001},// REG_TC_DBG_ReInitCmd 													
{0x002A,0x03FC},
{0x0F12,0x0001},
{0x002A,0x03FC},
{0x0F12,0x0001},
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



