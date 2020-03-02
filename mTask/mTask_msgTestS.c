#include "mCore.h"

void mMessageSubTestTask_getMsg(void);

m_task_t mMsgSubTest_taskMessage;
m_list_t* mMsgSubTest_readList = m_null;
m_uint8_t* mMsgSubTest_msg = m_null;
m_uint8_t mMsgSubTest_flag = 0;

void mMessageSubTestTask_init(void)
{
    mCore.task.registration(&mMsgSubTest_taskMessage,mMessageSubTestTask_getMsg); // 注册任务
}

void mMessageSubTestTask_getMsg(void)
{
    /* 订阅消息 */
    if(mMsgSubTest_readList == m_null) { mMsgSubTest_readList = mCore.message.subscribe("msgPub","msgSub"); } // 未订阅则订阅
    /* 读取消息 */
    if(mCore.message.readMsg(mMsgSubTest_readList,(void**)&mMsgSubTest_msg) == m_false) { return; } // 判断读取是否失败
    /* 处理消息 */
    if(*mMsgSubTest_msg == 96) { mMsgSubTest_flag = 1 - mMsgSubTest_flag; }
}
