# Nucleo_git [multitasking]
 STM32-Nucleo-github

 22���22
 @Biriuk
 peotr60@mail.ru
 
 �������� ���������������� STM32 � ����� STM32CubeIDE � ������� ���������� HAL
 � �������������� ���������� ����� NUCLEO-L452RE-P
 
 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  ��������!
  ��� ������������� RTC � ���������� HAL ������� ��������� ���, ������� ����� ������ ��������� �������:
  � ����� stm32l4xx_hal_rtc.c (Drivers / Src / stm32l4xx_hal_rtc.c) ���������������� ������:
  
  1560	// sTime->SubSeconds = (uint32_t)(hrtc->Instance->SSR);
  
  ��� ���� ������� ����� � ����, ��� ������ ��� ����� ���������� ������� ������� ������ ���
  ��������������� ��� ������ �������.
  
  ��� � ������ ������ �������� ��������� �������:
  
  1. �������� ����� �� ����������, ��������� ����.
  2. � ������, ���� � ���� StartMonitorTask �������� ������:
  HAL_RTC_SetTime(&hrtc, &nTime, RTC_FORMAT_BIN);
  �� ��������� ��������� ������� ��������������� ���������, ������, ��������� ���� PC13 
  ������������ ������� ���������, ��� ��������� � ��������� �� ��� ������� ������, ��������������,
  � ������������� ���������� ������������ ���������� �� ������� ������ B1_Blue. �����, ����� 
  ����������/��������� ������� �� ���� ������ ������� ������� ������-�� ������� ��������� ������� 
  �������.
  �.�. � ������ ������������� � ����� ��� ������������, ��� ���������� �������� �����, ������
  HAL_RTC_SetTime(&hrtc, &nTime, RTC_FORMAT_BIN);
  ��� PC13 ������������ ������!
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 
 ���� ��� ����������� �  GitHub �� ������:
 https://github.com/Peotr-B/Nucleo_git.git
 =============================================================================================
 
 �������� ���������
 ����� ��� ������� � ������� ���������� ������������� ����������� ��������� ������� ������ ���������
 ���������� ����������� LD4_Pin � ������� ��������� �� �������� �� ����� USB � ������ StartDefaultTask
 ����� ����, �� ����������� ��������� ����������� ����� ����� SWO �� �������� STM32 ST-LINK
 � ������� ������ printf � puts, ��� ���������������� ������� � main.c �������� ��������� ���������:
 *****************************************************************************************************
  /* USER CODE BEGIN 4 */
//��� ���������� ������ ��� ������������� ������ ���������� � ��������� ITM � ����� STM32CubeIDE
//(������ ���� � ������ ��������� SWO)
//https://www.youtube.com/watch?v=nE-YrKpWjso&list=PL9lkEHy8EJU8a_bqiJXwGTo-uM_cPa98P
int __io_putchar(int ch)
{
	ITM_SendChar(ch);
	return ch;
}

//���:

//STM32: ������� ����� SWO � STM32CubeIDE � ���������� ST-LINK
//https://www.youtube.com/watch?v=ST_fUu6ACzE
//https://www.youtube.com/watch?v=iC2-0Md-6yg

//int _write(int file, char *ptr, int len)
 //{
 //int i = 0;
 //for(i = 0; i < len; i++)
    //ITM_SendChar((*ptr++));
 //return len;
 //}

/* USER CODE END 4 */
 ****************************************************************************************************
  �����. � ��������� ������� ��������� ������ (�����):
 
 1. DefaultTask
 � ���� ������ ������������ ������� �������� ���������� � �������� 500 ��. ��� ������� ������� ������
 ������ ������� ����������� �� 100 ��.
 
 2. BtnTask
 � ���� ������ ������ �� ������ ��������� �������� ������� �������� ���������� � ���������� � �������
 QueueData ��� ������ ��������� ������ �� ������� ��
 
 3. PWM_Task
 � ���� ������ �������������� � ������� �� ��� PC6 ������ PWM
 
 4. INGPIO_Task
 � ���� ������ � ������� QueueData ���������� ������ � ���� PC8, �� ������� ����� ������, ��������, ������
 � ���� PC6 � ������� ��������
 
 5. GetQuTask
 � ���� ������ ������������ ����� �� ������� �� ����������, ���������� �� ������� QueueData
 
 6. MonitorTask
 � ���� ������ ������������ ����� �� ������� �� �������� ��������� ������� ������ ���������
 
 ����� ����, ��� ����������� �������� ������ ���������� �� ������� ��, ��� �������� ������������ �����,
 �������� ������� MutexMonitor � ������� GetQuTask � MonitorTask
 =============================================================================================
 
 24���22
 ������� ������� ������� ������ ��������� � ������� �:�:�
 ������-�� ����� � ����� ���������� � "1". ��������� ��� ������� �������� ������ ����� 60,
 ��������, 64. ������� ������ � BCD �� BIN, ������, � ������� �������� � ��������� ��������:
 --------------------------------------------------
 ������ ���������
 � � ���� � ����� ������ �� 90 ���. ��� �� ����? 
 �������� �������� ������ � ��� ����������!
 https://www.youtube.com/watch?v=0CaETEzPh0g
 --------------------------------------------------
 ����� ����� ������� ������ � ������� �������� ���������. �� ���� ���� ����������������� �� "23", 
 ����� �������� "0" ����� ����������� ���������� � ������ ���������� ������� (��. ���������� HAL RTC):
 --------------------------------------------------
 HAL_StatusTypeDef HAL_RTC_GetTime(RTC_HandleTypeDef *hrtc, RTC_TimeTypeDef *sTime, uint32_t Format)
 {...
 sTime->Hours = (uint8_t)RTC_Bcd2ToByte(sTime->Hours);
 (����� ��� �� ������� ������, �� ��� ���������� ����������)
 sTime->Minutes = (uint8_t)RTC_Bcd2ToByte(sTime->Minutes);
 sTime->Seconds = (uint8_t)RTC_Bcd2ToByte(sTime->Seconds);
  }
 --------------------------------------------------- 
 �������� ����������� ���� ��� ������ (MINIMAL_STACK_SIZE) � 128 �� 256 � Config parameters
 �� �������.
 ���������� � ��������� ������.
 --------------------------------------------------
 25���22
 ����� ������� �������� �������� �� ��������� �����:
  
 Pavel San
 ��� ��� ����������� ������ 32��� ��������� ����������� �� PC13?
 
 ��������� ��������
 ������ ������ ���� � ��������� RTC OUT ������ Disable ��������� No RTC Output. 
 �� ���� ��� ������ Errata, �.�. � ��������� �� ��� ������������� PC13 ����� ���� �������� � ��������� RTC. 
 https://www.youtube.com/watch?v=3Fkhim7chTk
 
 -------------------------------------------------------------------------------------
 26���22
 � ������������ ������ StartDefaultTask, � ������� ������������ ������� ����������� � ����� �� �������
 ������� ������ ���������, ������� ��������� ��� ������:
 BtnTask - ���������� ������� �� ������
 MonitorTask - ���������� ������� ����������  �� ������� � ��������� ������
               - �������
 ����� ������������� (�� �����������) ���������������, ��� ������� � ����� ���� ������ �� ������ ���� ������� 
 ����������� ��� ������ ������� osThreadYield(). 
 
 ��� ���������  ������������� ��������������� ���������� � ���� MX:
 Middleware / FREERTOS / Config parameters / 
 � USE_PREEMPTION ���������� Disabled
 
 ����������. 
 ����� ���������� �������� "0" � ������ define configUSE_PREEMPTION � ����� FreeRTOSConfig.h:
 
 62. #define configUSE_PREEMPTION      0
 
 ���� ���� ���������  � ���� Project Explorer: Core\Inc\FreeRTOS.h, ��� �� �����: 
 D:\STM32\STM32_File\Nucleo_git\Core\Inc
 ��.:
 [FreeRTOS Learning 05] �������� �������� FreeRTOSConfig.h ��� ���������� �������������� ��������� �������
 https://russianblogs.com/article/50011436450/

 ������! ��� ���������� ������� ������������� �������� "1". 
 �������! ������������� �������� ����� ��������� ����� ������ � ���� MX.

 -------------------------------------------------------------------------------------
 31���22
 ������� ������� ������: 
 PWM_TaskHandle ��� ����������� ������ ������� PWM �� ���� PC6
 -------------------------------------------------------------------------------------
 2���22
 ������� ������� ������� GPIO �� ������ PC6, PC8. 
 �� ������� TIM3 � ����� NVIC ��������  Setting TIM3 global interrupt, � ����� 
 �������������� ������� ����� �������� RTOS. 
 �����, ��� ��� ������ ��������� ���������� � ������ ������������� RTOS �� ���������.
 ������� ���������� �� TIM3.
 -------------------------------------------------------------------------------------
  3���22
 ��������! ������ ������������� ��������������� ����� �������� ��� ����������� ������� �����.
 � ������������ � ����, ����� ���������� �� ������������� ���������������, � ��������� ���������
 ���������������, ��������� ����������� ������ ��� ��������� ������������ ������� ����������
 ���������� �����.
 
 -------------------------------------------------------------------------------------
 6���22
 � ���� �������� ������������� ��������������� �������, ��� ��������� � ����� ������
 � ������ ������� ������������� ��� ��� ���������� ����� � �������� ��������� ����������
 � ����������� ���������� ���������������, �� �������������� ������� ����
 taskENTER_CRITICAL() --- taskEXIT_CRITICAL() � vTaskSuspendAll() --- xTaskResumeAll().
 ��������� ����� ��������� �����:
 [4.2. ����������� ������ � ������������ ��������]
 http://microsin.net/programming/arm/freertos-part4.html
 
 �������, � ������ ������� ����� ������������ �������� ��� ��������� �� ������� �����
 
 -------------------------------------------------------------------------------------
 6���22
 ��������!
 ������� ��������, ��� ��� ������� ���� CubeMX �� FREERTOS � ���� Categories ����������
 ������� ������� � ��������. ����� ����, ����� �� ������� FreeRTOS Heap Usage � ������ 
 HEAP STILL ABAILABLE ������������� �������� Bytes! ����� � �� ������� Config parameters
 �������� �������� TOTAL_HEAP_SIZE, � ��� ������ �� 12000 Bytes. ����� ���� ������� ������
 �� ����������, ����� ����, ��� ����������� ������ Memory Management scheme �� heap_4 
 (��� ���� �� ���������).
 ************************************************************************************************************ 
 
 ����� �������� (�� �������� � "FreeRTOS ��� ��������. ������� ��������"
 http://easyelectronics.ru/freertos_manual.html)
 
 1. ������. �������� �����
 2. �������. ����� ������� ����� ��������
 3. ��������
 4. Mutex
 5. ��������� ������
 6. ��������� � ���������������
 7. ��������, ��������� ������, � ��� �� �� ��� � ������������? ������� taskYIELD
 8. ������ freeRTOS � ������������
 9. ����������� aka Co-Routines
 
 �� ������ ���� ��������:
  STM32 � ����. FreeRTOS. ������������� ���������������
  https://microtechnics.ru/stm32-uchebnyj-kurs-freertos-chast-3/
  
  ������� ���������� STM32CubeIDE HAL ��������� ������ ��������� ������� RTC � ����������� ������� ������� �������
  https://russianblogs.com/article/7849848366/
  
  ���� ��. �.�.�������
  ������-��������� "SM32CubeIDE + FreeRTOS. ������� �����, �������, ���������"
  Vladimir Leonidov [BMSTU]
  https://www.youtube.com/watch?v=JKkyF53AAM4
  Lab 7 - FreeRTOS
    
  ������ RTOS_1
  
 STM32 � CubeIDE: ��� � ���������
 https://robotclass.ru/tutorials/stm32-cubeide-pwm/
 
 STM ���� 10. HAL. ������� PWM (���). ������ ������������ ������
 https://narodstream.ru/stm-urok-10-hal-izuchaem-pwm-shim-migaem-svetodiodami-plavno/
 
 STM32. PWM �� ��������. ��������� ���� �������������. ��������� � Arduino �� STM32
 https://www.youtube.com/watch?v=ZEdIQvwIpaM&t=2s
 https://github.com/Solderingironspb/Lessons-Stm32.git
 
 ���� 8. ���������� ������� �����-������ ����� ������� ���������� HAL
 http://mypractic.ru/urok-8-upravlenie-portami-vvoda-vyvoda-cherez-funkcii-biblioteki-hal.html
 
 FreeRTOS: ������������ ����������, ����� 4 (���������� ���������) 
 http://microsin.net/programming/arm/freertos-part4.html
 
 STM32 �� �������. ���� 1: �������, ��� �� �������� �����������
 https://pikabu.ru/story/stm32_ot_bulkina_urok_1_vvodnyiy_gde_myi_nemnozhko_pokhuliganim_5526124
 
 -------------------------------------------------------------------------------------
 
 � ���� ���������� ��������������:
 
 STM32 + FreeRTOS
 http://we.easyelectronics.ru/STM32/stm32-freertos.html
 
 Nucleo_git �� 5���22 (���������� ���������)
 
 Nucleo_git �� 14���22 (������������� ���������)
 
 ���� �3. ������������� ������� FreeRTOS �� ������� ������ ������ � 
 ���������� � ������� ���������������� STM32F103:
 https://texnohelp.com/stm32-freertos-lesson3/
 
 � �����:
 
 FreeRTOS ��� ��������. ������� ��������
 http://easyelectronics.ru/freertos_manual.html

 [STM32L4] ������� ��������� FreeRTOS ��� ������ � �������� ���� ���������������� ������
 https://russianblogs.com/article/88161192459/
 
 ����� ARM. RTOS ����� 5-��. ��������
 https://www.mcu.by/%D1%81%D1%82%D0%B0%D1%80%D1%82-arm-rtos-%D1%87%D0%B0%D1%81%D1%82%D1%8C-5-%D0%B0%D1%8F-%D0%BC%D1%8C%D1%8E%D1%82%D0%B5%D0%BA%D1%81%D1%8B/

 ������������� CMSIS-RTOS
 https://russianblogs.com/article/8958290203/

 FreeRTOS ������������� � ��������� ���������� ����������� �����. ����������� � ������������� ������
 https://www.youtube.com/watch?v=VuyL0sCVPk8

 ���������������� STM32: ������������� ���������� RTC
 https://eax.me/stm32-rtc/
 
 RTC HAL stm32
 https://istarik.ru/blog/stm32/116.html

 ������������� CMSIS-RTOS
 https://russianblogs.com/article/8958290203/

 FreeRTOS ������������� � ��������� ���������� ����������� �����. ����������� � ������������� ������
 https://www.youtube.com/watch?v=VuyL0sCVPk8
 
 ����� ����:
 
 STM32CubeMx. ������� ����� � FreeRTOS ��� STM32
 https://microtechnics.ru/stm32cubemx-bystryj-start-s-freertos-dlya-stm32/
 
 STM32 � ����. FreeRTOS. ���� ���������������, ������ ���������
 https://microtechnics.ru/stm32-uchebnyj-kurs-freertos-chast-2/
 
 STM32 � ����. FreeRTOS. ������������� ���������������
 https://microtechnics.ru/stm32-uchebnyj-kurs-freertos-chast-3/
 
 FreeRTOS: ������������ ����������
 http://microsin.net/programming/arm/freertos-part1.html
 http://microsin.net/programming/arm/freertos-part2.html
 
 START: �������� ������� � STM32 Cube MX � FreeRTOS ��� Atollic TrueSTUDIO
 http://www.milbestore.ru/article/sozdanie-proekta-s-stm32-cube-mx-i-freertos-dlya-atollic-truestudio
 
 STM32CubeIDE. ���������, ��������� � �������. ����� 1
 https://www.youtube.com/watch?v=6xhzoDGi4qA
 
 STM32CubeIDE. ��� �������: ������������� ���������
 https://www.youtube.com/watch?v=PSiq8-DFxYU
 
 CMSIS-RTOS2  Version 2.1.3
 https://arm-software.github.io/CMSIS_5/RTOS2/html/group__CMSIS__RTOS__TimerMgmt.html#ga7dc24a4c2b90334427081c3da7a71915
 https://arm--software-github-io.translate.goog/CMSIS_5/RTOS2/html/group__CMSIS__RTOS__TimerMgmt.html?_x_tr_sl=auto&_x_tr_tl=ru&_x_tr_hl=ru#ga7dc24a4c2b90334427081c3da7a71915

 CMSIS-RTOS ��� ����������������� � ����� CORTEX-M3
 http://repo.ssau.ru/bitstream/Metodicheskie-izdaniya/CMSISRTOS-dlya-mikrokontrollerov-s-yadrom-CortexM3-metod-ukazaniya-k-lab-rabote-Tekst-elektronnyi-87343/1/CMSIS-RTOS%20%D0%B4%D0%BB%D1%8F%20%D0%BC%D0%B8%D0%BA%D1%80%D0%BE%D0%BA%D0%BE%D0%BD%D1%82%D1%80%D0%BE%D0%BB%D0%BB%D0%B5%D1%80%D0%BE%D0%B2%20%D1%81%20%D1%8F%D0%B4%D1%80%D0%BE%D0%BC%202020.pdf
 
 STM32CubeIDE. ���������, ��������� � �������. ����� 1
 https://www.youtube.com/watch?v=6xhzoDGi4qA
 
 STM32CubeIDE. ��� �������: ������������� ���������
 https://www.youtube.com/watch?v=PSiq8-DFxYU
 