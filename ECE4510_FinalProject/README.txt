
Instructions for compiling:

1. Open an example project
2. Install the clock file system_stm32f4xx.c
3. Copy /lib folder to ~\IAR Embedded Workbench\arm\8.20.2\ST\STM32F4xx\STM32F4xx_DSP_StdPeriph_Lib\Libraries\STM32F4xx_StdPeriph_Driver\inc
4. Add all .c file in /lib to STM32F4xx_StdPeriph_Driver
5. Remove all TIM interrupt handlers and systick handlers that are not a part of the project

6. Compile the code