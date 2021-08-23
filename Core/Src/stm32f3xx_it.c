/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f3xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f3xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

extern uint8_t* packetToProcess[PACKETSINBUF];
extern uint8_t packetsNum;
extern uint8_t packetIndex;

extern uint8_t dataBuffer1[BUFSIZE];
extern uint8_t dataBuffer2[BUFSIZE];

extern uint8_t* dataPointer1;
extern uint8_t* endPacketPointer1;
extern uint8_t* dataPointer2;
extern uint8_t* endPacketPointer2;

extern uint32_t ADCBuf[3];

extern float biasReadVal;
extern float katodeReadVal;

float vref;
uint16_t* vrefCal = (uint16_t*)VREFINTCAL_ADDR;

extern uint8_t dataSent;

extern uint8_t timeElapsed;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

void saveInCircBuf(uint8_t* buffer, uint8_t** startPckPtr, uint8_t** endPckPtr,
                  uint8_t** pckArray, uint8_t* pckIdx, uint8_t* pckNum, uint8_t uartDir);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_adc1;
extern DAC_HandleTypeDef hdac;
extern TIM_HandleTypeDef htim2;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F3xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f3xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA1 channel1 global interrupt.
  */
void DMA1_Channel1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel1_IRQn 0 */

  /* USER CODE END DMA1_Channel1_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_adc1);
  /* USER CODE BEGIN DMA1_Channel1_IRQn 1 */

  /* USER CODE END DMA1_Channel1_IRQn 1 */
}

/**
  * @brief This function handles TIM2 global interrupt.
  */
void TIM2_IRQHandler(void)
{
  /* USER CODE BEGIN TIM2_IRQn 0 */

  /* USER CODE END TIM2_IRQn 0 */
  HAL_TIM_IRQHandler(&htim2);
  /* USER CODE BEGIN TIM2_IRQn 1 */

  /* USER CODE END TIM2_IRQn 1 */
}

/**
  * @brief This function handles USART1 global interrupt / USART1 wake-up interrupt through EXTI line 25.
  */
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */

  uint8_t isRx = huart1.Instance->ISR & USART_ISR_RXNE;

  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */

  if(isRx){
      saveInCircBuf(dataBuffer1,&dataPointer1,&endPacketPointer1,
                    packetToProcess,&packetIndex,&packetsNum,UART1);

      HAL_UART_Receive_IT(&huart1, endPacketPointer1, 1);
  }

  /* USER CODE END USART1_IRQn 1 */
}

/**
  * @brief This function handles USART2 global interrupt / USART2 wake-up interrupt through EXTI line 26.
  */
void USART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */

  uint8_t isRx = huart2.Instance->ISR & USART_ISR_RXNE;

  /* USER CODE END USART2_IRQn 0 */
  HAL_UART_IRQHandler(&huart2);
  /* USER CODE BEGIN USART2_IRQn 1 */

  if(isRx){
      saveInCircBuf(dataBuffer2,&dataPointer2,&endPacketPointer2,
                    packetToProcess,&packetIndex,&packetsNum,UART2);

      HAL_UART_Receive_IT(&huart2, endPacketPointer2, 1);
  }

  /* USER CODE END USART2_IRQn 1 */
}

/**
  * @brief This function handles Timer 6 interrupt and DAC underrun interrupts.
  */
void TIM6_DAC_IRQHandler(void)
{
  /* USER CODE BEGIN TIM6_DAC_IRQn 0 */

  /* USER CODE END TIM6_DAC_IRQn 0 */
  HAL_DAC_IRQHandler(&hdac);
  /* USER CODE BEGIN TIM6_DAC_IRQn 1 */

  /* USER CODE END TIM6_DAC_IRQn 1 */
}

/* USER CODE BEGIN 1 */

void saveInCircBuf(uint8_t* buffer, uint8_t** startPckPtr, uint8_t** endPckPtr,
                   uint8_t** pckArray, uint8_t* pckIdx, uint8_t* pckNum, uint8_t uartDir){
    /*
     * Se l'ultimo carattere è un carattere terminatore di comando ('\r' oppure '\n') controllo l'indirizzo successivo:
     * nel caso in cui l'indirizzo successivo sia tale che aggiungendo un pacchetto della MASSIMA
     * larghezza si arriva alla fine del buffer, allora punto all'inizio del buffer.
     * Altrimenti continuo a riempire il buffer normalmente, tanto i pacchetti saranno di dimensioni
     * inferiori a MAXPACKETSIZE e quindi non arriverò alla fine!
     */

    **endPckPtr = (**endPckPtr >= 'a' && **endPckPtr <= 'z') ?
                   **endPckPtr-32 : **endPckPtr; // converto in maiuscole

    if(**endPckPtr == ';' || **endPckPtr == '\r' || **endPckPtr == '\n'){
        *(++(*endPckPtr)) = uartDir;

        pckArray[*pckIdx] = *startPckPtr;

        *pckIdx = (++(*pckIdx) < PACKETSINBUF) ?
                   *pckIdx : 0;

        (*pckNum)++;

        *startPckPtr = (++(*endPckPtr) < &buffer[BUFSIZE-MAXPACKETSIZE]) ?
                        *endPckPtr : buffer;

        *endPckPtr = *startPckPtr;
    }else{
        *endPckPtr = (++(*endPckPtr) < &buffer[BUFSIZE]) ?
                      *endPckPtr : buffer;
    }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
  UNUSED(hadc);

  if(hadc->Instance == ADC1){
      vref = ADCBuf[VREFINT];

      biasReadVal = *vrefCal*3.3*ADCBuf[BIAS]/(4095*vref);
      katodeReadVal = *vrefCal*3.3*ADCBuf[KATODE]/(4095*vref);
  }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  UNUSED(huart);

  dataSent = 1;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  UNUSED(htim);

  if(htim->Instance == TIM2){
      timeElapsed = 1;
  }

}
/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
