#pragma once

#define RGB888(r, g, b) ( (r) << 16 | (g) << 8 | (b))

struct win_info {

#define RGB565_BPP_MODE 0b0101
#define RGB888_BPP_MODE 0b1011
#define PAL8_BPP_MODE 0b0011
	u8 bpp;

	size_t pixel_size; //每个像素占用的字节数

#define RGB565_SWAP_MODE 0b0010
#define PAL8_SWAP_MODE 0b0100
	u8 swap_mode; //相邻的几个像素的排列顺序,看手册, 如果是RGB565，看P1779,如果是RGB888，P1774

	u32 start_addr; //显存起始地址
	u8 alpha_val; //4位alpha值,数值越小，透明度越高

	/* @x, y: 画出的对应像素点的坐标
	 * @color: RGB888格式的颜色值
	 */
	void (*draw_pixel)(struct win_info *, size_t x, size_t y, u32 color);
};

extern struct win_info win[];

void draw_rect(struct win_info *info, size_t x, size_t y, size_t xlen, size_t ylen, u32 color);