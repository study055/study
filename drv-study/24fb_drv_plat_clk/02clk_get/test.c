#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h> //kzalloc
#include <linux/clk.h>
#include <plat/clock.h> //clk类的完整声明
#include <linux/err.h>

#include <linux/platform_device.h>

//匹配位于mach-tiny4412.c中line 459的platform_device

struct ldm_info {
	struct clk *gate_clk;
	struct clk *sclk_fimd;
};

static int ldm_probe(struct platform_device *pdev)
{
	int ret = 0;

	struct ldm_info *ldm = kzalloc(sizeof(struct ldm_info), GFP_KERNEL);
	//if (!ldm) {
	//}

	//私有数据通过pdev对象传递给出口函数做资源释放
	platform_set_drvdata(pdev, ldm);

	//1 提取出pdev中resource里包含的寄存器物理地址
	struct resource *res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		printk(KERN_WARNING "platform_get_resource regs addr failed\n");
		ret = -EINVAL;
		goto err_platform_get_resource;
	}
	printk("lcd regs start addr = 0x%x, size = 0x%x\n", res->start, res->end - res->start + 1);

	//2 从clock_exynos4.c中获取clk gate对应的clk对象，并打开使能
	ldm->gate_clk = clk_get(&pdev->dev, "lcd");
	if (IS_ERR(ldm->gate_clk)) {
		printk(KERN_WARNING "clk_get lcd failed\n");
		ret = PTR_ERR(ldm->gate_clk);
		goto err_clk_get_lcd;
	}

	//打开lcd控制器所在的总线的gate使能
	clk_enable(ldm->gate_clk);

	//3 获取lcd控制器所在的总线的频率
	ldm->sclk_fimd = clk_get(&pdev->dev, "sclk_fimd");
	if (IS_ERR(ldm->sclk_fimd)) {
		printk(KERN_WARNING "clk_get lcd rate failed\n");
		ret = PTR_ERR(ldm->sclk_fimd);
		goto err_clk_get_lcd_rate;
	}

	unsigned long clk_rate = clk_get_rate(ldm->sclk_fimd);
	printk("lcd clk rate = %lu\n", clk_rate);

	clk_enable(ldm->sclk_fimd);

	return 0;

err_clk_get_lcd_rate:
	clk_disable(ldm->gate_clk);
err_clk_get_lcd:
err_platform_get_resource:
	return ret;
}

static int ldm_remove(struct platform_device *pdev)
{
	struct ldm_info *ldm = platform_get_drvdata(pdev);

	clk_disable(ldm->sclk_fimd);
	clk_disable(ldm->gate_clk);

	kfree(ldm);

	return 0;
}

static struct platform_device_id ldm_id[] = {
	{"exynos4-fb"},
	{""},
};

static struct platform_driver ldm_drv = {
	.probe = ldm_probe,
	.remove = ldm_remove,
	.driver = {
		.name = "abc", //不做匹配关键词但也不能为NULL
	},
	.id_table = ldm_id,
};

static int __init ldm_init(void)
{
	printk(KERN_ALERT "%s\n", __FUNCTION__);

	return platform_driver_register(&ldm_drv);;
}

static void __exit ldm_exit(void)
{
	printk(KERN_ALERT "%s\n", __FUNCTION__);

	platform_driver_unregister(&ldm_drv);
}

module_init(ldm_init);
module_exit(ldm_exit);

MODULE_LICENSE("GPL");