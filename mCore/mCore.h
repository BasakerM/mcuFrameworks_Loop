#ifndef _mCore_h
#define _mCore_h

/**
 * 说明 :
 *      通过结构体成员地址计算获得该结构体首地址
 * 参数 :
 *      type : 结构体类型
 *      member : 结构体成员
 *      addr : 该结构体成员的地址
 * 返回 : 结构体的首地址
*/
#define m_getStruct(type, member, addr) ((type *)((char *)(addr) - (unsigned long)(&((type *)0)->member)))

#define m_null (0)
typedef enum _m_mCore_bool_t {m_false, m_true}m_bool_t; //布尔型
typedef char m_int8_t; //8位有符号整型
typedef unsigned char m_uint8_t; //8位无符号整型
typedef unsigned short m_uint16_t; //16位无符号整型
typedef unsigned long m_uint32_t; //32位无符号整型
typedef void (*m_call_t)(void); // 回调函数类型


m_int8_t m_interrupt_disable(void);
void m_interrupt_enable(m_int8_t);


/****************************************************************************************************/
//List

//链表结构
typedef struct _m_structList_t
{
    struct _m_structList_t* prev;
    struct _m_structList_t* next;
}m_list_t;

//链表管理器
typedef struct
{
    /**
     * 说明 :
     *      初始化列表,使列表指向自身
     * 参数 :
     *      _m_list_t* list //列表指针
     * 返回 : (void)
    */
    void (*init)(m_list_t* list);
    /**
     * 说明 :
     *      在某节点之前插入一个新节点
     * 参数 :
     *      _m_list_t* list //节点
     *      _m_list_t* ilist //要插入的节点
     * 返回 : (void)
    */
    void (*insert_before)(m_list_t* list,m_list_t* ilist);
    /**
     * 说明 :
     *      删除节点
     * 参数 :
     *      _m_list_t* rlist //要移除的节点
     * 返回 : (void)
    */
    void (*remove)(m_list_t* rlist);
}m_listManage_t;

//List
/****************************************************************************************************/


/****************************************************************************************************/
//Task

//任务块
typedef struct
{
    // m_uint8_t priority; // 优先级
    m_uint32_t tick; // 计时
    m_call_t call; //回调
    m_int8_t state; // 状态

    m_list_t readyList; //就绪链表
    m_list_t sleepList; //休眠链表
}m_task_t;

// 任务管理器
typedef struct
{
    /**
     * 说明 : 注册任务
     * 参数 :
     *      m_task_t* task : 任务块指针
     *      m_call_t call : 任务回调
     * 返回 : (void)
    */
    void (*registration)(m_task_t* task, m_call_t call);
    // /**
    //  * 说明 : 设置优先级
    //  * 参数 :
    //  *      m_task_t* task : 任务块指针
    //  *      m_uint8_t priority : 任务优先级
    //  * 返回 : (void)
    // */
    // void (*setPriority)(m_task_t* task, m_uint8_t priority);
    /**
     * 说明 : 设置回调
     * 参数 :
     *      m_task_t* task : 任务块指针
     *      m_call_t call : 任务回调
     * 返回 : (void)
    */
    void (*setCall)(m_task_t* task, m_call_t call);
   /**
    * 说明 : 相当于延时
     * 参数 :
     *      m_task_t* task : 任务块指针
     *      m_uint32_t time : 任务休眠的时间,单位 ms
     * 返回 : (void)
   */
    void (*sleep)(m_task_t* task, m_uint32_t time);
}m_taskManage_t;

typedef enum _m_task_state_t {m_ready, m_sleep}m_taskState_t;

//Task
/****************************************************************************************************/


/****************************************************************************************************/
//Message

//消息状态
// typedef enum _m_message_state_t {m_read, m_unread}m_messageState_t;

//消息块
typedef struct
{
    m_uint8_t state; // 消息状态
    void* msg; // 消息内容
    m_list_t listNode; //链表节点
}m_message_t;

//任务消息块
typedef struct
{
    char Publisher[16]; // 发布者
    char Subscriber[16]; // 订阅者
    m_list_t PublishList; // 发布链表
    m_list_t idleList; //空闲链表
    m_list_t listNode; //链表节点
}m_messageTask_t;

//消息管理器
typedef struct
{
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
    void (*registration)(m_messageTask_t* taskMsg, char* Publisher, char* Subscriber, m_message_t* msgBlock, m_uint8_t blockSize);
    /**
     * 说明 : 判断空闲链表是否挂载了消息块,即是否可以发布消息
     * 参数 :
     *      m_messageTask_t* taskMsg : 任务消息块指针
     * 返回 :
     *      m_bool_t : (是 : m_true , 否 : m_false)
    */
    m_bool_t (*isIdle)(m_messageTask_t* taskMsg);
    /**
     * 说明 : 发布消息
     * 参数 :
     *      m_messageTask_t* taskMsg : 任务消息块指针
     *      void* msg : 消息
     * 返回 :
     *      m_bool_t : 发布消息是否成功(成功 : m_true , 失败 : m_false)
    */
    m_bool_t (*publish)(m_messageTask_t* taskMsg, void* msg);
    /**
     * 说明 : 订阅消息
     * 参数 :
     *      char* Publisher : 消息发布者
     *      char* Subscriber : 消息订阅者
     * 返回 :
     *      m_list_t* : 消息发布链表地址
    */
    m_list_t* (*subscribe)(char* Publisher, char* Subscriber);
    /**
     * 说明 : 读取消息
     * 参数 :
     *      m_list_t* PublishList : 从该消息链表读取
     *      void** msg : 存放消息的地址
     * 返回 :
     *      m_bool_t : 读取消息是否成功(成功 : m_true , 失败 : m_false)
    */
    m_bool_t (*readMsg)(m_list_t* PublishList, void** msg);
}m_messageManage_t;

//Message
/****************************************************************************************************/


/****************************************************************************************************/
//Core

typedef struct
{
    m_listManage_t list; // 链表管理器
    m_taskManage_t task; // 任务管理器
    m_messageManage_t message; // 消息管理器
}m_core_t;

extern m_core_t mCore; // 

//Core
/****************************************************************************************************/

#endif
