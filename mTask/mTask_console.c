#include "mCore.h"
#include "mUartDriver.h"

void mConsoleTask_sendData(void);

m_task_t mTaskBlock_console;

void mConsoleTask_init(void)
{
    mUart1.init();
    mCore.task.registration(&mTaskBlock_console,mConsoleTask_sendData); // 注册任务
}

void mConsoleTask_readData(void)
{

}

void mConsoleTask_sendData(void)
{
    mUart1.printf("\nuart1");
    mCore.task.sleep(&mTaskBlock_console,1000);
}
