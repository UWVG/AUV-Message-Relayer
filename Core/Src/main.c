/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "dma.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "P30.h"
#include "STM32P30Device.h"
#include "ROSApi.h"
#include "std_msgs/UInt32.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
ros::NodeHandle nh;
std_msgs::UInt32 var;
std_msgs::UInt32 var1;
std_msgs::UInt32 var2;
ros::Publisher b30Publisher("B30",&var);
ros::Publisher p30Publisher("P30",&var1);
ros::Publisher t30Publisher("T30",&var2);
TaskHandle_t xHandle = NULL;
TaskHandle_t xHandle1 = NULL;
TaskHandle_t xHandle2 = NULL;
TaskHandle_t xHandle3 = NULL;
double 	b30_temperture;
int32_t b30_pressure;
STM32P30Device stm32P30Device;
P30 p30( &stm32P30Device );
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

extern "C"{
#include "B30.h"
void MX_FREERTOS_Init(void);
}
void StartROSSerialTask(void const * argument);
void StartB30Task(void const * argument);
void StartP30Task(void const * argument);
void StartT30Task(void const * argument);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  nh.initNode();
  nh.advertise(b30Publisher);
  nh.advertise(p30Publisher);
  nh.advertise(t30Publisher);
  printf("SYS RESTART!\n");
//  HAL_UART_Transmit(&huart1, (uint8_t*)"678910", 6, 5);
  xTaskCreate(	(TaskFunction_t)StartROSSerialTask,
				"ROSSerial",	/*lint !e971 Unqualified char types are allowed for strings and single characters only. */
				100,
				NULL,
				5,
				&xHandle);
  xTaskCreate(	(TaskFunction_t)StartB30Task,
  				"B30",	/*lint !e971 Unqualified char types are allowed for strings and single characters only. */
  				300,
  				NULL,
  				3,
  				&xHandle1);
  xTaskCreate(	(TaskFunction_t)StartP30Task,
				"P30",	/*lint !e971 Unqualified char types are allowed for strings and single characters only. */
				200,
				NULL,
				3,
				&xHandle2);
  xTaskCreate(	(TaskFunction_t)StartT30Task,
 				"T30",	/*lint !e971 Unqualified char types are allowed for strings and single characters only. */
 				200,
 				NULL,
 				4,
 				&xHandle3);
//  if(!xHandle)
//  {
//	  HAL_UART_Transmit(&huart1, (uint8_t*)"000000", 6, 5);
//  }
//  HAL_UART_Transmit(&huart1, (uint8_t*)"111111", 6, 5);
  /* USER CODE END 2 */

  /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init();
  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */


	 nh.spinOnce();

//	  while(!p30.initialize(15))
//	  {
//		  HAL_Delay(2000);
//	  }
//	  //p30.request(PingEnumNamespace::PingMessageId::PING1D_PING_INTERVAL, 100);
//	  //bool flag = p30.setPingInterval(50,1);
////	  bool flag = p30.setPingEnable();
////	  if(flag)
////		  HAL_UART_Transmit(&huart1, (uint8_t*)"CC", 3, 100);
//	  while(1)
//	  {
//
//		  	  p30.waitMessage(PingEnumNamespace::PingMessageId::PING1D_MODE_AUTO, 100);
//	  }
	  //p30.update();
	  //int t=HAL_GetTick();
	  HAL_Delay(1000);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void StartROSSerialTask(void const * argument)
{
	for(;;)
	{
		nh.spinOnce();
//		HAL_UART_Transmit(&huart1, (uint8_t*)"11111", 5, 5);
		vTaskDelay(5000);
	}
}

void StartB30Task(void const * argument)
{
	uint8_t ret;
	for(;;)
	{
		printf("B30 RESET!!!!\n");
		while((ret = B30_init()))
		{
			printf("hi2c1.state: %d\n", hi2c1.State);
			printf("B30_init ret: %d\n", ret);
			MX_I2C1_Init();
			vTaskDelay(1000);
		}
		while(!B30_GetData(&b30_pressure, &b30_temperture))
		{
			var.data = b30_pressure;

			vTaskSuspendAll();
			b30Publisher.publish(&var);
			if(!xTaskResumeAll())
				taskYIELD();

			vTaskDelay(20);
		}
	}
}
void StartP30Task(void const * argument)
{
	ping_message *msg;
	for(;;)
	{
		while(p30.initialize(50))
		{

		}
//		stm32P30Device.init();
		while(1)
		{
//			var2.data = HAL_GetTick();
//						vTaskSuspendAll();
//						t30Publisher.publish(&var2);
//		//				printf("current time:%d",var2.data);
//						if(!xTaskResumeAll())
//							taskYIELD();
//
//						vTaskDelay(20);
			msg=p30.read();
			if(msg)
			{
				p30.handleMessage(msg);
				var1.data = p30._distance;
				vTaskSuspendAll();
				p30Publisher.publish(&var1);
				if(!xTaskResumeAll())
					taskYIELD();
//				vTaskDelay(10);
//				HAL_UART_Transmit(&huart1, (uint8_t *)"000000000000000", 15, 100);
			}
			else
//				HAL_UART_Transmit(&huart1, (uint8_t *)"111111111111111", 15, 100);
				vTaskDelay(50);
		}
	}
}
void StartT30Task(void const * argument)
{
	int flag = 0;
	int time = 0;
	int dtime = 0;
	for(;;)
	{
		dtime += HAL_GetTick() - time;
//		printf("dtime %d\n", dtime);
		time   = HAL_GetTick();
		if (dtime >= 1000)
		{
			var2.data = flag;
			vTaskSuspendAll();
			t30Publisher.publish(&var2);
			if(!xTaskResumeAll())
				taskYIELD();
//			printf("--------------------percent: %d\n", flag);
			flag = 0;
			dtime = 0;
		}
		else
		{
			flag ++;
		}
		vTaskDelay(10);
	}
}
/* USER CODE END 4 */

 /**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
	  HAL_UART_Transmit(&huart1, (uint8_t *)"Error_Handler\n", 14, 100);
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
