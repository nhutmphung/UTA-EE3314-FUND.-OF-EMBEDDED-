/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include <stdio.h>
#include <stdint.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

/* USER CODE BEGIN PV */

// ── PID Gains ── Tune these freely ──────────────────────────────────────────
#define KP  0.6f
#define KI  0.5f
#define KD  0.4f
// ─────────────────────────────────────────────────────────────────────────────

// ── Closed-loop plant states (with PID controller) ──────────────────────────
// G(s) = 100 / (s^3 + 8s^2 + 17s + 10)
volatile float x1 = 0.0f;
volatile float x2 = 0.0f;
volatile float x3 = 0.0f;
volatile float y  = 0.0f;   // closed-loop output

// ── Open-loop plant states (no controller, driven directly by u) ─────────────
volatile float ox1 = 0.0f;
volatile float ox2 = 0.0f;
volatile float ox3 = 0.0f;
volatile float oy  = 0.0f;  // open-loop output

// ── Reference input (square wave) ───────────────────────────────────────────
volatile float u = 1.0f;

// ── PID controller internal state ───────────────────────────────────────────
volatile float p       = 0.0f;  // controller output (plant input)
volatile float e       = 0.0f;  // e[n]
volatile float e_prev  = 0.0f;  // e[n-1]
volatile float e_prev2 = 0.0f;  // e[n-2]
volatile float p_prev  = 0.0f;  // p[n-1]

volatile uint8_t print_flag = 0;

// Integration step size @ 1 ms
const float dt = 0.001f;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
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
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  MX_GPIO_Init();
  MX_TIM6_Init();
  MX_TIM7_Init();
  MX_TIM10_Init();
  MX_USART2_UART_Init();

  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim10);
  HAL_TIM_Base_Start_IT(&htim6);
  HAL_TIM_Base_Start_IT(&htim7);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      if (print_flag == 1)
      {
          print_flag = 0;

          // Channel 0 = step input (u)
          // Channel 1 = closed-loop output (with controller)
          // Channel 2 = open-loop output (no controller)
          printf("$%.1f %.4f %.4f;\r\n", u, y, oy);
          fflush(stdout);
      }
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

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState            = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM            = 16;
  RCC_OscInitStruct.PLL.PLLN            = 336;
  RCC_OscInitStruct.PLL.PLLP            = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ            = 2;
  RCC_OscInitStruct.PLL.PLLR            = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                   | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

int _write(int file, char *ptr, int len)
{
  extern UART_HandleTypeDef huart2;
  HAL_UART_Transmit(&huart2, (uint8_t *)ptr, len, HAL_MAX_DELAY);
  return len;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    // ── TIM10: 1 ms simulation time base ────────────────────────────────────
    if (htim->Instance == TIM10)
    {
        // ── CLOSED-LOOP (with PID controller) ───────────────────────────────

        // Step 1 — Compute error: e[n] = u[n] - y[n-1]
        e = u - y;

        // Step 2 — PID difference equation (backward Euler):
        //   p[n] = p[n-1]
        //        + (Kd/dt) * (e[n] - 2*e[n-1] + e[n-2])   <- derivative term
        //        + Kp       * (e[n] - e[n-1])               <- proportional term
        //        + Ki*dt    *  e[n]                          <- integral term
        p = p_prev
            + (KD / dt) * (e - 2.0f * e_prev + e_prev2)
            + KP        * (e - e_prev)
            + KI * dt   *  e;

        // Step 3 — Integrate closed-loop plant states (driven by p)
        float dx1 = x2;
        float dx2 = x3;
        float dx3 = p - 10.0f * x1 - 17.0f * x2 - 8.0f * x3;

        x1 += dx1 * dt;
        x2 += dx2 * dt;
        x3 += dx3 * dt;

        // Step 4 — Closed-loop plant output
        y = 100.0f * x1;

        // Step 5 — Shift PID history
        e_prev2 = e_prev;
        e_prev  = e;
        p_prev  = p;

        // ── OPEN-LOOP (no controller, driven directly by u) ──────────────────

        // Integrate open-loop plant states (driven by u)
        float odx1 = ox2;
        float odx2 = ox3;
        float odx3 = u - 10.0f * ox1 - 17.0f * ox2 - 8.0f * ox3;

        ox1 += odx1 * dt;
        ox2 += odx2 * dt;
        ox3 += odx3 * dt;

        // Open-loop plant output
        oy = (100.0f * ox1) / 10.0f;
    }

    // ── TIM6: Square wave reference command (every 5 seconds) ───────────────
    else if (htim->Instance == TIM6)
    {
        if (u > 0.5f) {
            u = 0.0f;
        } else {
            u = 1.0f;
        }

        // Reset open-loop states on every toggle for a clean step response
        ox1 = 0.0f;
        ox2 = 0.0f;
        ox3 = 0.0f;
        oy  = 0.0f;
    }

    // ── TIM7: Data logging trigger (every 10 ms) ─────────────────────────────
    else if (htim->Instance == TIM7)
    {
        print_flag = 1;
    }
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
