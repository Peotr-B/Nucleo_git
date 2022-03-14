# Nucleo_git
 STM32-Nucleo-github

 14мар22
 @Biriuk
 peotr60@mail.ru
 
 Изучение микроконтроллера STM32 в среде STM32CubeIDE с помощью библиотеки HAL
 с использованием отладочной платы NUCLEO-L452RE-P
 
  * Ветка Mutex, на которой расположен код для изучения мьютексов, сохраняемая в
  * GitHub по адресу:
  * https://github.com/Peotr-B/Nucleo_git.git
  *
  * Описание программы
  * Здесь как правило в учебных программах функционирует контрольная индикация штатной работы программы
  * встроееным светодиодом LD4_Pin в задаче StartDefaultTask
  * Кроме StartDefaultTask созданы следующие задачи:
  * Task1, посылающая в порт UART сообщение "Hi from Task1"
  * Task2, посылающая в порт UART сообщение "Hi from Task2"
  * кроме того, по обыкновению сообщения дублируются через канал SWO на STM32 ST-LINK
  * с помощью команд printf и puts, для функциональности которых в main.c вносятся следующие операторы:
  *****************************************************************************************************
  /* USER CODE BEGIN 4 */
//это стандартна¤ запись для посимвольного вывода информации в интерфейс ITM в среде STM32CubeIDE
//(должно быть в случае применени¤ SWO)
//https://www.youtube.com/watch?v=nE-YrKpWjso&list=PL9lkEHy8EJU8a_bqiJXwGTo-uM_cPa98P
int __io_putchar(int ch)
{
	ITM_SendChar(ch);
	return ch;
}

//или:

//STM32: отладка через SWO в STM32CubeIDE с доработкой ST-LINK
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
  * За основу взяты следующие материалы:
  *
  * STM Урок 104. FreeRTOS. Бинарные (двоичные) семафоры. Часть 2
  * https://narodstream.ru/stm-urok-104-freertos-binarnye-semafory-chast-2/
  *
  * Использование семафоров и мьютексов в FreeRTOS на Arduino Uno
  * https://microkontroller.ru/arduino-projects/ispolzovanie-semaforov-i-myuteksov-v-freertos-na-arduino-uno/
  *
  * CMSIS-RTOS ДЛЯ МИКРОКОНТРОЛЛЕРОВ С ЯДРОМ CORTEX-M3
  * http://repo.ssau.ru/bitstream/Metodicheskie-izdaniya/CMSISRTOS-dlya-mikrokontrollerov-s-yadrom-CortexM3-metod-ukazaniya-k-lab-rabote-Tekst-elektronnyi-87343/1/CMSIS-RTOS%20%D0%B4%D0%BB%D1%8F%20%D0%BC%D0%B8%D0%BA%D1%80%D0%BE%D0%BA%D0%BE%D0%BD%D1%82%D1%80%D0%BE%D0%BB%D0%BB%D0%B5%D1%80%D0%BE%D0%B2%20%D1%81%20%D1%8F%D0%B4%D1%80%D0%BE%D0%BC%202020.pdf
  *
  * Кармин Новиелло Освоение STM32
  * https://studfile.net/download.php?id=16485874&code=c62f016261edda346d8eeb9ceb166bd8&download-status=process0001
  *
  * Developing Applications on STM32Cube with RTOS
  * http://pro-interes.com/wp-content/uploads/2020/02/Developing-Applications-on-STM32Cube-with-RTOS-Rus.pdf
  *
  * Планировщик задач для ARM Cortex-M3
  * https://kit-e.ru/assets/files/pdf/2013_04_168.pdf
  *
  * FreeRTOS: практическое применение, часть 4 (управление ресурсами)
  * http://microsin.net/programming/arm/freertos-part4.html
  *
  * Старт ARM. RTOS часть 5-ая. Мьютексы
  * https://www.mcu.by/%D1%81%D1%82%D0%B0%D1%80%D1%82-arm-rtos-%D1%87%D0%B0%D1%81%D1%82%D1%8C-5-%D0%B0%D1%8F-%D0%BC%D1%8C%D1%8E%D1%82%D0%B5%D0%BA%D1%81%D1%8B/
  *
  Этапы изучения (по аналогии с "FreeRTOS для чайников. Краткое описание"
 http://easyelectronics.ru/freertos_manual.html)
 
 1. Задачи. Создание задач
 2. Очереди. Обмен данными между задачами
 3. Семафоры
 4. Mutex
 5. Системный таймер
 6. Диспетчер и Многозадачность
 7. Таймауты, обработка ошибок, и что же не так с прерываниями? Команда taskYIELD
 8. Работа freeRTOS с прерываниями
 9. Сопрограммы aka Co-Routines
 
 В ходе разработки использовалось:
 
 STM32 + FreeRTOS
 http://we.easyelectronics.ru/STM32/stm32-freertos.html
 
 STM32CubeMx. Быстрый старт с FreeRTOS для STM32
 https://microtechnics.ru/stm32cubemx-bystryj-start-s-freertos-dlya-stm32/
 
 STM32 с нуля. FreeRTOS. Типы многозадачности, пример программы
 https://microtechnics.ru/stm32-uchebnyj-kurs-freertos-chast-2/
 
 STM32 с нуля. FreeRTOS. Кооперативная многозадачность
 https://microtechnics.ru/stm32-uchebnyj-kurs-freertos-chast-3/
 
 FreeRTOS: практическое применение
 http://microsin.net/programming/arm/freertos-part1.html
 http://microsin.net/programming/arm/freertos-part2.html
 
 START: Создание проекта с STM32 Cube MX и FreeRTOS для Atollic TrueSTUDIO
 http://www.milbestore.ru/article/sozdanie-proekta-s-stm32-cube-mx-i-freertos-dlya-atollic-truestudio
 
 Создаем очереди на микроконтроллере STM32F103
 https://texnohelp.com/freertos-stm32-lesson1/
 
 STM32 и FreeRTOS. 1. Развлечение с потоками
 https://habr.com/ru/post/249273/
 
 STM32 и FreeRTOS. 2. Семафорим по-черному
 https://habr.com/ru/post/249283/
 
 STM32 и FreeRTOS. 3. Встаем в очередь
 https://habr.com/ru/post/249381/
 
 RTOS: взаимодействие между задачами
 http://www.milbestore.ru/article/rtos-vzaimodejstvie-mezhdu-zadachami
 
 FreeRTOS-очередь
 https://russianblogs.com/article/51871250461/
 
 Лекция-практикум "SM32CubeIDE + FreeRTOS. Примеры задач, очереди, семафоров"
 https://www.youtube.com/watch?v=JKkyF53AAM4
 
 STM32CubeIDE. Установка, настройка и отладка. Часть 1
 https://www.youtube.com/watch?v=6xhzoDGi4qA
 
 STM32CubeIDE. Как сделать: Использование отладчика
 https://www.youtube.com/watch?v=PSiq8-DFxYU
 
 STM Урок 104. FreeRTOS. Бинарные (двоичные) семафоры
 https://narodstream.ru/stm-urok-104-freertos-binarnye-semafory-chast-2/
 
 STM32 + FreeRTOS Часть 4. Функции управления очередями
 https://www.youtube.com/watch?v=pT5alekoeZk
 