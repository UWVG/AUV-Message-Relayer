/*
 * ROSSerialDevice.h
 * Description:Platform related api was included in this file.
 *			   If you want to use api of P30.h,
 *			   you need to inherit a new class from this class and implement virtual function,
 *			   then pass an object through the constructor of P30 class.
 *  Created on: 2021年2月17日
 *      Author: DevoutPrayer
 */

#ifndef ROS_SERIAL_ROSSERIALDEVICE_H_
#define ROS_SERIAL_ROSSERIALDEVICE_H_

class ROSSerialDevice {
public:
	ROSSerialDevice(){};
	virtual ~ROSSerialDevice(){};
	/**
	 * @brief Write several bytes to P30 device
	 *
	 * @param data:The data buffer that we want to send
	 * @param length:The size of buffer in bytes
	 *
	 * @return bytes have been written
	 */
	virtual int init(){return 0;};
	/**
	 * @brief Write several bytes to P30 device
	 *
	 * @param data:The data buffer that we want to send
	 * @param length:The size of buffer in bytes
	 *
	 * @return bytes have been written
	 */
	virtual uint32_t time(){return 0;};
	/**
	 * @brief Write several bytes to P30 device
	 *
	 * @param data:The data buffer that we want to send
	 * @param length:The size of buffer in bytes
	 *
	 * @return bytes have been written
	 */
	virtual int read(){return -1;};
	/**
	 * @brief Write several bytes to P30 device
	 *
	 * @param data:The data buffer that we want to send
	 * @param length:The size of buffer in bytes
	 *
	 * @return bytes have been written
	 */
	virtual int write(uint8_t* data, uint16_t length){return 0;};
};

#endif /* ROS_SERIAL_ROSSERIALDEVICE_H_ */
