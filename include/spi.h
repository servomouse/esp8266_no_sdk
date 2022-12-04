#ifndef SPI_APP_H
#define SPI_APP_H

#include "spi_register.h"
#include "ets_sys.h"
#include "osapi.h"
// #include "spi_flash.h"

// #define SPI_SLAVE_DEBUG
//SPI protocol selection
#define TWO_INTR_LINE_PROTOCOL 		0
#define ONE_INTR_LINE_WITH_STATUS 	1

struct spi_slave_status_element
{
	uint8 wr_busy:1;
	uint8 rd_empty :1;
	uint8 comm_cnt :3;
	uint8 res :3;
};

union spi_slave_status
{
	struct spi_slave_status_element elm_value;
	uint8 byte_value;
};

#define SPI_FLASH_BYTES_LEN               24
#define IODATA_START_ADDR                 BIT0
#define SPI_BUFF_BYTE_NUM                 32

/**
 * @brief The SPI module working speed.
 * @attention Max speed 80MHz
 */
typedef enum {
    spispeed_0_5MHz   = 160,
    spispeed_1MHz     = 80,
    spispeed_2MHz     = 40,
    spispeed_5MHz     = 16,
    spispeed_8MHz     = 10,
    spispeed_10MHz    = 8,

} spi_speed_t;

/**
 * @brief The SPI module can work in either master or slave mode.
 *
 */
typedef enum {
    SpiMode_Master = 0,
    SpiMode_Slave  = 1,
} SpiMode;

/**
 *  @brief SPI sub mode
 *
 * Support 4 sub modes based on SPI clock polarity and phase.
 * SPI_CPOL SPI_CPHA  SubMode
 *   0        0        0
 *   0        1        1
 *   1        0        2
 *   1        1        3
 */
typedef enum {
    SpiSubMode_0 = 0,
    SpiSubMode_1 = 1,
    SpiSubMode_2 = 2,
    SpiSubMode_3 = 3,
} SpiSubMode;

/**
 * @brief The SPI mode working speed.
 *
 */
typedef enum {
    SpiBitOrder_MSBFirst = 0,
    SpiBitOrder_LSBFirst = 1,
} SpiBitOrder;

// @brief SPI interrupt soource defined.
typedef enum {
    SpiIntSrc_TransDone = SPI_TRANS_DONE,
    SpiIntSrc_WrStaDone = SPI_SLV_WR_STA_DONE,
    SpiIntSrc_RdStaDone = SPI_SLV_RD_STA_DONE,
    SpiIntSrc_WrBufDone = SPI_SLV_WR_BUF_DONE,
    SpiIntSrc_RdBufDone = SPI_SLV_RD_BUF_DONE,
} SpiIntSrc;

// @brief SPI CS pin.
typedef enum {
    SpiPinCS_0 = 1,
    SpiPinCS_1 = 2,
    SpiPinCS_2 = 4,
} SpiPinCS;

/*SPI number define*/
#define SPI 			0
#define HSPI			1

void cache_flush(void);
//spi master init funtion
void spi_master_init(uint8 spi_no, spi_speed_t speed);
void spi_init(uint8 spi_no, spi_speed_t speed, SpiSubMode submode, SpiBitOrder bit_order, SpiMode mode);


//lcd drive function
void spi_lcd_9bit_write(uint8 spi_no,uint8 high_bit,uint8 low_8bit);
//use spi send 8bit data
void spi_mast_byte_write(uint8 spi_no,uint8 data);

//transmit data to esp8266 slave buffer,which needs 16bit transmission ,
//first byte is master command 0x04, second byte is master data
void spi_byte_write_espslave(uint8 spi_no,uint8 data);
//read data from esp8266 slave buffer,which needs 16bit transmission ,
//first byte is master command 0x06, second byte is to read slave data
void spi_byte_read_espslave(uint8 spi_no,uint8 *data);

 //esp8266 slave mode initial
void spi_slave_init(uint8 spi_no);
  //esp8266 slave isr handle funtion,tiggered when any transmission is finished.
  
 // 2line interrupt slave protocal
void spi_slave_isr_handler(void *para); 
  // 1line interrupt with status register slave protocal
void spi_slave_isr_sta(void *para);

//hspi test function, used to test esp8266 spi slave
void hspi_master_readwrite_repeat(void);


void spi_test_init(void);

void spi_WR_espslave(uint8 spi_no);

void set_data();


#endif

