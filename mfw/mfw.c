#include "mFw.h"
#include "app.h"
#include "mcu.h"

/* ================================================================= */
/* ======================console==================================== */

#define _console_uart uart4

/* 标准库所需 */
struct __FILE
{
	int handle;
};
FILE __stdout;

/* 重定义fputc函数 */
int fputc(int ch, FILE *f)
{
    _console_uart.send((unsigned char*)&ch, 1);
	return ch;
}

#define _consoleBuffSize 64
unsigned char _consoleBuff[_consoleBuffSize] = {0};

void _consoleInit(void)
{
	_console_uart.init(_consoleBuff, _consoleBuffSize);
}

/* ================================================================= */

list_t _task_readyListHeadNode, _task_sleepListHeadNode;

/* ================================================================= */
/* ============================thread================================== */



/* ================================================================= */
/* ============================mfw================================== */

void _mfwInit(void)
{
    _consoleInit();
    _taskInit();

    // _appInit();

    dbg("%d", align_ceil(89, 8));
    dbg("%d", align_floor(130, 8));
    
    _tick_config(1000);
}

void _mfwHandler(void)
{
    for(;;)
    {
        _taskHanle();   
    }
}

void _tick_handler(void)
{	
	_taskTick();
}
