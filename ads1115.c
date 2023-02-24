#include "ADS1115.h"
	
	
	
	//declare global variable
	
	////set 16-bit word 'content' of the configuratoin register in according to CONFIG_REG_1115 structure
	unsigned short formingConfigReg(struct CONFIG_REG_1115 *x){
		unsigned static short tm= 0;
		tm = (x->OS << 15) | (x->MUX << 12)|(x->PGA << 9) | (x->MODE << 8)|(x->DR << 5)|(x->COMP_MODE << 4)|(x->COMP_POL << 3)|(x->COMP_LAT << 2)|(x->COMP_QUE);
		return tm;
	}
	
	///primitives
	char ADS1115_Write(uint8_t Addr, uint8_t AddrPointRegister, uint16_t *DataFromRead){
            //    static unsigned char stVar[3] = { AddrPointRegister,(uint8_t)(DataFromRead[0] >> 8),(uint8_t)(DataFromRead[0] & 0x00ff) };
          static unsigned char stVar[3] = {0};
          /****the first byte - address of device shifted on left << 1
          the second - means a content of internal register of ADS1115*/  
           stVar[0]= AddrPointRegister;
           /****hte thrid byte - bits:15-8***/
           stVar[1]= (uint8_t)(DataFromRead[0] >> 8);
           /****the forth byte - bits:7-0***/ 
           stVar[2]= (uint8_t)(DataFromRead[0] & 0x00ff);
           /***START TRANSMISSION***/
                 return  (HAL_I2C_Master_Transmit(&hi2c2, Addr<<1, stVar, 3, 20) == HAL_OK) ? 0 : -1; 
                
	}
	
	char ADS1115_Read(uint8_t Addr, uint8_t AddrPointRegister, uint16_t *DataIntoWrite){
          unsigned static char arrayVar[2]={0};
          arrayVar[0] = AddrPointRegister;
                if(HAL_I2C_Master_Transmit(&hi2c2 ,Addr<<1,arrayVar,1,20) != HAL_OK)
                  return -1;
                if(HAL_I2C_Master_Receive(&hi2c2, ((Addr<<1)|0x01),arrayVar, 2, 20) != HAL_OK)
                  return -1;
                DataIntoWrite[0] =((uint16_t*)arrayVar)[0]; 
                return 0;
	}
        
        /////INITIALIZATION for the single shoot mode
        
        char ADS1115_SingleShootADCInit(uint8_t Addr,uint8_t channel,uint8_t Gain,uint8_t SamplingSpeed,struct CONFIG_REG_1115 *conf){
          static unsigned short toAdc=0;
          conf->MUX = channel;
          conf->PGA = Gain;
          conf->MODE = 1;
          conf->DR =  SamplingSpeed;
          toAdc = formingConfigReg(conf);
         //toAds = (0 << 15) | (channel << 12)|(Gain << 9) | (1 << 8)|(SamplingSpeed << 5)|(0 << 4)|(0 << 3)|(0 << 2)|(0);
         return (ADS1115_Write(ADS1115_ADDR, ADS1115_CONFIG_REG,&toAdc) == HAL_OK) ? 0 : -1;
        }
        
        char ADS1115_ContinuousADCInit(uint8_t Addr,uint8_t channel,uint8_t Gain,uint8_t SamplingSpeed,struct CONFIG_REG_1115 *conf){
          static unsigned short toAdc=0;
          conf->MUX = channel;
          conf->PGA = Gain;
          conf->MODE = 0;
          conf->DR =  SamplingSpeed;
          toAdc = formingConfigReg(conf);
         //toAds = (0 << 15) | (channel << 12)|(Gain << 9) | (1 << 8)|(SamplingSpeed << 5)|(0 << 4)|(0 << 3)|(0 << 2)|(0);
         return (ADS1115_Write(ADS1115_ADDR, ADS1115_CONFIG_REG,&toAdc) == HAL_OK) ? 0 : -1;
        }


        //start ADC after initialization - the data get from the CONFIG_REG_1115 structure
        char ADS1115_SingleShootADCStart(uint8_t Addr,struct CONFIG_REG_1115 *conf){
           static unsigned short toAdc=0;
           toAdc = formingConfigReg(conf);
           toAdc |= (1 << 15);
           return (ADS1115_Write(ADS1115_ADDR, ADS1115_CONFIG_REG,&toAdc) == HAL_OK) ? 0 : -1;
        }
        //checking if the convertion has done
  char ADS1115_IsFinished(uint8_t adr){
        static unsigned short toAdc=0;
        if (ADS1115_Read(adr,ADS1115_CONFIG_REG, &toAdc) != HAL_OK)
          return -1;
        return (toAdc & (1 << 15)) ?  1 : 0;
  }
      //read convertion data from the conversion register into a `res` 16-bit variable
char ADS1115_ReadResult(uint8_t adr,  short *res){
  static uint16_t read = 0; 
  if (ADS1115_Read(adr,ADS1115_CONVERSION_REG , &read) != HAL_OK){
          return -1;
  }
  res[0] = ((read << 8) | (read >> 8) );
  return 0;
}
        
        
