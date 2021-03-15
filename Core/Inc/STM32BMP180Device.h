/*
 * STM32BMP180Device.h
 *
 *  Created on: Mar 15, 2021
 *      Author: DevoutPrayer
 */

#ifndef INC_STM32BMP180DEVICE_H_
#define INC_STM32BMP180DEVICE_H_
#include "BMP180Device.h"
#include "i2c.h"


class STM32BMP180Device : public BMP180Device {
public:
	STM32BMP180Device(){};
	~STM32BMP180Device(){};
	s8 BMP180_I2C_bus_read(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt)
	{
			ret1 = HAL_I2C_Mem_Read_IT(&hi2c2, dev_addr|0x01, reg_addr, 1, reg_data, cnt);
			if(ret1)
				return ret1;
			while(hi2c2.State != HAL_I2C_STATE_READY)
			{
				BMP180_delay_msek(10);
			}
			return 0;
	}
	s8 BMP180_I2C_bus_write(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt)
	{

		    ret2 = HAL_I2C_Mem_Write_IT(&hi2c2, dev_addr&0xfe, reg_addr, 1, reg_data, cnt);
		    if(ret2)
		    	return ret2;
		    while(hi2c2.State != HAL_I2C_STATE_READY)
		    {
		    	BMP180_delay_msek(10);
		    }
		    return 0;
	}
    void BMP180_delay_msek(u32 msek)
    {
    		vTaskDelay(msek);
    }
private:
    int ret1;
    int ret2;
};

#endif /* INC_STM32BMP180DEVICE_H_ */
