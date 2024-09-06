/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usart.h"
#include "aht20.h"
#include "oled.h"
#include "stdio.h"
#include "string.h"
#include "ws2812.h"
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
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for oledTask */
osThreadId_t oledTaskHandle;
const osThreadAttr_t oledTask_attributes = {
    .name = "oledTask",
    .stack_size = 386 * 4,
    .priority = (osPriority_t)osPriorityNormal,
};
/* Definitions for rgbTask */
osThreadId_t rgbTaskHandle;
const osThreadAttr_t rgbTask_attributes = {
    .name = "rgbTask",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityNormal,
};
/* Definitions for keyTask */
osThreadId_t keyTaskHandle;
const osThreadAttr_t keyTask_attributes = {
    .name = "keyTask",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityBelowNormal7,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartOledTask(void *argument);
void StartRgbTask(void *argument);
void StartKeyTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void)
{
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

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
  /* creation of oledTask */
  oledTaskHandle = osThreadNew(StartOledTask, NULL, &oledTask_attributes);

  /* creation of rgbTask */
  rgbTaskHandle = osThreadNew(StartRgbTask, NULL, &rgbTask_attributes);

  /* creation of keyTask */
  keyTaskHandle = osThreadNew(StartKeyTask, NULL, &keyTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */
}

/* USER CODE BEGIN Header_StartOledTask */
/**
 * @brief  Function implementing the oledTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartOledTask */
void StartOledTask(void *argument)
{
  /* USER CODE BEGIN StartOledTask */
  /* Infinite loop */
  float Temperature = 0.0f, Humidity = 0.0f;
  char buf[25] = {0};
  for (;;)
  {
    // 中英文字符串混合显示&温湿度显示
    OLED_NewFrame();
    OLED_PrintString(31, 0, "演示程序", &font16x16, OLED_COLOR_REVERSED);
    OLED_ShowFrame();
    // osDelay(500);
    // OLED_PrintString(0, 22, "KEYSKING", &font16x16, OLED_COLOR_NORMAL);
    // OLED_ShowFrame();
    osDelay(270);
    AHT20_Read(&Temperature, &Humidity); // 测量温湿度
    sprintf(buf, "温度 %d℃", (int)Temperature);
    OLED_PrintString(0, 22, buf, &font16x16, OLED_COLOR_NORMAL);
    OLED_ShowFrame();
    osDelay(400);
    sprintf(buf, "湿度 %d%%", (int)Humidity);
    OLED_PrintString(0, 44, buf, &font16x16, OLED_COLOR_NORMAL);
    OLED_ShowFrame();
    osDelay(2500);
    // 显示变量值
    int count = 0;
    char buf[10] = {0};
    OLED_NewFrame();
    for (;;)
    {
      sprintf(buf, "%d %%", count);
      OLED_PrintASCIIString(40, 20, buf, &afont24x12, OLED_COLOR_NORMAL);
      OLED_ShowFrame();
      osDelay(15);
      if (count++ > 99)
      {
        break;
      }
    }
    osDelay(1000);
    // 直线绘制
    OLED_NewFrame();
    for (int i = 0; i < 128; i += 8)
    {
      OLED_DrawLine(0, 0, i, 63, OLED_COLOR_NORMAL);
      OLED_DrawLine(127, 0, 127 - i, 63, OLED_COLOR_NORMAL);
      OLED_ShowFrame();
      osDelay(30);
    }
    osDelay(1500);
    // 矩形绘制
    OLED_NewFrame();
    for (int i = 0; i < 64; i += 8)
    {
      OLED_DrawRectangle(i, i / 2, 127 - i * 2, 63 - i, OLED_COLOR_NORMAL);
      OLED_ShowFrame();
      osDelay(35);
    }
    osDelay(1500);
    // 矩形圆形
    OLED_NewFrame();
    for (int i = 63; i > 0; i -= 8)
    {
      OLED_DrawCircle(64, 32, i / 2, OLED_COLOR_NORMAL);
      OLED_ShowFrame();
      osDelay(40);
    }
    osDelay(1500);
    // 图片显示1
    OLED_NewFrame();
    OLED_DrawImage((128 - (bilibiliTVImg.w)) / 2, 0, &bilibiliTVImg, OLED_COLOR_NORMAL);
    OLED_ShowFrame();
    osDelay(1700);
    // 图片显示2
    // OLED_NewFrame();
    // OLED_DrawImage((128 - (bilibiliImg.w)) / 2, 0, &bilibiliImg, OLED_COLOR_NORMAL);
    // OLED_ShowFrame();
    // osDelay(3000);
  }
  /* USER CODE END StartOledTask */
}

/* USER CODE BEGIN Header_StartRgbTask */
/**
 * @brief Function implementing the rgbTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartRgbTask */
void StartRgbTask(void *argument)
{
  /* USER CODE BEGIN StartRgbTask */
  /* Infinite loop */
  for (;;)
  {
    // 渐变更新颜色，演示1次
    for (int i = 0; i < 1; i++)
    {
      ws2812_set_all(0xFFFFFF); // 纯白色
      ws2812_gradient(200, 3);
      osDelay(700);

      ws2812_set_all(0x20B2AA); // 浅海洋绿
      ws2812_gradient(200, 3);
      osDelay(700);

      ws2812_set_all(0xFFD700); // 金色
      ws2812_gradient(200, 3);
      osDelay(700);

      // 更多颜色：https://www.w3schools.com/colors/colors_picker.asp
    }

    // 彩虹效果，演示5次
    for (int i = 0; i < 5; i++)
    {
      rainbow_effect(255, 65);
    }
  }
  /* USER CODE END StartRgbTask */
}

/* USER CODE BEGIN Header_StartKeyTask */
/**
 * @brief Function implementing the keyTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartKeyTask */
void StartKeyTask(void *argument)
{
  /* USER CODE BEGIN StartKeyTask */
  /* Infinite loop */
  uint16_t cnt = 0;
  for (;;)
  {
    if (HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) == GPIO_PIN_RESET)
    {
      HAL_GPIO_TogglePin(BLUE_GPIO_Port, BLUE_Pin);
      while (HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) == GPIO_PIN_RESET)
      {
        osDelay(10);
      }
    }
    if (HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin) == GPIO_PIN_RESET)
    {
      HAL_GPIO_TogglePin(RED_GPIO_Port, RED_Pin);
      while (HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin) == GPIO_PIN_RESET)
      {
        osDelay(10);
      }
    }
    if (HAL_GPIO_ReadPin(KEY3_GPIO_Port, KEY3_Pin) == GPIO_PIN_RESET)
    {
      HAL_GPIO_TogglePin(GREEN_GPIO_Port, GREEN_Pin);
      while (HAL_GPIO_ReadPin(KEY3_GPIO_Port, KEY3_Pin) == GPIO_PIN_RESET)
      {
        osDelay(10);
      }
    }
    osDelay(100);
    if (++cnt > 10)
    {
      HAL_UART_Transmit_IT(&huart2, (uint8_t *)"Hello World!\r\n", 14);
      cnt = 0;
    }
  }
  /* USER CODE END StartKeyTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
