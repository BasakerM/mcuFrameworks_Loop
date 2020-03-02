#include "mCore.h"

void mMsgPubTestTask_sendMsg(void);

m_task_t mMsgPubTest_task;
m_messageTask_t mMsgPubTest_taskMessage;
m_message_t mMsgPubTest_msgBlock[10];
m_uint8_t mMsgPubTest_msg[10] = {0},mMsgPubTest_msgIndex = 0;

void mMessagePubTestTask_init(void)
{
    mCore.task.registration(&mMsgPubTest_task,mMsgPubTestTask_sendMsg); // 注册任务
    mCore.message.registration(&mMsgPubTest_taskMessage,"msgPub","msgSub",mMsgPubTest_msgBlock,10); // 注册消息
}

void mMsgPubTestTask_sendMsg(void)
{
    if(mCore.message.isIdle(&mMsgPubTest_taskMessage) == m_true) // 判断是否有空闲的消息块
    {
        mMsgPubTest_msg[mMsgPubTest_msgIndex] = 96; // 设置数据
        mCore.message.publish(&mMsgPubTest_taskMessage, &mMsgPubTest_msg[mMsgPubTest_msgIndex]); // 发布数据
        (mMsgPubTest_msgIndex == 9)? (mMsgPubTest_msgIndex = 0):(++mMsgPubTest_msgIndex); // 移动下标
    }
}
