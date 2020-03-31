#include "uart.h"
#include "fifo.h"

#ifdef _use_mcu_uart_stm32f103
/* stm32f103 */

/* stm32f103 */
#endif

/***************************************************************************/

#ifdef _use_mcu_uart_stm32f407
/* stm32f407 */

#include "stm32f4xx.h"

#ifdef _use_mcu_uart_4
/* uart4 */

#define _use_uart4_PC10_PC11
#ifdef _use_uart4_PC10_PC11
    #define _uart4_rcc RCC_AHB1Periph_GPIOC
    #define _uart4_gpio GPIOC
    #define _uart4_pinAF_tx GPIO_PinSource10
    #define _uart4_pinAF_rx GPIO_PinSource11
    #define _uart4_pin_tx GPIO_Pin_10
    #define _uart4_pin_rx GPIO_Pin_11
#endif

fifo_t _uart4_fifo;

/*
    说明：
        初始化串口
    参数：
        unsigned char* buf：串口数据缓冲区
        unsigned char size：缓冲区大小
    返回：
        void
*/
void _uart4_init(unsigned char* buf, unsigned char size)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    fifo_init(&_uart4_fifo,buf,size); // 初始化队列

    RCC_AHB1PeriphClockCmd(_uart4_rcc,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE); // 使能UART4时钟

    //串口2对应引脚复用映射
    GPIO_PinAFConfig(_uart4_gpio,_uart4_pinAF_tx,GPIO_AF_UART4); // 复用为UART4
    GPIO_PinAFConfig(_uart4_gpio,_uart4_pinAF_rx,GPIO_AF_UART4); // 复用为UART4

    //USART2端口配置
    GPIO_InitStructure.GPIO_Pin = _uart4_pin_tx | _uart4_pin_rx;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; // 复用功能
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; // 推挽复用输出
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; // 上拉
    GPIO_Init(_uart4_gpio,&GPIO_InitStructure);

    //USART2 初始化设置
    USART_InitStructure.USART_BaudRate = 115200;//波特率设置
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
    USART_Init(UART4, &USART_InitStructure); //初始化Uart4

    //Usart2 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;//Uart4中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

    USART_Cmd(UART4, ENABLE);  //使能Uart4

    //USART_ClearFlag(UART4, USART_FLAG_TC);
    USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//开启相关中断
}

/*
    说明：
        发送数据
    参数：
        unsigned char* buf：缓冲区指针
        unsigned short len：长度
    返回：
        unsigned short：实际发送的长度
*/
unsigned short _uart4_send(unsigned char* buf, unsigned short len)
{
    while(len--)
    {
        USART_SendData(UART4, *buf++);
        while (USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET);
    }
		return len;
}

/*
    说明：
        读取数据
    参数：
        unsigned char* buf：缓冲区指针
        unsigned short len：长度
    返回：
        unsigned short：实际读取的长度
*/
unsigned short _uart4_read(unsigned char* buf, unsigned short len)
{
    return fifo_read(&_uart4_fifo,buf,len);
}

void UART4_IRQHandler(void)
{
    unsigned char uartRecv = 0;
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
    {
        uartRecv = USART_ReceiveData(UART4);

        fifo_write(&_uart4_fifo,&uartRecv,1); // 入队

        USART_ClearITPendingBit(UART4,USART_IT_RXNE);
    }
}

_uartManage_t uart4 = {_uart4_init, _uart4_send, _uart4_read};

/* uart4 */
#endif

/* stm32f407 */
#endif

