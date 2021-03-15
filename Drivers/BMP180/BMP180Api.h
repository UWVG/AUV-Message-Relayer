#ifndef BMP180_API_H_
#define BMP180_API_H_

extern "C"{
#include "BMP180_driver/bmp180.h"
}
#include "BMP180Device.h"
static s8 BMP180_I2C_bus_write_Wrapper(u8, u8, u8*, u8);
static s8 BMP180_I2C_bus_read_Wrapper(u8, u8, u8*, u8);
static void BMP180_delay_msek_Wrapper(u32);
typedef struct BMP180Result_{
		s32 pressure;
		s16 temperature;
}BMP180Result;
class BMP180Api{
public:
        BMP180Api(BMP180Device* bmp180Device_, u8 dev_addr)
        {
				bmp180Device = bmp180Device_;
				bmp180.bus_write  = BMP180_I2C_bus_write_Wrapper;
                bmp180.bus_write  = BMP180_I2C_bus_write_Wrapper;
                bmp180.bus_read   = BMP180_I2C_bus_read_Wrapper;
                bmp180.delay_msec = BMP180_delay_msek_Wrapper;
                bmp180.dev_addr   = dev_addr;
        };
        ~BMP180Api(){};
        int init()
        {
        	return bmp180_init(&bmp180);
        }
        BMP180Result GetTemAndPre()
        {
        	uncom_temperature = bmp180_get_uncomp_temperature();
        	uncom_pressure = bmp180_get_uncomp_pressure();
        	bmp180Result.temperature = bmp180_get_temperature(uncom_temperature);
        	bmp180Result.pressure = bmp180_get_pressure(uncom_pressure);
        	return bmp180Result;
        }
        static BMP180Device *bmp180Device;
private:
        struct bmp180_t bmp180;
        BMP180Result bmp180Result;
        u32 uncom_pressure;
        u16 uncom_temperature;
};
BMP180Device* BMP180Api::bmp180Device = nullptr;
s8 BMP180_I2C_bus_write_Wrapper(u8 arg1, u8 arg2, u8* arg3, u8 arg4)
{
	return BMP180Api::bmp180Device->\
		   BMP180_I2C_bus_write(arg1, arg2, arg3, arg4);
}
s8 BMP180_I2C_bus_read_Wrapper(u8 arg1, u8 arg2, u8* arg3, u8 arg4)
{
	return BMP180Api::bmp180Device->\
		   BMP180_I2C_bus_read(arg1, arg2, arg3, arg4);
}
void BMP180_delay_msek_Wrapper(u32 arg)
{
	BMP180Api::bmp180Device->\
	BMP180_delay_msek(arg);
}
#endif
