#ifndef _global_h
#define _global_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

/* 获取容器中成员的相对地址偏移 */
#define offset_of(type, member) ((size_t)(&((type *)0)->member))
/* 获取容器地址 */
#define container_of(ptr, type, member) ({ \
    typeof(((type *)0)->member) *__mptr=(ptr); \
    (type *)((char *)(ptr) - offset_of(type, member)); })

/* 对齐操作，只能向 2^x 对齐 */
#define align_floor(value, align) ((value) & ~((align) - 1))
#define align_ceil(value, align) ( align_floor(value, align) + align)

/* 临界保护，在进入临界时使用一个变量记录临界状态，在退出时传入 */
unsigned char enterCriticality(void);
void exitCriticality(unsigned char);

/* 打印输出控制 */
#define GLOBAL_DEBUG
#define GLOBAL_ERROR
#ifdef GLOBAL_DEBUG
#define dbg(fmt, args...) printf("%s: " fmt "\n", __FUNCTION__, ##args)
#else
#define dbg(fmt, args...)
#endif
#ifdef GLOBAL_ERROR
#define err(fmt, args...) printf("\n****ERROR: FUNC(%s) LINE(%d): " fmt "\n\n", __FUNCTION__, __LINE__, ##args)
#else
#define err(fmt, args...)
#endif

#endif
