#include "Robot_Navi_Euro19.h"

//Motors realted variables
extern int PWM_R,PWM_L;
extern int PWM_Max;
extern int PWM_R_Min,PWM_L_Min;
extern int PWM_R_Min_Rot,PWM_L_Min_Rot;
extern int PWM_R_sign_counter,PWM_L_sign_counter;

//Odometry related variables
extern float total_right,total_left;
extern float current_x,current_y,current_phi_deg;
extern float spacing;

//Robot Navi Related Variables
float remain_distR, remain_distL, target_dist, target_angle;
float decel_dist, accel_dist = 30;
float accel_angle = 0, decel_angle = 100;
float decel_coeff = 0.4;
int right_correction,left_correction;
int PWM_RB, PWM_LB;
int coef_correct_dist = 60; //Correction move distance
int coef_correct_angle = 50; //Correction rotate
int deep_rec_orientation = -1;
float orientation_precision = 0.6;

void init()
{
	total_right = 0;
	total_left = 0;
	PWM_R_sign_counter = 0;
	PWM_L_sign_counter = 0;
}

void speed_controle_distance(int R, int L)
{
	//Calculate the remaining distance
	remain_distR = target_dist - total_right;
	remain_distL = target_dist - total_left;
	//Set the movement direction
	if (remain_distR>0) PWM_RB = R; else PWM_RB = -R;
	if (remain_distL>0) PWM_LB = L; else	PWM_LB = -L;
	//Set speed in accel/decel zone:
	//Right Wheel
	if (fabs(total_right)<accel_dist) //Accel Zone
		PWM_RB *= (fabs(total_right)/accel_dist);
	else if (fabs(remain_distR)<decel_dist) //Decel Zone
		PWM_RB *= (fabs(remain_distR)/decel_dist);
	//Left Wheel
	if (fabs(total_left)<accel_dist) //Accel Zone
		PWM_LB *= (fabs(total_left)/accel_dist);
	else if (fabs(remain_distL)<decel_dist) //Decel Zone
		PWM_LB *= (fabs(remain_distL)/decel_dist);
	//Set the min speed
	if ((PWM_RB<PWM_R_Min) && (PWM_RB>0)) PWM_RB = PWM_R_Min;
	if ((PWM_LB<PWM_L_Min) && (PWM_LB>0)) PWM_LB = PWM_L_Min;
	if ((PWM_RB>(-PWM_R_Min)) && (PWM_RB<0)) PWM_RB = -PWM_R_Min;
	if ((PWM_LB>(-PWM_L_Min)) && (PWM_LB<0)) PWM_LB = -PWM_L_Min;
	//Process the zero case
	if (PWM_RB==0) PWM_RB = (target_dist>0)? PWM_R_Min : -PWM_R_Min;
	if (PWM_LB==0) PWM_LB = (target_dist>0)? PWM_L_Min : -PWM_L_Min;
}

void move_distance (float distance, int R, int L)
{
	init();
	decel_dist = decel_coeff * distance;
	if (decel_dist<80) decel_dist = 80;
	if (decel_dist>400) decel_dist = 400;
	target_dist = distance;
	while ((PWM_L_sign_counter<4)||(PWM_R_sign_counter<4))
	{
		speed_controle_distance(R,L);
		//Orientation Correction
		left_correction = coef_correct_dist * (total_right-total_left);
		right_correction = - left_correction;
		PWM_R = PWM_RB + right_correction;
		PWM_L = PWM_LB + left_correction;
		//Speed Saturation
		if (PWM_R>PWM_Max) PWM_R = PWM_Max;
		if (PWM_L>PWM_Max) PWM_L = PWM_Max;
		if (PWM_R<-PWM_Max) PWM_R = -PWM_Max;
		if (PWM_L<-PWM_Max) PWM_L = -PWM_Max;
		//Execute
		run_motors();
	}
	//Terminate
	stop_motors();
}

void speed_controle_rotation(int R, int L)
{
	//Calculate the remaining distance
	if (target_angle>0)
	{
		remain_distR = -target_dist - total_right;
		remain_distL = target_dist - total_left;
	}
	else
	{
		remain_distR = target_dist - total_right;
		remain_distL = -target_dist - total_left;
	}
	//Set the movement direction
	if (remain_distR>0) PWM_RB = R; else PWM_RB = -R;
	if (remain_distL>0) PWM_LB = L; else	PWM_LB = -L;
	//Set speed in accel/decel zone:
	//Right Wheel
	if (fabs(total_right)<accel_angle) //Accel Zone
		PWM_RB *= (fabs(total_right)/accel_angle);
	else if (fabs(remain_distR)<decel_angle) //Decel Zone
		PWM_RB *= (fabs(remain_distR)/decel_angle);
	//Left Wheel
	if (fabs(total_left)<accel_angle) //Accel Zone
		PWM_LB *= (fabs(total_left)/accel_angle);
	else if (fabs(remain_distL)<decel_angle) //Decel Zone
		PWM_LB *= (fabs(remain_distL)/decel_angle);
	//Set the min speed
	if ((PWM_RB<PWM_R_Min_Rot) && (PWM_RB>0)) PWM_RB = PWM_R_Min_Rot;
	if ((PWM_LB<PWM_L_Min_Rot) && (PWM_LB>0)) PWM_LB = PWM_L_Min_Rot;
	if ((PWM_RB>(-PWM_R_Min_Rot)) && (PWM_RB<0)) PWM_RB = -PWM_R_Min_Rot;
	if ((PWM_LB>(-PWM_L_Min_Rot)) && (PWM_LB<0)) PWM_LB = -PWM_L_Min_Rot;
	//Process the zero case
	if (PWM_RB==0) PWM_RB = (target_angle>0)? PWM_R_Min : -PWM_R_Min_Rot;
	if (PWM_LB==0) PWM_LB = (target_angle>0)? PWM_L_Min : -PWM_L_Min_Rot;
}

void rotate(float angle, int R, int L)
{
	init();
	target_angle = angle;
	target_dist = fabs (PI*spacing*target_angle/360);
	while ((PWM_L_sign_counter<4)||(PWM_R_sign_counter<4))
	{
		speed_controle_rotation(R,L);
		//Position Correction;
		left_correction = coef_correct_angle * (total_right + total_left);
		right_correction = - left_correction;
		PWM_R = PWM_RB + right_correction;
		PWM_L = PWM_LB - left_correction;
		//Speed Saturation
		if (PWM_R>PWM_Max) PWM_R = PWM_Max;
		if (PWM_L>PWM_Max) PWM_L = PWM_Max;
		if (PWM_R<-PWM_Max) PWM_R = -PWM_Max;
		if (PWM_L<-PWM_Max) PWM_L = -PWM_Max;
		//Execute
		run_motors();
	}
	//Terminate
	stop_motors();
}

void orientate (float orientation, int R, int L)
{
	deep_rec_orientation++;
	if (deep_rec_orientation)
	{
		PWM_L_Min_Rot-=50;
		PWM_R_Min_Rot-=50;
	}
	target_angle = - (orientation - current_phi_deg);
	if (target_angle>180) target_angle -= 360;
	if (target_angle<-180) target_angle += 360;
	if (fabs(target_angle)>orientation_precision)
	{
		rotate(target_angle,R,L);
		HAL_Delay(150);
	}
	if ((fabs(orientation-current_phi_deg)>orientation_precision) && (deep_rec_orientation<3))
		orientate(orientation,R,L);
	else
	{
		PWM_R_Min_Rot += deep_rec_orientation*50;
		PWM_L_Min_Rot += deep_rec_orientation*50;
		deep_rec_orientation = -1;
	}
}

void Robot_Locate(float goal_x, float goal_y, int R, int L)
{
	int sens = (asinf((goal_y-current_y)/sqrtf((current_x-goal_x)*(current_x-goal_x)+(current_y-goal_y)*(current_y-goal_y)))>0)? 1 : -1;
	target_angle = sens * rad_to_deg(acosf((goal_x-current_x)/sqrtf((current_x-goal_x)*(current_x-goal_x)+(current_y-goal_y)*(current_y-goal_y))));
	orientate(target_angle,2500,2500);
	HAL_Delay(100);
	target_dist = sqrtf((current_x-goal_x)*(current_x-goal_x)+(current_y-goal_y)*(current_y-goal_y));
	move_distance (target_dist,R,L);
	HAL_Delay(100);
}
