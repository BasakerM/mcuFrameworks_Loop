#include "mCore.h"
#include "spi.h"

void mSpiTask_sendData(void);

#define button_spiBuffSize 64
unsigned char button_spiBuff[button_spiBuffSize] = {0};
m_task_t mTaskBlock_spi;

void mSpiTask_init(void)
{
    spi1.init(button_spiBuff, button_spiBuffSize);
    spi1.setMaster();
    mCore.task.registration(&mTaskBlock_spi,mSpiTask_sendData); // 注册任务
}

void mSpiTask_readData(void)
{

}

void mSpiTask_sendData(void)
{
    spi1.write("spi test",8);
    mCore.task.sleep(&mTaskBlock_spi,10);
}
