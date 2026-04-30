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
#include "adc.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>
#include "LIB_MPU6500_SPI.h"
#include "stdbool.h"
#include "LIB_Motores.h"
#include <math.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

typedef struct
{
	uint32_t debugEncA;
	uint32_t debugEncB;
	uint32_t debugTiempo;
	uint16_t debugPWM;
	float 	 Ax;
	float 	 Ay;
	float 	 Az;
	float 	 Gx;
	float 	 Gy;
	float 	 Gz;


}Debug_Datos;

Debug_Datos debugDatos[200];
Motores_Init Motor;
MPU6500_Init_Values_t 	MPU6500_Datos; //Iniciamos donde se guardaran todos los datos a leer
MPU6500_status_e	MPU6500_Status;
MPU6500_Init_float_t	MPU6500_Conv;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/*
 * Variables para los encoders
 * La relacion de engranajes es de 38/24=1.5833 para los pulsos por vuelta 4096*1.5833=6485
 */
const int8_t estadoTabla[16]={0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0}; //valor encoders de tabla de verdad
uint8_t estadoAnterior_L=0;
uint8_t estadoAnterior_R=0;
int32_t ticksD=0;
int32_t ticksI=0;
int32_t contD=0;
int32_t contI=0;


char bufferTxt[30];

uint32_t ADC_Sensores[4];
uint32_t ADC_IR[4];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

void Inicializar_Sistema();
void MPU6500();
uint32_t ADC_Read_Manual(ADC_HandleTypeDef *hadc, uint32_t channel);
void DEBUG_Encoder();
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
  MX_DMA_Init();
  MX_SPI3_Init();
  MX_USART3_UART_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
  Inicializar_Sistema();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	/*  HAL_Delay(2000);

	  uint32_t tiempo_rampa=1500;
uint32_t tiempo=HAL_GetTick();
uint32_t tiempo_10ms=0;
uint16_t pwmRampa=0;
uint16_t k=2;
uint16_t muestras=0;
Motor.ENABLE=true;
while((HAL_GetTick()-tiempo)<tiempo_rampa)
{
	if((HAL_GetTick()-tiempo_10ms)>10){

		if ((HAL_GetTick()-tiempo)<(0.2*tiempo_rampa))
		{*/
			/*
			 * pasos =0.2*tiempo_rampa/dt(10ms)=60pasos
			 * k=(pwmObjetivo-pwmActual)/pasos
			 * k=3.3
			 * pwm=pwm+k
			 */
		/*	pwmRampa=pwmRampa+k;
		}
		else if((HAL_GetTick()-tiempo)>(0.8*tiempo_rampa))
		{
			pwmRampa=pwmRampa-k;
		}
		else{
			pwmRampa=pwmRampa;
		}
		Motor.PWM_ML=pwmRampa;
		Motor.PWM_MR=pwmRampa;
		PWM_Motores(&Motor);
		debugDatos[muestras].debugTiempo=HAL_GetTick();
		debugDatos[muestras].debugEncA=ticksD;
		debugDatos[muestras].debugEncB=ticksI;
		debugDatos[muestras].debugPWM=pwmRampa;
		MPU6500();
		debugDatos[muestras].Ax=MPU6500_Conv.MPU6500_floatAX;
		debugDatos[muestras].Ay=MPU6500_Conv.MPU6500_floatAY;
		debugDatos[muestras].Az=MPU6500_Conv.MPU6500_floatAZ;
		debugDatos[muestras].Gx=MPU6500_Conv.MPU6500_floatGX;
		debugDatos[muestras].Gy=MPU6500_Conv.MPU6500_floatGY;
		debugDatos[muestras].Gz=MPU6500_Conv.MPU6500_floatGZ;

		muestras++;
		tiempo_10ms=HAL_GetTick();
	}
}
HAL_Delay(2000);
HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, 1);
uint16_t adcPul=0;

while(1)
{
	adcPul=(uint16_t)ADC_Read_Manual(&hadc1, 7);
		if(adcPul<4065 && adcPul>4000)
		{
			break;
		}
}
for (int var = 0; var < muestras; ++var) {
	sprintf(bufferTxt,"%ld,%d,%0.2f,%0.2f,%0.2f",debugDatos[var].debugTiempo, debugDatos[var].debugPWM,debugDatos[var].Ax,debugDatos[var].Ay,debugDatos[var].Az);
	HAL_UART_Transmit(&huart3, (uint8_t *)bufferTxt, strlen(bufferTxt), HAL_MAX_DELAY);
	sprintf(bufferTxt,",%0.2f,%0.2f,%0.2f\n",debugDatos[var].Gx,debugDatos[var].Gy,debugDatos[var].Gz);
		HAL_UART_Transmit(&huart3, (uint8_t *)bufferTxt, strlen(bufferTxt), HAL_MAX_DELAY);

HAL_Delay(10);

}
HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, 0);

ticksD=0;
ticksI=0;
for (int var = 0; var < muestras; ++var) {
	debugDatos[var].debugTiempo=0;
	debugDatos[var].debugEncA=0;
	debugDatos[var].debugEncB=0;
	debugDatos[var].debugPWM=0;

}
*/

/*

	  uint16_t val_B=0;
	  uint16_t val_C=0;

	  uint16_t val_A=0;
	  uint16_t val_D=0;

	  //irA
	  HAL_GPIO_WritePin(IR1_TX_GPIO_Port, IR1_TX_Pin, GPIO_PIN_SET);
	  HAL_Delay(1);
	  val_A=(uint16_t)ADC_Read_Manual(&hadc2, 3);
	  HAL_GPIO_WritePin(IR1_TX_GPIO_Port, IR1_TX_Pin, GPIO_PIN_RESET);
	  HAL_Delay(50);
	  //irD
	  HAL_GPIO_WritePin(IR4_TX_GPIO_Port, IR4_TX_Pin, GPIO_PIN_SET);
	  HAL_Delay(1);
	  val_D=(uint16_t)ADC_Read_Manual(&hadc2, 0);
	  HAL_GPIO_WritePin(IR4_TX_GPIO_Port, IR4_TX_Pin, GPIO_PIN_RESET);
	  HAL_Delay(50);
	  //irB
	  HAL_GPIO_WritePin(IR2_TX_GPIO_Port, IR2_TX_Pin, GPIO_PIN_SET);
	  HAL_Delay(1);
	  val_B=(uint16_t)ADC_Read_Manual(&hadc2, 2);
	  HAL_GPIO_WritePin(IR2_TX_GPIO_Port, IR2_TX_Pin, GPIO_PIN_RESET);
	  HAL_Delay(50);

	  //irC
	  HAL_GPIO_WritePin(IR3_TX_GPIO_Port, IR3_TX_Pin, GPIO_PIN_SET);
	  HAL_Delay(1);
	  val_C=(uint16_t)ADC_Read_Manual(&hadc2, 1);
	  HAL_GPIO_WritePin(IR3_TX_GPIO_Port, IR3_TX_Pin, GPIO_PIN_RESET);
	  HAL_Delay(50);

*/
/*

	  	sprintf(bufferTxt,"%d,%d\n",val_B,val_C);
	  		  	HAL_UART_Transmit(&huart3, (uint8_t *)bufferTxt, strlen(bufferTxt), HAL_MAX_DELAY);
	  	HAL_Delay(100);*/
/*

	  	float val=2676.42*powf((float)val_A,-0.3902)-70.3445;
	  	val+=1;
	  	sprintf(bufferTxt,"A=%0.3f",val);
	  	HAL_UART_Transmit(&huart3, (uint8_t *)bufferTxt, strlen(bufferTxt), HAL_MAX_DELAY);


	  	//val=76032.88*powf((float)val_B,-1.0849);
	  	val=20560.59*powf((float)val_B,-0.8696)+10.7348;
	  	sprintf(bufferTxt,"	 B=%0.3f",val);
	  	HAL_UART_Transmit(&huart3, (uint8_t *)bufferTxt, strlen(bufferTxt), HAL_MAX_DELAY);


	  	//val=134070.5663*powf((float)val_C,-1.1064);
	  	val=31275.7*powf((float)val_C,-0.8879)+6.8792;
	  	sprintf(bufferTxt,"	 C=%0.3f",val);
	  	HAL_UART_Transmit(&huart3, (uint8_t *)bufferTxt, strlen(bufferTxt), HAL_MAX_DELAY);


	  	//val=8419.137*powf((float)val_D,-0.65345);
	  	val=4738.58*powf((float)val_D,-0.5362)-22.3536;
	  	sprintf(bufferTxt,"	 D=%0.3f\r\n",val);
	  	HAL_UART_Transmit(&huart3, (uint8_t *)bufferTxt, strlen(bufferTxt), HAL_MAX_DELAY);
	  	HAL_Delay(500);*/





	for (int var = 0; var < 4; ++var) {

	  	sprintf(bufferTxt,"A%d=%ld, ",var,ADC_IR[var]);
	  	HAL_UART_Transmit(&huart3, (uint8_t *)bufferTxt, strlen(bufferTxt), HAL_MAX_DELAY);
	}

  	sprintf(bufferTxt,"\r\n");
	  	HAL_UART_Transmit(&huart3, (uint8_t *)bufferTxt, strlen(bufferTxt), HAL_MAX_DELAY);
HAL_Delay(500);


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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
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

  /** Initializes the CPU, AHB and APB buses clocks
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

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

	if(GPIO_Pin==Enc_D_A_Pin||GPIO_Pin==Enc_D_B_Pin)
		{
		uint8_t bitStatusL=((HAL_GPIO_ReadPin(Enc_D_A_GPIO_Port, Enc_D_A_Pin))?2:0) | ((HAL_GPIO_ReadPin(Enc_D_B_GPIO_Port, Enc_D_B_Pin))?1:0);
		ticksD+=(-estadoTabla[((estadoAnterior_L<<2)|bitStatusL)]);
		estadoAnterior_L=bitStatusL;
		}
	if(GPIO_Pin==Enc_I_A_Pin||GPIO_Pin==Enc_I_B_Pin)
		{
		uint8_t bitStatusR=((HAL_GPIO_ReadPin(Enc_I_A_GPIO_Port, Enc_I_A_Pin))?2:0) | ((HAL_GPIO_ReadPin(Enc_I_B_GPIO_Port, Enc_I_B_Pin))?1:0);
		ticksI+=(estadoTabla[((estadoAnterior_R<<2)|bitStatusR)]);
		estadoAnterior_R=bitStatusR;
		}
	if(GPIO_Pin==Enc_D_C_Pin)
	{
		contD++;
	}
	if(GPIO_Pin==Enc_I_C_Pin)
		{
			contI++;
		}
}

void Inicializar_Sistema()
{
	/*
	 * Inicializar PWM Timer 1 para motores
	 */
	Inicializar_Motores(&Motor);
	Inicializar_Motor_Coreless();

	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, 0);
	HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, 0);
	HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, 0);
	HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, 0);

	HAL_GPIO_WritePin(IR1_TX_GPIO_Port, IR1_TX_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(IR2_TX_GPIO_Port, IR2_TX_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(IR3_TX_GPIO_Port, IR3_TX_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(IR4_TX_GPIO_Port, IR4_TX_Pin, GPIO_PIN_RESET);


	HAL_GPIO_WritePin(LED1_GPIO_Port	, LED1_Pin,GPIO_PIN_SET);
	HAL_Delay(1000);
	MPU6500_Status=MPU6500_Init(&MPU6500_Datos,50,DPS500,G2);
	if (MPU6500_Status==MPU6500_fail) {
		for (;;) {
			sprintf(bufferTxt,"Fallo al iniciar MPU\r\n");
			HAL_UART_Transmit(&huart3, (uint8_t *)bufferTxt, strlen(bufferTxt), HAL_MAX_DELAY);
			HAL_GPIO_TogglePin(LED4_GPIO_Port, LED4_Pin);
			HAL_Delay(500);
		}
	}
	sprintf(bufferTxt," Exito al iniciar MPU\r\n");
	HAL_UART_Transmit(&huart3, (uint8_t *)bufferTxt, strlen(bufferTxt), HAL_MAX_DELAY);
	HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_RESET);

HAL_ADC_Start_DMA(&hadc2, ADC_Sensores, 4);
	HAL_TIM_Base_Start_IT(&htim4);

}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM4) {

    	static volatile uint16_t Timer_20ms=0;

    	static uint8_t paso = 0;

    	if(Timer_20ms++>334)
    	{
    		//ADC2->CR2|=ADC_CR2_SWSTART;
    		//LED1_GPIO_Port->ODR^=LED1_Pin;
    		Timer_20ms=0;
    	}
       	GPIOC->BSRR=(uint32_t)(IR1_TX_Pin|IR2_TX_Pin|IR3_TX_Pin|IR4_TX_Pin)<<16;



        IR1_TX_GPIO_Port->BSRR = (uint32_t)IR1_TX_Pin;

        for (int var = 0; var < 200; ++var) {
			__NOP();
		}
       	// 1. Asegurar que el ADC esté encendido
       	if (!(ADC1->CR2 & ADC_CR2_ADON)) ADC1->CR2 |= ADC_CR2_ADON;

       	// 2. Limpiar banderas y disparar
       	ADC1->SR = 0;
       	ADC1->CR2 |= ADC_CR2_SWSTART;

       	// 3. Verificar si el ADC aceptó el disparo
       	// Si el reloj del ADC es incorrecto, SWSTART se queda en 0
       	while(!(ADC1->SR & ADC_SR_EOC));

       	ADC_IR[0] = ADC1->DR;
        IR1_TX_GPIO_Port->BSRR = (uint32_t)IR1_TX_Pin<<16;
 /*
 *
 * void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM4) {
        static uint8_t canal = 0; // Para saber qué pulso dar
        static uint16_t resultados_adc[4];

        // 1. APAGAR PULSOS ANTERIORES (PC0-PC3)
        GPIOC->BSRR = 0x000F0000;

        // 2. ACTIVAR PULSO DEL CANAL ACTUAL
        GPIOC->BSRR = (1 << canal);

        // 3. PEQUEÑA ESPERA DE ESTABILIZACIÓN (Opcional)
        // A 168MHz, 20 NOPs son aprox 120 nanosegundos
        __NOP(); __NOP(); __NOP(); __NOP();

        // 4. DISPARAR CONVERSIÓN
        // Al estar en SCAN, el ADC sabe qué Rank (canal) sigue
        ADC1->CR2 |= ADC_CR2_SWSTART;

        // 5. ESPERAR FINALIZACIÓN (Polleo del bit EOC)
        // A 168MHz esto es casi instantáneo
        while(!(ADC1->SR & ADC_SR_EOC));

        // 6. LEER Y GUARDAR EL DATO
        resultados_adc[canal] = ADC1->DR;

        // 7. BAJAR EL PULSO (Opcional si quieres pulso corto)
        // GPIOC->BSRR = (1 << canal) << 16;

        // 8. PREPARAR SIGUIENTE CANAL
        canal++;
        if (canal >= 4) {
            canal = 0; // El ADC también volverá al Rank 1 automáticamente
        }

        // --- AQUÍ PUEDES METER TU LÓGICA DE LOS 20ms PARA EL ADC2 ---
    }
}
 *
 */



    }
}

uint32_t ADC_Read_Manual(ADC_HandleTypeDef *hadc, uint32_t channel) {
    ADC_ChannelConfTypeDef sConfig = {0};
    uint32_t result = 0;

    // 1. Configurar el canal específico en la posición 1
    sConfig.Channel = channel;
    sConfig.Rank = 1;
    // Aumenta el tiempo de muestreo si lees 0 o valores inestables
    sConfig.SamplingTime = ADC_SAMPLETIME_56CYCLES;

    // 2. Aplicar configuración
    if (HAL_ADC_ConfigChannel(hadc, &sConfig) != HAL_OK) {
        return 0; // Error de configuración
    }

    // 3. Ciclo de conversión
    HAL_ADC_Start(hadc);

    // Esperar con un timeout prudente (10ms es suficiente)
    if (HAL_ADC_PollForConversion(hadc, 10) == HAL_OK) {
        result = HAL_ADC_GetValue(hadc);
    }

    // 4. Parar el ADC para liberar el secuenciador
    HAL_ADC_Stop(hadc);

    return result;
}

void MPU6500()
{
	MPU6500_Read(&MPU6500_Datos);
	MPU6500_Conv=MPU6500_Converter(&MPU6500_Datos);
/*
	sprintf(bufferTxt," Gx= %.2f ",MPU6500_Conv.MPU6500_floatGX);
	HAL_UART_Transmit(&huart3, (uint8_t *)bufferTxt, strlen(bufferTxt), HAL_MAX_DELAY);

	sprintf(bufferTxt," Gy= %.2f ",MPU6500_Conv.MPU6500_floatGY);
	HAL_UART_Transmit(&huart3, (uint8_t *)bufferTxt, strlen(bufferTxt), HAL_MAX_DELAY);

	sprintf(bufferTxt," Gz= %.2f ",MPU6500_Conv.MPU6500_floatGZ);
	HAL_UART_Transmit(&huart3, (uint8_t *)bufferTxt, strlen(bufferTxt), HAL_MAX_DELAY);

	sprintf(bufferTxt," Ax= %.2f ",MPU6500_Conv.MPU6500_floatAX);
	HAL_UART_Transmit(&huart3, (uint8_t *)bufferTxt, strlen(bufferTxt), HAL_MAX_DELAY);

	sprintf(bufferTxt," Ay= %.2f ",MPU6500_Conv.MPU6500_floatAY);
	HAL_UART_Transmit(&huart3, (uint8_t *)bufferTxt, strlen(bufferTxt), HAL_MAX_DELAY);

	sprintf(bufferTxt," Az= %.2f \r\n",MPU6500_Conv.MPU6500_floatAZ);
	HAL_UART_Transmit(&huart3, (uint8_t *)bufferTxt, strlen(bufferTxt), HAL_MAX_DELAY);

	HAL_Delay(200);*/
}
void DEBUG_Encoder()
{
	sprintf(bufferTxt," I= %ld ",ticksI);
	HAL_UART_Transmit(&huart3, (uint8_t *)bufferTxt, strlen(bufferTxt), HAL_MAX_DELAY);

	sprintf(bufferTxt," contI= %ld ",contI);
	HAL_UART_Transmit(&huart3, (uint8_t *)bufferTxt, strlen(bufferTxt), HAL_MAX_DELAY);

		sprintf(bufferTxt," D= %ld ",ticksD);
		HAL_UART_Transmit(&huart3, (uint8_t *)bufferTxt, strlen(bufferTxt), HAL_MAX_DELAY);

		sprintf(bufferTxt," contD= %ld \r\n",contD);
	HAL_UART_Transmit(&huart3, (uint8_t *)bufferTxt, strlen(bufferTxt), HAL_MAX_DELAY);
	  	HAL_Delay(300);
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
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
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
