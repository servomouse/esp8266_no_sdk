#ifndef TIMER0_H
#define TIMER0_H

#include "ets_sys.h"

#ifdef __cplusplus
extern "C" {
#endif

/* timer related */
typedef void os_timer_func_t(void *timer_arg);
typedef void (*callback_with_arg_t)(void*);

typedef struct _os_timer_t {
    struct _os_timer_t *timer_next;
    void               *timer_handle;
    uint32_t             timer_expire;
    uint32_t             timer_period;
    os_timer_func_t    *timer_func;
    bool               timer_repeat_flag;
    void               *timer_arg;
} os_timer_t;

/** @addtogroup Timer_APIs
  * @{
  */

/**
  * @brief     Set the timer callback function.
  *
  * @attention 1. The callback function must be set in order to enable the timer.
  * @attention 2. Operating system scheduling is disabled in timer callback.
  *
  * @param     os_timer_t *ptimer : Timer structure
  * @param     os_timer_func_t *pfunction : timer callback function
  * @param     void *parg : callback function parameter
  *
  * @return    null
  */
void timer_setfn(os_timer_t *ptimer, os_timer_func_t *pfunction, void *parg);

/**
  * @brief  Enable the millisecond timer.
  *
  * @param  os_timer_t *ptimer : timer structure
  * @param  uint32_t milliseconds : Timing, unit: millisecond, range: 5 ~ 0x68DB8
  * @param  bool repeat_flag : Whether the timer will be invoked repeatedly or not
  *
  * @return null
  */
void timer_arm(os_timer_t *ptimer, uint32_t msec, bool repeat_flag);

/**
  * @brief  Disarm the timer
  *
  * @param  os_timer_t *ptimer : Timer structure
  *
  * @return null
  */
void timer_disarm(os_timer_t *ptimer);


void timer_attach(uint32_t milliseconds, bool repeat, callback_with_arg_t callback, void* arg);

//TIMER PREDIVED MODE
typedef enum {
    DIVDED_BY_1 = 0,		//timer clock
    DIVDED_BY_16 = 4,	//divided by 16
    DIVDED_BY_256 = 8,	//divided by 256
} TIMER_PREDIVED_MODE;

typedef enum {			//timer interrupt mode
    TM_LEVEL_INT = 1,	// level interrupt
    TM_EDGE_INT   = 0,	//edge interrupt
} TIMER_INT_MODE;

typedef enum {
    FRC1_SOURCE = 0,
    NMI_SOURCE = 1,
} FRC1_TIMER_SOURCE_TYPE;

void ICACHE_FLASH_ATTR hw_timer_init(bool repeat);
void  hw_timer_arm(uint32_t val);
void  hw_timer_set_func(void (* user_hw_timer_cb_set)(void));

#ifdef __cplusplus
}
#endif


#endif
