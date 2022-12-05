#include "timer.h"
#include "eagle_soc.h"

/* NOTE:
1 if use nmi source, for autoload timer , the timer setting val can't be less than 100.
2 if use nmi source, this timer has highest priority, can interrupt other isr.
3 if use frc1 source, this timer can't interrupt other isr.*/

#define FRC1_ENABLE_TIMER   BIT7
#define FRC1_AUTO_LOAD      BIT6

static void (* user_hw_timer_cb)(void) = NULL;

// simple blocking delay
inline void delay_us(uint32_t us)
{
	asm volatile("mov.n a2, %0\n_call0 delay4clk" : : "r"(us * (MAIN_MHZ / 8)) : "a2" );
}

// simple blocking delay
inline void delay_ms(uint32_t ms)
{
    for(typeof(ms)i=0; i<ms; i++)
	    delay_us(1000);
}

static uint32_t us_to_ticks(uint32_t val)
{
    if(val > 0)
    {
        if(val > 0x35A)
            return ((val>>2) * ((APB_CLK_FREQ>>4)/250000) + (val&0x3) * ((APB_CLK_FREQ>>4)/1000000));
        else
            return ((val * (APB_CLK_FREQ>>4)) / 1000000);
    }
    return 0;
}

/******************************************************************************
* Parameters   : uint32 val :
in autoload mode
                    50 ~ 0x7fffff;  for FRC1 source.
                    100 ~ 0x7fffff; for NMI source.
in non autoload mode:
                    10 ~ 0x7fffff;
*******************************************************************************/
void  hw_timer_arm(uint32_t val)
{
    RTC_REG_WRITE(FRC1_LOAD_ADDRESS, us_to_ticks(val));
}

/******************************************************************************
* Parameters   : void (* user_hw_timer_cb_set)(void): timer callback function,
*******************************************************************************/
void  hw_timer_set_func(void (* user_hw_timer_cb_set)(void))
{
    user_hw_timer_cb = user_hw_timer_cb_set;
}

static void  hw_timer_isr_cb(void * param)
{
    if (NULL != user_hw_timer_cb)
        (*(user_hw_timer_cb))();
}

/******************************************************************************
* Parameters   :
FRC1_TIMER_SOURCE_TYPE source_type:
                        FRC1_SOURCE,    timer use frc1 isr as isr source.
                        NMI_SOURCE,     timer use nmi isr as isr source.
u8 req:
                        0,  not autoload,
                        1,  autoload mode,
*******************************************************************************/
void ICACHE_FLASH_ATTR hw_timer_init(bool repeat)
{
    if (repeat)
        RTC_REG_WRITE(FRC1_CTRL_ADDRESS, FRC1_AUTO_LOAD | DIVDED_BY_16 | FRC1_ENABLE_TIMER | TM_EDGE_INT);
    else
        RTC_REG_WRITE(FRC1_CTRL_ADDRESS, DIVDED_BY_16 | FRC1_ENABLE_TIMER | TM_EDGE_INT);
    
    ETS_FRC_TIMER1_INTR_ATTACH(hw_timer_isr_cb, NULL);

    TM1_EDGE_INT_ENABLE();
    ETS_FRC1_INTR_ENABLE();
}
