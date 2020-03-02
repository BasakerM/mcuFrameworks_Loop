#ifndef _m_console_driver_h
#define _m_console_driver_h

typedef struct
{
    /*
    *   初始化 uart 驱动
    *   参数: (void)
    */
    void (*init)(void);
    /*
    *
    */
    int (*printf)(const char *, ...);
}mUartDriverManage_t;

extern mUartDriverManage_t mUart1;

#endif
