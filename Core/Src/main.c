/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * 22мар22
  * @Biriuk
  * peotr60@mail.ru
  *
  * Nucleo_git
  * Изучение микроконтроллера STM32 в среде STM32CubeIDE с помощью библиотеки HAL
  * с использованием отладочной платы NUCLEO-L452RE-P
  * 
  * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  * ВНИМАНИЕ!
  * При использовании RTC в библиотеке HAL имеется серьёзный баг, который можно обойти следующим образом:
  * В файле stm32l4xx_hal_rtc.c (Drivers / Src / stm32l4xx_hal_rtc.c) закомментировать строку:
  * 
  * 1560	// sTime->SubSeconds = (uint32_t)(hrtc->Instance->SSR);
  * 
  * При этом следует иметь в виду, что каждый раз после пересборки проекта придётся каждый раз
  * закоментировать эту строку вручную.
  * Подробнее см. README.md
  * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  * 
  * Ветка multutasking, на которой расположен код для изучения многозадачности,
  * сохраняемая в GitHub по адресу:
  * https://github.com/Peotr-B/Nucleo_git.git
  *
  * Рассмотрена многозадачность в среде CMSIS-RTOS2
  * В данном случае корпоративная многозадачность (co-operative multitasking)
  * ВНИМАНИЕ!
  * В ходе изучения корпоративной многозадачности выяснил, что применять её можно только
  * в случае крайней необходимости или для фрагментов задач с коротким временным интервалом
  * с применением выесняющей многозадачности, но использованием функций типа
  * taskENTER_CRITICAL() --- taskEXIT_CRITICAL() и vTaskSuspendAll() --- xTaskResumeAll().
  * Подробнее можно прочитать здесь:
  * [4.2. Критические секции и приостановка шедулера]
  * http://microsin.net/programming/arm/freertos-part4.html
  *
  * Поэтому, в данном примере решил использовать мьютексы для затратных по времени задач
  * Подробнее см. в README.md
  *
  * За основу взят материал:
  * STM32 с нуля. FreeRTOS. Кооперативная многозадачность
  * https://microtechnics.ru/stm32-uchebnyj-kurs-freertos-chast-3/
  *
  * Функция библиотеки STM32CubeIDE HAL управляет часами реального времени RTC и прерыванием сигнала тревоги таймера
  * https://russianblogs.com/article/7849848366/
  *
  * а также:
  *
  * FreeRTOS для чайников. Краткое описание
  * http://easyelectronics.ru/freertos_manual.html
  *
  * [STM32L4] Очередь сообщений FreeRTOS для приема и отправки трех последовательных портов
  * https://russianblogs.com/article/88161192459/
  *
  * Использование семафоров и мьютексов в FreeRTOS на Arduino Uno
  * https://microkontroller.ru/arduino-projects/ispolzovanie-semaforov-i-myuteksov-v-freertos-na-arduino-uno/
  *
  * Старт ARM. RTOS часть 5-ая. Мьютексы
  * https://www.mcu.by/%D1%81%D1%82%D0%B0%D1%80%D1%82-arm-rtos-%D1%87%D0%B0%D1%81%D1%82%D1%8C-5-%D0%B0%D1%8F-%D0%BC%D1%8C%D1%8E%D1%82%D0%B5%D0%BA%D1%81%D1%8B/
  *
  * Использование CMSIS-RTOS
  * https://russianblogs.com/article/8958290203/
  *
  * FreeRTOS Приоритизация и алгоритмы управления диспетчером задач. Вытесняющий и кооперативный режимы
  * https://www.youtube.com/watch?v=VuyL0sCVPk8
  *
  * FreeRTOS: практическое применение, часть 4 (управление ресурсами)
  * http://microsin.net/programming/arm/freertos-part4.html
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
#include "string.h" // это для функции strlen()
#include <stdio.h>
#include "stdbool.h" //для булевских функций
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* Определение типа для структуры, которая будет передаваться через очередь. */
typedef struct
{
    char cSource[128];
    //unsigned char ucValue;
    char ucValue;
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
 RTC_HandleTypeDef hrtc;

TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart2;

/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for BtnTask */
osThreadId_t BtnTaskHandle;
const osThreadAttr_t BtnTask_attributes = {
  .name = "BtnTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for MonitorTask */
osThreadId_t MonitorTaskHandle;
const osThreadAttr_t MonitorTask_attributes = {
  .name = "MonitorTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for PWM_Task */
osThreadId_t PWM_TaskHandle;
const osThreadAttr_t PWM_Task_attributes = {
  .name = "PWM_Task",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for INGPIO_Task */
osThreadId_t INGPIO_TaskHandle;
const osThreadAttr_t INGPIO_Task_attributes = {
  .name = "INGPIO_Task",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for GetQuTask */
osThreadId_t GetQuTaskHandle;
const osThreadAttr_t GetQuTask_attributes = {
  .name = "GetQuTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for QueueData */
osMessageQueueId_t QueueDataHandle;
const osMessageQueueAttr_t QueueData_attributes = {
  .name = "QueueData"
};
/* Definitions for MutexMonitor */
osMutexId_t MutexMonitorHandle;
const osMutexAttr_t MutexMonitor_attributes = {
  .name = "MutexMonitor"
};
/* USER CODE BEGIN PV */
char str1[128];
//unsigned char str2[256];
char str2[258];	//ВНИМАНИЕ! 256 + 2 на символы \r\n!!!
int LED_State = 0;
int T_LED = 200;
int Btn_State = 0;
int Btn_State_temp = 0;
//int v_Hours, v_Minutes, v_Seconds;	//Переменные для проверки в режиме отладки
uint32_t i, d;
//bool P_IN = 0;
int P_IN = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_RTC_Init(void);
static void MX_TIM3_Init(void);
void StartDefaultTask(void *argument);
void StartBtnTask(void *argument);
void StartMonitorTask(void *argument);
void StartPWM_Task(void *argument);
void StartINGPIO_Task(void *argument);
void StartGetQuTask(void *argument);

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
  /* Create the mutex(es) */
  /* creation of MutexMonitor */
  MutexMonitorHandle = osMutexNew(&MutexMonitor_attributes);

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

  /* creation of BtnTask */
  BtnTaskHandle = osThreadNew(StartBtnTask, NULL, &BtnTask_attributes);

  /* creation of MonitorTask */
  MonitorTaskHandle = osThreadNew(StartMonitorTask, NULL, &MonitorTask_attributes);

  /* creation of PWM_Task */
  PWM_TaskHandle = osThreadNew(StartPWM_Task, NULL, &PWM_Task_attributes);

  /* creation of INGPIO_Task */
  INGPIO_TaskHandle = osThreadNew(StartINGPIO_Task, NULL, &INGPIO_Task_attributes);

  /* creation of GetQuTask */
  GetQuTaskHandle = osThreadNew(StartGetQuTask, NULL, &GetQuTask_attributes);

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

  /*Configure GPIO pins : Btn_Blue_Pin IN_PWM_Pin */
  GPIO_InitStruct.Pin = Btn_Blue_Pin|IN_PWM_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

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
    /*
     RTC_TimeTypeDef nTime;

     //nTime.Hours = 0x0;	//Почему-то показывает на 1 час больше
     nTime.Hours = 23;
     nTime.Minutes = 0;
     nTime.Seconds = 0;	//Почему-то секунды считаются больше 60-ти (замечал 64 и более)
     HAL_RTC_SetTime(&hrtc,&nTime,RTC_FORMAT_BIN);
     //HAL_RTC_SetTime(&hrtc,&nTime,RTC_FORMAT_BCD);
     */
    /* Infinite loop */
    for (;;)
	{
	/*
	 // Получаем время работы программы в часовом формате:
	 HAL_RTC_GetTime(&hrtc,&nTime,RTC_FORMAT_BIN);
	 //HAL_RTC_GetTime(&hrtc,&nTime,RTC_FORMAT_BCD);
	 */

	//Переменные для проверки в режиме отладки:
	/*
	 v_Hours = nTime.Hours;
	 v_Minutes = nTime.Minutes;
	 v_Seconds = nTime.Seconds;
	 */
	/*
	 printf("Time--%d:%d:%d\r\n",nTime.Hours,nTime.Minutes,nTime.Seconds);	//Для SWO

	 //Для USB:
	 snprintf(str1, sizeof(str1), "Time %d:%d:%d\n", nTime.Hours, nTime.Minutes, nTime.Seconds);
	 HAL_UART_Transmit(&huart2, (uint8_t*)str1, strlen(str1), 1000);

	 //osDelay(100);
	 */

	HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
	LED_State = HAL_GPIO_ReadPin(LD4_GPIO_Port, LD4_Pin);

	//snprintf в stm32:
	//https://eax.me/stm32-spi-flash/
	//snprintf(str1, sizeof(str1), "UART: RTOS osProgTimer\n\r");
	//https://istarik.ru/blog/stm32/113.html
	//HAL_UART_Transmit(&huart2, (uint8_t*) str1, strlen(str1), 100);

	//printf("printf: LED_State = %d\n\r",LED_State);
	//puts("RTOS (puts): режим LED\n\r");
	//puts("puts: режим Timer1 = %lu\n\r",tim_cnt); //так не работает!

	osDelay(T_LED);

	}
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartBtnTask */
/**
* @brief Function implementing the BtnTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartBtnTask */
void StartBtnTask(void *argument)
{
  /* USER CODE BEGIN StartBtnTask */
    xData msg;
    /* Infinite loop */
    for (;;)
	{
	//if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_SET)
	if (HAL_GPIO_ReadPin(GPIOC, Btn_Blue_Pin) == GPIO_PIN_SET)
	    {
	    osDelay(50);	//антидребезг

	    if (HAL_GPIO_ReadPin(GPIOC, Btn_Blue_Pin) == GPIO_PIN_SET)
		{
		//Btn_State = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);
		Btn_State = HAL_GPIO_ReadPin(GPIOC, Btn_Blue_Pin);
		T_LED = 250;
		}
	    }

	if (HAL_GPIO_ReadPin(GPIOC, Btn_Blue_Pin) == GPIO_PIN_RESET)
	    {
	    osDelay(50);	//антидребезг

	    if (HAL_GPIO_ReadPin(GPIOC, Btn_Blue_Pin) == GPIO_PIN_RESET)
		{
		Btn_State = HAL_GPIO_ReadPin(GPIOC, Btn_Blue_Pin);
		T_LED = 500;
		}
	    }

	msg.ucValue = Btn_State;
	strcpy(msg.cSource, "Button = ");
	//osMessageQueuePut(QueueDataHandle, &msg, 0, osWaitForever);
	osMessageQueuePut(QueueDataHandle, &msg, 0, 100);

	//osDelay(1);
	osThreadYield();
	}
  /* USER CODE END StartBtnTask */
}

/* USER CODE BEGIN Header_StartMonitorTask */
/**
* @brief Function implementing the MonitorTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartMonitorTask */
void StartMonitorTask(void *argument)
{
  /* USER CODE BEGIN StartMonitorTask */
    //Это критическая секция!
    //xData msg;
    //osStatus_t status;
    RTC_TimeTypeDef nTime;

    //nTime.Hours = 0x0;	//Почему-то показывает на 1 час больше
    nTime.Hours = 23;
    nTime.Minutes = 0;
    nTime.Seconds = 0;//Почему-то секунды считаются больше 60-ти (замечал 64 и более)
    //HAL_RTC_SetTime(&hrtc, &nTime, RTC_FORMAT_BIN);
    //HAL_RTC_SetTime(&hrtc,&nTime,RTC_FORMAT_BCD);
    /* Infinite loop */
    for (;;)
	{
	if (osMutexAcquire(MutexMonitorHandle, osWaitForever) == osOK)
	    {
	    // Получаем время работы программы в часовом формате:
	    if (HAL_RTC_GetTime(&hrtc, &nTime, RTC_FORMAT_BIN) != HAL_OK)
			{
				printf("HAL_RTC_GetTime failed");
				Error_Handler();
			}

			//HAL_RTC_GetTime(&hrtc, &nTime, RTC_FORMAT_BIN);
			//HAL_RTC_GetDate(&hrtc, &nDate, FORMAT_BIN);
			//HAL_RTC_GetTime(&hrtc,&nTime,RTC_FORMAT_BCD);

			//Переменные для проверки в режиме отладки:
			/*
			 v_Hours = nTime.Hours;
			 v_Minutes = nTime.Minutes;
			 v_Seconds = nTime.Seconds;
			 */

			//uint ICR = (uint32_t)(hrtc.Instance->CR);
			//uint ICR = (uint32_t)(RTC->CR);
			//printf("CR =  %u\r\n", ICR);
			printf("Time--%d:%d:%d\r\n", nTime.Hours, nTime.Minutes,
					nTime.Seconds);		//Для SWO

			//Для USB:
			snprintf(str1, sizeof(str1), "Time %d:%d:%d\n", nTime.Hours,
					nTime.Minutes, nTime.Seconds);
			HAL_UART_Transmit(&huart2, (uint8_t*) str1, strlen(str1),
			HAL_MAX_DELAY);

			//status = osMessageQueueGet(QueueDataHandle, &msg, NULL, osWaitForever);
			//status = osMessageQueueGet(QueueDataHandle, &msg, NULL, 100);

			//if (status == osOK)
			/* Данные были успешно приняты из очереди, вывод принятого значения
			 и источника этого значения. */
			/*
			 {
			 printf("%s:%u\r\n", msg.cSource, msg.ucValue);	//Для SWO
			 //snprintf(str2, sizeof(str2), "%s:%u\r\n", msg.cSource, msg.ucValue);//Для USB
			 snprintf(str2, sizeof(str2), "%s:%u\r\n", msg.cSource, msg.ucValue);//Для USB
			 //HAL_UART_Transmit(&huart2, (uint8_t*) str2, strlen(str2), osWaitForever);
			 HAL_UART_Transmit(&huart2, (uint8_t*) str2, strlen(str2),
			 HAL_MAX_DELAY); //проверить этот вариант
			 */
			//возможно, вариант с HAL_MAX_DELAY более логичен, т.к. предотвращает теоритеически
			//возможное зависание программы
			//}
	    osMutexRelease(MutexMonitorHandle);
	    }

	//osDelay(1);
	osThreadYield();
	}
  /* USER CODE END StartMonitorTask */
}

/* USER CODE BEGIN Header_StartPWM_Task */
/**
* @brief Function implementing the PWM_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartPWM_Task */
void StartPWM_Task(void *argument)
{
  /* USER CODE BEGIN StartPWM_Task */
  /* Infinite loop */
  for(;;)
  {
      for (i = 0; i <= 131072; i++)
		{
			if (i < 65536)
				TIM3->CCR1 = i;
			else if ((i > 65535) && (i < 131072))
				TIM3->CCR1 = 131071 - i;
		}
      //osDelay(50);
      osThreadYield();
  }
  /* USER CODE END StartPWM_Task */
}

/* USER CODE BEGIN Header_StartINGPIO_Task */
/**
* @brief Function implementing the INGPIO_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartINGPIO_Task */
void StartINGPIO_Task(void *argument)
{
  /* USER CODE BEGIN StartINGPIO_Task */
    xData msg;
  /* Infinite loop */
  for(;;)
  {
      /*
      if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8) != P_IN)
      	    {
      	P_IN = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8);
      	    }
      */
      msg.ucValue = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8);
      strcpy(msg.cSource,"Port 8");
      osMessageQueuePut (QueueDataHandle, &msg, 0, 100);

    //osDelay(1);
    osThreadYield();
  }
  /* USER CODE END StartINGPIO_Task */
}

/* USER CODE BEGIN Header_StartGetQuTask */
/**
* @brief Function implementing the GetQuTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartGetQuTask */
void StartGetQuTask(void *argument)
{
  /* USER CODE BEGIN StartGetQuTask */
    xData msg;
    osStatus_t status;
    /* Infinite loop */
    for (;;)
	{
	if (osMutexAcquire(MutexMonitorHandle, osWaitForever) == osOK)
	    {
	    status = osMessageQueueGet(QueueDataHandle, &msg, NULL,
	    100);

	    if (status == osOK)
	    /* Данные были успешно приняты из очереди, вывод принятого значения
	     и источника этого значения. */
		{
		printf("%s:%u\r\n", msg.cSource, msg.ucValue);	//Для SWO
		//snprintf(str2, sizeof(str2), "%s:%u\r\n", msg.cSource, msg.ucValue);//Для USB
		snprintf(str2, sizeof(str2), "%s:%u\r\n", msg.cSource,
			msg.ucValue);	    //Для USB
		//HAL_UART_Transmit(&huart2, (uint8_t*) str2, strlen(str2), osWaitForever);
		HAL_UART_Transmit(&huart2, (uint8_t*) str2, strlen(str2),
		HAL_MAX_DELAY); //проверить этот вариант
		//возможно, вариант с HAL_MAX_DELAY более логичен, т.к. предотвращает теоритеически
		//возможное зависание программы
		}
	    osMutexRelease(MutexMonitorHandle);
	    }
	//osDelay(1);
	osThreadYield();
	}
  /* USER CODE END StartGetQuTask */
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
