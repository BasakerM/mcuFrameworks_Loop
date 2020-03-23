#include "spi.h"

#ifdef _use_mcu_spi_stm32f103
/* stm32f103 */

#include "stm32f10x.h"

#ifdef _use_mcu_spi_1
/* spi1 */

void _spi1_gpioInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef   NVIC_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //开启AHB DMA外设时钟
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure); 
}

void _spi1_setMode_master(void)
{
    SPI_InitTypeDef SPI_InitStructure;
    
	SPI_Cmd(SPI1, DISABLE);
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
 
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_Init(SPI1, &SPI_InitStructure);

    SPI_I2S_ITConfig(SPI1,SPI_I2S_IT_RXNE,DISABLE);

	SPI_Cmd(SPI1, ENABLE);
}

void _spi1_setMode_slave(void)
{
    SPI_InitTypeDef SPI_InitStructure;
    
	SPI_Cmd(SPI1, DISABLE);
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
 
	SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
	SPI_Init(SPI1, &SPI_InitStructure);
    
    SPI_I2S_ITConfig(SPI1,SPI_I2S_IT_RXNE,ENABLE);

	SPI_Cmd(SPI1, ENABLE);
}

fifo_t _spi1_fifo;
unsigned char _spi1_isMaster;

void _spi1_init(unsigned char* buf, unsigned char size)
{
    fifo.init(&_spi1_fifo, buf, size);
    _spi1_isMaster = 1;

    _spi1_gpioInit();
    _spi1_setMode_master();
}

void SPI1_IRQHandler(void)
{
    unsigned char ch = 0;
    ch = SPI_I2S_ReceiveData(SPI2);
    fifo.write(&_spi1_fifo, &ch, 1);
}

unsigned char _spi1_write(unsigned char* buf, unsigned char len)
{
    if(_spi1_isMaster != 1) { return 0; }

    while(len--)
    {
        while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
        SPI_I2S_SendData(SPI1, *buf++);
    }
    
    return 1;
}

unsigned char _spi1_read(unsigned char* buf, unsigned char len)
{
    if(_spi1_isMaster == 1) { return 0; }
    if(fifo.getReadableLen(&_spi1_fifo) < len) { return 0; }

    fifo.read(&_spi1_fifo, buf, len);

    return 1;
}

spiManage_t spi1 = {_spi1_init, _spi1_write, _spi1_read, _spi1_setMode_master, _spi1_setMode_slave};
/* spi1 */
#endif

#ifdef _use_mcu_spi_2
/* spi2 */

/* spi2 */
#endif

/* stm32f103 */
#endif

/*****************************************************************/

#ifdef _use_mcu_spi_stm32f407
/* stm32f407 */

#include "stm32f4xx.h"

#ifdef _use_mcu_spi_1
/* spi1 */

void _spi1_gpioInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef   NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOB时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOB时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);//使能SPI1时钟

    //GPIOFA5,6初始化设置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;//PA4,5,6复用功能输出
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化
    //GPIOFB5初始化设置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//PB5复用功能输出
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化

    GPIO_PinAFConfig(GPIOA,GPIO_PinSource4,GPIO_AF_SPI1); //PA4复用为 SPI1
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_SPI1); //PA5复用为 SPI1
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_SPI1); //PA6复用为 SPI1
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_SPI1); //PB5复用为 SPI1

    //这里只针对SPI口初始化
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,ENABLE);//复位SPI1
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,DISABLE);//停止复位SPI1

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//串行同步时钟的空闲状态为高电平
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//串行同步时钟的第二个跳变沿（上升或下降）数据被采样
    SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;		//定义波特率预分频的值:波特率预分频值为256
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
    SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
    SPI_Init(SPI1, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器

    //Spi1 NVIC 配置
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 
    NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    SPI_I2S_ITConfig(SPI1,SPI_I2S_IT_RXNE,ENABLE); 

    SPI_Cmd(SPI1, ENABLE); //使能SPI外设
}

void _spi1_setMode_master(void)
{
    SPI_InitTypeDef SPI_InitStructure;
    
	SPI_Cmd(SPI1, DISABLE);
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
 
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_Init(SPI1, &SPI_InitStructure);

    // SPI_I2S_ITConfig(SPI1,SPI_I2S_IT_RXNE,DISABLE);

	SPI_Cmd(SPI1, ENABLE);
}

void _spi1_setMode_slave(void)
{
    SPI_InitTypeDef SPI_InitStructure;
    
	SPI_Cmd(SPI1, DISABLE);
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
 
	SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
	SPI_Init(SPI1, &SPI_InitStructure);
    
    SPI_I2S_ITConfig(SPI1,SPI_I2S_IT_RXNE,ENABLE);

	SPI_Cmd(SPI1, ENABLE);
}

fifo_t _spi1_fifo;
unsigned char _spi1_isMaster;

void _spi1_init(unsigned char* buf, unsigned char size)
{
    fifo.init(&_spi1_fifo, buf, size);
    _spi1_isMaster = 1;

    _spi1_gpioInit();
    _spi1_setMode_master();
}

void SPI1_IRQHandler(void)
{
    unsigned char ch = 0;
    ch = SPI_I2S_ReceiveData(SPI2);
    fifo.write(&_spi1_fifo, &ch, 1);
}

unsigned char _spi1_write(unsigned char* buf, unsigned char len)
{
    if(_spi1_isMaster != 1) { return 0; }

    while(len--)
    {
        while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
        SPI_I2S_SendData(SPI1, *buf++);
    }
    
    return 1;
}

unsigned char _spi1_read(unsigned char* buf, unsigned char len)
{
    if(_spi1_isMaster == 1) { return 0; }
    if(fifo.getReadableLen(&_spi1_fifo) < len) { return 0; }

    fifo.read(&_spi1_fifo, buf, len);

    return 1;
}

spiManage_t spi1 = {_spi1_init, _spi1_write, _spi1_read, _spi1_setMode_master, _spi1_setMode_slave};

/* spi1 */
#endif

/* stm32f407 */
#endif
