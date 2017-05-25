/* linux/drivers/media/video/s5k5ba.c
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

#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/version.h>
#include <media/v4l2-device.h>
#include <media/v4l2-subdev.h>
#include <media/v4l2-i2c-drv.h>
#include <media/s5k5ba_platform.h>

#ifdef CONFIG_VIDEO_SAMSUNG_V4L2
#include <linux/videodev2_samsung.h>
#endif

#include "s5k5ba.h"

#define S5K5BA_DRIVER_NAME	"S5K5BA"

/* Default resolution & pixelformat. plz ref s5k5ba_platform.h */
#define DEFAULT_RES		WVGA	/* Index of resoultion */
#define DEFAUT_FPS_INDEX	S5K5BA_15FPS
#define DEFAULT_FMT		V4L2_PIX_FMT_UYVY	/* YUV422 */

#define REG_RESET   0X3021
static int debug = 0;

/*
 * Specification
 * Parallel : ITU-R. 656/601 YUV422, RGB565, RGB888 (Up to VGA), RAW10
 * Serial : MIPI CSI2 (single lane) YUV422, RGB565, RGB888 (Up to VGA), RAW10
 * Resolution : 1280 (H) x 1024 (V)
 * Image control : Brightness, Contrast, Saturation, Sharpness, Glamour
 * Effect : Mono, Negative, Sepia, Aqua, Sketch
 * FPS : 15fps @full resolution, 30fps @VGA, 24fps @720p
 * Max. pixel clock frequency : 48MHz(upto)
 * Internal PLL (6MHz to 27MHz input frequency)
 */

static int s5k5ba_init(struct v4l2_subdev *sd, u32 val);
/* Camera functional setting values configured by user concept */
struct s5k5ba_userset {
	signed int exposure_bias;	/* V4L2_CID_EXPOSURE */
	unsigned int ae_lock;
	unsigned int awb_lock;
	unsigned int auto_wb;	/* V4L2_CID_AUTO_WHITE_BALANCE */
	unsigned int manual_wb;	/* V4L2_CID_WHITE_BALANCE_PRESET */
	unsigned int wb_temp;	/* V4L2_CID_WHITE_BALANCE_TEMPERATURE */
	unsigned int effect;	/* Color FX (AKA Color tone) */
	unsigned int contrast;	/* V4L2_CID_CONTRAST */
	unsigned int saturation;	/* V4L2_CID_SATURATION */
	unsigned int sharpness;	/* V4L2_CID_SHARPNESS */
	unsigned int glamour;
};

struct s5k5ba_state {
	struct s5k5ba_platform_data *pdata;
	struct v4l2_subdev sd;
	struct v4l2_pix_format pix;
	struct v4l2_fract timeperframe;
	struct s5k5ba_userset userset;
	int framesize_index;
	int freq;	/* MCLK in KHz */
	int is_mipi;
	int isize;
	int ver;
	int fps;
	int check_previewdata;
};

enum {
	S5K5BA_PREVIEW_VGA,
    //S5K5BA_PREVIEW_SVGA,
};

struct s5k5ba_enum_framesize {
	unsigned int index;
	unsigned int width;
	unsigned int height;
};

struct s5k5ba_enum_framesize s5k5ba_framesize_list[] = {
	{ S5K5BA_PREVIEW_VGA, 640, 480 }
   // { S5K5BA_PREVIEW_SVGA, 800, 600 }
};

static inline struct s5k5ba_state *to_state(struct v4l2_subdev *sd)
{
	return container_of(sd, struct s5k5ba_state, sd);
}

static int s5k5ba_reset(struct v4l2_subdev *sd)
{
	return s5k5ba_init(sd, 0);
}

/*
 * S5K5BA register structure : 2bytes address, 2bytes value
 * retry on write failure up-to 5 times
 */
static inline int s5k5ba_write(struct v4l2_subdev *sd, u8 addr, u8 val)
{
	struct i2c_client *client = v4l2_get_subdevdata(sd);
	struct i2c_msg msg[1];
	unsigned char reg[2];
	int err = 0;
	int retry = 0;


	if (!client->adapter)
		return -ENODEV;

again:
	msg->addr = client->addr;
	msg->flags = 0;
	msg->len = 2;
	msg->buf = reg;

	reg[0] = addr & 0xff;
	reg[1] = val & 0xff;

	err = i2c_transfer(client->adapter, msg, 1);
	if (err >= 0)
		return err;	/* Returns here on success */

	/* abnormal case: retry 5 times */
	if (retry < 5) {
		dev_err(&client->dev, "%s: address: 0x%02x%02x, " \
			"value: 0x%02x%02x\n", __func__, \
			reg[0], reg[1], reg[2], reg[3]);
		retry++;
		goto again;
	}

	return err;
}

static int s5k5ba_i2c_write(struct v4l2_subdev *sd, unsigned char i2c_data[],
				unsigned char length)
{
	int err;
	//printk(KERN_INFO "###TEST_XU: %s ...\n",__func__);
	struct i2c_client *client = v4l2_get_subdevdata(sd);
	unsigned char buf[length], i;
	struct i2c_msg msg = {client->addr, 0, length, buf};
	//struct i2c_msg msg = {client->addr,client->flags, length, buf};
	for (i = 0; i < length; i++)
		buf[i] = i2c_data[i];
	
	//return i2c_transfer(client->adapter, &msg, 1) == 1 ? 0 : -EIO;
	if(err == 1)
		{
		printk(KERN_INFO "###TEST_XU: %s success err =%d\n",__func__,err);
		err = 0;
		}
		
	else
		printk(KERN_INFO "###TEST_XU: %s failed err =%d\n",__func__,err);
	return err;

}

static int s5k5ba_write_regs(struct v4l2_subdev *sd, unsigned char regs[],
				int size)
{
	struct i2c_client *client = v4l2_get_subdevdata(sd);
	int i, err;

	for (i = 0; i < size; i++) {
		err = s5k5ba_i2c_write(sd, &regs[i], sizeof(regs[i]));
		if (err < 0)
			v4l_info(client, "%s: register set failed\n", \
			__func__);
	}

	return 0;	/* FIXME */
}

static const char *s5k5ba_querymenu_wb_preset[] = {
	"WB Tungsten", "WB Fluorescent", "WB sunny", "WB cloudy", NULL
};

static const char *s5k5ba_querymenu_effect_mode[] = {
	"Effect Sepia", "Effect Aqua", "Effect Monochrome",
	"Effect Negative", "Effect Sketch", NULL
};

static const char *s5k5ba_querymenu_ev_bias_mode[] = {
	"-3EV",	"-2,1/2EV", "-2EV", "-1,1/2EV",
	"-1EV", "-1/2EV", "0", "1/2EV",
	"1EV", "1,1/2EV", "2EV", "2,1/2EV",
	"3EV", NULL
};

static struct v4l2_queryctrl s5k5ba_controls[] = {
	{
		/*
		 * For now, we just support in preset type
		 * to be close to generic WB system,
		 * we define color temp range for each preset
		 */
		.id = V4L2_CID_WHITE_BALANCE_TEMPERATURE,
		.type = V4L2_CTRL_TYPE_INTEGER,
		.name = "White balance in kelvin",
		.minimum = 0,
		.maximum = 10000,
		.step = 1,
		.default_value = 0,	/* FIXME */
	},
	{
		.id = V4L2_CID_WHITE_BALANCE_PRESET,
		.type = V4L2_CTRL_TYPE_MENU,
		.name = "White balance preset",
		.minimum = 0,
		.maximum = ARRAY_SIZE(s5k5ba_querymenu_wb_preset) - 2,
		.step = 1,
		.default_value = 0,
	},
	{
		.id = V4L2_CID_AUTO_WHITE_BALANCE,
		.type = V4L2_CTRL_TYPE_BOOLEAN,
		.name = "Auto white balance",
		.minimum = 0,
		.maximum = 1,
		.step = 1,
		.default_value = 0,
	},
	{
		.id = V4L2_CID_EXPOSURE,
		.type = V4L2_CTRL_TYPE_MENU,
		.name = "Exposure bias",
		.minimum = 0,
		.maximum = ARRAY_SIZE(s5k5ba_querymenu_ev_bias_mode) - 2,
		.step = 1,
		.default_value = \
			(ARRAY_SIZE(s5k5ba_querymenu_ev_bias_mode) - 2) / 2,
			/* 0 EV */
	},
	{
		.id = V4L2_CID_COLORFX,
		.type = V4L2_CTRL_TYPE_MENU,
		.name = "Image Effect",
		.minimum = 0,
		.maximum = ARRAY_SIZE(s5k5ba_querymenu_effect_mode) - 2,
		.step = 1,
		.default_value = 0,
	},
	{
		.id = V4L2_CID_CONTRAST,
		.type = V4L2_CTRL_TYPE_INTEGER,
		.name = "Contrast",
		.minimum = 0,
		.maximum = 4,
		.step = 1,
		.default_value = 2,
	},
	{
		.id = V4L2_CID_SATURATION,
		.type = V4L2_CTRL_TYPE_INTEGER,
		.name = "Saturation",
		.minimum = 0,
		.maximum = 4,
		.step = 1,
		.default_value = 2,
	},
	{
		.id = V4L2_CID_SHARPNESS,
		.type = V4L2_CTRL_TYPE_INTEGER,
		.name = "Sharpness",
		.minimum = 0,
		.maximum = 4,
		.step = 1,
		.default_value = 2,
	},
};

const char **s5k5ba_ctrl_get_menu(u32 id)
{
	switch (id) {
	case V4L2_CID_WHITE_BALANCE_PRESET:
		return s5k5ba_querymenu_wb_preset;

	case V4L2_CID_COLORFX:
		return s5k5ba_querymenu_effect_mode;

	case V4L2_CID_EXPOSURE:
		return s5k5ba_querymenu_ev_bias_mode;

	default:
		return v4l2_ctrl_get_menu(id);
	}
}

static inline struct v4l2_queryctrl const *s5k5ba_find_qctrl(int id)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(s5k5ba_controls); i++)
		if (s5k5ba_controls[i].id == id)
			return &s5k5ba_controls[i];

	return NULL;
}

static int s5k5ba_queryctrl(struct v4l2_subdev *sd, struct v4l2_queryctrl *qc)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(s5k5ba_controls); i++) {
		if (s5k5ba_controls[i].id == qc->id) {
			memcpy(qc, &s5k5ba_controls[i], \
				sizeof(struct v4l2_queryctrl));
			return 0;
		}
	}

	return -EINVAL;
}

static int s5k5ba_querymenu(struct v4l2_subdev *sd, struct v4l2_querymenu *qm)
{
	struct v4l2_queryctrl qctrl;

	qctrl.id = qm->id;
	s5k5ba_queryctrl(sd, &qctrl);

	return v4l2_ctrl_query_menu(qm, &qctrl, s5k5ba_ctrl_get_menu(qm->id));
}

/*
 * Clock configuration
 * Configure expected MCLK from host and return EINVAL if not supported clock
 * frequency is expected
 * 	freq : in Hz
 * 	flag : not supported for now
 */
static int s5k5ba_s_crystal_freq(struct v4l2_subdev *sd, u32 freq, u32 flags)
{
	int err = -EINVAL;

	return err;
}

static int s5k5ba_g_fmt(struct v4l2_subdev *sd, struct v4l2_format *fmt)
{
	int err = 0;

	return err;
}

static int s5k5ba_s_fmt(struct v4l2_subdev *sd, struct v4l2_format *fmt)
{
	int err = 0;

	return err;
}
static int s5k5ba_enum_framesizes(struct v4l2_subdev *sd,
					struct v4l2_frmsizeenum *fsize)
{
	struct s5k5ba_state *state = to_state(sd);
	int num_entries = sizeof(s5k5ba_framesize_list) /
				sizeof(struct s5k5ba_enum_framesize);
	struct s5k5ba_enum_framesize *elem;
	int index = 0;
	int i = 0;


	/* The camera interface should read this value, this is the resolution
	 * at which the sensor would provide framedata to the camera i/f
	 *
	 * In case of image capture,
	 * this returns the default camera resolution (WVGA)
	 */
	fsize->type = V4L2_FRMSIZE_TYPE_DISCRETE;

	index = state->framesize_index;

	for (i = 0; i < num_entries; i++) {
		elem = &s5k5ba_framesize_list[i];
		if (elem->index == index) {
			fsize->discrete.width =
			    s5k5ba_framesize_list[index].width;
			fsize->discrete.height =
			    s5k5ba_framesize_list[index].height;
			return 0;
		}
	}

	return -EINVAL;
}

static int s5k5ba_enum_frameintervals(struct v4l2_subdev *sd,
					struct v4l2_frmivalenum *fival)
{
	int err = 0;

	return err;
}

static int s5k5ba_enum_fmt(struct v4l2_subdev *sd, struct v4l2_fmtdesc *fmtdesc)
{
	int err = 0;

	return err;
}

static int s5k5ba_try_fmt(struct v4l2_subdev *sd, struct v4l2_format *fmt)
{
	int err = 0;

	return err;
}

static int s5k5ba_g_parm(struct v4l2_subdev *sd, struct v4l2_streamparm *param)
{
	struct i2c_client *client = v4l2_get_subdevdata(sd);
	int err = 0;

	dev_dbg(&client->dev, "%s\n", __func__);

	return err;
}

static int s5k5ba_s_parm(struct v4l2_subdev *sd, struct v4l2_streamparm *param)
{
	struct i2c_client *client = v4l2_get_subdevdata(sd);
	int err = 0;

	dev_dbg(&client->dev, "%s: numerator %d, denominator: %d\n", \
		__func__, param->parm.capture.timeperframe.numerator, \
		param->parm.capture.timeperframe.denominator);

	return err;
}

static int s5k5ba_g_ctrl(struct v4l2_subdev *sd, struct v4l2_control *ctrl)
{
printk(KERN_INFO "###TEST_XU: %s ...\n",__func__);
	struct i2c_client *client = v4l2_get_subdevdata(sd);
	struct s5k5ba_state *state = to_state(sd);
	struct s5k5ba_userset userset = state->userset;
	int err = -EINVAL;

	switch (ctrl->id) {
	case V4L2_CID_EXPOSURE:
		ctrl->value = userset.exposure_bias;
		err = 0;
		break;

	case V4L2_CID_AUTO_WHITE_BALANCE:
		ctrl->value = userset.auto_wb;
		err = 0;
		break;

	case V4L2_CID_WHITE_BALANCE_PRESET:
		ctrl->value = userset.manual_wb;
		err = 0;
		break;

	case V4L2_CID_COLORFX:
		ctrl->value = userset.effect;
		err = 0;
		break;

	case V4L2_CID_CONTRAST:
		ctrl->value = userset.contrast;
		err = 0;
		break;

	case V4L2_CID_SATURATION:
		ctrl->value = userset.saturation;
		err = 0;
		break;

	case V4L2_CID_SHARPNESS:
		ctrl->value = userset.saturation;
		err = 0;
		break;

	default:
		dev_err(&client->dev, "%s: no such ctrl\n", __func__);
		break;
	}

	return err;
}

static int s5k5ba_s_ctrl(struct v4l2_subdev *sd, struct v4l2_control *ctrl)
{
printk(KERN_INFO "###TEST_XU: %s ...\n",__func__);
#if 0//ifdef S5K5BA_COMPLETE
	struct i2c_client *client = v4l2_get_subdevdata(sd);
	struct s5k5ba_state *state = to_state(sd);
	int err = -EINVAL;

	switch (ctrl->id) {
	case V4L2_CID_EXPOSURE:
		dev_dbg(&client->dev, "%s: V4L2_CID_EXPOSURE\n", __func__);
		err = s5k5ba_write_regs(sd, \
		(unsigned char *) s5k5ba_regs_ev_bias[ctrl->value], \
			sizeof(s5k5ba_regs_ev_bias[ctrl->value]));
		break;

	case V4L2_CID_AUTO_WHITE_BALANCE:
		dev_dbg(&client->dev, "%s: V4L2_CID_AUTO_WHITE_BALANCE\n", \
			__func__);
		err = s5k5ba_write_regs(sd, \
		(unsigned char *) s5k5ba_regs_awb_enable[ctrl->value], \
			sizeof(s5k5ba_regs_awb_enable[ctrl->value]));
		break;

	case V4L2_CID_WHITE_BALANCE_PRESET:
		dev_dbg(&client->dev, "%s: V4L2_CID_WHITE_BALANCE_PRESET\n", \
			__func__);
		err = s5k5ba_write_regs(sd, \
		(unsigned char *) s5k5ba_regs_wb_preset[ctrl->value], \
			sizeof(s5k5ba_regs_wb_preset[ctrl->value]));
		break;

	case V4L2_CID_COLORFX:
		dev_dbg(&client->dev, "%s: V4L2_CID_COLORFX\n", __func__);
		err = s5k5ba_write_regs(sd, \
		(unsigned char *) s5k5ba_regs_color_effect[ctrl->value], \
			sizeof(s5k5ba_regs_color_effect[ctrl->value]));
		break;

	case V4L2_CID_CONTRAST:
		dev_dbg(&client->dev, "%s: V4L2_CID_CONTRAST\n", __func__);
		err = s5k5ba_write_regs(sd, \
		(unsigned char *) s5k5ba_regs_contrast_bias[ctrl->value], \
			sizeof(s5k5ba_regs_contrast_bias[ctrl->value]));
		break;

	case V4L2_CID_SATURATION:
		dev_dbg(&client->dev, "%s: V4L2_CID_SATURATION\n", __func__);
		err = s5k5ba_write_regs(sd, \
		(unsigned char *) s5k5ba_regs_saturation_bias[ctrl->value], \
			sizeof(s5k5ba_regs_saturation_bias[ctrl->value]));
		break;

	case V4L2_CID_SHARPNESS:
		dev_dbg(&client->dev, "%s: V4L2_CID_SHARPNESS\n", __func__);
		err = s5k5ba_write_regs(sd, \
		(unsigned char *) s5k5ba_regs_sharpness_bias[ctrl->value], \
			sizeof(s5k5ba_regs_sharpness_bias[ctrl->value]));
		break;
	case V4L2_CID_CAM_PREVIEW_ONOFF:
		if (state->check_previewdata == 0)
			err = 0;
		else
			err = -EIO;
		break;

	case V4L2_CID_CAMERA_RESET:
		dev_dbg(&client->dev, "%s: V4L2_CID_CAMERA_RESET\n", __func__);
		err = s5k5ba_reset(sd);
		break;	

	default:
		dev_err(&client->dev, "%s: no such control\n", __func__);
		err = 0;
		break;
	}

	if (err < 0)
		goto out;
	else
		return 0;

out:
	dev_dbg(&client->dev, "%s: vidioc_s_ctrl failed\n", __func__);
	return err;
#else
	return 0;
#endif
}

#if 1//modify by xu_bin for write /read reg short
static int s5k5ca_write(struct v4l2_subdev *sd, unsigned short reg,
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

static int s5k5ca_read(struct v4l2_subdev *sd, unsigned short reg,
		unsigned char *value)
{
 struct i2c_client *client = v4l2_get_subdevdata(sd);

int err = -1;
 u8 buf[4] = {0};
 u8 ucData[2] = {0};
 int ret = -1;
 int iCnt = 10;
 
 buf[0] = reg >> 8; 
 buf[1] = reg & 0xff;
 struct i2c_msg msg[2] = { 
      { 
		 .addr = client->addr,
	        .flags = 0 /*| I2C_M_IGNORE_NAK*/,
	        .buf = buf, 
	        .len = 2 
       },//write reg addr msg,write two byte reg addr.
      { 
	      .addr = client->addr,
	      .flags = I2C_M_RD /*| I2C_M_IGNORE_NAK*/,
	      .buf = ucData, 
	      .len = 2 } //read msg ,if read two byte ,then len=2
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
#endif


static int s5k5ba_init(struct v4l2_subdev *sd, u32 val)
{
	struct i2c_client *client = v4l2_get_subdevdata(sd);
	struct s5k5ba_state *state = to_state(sd);
	int err = -EINVAL, i,j;
	
	v4l_info(client, "%s: camera initialization start\n", __func__);
#if 0//modify by xu_bin for test IIC read/rwite &sensor-id
for(i=0;i<10;i++)  //add by xu_bin for test write and read  ,loop 10 times
{			
  		s5k5ca_write(sd,0xFCFC,0xD000);
	       s5k5ca_write(sd,0x0028,0x7000);
	       s5k5ca_write(sd,0x002a,0x2700);
		s5k5ca_write(sd,0x0F12,0x0001);

		s5k5ca_write(sd,0xFCFC,0xD000);
	       s5k5ca_write(sd,0x002C,0x7000);
	       s5k5ca_write(sd,0x002E,0x2700);
	       s5k5ca_read(sd, 0x0F12, &val);
		printk("read value: 0x%x \n", val);
}
//get sensor-id
for(i=0;i<10;i++)//add by xu_bin for get  sensor-id ,loop 10 times
	while(0)
	{
	  	s5k5ca_write(sd,0xFCFC,0xD000);
	       s5k5ca_write(sd,0x002C,0x7000);
	       s5k5ca_write(sd,0x002E,0x0152);
	       s5k5ca_read(sd, 0x0F12, &val);
		printk("read value: 0x%x \n", val);           
	}
#else

for (i = 0; i < S5K5BA_INIT_REGS1; i++) //init camera regs  
{	
	err= s5k5ca_write(sd,s5k5ba_init0[i][0],s5k5ba_init0[i][1]);
	if (err < 0)
		v4l_info(client, "%s: register set failed\n", __func__);
}

mdelay(2000);//delay 20S  

for (i = 0; i < S5K5BA_INIT_REGS; i++) //init camera regs  
{	
	err= s5k5ca_write(sd,s5k5ba_init_reg[i][0],s5k5ba_init_reg[i][1]);
	if (err < 0)
		v4l_info(client, "%s: register set failed\n", __func__);
}
#endif

	if (err < 0) {
		/* This is preview fail */
		state->check_previewdata = 100;
		v4l_err(client,
			"%s: camera initialization failed. err(%d)\n",
			__func__, state->check_previewdata);
		return -EIO;
	}
	v4l_info(client, "%s: registers set success  i =  %d\n", __func__,i);
	/* This is preview success */
	state->check_previewdata = 0;
	return 0;
}

static int nt99250_detect(struct v4l2_subdev *sd)
{
	
	unsigned char v = 0;
	int ret;
	unsigned short pid = 0;
	ret = s5k5ca_read(sd, REG_MIDH, &v);
	printk("var high 0x%x\n", v);
	if (ret < 0)
	{
		printk("ov read error \n");
		return ret;
	}
	pid |= (v << 8);
	v = 0;
	while(1)
	{	
		ret = s5k5ca_read(sd, REG_MIDL, &v);
		printk("var low 0x%x\n", v);
		}

	if (ret < 0)
		return ret;
	pid |= (v & 0xff);
	printk(KERN_INFO "###TEST_XU: %s  pid=%d...\n",__func__,pid);
	if (pid != 0x2501 && pid != 0x2500)
    {
    	
        printk("error: devicr mismatched \n");
        ret = -ENODEV;
		return -1;
    }

	ret = s5k5ba_init(sd, 0);
	if (ret < 0)
	{
		printk("nt99250 init failed \n");
		return ret;
	}
	return 0;
}

/*
 * s_config subdev ops
 * With camera device, we need to re-initialize every single opening time
 * therefor, it is not necessary to be initialized on probe time.
 * except for version checking.
 * NOTE: version checking is optional
 */
static int s5k5ba_s_config(struct v4l2_subdev *sd, int irq, void *platform_data)
{
	struct i2c_client *client = v4l2_get_subdevdata(sd);
	struct s5k5ba_state *state = to_state(sd);
	struct s5k5ba_platform_data *pdata;
	
	dev_info(&client->dev, "fetching platform data\n");

	pdata = client->dev.platform_data;

	if (!pdata) {
		dev_err(&client->dev, "%s: no platform data\n", __func__);
		return -ENODEV;
		
	}

/*
 * Assign default format and resolution
 * Use configured default information in platform data
 * or without them, use default information in driver
*/
	if (!(pdata->default_width && pdata->default_height)) {
		/* TODO: assign driver default resolution */
	} else {
		state->pix.width = pdata->default_width;
		state->pix.height = pdata->default_height;
	}

	if (!pdata->pixelformat)
		state->pix.pixelformat = DEFAULT_FMT;
	else
		state->pix.pixelformat = pdata->pixelformat;

	if (!pdata->freq)
		state->freq = 24000000;	/* 24MHz default */
	else
		state->freq = pdata->freq;

	if (!pdata->is_mipi) {
		state->is_mipi = 0;
		dev_info(&client->dev, "parallel mode\n");
	} else
		state->is_mipi = pdata->is_mipi;

	return 0;
}

static const struct v4l2_subdev_core_ops s5k5ba_core_ops = {
	.init = s5k5ba_init,	/* initializing API */
	.s_config = s5k5ba_s_config,	/* Fetch platform data */
	.queryctrl = s5k5ba_queryctrl,
	.querymenu = s5k5ba_querymenu,
	.g_ctrl = s5k5ba_g_ctrl,
	.s_ctrl = s5k5ba_s_ctrl,
};

static const struct v4l2_subdev_video_ops s5k5ba_video_ops = {
	.s_crystal_freq = s5k5ba_s_crystal_freq,
	.g_fmt = s5k5ba_g_fmt,
	.s_fmt = s5k5ba_s_fmt,
	.enum_framesizes = s5k5ba_enum_framesizes,
	.enum_frameintervals = s5k5ba_enum_frameintervals,
	.enum_fmt = s5k5ba_enum_fmt,
	.try_fmt = s5k5ba_try_fmt,
	.g_parm = s5k5ba_g_parm,
	.s_parm = s5k5ba_s_parm,
};

static const struct v4l2_subdev_ops s5k5ba_ops = {
	.core = &s5k5ba_core_ops,
	.video = &s5k5ba_video_ops,
};

/*
 * s5k5ba_probe
 * Fetching platform data is being done with s_config subdev call.
 * In probe routine, we just register subdev device
 */
static int s5k5ba_probe(struct i2c_client *client,
			 const struct i2c_device_id *id)
{
	struct s5k5ba_state *state;
	struct v4l2_subdev *sd;
	int ret;
	printk(KERN_INFO "###TEST_XU: %s ...\n",__func__);

	state = kzalloc(sizeof(struct s5k5ba_state), GFP_KERNEL);
	if (state == NULL)
		return -ENOMEM;

	sd = &state->sd;
	strcpy(sd->name, S5K5BA_DRIVER_NAME);

	/* Registering subdev */
	v4l2_i2c_subdev_init(sd, client, &s5k5ba_ops);
	
	s5k5ba_write(sd, REG_RESET, 0x00);
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

	dev_info(&client->dev, "s5k5ba has been probed\n");
	return 0;
}


static int s5k5ba_remove(struct i2c_client *client)
{
	struct v4l2_subdev *sd = i2c_get_clientdata(client);

	v4l2_device_unregister_subdev(sd);
	kfree(to_state(sd));
	return 0;
}

static const struct i2c_device_id s5k5ba_id[] = {
	{ S5K5BA_DRIVER_NAME, 0 },
	{ },
};
MODULE_DEVICE_TABLE(i2c, s5k5ba_id);

static struct v4l2_i2c_driver_data v4l2_i2c_data = {
	.name = S5K5BA_DRIVER_NAME,
	.probe = s5k5ba_probe,
	.remove = s5k5ba_remove,
	.id_table = s5k5ba_id,
};

MODULE_DESCRIPTION("Samsung Electronics S5K5BA UXGA camera driver");
MODULE_AUTHOR("Jinsung Yang <jsgood.yang@samsung.com>");
MODULE_LICENSE("GPL");




