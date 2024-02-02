/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include  "led.h"
#include  "key.h"
#include  "lcd_drive.h"
#include  "gui.h"
#include  "tftDisp.h"
#include  "w25q64.h"
#include  "touch.h"
#include  "GUI.h"
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
static void MX_NVIC_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// 当存满后，触发 "DMA2_Stream0_IRQHandler" 中断， 调用"ADC_DMAConvCplt"见下行,再调用"HAL_ADC_ConvCpltCallback" 进行数据处理
// 在"HAL_ADC_Start_DMA"中设置DMA完成后执行"hadc->DMA_Handle->XferCpltCallback = ADC_DMAConvCplt;"

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
    struKEY     key;
    struTouch   touch={0,0,NO_TOUCH,X_NO_MOVING,Y_NO_MOVING,0,0,0}; // 电容触摸屏的参数
    int         digital_a, digital_b;
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
//  MX_DMA_Init();
  MX_TIM7_Init();
  MX_FSMC_Init();
  MX_SPI2_Init();
//  MX_USART1_UART_Init();
//  MX_ADC1_Init();
//  MX_DAC_Init();
  MX_TIM2_Init();

  /* Initialize interrupts */
  MX_NVIC_Init();
  /* USER CODE BEGIN 2 */
  
    W25Q64_CS_GPIO_Init();    // W25Q64.CS引脚初始化
    FT6336_I2C_GPIO_Init();   // LCD屏的触摸屏的2个引脚初始化
    LCD_Initialize();         // LCD屏的复位引脚，背光引脚 初始化
    Gui_Draw_Asc24_Hzk24(0,0, RED_4_4, BLUE_1_4,(uint8_t*)" ");

    KEY_Param_Init(&key);     // 
    Gui_Draw_Asc24_Hzk24(10,10, RED_4_4, WHITE_2_4,(uint8_t*)"slkjhjh");
    
    LED_GPIO_Init();
    KEY_GPIO_Init();
    HAL_TIM_Base_Start_IT(&htim7);
    
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
    HAL_TIM_Base_Start_IT(&htim2);
    
    digital_a = 15;
    digital_b = 20;
    
    Disp_Digital(digital_a, 100, 50);
    Disp_Digital(digital_b, 200, 50);
    // 设置串口1每接收到 1 个字节的数据保存在U1_Rx_aByte内, 并触发执行串口1接收中断回调函数
    HAL_UART_Receive_IT(&huart1, U1_Rx_Buf, 3); 
    U1_Point = 0;
    U1_Rx_Success = ERROR;

//    ///////////////////////////////////////////////////////
//    // TIM2的CC2匹配事件触发ADC, PA1输出脉冲占空比 = PULSE/(PERIOD+1)*100% 
//    // 理论ADC最快(TIM2) 2.4MHz的溢出频率
//    // DAC输出Buffer关闭，输出速度会快些,但是带负载能力较差，要外接负载高阻信号才不变形;
//    // DAC输出Buffer打开，输出速度会慢些,但是带负载能力较强;
//    // 设置ADC1_IN10经触发进行ADC, 数据通过DMA2_stream0 通道传输到 ADC1_Value
//    HAL_ADC_Start_DMA(&hadc1, &ADC1_Value, 1); 
//    
//    // 设置TIM2的OC2REF或者UPDATE触发进行DAC, 数据ADC1_Value 通过DMA1_stream5 通道传输到 DAC1进行D/A
//    HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, &ADC1_Value, 1, DAC_ALIGN_12B_R); // 
//    
//    // 启动TIM2计数，并且开启CH2的PWM模式; CH2触发ADC,并且CH2(PA1)输出PWM信号;
//    // TIM2的OC2REF或者UPDATE又触发DAC,在DAC1引脚输出模拟量
//    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);   
    Gui_Draw_Rect(50,100, 400, 300, WHITE_4_4);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
        if(Read_Key_En == ENABLE)
        {
            Read_Key_En = DISABLE;
            Key_Read(&key);
            Key_Process(&key, digital_a, digital_b); 
            
            ReadCTP(&touch);         // 读取触摸屏的触摸坐标及状态
            Touch_Disp_XY(&touch);   // 显示xy坐标
            Touch_Process(&touch);   // 触摸屏的处理
        }
//        
        if(U1_Rx_Success == SUCCESS) // 串口1成功接收一个（帧）数据
        {
            U1_Rx_Success = ERROR;
            // U1_Process(U1_Rx_aByte); // 串口1成功接收后的数据的处理
            digital_a = U1_Rx_Buf[0];
            digital_b = U1_Rx_Buf[1];
            Disp_Digital(digital_a, 100, 50);
            Disp_Digital(digital_b, 200, 50);
            
            digital_a = U1_Rx_Buf[2];
            Disp_Digital(digital_a, 300, 50);
            
            HAL_UART_Receive_IT(&huart1, U1_Rx_Buf, 3); // 每次接收完成都要再次设置串口1
        }

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief NVIC Configuration.
  * @retval None
  */
static void MX_NVIC_Init(void)
{
  /* USART1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USART1_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
  /* TIM2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(TIM2_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(TIM2_IRQn);
  /* TIM7_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(TIM7_IRQn, 7, 0);
  HAL_NVIC_EnableIRQ(TIM7_IRQn);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
