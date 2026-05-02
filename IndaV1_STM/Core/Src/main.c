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

typedef struct
{
	float	kp;
	float	ki;
	float	kd;
	float	ultimoError;
	float	integral;
	float	winup;
	float	PWM_Maximo;

}PID;

Debug_Datos debugDatos[200];
Motores_Init Motor;
MPU6500_Init_Values_t 	MPU6500_Datos; //Iniciamos donde se guardaran todos los datos a leer
MPU6500_status_e	MPU6500_Status;
MPU6500_Init_float_t	MPU6500_Conv;


PID TestPID={0.8,0.1,0.5 ,0,0,50,100};
PID TestPID2={1.2,0,0 ,0,0,50,100};

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
float    ADC_Distancia[4];
const uint32_t ADC_CanalTX[4]={IR1_TX_Pin,IR2_TX_Pin,IR3_TX_Pin,IR4_TX_Pin};
const uint32_t ADC_CanalRX[4]={3,2,1,0};
const uint8_t ADC_Secuencia[4]={0,2,1,3};
const float ADC_Const_a[4]={982.65,		1232.95,	1604.23	,	826.01};
const float ADC_Const_b[4]={-0.2653,	-0.5104,	-0.4915,	-0.292};
const float ADC_Const_c[4]={-79.8326,	3.9067	,	-5.3567	,	-46.3869};

bool flagDatos_20ms=false;
bool flagPID_3ms=false;
bool flagDistanciaIR=false;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

void Inicializar_Sistema();
void MPU6500();
uint32_t ADC_Read_Manual(ADC_HandleTypeDef *hadc, uint32_t channel);
void DEBUG_Encoder();
float funcion_calcularPID(PID* pid,int16_t setpoint,int16_t actual,float dt);

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

if(flagDistanciaIR==true){
		for (int var = 0; var < 4; ++var) {
			ADC_Distancia[var]=ADC_Const_a[var]*powf((float)ADC_IR[var],ADC_Const_b[var])+ADC_Const_c[var];
		}
	flagDistanciaIR=false;
  }

static float voltaje=0;
static uint8_t pulsador=0;
if (flagDatos_20ms==true)
{
	voltaje=(ADC_Sensores[0]*3.35)/4095;
	voltaje*=2.74074;

	if (ADC_Sensores[3]>3950)pulsador=1;
		else if (ADC_Sensores[3]>2650 && ADC_Sensores[3]<2850)pulsador=2;
			else if (ADC_Sensores[3]>1000 && ADC_Sensores[3]<1200)pulsador=3;
	flagDatos_20ms=false;
}
static float error=0;

static float pwmgiro,pwmdistancia=0;

if(flagPID_3ms==true)
{
	MPU6500_Read(&MPU6500_Datos);
	MPU6500_Conv=MPU6500_Converter(&MPU6500_Datos);

	/// test con 2 pid para mantener la distancia contra la pared

	/*error=(((ADC_Distancia[0]+ADC_Distancia[3])/2)-80);
	if(ADC_Distancia[0]>200 || ADC_Distancia[3]>200)
	{
		pwmgiro=funcion_calcularPID(&TestPID,0 , 0, 0.02);
		pwmdistancia=-funcion_calcularPID(&TestPID2,0, 0, 0.02);
	}
	else{
		pwmgiro=funcion_calcularPID(&TestPID,0 , ADC_Distancia[0]-ADC_Distancia[3], 0.02);
		    pwmdistancia=-funcion_calcularPID(&TestPID2,0, error, 0.02);
	}*/

	if(MPU6500_Conv.MPU6500_floatGZ<0.25 && MPU6500_Conv.MPU6500_floatGZ>-0.25)
	{
		MPU6500_Conv.MPU6500_floatGZ=0;
	}
	error=error+MPU6500_Conv.MPU6500_floatGZ*0.003;

	pwmgiro=funcion_calcularPID(&TestPID,90 ,error , 0.02);
	Motor.ENABLE=true;
	Motor.PWM_MR=(int16_t)(pwmdistancia+pwmgiro);
	Motor.PWM_ML=(int16_t)(pwmdistancia-pwmgiro);
	PWM_Motores(&Motor);

flagPID_3ms=false;
}
static uint32_t tiempo=0;
static uint16_t sumador_reset_error=0;
if(sumador_reset_error>60)
{
	error=0;
	sumador_reset_error=0;
}
if((HAL_GetTick()-tiempo)>500)
{sumador_reset_error++;
/*
	sprintf(bufferTxt,"vol=%0.2f, ",voltaje);
		HAL_UART_Transmit(&huart3, (uint8_t *)bufferTxt, strlen(bufferTxt), HAL_MAX_DELAY);
		sprintf(bufferTxt,"pulso=%d ",pulsador);
			HAL_UART_Transmit(&huart3, (uint8_t *)bufferTxt, strlen(bufferTxt), HAL_MAX_DELAY);
*/
	/*
	for (int var = 0; var < 4; ++var) {
		sprintf(bufferTxt,"A%d=%0.2f, ",var,ADC_Distancia[var]);
			HAL_UART_Transmit(&huart3, (uint8_t *)bufferTxt, strlen(bufferTxt), HAL_MAX_DELAY);

	}

*/
/*	sprintf(bufferTxt,"A=%0.2f, ",ADC_Distancia[0]);
	HAL_UART_Transmit(&huart3, (uint8_t *)bufferTxt, strlen(bufferTxt), HAL_MAX_DELAY);
	sprintf(bufferTxt,"D=%0.2f, ",ADC_Distancia[3]);
		HAL_UART_Transmit(&huart3, (uint8_t *)bufferTxt, strlen(bufferTxt), HAL_MAX_DELAY);
*/
	sprintf(bufferTxt,"pwmD=%0.2f, ",error);
	HAL_UART_Transmit(&huart3, (uint8_t *)bufferTxt, strlen(bufferTxt), HAL_MAX_DELAY);
	sprintf(bufferTxt,"pwmG=%0.2f, ",pwmgiro);
		HAL_UART_Transmit(&huart3, (uint8_t *)bufferTxt, strlen(bufferTxt), HAL_MAX_DELAY);

	//MPU6500();
sprintf(bufferTxt,"\r\n");
HAL_UART_Transmit(&huart3, (uint8_t *)bufferTxt, strlen(bufferTxt), HAL_MAX_DELAY);
tiempo=HAL_GetTick();
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


	LED1_GPIO_Port->BSRR=(uint32_t)LED1_Pin;
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
	LED1_GPIO_Port->BSRR=(uint32_t)LED1_Pin<<16;
	HAL_ADC_Start_DMA(&hadc2, ADC_Sensores, 4);
	HAL_TIM_Base_Start_IT(&htim4);

}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM4) {

    	static volatile uint16_t Timer_20ms=0;
    	static volatile uint16_t Timer_3ms=0;
    	static uint8_t canal = 0;

    	if(Timer_20ms++>334)
    	{
    		///habilitar DMA
    		ADC2->CR2|=ADC_CR2_SWSTART;
    		flagDatos_20ms=true;
    		Timer_20ms=0;
    	}
    	if(Timer_3ms++>50)
    	{
    	flagPID_3ms=true;
    	Timer_3ms=0;
    	}

       	GPIOC->BSRR=(uint32_t)(IR1_TX_Pin|IR2_TX_Pin|IR3_TX_Pin|IR4_TX_Pin)<<16;


        GPIOC->BSRR = ADC_CanalTX[ADC_Secuencia[canal]];
        // 2. CAMBIAR CANAL EN EL ADC
           // Escribimos en SQR3 (los bits 0-4 definen el Rank 1)
           // Esto le dice al ADC: "En el próximo SWSTART, lee este canal específico"
        ADC1->SQR3 = ADC_CanalRX[ADC_Secuencia[canal]];

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

       	ADC_IR[ADC_Secuencia[canal]] = ADC1->DR;

       	canal++;
       	if (canal>3) {
       		flagDistanciaIR=true;
		canal=0;
		}

    	GPIOC->BSRR=(uint32_t)(IR1_TX_Pin|IR2_TX_Pin|IR3_TX_Pin|IR4_TX_Pin)<<16;




    }
}


void MPU6500()
{

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
/**
 * @brief PID para los motores
 * @param PID: se debe mandar una estructura con los datos del pid a controlar
 * @paran setpoint: valor a cual se quiere llegar
 * @param actual: valor leido por los sensores
 * @param dt: Tiempo de muestreo segun timer puede variar
 */
float funcion_calcularPID(PID* pid,int16_t setpoint,int16_t actual,float dt)
{
	float error=(float)(setpoint-actual);
	float P=error*pid->kp;

	pid->integral=pid->integral+error;
	if(pid->integral>(pid->winup))pid->integral=(pid->winup);
	if(pid->integral<-(pid->winup))pid->integral=-(pid->winup);

	float I=pid->ki*pid->integral;

	float D=pid->kd*(error-pid->ultimoError);
	pid->ultimoError=error;

	float PIDout=P+I+D;
	if(PIDout>(pid->PWM_Maximo))PIDout=(pid->PWM_Maximo);
	if(PIDout<-(pid->PWM_Maximo))PIDout=-(pid->PWM_Maximo);
	return PIDout;
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
