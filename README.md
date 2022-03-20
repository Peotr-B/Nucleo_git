# Nucleo_git
 STM32-Nucleo-github

 20���22
 @Biriuk
 peotr60@mail.ru
 
 �������� ���������������� STM32 � ����� STM32CubeIDE � ������� ���������� HAL
 � �������������� ���������� ����� NUCLEO-L452RE-P
 
  * ����� osTimer, �� ������� ���������� ��� ��� �������� ���������, ����������� �
  * GitHub �� ������:
  * https://github.com/Peotr-B/Nucleo_git.git
  *
  * �������� ���������
  * ����� ��� ������� � ������� ���������� ������������� ����������� ��������� ������� ������ ���������
  * ���������� ����������� LD4_Pin � ������ StartDefaultTask
  * ����� StartDefaultTask �������:
  * 
  * 
  * ����� ����, �� ����������� ��������� ����������� ����� ����� SWO �� STM32 ST-LINK
  * � ������� ������ printf � puts, ��� ���������������� ������� � main.c �������� ��������� ���������:
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
  * �� ������ ����� ��������� ���������:
  *
  * STM ���� 112. FreeRTOS. �������
  * https://narodstream.ru/stm-urok-112-freertos-tajmery/
  *
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
 
 CMSIS-RTOS2  Version 2.1.3
 https://arm-software.github.io/CMSIS_5/RTOS2/html/group__CMSIS__RTOS__TimerMgmt.html#ga7dc24a4c2b90334427081c3da7a71915
 https://arm--software-github-io.translate.goog/CMSIS_5/RTOS2/html/group__CMSIS__RTOS__TimerMgmt.html?_x_tr_sl=auto&_x_tr_tl=ru&_x_tr_hl=ru#ga7dc24a4c2b90334427081c3da7a71915

 CMSIS-RTOS ��� ����������������� � ����� CORTEX-M3
 http://repo.ssau.ru/bitstream/Metodicheskie-izdaniya/CMSISRTOS-dlya-mikrokontrollerov-s-yadrom-CortexM3-metod-ukazaniya-k-lab-rabote-Tekst-elektronnyi-87343/1/CMSIS-RTOS%20%D0%B4%D0%BB%D1%8F%20%D0%BC%D0%B8%D0%BA%D1%80%D0%BE%D0%BA%D0%BE%D0%BD%D1%82%D1%80%D0%BE%D0%BB%D0%BB%D0%B5%D1%80%D0%BE%D0%B2%20%D1%81%20%D1%8F%D0%B4%D1%80%D0%BE%D0%BC%202020.pdf
 
 ����������� ������� ���������� �������� CMSIS-RTOS
 https://russianblogs.com/article/8447562278/
 
 Developing Applications on STM32Cube with RTOS
 http://pro-interes.com/wp-content/uploads/2020/02/Developing-Applications-on-STM32Cube-with-RTOS-Rus.pdf

 ����������� ����� ��� ARM Cortex-M3
 https://kit-e.ru/assets/files/pdf/2013_04_168.pdf

 FreeRTOS: ������������ ����������, ����� 4 (���������� ���������)
 http://microsin.net/programming/arm/freertos-part4.html
  
 ������� �����. ����������� ������� ���������� �������� CMSIS-RTOS
 https://russianblogs.com/article/8447562278/

 ����� ARM. RTOS ����� 7-��. ����������� ������
 https://www.mcu.by/%D1%81%D1%82%D0%B0%D1%80%D1%82-arm-rtos-%D1%87%D0%B0%D1%81%D1%82%D1%8C-7-%D0%B0%D1%8F-%D0%BF%D1%80%D0%BE%D0%B3%D1%80%D0%B0%D0%BC%D0%BC%D0%BD%D1%8B%D0%B9-%D1%82%D0%B0%D0%B9%D0%BC%D0%B5%D1%80/

 23.7. ����������� �������
 https://studfile.net/preview/16485874/page:175/
 https://studfile.net/preview/16485874/page:176/
 Karmin_Noviello_-_Osvoenie_STM32.pdf, ���.674
 
 ������� ������ �����:
 STM ���� 112. FreeRTOS. �������
 https://narodstream.ru/stm-urok-112-freertos-tajmery/
 
 FreeRTOS ��� ��������. ������� ��������
 http://easyelectronics.ru/freertos_manual.html
 
 FreeRTOS ����������� �������
 https://www.youtube.com/watch?v=0zpugp5ZmpU
 
 ������� stm32 HAL - ����� ������
 https://istarik.ru/blog/stm32/118.html
 
 STM32 � ����. FreeRTOS. ���� ���������������, ������ ���������
 https://microtechnics.ru/stm32-uchebnyj-kurs-freertos-chast-2/
 
 STM32 � ����. FreeRTOS. ������������� ���������������
 https://microtechnics.ru/stm32-uchebnyj-kurs-freertos-chast-3/
 
 FreeRTOS: ������������ ����������
 http://microsin.net/programming/arm/freertos-part1.html
 http://microsin.net/programming/arm/freertos-part2.html
 
 START: �������� ������� � STM32 Cube MX � FreeRTOS ��� Atollic TrueSTUDIO
 http://www.milbestore.ru/article/sozdanie-proekta-s-stm32-cube-mx-i-freertos-dlya-atollic-truestudio
 
 RTOS: �������������� ����� ��������
 http://www.milbestore.ru/article/rtos-vzaimodejstvie-mezhdu-zadachami
 
 STM32CubeIDE. ���������, ��������� � �������. ����� 1
 https://www.youtube.com/watch?v=6xhzoDGi4qA
 
 STM32CubeIDE. ��� �������: ������������� ���������
 https://www.youtube.com/watch?v=PSiq8-DFxYU

 