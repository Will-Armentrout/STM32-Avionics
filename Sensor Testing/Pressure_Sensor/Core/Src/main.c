/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_I2C1_Init(void);
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
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  /* Pressure Sensor Loop Variables*/
  	  /* Slave Address of the BMP280 */
  	  uint16_t BMP_Address = 0xEC; /* BMP 280 Address */

  	  /* Register Addresses */
  	  uint8_t regAddress_CTRL_MEAS = 0xF4; /* Oversampling control */
  	  uint8_t regAddress_CONFIG = 0xF5; /* Data rate/filter and interface */
  	  uint8_t regAddress_PRESS_MSB = 0xF7; /* Pressure Most Significant Bits */

  	  /* Trimming Register Addresses */
  	  uint8_t regAddress_Dig_T1 = 0x88;
  	  uint8_t regAddress_Dig_T2 = 0x8A;
  	  uint8_t regAddress_Dig_T3 = 0x8C;
	  uint8_t regAddress_Dig_P1 = 0x8E;
	  uint8_t regAddress_Dig_P2 = 0x90;
	  uint8_t regAddress_Dig_P3 = 0x92;
	  uint8_t regAddress_Dig_P4 = 0x94;
	  uint8_t regAddress_Dig_P5 = 0x96;
	  uint8_t regAddress_Dig_P6 = 0x98;
	  uint8_t regAddress_Dig_P7 = 0x9A;
	  uint8_t regAddress_Dig_P8 = 0x9C;
	  uint8_t regAddress_Dig_P9 = 0x9E;


  	  /* Register Values */
  	  uint8_t regCTRL_MEAS = 0x27; /* Oversampling x1 for Pressure and Temp, Normal Mode 00100111*/
  	  uint8_t regCONFIG = 0x00; /* Not sure about this. 0.5ms rate, filter off, SPI off */


  	  /* Output Buffers */
  	  uint8_t data[6]; /* Buffer for all the data */

  	  int32_t press; /* Pressure value */
  	  int32_t temp; /* Temperature Value */
  	  uint16_t dig_T1; /* Trimming Value T1 */
  	  short dig_T2; /* Trimming Value T2 */
  	  short dig_T3; /* Trimming Value T3 */
  	  uint16_t dig_P1; /*Trimming Value P1 */
  	  short dig_P2; /* Trimming Value P2 */
  	  short dig_P3; /* Trimming Value P3 */
  	  short dig_P4; /* Trimming Value P4 */
  	  short dig_P5; /* Trimming Value P5 */
  	  short dig_P6; /* Trimming Value P6 */
  	  short dig_P7; /* Trimming Value P7 */
  	  short dig_P8; /* Trimming Value P8 */
  	  short dig_P9; /* Trimming Value P9 */

  	  /* Temperature Compensation Variables */
	  int32_t temp_var1;
	  int32_t temp_var2;
	  int32_t temperature; /* Output Temperature */
	  int32_t t_fine; /* Fine Output for Pressure Compensation */

	  /* Pressure Compensation Variables */
	  int64_t press_var1;
	  int64_t press_var2;
	  int64_t pressure;

  	  float pressVal; /* Decimal Pressure */
  	  float tempVal; /* Decimal Temperature */
  	  float altitude; /* Altitude */
  	  float seaLevel_Press = 102090; /* Sea Level Pressure Equivalent (Update by day) */

  	  /* Configuration of the BMP280 */
  	  HAL_I2C_Mem_Write(&hi2c1, BMP_Address, regAddress_CTRL_MEAS, I2C_MEMADD_SIZE_8BIT, &regCTRL_MEAS, 1, 2000u); /* Writing to the ctrl measurement register */
  	  HAL_I2C_Mem_Write(&hi2c1, BMP_Address, regAddress_CONFIG, I2C_MEMADD_SIZE_8BIT, &regCONFIG, 1, 2000u); /* Writing to the ctrl measurement register */

  	  /* Reading the Trimming Values */
	  HAL_I2C_Mem_Read(&hi2c1, BMP_Address, regAddress_Dig_T1, I2C_MEMADD_SIZE_8BIT, &dig_T1, 2, 2000u); /* Getting the Trimming Values */
	  HAL_I2C_Mem_Read(&hi2c1, BMP_Address, regAddress_Dig_T2, I2C_MEMADD_SIZE_8BIT, &dig_T2, 2, 2000u); /* Getting the Trimming Values */
	  HAL_I2C_Mem_Read(&hi2c1, BMP_Address, regAddress_Dig_T3, I2C_MEMADD_SIZE_8BIT, &dig_T3, 2, 2000u); /* Getting the Trimming Values */
	  HAL_I2C_Mem_Read(&hi2c1, BMP_Address, regAddress_Dig_P1, I2C_MEMADD_SIZE_8BIT, &dig_P1, 2, 2000u); /* Getting the Trimming Values */
	  HAL_I2C_Mem_Read(&hi2c1, BMP_Address, regAddress_Dig_P2, I2C_MEMADD_SIZE_8BIT, &dig_P2, 2, 2000u); /* Getting the Trimming Values */
	  HAL_I2C_Mem_Read(&hi2c1, BMP_Address, regAddress_Dig_P3, I2C_MEMADD_SIZE_8BIT, &dig_P3, 2, 2000u); /* Getting the Trimming Values */
	  HAL_I2C_Mem_Read(&hi2c1, BMP_Address, regAddress_Dig_P4, I2C_MEMADD_SIZE_8BIT, &dig_P4, 2, 2000u); /* Getting the Trimming Values */
	  HAL_I2C_Mem_Read(&hi2c1, BMP_Address, regAddress_Dig_P5, I2C_MEMADD_SIZE_8BIT, &dig_P5, 2, 2000u); /* Getting the Trimming Values */
	  HAL_I2C_Mem_Read(&hi2c1, BMP_Address, regAddress_Dig_P6, I2C_MEMADD_SIZE_8BIT, &dig_P6, 2, 2000u); /* Getting the Trimming Values */
	  HAL_I2C_Mem_Read(&hi2c1, BMP_Address, regAddress_Dig_P7, I2C_MEMADD_SIZE_8BIT, &dig_P7, 2, 2000u); /* Getting the Trimming Values */
	  HAL_I2C_Mem_Read(&hi2c1, BMP_Address, regAddress_Dig_P8, I2C_MEMADD_SIZE_8BIT, &dig_P8, 2, 2000u); /* Getting the Trimming Values */
	  HAL_I2C_Mem_Read(&hi2c1, BMP_Address, regAddress_Dig_P9, I2C_MEMADD_SIZE_8BIT, &dig_P9, 2, 2000u); /* Getting the Trimming Values */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  HAL_I2C_Mem_Read(&hi2c1, BMP_Address, regAddress_PRESS_MSB, I2C_MEMADD_SIZE_8BIT, &data, 6, 2000u); /* Reading pressure and temperature */

	  press = (int32_t)((data[0] << 12) | data[1] << 4 | data[2]);
	  temp = (int32_t)((data[3] << 12) | data[4] << 4 | data[5]);

	  /* Compensation of Temperature */
	  temp_var1 = ((((temp>>3) - ((int32_t)dig_T1<<1))) * ((int32_t)dig_T2)) >> 11;
	  //var2 = (((((temp >> 17) - ((int32_t)dig_T1 >> 13)) * ((temp >> 17) - ((int32_t)dig_T1))) >> 13) * ((int32_t)dig_T3));
	  temp_var2 = (((((temp>>4) - ((int32_t)dig_T1)) * ((temp>>4) - ((int32_t)dig_T1))) >> 12) * ((int32_t)dig_T3)) >> 14;
	  t_fine = temp_var1 + temp_var2;
	  temperature = (t_fine * 5 + 128) >> 8;

	  tempVal = ((float)temperature)/100;

	  /* Compensation of Pressure */
	  press_var1 = ((int64_t)t_fine) - 128000;
	  press_var2 = press_var1 * press_var1 * (int64_t)dig_P6;
	  press_var2 = press_var2 + ((press_var1*(int64_t)dig_P5)<<17);
	  press_var2 = 	  press_var2 + (((int64_t)dig_P4)<<35);
	  press_var1 = ((press_var1 * press_var1 * (int64_t)dig_P3)>>8) + ((press_var1 * (int64_t)dig_P2)<<12);
	  press_var1 = (((((int64_t)1)<<47)+press_var1))*((int64_t)dig_P1)>>33;
	  pressure = 1048576 - press;
	  pressure = (((pressure<<31)-press_var2)*3125)/press_var1;
	  press_var1 = (((int64_t)dig_P9) * (pressure>>13) * (pressure>>13)) >> 25;
	  press_var2 = (((int64_t)dig_P8) * pressure) >> 19;
	  pressure = ((pressure + press_var1 + press_var2) >> 8) + (((int64_t)dig_P7)<<4);

	  pressVal = ((float)pressure)/256;

	  altitude = 44330 * (1 - pow((pressVal/seaLevel_Press),(1/5.255)));

	  HAL_Delay(2000); /* Delay for 2 seconds */
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_I2C1;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x0010020A;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 38400;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
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
  __disable_irq();
  while (1)
  {
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
