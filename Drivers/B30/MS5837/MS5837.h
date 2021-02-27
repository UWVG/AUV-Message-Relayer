/////////////////////////////////////////////////////////////////////////////////////////		 
//B30 Depth sensor test routine
//Development board: searobotix STM32 test board
//Official  website: http://searobotix.com
//Revision     date: December 21, 2020
//Routine   version: v1.3
//Contact     email:info@searobotix.com
//Special statement: This program comes from the network, users bear all consequences!
/////////////////////////////////////////////////////////////////////////////////////////
#ifndef __MS5837_H_
#define __MS5837_H_

#include "i2c.h"
#include "stm32h7xx_hal.h"




#define MS5837_30BA_WriteCommand		0xEC
#define MS5837_30BA_ReadCommand			0xED

#define MS5837_30BA_ResetCommand		0x1E
#define	MS5837_30BA_PROM_RD				0xA0
#define MS5837_30BA_ADC_RD				0x00

#define MS5837_30BA_OSR256				0x40
#define MS5837_30BA_OSR512				0x42
#define MS5837_30BA_OSR1024				0x44
#define MS5837_30BA_OSR2048				0x46
#define MS5837_30BA_OSR4096				0x48
#define	MS5837_30BA_D2_OSR_8192			0x58
#define	MS5837_30BA_D1_OSR_8192			0x48

uint8_t	MS5837_30BA_PROM(void);
uint8_t	MS5837_30BA_GetData(void);
uint8_t	MS5837_30BA_ReSet(void);
uint8_t	MS5837_30BA_Crc4(void);

extern double Temperature;
extern int32_t Pressure;

#endif

