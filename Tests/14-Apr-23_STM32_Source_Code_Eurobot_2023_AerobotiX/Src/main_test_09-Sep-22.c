/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "math.h"
#include "stdio.h"

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
TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim7;

UART_HandleTypeDef huart5;
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_rx;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM4_Init(void);
static void MX_DMA_Init(void);
static void MX_UART5_Init(void);
static void MX_TIM7_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM3_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int32_t counterL = 0;
int32_t counterR = 0;
int16_t countL = 0;
uint32_t prev1=0;
uint32_t prev2=0;
int32_t dl1=0;
int32_t dl2=0;
int16_t countR = 0;
double coefDistD=0.223987864;
double coefDistG= 0.221092457;

double coefAngD=0.149028237 ;
double coefAngG =0.0755143391;

int i=0 ;

double xR=0;
double yR=0;
double xC=0;
double yC=1000;
double DistC;
double DistA=1000;

double Err ;

double ErrD;
double ErrprevD=0;
double dErrD; //Err-Errprev
double sumErrD ;

double ErrG;
double ErrprevG=0;
double dErrG;
double sumErrG ;

double dAngl=0;
double dDist=0;
double DistCD ;
double DistCG ;
double dx=0,dy=0;


double KpPos1=0.2;
double Kppos2=0.078 ; //0.075

double KdPos=1.65;
double Kipos=0 ;

double x1,x2 ;


double orientationR;
double orientationCible;
int signe=1;
double dD=0 ;
double dG=0 ;
int cmdD=100;
int cmdG=100;
double PI = 3.14159265 ;
double deltaAngl ;
double dAnglprev ;
double deltaX ;
double dDistprev ;
double orientation= 3.14159265*0.5 ;
double consigneOrientation ;

double 	deltaErreur ;
double erreurAngle ;
double erreurPre;
double sumErr ;

double Kpa=6.59;
double Kda=6.8;
double Kia=3.9 ;

double deltaCommande ;

double errVit ;
double Kpv=5;
double deltaVitesse ;
double derrVit ;
double errVitprev=0 ;
double Kdv=60 ;
double sumErrVit ;
double Kiv =0  ;

double r1;
double r2;
double l1;
double l2;



double a ;
double b ;
double anglR ;
double sumErra ;

double cmptR1=0 ;
double cmptR2=0 ;
double cmptL1=0 ;
double cmptL2=0 ;

int time=-1 ;
int time2 = 0 ;

uint8_t txText = '1';

void hz3alam() {
txText= 'a' ;
HAL_UART_Transmit(&huart5, &txText, 1, 100);
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	 if(htim->Instance == TIM7){

		time ++ ;

	     }
}


void cho3laDispatching(){ // this
	txText='i';
	HAL_UART_Transmit(&huart5, &txText, 1, 100);
}
void ballThrow(){ // this
	txText='k';
	HAL_UART_Transmit(&huart5, &txText, 1, 100);
}
void ballReset(){ // this
	txText='l';
	HAL_UART_Transmit(&huart5, &txText, 1, 100);
}
void getDiscs_ThrowWd(){ // this
	txText='m';
	HAL_UART_Transmit(&huart5, &txText, 1, 100);
	}
void tirDiscs_Door_Thrower_Control(){ // this
	txText='n';
	HAL_UART_Transmit(&huart5, &txText, 1, 100);
}


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	countR = __HAL_TIM_GET_COUNTER(&htim3);
	dl1=countL-prev1;
	if(dl1>30000)
		dl1=dl1-65535;
	if(dl1<-30000)
		dl1=dl1+65535;
	counterL = counterL+dl1;
	prev1= counterL;
	countL = __HAL_TIM_GET_COUNTER(&htim2);
		dl2=countR-prev2;
		if(dl2>30000)
			dl2=dl2-65535;
		if(dl2<-30000)
			dl2=dl2+65535;
		counterR = counterR+dl2;
		prev2= counterR;

}
void forward(int x,int y)
{
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,x);
    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,0);//arriere gche


    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,y);//arriere dte
    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,0);//avanc dte
}
void backward(int x,int y)
{
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,x);//avance gche


    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,0);//avance dte
    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,y);
}
void right(int x ,int y )
{
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,x);
    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,0);//avance gche
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,0);
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,y);//arriere dte
	}
void stop()
{
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,0);//avance gche
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,0);
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,0);//arriere dte
	}
void left(int x , int y)
{
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);//arriere gche
    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,x);
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,y);//avance dte
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,0);
	}
void update_position()
{
			dG=coefDistG*countL ;
			dD=coefDistD*countR;

		  	dDist=(dD+dG)/2.000;
		  	dAngl=(dD-dG)/196; //196
		  	deltaX = dDist - dDistprev ;
		  	dy=deltaX*cos(dAngl);
		  	dx=deltaX*sin(dAngl);


		  	deltaAngl=dAngl - dAnglprev ;



		  	dAnglprev=dAngl ;

		  	orientation+=deltaAngl;
		  	//orientation= atan2(sin(dAngl), cos(dAngl));
		  	anglR=orientation*57.3248407643;

		  	/*if(orientation<-PI)
		  		orientation+=2*PI;
		  	if(orientation>PI)
		  		orientation-=2*PI;*/


		    xR+=dx;
		  	yR+=dy;

		  	dDistprev = dDist ;

	}

void go(double distance)
	  {
	b=0;
	i=1 ;
	TIM2->CNT = 0 ;
	TIM4->CNT = 0 ;

	while(1){

		dG=coefDistG*countL ;
		dD=coefDistD*countR;
		r1=dD;
		l1=dG;
		dDist=(dD+dG)/2;
		dAngl=(dD-dG)/385;

		deltaX = dDist - dDistprev ;
		deltaAngl=dAngl - dAnglprev ;
		dy=deltaX*cos(dAngl);
		dx=-deltaX*sin(dAngl);
		dAnglprev=dAngl ;

		orientation+=deltaAngl;

		if(orientation<-PI)
			orientation+=2*PI;
		if(orientation>PI)
			orientation-=2*PI;



		xR+=dx;
		yR+=dy;

		dDistprev = dDist ;

			DistC=distance-dDist;

			DistCD=distance-dD;
			DistCG=distance-dG;

			if(DistC<0) DistC=0 ;
			if(DistCD<0) DistCD=0 ;
			if(DistCG<0) DistCG=0 ;

			Err=DistC ;

			ErrG=DistCG ;
			dErrG=ErrG-ErrprevG;
			ErrprevG=ErrG;
			sumErrG+=dErrG ;

			ErrD=DistCD ;
			dErrD=ErrD-ErrprevD;
			ErrprevD=ErrD;
			sumErrD+=dErrD ;

			//PID vitesse *******************************************************
			errVit = dD-dG ;
			derrVit = errVit - errVitprev ;
			errVitprev = errVit ;
			sumErrVit+= derrVit  ;
			deltaVitesse = Kpv*errVit+Kdv*derrVit+Kiv*sumErrVit ;


			//**************


			if ((Err>0.8*distance)&&(distance>200))
											{
											cmdD=130 ;
											cmdG = cmdD ;
											//cmdG -= deltaCommande;
											//cmdD += deltaCommande;
											cmdD -= deltaVitesse ;
											cmdG += deltaVitesse ;
											//cmdD = (((cmdD - 0)*(255 - 70))/(255 - 0)) + 70;
											//cmdG = (((cmdG - 0)*(255 - 70))/(255 - 0)) + 70;

											}   //}

											else if ((Err<0.8*distance)&&(Err>160)&&(distance>200))
											{
												//x1=(ErrD*255)/distance ;
												//x2=(ErrG*255)/distance ;
											cmdD=130 ;//+ KpPos1*ErrD+KdPos*dErrD+Kipos*sumErrD ;
											cmdG=130 ;//+ KpPos1*ErrG+KdPos*dErrG+Kipos*sumErrG ;
											//cmdG -= deltaCommande  ;
											//cmdD += deltaCommande  ;
											cmdD -= deltaVitesse ;
											cmdG += deltaVitesse ;
											//cmdD = (((cmdD - 0)*(255 - 70))/(600 - 0)) + 70;
											//cmdG = (((cmdG - 0)*(255 - 70))/(600 - 0)) + 70;
											}
											else
											{
											cmdD=105 + Kppos2*ErrD+KdPos*dErrD+Kipos*sumErrD ;
											cmdG=105 + Kppos2*ErrG+KdPos*dErrG+Kipos*sumErrG ;
											//cmdG -= deltaCommande  ;
											//cmdD += deltaCommande  ;
											cmdD -= deltaVitesse ;
											cmdG += deltaVitesse ;
											//cmdD = (((cmdD - 0)*(255 - 70))/(255 - 0)) + 70;
											//cmdG = (((cmdG - 0)*(255 - 70))/(255 - 0)) + 70;
											}

								if(cmdD>255)
								{
								  	cmdD = 255;
								}else if(cmdD < 0)
								{
								  	cmdD = 0;
								}

								if(cmdG>255)
								{
								  	cmdG = 255;
								}else if(cmdG < 0)
								{
								  	cmdG = 0;
								}




								 forward(cmdG,cmdD);
								 if(b==0)
								 b=1;
								 if (b==2)
									 b=3;
								 if ( dDist>distance)
							 {
									 backward(85,85);
									 b=2;
							 }
							if (b>=2)
							{

								stop() ;
				 				break;
							}

					/* if ( (DistCG>-2)&&(DistCG<2)  &&  (DistCD>-2)&&(DistCD<2)   )
					 {
						 update_position();
						 HAL_Delay(100);
						 	 r2=dD;
						 	 l2=dG;
						 	if ( (r1==r2)&&(l1==l2))
						 				{
						 			time=8;
						 				 stop() ;
						 				break;
										}
*/



					 //}

	}
	TIM2->CNT = 0 ;
	TIM4->CNT = 0 ;
	}
void back(double distance)

{
	b=0;
	TIM2->CNT = 0 ;
	TIM4->CNT = 0 ;

	while(1){

		dG=-coefDistG*countL ;
		dD=-coefDistD*countR;

		dDist=(dD+dG)/2;
		dAngl=(dD-dG)/196;

		deltaX = dDist - dDistprev ;
		deltaAngl=dAngl - dAnglprev ;
		dy=deltaX*cos(dAngl);
		dx=-deltaX*sin(dAngl);
		dAnglprev=dAngl ;

		orientation+=deltaAngl;

		if(orientation<-PI)
			orientation+=2*PI;
		if(orientation>PI)
			orientation-=2*PI;


		xR+=dx;
		yR+=dy;

		dDistprev = dDist ;

			DistC=distance+dDist;

			DistCD=distance+dD;
			DistCG=distance+dG;

			if(DistC<0) DistC=0 ;
			if(DistCD<0) DistCD=0 ;
			if(DistCG<0) DistCG=0 ;

			Err=DistC ;

			ErrG=DistCG ;
			dErrG=ErrG-ErrprevG;
			ErrprevG=ErrG;
			sumErrG+=dErrG ;

			ErrD=DistCD ;
			dErrD=ErrD-ErrprevD;
			ErrprevD=ErrD;
			sumErrD+=dErrD ;

			//PID vitesse *******************************************************
			errVit = dD-dG ;
			derrVit = errVit - errVitprev ;
			errVitprev = errVit ;
			sumErrVit+= derrVit  ;
			deltaVitesse = Kpv*errVit+Kdv*derrVit+Kiv*sumErrVit ;


			//**************


			if ((Err>0.8*distance)&&(distance>200))
											{
											cmdD=120 ;
											cmdG = cmdD ;
											//cmdG -= deltaCommande;
											//cmdD += deltaCommande;
											cmdD -= deltaVitesse ;
											cmdG += deltaVitesse ;
											//cmdD = (((cmdD - 0)*(255 - 70))/(255 - 0)) + 70;
											//cmdG = (((cmdG - 0)*(255 - 70))/(255 - 0)) + 70;

											}   //}

											else if ((Err<0.8*distance)&&(Err>160)&&(distance>200))
											{
												//x1=(ErrD*255)/distance ;
												//x2=(ErrG*255)/distance ;
											cmdD=135 ;//+ KpPos1*ErrD+KdPos*dErrD+Kipos*sumErrD ;
											cmdG=135 ;//+ KpPos1*ErrG+KdPos*dErrG+Kipos*sumErrG ;
											//cmdG -= deltaCommande  ;
											//cmdD += deltaCommande  ;
											cmdD -= deltaVitesse ;
											cmdG += deltaVitesse ;
											//cmdD = (((cmdD - 0)*(255 - 70))/(600 - 0)) + 70;
											//cmdG = (((cmdG - 0)*(255 - 70))/(600 - 0)) + 70;
											}
											else
											{
											cmdD=105+ Kppos2*ErrD+KdPos*dErrD+Kipos*sumErrD ;
											cmdG=105+ Kppos2*ErrG+KdPos*dErrG+Kipos*sumErrG ;
											//cmdG -= deltaCommande  ;
											//cmdD += deltaCommande  ;
											cmdD -= deltaVitesse ;
											cmdG += deltaVitesse ;
											//cmdD = (((cmdD - 0)*(255 - 70))/(255 - 0)) + 70;
											//cmdG = (((cmdG - 0)*(255 - 70))/(255 - 0)) + 70;
											}

								if(cmdD>255)
								{
								  	cmdD = 255;
								}else if(cmdD < 0)
								{
								  	cmdD = 0;
								}

								if(cmdG>255)
								{
								  	cmdG = 255;
								}else if(cmdG < 0)
								{
								  	cmdG = 0;
								}




								 backward(cmdG,cmdD);
								 if(b==0)
								 b=1;
								 if (b==2)
									 b=3;
								 if ( dDist>distance)
							 {
									 forward(85,85);
									 b=2;
							 }

							if (b>=2)
							{/*forward (40,0);
							HAL_Delay(50);*/

								stop() ;
				 				break;
							}

					/* if ( (DistCG>-2)&&(DistCG<2)  &&  (DistCD>-2)&&(DistCD<2)   )
					 {
						 update_position();
						 HAL_Delay(100);
						 	 r2=dD;
						 	 l2=dG;
						 	if ( (r1==r2)&&(l1==l2))
						 				{
						 			time=8;
						 				 stop() ;
						 				break;
										}
*/



					 //}

	}
	TIM2->CNT = 0 ;
	TIM4->CNT = 0 ;
	}

void pid_angle(double consigne)
{	orientation= 3.14159265*0.5 ;
i=2 ;
	while(1)
	{

	update_position();
	//consigneOrientation = signe * acos((xC-xR)/DistC);
	erreurAngle =  consigne - anglR;




		     	deltaErreur = erreurAngle - erreurPre;
		     			erreurPre  = erreurAngle;
		     	sumErra+=deltaErreur;

		     		a=	sqrt((erreurAngle)*(erreurAngle));
		  			//Calcul de la valeur � envoyer aux moteurs pour tourner
		  		deltaCommande = Kpa*a+ Kda * deltaErreur+Kia*sumErra;
		  			if (a<10)
		  			{
		  				cmdG = 5.87*deltaCommande;
		  				cmdD = 5.87*deltaCommande;
		  			}
		  			else
		  			{
		  				cmdG = 0.8*deltaCommande;
		  				cmdD = 0.8*deltaCommande;
		  			}

		  				if(cmdD>70)
		  				{
		  				  	cmdD = 70;

		  				}

		  				if(cmdG>70)
		  				{
		  				  	cmdG = 70;
		  				}
		 	if (erreurAngle<0)
		  	{

		  		right(cmdG+85,cmdD+85);
		  		b=16;

		  		if(cmptL1==0)
		  			cmptR1=1;
		  		if(cmptL1==1)
		  			cmptR1=2;
		  		if(cmptL1==2)
		  			cmptR1=3;

		  		if(cmptL2==1)
		  			cmptR2=1;
		  		if(cmptL2==2)
		  			cmptR2=2;
		  		if(cmptL2==3)
		  			cmptR2=3;

		  	}
		  	else
		  	{
		  		left(cmdG+85,cmdD+85);
		  		b=2;
		  		if (cmptR1==1)
		  			cmptL1=1;
		  		if (cmptR1==2)
		  			cmptL1=2;
		  		if (cmptR1==3)
		   			cmptL1=3;
		  		if(cmptR2==0)
	  			cmptL2=1;
		  		if(cmptR2==1)
		  		cmptL2=2;
		  		if(cmptR2==2)
		  		cmptL2=3;

		  	}



		 if (((cmptR1==3)&&(cmptL1==2))|| ((cmptR2==2)&&(cmptL2==3)))
			{
		 		stop() ;
		 		cmptR1=0 ;
		 		cmptR2=0 ;
		 		cmptL1=0 ;
		 		cmptL2=0 ;
		 		i=5;


		 		break ;}

	}
}

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
  MX_TIM2_Init();
  MX_TIM4_Init();
  MX_TIM3_Init();
  MX_DMA_Init();
  MX_UART5_Init();
  MX_TIM7_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Encoder_Start_IT(&htim2,TIM_CHANNEL_ALL);
  HAL_TIM_Encoder_Start_IT(&htim4,TIM_CHANNEL_ALL);
  HAL_TIM_Encoder_Start_IT(&htim3,TIM_CHANNEL_ALL);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
  HAL_TIM_Base_Start_IT(&htim7);

  go(1000) ;
  pid_angle(0) ;
  pid_angle(0) ;
  go(1000);
  pid_angle(0) ;
  pid_angle(0) ;
  /*pid_angle(0) ;
  pid_angle(47.3579) ;
  go(1684.34) ;
  pid_angle(132.6421) ;
  pid_angle(consigne)*/

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
   {
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
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

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 255;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 65535;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
  sConfig.IC1Polarity = TIM_ICPOLARITY_FALLING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 0;
  sConfig.IC2Polarity = TIM_ICPOLARITY_FALLING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 0;
  if (HAL_TIM_Encoder_Init(&htim2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 65535;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI1;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 0;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 0;
  if (HAL_TIM_Encoder_Init(&htim3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 0;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 65535;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
  sConfig.IC1Polarity = TIM_ICPOLARITY_FALLING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 0;
  sConfig.IC2Polarity = TIM_ICPOLARITY_FALLING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 0;
  if (HAL_TIM_Encoder_Init(&htim4, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */

}

/**
  * @brief TIM7 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM7_Init(void)
{

  /* USER CODE BEGIN TIM7_Init 0 */

  /* USER CODE END TIM7_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM7_Init 1 */

  /* USER CODE END TIM7_Init 1 */
  htim7.Instance = TIM7;
  htim7.Init.Prescaler = 1343;
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = 62499;
  htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM7_Init 2 */

  /* USER CODE END TIM7_Init 2 */

}

/**
  * @brief UART5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART5_Init(void)
{

  /* USER CODE BEGIN UART5_Init 0 */

  /* USER CODE END UART5_Init 0 */

  /* USER CODE BEGIN UART5_Init 1 */

  /* USER CODE END UART5_Init 1 */
  huart5.Instance = UART5;
  huart5.Init.BaudRate = 115200;
  huart5.Init.WordLength = UART_WORDLENGTH_8B;
  huart5.Init.StopBits = UART_STOPBITS_1;
  huart5.Init.Parity = UART_PARITY_NONE;
  huart5.Init.Mode = UART_MODE_TX_RX;
  huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart5.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart5) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART5_Init 2 */

  /* USER CODE END UART5_Init 2 */

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
  huart2.Init.BaudRate = 115200;
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

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : B1_Pin PC0 */
  GPIO_InitStruct.Pin = B1_Pin|GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

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

