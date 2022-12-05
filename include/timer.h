#pragma once

#include "ets_sys.h"

typedef void ETSTimerFunc(void *timer_arg);

typedef struct _ETSTIMER_
{
    struct _ETSTIMER_    *timer_next;
    uint32_t              timer_expire;
    uint32_t              timer_period;
    ETSTimerFunc         *timer_func;
    void                 *timer_arg;
} ETSTimer;

typedef void os_timer_func_t(void *timer_arg);
typedef void (*callback_with_arg_t)(void*);

typedef struct _os_timer_t
{
    struct _os_timer_t *timer_next;
    void               *timer_handle;
    uint32_t            timer_expire;
    uint32_t            timer_period;
    os_timer_func_t    *timer_func;
    bool                timer_repeat_flag;
    void               *timer_arg;
} os_timer_t;

//TIMER PREDIVED MODE
typedef enum
{
    DIVDED_BY_1 = 0,		//timer clock
    DIVDED_BY_16 = 4,	//divided by 16
    DIVDED_BY_256 = 8,	//divided by 256
} TIMER_PREDIVED_MODE;

typedef enum
{			//timer interrupt mode
    TM_LEVEL_INT = 1,	// level interrupt
    TM_EDGE_INT   = 0,	//edge interrupt
} TIMER_INT_MODE;

typedef enum
{
    FRC1_SOURCE = 0,
    NMI_SOURCE = 1,
} FRC1_TIMER_SOURCE_TYPE;

void ICACHE_FLASH_ATTR hw_timer_init(bool repeat);
void  hw_timer_arm(uint32_t val);
void  hw_timer_set_func(void (* user_hw_timer_cb_set)(void));

// simple blocking delays
void delay_us(uint32_t us);
void delay_ms(uint32_t ms);


//------------------------------- Test Code --------------------------------------
#if 0
void cool_cb(void)
{
  ;// Some important work
}

void main(void)
{
  // cool_cb() will be called every 500 ms
  hw_timer_init(true);        // init timer in repeated mode
  hw_timer_set_func(cool_cb); // set callback
  hw_timer_arm(500000);       // set period 500ms
}
#endif

