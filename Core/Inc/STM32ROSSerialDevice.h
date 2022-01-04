/*
 * STM32ROSSerialDevice.h
 *
 *  Created on: Feb 17, 2021
 *      Author: DevoutPrayer
 */

#ifndef INC_STM32ROSSERIALDEVICE_H_
#define INC_STM32ROSSERIALDEVICE_H_

#include "ROSSerialDevice.h"
#include "usart.h"

#define ROS_RX_BUFFER_SIZE 128

class STM32ROSSerialDevice : public ROSSerialDevice {

public:
	STM32ROSSerialDevice(){};
	virtual ~STM32ROSSerialDevice(){};

	static void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
	{
			*(rx_buffer + write_index) = byte;
			write_index = (write_index + 1)%ROS_RX_BUFFER_SIZE;
			count ++;
	}
	/**
	 * @brief Initlalization
	 *
	 * @return o on success
	 */
	virtual int init()
	{
		huart7.RxCpltCallback = HAL_UART_RxCpltCallback;
		HAL_UART_Receive_DMA(&huart7, &byte, 1);
		return 0;
	}
	/**
	 * @brief Get time in ms
	 *
	 * @return time in ms
	 */
	virtual uint32_t time()
	{
		return HAL_GetTick();
	};
	/**
	 * @brief Read one byte from peer agent
	 *
	 * @return 0~255 on success, -1 on failuare
	 */
	virtual int read()
	{
		if(count > 0)
		{
			read_index = (read_index + 1)%ROS_RX_BUFFER_SIZE;
			__disable_irq();
			count --;
			__enable_irq();
			return *(rx_buffer + read_index);
		}
		return -1;
	}
	/**
	 * @brief Write several bytes to peer agent
	 *
	 * @param data:The data buffer that we want to send
	 * @param length:The size of buffer in bytes
	 *
	 * @return bytes have been written
	 */
	virtual int write(uint8_t* data, uint16_t length)
	{
		if(HAL_UART_Transmit(&huart7, data, length, 100))
			return 0;
		return length;
	}
	static uint8_t rx_buffer[ROS_RX_BUFFER_SIZE];
	static uint8_t read_index;
	static uint8_t write_index ;
	static int16_t count ;
	static uint8_t byte;

};

uint8_t STM32ROSSerialDevice::rx_buffer[ROS_RX_BUFFER_SIZE] = {0};
uint8_t STM32ROSSerialDevice::read_index = ROS_RX_BUFFER_SIZE - 1;
uint8_t STM32ROSSerialDevice::write_index = 0;
int16_t STM32ROSSerialDevice::count = 0;
uint8_t STM32ROSSerialDevice::byte = 0;

#endif /* INC_STM32ROSSERIALDEVICE_H_ */
