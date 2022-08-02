# Nucleo_git
 STM32-Nucleo-github

 26апр22
 @Biriuk
  peotr60@mail.ru

  Nucleo_git
  Изучение микроконтроллера STM32 в среде STM32CubeIDE с помощью библиотеки HAL
  с использованием отладочной платы NUCLEO-L452RE-P
 
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  ВНИМАНИЕ!
  При использовании RTC в библиотеке HAL имеется серьёзный баг (время стоит), который можно 
  обойти следующим образом:
  В файле stm32l4xx_hal_rtc.c (Drivers / Src / stm32l4xx_hal_rtc.c) закомментировать строку:

  1560	// sTime->SubSeconds = (uint32_t)(hrtc->Instance->SSR);
 
  При этом следует иметь в виду, что каждый раз после пересборки проекта придётся каждый раз
  закоментировать эту строку вручную.
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  ВНИМАНИЕ! 
  В проекте Nucleo_git на ветке изучения прерываний совместно с RTOS-CMSIS обнаружил проблему, 
  решить которую не мог несколько дней. Не было вывода информации на ПК по каналу SWO. Сравнение 
  с действующим проектом RTOS_ETAPY показало отсутствие файлов syscalls.c и sysmem.c в Core / Src. 
  Тупо взял эти файлы из проекта RTOS_ETAPY, и вставил в D:\STM32\STM32_File\Nucleo_git\Core\Src, 
  и ВСЁ ЗАРАБОТАЛО!
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  
  Ветка RTOS_ISR, на которой расположен код для изучения прерываний в среде RTOS,
  сохраняемая в GitHub по адресу:
  https://github.com/Peotr-B/Nucleo_git.git
  
  ****************************************************************************************
  ВАЖНО!
  Кармин Новиелло - Освоение STM32.pdf, стр.633 (в Acrobat 662): 

  Примите во внимание, что FreeRTOS предоставляет два отдельных API-интерфейса 
  для управления очередями из потока и очередями из ISR. Например, функция xQueueReceive() 
  используется для снятия элемента с очереди из потока, а функция xQueueReceiveFromISR() 
  используется для безопасного снятия элементов с очереди из ISR. 
  Уровень CMSIS-RTOS, разработанный ST, предназначен для абстрагирования от этого аспекта 
  и автоматически проверяет, выполняем ли мы вызов из потока или из ISR. Как обычно, 
  за счет потери в скорости.
  ****************************************************************************************

  За основу взят материал:
 
  STM32 от Булкина. Урок 1: Вводный, где мы немножко похулиганим
  https://pikabu.ru/story/stm32_ot_bulkina_urok_1_vvodnyiy_gde_myi_nemnozhko_pokhuliganim_5526124
 
  а также:
  
  Реагируем на нажатие кнопки правильно. STM32CubeMx + HAL
  https://zen.yandex.ru/media/id/5d12215f6f100900afa353f1/reagiruem-na-najatie-knopki-pravilno-stm32cubemx--hal-5d2b00f5e6cb9b00ad9c0fe3
  
  Устранение "дребезга" кнопок 
  https://istarik.ru/blog/stm32/148.html
  
  Библиотека Button для обработки нажатий кнопок на STM32. KY-004
  https://microtechnics.ru/profilegrid_blogs/biblioteka-button-dlya-obrabotki-nazhatij-knopok-na-stm32-ky-004/
 
  Микроконтроллеры STM32: основы использования таймеров, прерываний и ШИМ
  https://eax.me/stm32-interrupts-timers-pwm/

  HAL: внешние прерывания
  http://www.milbestore.ru/article/hal-vneshnie-preryvaniya

  STM32F103 + SPL + HAL: GPIO и внешние прерывания
  http://www.count-zero.ru/2016/stm32_gpio/
  Внешние прерывания в STM32 походят на прерывания STM8 "L"-серии, с той оговоркой, что если 
  в STM8L можно было прерывания назначить по номеру пина или/и букве порта, то в STM32 
  прерывания назначаются только по номеру пина. Например:
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13) (без указания буквы порта) 
  
  STM Урок 116. FreeRTOS. Прерывания. Очереди в прерываниях
  https://narodstream.ru/stm-urok-116-freertos-preryvaniya-ocheredi-v-preryvaniyax-chast-1/
  
  STM32. Урок 12. NVIC. EXTI. Примеры внешних прерываний
  https://www.youtube.com/watch?v=AUz1_qFrSCU
 
  Используем светофоры в системе FreeRTOS на базе платы Arduino Nano в среде программирования Arduino IDE
  https://texnohelp.com/arduino_freertos_lesson3/
 
  Осваиваем FreeRtos на примере приема-передачи по UART
  https://kkmspb.ru/development/microcontrollers/UART/ISR_UART_Atollic_STM32F407VE.php
 
  Прерывания UART в FreeRTOS с драйвером STM32 HAL
  https://question-it.com/questions/3532617/preryvanija-uart-v-freertos-s-drajverom-stm32-hal
 
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
 