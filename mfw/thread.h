#ifndef _thread_h
#define _thread_h

#include "global.h"
#include "list.h"

#define _CONSOLE_DEBUG
#define _CONSOLE_ERROR

#ifdef _CONSOLE_DEBUG
#define _console_dbg(fmt, args...) dbg(fmt, args...)
#else
#define _console_dbg(fmt, args...)
#endif
#ifdef _CONSOLE_ERROR
#define _console_err(fmt, args...) err(fmt, args...)
#else
#define _console_err(fmt, args...)
#endif


#define _threadNumMax ((size_t)4) // 最大线程数
#define _thread_statck_size ((size_t)256) // 单个线程的栈空间大小

/* 线程控制块结构体 */
typedef struct _m_struct_thread_t
{
	void *sp; // 线程栈指针
	uint8_t priority; // 线程优先级
	uint32_t timeout; // 超时时间
	list_t timeout_list; // 超时链表
}_m_thread_t;

static inline void thread_creat(void)
{

}

static inline void thread_sleep(void)
{

}

static inline void _threadInit(void)
{
    _console_dbg("thread init");
}

static inline void _threadTick(void)
{

}

#endif
