/////////////////////////////////////////////////////////////////////////////////////////		 
//B30 Depth sensor test routine
//Development board: searobotix STM32 test board
//Official  website: http://searobotix.com
//Revision     date: December 21, 2020
//Routine   version: v1.3
//Contact     email:info@searobotix.com
//Special statement: This program comes from the network, users bear all consequences!
/////////////////////////////////////////////////////////////////////////////////////////	
#include "MS5837.h"
#include "math.h"

#define MAX_FLAG 10

static uint32_t Cal_C[7];
static int32_t dT,TEMP;
static int64_t OFF_,SENS;
static uint32_t D1_Pres,D2_Temp;
static int32_t OFFi=0,SENSi=0,Ti=0;
static int64_t OFF2 = 0;
static int64_t SENS2 = 0;
static uint8_t MS5837_30BA_GetConversion(uint8_t command, uint32_t* conversion);
static uint8_t flag = 0;

double Temperature;
int32_t Pressure;
#include "usart.h"
#include "stdio.h"
extern void vTaskDelay(const uint32_t time);


/*******************************************************************************
  * @ function        name MS5837BA_RESET
  * @ function description reset ms5837
  * @ input  parameter none
  * @ output parameter none
  * @ return parameter 0 on success, others on failure
*******************************************************************************/
uint8_t MS5837_30BA_ReSet(void)
{
		uint8_t  data = MS5837_30BA_ResetCommand;
		if (HAL_OK != HAL_I2C_Master_Transmit(&hi2c2, MS5837_30BA_WriteCommand, &data, 1, 10))
		{
			printf("hi2c1 tx failed\n");
			return 1;
		}
		flag = 0;
		while(hi2c2.State != HAL_I2C_STATE_READY)
		{
			vTaskDelay(5);
			if(flag > MAX_FLAG)
				return 2;
			flag++;
		}

		return 0;
}
/*******************************************************************************
  * @ function        name MS5837_PROM
  * @ function description init 5837_30BA
  * @ input  parameter none
  * @ output parameter none
  * @ return parameter 0 on success, others on failure

  * @Global   variable Cal_ C array, used for CRC verification and calculation
  *                    of pressure value and temperature value
*******************************************************************************/
uint8_t MS5837_30BA_PROM(void)
{
		uint8_t inth,intl;
		uint8_t data[3];
		MS5837_30BA_ReSet();
		vTaskDelay(20);
		for(int i = 0; i < 7; i++)
		{
			data[0] = MS5837_30BA_PROM_RD + (i*2);

			if ( HAL_OK != HAL_I2C_Master_Transmit(&hi2c2, MS5837_30BA_WriteCommand, data, 1, 10) )
			{
				return 2*i+1;
			}
			flag = 0;
			while(hi2c2.State != HAL_I2C_STATE_READY)
			{
				vTaskDelay(5);
				if(flag > MAX_FLAG)
					return 1;
				flag++;
			}

			if ( HAL_OK != HAL_I2C_Master_Receive(&hi2c2, MS5837_30BA_ReadCommand, data, 2, 10) )
			{
				return 2*i+2;
			}
			flag = 0;
			while(hi2c2.State != HAL_I2C_STATE_READY)
			{
				vTaskDelay(5);
				if(flag > MAX_FLAG)
					return 1;
				flag++;
			}

			inth = data[0];
			intl = data[1];
			Cal_C[i] = (((uint16_t)inth << 8) | intl);
		}
		return 0;
}


/*******************************************************************************
  * Function Prototype: unsigned long MS5837BA_getConversion(void)
  * Function          : Read conversion result of 5837
*******************************************************************************/
uint8_t MS5837_30BA_GetConversion(uint8_t command,uint32_t* conversion)
{
			uint8_t  data[3];
			uint8_t  temp[3];
			data[0] = command;

			if(HAL_I2C_Master_Transmit(&hi2c2, MS5837_30BA_WriteCommand, data, 1, 10))
			{
				return 1;
			}
			flag = 0;
			do
			{
				vTaskDelay(1);
				if(flag > MAX_FLAG)
					return 1;
				flag++;
//				printf("assert1: %d\n", hi2c1.State);
			}while(hi2c2.State != HAL_I2C_STATE_READY);
			vTaskDelay(10);

			data[0] = 0;
			if(HAL_I2C_Master_Transmit(&hi2c2, MS5837_30BA_WriteCommand, data, 1, 10))
			{
				return 2;
			}
			flag = 0;
			do
			{
				vTaskDelay(1);
				if(flag > MAX_FLAG)
					return 2;
				flag++;
//				printf("assert2: %d\n", hi2c1.State);
			}while(hi2c2.State != HAL_I2C_STATE_READY);

			if (HAL_I2C_Master_Receive_IT(&hi2c2, MS5837_30BA_ReadCommand, data, 3))
			{
				return 3;
			}
			flag = 0;
			do
			{
				vTaskDelay(1);
				if(flag > MAX_FLAG)
					return 3;
				flag++;
//				printf("assert3: %d\n", hi2c1.State);
			}while(hi2c2.State != HAL_I2C_STATE_READY);

			temp[0] = data[0];
			temp[1] = data[1];
			temp[2] = data[2];
			*conversion = (unsigned long)temp[0] * 65536 + (unsigned long)temp[1] * 256 + (unsigned long)temp[2];
			return 0;
}


/*******************************************************************************
  * Function Prototype: void MS5837_30BA_GetData(void)
  * Function          : Obtain the temperature and pressure data and carry out
  *                     the second-order temperature compensation.
*******************************************************************************/

uint8_t MS5837_30BA_GetData(void)
{
		if(MS5837_30BA_GetConversion(MS5837_30BA_D2_OSR_8192, &D2_Temp))
			return 1;
		vTaskDelay(20);
		if(MS5837_30BA_GetConversion(MS5837_30BA_D1_OSR_8192, &D1_Pres))
			return 2;
//		vTaskDelay(20);
		dT=D2_Temp - (((uint32_t)Cal_C[5])*256l);
		SENS=(int64_t)Cal_C[1]*32768l+((int64_t)Cal_C[3]*dT)/256l;
		OFF_=(int64_t)Cal_C[2]*65536l+((int64_t)Cal_C[4]*dT)/128l;
		TEMP = 2000l+(int64_t)(dT)*Cal_C[6]/8388608LL;

		if(TEMP<2000)
		{
				Ti = (3*(int64_t)(dT)*(int64_t)(dT)/(8589934592LL));
				OFFi = (3*(TEMP-2000)*(TEMP-2000))/2;
				SENSi = (5*(TEMP-2000)*(TEMP-2000))/8;
		}
		else
		{
				Ti = 2*(dT*dT)/(137438953472LL);
				OFFi = (1*(TEMP-2000)*(TEMP-2000))/16;
				SENSi = 0;
		 }
		OFF2 = OFF_-OFFi;
		SENS2 = SENS-SENSi;
		Pressure=((D1_Pres*SENS2)/2097152l-OFF2)/8192l/10;
		Temperature=(TEMP-Ti)/100.0;
		return 0;
}

/*******************************************************************************
  * Function Prototype: unsigned char MS5837_30BA_Crc4()
  * Function          : Get Cal_ C array and run CRC verification
*******************************************************************************/

uint8_t MS5837_30BA_Crc4()
{
		int cnt;
		int t;
		unsigned int 	n_rem=0;
		unsigned char 	n_bit;
		unsigned char  	a=0;
		unsigned char  	b=0;
		unsigned short  int n_prom[8];

		for( t=0;t<7;t++)
		{
				n_prom[t]=Cal_C[t];
		}
		n_prom[0]=((n_prom[0]) & 0x0FFF);
		n_prom[7]=0;
		for (cnt = 0; cnt < 16; cnt++)
		{
			if(cnt%2==1)
					n_rem ^= (unsigned short) ((n_prom[cnt>>1]) & 0x00FF);
			else
					n_rem ^= (unsigned short) (n_prom[cnt>>1]>>8);
			for(n_bit = 8; n_bit > 0; n_bit--)
			{
					if(n_rem & (0x8000))
							n_rem = (n_rem << 1) ^ 0x3000;
					else
							n_rem = (n_rem << 1);
			}
		}
		n_rem = ((n_rem >> 12) & 0x000F);
		a=(n_rem ^ 0x00);
		b=Cal_C[0]>>12;
		if (a==b)
		{
				return 0;
		}
		else
				return 1;
}
