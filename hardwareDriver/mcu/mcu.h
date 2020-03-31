#ifndef _mcu_h
#define _mcu_h

#include "stm32f4xx.h"

#include "uart.h"

#define _tick_config(a) SysTick_Config(SystemCoreClock/a)
#define _tick_handler SysTick_Handler

#endif
