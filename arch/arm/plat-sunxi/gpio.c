#include <linux/kernel.h>
#include <linux/io.h>
#include <plat/gpio.h>
#include <plat/platform.h>

struct sunxi_gpio {
	__u32 cfg[4];
	__u32 dat;
	__u32 drv[2];
	__u32 pull[2];
}__attribute__((packed));

#define GPIO_CFG_INDEX(pin) (((pin) & 0x1F) >> 3)
#define GPIO_CFG_OFFSET(pin) (((pin) & 0x07) << 2)


void sunxi_gpio_set_func(int bank, int pin, int func)
{
	__u32 cfg;
	__u32 index;
	__u32 offset;
	struct sunxi_gpio *pio;

	pio = (struct sunxi_gpio*)(SW_VA_PORTC_IO_BASE+bank*sizeof(struct sunxi_gpio));

	if(bank>8 || bank<0) return;
	if(pin>31 || pin<0) return;

	index = GPIO_CFG_INDEX(pin);
	offset = GPIO_CFG_OFFSET(pin);

	cfg = readl(&pio->cfg[0] + index);
	cfg &= ~(0xf << offset);
	writel(cfg, &pio->cfg[0] + offset);

}

int sunxi_gpio_get_func(int bank, int pin)
{
	return 0;
}

int gpio_init(void)
{
}
arch_initcall(gpio_init);
