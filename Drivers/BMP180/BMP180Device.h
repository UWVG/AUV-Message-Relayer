#ifndef BMP180_DEVICE_H_
#define BMP180_DEVICE_H_
extern "C"{
#include "BMP180_driver/bmp180.h"
}
class BMP180Device{
public:
        BMP180Device(){};
        ~BMP180Device(){};
        /* \Brief: The function is used as I2C bus read
        *  \Return : Status of the I2C read
        *  \param dev_addr : The device address of the sensor
        *  \param reg_addr : Address of the first register, will data is going to be read
        *  \param reg_data : This data read from the sensor, which is hold in an array
        *  \param cnt : The no of byte of data to be read
        */
        virtual s8 BMP180_I2C_bus_read(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt)
        {
                return -1;
        }
        /* \Brief: The function is used as I2C bus write
        *  \Return : Status of the I2C write
        *  \param dev_addr : The device address of the sensor
        *  \param reg_addr : Address of the first register, will data is going to be written
        *  \param reg_data : It is a value hold in the array,
        *      will be used for write the value into the register
        *  \param cnt : The no of byte of data to be write
        */
        virtual s8 BMP180_I2C_bus_write(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt)
        {
                return -1;
        }
        /* \Brief : The delay routine
        *  \param : delay in ms
        */
        virtual void BMP180_delay_msek(u32 msek){};
};
#endif

