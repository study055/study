#include <stdio.h>
#include <stdlib.h>

//将2进制文件封装成可以被E4412_N.bl1.bin加载执行的文件
//目标文件固定大小为14K，在生成的目标文件的14K-4的位置，写入校验码（将源文件的前14k-4相加而得），如果源文件小于14k-4，那么在目标文件中填0

#define SOURCE_DATA_SIZE (14 * 1024 - 4)

typedef unsigned int u32;

int main(int argc, char **argv)
{
	if (argc != 3) {
		fprintf(stderr, "Usage: mkbl2 <source bin file> <dest bin file>\n");
		return -1;
	}

	FILE *fp_src = fopen(argv[1], "r");
	if (!fp_src) {
		perror("fopen src file");
		goto err_fopen_src;
	}

	FILE *fp_dst = fopen(argv[2], "w");
	if (!fp_dst) {
		perror("create dst file");
		goto err_fopen_dst;
	}

	//在内存中创建缓冲区用来存放源文件的前14k-4的内容
	unsigned char *buf = calloc(1, SOURCE_DATA_SIZE);

	u32 file_size = fread(buf, 1, SOURCE_DATA_SIZE, fp_src);

	//计算checksum，将buf内容全部相加
	u32 checksum = 0;
	size_t i = 0;
	for (i = 0; i < file_size; ++i) {
		checksum += buf[i];
	}

	fwrite(buf, SOURCE_DATA_SIZE, 1, fp_dst);
	fwrite(&checksum, sizeof(checksum), 1, fp_dst);

	fclose(fp_dst);
	fclose(fp_src);

	return 0;

err_fopen_dst:
	fclose(fp_src);
err_fopen_src:
	return -1;
}
