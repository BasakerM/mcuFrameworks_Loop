#ifndef _task_h
#define _task_h

#include "list.h"

typedef struct
{
    // m_uint8_t priority; // 优先级
    unsigned long tick; // 计时
    void (*call)(void); //回调
    unsigned char state; // 状态

    list_t readyList; //就绪链表
    list_t sleepList; //休眠链表
}task_t;

unsigned char m_interrupt_disable(void);
void m_interrupt_enable(unsigned char);

#define _task_getStruct(type, member, addr) ((type *)((char *)(addr) - (unsigned long)(&((type *)0)->member)))

typedef enum {_task_ready, _task_slepp}_task_State_t;

extern list_t _task_readyListHeadNode, _task_sleepListHeadNode;


/**
 * 说明 : 注册任务
 * 参数 :
 *      task_t* task : 任务块指针
 *      void (*call)(void) : 任务回调
 * 返回 : (void)
*/
static inline void task_registration(task_t* task, void (*call)(void))
{
    if(call == 0) { return; } // 未指定回调

    list_init(&(task->readyList)); // 初始化就绪链表
    list_init(&(task->sleepList)); // 初始化休眠链表
    task->call = call; // 指定回调
    task->tick = 0; // 清零计时
    task->state = _task_ready; // 设置状态为就绪
    list_insertBefore(&_task_readyListHeadNode, &task->readyList); // 插入到就绪链表头节点的前面,即链表的尾部
}

/**
 * 说明 : 设置回调
 * 参数 :
 *      task_t* task : 任务块指针
 *      void (*call)(void) : 任务回调
 * 返回 : (void)
*/
static inline void task_setCall(task_t* task, void (*call)(void))
{
    if(call == 0) { return; } // 未指定回调

    task->call = call;
}

/**
 * 说明 : 相当于延时
 * 参数 :
 *      task_t* task : 任务块指针
 *      unsigned long time : 任务休眠的时间,单位 ms
 * 返回 : (void)
*/
static inline void task_sleep(task_t* task, unsigned long time)
{
    char temp = 0;
    task->tick = time;
    task->state = _task_slepp;
    temp = enterCriticality(); // 进入临界区
    list_insertBefore(&_task_sleepListHeadNode,&task->sleepList);
    exitCriticality(temp); // 退出临界区
}

/***************************************************************************/

/**
 * 说明 : 初始化任务功能,需要使用一个定时器
 * 参数 : (void)
 * 返回 : (void)
*/
static inline void _taskInit(void)
{
    list_init(&_task_readyListHeadNode); // 初始化任务就绪列表
    list_init(&_task_sleepListHeadNode); // 初始化任务休眠列表
}

/**
 * 说明 : 定时器中断,实现时间片任务功能
 * 参数 : (void)
 * 返回 : (void)
*/
static inline void _taskTick(void)
{
    list_t* searchNode;
    task_t* tempTask;

    searchNode = _task_sleepListHeadNode.next;
    for(;searchNode != &_task_sleepListHeadNode;)
    {
        tempTask = _task_getStruct(task_t,sleepList,searchNode); // 获取任务块首地址
        searchNode = searchNode->next;
        if(--(tempTask->tick)) { continue; } // 时间未到,返回
        list_remove(&tempTask->sleepList); // 从休眠链表移除该节点
        tempTask->state = _task_ready;
    }
}

/**
 * 说明 : 循环执行任务,需要放在主循环中
 * 参数 : (void)
 * 返回 : (void)
*/
static inline void _taskHanle(void)
{
    list_t* searchNode;
    task_t* tempTask;

    searchNode = _task_readyListHeadNode.next;
    for(;searchNode != &_task_readyListHeadNode;)
    {
        tempTask = _task_getStruct(task_t,readyList,searchNode); // 获取任务块首地址
        searchNode = searchNode->next;
        if(tempTask->sleepList.next != &tempTask->sleepList) { continue; } // 链表未指向自身,说明已挂到休眠链表,即任务处于休眠状态
        tempTask->call();
    }
}

#endif
