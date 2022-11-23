//#include "c_types.h"
#include "esp8266_auxrom.h"
#include "eagle_soc.h"
#include "nosdk8266.h"
#include "timer.h"
//#include <math.h>

// TODO: Use float number (related to 8) to fix the drift
inline void call_delay_us(uint32_t time)
{
	asm volatile("mov.n a2, %0\n_call0 delay4clk" : : "r"(time * (MAIN_MHZ / 8)) : "a2" );
}

#define DELAY 1000 /* milliseconds */

LOCAL os_timer_t blink_timer;

void hw_test_timer_cb(void)
{
	static uint8_t led_state=1;
	if(led_state == 0)
	{
		PIN_OUT_SET = _BV(2); //Turn GPIO2 light on.
		led_state = 1;
	}
	else
	{
		PIN_OUT_CLEAR = _BV(2); //Turn GPIO2 light off.
		led_state = 0;
	}
}

LOCAL void ICACHE_FLASH_ATTR blink_cb(void *arg)
{
	static uint8_t led_state=1;
	if(led_state == 0)
	{
		PIN_OUT_SET = _BV(2); //Turn GPIO2 light on.
		led_state = 1;
	}
	else
	{
		PIN_OUT_CLEAR = _BV(2); //Turn GPIO2 light off.
		led_state = 0;
	}
}

int main() {
	int i = 0;
	nosdk8266_init();

	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U,FUNC_GPIO2);
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO5_U,FUNC_GPIO5);
	PIN_DIR_OUTPUT = _BV(2); //Enable GPIO2 light off.

	// Set up a timer to blink the LED
	// os_timer_disarm(ETSTimer *ptimer)
	// timer_disarm(&blink_timer);
	// os_timer_setfn(ETSTimer *ptimer, ETSTimerFunc *pfunction, void *parg)
	// timer_setfn(&blink_timer, (os_timer_func_t *)blink_cb, (void *)0);
	// void os_timer_arm(ETSTimer *ptimer,uint32_t milliseconds, bool repeat_flag)
	// timer_arm(&blink_timer, DELAY, 1);
	hw_timer_init(true);
    hw_timer_set_func(hw_test_timer_cb);
    hw_timer_arm(500000);

	// timer_attach(DELAY, 1, (os_timer_func_t *)blink_cb, (void *)0);

	while(1)
	{
		// PIN_OUT_SET = _BV(2); //Turn GPIO2 light on.
		call_delay_us(500000);
		// float aa = 12.34;
		// printf("float test: %f\n", aa);
		printf("Hello World %d\n", i);
		// printf("PLL divider register values: (1)0x%x | (2)0x%x\n", rom_i2c_readReg(103, 4, 1), rom_i2c_readReg(103, 4, 2));
		// PIN_OUT_CLEAR = _BV(2); //Turn GPIO2 light off.
		call_delay_us(500000);
		i++;
	}
}

