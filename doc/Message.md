<h1>Message 消息机制</h1>

<h2>目录</h2>

[数据结构](#1)

[功能接口](#2)

[使用示例](#3)

<h2 id="1">数据结构</h2>

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

<h2 id="2">功能接口</h2>

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

<h2 id="3">使用示例<h2>

<h3>1.发布方<h3>

<h4>1.1变量定义<h4>

    m_messageTask_t mMsgPubTest_taskMessage; // 消息任务块
    m_message_t mMsgPubTest_msgBlock[10]; // 消息块
    m_uint8_t mMsgPubTest_msg[10] = {0}; // 存放消息
    m_uint8_t mMsgPubTest_msgIndex = 0; // 下标

<h4>1.2注册消息<h4>

    void mMessagePubTestTask_init(void)
    {
        mCore.task.registration(&mMsgPubTest_task,mMsgPubTestTask_sendMsg); // 注册任务
        mCore.message.registration(&mMsgPubTest_taskMessage,"msgPub","msgSub",mMsgPubTest_msgBlock,10); // 注册消息
    }

<h4>1.3发布消息<h4>

    void mMsgPubTestTask_pubMsg(void)
    {
        if(mCore.message.isIdle(&mMsgPubTest_taskMessage) == m_true) // 判断是否有空闲的消息块
        {
            mMsgPubTest_msg[mMsgPubTest_msgIndex] = 96; // 设置数据
            mCore.message.publish(&mMsgPubTest_taskMessage, &mMsgPubTest_msg[mMsgPubTest_msgIndex]); // 发布数据
            (mMsgPubTest_msgIndex == 9)? (mMsgPubTest_msgIndex = 0):(++mMsgPubTest_msgIndex); // 移动下标
        }
    }

<h3>2.订阅方<h3>

<h4>2.1变量定义<h4>

    m_list_t* mMsgSubTest_readList = m_null; // 消息读取链表指针
    m_uint8_t* mMsgSubTest_msg = m_null; // 消息指针

<h4>2.2读取消息<h4>

    void mMessageSubTestTask_readMsg(void)
    {
        /* 订阅消息 */
        if(mMsgSubTest_readList == m_null) { mMsgSubTest_readList = mCore.message.subscribe("msgPub","msgSub"); } // 未订阅则订阅
        /* 读取消息 */
        if(mCore.message.readMsg(mMsgSubTest_readList,&mMsgSubTest_msg) == m_false) { return; } // 判断读取是否失败
        /* 处理消息 */
        if(*mMsgSubTest_msg == 96) { mMsgSubTest_flag = 1 - mMsgSubTest_flag; }
    }
