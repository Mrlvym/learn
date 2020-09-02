/*
 * (C) Copyright 2002
 * Gary Jennejohn, DENX Software Engineering, <garyj@denx.de>
 *
 * (C) Copyright 2008
 * Guennadi Liakhovetki, DENX Software Engineering, <lg@denx.de>
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


//输出可执行文件elf格式，32位ARM指令，小端模式
OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")

//指定输入平台为ARM
OUTPUT_ARCH(arm)

//指定输出可执行文件的其实代码为_start
ENTRY(_start)
SECTIONS
{
	. = 0x00000000; //定位当前地址为0地址（这里是链接地址）

	. = ALIGN(4);	//代码4字节对齐
	.text      :	//指定代码段
	{
	  //代码的第一个代码段（这些是uboot的前16kb的代码段）
	  cpu/arm1176/start.o	(.text)
	  cpu/arm1176/s3c64xx/cpu_init.o	(.text)
	  //其他代码段
	  *(.text)
	}

	. = ALIGN(4);	//代码以4字节对齐
	.rodata : { *(SORT_BY_ALIGNMENT(SORT_BY_NAME(.rodata*))) }		//指定只读数据段

	. = ALIGN(4);	//代码以4字节对齐
	.data : { *(.data) }		//指定读写数据段

	. = ALIGN(4);	//代码以4字节对齐
	.got : { *(.got) }		//指定got段，got段是uboot自定义的一个段，非标准段

	__u_boot_cmd_start = .;		//u_boot_cmd 起始位置，其赋值为当前位置
	.u_boot_cmd : { *(.u_boot_cmd) }	//u_boot_cmd 段，uboot把所有的uboot命令放在改段。
	__u_boot_cmd_end = .;		//u_boot_cmd 结束位置，其赋值为当前位置

	. = ALIGN(4);	//代码以4字节对齐
	.mmudata : { *(.mmudata) }		//指定mmu数据段

	. = ALIGN(4);	//代码以4字节对齐
	__bss_start = .;	//bss段的开始位置，把__bss_start赋值为当前位置
	.bss : { *(.bss) . = ALIGN(4); }	//指定bss段
	_end = .;	//bss段的结束位置，把_end赋值为当前位置
}

//链接脚本中处理.text、.data、.rodata、.data、.bss段等这些编译工具自带的段之外，编译工具还允许我们自定义段。
//譬如说uboot的.u_boot_cmd段就是自定义段。自定义段很重要。
