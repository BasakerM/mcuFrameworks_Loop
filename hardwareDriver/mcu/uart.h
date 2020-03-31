#ifndef _uart_h
#define _uart_h

/* mcu */
// #define _use_mcu_uart_stm32f103
#define _use_mcu_uart_stm32f407

/* uart */
// #define _use_mcu_uart_1
// #define _use_mcu_uart_3
#define _use_mcu_uart_4

typedef struct
{
    /*
        说明：
            初始化串口
        参数：
            unsigned char* buf：串口数据缓冲区
            unsigned char size：缓冲区大小
        返回：
            void
    */
    void (*init)(unsigned char* buf, unsigned char size);

    /*
        说明：
           发送数据
        参数：
           unsigned char* buf：缓冲区指针
           unsigned short len：长度
        返回：
            unsigned short：实际发送的长度
    */
    unsigned short (*send)(unsigned char* buf, unsigned short len);

    /*
        说明：
           读取数据
        参数：
           unsigned char* buf：缓冲区指针
           unsigned short len：长度
        返回：
            unsigned short：实际读取的长度
    */
    unsigned short (*read)(unsigned char* buf, unsigned short len);
}_uartManage_t;

#ifdef _use_mcu_uart_1
extern _uartManage_t uart1;
#endif
#ifdef _use_mcu_uart_3
extern _uartManage_t uart3;
#endif
#ifdef _use_mcu_uart_4
extern _uartManage_t uart4;
#endif

#endif
