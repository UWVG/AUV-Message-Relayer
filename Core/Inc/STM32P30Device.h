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

#define P30_RX_BUFFER_SIZE 256

class STM32P30Device : public P30Device {
public:
	STM32P30Device(){};
	virtual ~STM32P30Device(){};


	static void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
	{

			*(rx_buffer + write_index) = byte;
			write_index = (write_index + 1)%P30_RX_BUFFER_SIZE;
			count ++;
//			if (HAL_OK != HAL_UART_Receive_IT(&huart3, &byte, 1))
//			{
//				HAL_UART_Transmit(&huart1, (uint8_t *)"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 1, 100);
//			}
//			HAL_UART_Transmit(&huart1, &byte, 1, 10);
	}

	uint8_t init()
	{
		huart3.RxCpltCallback = HAL_UART_RxCpltCallback;
		HAL_UART_Receive_DMA(&huart3, &byte, 1);
		return 0;
	}

	size_t write(uint8_t* data, uint16_t length)
	{
			if(HAL_UART_Transmit(&huart3, data, length, 100))
				return 0;
			return length;
	}

	uint16_t readByte()
	{
		if(count > 0)
		{
			read_index = (read_index + 1)%P30_RX_BUFFER_SIZE;
			__disable_irq();
			count --;
			__enable_irq();
//			HAL_UART_Transmit(&huart1, rx_buffer + read_index, 1, 100);
			return *(rx_buffer + read_index);
		}
		else
		{
//			HAL_UART_Transmit(&huart1, (uint8_t *)&count, 2, 100);
//			HAL_UART_Transmit(&huart1, (uint8_t *)&count, 2, 100);
		}
//		HAL_UART_Transmit(&huart1, (uint8_t*)"N", 1, 100);
		return 256;
	}

	uint32_t get_tick()
	{
			return HAL_GetTick();
	}
	static uint8_t rx_buffer[P30_RX_BUFFER_SIZE];
	static uint8_t read_index;
	static uint8_t write_index ;
	static int16_t count ;
	static uint8_t byte;
};

uint8_t STM32P30Device::rx_buffer[P30_RX_BUFFER_SIZE] = {0};
uint8_t STM32P30Device::read_index = P30_RX_BUFFER_SIZE - 1;
uint8_t STM32P30Device::write_index = 0;
int16_t STM32P30Device::count = 0;
uint8_t STM32P30Device::byte = 0;

#endif /* INC_STM32P30DEVICE_H_ */
