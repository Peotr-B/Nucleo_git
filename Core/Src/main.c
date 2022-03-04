/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  *26янв22
  * @Biriuk
  * peotr60@mail.ru
  *
  * Nucleo_git
  * Изучение микроконтроллера STM32 в среде STM32CubeIDE с помощью библиотеки HAL
  * с использованием отладочной платы NUCLEO-L452RE-P
  *
  * Это исходный код. От него будут отходить различные ветки, сохраняемые в
  * GitHub по адресу:
  * https://github.com/Peotr-B/Nucleo_git.git
  *
  * ВНИМАНИЕ!!!
  * После пересборки проекта портятся строки №220:
  * Sender1Handle = osThreadNew(StartSender, &(xStructsToSend[0]), &Sender1_attributes);
  * и №226:
  * Sender2Handle = osThreadNew(StartSender, &(xStructsToSend[1]), &Sender2_attributes);
  * Можно и так:
  * Sender1Handle = osThreadNew(StartSender, (void*) &xStructsToSend[0], &Sender1_attributes);
  * Sender2Handle = osThreadNew(StartSender, (void*) &(xStructsToSend[1]), &Sender2_attributes);
  *
  * Этот скетч основан на
  * "Пример 11. Блокировка при отправке в очередь / отправка структур через очередь"
  * в статье
  * "FreeRTOS: практическое применение, часть 2 (управление очередями) "
  * http://microsin.net/programming/arm/freertos-part2.html
  * А также использовал:
  * Лекция-практикум "SM32CubeIDE + FreeRTOS. Примеры задач, очереди, семафоров"
  * и прилагаемый проект на GIT:
  * https://www.youtube.com/redirect?event=video_description&redir_token=QUFFLUhqbDA0eVlLNmN5dlVnNzl1Qzh2RUo0NTZka3hyUXxBQ3Jtc0trcG51WGtHZlVfMlQ2ZDNPQTdpT1N5UzhLa0pWRlZOVjQ2RVdZZWNvbVJ3TG9rMGJWanRGcllfYy1mLW44bTQ0VmFRR2t5WGVrbjJnVHZsVjl1dGpQRkhudlVWcWRzSm84QWhMZmJZSlZMWnRHcWpFZw&q=https%3A%2F%2Fgithub.com%2FLeonidov%2FSTM32-Labs%2Ftree%2Fmaster%2FLab%25207%2520-%2520FreeRTOS
  * лекция №7
  * с учётом аналогичности функций xQueueSendToBack и osMessageQueuePut
  * (первая - от RTOS, вторая - от CMSIS)
  *
  * Ещё см. READMY.md
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h" //для функции strlen()
#include <stdio.h>
//#include "task.h"	//для работы с задачами
//#include "queue.h"	//для работы с очередями
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* Определение типа для структуры, которая будет передаваться через очередь. */
typedef struct
{
  unsigned char ucValue;
  unsigned char ucSource;
  //uint8_t ucSource;
} xData;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal3,
};
/* Definitions for Sender1 */
osThreadId_t Sender1Handle;
const osThreadAttr_t Sender1_attributes = {
  .name = "Sender1",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal2,
};
/* Definitions for Receiver */
osThreadId_t ReceiverHandle;
const osThreadAttr_t Receiver_attributes = {
  .name = "Receiver",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal1,
};
/* Definitions for Sender2 */
osThreadId_t Sender2Handle;
const osThreadAttr_t Sender2_attributes = {
  .name = "Sender2",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal2,
};
/* Definitions for QueueData */
osMessageQueueId_t QueueDataHandle;
const osMessageQueueAttr_t QueueData_attributes = {
  .name = "QueueData"
};
/* USER CODE BEGIN PV */

/* Определение двух переменных, имеющих тип xData. Эти переменные
   будут передаваться через очередь. */
//static const xData xStructsToSend[ 2 ] =
static xData xStructsToSend[ 2 ] =
{
		//{ 100, mainSENDER_1 }, /* Используется Sender1 (передатчик 1). */
		//{ 200, mainSENDER_2 }  /* Используется Sender2 (передатчик 2). */

		{ 100, 1 }, /* Используется Sender1 (передатчик 1). */
		{ 200, 2 }  /* Используется Sender2 (передатчик 2). */
};

char trans_str[64] = {0,};
int LED_State = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
void StartDefaultTask(void *argument);
void StartSender(void *argument);
void StartReceiver(void *argument);

/* USER CODE BEGIN PFP */

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
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of QueueData */
  QueueDataHandle = osMessageQueueNew (3, sizeof(xData), &QueueData_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of Sender1 */
  Sender1Handle = osThreadNew(StartSender, (void*) &xStructsToSend[0], &Sender1_attributes);

  /* creation of Receiver */
  ReceiverHandle = osThreadNew(StartReceiver, NULL, &Receiver_attributes);

  /* creation of Sender2 */
  Sender2Handle = osThreadNew(StartSender, (void*) &xStructsToSend[1], &Sender2_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, SMPS_EN_Pin|SMPS_V1_Pin|SMPS_SW_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : SMPS_EN_Pin SMPS_V1_Pin SMPS_SW_Pin */
  GPIO_InitStruct.Pin = SMPS_EN_Pin|SMPS_V1_Pin|SMPS_SW_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : SMPS_PG_Pin */
  GPIO_InitStruct.Pin = SMPS_PG_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(SMPS_PG_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD4_Pin */
  GPIO_InitStruct.Pin = LD4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD4_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
//это стандартная запись для посимвольного вывода информации в интерфейс ITM в среде STM32CubeIDE
//(должно быть в случае применения SWO)
//https://www.youtube.com/watch?v=nE-YrKpWjso&list=PL9lkEHy8EJU8a_bqiJXwGTo-uM_cPa98P
int __io_putchar(int ch)
{
	ITM_SendChar(ch);
	return ch;
}

//или:

//STM32: Отладка через SWO в STM32CubeIDE с доработкой ST-LINK
//https://www.youtube.com/watch?v=ST_fUu6ACzE

//int _write(int file, char *ptr, int len)
 //{
 //int i = 0;
 //for(i = 0; i < len; i++)
    //ITM_SendChar((*ptr++));
 //return len;
 //}

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
	  HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
	  LED_State = HAL_GPIO_ReadPin(LD4_GPIO_Port, LD4_Pin);
	  puts("LED_State меняет своё значение");
	  printf("LED_State = %d.\n", LED_State);
    osDelay(500);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartSender */
/**
* @brief Function implementing the Sender1 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartSender */
void StartSender(void *argument)
{
  /* USER CODE BEGIN StartSender */
  /* Infinite loop */
  for(;;)
  {
	  
	osMessageQueuePut(QueueDataHandle, argument, 0, osWaitForever);
	
    //osDelay(1);		//Нужно ли при наличии функции osThreadYield? Видел без этого
	
	osThreadYield();
	//Проверить использование этой функции, см.:
	//https://arm-software.github.io/CMSIS_5/RTOS2/html/group__CMSIS__RTOS__Message.html
	//https://microtechnics.ru/stm32-uchebnyj-kurs-freertos-chast-3/
	//https://russianblogs.com/article/88161192459/
  }
  /* USER CODE END StartSender */
}

/* USER CODE BEGIN Header_StartReceiver */
/**
* @brief Function implementing the Receiver thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartReceiver */
void StartReceiver(void *argument)
{
  /* USER CODE BEGIN StartReceiver */
  xData xReceivedStructure;
  osStatus_t status;
  /* Infinite loop */
  for(;;)
  {
	  status = osMessageQueueGet(QueueDataHandle, &xReceivedStructure, NULL, osWaitForever);
	  
	  if (status == osOK) 
		/* Данные были успешно приняты из очереди, вывод принятого значения
           и источника этого значения. */
	  {
		  //if( xReceivedStructure.ucSource == mainSENDER_1 )
		  if( xReceivedStructure.ucSource == 1 )
         {
			//HAL_UART_Transmit(&huart2, (uint8_t*)msg.Buf, strlen(msg.Buf), osWaitForever);
			//HAL_UART_Transmit(&huart2, "From Sender 1 = ", (uint8_t*)xReceivedStructure.ucValue, strlen(xReceivedStructure.ucValue), osWaitForever);
            //vPrintStringAndNumber( "From Sender 1 = ", xReceivedStructure.ucValue );
			
			snprintf(trans_str, 63, "UART: From Sender 1 = %c \n", (unsigned char)xReceivedStructure.ucValue);
			HAL_UART_Transmit(&huart2, (unsigned char*) trans_str,
			strlen(trans_str), osWaitForever);
			
			printf("printf: From Sender 1 = %u.\n", (unsigned char)xReceivedStructure.ucValue);
			puts("puts: Очередь работает правильно!");
         }
         else
         {
			//HAL_UART_Transmit(&huart2, "From Sender 2 = ", (uint8_t*)xReceivedStructure.ucValue, strlen(xReceivedStructure.ucValue), osWaitForever);
            //vPrintStringAndNumber( "From Sender 2 = ", xReceivedStructure.ucValue );
			
			snprintf(trans_str, 63, "UART: From Sender 2 = %u \n", (uint8_t)xReceivedStructure.ucValue);
			HAL_UART_Transmit(&huart2, (unsigned char*) trans_str,
			strlen(trans_str), osWaitForever);
			
			printf("printf: From Sender 2 = %u.\n", (uint8_t)xReceivedStructure.ucValue);
			puts("puts: Очередь работает правильно!");
         }
	  }
	  
    //osDelay(1);		//Нужно ли при наличии функции osThreadYield? Видел без этого
	osThreadYield();
  }
  /* USER CODE END StartReceiver */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM2 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM2) {
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

