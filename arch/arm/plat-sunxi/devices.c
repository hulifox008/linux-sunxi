/*
 * arch/arm/plat-sunxi/devices.c
 *
 * (C) Copyright 2007-2012
 * Allwinner Technology Co., Ltd. <www.allwinnertech.com>
 * Benn Huang <benn@allwinnertech.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/serial_8250.h>
#include <linux/clk.h>
#include <linux/dma-mapping.h>
#include <linux/pda_power.h>
#include <linux/io.h>
#include <linux/i2c.h>

#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/mach/time.h>
#include <asm/setup.h>
#include <asm/pmu.h>
#include <mach/hardware.h>
#include <plat/i2c.h>
#include <plat/gpio.h>

/* Register base define */
#define UART_BASE       SW_PA_UART0_IO_BASE 
#define UART_BASE_OS    (0x400)
#define UARTx_BASE(x)   (UART_BASE + (x) * UART_BASE_OS)

static struct resource sw_uart_res[8][2] = {
    {/* uart0 resource */
        {.start = UARTx_BASE(0),      .end = UARTx_BASE(0) + UART_BASE_OS - 1, .flags = IORESOURCE_MEM}, /*base*/
        {.start = SW_INT_IRQNO_UART0, .end = SW_INT_IRQNO_UART0,           .flags = IORESOURCE_IRQ}, /*irq */
    },
    {/* uart1 resource */
        {.start = UARTx_BASE(1),      .end = UARTx_BASE(1) + UART_BASE_OS - 1, .flags = IORESOURCE_MEM}, /*base*/
        {.start = SW_INT_IRQNO_UART1, .end = SW_INT_IRQNO_UART1,           .flags = IORESOURCE_IRQ}, /*irq */
    },
    {/* uart2 resource */
        {.start = UARTx_BASE(2),      .end = UARTx_BASE(2) + UART_BASE_OS - 1, .flags = IORESOURCE_MEM}, /*base*/
        {.start = SW_INT_IRQNO_UART2, .end = SW_INT_IRQNO_UART2,           .flags = IORESOURCE_IRQ}, /*irq */
    },
    {/* uart3 resource */
        {.start = UARTx_BASE(3),      .end = UARTx_BASE(3) + UART_BASE_OS - 1, .flags = IORESOURCE_MEM}, /*base*/
        {.start = SW_INT_IRQNO_UART3, .end = SW_INT_IRQNO_UART3,           .flags = IORESOURCE_IRQ}, /*irq */
    },
    {/* uart4 resource */
        {.start = UARTx_BASE(4),      .end = UARTx_BASE(4) + UART_BASE_OS - 1, .flags = IORESOURCE_MEM}, /*base*/
        {.start = SW_INT_IRQNO_UART4, .end = SW_INT_IRQNO_UART4,           .flags = IORESOURCE_IRQ}, /*irq */
    },
    {/* uart5 resource */
        {.start = UARTx_BASE(5),      .end = UARTx_BASE(5) + UART_BASE_OS - 1, .flags = IORESOURCE_MEM}, /*base*/
        {.start = SW_INT_IRQNO_UART5, .end = SW_INT_IRQNO_UART5,           .flags = IORESOURCE_IRQ}, /*irq */
    },
    {/* uart6 resource */
        {.start = UARTx_BASE(6),      .end = UARTx_BASE(6) + UART_BASE_OS - 1, .flags = IORESOURCE_MEM}, /*base*/
        {.start = SW_INT_IRQNO_UART6, .end = SW_INT_IRQNO_UART6,           .flags = IORESOURCE_IRQ}, /*irq */
    },
    {/* uart7 resource */
        {.start = UARTx_BASE(7),      .end = UARTx_BASE(7) + UART_BASE_OS - 1, .flags = IORESOURCE_MEM}, /*base*/
        {.start = SW_INT_IRQNO_UART7, .end = SW_INT_IRQNO_UART7,           .flags = IORESOURCE_IRQ}, /*irq */
    },
};

static struct platform_device sw_uart0_dev = {
	.name = "sunxi-uart", 
	.id = 0,
	.num_resources = ARRAY_SIZE(sw_uart_res[0]),
	.resource = &sw_uart_res[0][0], 
	.dev={}
};

/* mmc */
static struct resource sunximmc_resources[2] = {
	{ 
	.start = SW_PA_SDC0_IO_BASE,      
	.end = SW_PA_SDC0_IO_BASE+0x1000-1,  
	.flags	= IORESOURCE_MEM
	},  /* reg resource */
	
	{ 
	.start	= SW_INT_IRQNO_SDMC0,  
	.end = SW_INT_IRQNO_SDMC0,     
	.flags	= IORESOURCE_IRQ
	}  /* irq resource */
};

static struct platform_device awmmc0_device = {
	.name = "sunxi-mmc",
	.id = 0,
	.num_resources = ARRAY_SIZE(sunximmc_resources),
	.resource = &sunximmc_resources[0],
	.dev={}
};

/* wemac */
static struct resource wemac_resources[] = {
	[0] = {
		.start	= SW_PA_EMAC_IO_BASE,
		.end	= SW_PA_EMAC_IO_BASE+1024,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= SW_PA_SRAM_IO_BASE,
		.end	= SW_PA_SRAM_IO_BASE+1024,
		.flags	= IORESOURCE_MEM,
	},
	[2] = {
		.start	= PIO_BASE,
		.end	= PIO_BASE+1024,
		.flags	= IORESOURCE_MEM,
	},
	[3] = {
		.start	= SW_PA_CCM_IO_BASE,
		.end	= SW_PA_CCM_IO_BASE+1024,
		.flags	= IORESOURCE_MEM,
	},
	[4] = {
		.start	= SW_INT_IRQNO_EMAC,
		.end	= SW_INT_IRQNO_EMAC,
		.flags	= IORESOURCE_IRQ,
	}
};

static struct platform_device wemac_device = {
	.name		= "wemac",
	.id		= 0,
	.num_resources	= ARRAY_SIZE(wemac_resources),
	.resource	= wemac_resources,
	.dev		= {
	}
};

/* ahci */
static struct resource sw_ahci_resources[] = {
	[0] = {
		.start		= SW_PA_SATA_IO_BASE,
		.end		= SW_PA_SATA_IO_BASE + 0x1000 - 1,
		.flags		= IORESOURCE_MEM,
	},

	[1] = {
    	.start	= SW_INT_IRQNO_SATA,
    	.end	= SW_INT_IRQNO_SATA,
    	.flags	= IORESOURCE_IRQ,
    },
};

struct platform_device sw_ahci_device  = {
	.name		= "sw_ahci",
	.id			= 0,
	.dev 		= {
	},
	.num_resources	= ARRAY_SIZE(sw_ahci_resources),
	.resource		= sw_ahci_resources,
};

/* spdif */
static struct platform_device sun4i_spdif_device = {
	.name = "sun4i-spdif",
};

/* dma */
static struct platform_device sw_pdev_dmac = {
	.name = "sw_dmac",
};

/* nand */
static struct resource sw_res_nand =
{
	.start = SW_PA_NANDFLASHC_IO_BASE,
	.end = SW_PA_NANDFLASHC_IO_BASE + 0x1000,
	.flags = IORESOURCE_MEM,
};

struct platform_device sw_pdev_nand =
{
	.name = "sw_nand",
	.id = -1,
	.num_resources = 1,
	.resource = &sw_res_nand,
	.dev = {}
};

/* twi0 */
static struct sunxi_i2c_platform_data sunxi_twi0_pdata[] = {
	{
		.bus_num   = 0,
		.frequency = I2C0_TRANSFER_SPEED,
	},
};

static struct resource sunxi_twi0_resources[] = {
	{
		.start	= TWI0_BASE_ADDR_START,
		.end	= TWI0_BASE_ADDR_END,
		.flags	= IORESOURCE_MEM,
	}, {
		.start	= SW_INT_IRQNO_TWI0,
		.end	= SW_INT_IRQNO_TWI0,
		.flags	= IORESOURCE_IRQ,
	},
};

struct platform_device sunxi_twi0_device = {
	.name		= "sunxi-i2c",
	.id		    = 0,
	.resource	= sunxi_twi0_resources,
	.num_resources	= ARRAY_SIZE(sunxi_twi0_resources),
	.dev = {
		.platform_data = sunxi_twi0_pdata,
	},
};

/* twi1 */
static struct sunxi_i2c_platform_data sunxi_twi1_pdata[] = {
	{
		.bus_num   = 1,
    	.frequency = I2C1_TRANSFER_SPEED,
	},
};

static struct resource sunxi_twi1_resources[] = {
	{
		.start	= TWI1_BASE_ADDR_START,
		.end	= TWI1_BASE_ADDR_END,
		.flags	= IORESOURCE_MEM,
	}, {
		.start	= SW_INT_IRQNO_TWI1,
		.end	= SW_INT_IRQNO_TWI1,
		.flags	= IORESOURCE_IRQ,
	},
};

struct platform_device sunxi_twi1_device = {
	.name		= "sunxi-i2c",
	.id		    = 1,
	.resource	= sunxi_twi1_resources,
	.num_resources	= ARRAY_SIZE(sunxi_twi1_resources),
	.dev = {
		.platform_data = sunxi_twi1_pdata,
	},
};

/* twi2 */
static struct sunxi_i2c_platform_data sunxi_twi2_pdata[] = {
	{
		.bus_num   = 2,
    	.frequency = I2C2_TRANSFER_SPEED,
	},
};

static struct resource sunxi_twi2_resources[] = {
	{
		.start	= TWI2_BASE_ADDR_START,
		.end	= TWI2_BASE_ADDR_END,
		.flags	= IORESOURCE_MEM,
	}, {
		.start	= SW_INT_IRQNO_TWI2,
		.end	= SW_INT_IRQNO_TWI2,
		.flags	= IORESOURCE_IRQ,
	},
};

struct platform_device sunxi_twi2_device = {
	.name		= "sunxi-i2c",
	.id		    = 2,
	.resource	= sunxi_twi2_resources,
	.num_resources	= ARRAY_SIZE(sunxi_twi2_resources),
	.dev = {
		.platform_data = sunxi_twi2_pdata,
	},
};

static struct resource sunxi_pmu_resources[] = {
	{
		.start	= SW_INT_IRQNO_PLE_PFM,
		.end	= SW_INT_IRQNO_PLE_PFM,
		.flags	= IORESOURCE_IRQ,
	},
};

struct platform_device sunxi_pmu_device = {
	.name		= "arm-pmu",
	.id		= ARM_PMU_DEVICE_CPU,
	.resource	= sunxi_pmu_resources,
	.num_resources	= ARRAY_SIZE(sunxi_pmu_resources),
};


#if defined(CONFIG_MALI_DRM) || defined(CONFIG_MALI_DRM_MODULE)
static struct platform_device sunxi_device_mali_drm = {
	.name = "mali_drm",
	.id   = -1,
};
#endif

static struct platform_device *sw_pdevs[] __initdata = {
	&sw_uart0_dev,
	&sw_pdev_dmac,
	&sw_pdev_nand,
    &sw_ahci_device,
	&awmmc0_device,
    &wemac_device,
	&sunxi_twi0_device,
	&sunxi_twi1_device,
	&sunxi_twi2_device,
	&sunxi_pmu_device,
#if defined(CONFIG_MALI_DRM) || defined(CONFIG_MALI_DRM_MODULE)
	&sunxi_device_mali_drm,
#endif

};

void __init sw_pdev_init(void)
{
	platform_add_devices(sw_pdevs, ARRAY_SIZE(sw_pdevs));
}
