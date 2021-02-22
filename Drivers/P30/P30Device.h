/*
 * P30Device.h
 *	Description:Platform related api was included in this file.
 *				If you want to use api of P30.h,
 *				you need to inherit a new class from this class and implement virtual function,
 *				then pass an object through the constructor of P30 class.
 *  Created on: 2021年2月13日
 *      Author: DevoutPrayer
 */

#ifndef P30_P30DEVICE_H_
#define P30_P30DEVICE_H_

class P30Device {
public:
	P30Device(){};
	virtual ~P30Device(){};
	/**
	 * @brief Write several bytes to P30 device
	 *
	 * @param data:The data buffer that we want to send
	 * @param length:The size of buffer in bytes
	 *
	 * @return bytes have been written
	 */
	virtual size_t write(uint8_t* data, uint16_t length){return 0;};
	/**
	 * @brief Read one byte from P30 device
	 *
	 * @return <=255 read on success
	 * @return >255	 read on failuare
	 */
	virtual uint16_t readByte(){return 256;};

	virtual uint32_t get_tick(){return 0;};

	virtual uint8_t init(){return 1;};
};

#endif /* P30_P30DEVICE_H_ */
