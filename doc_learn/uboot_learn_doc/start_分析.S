/*
 *  armboot - Startup Code for S5PC110/ARM-Cortex CPU-core
 *
 *  Copyright (c) 2009	Samsung Electronics
 *
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *
 * Base codes by scsuh (sc.suh)
 */


/***************************
*
*start.S文件的头文件包含分析，start.S文件在uboot\cpu\arm_cortexa9文件夹中
*
***************************/


//里面包含了include/configs/smdkc210.h（这个文件是uboot配置的主要文件）
#include <config.h>		//（include/config.h）
//里面宏定义了uboot的版本号，这个宏在我们的程序中会被调用，在启动uboot时会打印出Uboot的版本号
#include <version.h>	//（include/version.h）
//要想判断这个宏是否被定义了就要去include/configs/smdkc210.h文件下查看
#if defined(CONFIG_ENABLE_MMU)
#include <asm/proc/domain.h>
//asm目录不是uboot的原始目录，这个是一个链接文件指向到asm-arm文件夹，实际包含的
是include/asm-arm/proc-armv/domain.h文件。
//为什么start.S不直接包含include/asm-arm/proc-armv/domain.h这个文件,而是采用符号链接的方式
include/asm/proc/domain.h包含这个文件，这是为了uboot的可移植性考虑，当我们需要移植到别的硬件平台时，
只需要修改符号链接的指向就可以达到移植的目的。譬如说：让ams执行arm-mpis就可以是想往mpis平台移植uboot。
#endif
#if defined(CONFIG_S5PV310)
#include <s5pv310.h>
#endif
#if defined(CONFIG_S5PC210)
#include <s5pc210.h>
#endif

#ifndef CONFIG_ENABLE_MMU
#ifndef CFG_PHY_UBOOT_BASE
#define CFG_PHY_UBOOT_BASE	CFG_UBOOT_BASE
#endif
#endif

/*
 *************************************************************************
 *
 * Jump vector table as in table 3.1 in [1]
 *
 *************************************************************************
 */
 
 //uboot在start.S的开头位置放了16字节的填充位，这个占位的16字节是为了保证正式的image的头部确实有16个字节，
 但是这16个字节的内容是不正确的，还是需要后面去计算校验和然后重新填充的。（计算校验和是在sd_fusing\mk_bl1ah.c文件中计算）
	.word 0x2000
	.word 0x0
	.word 0x0
	.word 0x0

/*******************************
*
*异常向量表相关的分析
*
*******************************/	
（1）异常向量表是硬件决定的，软件只是参照硬件的设计来实现它。
（2）异常向量表中每种异常都应该被处理，否则真遇到了这种异常情况，程序就会跑飞。
	 但是我们的uboot中并没有非常细致的处理这些异常，只是做了一些简单的处理。这是
	 因为uboot不是那么容易出错，即使出错了，uboot运行的时间很短，我们只需要重启就可以了。
（3）复位异常处的代码是b  reset，因此CPU在复位之后真正执行的是reset部分的代码，应此我们应该详细分析reset部分代码。

	
.globl _start

//_start是整个uboot代码的入口位置，在链接脚本uboot.lbs中用ENTRY(_start)指定
_start: b	reset						/*复位异常,0x00*/
	ldr	pc, _undefined_instruction		/*未定义指令异常,0x04*/
	ldr	pc, _software_interrupt			/*软中断异常,0x08*/
	ldr	pc, _prefetch_abort				/*内存操作异常,0x0c*/
	ldr	pc, _data_abort					/*数据异常,0x10*/
	ldr	pc, _not_used					/*未适用,0x14*/
	ldr	pc, _irq						/*慢速中断异常,0x18*/
	ldr	pc, _fiq						/*快速中断异常,0x1c*/

_undefined_instruction:
	.word undefined_instruction
_software_interrupt:
	.word software_interrupt
_prefetch_abort:
	.word prefetch_abort
_data_abort:
	.word data_abort
_not_used:
	.word not_used
_irq:
	.word irq
_fiq:
	.word fiq
_pad:
	.word 0x12345678 /* now 16*4=64 */
.global _end_vect
_end_vect:

	.balignl 16,0xdeadbeef   
	//（1）这是一个伪指令，让当前地址对齐排布，如果当前地址不对齐的话，则地址自动往后走，直到对齐
		   并且往后走的那些内存要用0xdeadbeef来填充。
	//（2）0xdeadbeef 是一个十六进制数字，组成这个数字的十六进制全是abcdef之间的字母。组成了deadbeef（坏牛肉）
		   就是无用的意思。
	//（3）为什么要对齐访问？有时候是效率的要求，有时候是硬件的特殊要求。
/*
 *************************************************************************
 *
 * Startup Code (reset vector)
 *
 * do important init only if we don't start from memory!
 * setup Memory and board specific bits prior to relocation.
 * relocate armboot to ram
 * setup stack
 *
 *************************************************************************
 */

/***********************************
*
*TEXT_BASE链接地址分析
*
***********************************/

//（1）TEXT_BASE就是在分析Makefile时的那个配置阶段的链接地址（值就是那个时候指定的值）
//（2）源代码中和配置Makefile中很多变量是可以互相运送的，简单说来就是有些符号的值可以从Makefile中传递到源码中去使用
_TEXT_BASE:
	.word	TEXT_BASE

/*
 * Below variable is very important because we use MMU in U-Boot.
 * Without it, we cannot run code correctly before MMU is ON.
 * by scsuh.
 */
_TEXT_PHY_BASE:
	.word	CFG_PHY_UBOOT_BASE	//物理基地址

.globl _armboot_start
_armboot_start: 
	.word _start

/*
 * These are defined in the board-specific linker script.
 */
.globl _bss_start
_bss_start:
	.word __bss_start

.globl _bss_end
_bss_end:
	.word _end

    .macro  cache_invalidate_dcache_v7
    MRC     p15, 1, r0, c0, c0, 1      @ read Cache Level ID register (clidr)
    ANDS    r3, r0, #0x7000000         @ extract level of coherency from clidr
    MOV     r3, r3, lsr #23            @ left align level of coherency bit field
    BEQ     finished_inval             @ if loc is 0, then no need to clean

    MOV     r10, #0                    @ start clean at cache level 0 (in r10)
loop_1:
    ADD     r2, r10, r10, lsr #1       @ work out 3x current cache level
    MOV     r1, r0, lsr r2             @ extract cache type bits from clidr
    AND     r1, r1, #7                 @ mask of the bits for current cache only
    CMP     r1, #2                     @ see what cache we have at this level
    BLT     skip_inval                 @ skip if no cache, or just i-cache
    MCR     p15, 2, r10, c0, c0, 0     @ select current cache level in cssr
    MOV     r1, #0
    MCR     p15, 0, r1, c7, c5, 4      @ prefetchflush to synch the new cssr&csidr
    MRC     p15, 1, r1, c0, c0, 0      @ read the new csidr
    AND     r2, r1, #7                 @ extract the length of the cache lines
    ADD     r2, r2, #4                 @ add 4 (line length offset)
    LDR     r6, =0x3ff
    ANDS    r6, r6, r1, lsr #3         @ find maximum number on the way size
    CLZ     r5,r6                       @ DCI 0xE16F5F16 , find bit position of way size increment
    LDR     r7, =0x7fff
    ANDS    r7, r7, r1, lsr #13        @ extract max number of the index size
loop_2:
    MOV     r8, r6                     @ create working copy of max way size
	loop_3:
    ORR     r11, r10, r8, lsl r5       @ factor way and cache number into r11
    ORR     r11, r11, r7, lsl r2       @ factor index number into r11
    MCR     p15, 0, r11, c7, c6, 2     @ invalidate by set/way
    SUBS    r8, r8, #1                 @ decrement the way
    BGE     loop_3

    SUBS    r7, r7, #1                 @ decrement the index
    BGE     loop_2
skip_inval:
    ADD     r10, r10, #2               @ increment cache number
    CMP     r3, r10
    BGT     loop_1
finished_inval:
    .endm

#if defined(CONFIG_USE_IRQ)
/* IRQ stack memory (calculated at run-time) */
.globl IRQ_STACK_START
IRQ_STACK_START:
	.word	0x0badc0de

/* IRQ stack memory (calculated at run-time) */
.globl FIQ_STACK_START
FIQ_STACK_START:
	.word 0x0badc0de
#endif

/*
 * the actual reset code
 */







/**********************************************************************
*
*接下类分析的reset代码段是复位之后执行的具体代码，需要详细分析
*
**********************************************************************/

reset:
#if 0
	/*
	 * set the cpu to SVC32 mode and IRQ & FIQ disable
	 */
	mrs	r0, cpsr
	bic	r0, r0, #0x3f
	orr	r0, r0, #0xd3
	msr	cpsr, r0
#else//*****ly
	mrs	r0, cpsr
	bic	r0, r0, #0x1f
	orr	r0, r0, #0xd3
	msr	cpsr,r0	
	//（1）以上几句是将CPU的状态切换为ARM状态的SVC模式，禁止FIQ和IRQ。（其实在复位时默认就会进入SVC模式）
#endif
//	cache_invalidate_dcache_v7			@ call cache macro
#if 1 //*****ly

//（2）cache相关的初始胡
cache_init:
	mrc	p15, 0, r0, c0, c0, 0	@ read main ID register
	and	r1, r0, #0x00f00000	@ variant
	and	r2, r0, #0x0000000f	@ revision
	orr	r2, r2, r1, lsr #20-4	@ combine variant and revision
	cmp	r2, #0x30
	mrceq	p15, 0, r0, c1, c0, 1	@ read ACTLR
	orreq	r0, r0, #0x6		@ Enable DP1(2), DP2(1)
	mcreq	p15, 0, r0, c1, c0, 1	@ write ACTLR
	/*
	 * Invalidate L1 I/D
	 （3）关闭BL1
	 */
	mov	r0, #0			@ set up for MCR
	mcr	p15, 0, r0, c8, c7, 0	@ invalidate TLBs
	mcr	p15, 0, r0, c7, c5, 0	@ invalidate icache

	/*
	 * disable MMU stuff and caches
	 *（4）关闭MMU和cache
	 */
	mrc	p15, 0, r0, c1, c0, 0
	bic	r0, r0, #0x00002000	@ clear bits 13 (--V-)
	bic	r0, r0, #0x00000007	@ clear bits 2:0 (-CAM)
	orr	r0, r0, #0x00001000	@ set bit 12 (---I) Icache
	orr	r0, r0, #0x00000002	@ set bit 1  (--A-) Align
	orr	r0, r0, #0x00000800	@ set bit 11 (Z---) BTB
	mcr	p15, 0, r0, c1, c0, 0
#endif
	

        /* 
		*
		*LED Test Code 
		*LED提示相关的配置
		*
		*/
		#if 0
        ldr r0, =0x11000080
        ldr r1, [r0]
        bic r1, r1, #0xf00
        orr r1, r1, #0x100
        str r1, [r0]
        #endif

	（5）跟拨码开关相关的一个寄存器地址，用来选择启动的方式（r2寄存器里的值就是设置的启动方式）
	/* Read booting information */
        ldr	r0, =POWER_BASE
        ldr	r1, [r0,#OMR_OFFSET]
        bic	r2, r1, #0xffffffc1

	/* NAND BOOT（设置从NAND启动） */
@	cmp	r2, #0x0		@ 512B 4-cycle
@	moveq	r3, #BOOT_NAND

@	cmp	r2, #0x2		@ 2KB 5-cycle
@	moveq	r3, #BOOT_NAND

@	cmp	r2, #0x4		@ 4KB 5-cycle	8-bit ECC
@	moveq	r3, #BOOT_NAND

	cmp     r2, #0xA
        moveq   r3, #BOOT_ONENAND

@	cmp	r2, #0x6		@ 4KB 5-cycle	16-bit ECC
@	moveq	r3, #BOOT_NAND

	/* SD/MMC BOOT （设置从SD卡/EMMC启动）*/
	cmp     r2, #0x4
        moveq   r3, #BOOT_MMCSD	

	/* eMMC4.3 BOOT */
	cmp		r2, #0x6
	moveq	r3, #BOOT_EMMC43

	/* eMMC441 BOOT */
	cmp		r2, #0x28
	moveq	r3, #BOOT_EMMC441
	
        /* NOR BOOT（设置从NOR 启动） */
@	cmp     r2, #0x14
@	moveq   r3, #BOOT_NOR	

	/* For second device booting */
	/* OneNAND BOOTONG failed */
@	cmp     r2, #0x8
@	moveq   r3, #BOOT_SEC_DEV

	/* Uart BOOTONG failed */
@	cmp     r2, #(0x1<<4)
@	moveq   r3, #BOOT_SEC_DEV
	
	ldr	r0, =INF_REG_BASE
	str	r3, [r0, #INF_REG3_OFFSET]     

	/*
	 * Go setup Memory and board specific bits prior to relocation.
	 */
	 
	//在这里应该也要有一次设置栈的操作，用来调用lowlevel_init函数使用 
	 
	//（6）跳转到lowlevel_init(低层次初始化函数)	.
	bl	lowlevel_init	/* go setup pll,mux,memory */


	//（7）（PS_HOLD_CONTROL）供电锁存
	ldr	r0, =0x1002330C  /* PS_HOLD_CONTROL register */
	ldr	r1, =0x00005300	 /* PS_HOLD output high	*/
	str	r1, [r0]


	/* get ready to call C functions */
	/*（8）设置DDR中的栈，此时在lowlevel_init中已近初始化了DDR，已经有大把的空间可以使用，没有必要再放在SRAM中，
		   还有SRAM的空间有限，当大量使用栈的时候可能会溢出，使用的时候需要小心栈溢出*/
	ldr	sp, _TEXT_PHY_BASE	/* setup temp stack pointer */
	sub	sp, sp, #12
	mov	fp, #0			/* no previous frame, so fp=0 */


	/* I-Cache & D-Cache Enabled    */  
//	mrc p15, 0, r0, c1, c0, 0
//	orr r0, r0, #(0x1 << 2)
//	orr r0, r0, #(0x1 << 12)
//	mcr p15, 0, r0, c1, c0, 0

	/* when we already run in ram, we don't need to relocate U-Boot.
	 * and actually, memory controller must be configured before U-Boot
	 * is running in ram.
	 */
	 /*（9）再次用相同的代码判断运行地址是在SRAM中还是在DDR中，来决定是否需要重定位
			
			冷启动时当前情况是uboot的前一部分（16Kb或者8Kb）开机自动从SD卡中加载到SRAM中正在运行，
			uboot的第二部分（其实第二部分是整个uboot）还躺在SD卡的某个扇区开头的N个扇区中。此时
			uboot的第一阶段已经结束了（第一阶段该做的事情基本做完了），结束之前要吧第二部分加载到DDR
			中链接地址（TEXT_BASE），这个加载过程就叫做重定位。
			*/
	ldr	r0, =0xff000fff
	bic	r1, pc, r0		/* r0 <- current base addr of code */
	ldr	r2, _TEXT_BASE		/* r1 <- original base addr in ram */
	bic	r2, r2, r0		/* r0 <- current base addr of code */
	cmp     r1, r2                  /* compare r0, r1                  */
	/*
	 * bl 	movi_bl2_copy
	 * 这里是重定位已经结束
	*/
	beq     after_copy		/* r0 == r1 then skip flash copy   */
	/*跳转到after_copy去执行*/


	ldr	r0, =0x11000104  /* GPL2(0)  */
	ldr	r1, =0x00000001	 /* GPL2(0 output high	*/
	str	r1, [r0]
	
	ldr	r0, =0x11000100  /* GPL2(0)  */
	ldr	r1, =0x00000001	 /* GPL2(0 output high	*/
	str	r1, [r0]

	
	/* wait ?us */
	mov	r1, #0x10000
9:	subs	r1, r1, #1
	bne	9b

//	ldr	r0, =0x11000C44 /* GPX2(0)  */
//	ldr	r1, [r0]
//	bic	r2, r1, #0xfe
//	ldr	r1, =0x1



//	ldr	r0, =INF_REG_BASE
//	ldr	r3, [r0, #INF_REG3_OFFSET]  
	
//	cmp	r1, r2
//	moveq   r3, #BOOT_MMCSD	
//	ldr	r0, =INF_REG_BASE
//	str	r3, [r0, #INF_REG3_OFFSET]
//	cmp	r1, r2
//	beq     mmcsd_boot



	ldr	r0, =INF_REG_BASE
	ldr	r1, [r0, #INF_REG3_OFFSET]
	cmp	r1, #BOOT_NAND		/* 0x0 => boot device is nand */
	beq		nand_boot
	
	cmp	r1, #BOOT_ONENAND	/* 0x1 => boot device is onenand */
	beq		onenand_boot
	
	cmp     r1, #BOOT_EMMC441
	beq     emmc441_boot
	
	cmp     r1, #BOOT_EMMC43
	beq     emmc_boot
	
	cmp     r1, #BOOT_MMCSD
	beq     mmcsd_boot
	
	cmp     r1, #BOOT_NOR
	beq     nor_boot
	
	cmp     r1, #BOOT_SEC_DEV
	beq     mmcsd_boot

nand_boot:
	mov	r0, #0x1000
	bl	copy_from_nand
	b	after_copy

onenand_boot:
	bl	onenand_bl2_copy  /*goto 0x1010*/
	b	after_copy
//ly
second_mmcsd_boot:
	ldr   r3, =BOOT_MMCSD	
	ldr	r0, =INF_REG_BASE
	str	r3, [r0, #INF_REG3_OFFSET]
	
mmcsd_boot:
#ifdef CONFIG_CLK_1000_400_200
	ldr	r0, =CMU_BASE
	ldr	r2, =CLK_DIV_FSYS2_OFFSET
	ldr	r1, [r0, r2]
	orr r1, r1, #0xf
	str r1, [r0, r2]
#endif
	bl      movi_uboot_copy
	b       after_copy

emmc_boot:
#if defined(CONFIG_CLK_1000_400_200) || defined(CONFIG_CLK_1000_200_200) || defined(CONFIG_CLK_800_400_200)
	ldr	r0, =CMU_BASE
	ldr	r2, =CLK_DIV_FSYS1_OFFSET
	ldr	r1, [r0, r2]
	orr r1, r1, #0x3
	str r1, [r0, r2]
#endif
	bl		emmc_uboot_copy
	b	after_copy

emmc441_boot:
#if defined(CONFIG_CLK_1000_400_200) || defined(CONFIG_CLK_1000_200_200) || defined(CONFIG_CLK_800_400_200)
	ldr	r0, =CMU_BASE
	ldr	r2, =CLK_DIV_FSYS3_OFFSET
	ldr	r1, [r0, r2]
	orr r1, r1, #0x3
	str r1, [r0, r2]
#endif
	bl		emmc441_uboot_copy
//ly 20110824
	ldr   r0, =0x43e00000
	ldr   r1, [r0]
	ldr   r2, =0x2000
	cmp r1, r2
	bne  second_mmcsd_boot
	b	after_copy


nor_boot:
@	bl	read_hword
	b	after_copy

/*

*********************************************
*（10）这里是虚拟地址映射（MMU）相关的代码after_copy
	1，物理地址就是物理设备设计生产的时候赋予的地址。像裸机使用的寄存器地址就是CPU设计时指定的，
	   这个就是物理地址，物理地址是硬件编码的，是设计生产时确定好的，一旦确定了就不能改变。
	2，一个事实就是：寄存器的物理地址是没办法通过编程进行修改的，是多少就是多少，只能同查询数据
	   手册获得并进行操作。坏处就是不够灵活。一个解决方案就是使用虚拟地址。
	3，虚拟地址意思就是在我们软件操作和硬件被操作之间增加一个层次，叫做虚拟地址映射层。有了虚拟地址
	   映射后，软件操作只需要操纵虚拟地址，硬件操作还是原来的物理地址，映射层建立了一个虚拟地址到物理
	   地址的一个映射表。当我们软件运行的时候，软件中使用的虚拟地址就在映射表中查找到对应的物理地址，然后
	   再发送给指定的硬件去执行（虚拟地址到物理地址的映射是不能够用软件来实现的（因为软件中用的就是虚拟地址））。
	4，MMU就是内存管理单元。MMU实际就是SOC上的一个硬件单元，他的主要功能就是实现虚拟地址到物理地址的映射。
	   MMU单元在CP15协处理器中进行控制，也就是所要控制MMU进行虚拟地址的映射，方法就是对CP15协处理器进行编程。
	5，地址映射的额外收益
		（1）访问控制：就是在管理上对内存进行分块，然后没块进行独立的虚拟地址映射，然后再每一块的映射关系中同时还
			 实现了访问控制（对该块可读、可写、只读、只写、不可访问等控制）
		（2）cache ：是快速缓存，比CPU慢比DDR快。CPU嫌弃DDR太慢了，于是将DDR中常用的内容事先读取到cache中，CPU每次找内容
			 的时候优先在cache中寻找，cache中没有的时候才去DDR中去寻找。
		参考博客：http://blog.chinaunix.net/xmlrpc.php?r=blog/article&uid=22891521&id=2109284
*********************************************

*/

after_copy:

/*判断在X210_sd.h中是否定义了CONFIG_ENABLE_MMU这个宏，如果定义了就代表使用虚拟地址映射，否则不使用虚拟地址映射*/
#if defined(CONFIG_ENABLE_MMU)
enable_mmu:
	/* enable domain access （启用域访问（C3寄存器））*/
	ldr	r5, =0x0000ffff
	mcr	p15, 0, r5, c3, c0, 0		@load domain access register

	/* Set the TTB register （设置TTB（转换表基地址）寄存器C2）*/
	ldr	r0, _mmu_table_base		//通过这个_mmu_table_base-->mmu_table搜索可以追到lowlevel_init.S中去。
	ldr	r1, =CFG_PHY_UBOOT_BASE
	ldr	r2, =0xfff00000
	bic	r0, r0, r2
	orr	r1, r0, r1
	mcr	p15, 0, r1, c2, c0, 0

	/* Enable the MMU （使能MMU）
		cp15的c1寄存器的bit0控制着MMU的开关。只要置1就会开启MMU
	*/
mmu_on:
	mrc	p15, 0, r0, c1, c0, 0
	orr	r0, r0, #1
	mcr	p15, 0, r0, c1, c0, 0
	nop
	nop
	nop
	nop
#endif


#ifdef CONFIG_EVT1
	/* store DMC density information in u-boot C level variable */
	ldr	r0, = CFG_UBOOT_BASE
	sub	r0, r0, #4
	ldr	r1, [r0]
	ldr	r0, _dmc_density
	str	r1, [r0]
#endif



skip_hw_init:
	/* Set up the stack	（第三次设置栈）					    */
stack_setup:
#if defined(CONFIG_MEMORY_UPPER_CODE)
	/*（11）第三次设置栈，还是在DDR中，之前虽然已经在DDR中设置过一次了，
			本次设置是为了将栈的位置放在合适（安全，紧凑（紧贴着uboot）而不浪费内存）的位置*/
	ldr	sp, =(CFG_UBOOT_BASE + CFG_UBOOT_SIZE - 0x1000)
#else
	ldr	r0, _TEXT_BASE		/* upper 128 KiB: relocated uboot   */
	sub	r0, r0, #CONFIG_SYS_MALLOC_LEN	/* malloc area                      */
	sub	r0, r0, #CFG_GBL_DATA_SIZE /* bdinfo                        */
#if defined(CONFIG_USE_IRQ)
	sub	r0, r0, #(CONFIG_STACKSIZE_IRQ+CONFIG_STACKSIZE_FIQ)
#endif
	sub	sp, r0, #12		/* leave 3 words for abort-stack    */

#endif


/*（12）清空bss段_bss_start和_bss_end是从链接脚本中得来的*/
clear_bss:
	ldr	r0, _bss_start		/* find start of bss segment        */
	ldr	r1, _bss_end		/* stop here                        */
	mov 	r2, #0x00000000		/* clear                            */

clbss_l:
	str	r2, [r0]		/* clear loop...                    */
	add	r0, r0, #4
	cmp	r0, r1
	ble	clbss_l
	
	/*	
	（13）这是uboot第一阶段的最后一条指令，执行完之后uboot第一阶段就结束了，
		也是uboot第一阶段和第二阶段的分界线，是一个远跳转。
		注意：这个远跳转是直接从SRAM中的第一阶段，直接跳转到了DDR中的第二阶段去执行。（DDR中的第一阶段不会执行）
	*/
	ldr	pc, _start_armboot

_start_armboot:
	.word start_armboot    /*start_armboot在lib_arm/board.c中，是uboot的第二阶段，用C语言实现*/

#ifdef CONFIG_EVT1
_dmc_density:
	.word dmc_density
#endif

#if defined(CONFIG_ENABLE_MMU)
_mmu_table_base:
	.word mmu_table
#endif


/*
 * copy U-Boot to SDRAM and jump to ram (from NAND or OneNAND)
 * r0: size to be compared
 * Load 1'st 2blocks to RAM because U-boot's size is larger than 1block(128k) size
 */
	.globl copy_from_nand
copy_from_nand:
	push	{lr}		/* save return address */

	mov	r9, r0
	
	mov	r9, #0x100		/* Compare about 8KB */
	bl	copy_uboot_to_ram

	tst 	r0, #0x0
	bne	copy_failed

	ldr	r0, =0xd0020000

	ldr	r1, _TEXT_PHY_BASE	/* 0x23e00000 */
1:	ldr	r3, [r0], #4
	ldr	r4, [r1], #4
	teq	r3, r4
	bne	compare_failed	/* not matched */
	subs	r9, r9, #4
	bne	1b

	pop	{pc}		/* all is OK */

copy_failed:
	nop			/* copy from nand failed */
	b	copy_failed

compare_failed:
	nop			/* compare failed */
	b	compare_failed

/*
 * we assume that cache operation is done before. (eg. cleanup_before_linux())
 * actually, we don't need to do anything about cache if not use d-cache in U-Boot
 * So, in this function we clean only MMU. by scsuh
 *
 * void	theLastJump(void *kernel, int arch_num, uint boot_params);
 */
#if defined(CONFIG_ENABLE_MMU)
	.globl theLastJump
theLastJump:
	mov	r9, r0
	ldr	r3, =0xfff00000
	ldr	r4, _TEXT_PHY_BASE
	adr	r5, phy_last_jump
	bic	r5, r5, r3
	orr	r5, r5, r4
	mov	pc, r5
phy_last_jump:
	/*
	 * disable MMU stuff
	 */
	mrc	p15, 0, r0, c1, c0, 0
	bic	r0, r0, #0x00002300	/* clear bits 13, 9:8 (--V- --RS) */
	bic	r0, r0, #0x00000087	/* clear bits 7, 2:0 (B--- -CAM) */
	orr	r0, r0, #0x00000002	/* set bit 2 (A) Align */
	orr	r0, r0, #0x00001000	/* set bit 12 (I) I-Cache */
	mcr	p15, 0, r0, c1, c0, 0

	mcr	p15, 0, r0, c8, c7, 0	/* flush v4 TLB */

	mov	r0, #0
	mov	pc, r9
#endif
/*
 *************************************************************************
 *
 * Interrupt handling
 *
 *************************************************************************
 */
@
@ IRQ stack frame.
@
#define S_FRAME_SIZE	72

#define S_OLD_R0	68
#define S_PSR		64
#define S_PC		60
#define S_LR		56
#define S_SP		52

#define S_IP		48
#define S_FP		44
#define S_R10		40
#define S_R9		36
#define S_R8		32
#define S_R7		28
#define S_R6		24
#define S_R5		20
#define S_R4		16
#define S_R3		12
#define S_R2		8
#define S_R1		4
#define S_R0		0

#define MODE_SVC 0x13
#define I_BIT	 0x80

/*
 * use bad_save_user_regs for abort/prefetch/undef/swi ...
 * use irq_save_user_regs / irq_restore_user_regs for IRQ/FIQ handling
 */

	.macro	bad_save_user_regs
	sub	sp, sp, #S_FRAME_SIZE		@ carve out a frame on current user stack
	stmia	sp, {r0 - r12}			@ Save user registers (now in svc mode) r0-r12

	ldr	r2, _armboot_start
	sub	r2, r2, #(CONFIG_SYS_MALLOC_LEN)
	sub	r2, r2, #(CFG_GBL_DATA_SIZE+8)	@ set base 2 words into abort stack
	ldmia	r2, {r2 - r3}			@ get values for "aborted" pc and cpsr (into parm regs)
	add	r0, sp, #S_FRAME_SIZE		@ grab pointer to old stack

	add	r5, sp, #S_SP
	mov	r1, lr
	stmia	r5, {r0 - r3}			@ save sp_SVC, lr_SVC, pc, cpsr
	mov	r0, sp				@ save current stack into r0 (param register)
	.endm

	.macro	irq_save_user_regs
	sub	sp, sp, #S_FRAME_SIZE
	stmia	sp, {r0 - r12}			@ Calling r0-r12
	add	r8, sp, #S_PC			@ !!!! R8 NEEDS to be saved !!!! a reserved stack spot would be good.
	stmdb	r8, {sp, lr}^			@ Calling SP, LR
	str	lr, [r8, #0]			@ Save calling PC
	mrs	r6, spsr
	str	r6, [r8, #4]			@ Save CPSR
	str	r0, [r8, #8]			@ Save OLD_R0
	mov	r0, sp
	.endm

	.macro	irq_restore_user_regs
	ldmia	sp, {r0 - lr}^			@ Calling r0 - lr
	mov	r0, r0
	ldr	lr, [sp, #S_PC]			@ Get PC
	add	sp, sp, #S_FRAME_SIZE
	subs	pc, lr, #4			@ return & move spsr_svc into cpsr
	.endm

	.macro get_bad_stack
	ldr	r13, _armboot_start		@ setup our mode stack (enter in banked mode)
	sub	r13, r13, #(CONFIG_SYS_MALLOC_LEN)	@ move past malloc pool
	sub	r13, r13, #(CFG_GBL_DATA_SIZE+8) @ move to reserved a couple spots for abort stack

	str	lr, [r13]			@ save caller lr in position 0 of saved stack
	mrs	lr, spsr			@ get the spsr
	str	lr, [r13, #4]			@ save spsr in position 1 of saved stack

	mov	r13, #MODE_SVC			@ prepare SVC-Mode
	@ msr	spsr_c, r13
	msr	spsr, r13			@ switch modes, make sure moves will execute
	mov	lr, pc				@ capture return pc
	movs	pc, lr				@ jump to next instruction & switch modes.
	.endm

	.macro get_bad_stack_swi
	sub	r13, r13, #4			@ space on current stack for scratch reg.
	str	r0, [r13]			@ save R0's value.
	ldr	r0, _armboot_start		@ get data regions start
	sub	r0, r0, #(CONFIG_SYS_MALLOC_LEN)	@ move past malloc pool
	sub	r0, r0, #(CFG_GBL_DATA_SIZE+8)	@ move past gbl and a couple spots for abort stack
	str	lr, [r0]			@ save caller lr in position 0 of saved stack
	mrs	r0, spsr			@ get the spsr
	str	lr, [r0, #4]			@ save spsr in position 1 of saved stack
	ldr	r0, [r13]			@ restore r0
	add	r13, r13, #4			@ pop stack entry
	.endm

	.macro get_irq_stack			@ setup IRQ stack
	ldr	sp, IRQ_STACK_START
	.endm

	.macro get_fiq_stack			@ setup FIQ stack
	ldr	sp, FIQ_STACK_START
	.endm

/*
 * exception handlers
 */
	.align	5
undefined_instruction:
	get_bad_stack
	bad_save_user_regs
	bl	do_undefined_instruction

	.align	5
software_interrupt:
	get_bad_stack_swi
	bad_save_user_regs
	bl	do_software_interrupt

	.align	5
prefetch_abort:
	get_bad_stack
	bad_save_user_regs
	bl	do_prefetch_abort

	.align	5
data_abort:
	get_bad_stack
	bad_save_user_regs
	bl	do_data_abort

	.align	5
not_used:
	get_bad_stack
	bad_save_user_regs
	bl	do_not_used

#if defined(CONFIG_USE_IRQ)

	.align	5
irq:
	get_irq_stack
	irq_save_user_regs
	bl	do_irq
	irq_restore_user_regs

	.align	5
fiq:
	get_fiq_stack
	/* someone ought to write a more effiction fiq_save_user_regs */
	irq_save_user_regs
	bl	do_fiq
	irq_restore_user_regs

#else

	.align	5
irq:
	get_bad_stack
	bad_save_user_regs
	bl	do_irq

	.align	5
fiq:
	get_bad_stack
	bad_save_user_regs
	bl	do_fiq

#endif
	.align 5
.global arm_cache_flush
arm_cache_flush:
       mcr     p15, 0, r1, c7, c5, 0           @ invalidate I cache
       mov     pc, lr                          @ back to caller

/*
 *     v7_flush_dcache_all()
 *
 *     Flush the whole D-cache.
 *
 *     Corrupted registers: r0-r5, r7, r9-r11
 *
 *     - mm    - mm_struct describing address space
 */
       .align 5
.global v7_flush_dcache_all
v7_flush_dcache_all:

	ldr	r0, =0xffffffff
	mrc	p15, 1, r0, c0, c0, 1 		@ Read CLIDR
	ands	r3, r0, #0x7000000
	mov	r3, r3, LSR #23       		@ Cache level value (naturally aligned)
	beq 	Finished
	mov	r10, #0
Loop1:         
	add	r2, r10, r10, LSR #1  		@ Work out 3xcachelevel
	mov	r1, r0, LSR r2        		@ bottom 3 bits are the Ctype for this level
	and	r1, r1, #7            		@ get those 3 bits alone
	cmp	r1, #2
	blt	Skip                   		@ no cache or only instruction cache at this level
	mcr	p15, 2, r10, c0, c0, 0 		@ write the Cache Size selection register
	mov	r1, #0
	mcr	p15, 0, r1, c7, c5, 4 		@ PrefetchFlush to sync the change to the CacheSizeID reg
	mrc	p15, 1, r1, c0, c0, 0 		@ reads current Cache Size ID register
	and	r2, r1, #0x7           		@ extract the line length field
	add	r2, r2, #4            		@ add 4 for the line length offset (log2 16 bytes)
	ldr	r4, =0x3FF
	ands	r4, r4, r1, LSR #3   		@ R4 is the max number on the way size (right aligned)
	clz	r5, r4                		@ R5 is the bit position of the way size increment
	ldr	r7, =0x00007FFF
	ands	r7, r7, r1, LSR #13  		@ R7 is the max number of the index size (right aligned)
Loop2:         
	mov	r9, r4                      	@ R9 working copy of the max way size (right aligned)
Loop3:         
	orr	r11, r10, r9, LSL r5        	@ factor in the way number and cache number into R11
	orr	r11, r11, r7, LSL r2        	@ factor in the index number
	mcr	p15, 0, r11, c7, c6, 2 		@ invalidate by set/way
	subs	r9, r9, #1                 	@ decrement the way number
	bge	Loop3
	subs	r7, r7, #1                 	@ decrement the index
	bge	Loop2
Skip:          
	add	r10, r10, #2                	@ increment the cache number
	cmp	r3, r10
	bgt	Loop1
Finished:
	mov	pc, lr
	
       .align  5
.global disable_l2cache
disable_l2cache:
	mrc     p15, 0, r0, c1, c0, 1
	bic     r0, r0, #(1<<1)
	mcr     p15, 0, r0, c1, c0, 1
	mov	pc, lr


       .align  5
.global enable_l2cache
enable_l2cache:
	mrc     p15, 0, r0, c1, c0, 1
	orr     r0, r0, #(1<<1)
	mcr     p15, 0, r0, c1, c0, 1
	mov     pc, lr

       .align  5
.global set_l2cache_auxctrl
set_l2cache_auxctrl:
	mov	r0, #0x0
	mcr     p15, 1, r0, c9, c0, 2
	mov     pc, lr

       .align  5
.global set_l2cache_auxctrl_cycle
set_l2cache_auxctrl_cycle:
	mrc 	p15, 1, r0, c9, c0, 2
	bic 	r0, r0, #(0x1<<29)
	bic 	r0, r0, #(0x1<<21)
	bic 	r0, r0, #(0x7<<6)
	bic 	r0, r0, #(0x7<<0)
	mcr 	p15, 1, r0, c9, c0, 2
	mov     pc,lr

	.align 5
CoInvalidateDCacheIndex:
	;/* r0 = index */
	mcr     p15, 0, r0, c7, c6, 2
	mov     pc,lr


#if defined(CONFIG_INTEGRATOR) && defined(CONFIG_ARCH_CINTEGRATOR)
/* Use the IntegratorCP function from board/integratorcp/platform.S */
#elif defined(CONFIG_S5PC11X) || defined(CONFIG_S5PC210)|| defined(CONFIG_S5PV310)
/* For future usage of S3C64XX*/
#else
	.align	5
.globl reset_cpu
reset_cpu:
	ldr	r1, rstctl	/* get addr for global reset reg */
	mov	r3, #0x2	/* full reset pll+mpu */
	str	r3, [r1]	/* force reset */
	mov	r0, r0
_loop_forever:
	b	_loop_forever
rstctl:
	.word	PM_RSTCTRL_WKUP

#endif
