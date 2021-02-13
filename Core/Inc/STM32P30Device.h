/*
 * STM32P30Device.h
 *
 *  Created on: 2021年2月13日
 *      Author: DevoutPrayer
 */

#ifndef INC_STM32P30DEVICE_H_
#define INC_STM32P30DEVICE_H_
#include "P30Device.h"
#include "usart.h"

class STM32P30Device : public P30Device {
public:
	STM32P30Device(){};
	virtual ~STM32P30Device(){};

	size_t write(uint8_t* data, uint16_t length)
	{
			if(HAL_UART_Transmit(&huart2, data, length, 100))
				return 0;
			return length;
	}

	uint16_t readByte()
	{
			uint16_t byte = 0;
			if(HAL_UART_Receive(&huart2, (uint8_t*)&byte, 1, 5))
					return 256;
			return byte;
	}

	uint32_t get_tick()
	{
			return HAL_GetTick();
	}
};

#endif /* INC_STM32P30DEVICE_H_ */
