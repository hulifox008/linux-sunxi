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

	if(bank>8 || bank<0) return;
	if(pin>31 || pin<0) return;

	pio = (struct sunxi_gpio*)(SW_VA_PORTC_IO_BASE+bank*sizeof(struct sunxi_gpio));
	index = GPIO_CFG_INDEX(pin);
	offset = GPIO_CFG_OFFSET(pin);

	cfg = readl(&pio->cfg[0] + index);
	cfg &= ~(0xf << offset);
	cfg |= func<<offset;
	writel(cfg, &pio->cfg[0] + index);

}

void sunxi_gpio_set_value(int bank, int pin, int value)
{
	struct sunxi_gpio *pio;
	__u32 data;

	pio = (struct sunxi_gpio*)(SW_VA_PORTC_IO_BASE+bank*sizeof(struct sunxi_gpio));
	if(value)
		data = readl(&pio->dat) | (1<<pin);
	else
		data = readl(&pio->dat) & ~(1<<pin);
	writel(data, &pio->dat);

}

int sunxi_gpio_get_func(int bank, int pin)
{
	return 0;
}

void dump_gpio(void)
{
	int i;
	__u32 *p = SW_VA_PORTC_IO_BASE;
	printk("\n");
	for(i=0;i<18;i++)
	{
		printk("%08X ", p[i]);
		if((i+1)%4 == 0) printk("\n");
	}
	printk("\n");
}

int gpio_init(void)
{
	printk("GPIO Initializing.\n");
	sunxi_gpio_set_func(SUNXI_GPIO_B, 22, SUN4I_GPB22_UART0_TX);
	sunxi_gpio_set_func(SUNXI_GPIO_B, 23, SUN4I_GPB23_UART0_RX);
#ifdef CONFIG_SW_SATA_AHCI_PLATFORM
	sunxi_gpio_set_func(SUNXI_GPIO_B, 8, SUNXI_GPIO_OUTPUT);
	sunxi_gpio_set_value(SUNXI_GPIO_B, 8, 1);
#endif
	return 0;
}
arch_initcall(gpio_init);
