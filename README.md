# Nucleo_git
 STM32-Nucleo-github

 26���22
 @Biriuk
  peotr60@mail.ru

  Nucleo_git
  �������� ���������������� STM32 � ����� STM32CubeIDE � ������� ���������� HAL
  � �������������� ���������� ����� NUCLEO-L452RE-P
 
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  ��������!
  ��� ������������� RTC � ���������� HAL ������� ��������� ��� (����� �����), ������� ����� 
  ������ ��������� �������:
  � ����� stm32l4xx_hal_rtc.c (Drivers / Src / stm32l4xx_hal_rtc.c) ���������������� ������:

  1560	// sTime->SubSeconds = (uint32_t)(hrtc->Instance->SSR);
 
  ��� ���� ������� ����� � ����, ��� ������ ��� ����� ���������� ������� ������� ������ ���
  ��������������� ��� ������ �������.
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  ��������! 
  � ������� Nucleo_git �� ����� �������� ���������� ��������� � RTOS-CMSIS ��������� ��������, 
  ������ ������� �� ��� ��������� ����. �� ���� ������ ���������� �� �� �� ������ SWO. ��������� 
  � ����������� �������� RTOS_ETAPY �������� ���������� ������ syscalls.c � sysmem.c � Core / Src. 
  ���� ���� ��� ����� �� ������� RTOS_ETAPY, � ������� � D:\STM32\STM32_File\Nucleo_git\Core\Src, 
  � �Ѩ ����������!
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  
  ����� RTOS_ISR, �� ������� ���������� ��� ��� �������� ���������� � ����� RTOS,
  ����������� � GitHub �� ������:
  https://github.com/Peotr-B/Nucleo_git.git
  
  ****************************************************************************************
  �����!
  ������ �������� - �������� STM32.pdf, ���.633 (� Acrobat 662): 

  ������� �� ��������, ��� FreeRTOS ������������� ��� ��������� API-���������� 
  ��� ���������� ��������� �� ������ � ��������� �� ISR. ��������, ������� xQueueReceive() 
  ������������ ��� ������ �������� � ������� �� ������, � ������� xQueueReceiveFromISR() 
  ������������ ��� ����������� ������ ��������� � ������� �� ISR. 
  ������� CMSIS-RTOS, ������������� ST, ������������ ��� ��������������� �� ����� ������� 
  � ������������� ���������, ��������� �� �� ����� �� ������ ��� �� ISR. ��� ������, 
  �� ���� ������ � ��������.
  ****************************************************************************************

  �� ������ ���� ��������:
 
  STM32 �� �������. ���� 1: �������, ��� �� �������� �����������
  https://pikabu.ru/story/stm32_ot_bulkina_urok_1_vvodnyiy_gde_myi_nemnozhko_pokhuliganim_5526124
 
  � �����:
  
  ��������� �� ������� ������ ���������. STM32CubeMx + HAL
  https://zen.yandex.ru/media/id/5d12215f6f100900afa353f1/reagiruem-na-najatie-knopki-pravilno-stm32cubemx--hal-5d2b00f5e6cb9b00ad9c0fe3
  
  ���������� "��������" ������ 
  https://istarik.ru/blog/stm32/148.html
  
  ���������� Button ��� ��������� ������� ������ �� STM32. KY-004
  https://microtechnics.ru/profilegrid_blogs/biblioteka-button-dlya-obrabotki-nazhatij-knopok-na-stm32-ky-004/
 
  ���������������� STM32: ������ ������������� ��������, ���������� � ���
  https://eax.me/stm32-interrupts-timers-pwm/

  HAL: ������� ����������
  http://www.milbestore.ru/article/hal-vneshnie-preryvaniya

  STM32F103 + SPL + HAL: GPIO � ������� ����������
  http://www.count-zero.ru/2016/stm32_gpio/
  ������� ���������� � STM32 ������� �� ���������� STM8 "L"-�����, � ��� ���������, ��� ���� 
  � STM8L ����� ���� ���������� ��������� �� ������ ���� ���/� ����� �����, �� � STM32 
  ���������� ����������� ������ �� ������ ����. ��������:
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13) (��� �������� ����� �����) 
  
  STM ���� 116. FreeRTOS. ����������. ������� � �����������
  https://narodstream.ru/stm-urok-116-freertos-preryvaniya-ocheredi-v-preryvaniyax-chast-1/
  
  STM32. ���� 12. NVIC. EXTI. ������� ������� ����������
  https://www.youtube.com/watch?v=AUz1_qFrSCU
 
  ���������� ��������� � ������� FreeRTOS �� ���� ����� Arduino Nano � ����� ���������������� Arduino IDE
  https://texnohelp.com/arduino_freertos_lesson3/
 
  ��������� FreeRtos �� ������� ������-�������� �� UART
  https://kkmspb.ru/development/microcontrollers/UART/ISR_UART_Atollic_STM32F407VE.php
 
  ���������� UART � FreeRTOS � ��������� STM32 HAL
  https://question-it.com/questions/3532617/preryvanija-uart-v-freertos-s-drajverom-stm32-hal
 
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
 
 � ���� ���������� ��������������:
 
 STM32 + FreeRTOS
 http://we.easyelectronics.ru/STM32/stm32-freertos.html
 
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
 
 ������� ������� �� ���������������� STM32F103
 https://texnohelp.com/freertos-stm32-lesson1/
 
 STM32 � FreeRTOS. 1. ����������� � ��������
 https://habr.com/ru/post/249273/
 
 STM32 � FreeRTOS. 2. ��������� ��-�������
 https://habr.com/ru/post/249283/
 
 STM32 � FreeRTOS. 3. ������ � �������
 https://habr.com/ru/post/249381/
 
 RTOS: �������������� ����� ��������
 http://www.milbestore.ru/article/rtos-vzaimodejstvie-mezhdu-zadachami
 
 FreeRTOS-�������
 https://russianblogs.com/article/51871250461/
 
 ������-��������� "SM32CubeIDE + FreeRTOS. ������� �����, �������, ���������"
 https://www.youtube.com/watch?v=JKkyF53AAM4
 
 STM32CubeIDE. ���������, ��������� � �������. ����� 1
 https://www.youtube.com/watch?v=6xhzoDGi4qA
 
 STM32CubeIDE. ��� �������: ������������� ���������
 https://www.youtube.com/watch?v=PSiq8-DFxYU
 
 STM ���� 104. FreeRTOS. �������� (��������) ��������
 https://narodstream.ru/stm-urok-104-freertos-binarnye-semafory-chast-2/
 
 STM32 + FreeRTOS ����� 4. ������� ���������� ���������
 https://www.youtube.com/watch?v=pT5alekoeZk
 