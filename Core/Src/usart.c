/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
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
#include "usart.h"

/* USER CODE BEGIN 0 */
#define RXBUFFERSIZE 256     //�������ֽ���
char RxBuffer[RXBUFFERSIZE]; //��������
uint8_t aRxBuffer;           //�����жϻ���
uint8_t Uart1_Rx_Cnt = 0;    //���ջ������

#define U3_RXBUFFERSIZE 36
uint8_t USART3_RX_BUF[U3_RXBUFFERSIZE];
uint8_t sk08_aRxBuffer; //�����жϻ���
char fina_data1[5];
int finaldata1;
uint8_t str1[4] = {0x80, 0x06, 0x02, 0x78};    //���β�������
uint8_t str2[4] = {0x80, 0x06, 0x07, 0x75};    //��ȡ��������
/* USER CODE END 0 */

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 921600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}
/* USART2 init function */

void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 2);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspInit 0 */

  /* USER CODE END USART2_MspInit 0 */
    /* USART2 clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART2 interrupt Init */
    HAL_NVIC_SetPriority(USART2_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspInit 1 */

  /* USER CODE END USART2_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspDeInit 0 */

  /* USER CODE END USART2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();

    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2|GPIO_PIN_3);

    /* USART2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspDeInit 1 */

  /* USER CODE END USART2_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
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
        if (Uart1_Rx_Cnt >= 255) //����ж�
        {
            Uart1_Rx_Cnt = 0;
            memset(RxBuffer, 0x00, sizeof(RxBuffer));
            HAL_UART_Transmit(&huart1, (uint8_t *)"�������", 10, 0xFFFF);
        } else {
            RxBuffer[Uart1_Rx_Cnt++] = aRxBuffer;                                             //��������ת��
            if ((RxBuffer[Uart1_Rx_Cnt - 1] == 0x0A) && (RxBuffer[Uart1_Rx_Cnt - 2] == 0x0D)) //�жϽ���λ
            {
                HAL_UART_Transmit(&huart1, (uint8_t *)&RxBuffer, Uart1_Rx_Cnt, 0xFFFF); //���յ�����Ϣ���ͳ�ȥ
                while (HAL_UART_GetState(&huart1) == HAL_UART_STATE_BUSY_TX)
                    ; //���UART���ͽ���
                Uart1_Rx_Cnt = 0;
                memset(RxBuffer, 0x00, sizeof(RxBuffer)); //�������
            }
        }
        HAL_UART_Receive_IT(&huart1, (uint8_t *)&aRxBuffer, 1); //�ٿ��������ж�
    }
    if (huart == &huart2) {
        if (sk08_aRxBuffer == 0x80) {
            uflag = 1;
        }
        if (uflag) {
            USART3_RX_BUF[seri_count++] = sk08_aRxBuffer;
            if (seri_count == 11) {
                if (USART3_RX_BUF[0] == 0x80) {
                    for (i = 0; i < 11 - 1; i++) {
                        check_sum += USART3_RX_BUF[i];
                        printf(" 0x%x",USART3_RX_BUF[i]);
                    }
                    printf(" 0x%x",USART3_RX_BUF[10]);
                    check_sum = ~check_sum + 1;
                    if (check_sum == USART3_RX_BUF[10]) {
                        fina_data1[0] = USART3_RX_BUF[4]; //ʮλ��
                        fina_data1[1] = USART3_RX_BUF[5]; //��λ��
                        // data[6]��С����.
                        fina_data1[2] = USART3_RX_BUF[7];      //С�����һλ
                        fina_data1[3] = USART3_RX_BUF[8];      //С�������λ
                        fina_data1[4] = USART3_RX_BUF[9];      //С�������λ
                        sscanf(fina_data1, "%d", &finaldata1); //�ַ���תint
                        printf("����ֵ=%dmm\r\n", finaldata1); // print�ô���1������2�����ͼ���ģ��ͨѶ
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
/* USER CODE END 1 */
