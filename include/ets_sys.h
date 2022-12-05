#pragma once

#include "c_types.h"

typedef struct ETSEventTag
{
    uint32_t sig;
    uint32_t par;
}ETSEvent;

typedef void (*ETSTask)(ETSEvent *e);

/* interrupt related */
#define ETS_SDIO_INUM       1
#define ETS_SPI_INUM	    2
#define ETS_GPIO_INUM       4
#define ETS_UART_INUM       5
#define ETS_UART1_INUM      5
#define ETS_FRC_TIMER1_INUM 9  /* use edge*/

typedef void (* ets_isr_t)(void *);

void ets_intr_lock(void);
void ets_intr_unlock(void);
void ets_isr_attach(int i, ets_isr_t func, void *arg);
void ets_isr_mask(uint32 mask);
void ets_isr_unmask(uint32 unmask);

void NmiTimSetFunc(void (*func)(void));

#define ETS_INTR_LOCK() ets_intr_lock()

#define ETS_INTR_UNLOCK() ets_intr_unlock()

#define ETS_FRC_TIMER1_INTR_ATTACH(func, arg) ets_isr_attach(ETS_FRC_TIMER1_INUM, (func), (void *)(arg))

#define ETS_FRC_TIMER1_NMI_INTR_ATTACH(func) NmiTimSetFunc(func)

#define ETS_SDIO_INTR_ATTACH(func, arg) ets_isr_attach(ETS_SDIO_INUM, (func), (void *)(arg))

#define ETS_GPIO_INTR_ATTACH(func, arg) ets_isr_attach(ETS_GPIO_INUM, (func), (void *)(arg))

#define ETS_UART_INTR_ATTACH(func, arg) ets_isr_attach(ETS_UART_INUM, (func), (void *)(arg))

#define ETS_SPI_INTR_ATTACH(func, arg) ets_isr_attach(ETS_SPI_INUM, (func), (void *)(arg))

#define ETS_INTR_ENABLE(inum) ets_isr_unmask((1<<inum))

#define ETS_INTR_DISABLE(inum) ets_isr_mask((1<<inum))

#define ETS_UART_INTR_ENABLE() ETS_INTR_ENABLE(ETS_UART_INUM)

#define ETS_UART_INTR_DISABLE() ETS_INTR_DISABLE(ETS_UART_INUM)

#define ETS_FRC1_INTR_ENABLE() ETS_INTR_ENABLE(ETS_FRC_TIMER1_INUM)

#define ETS_FRC1_INTR_DISABLE() ETS_INTR_DISABLE(ETS_FRC_TIMER1_INUM)

#define ETS_GPIO_INTR_ENABLE() ETS_INTR_ENABLE(ETS_GPIO_INUM)

#define ETS_GPIO_INTR_DISABLE() ETS_INTR_DISABLE(ETS_GPIO_INUM)
    
#define ETS_SPI_INTR_ENABLE() ETS_INTR_ENABLE(ETS_SPI_INUM)

#define ETS_SPI_INTR_DISABLE() ETS_INTR_DISABLE(ETS_SPI_INUM)

#define ETS_SDIO_INTR_ENABLE() ETS_INTR_ENABLE(ETS_SDIO_INUM)

#define ETS_SDIO_INTR_DISABLE() ETS_INTR_DISABLE(ETS_SDIO_INUM)
