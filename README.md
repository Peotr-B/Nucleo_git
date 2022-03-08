# Nucleo_git
 STM32-Nucleo-github

 018мар22
 @Biriuk
 peotr60@mail.ru
 
 Изучение микроконтроллера STM32 в среде STM32CubeIDE с помощью библиотеки HAL
 с использованием отладочной платы NUCLEO-L452RE-P
 
  https://github.com/Peotr-B/Nucleo_git.git
 
 Данный скетч создан при изучении бинарного семафора в FreeRTOS
 За основу взят:
 STM Урок 104. FreeRTOS. Бинарные (двоичные) семафоры. Часть 1, Часть 2
 https://narodstream.ru/stm-urok-104-freertos-binarnye-semafory-chast-1/
 В данном скетче семафор не позволяет реализоваться задаче Task2
 Задача Default по обыкновению используется для контроля работы скетча (мигание светодиода)

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
 