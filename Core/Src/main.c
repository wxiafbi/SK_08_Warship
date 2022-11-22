/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
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
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>

#define RXBUFFERSIZE 256     //最大接收字节数
char RxBuffer[RXBUFFERSIZE]; //接收数据
uint8_t aRxBuffer;           //接收中断缓冲
uint8_t Uart1_Rx_Cnt = 0;    //接收缓冲计数

#define U3_RXBUFFERSIZE 36
uint8_t USART3_RX_BUF[U3_RXBUFFERSIZE];
uint8_t sk08_aRxBuffer; //接收中断缓冲
char fina_data1[5];
int finaldata1;
uint8_t str1[4] = {0x80, 0x06, 0x02, 0x78};
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
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
    HAL_UART_Receive_IT(&huart1, (uint8_t *)&aRxBuffer, 1);
    HAL_UART_Receive_IT(&huart2, (uint8_t *)&sk08_aRxBuffer, 1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
        HAL_UART_Transmit(&huart2, (uint8_t *)&str1, 4, 0xffff);
        HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_5);
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
        HAL_Delay(2000);
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int _io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__*/

/******************************************************************
 *@brief  Retargets the C library printf  function to the USART.
 *@param  None
 *@retval None
 ******************************************************************/
PUTCHAR_PROTOTYPE
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
    return ch;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(huart);
    /* NOTE: This function Should not be modified, when the callback is needed,
             the HAL_UART_TxCpltCallback could be implemented in the user file
     */
    static uint8_t seri_count = 0;
    uint8_t check_sum         = 0;
    uint8_t i, x;

    static uint8_t uflag = 0;

    if (huart == &huart1) {
        if (Uart1_Rx_Cnt >= 255) //溢出判断
        {
            Uart1_Rx_Cnt = 0;
            memset(RxBuffer, 0x00, sizeof(RxBuffer));
            HAL_UART_Transmit(&huart1, (uint8_t *)"数据溢出", 10, 0xFFFF);
        } else {
            RxBuffer[Uart1_Rx_Cnt++] = aRxBuffer;                                             //接收数据转存
            if ((RxBuffer[Uart1_Rx_Cnt - 1] == 0x0A) && (RxBuffer[Uart1_Rx_Cnt - 2] == 0x0D)) //判断结束位
            {
                HAL_UART_Transmit(&huart1, (uint8_t *)&RxBuffer, Uart1_Rx_Cnt, 0xFFFF); //将收到的信息发送出去
                while (HAL_UART_GetState(&huart1) == HAL_UART_STATE_BUSY_TX)
                    ; //检测UART发送结束
                Uart1_Rx_Cnt = 0;
                memset(RxBuffer, 0x00, sizeof(RxBuffer)); //清空数组
            }
        }
        HAL_UART_Receive_IT(&huart1, (uint8_t *)&aRxBuffer, 1); //再开启接收中断
    }
    if (huart == &huart2) {
        if (sk08_aRxBuffer == 0x80) {
            uflag = 1;
        }
        if (uflag) {
            USART3_RX_BUF[seri_count++] = sk08_aRxBuffer;
            if (seri_count == 11) {
                if (USART3_RX_BUF[0] == 0x80) {
                    // beep_on_200ms();
                    for (i = 0; i < 11 - 1; i++) {
                        check_sum += USART3_RX_BUF[i];
                        printf(" 0x%x",USART3_RX_BUF[i]);
                    }
                    printf(" 0x%x",USART3_RX_BUF[10]);
                    check_sum = ~check_sum + 1;
                    if (check_sum == USART3_RX_BUF[10]) {
                        fina_data1[0] = USART3_RX_BUF[4]; //十位数
                        fina_data1[1] = USART3_RX_BUF[5]; //个位数
                        // data[6]是小数点.
                        fina_data1[2] = USART3_RX_BUF[7];      //小数点后一位
                        fina_data1[3] = USART3_RX_BUF[8];      //小数点后两位
                        fina_data1[4] = USART3_RX_BUF[9];      //小数点后三位
                        sscanf(fina_data1, "%d", &finaldata1); //字符串转int
                        // HAL_Delay(10);
                        printf("距离值=%dmm\r\n", finaldata1); // print用串口1，串口2用来和激光模块通讯
                        /* code */
                        for (x = 0; x < 6; x++) {
                            fina_data1[x] = 0;
                        }
                    }
                    seri_count = 0;
                    uflag      = 0;
                }
            }
        }
        HAL_UART_Receive_IT(&huart2, (uint8_t *)&sk08_aRxBuffer, 1);
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
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1) {
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
