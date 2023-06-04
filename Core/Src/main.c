/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * 4май23
 * Проверка работоспособности перед возобновлением работы
 * Пришлось обновлять Github Desctop
 * 26апр22
 * @Biriuk
 * peotr60@mail.ru
 *
 * Nucleo_git
 * Изучение микроконтроллера STM32 в среде STM32CubeIDE с помощью библиотеки HAL
 * с использованием отладочной платы NUCLEO-L452RE-P в части применения прерываний
 * совместно с RTOS-CMSIS
 *
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * ВНИМАНИЕ!
 * При использовании RTC в библиотеке HAL имеется серьёзный баг (время стоит), который можно
 * обойти следующим образом:
 * В файле stm32l4xx_hal_rtc.c (Drivers / Src / stm32l4xx_hal_rtc.c) закомментировать строку:
 *
 * 1560	// sTime->SubSeconds = (uint32_t)(hrtc->Instance->SSR);
 *
 * При этом следует иметь в виду, что каждый раз после пересборки проекта придётся каждый раз
 * закоментировать эту строку вручную.
 * Подробнее см. README.md
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 *
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * ВНИМАНИЕ! В проекте Nucleo_git на ветке изучения прерываний совместно с RTOS-CMSIS обнаружил
 * проблему, решить которую не мог несколько дней. Не было вывода информации на ПК по каналу SWO.
 * Сравнение с действующим проектом RTOS_ETAPY показало отсутствие файлов syscalls.c и sysmem.c
 * в Core / Src. Тупо взял эти файлы из проекта RTOS_ETAPY, и вставил в
 * D:\STM32\STM32_File\Nucleo_git\Core\Src, и ВСЁ ЗАРАБОТАЛО!
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 *
 * Ветка RTOS_ISR, на которой расположен код для изучения прерываний в среде RTOS,
 * сохраняемая в GitHub по адресу:
 * https://github.com/Peotr-B/Nucleo_git.git
 *
 * Ещё см. READMY.md
 *
 * За основу взят материал:
 *
 * STM32 от Булкина. Урок 1: Вводный, где мы немножко похулиганим
 * https://pikabu.ru/story/stm32_ot_bulkina_urok_1_vvodnyiy_gde_myi_nemnozhko_pokhuliganim_5526124
 *
 * а также:
 *
 * FreeRTOS для чайников. Краткое описание
 * http://easyelectronics.ru/freertos_manual.html
 *
 * STM Урок 116. FreeRTOS. Прерывания. Очереди в прерываниях
 * https://narodstream.ru/stm-urok-116-freertos-preryvaniya-ocheredi-v-preryvaniyax-chast-1/
 *
 * Используем светофоры в системе FreeRTOS на базе платы Arduino Nano в среде программирования Arduino IDE
 * https://texnohelp.com/arduino_freertos_lesson3/
 *
 * Осваиваем FreeRtos на примере приема-передачи по UART
 * https://kkmspb.ru/development/microcontrollers/UART/ISR_UART_Atollic_STM32F407VE.php
 *
 * Прерывания UART в FreeRTOS с драйвером STM32 HAL
 * https://question-it.com/questions/3532617/preryvanija-uart-v-freertos-s-drajverom-stm32-hal
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
#include "string.h" // это для функции strlen()
#include <stdio.h>
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
RTC_HandleTypeDef hrtc;

TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart2;

/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for ButtonPress */
osThreadId_t ButtonPressHandle;
const osThreadAttr_t ButtonPress_attributes = {
  .name = "ButtonPress",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Led */
osThreadId_t LedHandle;
const osThreadAttr_t Led_attributes = {
  .name = "Led",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for PWMod */
osThreadId_t PWModHandle;
const osThreadAttr_t PWMod_attributes = {
  .name = "PWMod",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Monitor */
osThreadId_t MonitorHandle;
const osThreadAttr_t Monitor_attributes = {
  .name = "Monitor",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Led_PWMSem */
osSemaphoreId_t Led_PWMSemHandle;
const osSemaphoreAttr_t Led_PWMSem_attributes = {
  .name = "Led_PWMSem"
};
/* USER CODE BEGIN PV */
//RTC_TimeTypeDef rtcTime;
//RTC_DateTypeDef rtcDate;
RTC_TimeTypeDef sTime;
RTC_DateTypeDef sDate;

int LED_State = 0;
int T_LED = 200;
int Btn_State = 0;
char str1[258];	//ВНИМАНИЕ! 256 + 2 на символы \r\n!!!

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_RTC_Init(void);
static void MX_TIM3_Init(void);
void StartDefaultTask(void *argument);
void StartButtonTask(void *argument);
void StartLedTask(void *argument);
void StartPWModTask(void *argument);
void StartMonitorTask(void *argument);

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
  MX_RTC_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of Led_PWMSem */
  Led_PWMSemHandle = osSemaphoreNew(1, 1, &Led_PWMSem_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of ButtonPress */
  ButtonPressHandle = osThreadNew(StartButtonTask, NULL, &ButtonPress_attributes);

  /* creation of Led */
  LedHandle = osThreadNew(StartLedTask, NULL, &Led_attributes);

  /* creation of PWMod */
  PWModHandle = osThreadNew(StartPWModTask, NULL, &PWMod_attributes);

  /* creation of Monitor */
  MonitorHandle = osThreadNew(StartMonitorTask, NULL, &Monitor_attributes);

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

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
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
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0;
  sTime.Minutes = 0;
  sTime.Seconds = 0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 1;
  sDate.Year = 0;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 65535;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

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
  HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : BUTTON_Pin */
  GPIO_InitStruct.Pin = BUTTON_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BUTTON_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD4_Pin */
  GPIO_InitStruct.Pin = LD4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD4_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

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
	/*****************
	 * Задача по-умолчанию.
	 * Я предпочитаю пихать в неё всякие рутинные
	 * вещи, вроде инициальзации и сброса состояний,
	 * опроса с равными промежутками и прочее.
	 *
	 * Например, сейчас я сделал запуск задачи раз в
	 * секунду, и запрашиваю текущее время и дату
	 *****************/

	TickType_t xLastWakeTime;
	//const TickType_t xFrequency = 1000;
	const TickType_t xFrequency = T_LED;	//Уменьшиил такт для удобства
	xLastWakeTime = xTaskGetTickCount();

	/* Infinite loop */
	for (;;)
	{
		HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
		LED_State = HAL_GPIO_ReadPin(LD4_GPIO_Port, LD4_Pin);
		//osDelay(T_LED);

		// Тут можно запускать какие-то действия
		//HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
		//HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

		if (HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
		{
			printf("HAL_RTC_GetTime failed");
			Error_Handler();
		}

		//uint ICR = (uint32_t)(hrtc.Instance->CR);
		//uint ICR = (uint32_t)(RTC->CR);
		//printf("CR =  %u\r\n", ICR);

		puts("RTOS (puts): режим LED + Time");  //Для puts, перевод каретки задан по умолчанию
		printf("Time--%d:%d:%d\r\n", sTime.Hours, sTime.Minutes, sTime.Seconds);//Для SWO

		//Для USB:
		snprintf(str1, sizeof(str1), "Time %d:%d:%d\n", sTime.Hours,
				sTime.Minutes, sTime.Seconds);
		HAL_UART_Transmit(&huart2, (uint8_t*) str1, strlen(str1),
		HAL_MAX_DELAY);

		// Благодаря этой функции, задача будет
		// уходить на новый круг в равные промежутки
		// времени.
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
		//osDelay(1);
	}
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartButtonTask */
/**
 * @brief Function implementing the ButtonPress thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartButtonTask */
void StartButtonTask(void *argument)
{
  /* USER CODE BEGIN StartButtonTask */
	/* Infinite loop */
	for (;;)
	{
		osDelay(1);
	}
  /* USER CODE END StartButtonTask */
}

/* USER CODE BEGIN Header_StartLedTask */
/**
 * @brief Function implementing the Led thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartLedTask */
void StartLedTask(void *argument)
{
  /* USER CODE BEGIN StartLedTask */
	/* Infinite loop */
	for (;;)
	{
		osDelay(1);
	}
  /* USER CODE END StartLedTask */
}

/* USER CODE BEGIN Header_StartPWModTask */
/**
 * @brief Function implementing the PWMod thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartPWModTask */
void StartPWModTask(void *argument)		//см. скетч от 17апр22
{
  /* USER CODE BEGIN StartPWModTask */
	/* Infinite loop */
	for(;;)
	  {
	      for (int i = 0; i <= 131072; i++)
			{
				if (i < 65536)
					TIM3->CCR1 = i;
				else if ((i > 65535) && (i < 131072))
					TIM3->CCR1 = 131071 - i;
			}
	      //osDelay(50);
	      osThreadYield();
	  }
  /* USER CODE END StartPWModTask */
}

/* USER CODE BEGIN Header_StartMonitorTask */
/**
 * @brief Function implementing the Monitor thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartMonitorTask */
void StartMonitorTask(void *argument)
{
  /* USER CODE BEGIN StartMonitorTask */
	/* Infinite loop */
	for (;;)
	{
		osDelay(1);
	}
  /* USER CODE END StartMonitorTask */
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
