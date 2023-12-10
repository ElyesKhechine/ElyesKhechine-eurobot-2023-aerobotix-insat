#include "Odometry.h"
//test solo dez robot b ydik bch taaref odometry tmchi w lee 

long volatile millis=0,t=0;

TIM_HandleTypeDef* htim_right_encoder;
TIM_TypeDef* right_TIM;
int right_resolution;
int right_precision;
int right_sens;
volatile unsigned int current_right_count;
volatile unsigned int last_right_count;
volatile long d_right;
volatile long total_right_count;
volatile float total_right=0,d_right_counter=0;
volatile double right_speed,right_encoder_speed;

TIM_HandleTypeDef* htim_left_Encoder;
TIM_TypeDef* left_TIM;
int left_resolution;
int left_precision;
int left_sens;
volatile unsigned int current_left_count;
volatile unsigned int last_left_count;
volatile long d_left;
volatile long total_left_count;
volatile float total_left=0,d_left_counter=0,total_centre=0;
volatile double left_speed,left_encoder_speed;

volatile float dR,dL,dC;
volatile float left_radius,right_radius,spacing_encoder,spacing_wheel;
volatile float current_x=0;
volatile float current_y=0;
volatile double current_phi_rad=0;
volatile double phi_speed,d_phi_counter;
volatile float current_phi_deg;

float ref_x,ref_y,dec=100;

/*
  set_right_encoder(&htim3,TIM3,600,4,1);
  set_left_encoder(&htim4,TIM4,600,4,1);
tim3&4 counters 
*/

void set_right_encoder(TIM_HandleTypeDef* htim, TIM_TypeDef* TIM, int resolution, int precision, int sens)
{
	htim_right_encoder = htim;
	right_TIM = TIM;
	right_resolution = resolution;
	right_precision = precision;
	right_sens=-sens;
	HAL_TIM_Encoder_Start(htim_right_encoder,TIM_CHANNEL_1);
	right_TIM->CNT = 0;
	total_right_count = 0;
}

void set_left_encoder(TIM_HandleTypeDef* htim, TIM_TypeDef* TIM, int resolution, int precision, int sens)
{
	htim_left_Encoder = htim;
	left_TIM = TIM;
	left_resolution = resolution;
	left_precision = precision;
	left_sens=-sens;
	HAL_TIM_Encoder_Start(htim_left_Encoder,TIM_CHANNEL_1);
	left_TIM->CNT = 0;
	total_left_count = 0;
}

void read_right_encoder(void)
{
	last_right_count = current_right_count;
	current_right_count = right_sens*right_TIM->CNT;
	d_right = current_right_count - last_right_count;
        // faza mtaa overflow bech register ma yetaabech 
	if (d_right>30000)
		d_right = d_right - 65535;
	if (d_right<-30000)
		d_right = d_right + 65535;
	total_right_count = total_right_count + d_right;
}

void read_left_encoder(void)// left_TIM->CNT nbre tick (encoder timer) 
{
	last_left_count = current_left_count;
	current_left_count = left_sens*left_TIM->CNT;
	d_left = current_left_count - last_left_count;
	// faza mtaa overflow bech register ma yetaabech 
        if (d_left>30000)
		d_left = d_left - 65535;
	if (d_left<-30000)
		d_left = d_left + 65535;
	total_left_count = total_left_count + d_left;
}

void set_dimentions(float right_wheel_radius, float left_wheel_radius, float encoder_spacing, float wheels_spacing)
{
	right_radius = right_wheel_radius;
	left_radius = left_wheel_radius;
	spacing_encoder = encoder_spacing;
	spacing_wheel = wheels_spacing;
}

void update_position(void)  // 1ms tsir l fonction hedhi , kol 10ms tehseb vitesse  
{
	read_right_encoder();
	read_left_encoder();
	dR = ticks_to_distance(d_right,right_radius,right_resolution,right_precision);
	total_right += dR;// total distance mtaa 3ajla ymin 
	d_right_counter += dR; // lel calcul mtaa vitesse baaed kol 10 marrat nodkhlou f loop ( 10ms presque) w baaed reset 
	dL = ticks_to_distance(d_left,left_radius,left_resolution,left_precision);
	total_left += dL; // total distance mtaa 3ajla ysar 
	d_left_counter += dL; // lel calcul mtaa vitesse baaed kol 10 marrat nodkhlou f loop ( 10ms presque) w baaed reset 
	dC = (dR+dL)/2; // distance parcourue par point reference  
	total_centre+=dC; // total distance mtaa point ref 
	
	current_x += dC*cos(current_phi_rad);
	current_y += dC*sin(current_phi_rad);
	current_phi_rad += ((dR-dL)/spacing_encoder);
	d_phi_counter += ((dR-dL)/spacing_encoder);
        // interval bin -PI w PI 
	while (current_phi_rad>PI)
	{
		current_phi_rad -= 2*PI;
	}
	while (current_phi_rad<-PI)
	{
		current_phi_rad += 2*PI;
	}
	current_phi_deg = rad_to_deg(current_phi_rad);
	//Robot navi 2020
	millis++;
	t++;// compteur bch tehseb speed baaed 10 marrat m dkhoul lel interruption 

/* 
		run_motors();
		do delta=t-t0;
		while (delta<T);  
hedhi fazet l compteur ma yaawed l boucle ken ma twali t=10 f code navig 

*/
/* curve faza okhra 
	ref_x = current_x + cos(current_phi_rad) *dec;
	ref_y = current_y + sin(current_phi_rad) *dec;
*/
}

void speed_calcul(void)
{
	right_encoder_speed = d_right_counter/T*1000;
	d_right_counter = 0;
	left_encoder_speed = d_left_counter/T*1000;
	d_left_counter = 0;
	phi_speed = d_phi_counter/T*1000;
	d_phi_counter = 0;
/* vitesse mehich jeya ml 3ajla limin walla li ysar khw 
	right_speed = (right_encoder_speed + left_encoder_speed)/2 + phi_speed * spacing_wheel/2;
	left_speed = (right_encoder_speed + left_encoder_speed)/2 - phi_speed * spacing_wheel/2;
*/ 

}

void reset_position(void)
{
	current_x = 0;
	current_y = 0;
	current_phi_deg = 0;
	current_phi_rad = 0;
}

float ticks_to_distance(int x, float r, int resolution, int precision) 
{
	return (x*2*PI*r/(resolution*precision));
}

float rad_to_deg(double x)
{
	return (x*360/(2*PI));
}






// interruption temporaire pour executer l odometry with this function [HAL_TIM_Base_Start_IT(&htim7);] -----> timer7 
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance==TIM7) //check if the interrupt comes from TIM7
	{
		update_position();
		PWM_sign_change_counter();
		if (t%T == 0)
			speed_calcul();
	}
}
