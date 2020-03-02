#include "mCore.h"
#include <stdio.h>
#include <string.h>
#include "stm32f4xx.h"

/****************************************************************************************************/
//函数声明

//Task
void _m_systemClock_init(void);
void _m_systick_init(void);
void _m_task_init(void);
void _m_task_isr(void);
void _m_task_loop(void);
void _m_task_registration(m_task_t* task, m_call_t call);
void _m_task_setPriority(m_task_t* task, m_uint8_t priority);
void _m_task_setCall(m_task_t* task, m_call_t call);
void _m_task_sleep(m_task_t* task, m_uint32_t time);

//Message
void _m_message_init(void);
void _m_message_registration(m_messageTask_t* taskMsg, char* Publisher, char* Subscriber, m_message_t* msgBlock, m_uint8_t blockSize);
m_bool_t _m_message_isIdle(m_messageTask_t* taskMsg);
m_bool_t _m_message_publish(m_messageTask_t* taskMsg, void* msg);
m_list_t* _m_message_subscribe(char* Publisher, char* Subscriber);
m_bool_t _m_message_readMsg(m_list_t* PublishList, void** msg);

//List
void _m_list_init(m_list_t *list);
void _m_list_insert_before(m_list_t *list,m_list_t *ilist);
void _m_list_remove(m_list_t *rlist);

//函数声明
/****************************************************************************************************/


/****************************************************************************************************/
//变量定义

// 为接口结构指定函数实现
m_core_t mCore = {
    //List
    _m_list_init, _m_list_insert_before, _m_list_remove,
    //Task
    _m_task_registration, _m_task_setCall, _m_task_sleep,
    //Message
    _m_message_registration, _m_message_isIdle, _m_message_publish, _m_message_subscribe, _m_message_readMsg};

//Task
m_list_t _m_taskList_ready; // 任务就绪链表
m_list_t _m_taskList_sleep; // 任务休眠链表
//Message
m_list_t _m_meaasgeList; // 消息链表

//变量定义
/****************************************************************************************************/


/****************************************************************************************************/
//main

int main (void)
{
    _m_systemClock_init(); // 初始化系统时钟
    
    _m_message_init(); // 初始化消息功能
    
    _m_task_init(); // 初始化任务功能

    _m_systick_init(); // 初始化 systick

    while(1)
    {
        _m_task_loop(); // 循环执行任务功能
    }
}

/**
 * 说明 : 初始化系统时钟
 * 参数 : (void)
 * 返回 : (void)
*/
void _m_systemClock_init(void)
{

}
void _m_systick_init(void)
{
    SysTick->LOAD  = (uint32_t)(SystemCoreClock/1000 - 1UL);                         /* set reload register */
    NVIC_SetPriority (SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL); /* set Priority for Systick Interrupt */
    SysTick->VAL   = 0UL;                                             /* Load the SysTick Counter Value */
    SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
                    SysTick_CTRL_TICKINT_Msk   |
                    SysTick_CTRL_ENABLE_Msk;                         /* Enable SysTick IRQ and SysTick Timer */
}
void SysTick_Handler(void)
{
    _m_task_isr();
}
//main
/****************************************************************************************************/


/****************************************************************************************************/
//Task

/**
 * 说明 : 注册任务
 * 参数 :
 *      m_task_t* task : 任务块指针
 *      m_call_t call : 任务回调
 * 返回 : (void)
*/
/*inline */void _m_task_registration(m_task_t* task, m_call_t call)
{
    if(call == m_null) { return; } // 未指定回调

    mCore.list.init(&task->readyList); // 初始化就绪链表
    mCore.list.init(&task->sleepList); // 初始化休眠链表
    task->call = call; // 指定回调
    task->tick = 0; // 清零计时
    task->state = m_ready; // 设置状态为就绪
    mCore.list.insert_before(&_m_taskList_ready, &task->readyList); // 插入到链表头的前面,即链表的尾部
}

/**
 * 说明 : 设置回调
 * 参数 :
 *      m_task_t* task : 任务块指针
 *      m_call_t call : 任务回调
 * 返回 : (void)
*/
/*inline */void _m_task_setCall(m_task_t* task, m_call_t call)
{
    if(call == m_null) { return; } // 未指定回调

    task->call = call;
}

/**
 * 说明 : 相当于延时
 * 参数 :
 *      m_task_t* task : 任务块指针
 *      m_uint32_t time : 任务休眠的时间,单位 ms
 * 返回 : (void)
*/
/*inline */void _m_task_sleep(m_task_t* task, m_uint32_t time)
{
    m_int8_t temp = 0;
    task->tick = time;
    task->state = m_sleep;
    temp = m_interrupt_disable(); // 进入临界区
    mCore.list.insert_before(&_m_taskList_sleep,&task->sleepList);
    m_interrupt_enable(temp); // 退出临界区
}

extern void TaskInit_config(void);
/**
 * 说明 : 初始化任务功能,需要使用一个定时器
 * 参数 : (void)
 * 返回 : (void)
*/
void _m_task_init(void)
{
    mCore.list.init(&_m_taskList_ready); // 初始化任务就绪列表
    mCore.list.init(&_m_taskList_sleep); // 初始化任务休眠列表

    TaskInit_config(); // 初始化添加的任务
}

/**
 * 说明 : 定时器中断,实现时间片任务功能
 * 参数 : (void)
 * 返回 : (void)
*/
/*inline */void _m_task_isr(void)
{
    m_list_t* searchNode;
    m_task_t* tempTask;

    searchNode = _m_taskList_sleep.next;
    for(;searchNode != &_m_taskList_sleep;)
    {
        tempTask = m_getStruct(m_task_t,sleepList,searchNode); // 获取任务块首地址
        searchNode = searchNode->next;
        if(--(tempTask->tick)) { continue; } // 时间未到,返回
        mCore.list.remove(&tempTask->sleepList); // 从休眠链表移除该节点
        tempTask->state = m_ready;
    }
}

/**
 * 说明 : 循环执行任务,需要放在主循环中
 * 参数 : (void)
 * 返回 : (void)
*/
/*inline */void _m_task_loop(void)
{
    m_list_t* searchNode;
    m_task_t* tempTask;

    searchNode = _m_taskList_ready.next;
    for(;searchNode != &_m_taskList_ready;)
    {
        tempTask = m_getStruct(m_task_t,readyList,searchNode); // 获取任务块首地址
        searchNode = searchNode->next;
        if(tempTask->sleepList.next != &tempTask->sleepList) { continue; } // 链表未指向自身,说明已挂到休眠链表,即任务处于休眠状态
        tempTask->call();
    }
}

//Task
/****************************************************************************************************/


/****************************************************************************************************/
//Message

/**
 * 说明 : 初始化消息功能
 * 参数 : (void)
 * 返回 : (void)
*/
void _m_message_init(void)
{
    mCore.list.init(&_m_meaasgeList); // 初始化消息链表
}

/**
 * 说明 : 注册消息
 * 参数 :
 *      m_messageTask_t* taskMsg : 任务消息块地址
 *      char* Publisher : 消息发布者
 *      char* Subscriber : 消息订阅者
 *      m_message_t* msgBlock : 消息块地址
 *      m_uint8_t blockSize : 消息块大小
 * 返回 : (void)
*/
void _m_message_registration(m_messageTask_t* taskMsg, char* Publisher, char* Subscriber, m_message_t* msgBlock, m_uint8_t blockSize)
{
    m_uint8_t nameLen = 0;
    /* 判断消息的发布和订阅方是否符合标准 */
    if(Publisher == m_null || Subscriber == m_null) { return; }
    nameLen = strlen(Publisher);
    if(nameLen == 0 || nameLen > 16) { return; }
    nameLen = strlen(Subscriber);
    if(nameLen == 0 || nameLen > 16) { return; }
    /* 设置参数 */
    strcpy(taskMsg->Publisher,Publisher); // 指定发布方
    strcpy(taskMsg->Subscriber,Subscriber); // 指定订阅方
    mCore.list.init(&taskMsg->PublishList); // 初始化发布链表
    mCore.list.init(&taskMsg->idleList); // 初始化空闲链表
    mCore.list.insert_before(&_m_meaasgeList,&taskMsg->listNode); // 挂载到消息链表
    for(;blockSize != 0;--blockSize,++msgBlock)
    {
        mCore.list.insert_before(&taskMsg->idleList,&msgBlock->listNode); // 将消息块插入空闲链表
    }
}

/**
 * 说明 : 判断空闲链表是否空闲,即是否可以发布消息
 * 参数 :
 *      m_messageTask_t* taskMsg : 任务消息块地址
 * 返回 :
 *      m_bool_t : (是 : m_true , 否 : m_false)
*/
m_bool_t _m_message_isIdle(m_messageTask_t* taskMsg)
{
    if(taskMsg->idleList.next == &taskMsg->idleList) {return m_false;} // 空闲链表为空
    return m_true;
}

/**
 * 说明 : 发布消息
 * 参数 :
 *      m_messageTask_t* taskMsg : 任务消息块地址
 *      void* msg : 消息
 * 返回 :
 *      m_bool_t : 发布消息是否成功(成功 : m_true , 失败 : m_false)
*/
m_bool_t _m_message_publish(m_messageTask_t* taskMsg, void* msg)
{
    m_message_t* tempMsg;
    if(taskMsg->PublishList.next != &taskMsg->PublishList) // 发布链表不为空
    {
        tempMsg = m_getStruct(m_message_t,listNode,taskMsg->PublishList.next); // 获取发布链表下的第一个消息块
        if(tempMsg->state == 1) // 消息已读
        {
            mCore.list.remove(&tempMsg->listNode); // 从发布链表中将该消息块移除
            mCore.list.insert_before(&taskMsg->idleList,&tempMsg->listNode); // 插入空闲链表
        }
    }
    if(taskMsg->idleList.next != &taskMsg->idleList) // 空闲链表不为空
    {
        tempMsg = m_getStruct(m_message_t,listNode,taskMsg->idleList.next); // 获取空闲链表下的第一个消息块
        tempMsg->state = 0; // 设置状态为未读
        tempMsg->msg = msg; // 设置消息
        mCore.list.insert_before(&taskMsg->PublishList,&tempMsg->listNode); // 将新消息插入到发布链表
        return m_true;
    }
    return m_false; // 空闲链表是空的,那么没有空闲的消息块可以用来发布新的消息
}

/**
 * 说明 : 订阅消息
 * 参数 :
 *      char* Publisher : 消息发布者
 *      char* Subscriber : 消息订阅者
 * 返回 :
 *      m_list_t* : 消息发布链表地址
*/
m_list_t* _m_message_subscribe(char* Publisher, char* Subscriber)
{
    m_list_t* searchNode;
    m_messageTask_t* tempMsgTask;
    m_uint8_t nameLen = 0;
    /* 判断消息的发布和订阅方是否符合标准 */
    if(Publisher == m_null || Subscriber == m_null) { return m_null; }
    nameLen = strlen(Publisher);
    if(nameLen == 0 || nameLen > 16) { return m_null; }
    nameLen = strlen(Subscriber);
    if(nameLen == 0 || nameLen > 16) { return m_null; }
    /* 遍历链表 */
    searchNode = _m_meaasgeList.next;
    for(; searchNode != &_m_meaasgeList;)
    {
        tempMsgTask = m_getStruct(m_messageTask_t,listNode,searchNode);
        /* 判断发布和订阅方 */
        if(strcmp(tempMsgTask->Publisher,Publisher) != 0) { continue; }
        if(strcmp(tempMsgTask->Subscriber,Subscriber) != 0) { continue; }
        return &tempMsgTask->PublishList;
    }
    return m_null;
}

/**
 * 说明 : 读取消息
 * 参数 :
 *      m_list_t* PublishList : 从该消息链表读取
 *      void** msg : 存放消息的地址
 * 返回 :
 *      m_bool_t : 读取消息是否成功(成功 : m_true , 失败 : m_false)
*/
m_bool_t _m_message_readMsg(m_list_t* PublishList, void** msg)
{
    m_list_t* searchNode;
    m_message_t* tempMsg;

    searchNode = PublishList->next;
    if(searchNode == PublishList) { return m_false; } // 发布链表为空
    tempMsg = m_getStruct(m_message_t,listNode,searchNode); // 获得消息块
    *msg = tempMsg->msg; // 获取消息
    tempMsg->state = 1; // 标记消息为已读
    return m_true;
}

//Message
/****************************************************************************************************/


/****************************************************************************************************/
//List

/**
 * 说明 :
 *      初始化列表,使列表指向自身
 * 参数 :
 *      _m_list_t* list //列表指针
 * 返回 : (void)
*/
/*inline */void _m_list_init(m_list_t* list)
{
    list->next = list;
    list->prev = list;
}

/**
 * 说明 :
 *      在某节点之前插入一个新节点
 * 参数 :
 *      _m_list_t* list //节点
 *      _m_list_t* ilist //要插入的节点
 * 返回 : (void)
*/
/*inline */void _m_list_insert_before(m_list_t* list,m_list_t* ilist)
{
	ilist->prev = list->prev;
	ilist->prev->next = ilist;
	ilist->next = list;
	list->prev = ilist;
}

/**
 * 说明 :
 *      删除节点
 * 参数 :
 *      _m_list_t* rlist //要移除的节点
 * 返回 : (void)
*/
/*inline */void _m_list_remove(m_list_t* rlist)
{
	rlist->prev->next = rlist->next;
	rlist->next->prev = rlist->prev;
	rlist->next = rlist;
	rlist->prev = rlist;
	
}

//List
/****************************************************************************************************/
