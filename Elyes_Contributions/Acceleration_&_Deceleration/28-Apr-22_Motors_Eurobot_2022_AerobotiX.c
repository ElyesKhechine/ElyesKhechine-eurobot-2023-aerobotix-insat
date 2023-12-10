#include "Motors.h"

TIM_HandleTypeDef* htim_Motors;
uint32_t right_forward, right_backward, left_forward, left_backward;
int PWM_R,PWM_L;
extern int PWM_LB,PWM_RB;
int PWM_Max;
int PWM_R_Min,PWM_L_Min;
int PWM_R_Min_Rot,PWM_L_Min_Rot;
int PWM_R_sign,PWM_L_sign;
int PWM_R_last_sign,PWM_L_last_sign;
int PWM_R_sign_counter,PWM_L_sign_counter;

void set_motors (TIM_HandleTypeDef* htim, int MAX_PWM, uint32_t forward_right, uint32_t backward_right, uint32_t forward_left, uint32_t backward_left)
{
	htim_Motors = htim;
	PWM_Max = MAX_PWM;
	right_forward = forward_right;
	right_backward = backward_right;
	left_forward = forward_left;
	left_backward = backward_left;
	HAL_TIM_PWM_Start(htim_Motors, TIM_CHANNEL_1);  
	HAL_TIM_PWM_Start(htim_Motors, TIM_CHANNEL_2);  
	HAL_TIM_PWM_Start(htim_Motors, TIM_CHANNEL_3);  
	HAL_TIM_PWM_Start(htim_Motors, TIM_CHANNEL_4);
	stop_motors();
}

void stop_motors (void)
{
	PWM_R = 0;
	PWM_L = 0;
	PWM_RB = 0;
	PWM_LB = 0;
	__HAL_TIM_SetCompare(htim_Motors, TIM_CHANNEL_1, 0); 
	__HAL_TIM_SetCompare(htim_Motors, TIM_CHANNEL_2, 0);
	__HAL_TIM_SetCompare(htim_Motors, TIM_CHANNEL_3, 0); 
	__HAL_TIM_SetCompare(htim_Motors, TIM_CHANNEL_4, 0);
}

void run_motors(void)
{
	if (PWM_R>0)
	{
		if (PWM_R<PWM_R_Min) PWM_R=PWM_R_Min;
		if (PWM_R>PWM_Max) PWM_R=PWM_Max;
		__HAL_TIM_SetCompare(htim_Motors, right_forward, PWM_R); 
		__HAL_TIM_SetCompare(htim_Motors, right_backward, 0);
	}
	else
	{
		if (PWM_R>-PWM_R_Min) PWM_R=-PWM_R_Min;
		if (PWM_R<-PWM_Max) PWM_R=-PWM_Max;
		__HAL_TIM_SetCompare(htim_Motors, right_forward, 0); 
		__HAL_TIM_SetCompare(htim_Motors, right_backward, -PWM_R);
	}
	if (PWM_L>0)
	{
		if (PWM_L<PWM_L_Min) PWM_L=PWM_L_Min;
		if (PWM_L>PWM_Max) PWM_L=PWM_Max;
		__HAL_TIM_SetCompare(htim_Motors, left_forward, PWM_L); 
		__HAL_TIM_SetCompare(htim_Motors, left_backward, 0);
	}
	else
	{
		if (PWM_L>-PWM_L_Min) PWM_L=-PWM_L_Min;
		if (PWM_L<-PWM_Max) PWM_L=-PWM_Max;
		__HAL_TIM_SetCompare(htim_Motors, left_forward, 0); 
		__HAL_TIM_SetCompare(htim_Motors, left_backward, -PWM_L);
	}
		/*if (PWM_R>0)
	{
		if (PWM_R<PWM_R_Min) PWM_R=PWM_R_Min;
		if (PWM_R>PWM_Max) PWM_R=PWM_Max;
		__HAL_TIM_SetCompare(htim_Motors, right_forward, PWM_R); 
		//__HAL_TIM_SetCompare(htim_Motors, right_backward, 0);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,SET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,RESET);
	}
	else
	{
		if (PWM_R>-PWM_R_Min) PWM_R=-PWM_R_Min;
		if (PWM_R<-PWM_Max) PWM_R=-PWM_Max;
		__HAL_TIM_SetCompare(htim_Motors, right_forward, 0); 
	//	__HAL_TIM_SetCompare(htim_Motors, right_backward, -PWM_R);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,SET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,RESET);
	}
	if (PWM_L>0)
	{
		if (PWM_L<PWM_L_Min) PWM_L=PWM_L_Min;
		if (PWM_L>PWM_Max) PWM_L=PWM_Max;
		__HAL_TIM_SetCompare(htim_Motors, left_forward, PWM_L); 
		//__HAL_TIM_SetCompare(htim_Motors, left_backward, 0);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,SET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,RESET);
	}
	else
	{
		if (PWM_L>-PWM_L_Min) PWM_L=-PWM_L_Min;
		if (PWM_L<-PWM_Max) PWM_L=-PWM_Max;
		__HAL_TIM_SetCompare(htim_Motors, left_forward, 0); 
		//__HAL_TIM_SetCompare(htim_Motors, left_backward, -PWM_L);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,SET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,RESET);
	}*/
}

void run_forward (int right_PWM, int left_PWM)
{
	__HAL_TIM_SetCompare(htim_Motors, right_forward, right_PWM); 
	__HAL_TIM_SetCompare(htim_Motors, right_backward, 0);
	__HAL_TIM_SetCompare(htim_Motors, left_forward, left_PWM); 
	__HAL_TIM_SetCompare(htim_Motors, left_backward, 0);
}
void run_backward (int right_PWM, int left_PWM)
{
	__HAL_TIM_SetCompare(htim_Motors, right_forward, 0); 
	__HAL_TIM_SetCompare(htim_Motors, right_backward, right_PWM);
	__HAL_TIM_SetCompare(htim_Motors, left_forward, 0); 
	__HAL_TIM_SetCompare(htim_Motors, left_backward, left_PWM);
}

void set_PWM_min (int PWM_Min_R, int PWM_Min_L, int PWM_Min_R_Rot, int PWM_Min_L_Rot)
{
	PWM_R_Min=PWM_Min_R;
	PWM_L_Min=PWM_Min_L;
	PWM_R_Min_Rot=PWM_Min_R_Rot;
	PWM_L_Min_Rot=PWM_Min_L_Rot;
}

void PWM_sign_change_counter(void)
{
	PWM_L_last_sign = PWM_L_sign;
	PWM_R_last_sign = PWM_R_sign;
	if (PWM_R >= 0) PWM_R_sign = 1;
		else PWM_R_sign = -1;
	if (PWM_R >= 0) PWM_L_sign = 1;
		else PWM_L_sign = -1;
	if (PWM_L_sign != PWM_L_last_sign) PWM_L_sign_counter++;
	if (PWM_R_sign != PWM_R_last_sign) PWM_R_sign_counter++;
} 
