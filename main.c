//#include "c_types.h"
#include "esp8266_auxrom.h"
#include "eagle_soc.h"
#include "nosdk8266.h"
#include "timer.h"
#include "spi.h"
//#include <math.h>

#define DELAY 1000 /* milliseconds */

#define PWM_0_OUT_IO_MUX PERIPHS_IO_MUX_MTDI_U
#define PWM_0_OUT_IO_NUM 12
#define PWM_0_OUT_IO_FUNC FUNC_GPIO12

#define PWM_1_OUT_IO_MUX PERIPHS_IO_MUX_MTDO_U
#define PWM_1_OUT_IO_NUM 15
#define PWM_1_OUT_IO_FUNC FUNC_GPIO15

#define PWM_2_OUT_IO_MUX PERIPHS_IO_MUX_MTCK_U
#define PWM_2_OUT_IO_NUM 13
#define PWM_2_OUT_IO_FUNC FUNC_GPIO13

LOCAL os_timer_t blink_timer;

uint32 io_info[][3] = {   {PWM_0_OUT_IO_MUX,PWM_0_OUT_IO_FUNC,PWM_0_OUT_IO_NUM},
       	                  {PWM_1_OUT_IO_MUX,PWM_1_OUT_IO_FUNC,PWM_1_OUT_IO_NUM},
               	          {PWM_2_OUT_IO_MUX,PWM_2_OUT_IO_FUNC,PWM_2_OUT_IO_NUM},
			};

u32 duty[3] = {600,604,634};

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

void ICACHE_FLASH_ATTR spi_init()
{
    spi_test_init();
}

void spi_master_write(uint8_t *data, uint32_t len)
{
	SET_PERI_REG_MASK(SPI_PIN(SPI), SPI_CS_DIS);

	for(typeof(len)i=0; i<len; i++)
		spi_mast_byte_write(HSPI, data[i]);
	
	CLEAR_PERI_REG_MASK(SPI_PIN(SPI), SPI_CS_DIS);
}

int main() {
	int i = 0;
	nosdk8266_init();

	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_GPIO2);
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO5_U, FUNC_GPIO5);
	PIN_DIR_OUTPUT = _BV(2); //Enable GPIO2 light off.

	// timer example
	hw_timer_init(true);
    hw_timer_set_func(hw_test_timer_cb);
    hw_timer_arm(500000);

	// PWM example
	// pwm_init(1000, duty, 3, io_info);
	// pwm_start();

	// SPI master example
	spi_master_init(HSPI);
	spi_mast_byte_write(HSPI,0xAA);


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

