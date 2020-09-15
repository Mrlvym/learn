/*
 * (C) Copyright 2002-2006
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/*
 * To match the U-Boot user interface on ARM platforms to the U-Boot
 * standard (as on PPC platforms), some messages with debug character
 * are removed from the default U-Boot build.
 *
 * Define DEBUG here if you want additional info as shown below
 * printed upon startup:
 *
 * U-Boot code: 00F00000 -> 00F3C774  BSS: -> 00FC3274
 * IRQ Stack: 00ebff7c
 * FIQ Stack: 00ebef7c
 */

#include <common.h>
#include <command.h>
#include <malloc.h>
#include <asm/io.h>
#include <movi.h>
#include <stdio_dev.h>
#include <timestamp.h>
#include <version.h>
#include <net.h>
#include <serial.h>
#include <nand.h>
#include <onenand_uboot.h>
#include <s5pc210.h>
#ifdef CONFIG_GENERIC_MMC
#include <mmc.h>
#endif
#undef DEBUG

#ifdef CONFIG_BITBANGMII
#include <miiphy.h>
#endif

#ifdef CONFIG_DRIVER_SMC91111
#include "../drivers/net/smc91111.h"
#endif
#ifdef CONFIG_DRIVER_LAN91C96
#include "../drivers/net/lan91c96.h"
#endif

#ifdef CONFIG_RECOVERY
extern int recovery_preboot(void);
#endif
DECLARE_GLOBAL_DATA_PTR;

ulong monitor_flash_len;

#ifdef CONFIG_HAS_DATAFLASH
extern int  AT91F_DataflashInit(void);
extern void dataflash_print_info(void);
#endif

#ifndef CONFIG_IDENT_STRING
#define CONFIG_IDENT_STRING ""
#endif

const char version_string[] =
	U_BOOT_VERSION" (" U_BOOT_DATE " - " U_BOOT_TIME ")"CONFIG_IDENT_STRING;

#ifdef CONFIG_DRIVER_RTL8019
extern void rtl8019_get_enetaddr (uchar * addr);
#endif

#if defined(CONFIG_HARD_I2C) || \
    defined(CONFIG_SOFT_I2C)
#include <i2c.h>
#endif


/************************************************************************
 * Coloured LED functionality
 ************************************************************************
 * May be supplied by boards if desired
 */
void inline __coloured_LED_init (void) {}
void coloured_LED_init (void) __attribute__((weak, alias("__coloured_LED_init")));
void inline __red_LED_on (void) {}
void red_LED_on (void) __attribute__((weak, alias("__red_LED_on")));
void inline __red_LED_off(void) {}
void red_LED_off(void) __attribute__((weak, alias("__red_LED_off")));
void inline __green_LED_on(void) {}
void green_LED_on(void) __attribute__((weak, alias("__green_LED_on")));
void inline __green_LED_off(void) {}
void green_LED_off(void) __attribute__((weak, alias("__green_LED_off")));
void inline __yellow_LED_on(void) {}
void yellow_LED_on(void) __attribute__((weak, alias("__yellow_LED_on")));
void inline __yellow_LED_off(void) {}
void yellow_LED_off(void) __attribute__((weak, alias("__yellow_LED_off")));
void inline __blue_LED_on(void) {}
void blue_LED_on(void) __attribute__((weak, alias("__blue_LED_on")));
void inline __blue_LED_off(void) {}
void blue_LED_off(void) __attribute__((weak, alias("__blue_LED_off")));

/************************************************************************
 * Init Utilities							*
 ************************************************************************
 * Some of this code should be moved into the core functions,
 * or dropped completely,
 * but let's get it working (again) first...
 */

#if defined(CONFIG_ARM_DCC) && !defined(CONFIG_BAUDRATE)
#define CONFIG_BAUDRATE 115200
#endif
static int init_baudrate (void)
{
	char tmp[64];	/* long enough for environment variables */
	int i = getenv_r ("baudrate", tmp, sizeof (tmp));
	gd->bd->bi_baudrate = gd->baudrate = (i > 0)
			? (int) simple_strtoul (tmp, NULL, 10)
			: CONFIG_BAUDRATE;

	return (0);
}

static int off_charge(void)
{
	volatile int value = 0;
	int i,j;
	int val;

	/*---only evt use this---*/
	val = readl(GPX2PUD);
	val &= ~(0x3<<12); //dok,pullup/down disable
	writel(val,GPX2PUD);
	
	/*---volume key setting---*/
	__REG(GPL2CON) = __REG(GPL2CON)|0x1<<0;
	__REG(GPL2DAT) = __REG(GPL2DAT)|0x1<<0;//output 1

	__REG(GPX2CON) = __REG(GPX2CON)&~0xff; //gpx2.0,gpx2.1

	val = readl(GPX2PUD);
	val &= ~(0xf);
	val |= (0x5);
	writel(val,GPX2PUD);

	
	/*---gpx2.7 cok ---setting--*/
	val = readl(GPX2CON);
	val &= ~(0xf<<28);
	writel(val,GPX2CON);
	
	val = readl(GPX2PUD);
	val &= ~(0x3<<14); //cok gpx2.7,pullup/down disable
	writel(val,GPX2PUD);


	/*---this don't need ---*/
	val = readl(GPX1PUD);
	val &= ~(0x3<<10); //uok,pullup/down disable
	writel(val,GPX1PUD);
	

	/*---on key setting gpx0.2--*/
	val = readl(GPX0PUD);
	val &= ~(0x3<<4); //on_key,pullup/down disable
	writel(val,GPX0PUD);
	
	return 0;
}

static int display_banner (void)
{
	printf ("\n\n%s\n\n", version_string);
	debug ("U-Boot code: %08lX -> %08lX  BSS: -> %08lX\n",
	       _armboot_start, _bss_start, _bss_end);
#ifdef CONFIG_MODEM_SUPPORT
	debug ("Modem Support enabled\n");
#endif
#ifdef CONFIG_USE_IRQ
	debug ("IRQ Stack: %08lx\n", IRQ_STACK_START);
	debug ("FIQ Stack: %08lx\n", FIQ_STACK_START);
#endif

	return (0);
}

/*
 * WARNING: this code looks "cleaner" than the PowerPC version, but
 * has the disadvantage that you either get nothing, or everything.
 * On PowerPC, you might see "DRAM: " before the system hangs - which
 * gives a simple yet clear indication which part of the
 * initialization if failing.
 */
static int display_dram_config (void)
{
	int i;

#ifdef DEBUG
	puts ("RAM Configuration:\n");

	for(i=0; i<CONFIG_NR_DRAM_BANKS; i++) {
		printf ("Bank #%d: %08lx ", i, gd->bd->bi_dram[i].start);
		print_size (gd->bd->bi_dram[i].size, "\n");
	}
#else
	ulong size = 0;

	for (i=0; i<CONFIG_NR_DRAM_BANKS; i++) {
		size += gd->bd->bi_dram[i].size;
	}

#if  defined(CONFIG_SCP_1GDDR) ||  defined(CONFIG_SCP_2GDDR) || defined(CONFIG_SCP_1GDDR_Ubuntu) || defined(CONFIG_SCP_2GDDR_Ubuntu)  //add by dg

	size += 0x100000;
#endif

#if defined(CONFIG_POP_2GDDR) || defined(CONFIG_POP_2GDDR_Ubuntu) 
	puts("DRAM: ");
    puts("2G\n");
#else
  	puts("DRAM:	");
	print_size(size, "\n");
#endif
#endif

	return (0);
}

#ifndef CONFIG_SYS_NO_FLASH
static void display_flash_config (ulong size)
{
	puts ("Flash: ");
	print_size (size, "\n");
}
#endif /* CONFIG_SYS_NO_FLASH */

#if defined(CONFIG_HARD_I2C) || defined(CONFIG_SOFT_I2C)
static int init_func_i2c (void)
{
	puts ("I2C:   ");
	i2c_init (CONFIG_SYS_I2C_SPEED, CONFIG_SYS_I2C_SLAVE);
	puts ("ready\n");
	return (0);
}
#endif

#if defined(CONFIG_CMD_PCI) || defined (CONFIG_PCI)
#include <pci.h>
static int arm_pci_init(void)
{
	pci_init();
	return 0;
}
#endif /* CONFIG_CMD_PCI || CONFIG_PCI */

/*
 * Breathe some life into the board...
 *
 * Initialize a serial port as console, and carry out some hardware
 * tests.
 *
 * The first part of initialization is running from Flash memory;
 * its main purpose is to initialize the RAM so that we
 * can relocate the monitor code to RAM.
 */

/*
 * All attempts to come up with a "common" initialization sequence
 * that works for all boards and architectures failed: some of the
 * requirements are just _too_ different. To get rid of the resulting
 * mess of board dependent #ifdef'ed code we now make the whole
 * initialization sequence configurable to the user.
 *
 * The requirements for any new initalization function is simple: it
 * receives a pointer to the "global data" structure as it's only
 * argument, and returns an integer return code, where 0 means
 * "continue" and != 0 means "fatal error, hang the system".
 */

/*将返回值为int类型，形参为void类型的一个函数类型重命名为init_fnc_t*/
typedef int (init_fnc_t) (void);

int print_cpuinfo (void);

init_fnc_t *init_sequence[] = {
#if defined(CONFIG_ARCH_CPU_INIT)//这个宏定义应该没有定义（因为CPU相关的初始化在uboot第一阶段已经完成了）
	arch_cpu_init,		/* basic arch cpu dependent setup */
#endif
	board_init,		/* basic board dependent setup */ //开发板相关的初始化（主要做了网卡的初始化和DDR初始化）
	
	/*********************************************
		board_init在board\samsung\smdkc210\smdkc210.c文件中
		
		int board_init(void)
		{
			//这里是声明了一下gd变量，因为下面有使用到gd变量
			DECLARE_GLOBAL_DATA_PTR;
		//网卡相关初始化（使用的是SMC911X网卡）
		#ifdef CONFIG_DRIVER_SMC911X
			smc9115_pre_init();
			smc9115_pre_init这个函数中主要是网卡的GPIO相关的配置工作，应为SMC911X网卡的驱动是
			现成的，我们所要做的是进行SMC911X网卡的驱动移植，只需要修改相关的硬件配置就可以了
		#endif
		
		//网卡初始化使用的是（DM9000网卡）
		#ifdef CONFIG_DRIVER_DM9000
		//	dm9000_pre_init();
		#endif

			下面是关于初试化DDR的背景知识
			在这里的初始化DDR和我们在汇编阶段lowlevel_init中初始化DDR是不同的，
			当时是硬件的初始化为了让DDR工作起来，现在是软件结构中DDR相关的属性
			配置，和地址设置相关的初始化。是纯软件层面的。
			软件层次初始化DDR的原因是：对于uboot来说，他怎么知道开发板上到底有几片DDR内存，
			每一片的地址、长度信息呢？在uboot的设计中采用了一种简单直接有效的方式：程序员
			在移植uboot时，程序员自己再smdkc210.h文件中用宏定义（MACH_TYPE、PHYS_SDRAM_1）的
			方式配置出DDR内存的信息，然后uboot只需要读取这些配置信息就可以了。（实际上还有一种
			思路：就是uboot通过代码去读取硬件信息来知道DDR配置，但是uboot没有，但是电脑的bios
			用的是这种方式）
			
			smdkc210.h文件中的配置如下：
			配置了有几片DDR内存，每一片DDR内存的地址、长度。这些配置信息我们在uboot代码中使用到
			内存的时候就可以从这里提取使用（想象我们uboot使用内存时使用的都是宏定义而不是直接使用数值）
			
			#define CONFIG_NR_DRAM_BANKS    2          /* we have 2 bank of DRAM */
			#define SDRAM_BANK_SIZE         0x20000000    /* 512 MB */
			#define PHYS_SDRAM_1            MEMORY_BASE_ADDRESS /* SDRAM Bank #1 */
			#define PHYS_SDRAM_1_SIZE       SDRAM_BANK_SIZE
			#define PHYS_SDRAM_2            (MEMORY_BASE_ADDRESS + SDRAM_BANK_SIZE) /* SDRAM Bank #2 */
			#define PHYS_SDRAM_2_SIZE       SDRAM_BANK_SIZE
			
			//bi_arch_number代表开发板的机器码（开发板的唯一编号）
			//机器码的作用是在uboot和linux内核之间进行比对和配置。（因为嵌入式的定制化太高了）
			//在uboot给linux传参的时候机器码会传递给linux内核，然后linux内核会跟自身机器码对比
			//（机器码必须要保证uboot和linux相同）
			gd->bd->bi_arch_number = MACH_TYPE;
			//bi_boot_params表示uboot给linux传参的内存地址，uboot事先将准备好的传参放在我们内存的一个地址处
			//（bi_boot_params），然后linux就启动了内核，真正是通过寄存器r0，r1，r2来进行传递的其中有一个寄存器
			//里放的就是bi_boot_params，内核读取寄存器的值，就知道了传递参数的地址，就去拿uboot传递的参数。
			gd->bd->bi_boot_params = (PHYS_SDRAM_1+0x100);

			return 0;
		}
	*********************************************/
//#if defined(CONFIG_USE_IRQ)
	interrupt_init,		/* set up exceptions */	//看名字是中断相关的初始化，实际是定时器相关初始化
	/****************************************************************
		interrupt_init函数在cpu\arm_cortexa9\s5pc210\interrupt.c中
		这里面初试化了TIM4，因为TIM4没有中断功能，因此采用了轮询的方式来实现
		uboot的bootdelay的定时时间。	
	*****************************************************************/
//#endif
	//timer_init,		/* initialize timer */
#ifdef CONFIG_FSL_ESDHC
	//get_clocks,
#endif
	env_init,		/* initialize environment */ //环境变量初始化相关
	/*******************************************************************
		env_init函数在common\env_nand.c中
		有很多env_init函数，是应为uboot的启动介质有很多种（iNand、emmc、sd卡等）,每种不同的介质在
		存取env时候的方式是不同的所以才导致了有很多的env_init函数。
		
		env_init函数对DDR中维护的那一份环境变量做了基本的判定，判定里面有没有能用的环境变量，
		当前因为我们还没有做从SD卡到DDR中的重定位，所以这些环境变量还不能够使用。
	********************************************************************/
	init_baudrate,		/* initialze baudrate settings */	//串口通信波特率相关的初始化
	/*************************************************************************************
		init_baudrate函数在lib_arm\board.c中
		
		static int init_baudrate (void)
		{
			char tmp[64];	/* long enough for environment variables */
			int i = getenv_r ("baudrate", tmp, sizeof (tmp));
			gd->bd->bi_baudrate = gd->baudrate = (i > 0)
					? (int) simple_strtoul (tmp, NULL, 10)
					: CONFIG_BAUDRATE;

			return (0);
		}
		
		重点关注getenv_r ("baudrate", tmp, sizeof (tmp));这个函数，这个函数是用来读取环境变量值的函数
		读取回来得值会存放在tmp数组中，都是字符串形式，需要转换一下。
		
	*************************************************************************************/
	serial_init,		/* serial communications setup */	//串口相关初始化（在汇编阶段串口已经初始化过硬件寄存器了）
	/*************************************************************************
		serial_init函数在cpu\arm_cortexa9\s5pc210\serial.c中
		其实什么也没有做
	*************************************************************************/
	console_init_f,		/* stage 1 init of console */  //控制台相关初始化（_f是代表分阶段初始化）
	/*********************************************************************************
		console_init_f函数在common\console.c中
		int console_init_f(void)
		{
			gd->have_console = 1; //将这个代表开启控制台

		#ifdef CONFIG_SILENT_CONSOLE
			if (getenv("silent") != NULL)
				gd->flags |= GD_FLG_SILENT;
		#endif

			return 0;
		}
			
	*********************************************************************************/
	off_charge,		// xiebin.wang @ 20110531,for charger&power off device.

	display_banner,		/* say that we are here */	//用来串口打印uboot的logo（版本号）
#if defined(CONFIG_DISPLAY_CPUINFO)
	print_cpuinfo,		/* display cpu info (and speed) */  //用串口打印CPU配置信息
	/***************************************************************************
		print_cpuinfo函数在cpu\arm_cortexa9\s5pc210\speed.c文件中
		里面有get_PLLCLK、get_APLL_CLK、get_MPLL_CLK、get_ARM_CLK这几个函数
		都是自动获取我们cpu相关的配置信息的函数
	****************************************************************************/
#endif
#if defined(CONFIG_DISPLAY_BOARDINFO)
	checkboard,		/* display board info */	//通过串口打印开发板名称
	/*************************************************************************
		checkboard函数在board\samsung\smdkc210\smdkc210.c文件中
		int checkboard(void)
		{
			printf("Board:	%s%s\n", CONFIG_DEVICE_STRING,CORE_NUM_STR);
			return (0);
		}
		#define CONFIG_DEVICE_STRING    "iTOP-4412"	//这个宏定义就是填充了开发板的名称
	*************************************************************************/
#endif
//如果宏定义了CONFIG_HARD_I2C或者CONFIG_SOFT_I2C就会启动i2c功能，这两宏的定义还是有条件（在smdkc210.h文件中）
#if defined(CONFIG_HARD_I2C) || defined(CONFIG_SOFT_I2C)
	//init_func_i2c,
	/*********************************************************
		就在该文件中
		static int init_func_i2c (void)
		{
			puts ("I2C:   ");
			i2c_init (CONFIG_SYS_I2C_SPEED, CONFIG_SYS_I2C_SLAVE);
			puts ("ready\n");
			return (0);
		}
	*********************************************************/
#endif
	dram_init,		/* configure available RAM banks */	//初始化gd->bd->bi_dram，开发板的SDRAM配置信息
	/****************************************************************************
		dram_init函数在board\samsung\smdkc210\smdkc210.c文件中
		
		dram_init都是在给gd-bd里面的关于DDR配置部分的全局变量赋值，让gd-bd数据记录下当前开发板的DDR的配置信息，
		以便于uboot使用内存。
		int dram_init(void)
		{
			DECLARE_GLOBAL_DATA_PTR;

			//if(((PRO_ID & 0x300) >> 8) == 2){
			if(((*((volatile unsigned long *)CHIP_ID_BASE) & 0x300) >> 8) == 2){
		#ifdef CONFIG_EVT1
				printf("POP type: ");
				if(dmc_density == 6){
					printf("POP_B\n");
					nr_dram_banks = 2;
					gd->bd->bi_dram[0].start = PHYS_SDRAM_1;
					gd->bd->bi_dram[0].size = PHYS_SDRAM_1_SIZE;
					gd->bd->bi_dram[1].start = PHYS_SDRAM_2;
					gd->bd->bi_dram[1].size = 0x4000000;//PHYS_SDRAM_2_SIZE;
					/*gd->bd->bi_dram[2].start = PHYS_SDRAM_3;
					gd->bd->bi_dram[2].size = PHYS_SDRAM_3_SIZE;
					gd->bd->bi_dram[3].start = PHYS_SDRAM_4;
					gd->bd->bi_dram[3].size = PHYS_SDRAM_4_SIZE;*
				}
				else if(dmc_density == 5){
					printf("POP_A\n");
					nr_dram_banks = 2;
					gd->bd->bi_dram[0].start = PHYS_SDRAM_1;
					gd->bd->bi_dram[0].size = PHYS_SDRAM_1_SIZE;
					gd->bd->bi_dram[1].start = PHYS_SDRAM_2;
					gd->bd->bi_dram[1].size = PHYS_SDRAM_2_SIZE;
				}
				else{//ly
							printf("POP for C220\n");

		#if defined(CONFIG_POP_2GDDR) || defined(CONFIG_POP_2GDDR_Ubuntu)
							nr_dram_banks = 8;
		#else
							nr_dram_banks = 4;
		#endif

							gd->bd->bi_dram[0].start = PHYS_SDRAM_1;
							gd->bd->bi_dram[0].size = PHYS_SDRAM_1_SIZE;
							gd->bd->bi_dram[1].start = PHYS_SDRAM_2;
							gd->bd->bi_dram[1].size = PHYS_SDRAM_2_SIZE;
							gd->bd->bi_dram[2].start = PHYS_SDRAM_3;
							gd->bd->bi_dram[2].size = PHYS_SDRAM_3_SIZE;
							gd->bd->bi_dram[3].start = PHYS_SDRAM_4;
							gd->bd->bi_dram[3].size = PHYS_SDRAM_4_SIZE;

		#if defined(CONFIG_POP_2GDDR) || defined(CONFIG_POP_2GDDR_Ubuntu)

							gd->bd->bi_dram[4].start = PHYS_SDRAM_5;
							gd->bd->bi_dram[4].size = PHYS_SDRAM_5_SIZE;
							gd->bd->bi_dram[5].start = PHYS_SDRAM_6;
							gd->bd->bi_dram[5].size = PHYS_SDRAM_6_SIZE;
							gd->bd->bi_dram[6].start = PHYS_SDRAM_7;
							gd->bd->bi_dram[6].size = PHYS_SDRAM_7_SIZE;
							gd->bd->bi_dram[7].start = PHYS_SDRAM_8;
							gd->bd->bi_dram[7].size = PHYS_SDRAM_8_SIZE;

		#endif
					
				}
		#else
				nr_dram_banks = 2;
				gd->bd->bi_dram[0].start = PHYS_SDRAM_1;
				gd->bd->bi_dram[0].size = PHYS_SDRAM_1_SIZE;
				gd->bd->bi_dram[1].start = PHYS_SDRAM_2;
				gd->bd->bi_dram[1].size = PHYS_SDRAM_2_SIZE;
		#endif
			}
			else{
				nr_dram_banks = 8;
				gd->bd->bi_dram[0].start = PHYS_SDRAM_1;
				gd->bd->bi_dram[0].size = PHYS_SDRAM_1_SIZE;
				gd->bd->bi_dram[1].start = PHYS_SDRAM_2;
				gd->bd->bi_dram[1].size = PHYS_SDRAM_2_SIZE;
				gd->bd->bi_dram[2].start = PHYS_SDRAM_3;
				gd->bd->bi_dram[2].size = PHYS_SDRAM_3_SIZE;
				gd->bd->bi_dram[3].start = PHYS_SDRAM_4;
				gd->bd->bi_dram[3].size = PHYS_SDRAM_4_SIZE;
				gd->bd->bi_dram[4].start = PHYS_SDRAM_5;
				gd->bd->bi_dram[4].size = PHYS_SDRAM_5_SIZE;
				gd->bd->bi_dram[5].start = PHYS_SDRAM_6;
				gd->bd->bi_dram[5].size = PHYS_SDRAM_6_SIZE;
				gd->bd->bi_dram[6].start = PHYS_SDRAM_7;
				gd->bd->bi_dram[6].size = PHYS_SDRAM_7_SIZE;
				gd->bd->bi_dram[7].start = PHYS_SDRAM_8;
				gd->bd->bi_dram[7].size = PHYS_SDRAM_8_SIZE;
			
			}

		/*
			gd->bd->bi_dram[0].start = PHYS_SDRAM_1;
			gd->bd->bi_dram[0].size = PHYS_SDRAM_1_SIZE;

		#if defined(CONFIG_SPARSEMEM)
			gd->bd->bi_dram[1].start = PHYS_SDRAM_2;
			gd->bd->bi_dram[1].size = PHYS_SDRAM_2_SIZE;
		#endif
		*/

		//for trustzone
		#ifdef CONFIG_TRUSTZONE
			gd->bd->bi_dram[nr_dram_banks - 1].size -= 0x100000;
		#endif

			return 0;
		}
	****************************************************************************/
#if defined(CONFIG_CMD_PCI) || defined (CONFIG_PCI)
	//arm_pci_init,
#endif
	display_dram_config,	//打印显示DDR相关的配置信息
	/***********************************************************
		display_dram_config函数就在该文件中
		重点关注的函数是print_size
		在uboot命令行模式下使用bdinfo命令可以查看DDR硬件相关的配置信息，他会把gd-bd下跟硬件配置相关的东西都打印出来
		static int display_dram_config (void)
		{
			int i;

		#ifdef DEBUG
			puts ("RAM Configuration:\n");

			for(i=0; i<CONFIG_NR_DRAM_BANKS; i++) {
				printf ("Bank #%d: %08lx ", i, gd->bd->bi_dram[i].start);
				print_size (gd->bd->bi_dram[i].size, "\n");
			}
		#else
			ulong size = 0;

			for (i=0; i<CONFIG_NR_DRAM_BANKS; i++) {
				size += gd->bd->bi_dram[i].size;
			}

		#if  defined(CONFIG_SCP_1GDDR) ||  defined(CONFIG_SCP_2GDDR) || defined(CONFIG_SCP_1GDDR_Ubuntu) || defined(CONFIG_SCP_2GDDR_Ubuntu)  //add by dg

			size += 0x100000;
		#endif

		#if defined(CONFIG_POP_2GDDR) || defined(CONFIG_POP_2GDDR_Ubuntu) 
			puts("DRAM: ");
			puts("2G\n");
		#else
			puts("DRAM:	");
			print_size(size, "\n");
		#endif
		#endif

			return (0);
		}
	***********************************************************/
	NULL,
};


/*

接下来start_armboot这个函数是uboot启动的第二阶段，接下来我们需要分析start_armboot这个函数。

*/


void start_armboot (void)
{
	init_fnc_t **init_fnc_ptr;//定义了一个二级函数指针
	char *s;
	int mmc_exist = 0;
#if defined(CONFIG_VFD) || defined(CONFIG_LCD)
	unsigned long addr;
#endif

	/* Pointer is writable since we allocated a register for it */
	gd = (gd_t*)(_armboot_start - CONFIG_SYS_MALLOC_LEN - sizeof(gd_t));
	/*
	**********************************************注释*******************************************************************************************
		gd_t是一个结构体变量，gd是一个gd_t*类型的结构体指针变量在本文件的第73行，
		用一个宏DECLARE_GLOBAL_DATA_PTR来定义。
		#define DECLARE_GLOBAL_DATA_PTR     register volatile gd_t * gd asm ("r8")
		asm ("r8")代表指定存放在r8寄存器
		为什么要将gd定义为register呢？gd这个全局变量是uboot中很重要的一个全局变量。
		gd里边包含了uboot所用到的几乎所有的全局变量，这个gd变量经常被访问。将这个变量放在
		register中可以提高访问的效率从而提高运行的效率。
		
		
		gd结构体分析
		typedef	struct	global_data {
		volatile bd_t		*bd;				//存放开发板相关信息的（这个是很重要的结构体需要重点关注）
		volatile unsigned long	flags;			//存放一些标志位相关的
		volatile unsigned long	baudrate;		//存放通信波特率相关信息
		volatile unsigned long	have_console;	/* serial_init() was called * 	//代表当前是否有控制台（能够标准的printf和scanf）
		volatile unsigned long	env_addr;	/* Address  of Environment struct *	//环境变量地址相关的
		volatile unsigned long	env_valid;	/* Checksum of Environment valid? *	//在内存中的环境变量是否能够使用
		volatile unsigned long	fb_base;	/* base address of frame buffer *	//缓存的基地址
		#ifdef CONFIG_VFD
			volatile unsigned char	vfd_type;	/* display type *
		#endif
		#ifdef CONFIG_FSL_ESDHC
			volatile unsigned long	sdhc_clk;
		#endif
		#if 0
			unsigned long	cpu_clk;	/* CPU clock in Hz!		*
			unsigned long	bus_clk;
			phys_size_t	ram_size;	/* RAM size *
			unsigned long	reset_status;	/* reset status register at boot *
		#endif
			volatile void		**jt;		/* jump table * //跳转表
		} gd_t;
		
		bd结构体分析
		typedef struct bd_info {

			intbi_baudrate;/* serial console baudrate * //硬件串口波特率

			unsigned longbi_ip_addr;/* IP Address * //开发板IP地址

			unsigned charbi_enetaddr[6];//开发板网卡地址 /* Ethernet adress *

			struct environment_s       *bi_env;//环境变量指针

			ulong        bi_arch_number;/* unique id for this board * //机器码

			ulong        bi_boot_params;/* where this board expects params * //uboot启动参数

			struct/* RAM configuration *

			{

				ulong start;

				ulong size;

			}bi_dram[CONFIG_NR_DRAM_BANKS];//内存插条信息

			#ifdef CONFIG_HAS_ETH1

				/* second onboard ethernet port *

				unsigned char   bi_enet1addr[6];//第二块网卡的地址

			#endif

		} bd_t;
		
	*****************************************注释********************************************************************************************	
	*/
	/* compiler optimization barrier needed for GCC >= 3.4 */
	__asm__ __volatile__("": : :"memory");
	
	/*清空gd结构体，和bd结构体*/
	memset ((void*)gd, 0, sizeof (gd_t));
	gd->bd = (bd_t*)((char*)gd - sizeof(bd_t));
	memset (gd->bd, 0, sizeof (bd_t));

//	gd->flags |= GD_FLG_RELOC;

	monitor_flash_len = _bss_start - _armboot_start;
	
	/*
		这个for循环是遍历init_sequence（是一个包含了很多函数的函数指针数组）数组，直到NULL为止
		在init_sequence函数指针数组中存放的都是跟board硬件初始化相关的一些函数，通过for循环的方式
		来依次执行这些初始化函数。
	*/
	for (init_fnc_ptr = init_sequence; *init_fnc_ptr; ++init_fnc_ptr) {
		/*
			当判断init_sequence数组中的函数有一个执行不成功时（执行成功就会返回0），
			调用hang函数（是一个死循环）来挂起uboot的程序，此时不能够恢复，只能重启。
		*/
		if ((*init_fnc_ptr)() != 0) {	
			hang ();//死循环，只能重启uboot
		}
	}
	
	/* armboot_start is defined in the board-specific linker script */
	/********************************************************************
		mem_malloc_init函数在当前文件中，用来初始化uboot的堆管理器
		uboot在DDR中自己维护了一段堆内存（896KB左右），肯定有一套代码来管理这个堆内存
		初始化完之后，我们就可以使用malloc和free函数来进行堆内存的分配和释放
	*********************************************************************/
	 /* 初始化堆空间 */
	mem_malloc_init (_armboot_start - CONFIG_SYS_MALLOC_LEN,
			CONFIG_SYS_MALLOC_LEN);

#ifndef CONFIG_SYS_NO_FLASH
	/* configure available FLASH banks */
	display_flash_config (flash_init ());	//这里打印的是norflash相关的信息
#endif /* CONFIG_SYS_NO_FLASH */

#ifdef CONFIG_VFD	//uboot中自带的LCD显示相关的内容，我们没有使用
#	ifndef PAGE_SIZE
#	  define PAGE_SIZE 4096
#	endif
	/*
	 * reserve memory for VFD display (always full pages)
	 */
	/* bss_end is defined in the board-specific linker script */
	addr = (_bss_end + (PAGE_SIZE - 1)) & ~(PAGE_SIZE - 1);
	vfd_setmem (addr);
	gd->fb_base = addr;
#endif /* CONFIG_VFD */

#ifdef CONFIG_LCD	//uboot中自带的LCD显示相关的内容，我们没有使用
	/* board init may have inited fb_base */
	if (!gd->fb_base) {
#		ifndef PAGE_SIZE
#		  define PAGE_SIZE 4096
#		endif
		/*
		 * reserve memory for LCD display (always full pages)
		 */
		/* bss_end is defined in the board-specific linker script */
		addr = (_bss_end + (PAGE_SIZE - 1)) & ~(PAGE_SIZE - 1);
		lcd_setmem (addr);
		gd->fb_base = addr;
	}
#endif /* CONFIG_LCD */


/*这些代码是开发板独有的配置信息，可能会有很多，用条件编译来进行选择*/
#if defined(CONFIG_CMD_NAND)
	puts ("NAND:	");
	nand_init();		/* go init the NAND */
#endif

#if defined(CONFIG_CMD_ONENAND)
	onenand_init();
#endif

#ifdef CONFIG_GENERIC_MMC
	puts ("MMC:   ");
	mmc_exist = mmc_initialize (gd->bd);
	/*******************************************************
		mmc_initialize函数在drivers\mmc\mmc.c文件中
		是MMC相关的一些基础的初始化，其实就是用来初始化SoC内部的SD/MMC控制器
		uboot中对硬件的操作（譬如网卡、SD卡.......）都是借用了linux内核中的驱动
		来实现的，uboot根目录下的drivers文件夹，这里面放的全都是从linux内核中移植
		过来的各种驱动源码。
		mmc_initialize函数是具体硬件架构无关的一个MMC初始化函数，所有的使用了这套架构
		的代码都调用了这个函数来完成MMC初始化。mmc_initialize函数中调用board_mmc_init
		或者cpu_mmc_init来完成具体的硬件的MMc控制器初始化工作。
	********************************************************/
	if (mmc_exist != 0)
	{
		puts ("0 MB\n");
	}

#endif


/*DATAFLASH一般是SPI相关的flash*/	
#ifdef CONFIG_HAS_DATAFLASH
	AT91F_DataflashInit();
	dataflash_print_info();
#endif

	/* initialize environment */
	/*********************************************************************************
		这里是环境变量从SD到DDR中的重定位（relocate）
		SD卡中有一些（8个）独立的扇区用来存放uboot的环境变量，但是我们在烧录/部署系统时，我们只
		烧录了uboot、kernel和rootfs分区并没有烧录uboot环境变量分区，此时我们去读取Uboot的环境变量
		分区会出错。那么环境变量是从哪里来的呢？其实在uboot的源码里面有一份硬编码的uboot环境变量，
		当第一次读取失败时，uboot会读取代码中的这一份uboot环境变量到SD卡中的uboot环境变量分区中去，
		然后再经过重定位到DDR中。之后在SD卡的uboot环境变量分区就有了一份uboot环境变量。
	*********************************************************************************/
	env_relocate ();

#ifdef CONFIG_VFD
	/* must do this after the framebuffer is allocated */
	drv_vfd_init();
#endif /* CONFIG_VFD */

#ifdef CONFIG_SERIAL_MULTI
	serial_initialize();
#endif

	/* IP Address */
	/**************************************************************
		开发板的IP地址是在gd->bd中维护的，来源于环境变量ipaddr。getenv函数用来回去字符串
		类型的IP地址，然后用string_to_ip函数将字符串的IP地址转换为点分十进制格式。
	**************************************************************/
	gd->bd->bi_ip_addr = getenv_IPaddr ("ipaddr");
	
	/***************************************************************
		是设备相关初始化（可能不叫这个名字例如：devices_init），这里的设备指的是开发板上的
		硬件设备。放在这里初试化的设备都是驱动设备，这个函数本来就是从驱动框架中衍生的。
		uboot中很多设备的驱动都是直接从linux内核移植过来的。这里只是集中到一个函数里去做
		这些设备的初始化工作。
	******************************************************************/
	stdio_init ();	/* get the devices list going. */

	jumptable_init (); //跳转表初始化（本身不重要）

#if defined(CONFIG_API)
	/* Initialize API */
	api_init ();
#endif
	/**************************************************************
		console_init_r是控制台的第二阶段初始化（console_init_f是第一阶段），第一阶段没做什么初始化
		第二阶段做了很多初始化
	****************************************************************/
	console_init_r ();	/* fully init console as a device */
	/**************************************************
		控制台相关的纯软件配置相关的初始化，实际上就是直接调用的串口操控的函数，
		实际上用不用没有太大的区别，但是在linux内核中控制台就不一样了，能够提供
		串口的缓冲机制。
	****************************************************/
	

#if defined(CONFIG_ARCH_MISC_INIT)
	/* miscellaneous arch dependent initialisations */
	arch_misc_init ();
#endif
#if defined(CONFIG_MISC_INIT_R)
	/* miscellaneous platform dependent initialisations */
	misc_init_r ();
#endif

	/* enable exceptions */
	enable_interrupts ();
	/*************************************************************
		中断初始化相关的程序，这里指的是CPSR中总中断标志位的使能。
		因为我们的uboot中没有定义使用中断的宏，因此enable_interrupts
		函数是一个空函数。
		
		编程技巧：用宏定义来进行条件编译的时候，有两种方式
			1，在调用函数的时候使用宏的条件编译，当定义了这个宏就调用这个函数
			   如果没有定义宏就不调用函数。
			2,在函数实现的时候使用宏定义进行条件编译，函数调用是都有的，但是根据
			  宏定义进行条件编译调用两个或者多个函数实现体，具体定义了哪个宏，就会
			  调用哪个函数实现体。
	******************************************************************/

	/* Perform network card initialisation if necessary */
#ifdef CONFIG_DRIVER_TI_EMAC
	/* XXX: this needs to be moved to board init */
extern void davinci_eth_set_mac_addr (const u_int8_t *addr);
	if (getenv ("ethaddr")) {
		uchar enetaddr[6];
		eth_getenv_enetaddr("ethaddr", enetaddr);
		davinci_eth_set_mac_addr(enetaddr);
	}
#endif

#if defined(CONFIG_DRIVER_SMC91111) || defined (CONFIG_DRIVER_LAN91C96)
	/* XXX: this needs to be moved to board init */
	if (getenv ("ethaddr")) {
		uchar enetaddr[6];
		eth_getenv_enetaddr("ethaddr", enetaddr);
		smc_set_mac_addr(enetaddr);
	}
#endif /* CONFIG_DRIVER_SMC91111 || CONFIG_DRIVER_LAN91C96 */

	/***********************************************************
		loadaddr、bootfile这两个环境变量都是内核启动有关的，、
		在启动内核时会参考这两个环境变量的值
	************************************************************/
	/* Initialize from environment */
	if ((s = getenv ("loadaddr")) != NULL) {
		load_addr = simple_strtoul (s, NULL, 16);
	}
#if defined(CONFIG_CMD_NET)
	if ((s = getenv ("bootfile")) != NULL) {
		copy_filename (BootFile, s, sizeof (BootFile));
	}
#endif

#ifdef BOARD_LATE_INIT
	board_late_init ();
	/*****************************************************
		这个函数是开发板晚期的初始化相关的初始化部分，就是剩下的一些必须放在最后初始化的部分。
		int board_late_init (void)
		{
			int ret = check_bootmode();
			if ((ret == BOOT_MMCSD || ret == BOOT_EMMC441 || ret == BOOT_EMMC43 )
						&& boot_mode == 0) {
				//printf("board_late_init\n");
				char boot_cmd[100];
		/* modify by cym 20131206 *
		#if 0
				sprintf(boot_cmd, "movi read kernel 40008000;movi read rootfs 40d00000 100000;bootm 40008000 40d00000");
		#else
		#ifdef SMDK4412_SUPPORT_UBUNTU
				sprintf(boot_cmd, "movi read kernel 40008000;bootm 40008000 40d00000");
		#else
				sprintf(boot_cmd, "movi read kernel 40008000;movi read rootfs 40df0000 100000;bootm 40008000 40df0000");
		#endif
		#endif
		/* end modify *
				setenv("bootcmd", boot_cmd);
				}

			return 0;
		}		
	*******************************************************/
#endif

#ifdef CONFIG_BITBANGMII
	bb_miiphy_init();
#endif
#if defined(CONFIG_CMD_NET)
#if defined(CONFIG_NET_MULTI)
	puts ("Net:   ");
#endif
	eth_initialize(gd->bd);
	/********************************************************、
		网卡相关的初始化。
	**********************************************************/
#if defined(CONFIG_RESET_PHY_R)
	debug ("Reset Ethernet PHY\n");
	reset_phy();
#endif
#endif

/*
	char tmp_cmd[100];	
	sprintf(tmp_cmd, "emmc open 0");
	run_command(tmp_cmd, 0);
*/				
	/* main_loop() can return to retry autoboot, if so just run it again. */
#ifdef CONFIG_RECOVERY //mj for factory reset
	recovery_preboot();
	/********************************************
		uboot启动进入命令行之前的一些初始化，如LCD和显示logo相关的初始化。
	********************************************/
#endif

	/*************************************************************************
		check menukey to update from sd
		这是uboot启动最后阶段设计的一个自动更新功能，就是我们将linux内核镜像放在sd卡中，
		然后再启动的时候按下指定的按键就会进入自动更新模式，这样uboot就会从sd卡中读取内核
		镜像，然后将内核烧录到iNand中去，下次启动时将sd卡拔掉就能直接从iNand中启动内核。
		这种方式比较适合量产的时候的内核部署。
	***************************************************************************/

	/***************************************************************************
		上边都是uboot对硬件什么的初始化工作
		接下来的main_loop函数就是在uboot启动过程到了bootdelay倒计时的时候，
		按下任意键进入uboot的命令行模式，在命令行下输入的命令都是由main_loop
		函数来进行接收解析处理的。
	***************************************************************************/
	
	for (;;) {
		main_loop ();
	}

	/* NOTREACHED - no way out of command loop except booting */
}

void hang (void)
{
	puts ("### ERROR ### Please RESET the board ###\n");
	for (;;);
}
