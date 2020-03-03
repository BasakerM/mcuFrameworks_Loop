#ifndef _MALLOC_H
#define _MALLOC_H
#include "stm32f4xx.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//内存管理 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/7/8
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 

#ifndef NULL
#define NULL 0
#endif

//定义三个内存池
#define SRAMIN 	0  //内部内存池
#define SRAMEX 	1  //外部内存池
#define SRAMCCM 2  //CCM内存池(此部分SRAM仅仅CPU可以访问！！！)

#define SRAMBANK  3 //定义支持的SRAM块数


//mem1内存参数设定,mem1完全处于内部SRAM里面
#define MEM1_BLOCK_SIZE	32  			//内存块大小为32字节
#define MEM1_MAX_SIZE		100*1024 	//最大管理内存 110k
#define MEM1_ALLOC_TABLE_SIZE MEM1_MAX_SIZE/MEM1_BLOCK_SIZE  //内存表大小

//mem2内存参数设定,mem2处于外部SRAM里面
#define MEM2_BLOCK_SIZE	32  			//内存块大小为32字节
#define MEM2_MAX_SIZE		200*1024 	//最大管理内存 200k
#define MEM2_ALLOC_TABLE_SIZE MEM2_MAX_SIZE/MEM2_BLOCK_SIZE  //内存表大小

//mem3内存参数设定,mem3处于CCM,用于管理CCM(特别注意,这部分SRAM,近CPU可以访问)
#define MEM3_BLOCK_SIZE	32  			//内存块大小为32字节
#define MEM3_MAX_SIZE		60*1024 	//最大管理内存 60k
#define MEM3_ALLOC_TABLE_SIZE MEM3_MAX_SIZE/MEM3_BLOCK_SIZE  //内存表大小

//内存管理控制器
struct _m_mallco_dev
{
	void (*init)(unsigned char);  		//初始化
	unsigned char (*perused)(unsigned char); 		//内存使用率
	unsigned char *membase[SRAMBANK]; //内存池,管理SRAMBANK个区域的内存
	unsigned short *memmap[SRAMBANK];  //内存状态表
	unsigned char memrdy[SRAMBANK];   //内存管理是否就绪
};
extern struct _m_mallco_dev mallco_dev;  //在malloc.c里面定义

void mymemset(void *s,unsigned char c,unsigned long count);	 //设置内存
void mymemcpy(void *des,void *src,unsigned long n);//复制内存     
void mymem_init(unsigned char memx);					 //内存管理初始化函数(外/内部调用)
unsigned long mymem_malloc(unsigned char memx,unsigned long size);		 //内存分配(内部调用)
unsigned char mymem_free(unsigned char memx,unsigned long offset);		 //内存释放(内部调用)
unsigned char mem_perused(unsigned char memx);				 //获得内存使用率(外/内部调用) 
////////////////////////////////////////////////////////////////////////////////
//用户调用函数
void myfree(unsigned char memx,void *ptr);  			//内存释放(外部调用)
void *mymalloc(unsigned char memx,unsigned long size);			//内存分配(外部调用)
void *myrealloc(unsigned char memx,void *ptr,unsigned long size);//重新分配内存(外部调用)
#endif


