/*
 * B30.h
 *
 *  Created on: 2021年2月20日
 *      Author: DevoutPrayer
 */

#ifndef B30_B30_H_
#define B30_B30_H_
#include "MS5837/MS5837.h"

uint8_t B30_init()
{
		if(MS5837_30BA_ReSet())
			return 1;
		HAL_Delay(20);
		if(MS5837_30BA_PROM())
			return 2;
		HAL_Delay(20);
		if(MS5837_30BA_Crc4())
			return 3;
		return 0;
}

uint8_t B30_GetData(int32_t* PRESSURE,double* TEMPETURE)
{
	uint8_t ret;
	ret = MS5837_30BA_GetData();
//	HAL_UART_Transmit(&huart1, (uint8_t *)&ret, 1, 100);
		if(ret)
			return 1;
		*TEMPETURE = Temperature;
		*PRESSURE = Pressure;
		return 0;
}



#endif /* B30_B30_H_ */
