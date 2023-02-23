#include "main.h"

/********
 external variables */
extern I2C_HandleTypeDef hi2c2;
extern struct CONFIG_REG_1115 myInitStruct;
///the first and the second bits of adress are define hardware - by ADDR pin of IC
#include <stdio.h>
#include "stm32f1xx_hal.h"
///adress point register defintions
#define ADS1115_CONVERSION_REG 0
#define ADS1115_CONFIG_REG 1
#define ADS1115_LO_THRESH_REG 2
#define ADS1115_HI_THRESH_REG 3
#define ADS1115_ADDR 0x48  
/////input channels definition
#define ADS1115_AIN0POS_AIN1NEG 0x00
#define ADS1115_AIN0POS_AIN3NEG 0x01
#define ADS1115_AIN1POS_AIN3NEG 0x02
#define ADS1115_AIN2POS_AIN3NEG 0x03
#define ADS1115_AIN0POS_GND 0x04
#define ADS1115_AIN1POS_GND 0x05
#define ADS1115_AIN2POS_GND 0x06
#define ADS1115_AIN3POS_GND 0x07
//samplingSpeed in samples per second
#define ADS1115_SPS_8 0x00
#define ADS1115_SPS_16 0x01
#define ADS1115_SPS_32 0x02
#define ADS1115_SPS_64 0x03
#define ADS1115_SPS_128 0x04
#define ADS1115_SPS_250 0x05
#define ADS1115_SPS_475 0x06
#define ADS1115_SPS_860 0x07
//GAIN (reference voltage in millivolts)
#define ADS1115_FSR_6144 0x00
#define ADS1115_FSR_4096 0x01
#define ADS1115_FSR_2048 0x02
#define ADS1115_FSR_1024 0x03
#define ADS1115_FSR_512 0x04
#define ADS1115_FSR_256 0x05




/*********I N I T structure like in HAL stm32 library */
struct CONFIG_REG_1115{
	//Operational status or single-shot conversion start
	unsigned OS : 1;
	//Input multiplexer configuration (ADS1115 only)
	unsigned MUX : 3;
	//Programmable gain amplifier configuration
	unsigned PGA : 3;
	//Device operating mode
	unsigned MODE : 1;
	//Data rate
	unsigned DR : 3;
	//Comparator mode (ADS1114 and ADS1115 only)
	unsigned COMP_MODE : 1;
	//Comparator polarity (ADS1114 and ADS1115 only)
	unsigned COMP_POL : 1;
	//Latching comparator (ADS1114 and ADS1115 only)
	unsigned COMP_LAT : 1;
	//Comparator queue and disable (ADS1114 and ADS1115 only)
	unsigned COMP_QUE : 2;
	};

unsigned short formingConfigReg(struct CONFIG_REG_1115 *x);
char ADS1115_Write(uint8_t Addr, uint8_t AddrPointRegister, uint16_t *DataFromSend);
char ADS1115_Read(uint8_t Addr, uint8_t AddrPointRegister, uint16_t *DataIntoRead);
char ADS1115_SingleShootADCInit(uint8_t Addr,uint8_t channel,uint8_t Gain,uint8_t SamplingSpeed,struct CONFIG_REG_1115 *conf);
 char ADS1115_ContinuousADCStart(uint8_t Addr,uint8_t channel,uint8_t Gain,uint8_t SamplingSpeed,struct CONFIG_REG_1115 *conf);
 char ADS1115_SingleShootADCStart(uint8_t Addr,struct CONFIG_REG_1115 *conf);
  char ADS1115_IsFinished(uint8_t adr);
  char ADS1115_ReadResult(uint8_t adr,  short *res);