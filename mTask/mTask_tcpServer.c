#include "mCore.h"
#include "mTcpDriver.h"

void mTcpServerTask_sendData(void);

m_task_t mTaskBlock_tcpServer;

void mTcpServerTask_init(void)
{
    mTcp.serverInit();
    mCore.task.registration(&mTaskBlock_tcpServer,mTcpServerTask_sendData); // 注册任务
}

void mTcpServerTask_sendData(void)
{
    mTcp.serverLoop();
    mTcp.serverSendData("\ntcp test");
    // mCore.task.sleep(&mTaskBlock_tcpServer,1000);
}
