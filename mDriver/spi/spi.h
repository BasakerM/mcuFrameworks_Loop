#ifndef _spi_h
#define _spi_h

/* mcu */
// #define _use_mcu_spi_stm32f103
#define _use_mcu_spi_stm32f407

/* spi */
#define _use_mcu_spi_1

#include "fifo.h"

/* api */
typedef struct
{
    void (*init)(unsigned char* buf, unsigned char size);
    unsigned char (*write)(unsigned char* buf, unsigned char len);
    unsigned char (*read)(unsigned char* buf, unsigned char len);
    void (*setMaster)(void);
    void (*setSlave)(void);
}spiManage_t;

#ifdef _use_mcu_spi_1
extern spiManage_t spi1;
#endif

#endif
