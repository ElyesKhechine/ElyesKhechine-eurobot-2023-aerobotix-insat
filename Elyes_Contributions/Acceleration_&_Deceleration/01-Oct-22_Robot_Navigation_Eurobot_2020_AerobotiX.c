#include "Robot_Navi_Euro20.h"
#include <stdlib.h>

extern volatile long millis,t;
unsigned long long int delta,t0;

//Motors realted variables
extern int PWM_R,PWM_L;
extern int PWM_Max;
extern int PWM_R_Min,PWM_L_Min;
extern int PWM_R_Min_Rot,PWM_L_Min_Rot;
extern int PWM_R_sign_counter,PWM_L_sign_counter;

//Odometry related variables
extern volatile float total_right,total_left, total_centre;
extern volatile float current_x,current_y,current_phi_deg,current_phi_rad;
extern float ref_x, ref_y;
extern  float spacing_encoder,spacing_wheel,dec;
extern volatile double right_speed, left_speed;

//Robot Navi Related Variables
float goal_distance_mm, target_angle;
float accel_dist_mm, decel_dist_mm;
int PWM_LB,PWM_RB;
int coef_correct_angle=100; //50 Correction rotate
int right_correction=0,left_correction=0;
float temps_rampe_s = 0;
float acceleration_mm = 200;  /* en mm/s-2   */
float origine_des_temps = 0;	 /*  en ms    */
int first = 1;
float vitesse_de_retour_en_place_mm = 500 ;
float error ;
float i_error ;
float previous_error ;

//Speed Regulation
float speed_ref, ramp = 500,rampR=100,rampC=500;//max 1500
int sens;
double right_error=0,i_right_error=0;
double left_error=0,i_left_error=0;
float kp = 15, ki = 0.5, kd = 5;
float kp_rot = 15, ki_rot = 0.5, kd_rot = 11;
float kp_sur_place = 0,kd_sur_place = 0;

//Move
int coef_correct_dist = 20;
int coef_correct_erreur_angle = 50;
int coef_correct_erreur_vitesse_angulaire = 2;

//Trajectory
float target_x, target_y;
float target_x_prime, target_y_prime;
float right_target_speed, left_target_speed;

// curv
float remain_distC=0,goalC=0;
float speed_refR=0,speed_refL=0,prev_speed_refR=0,prev_speed_refL=0,new_speed_refR=0,new_speed_refL=0,speedC=0,speed_refC=0;
float kpL = 15.5, kiL = 1.75;
float kpR = 15.5, kiR = 1.75;
float corde=0,tetaC=0,phi_prim=0,corde_angle=0,Xc=0,Yc=0,Rayon=0,phi_target_rad=0,sens_de_mouvement=0;
float Distance_empietement=50;
float** matrix;
int flag1=0,flag2=0;

float constrain (float x,float min,float Max)
{
	if (x<min) return min;
	if (x>Max) return Max;
	else return(x);
}

void init (void)
{
	total_right=0;
	total_left=0;
	total_centre=0;
	PWM_L_sign_counter=0;
	PWM_R_sign_counter=0;
	i_right_error = 0;
	i_left_error = 0;
	right_error=0;
	left_error=0;
	PWM_R = 0;
	PWM_L = 0;
}



void move_distance(float goal_x, float goal_y,float distance_mm,float vitesse_max_mm)
{
	temps_rampe_s = vitesse_max_mm / acceleration_mm ;
	init();  
	//Set accel/decel distance
	if (fabs(distance_mm) < (2*0.5*acceleration_mm*temps_rampe_s*temps_rampe_s))
	{
		accel_dist_mm = distance_mm/2;
		decel_dist_mm = distance_mm/2;
		temps_rampe_s = sqrt(2*accel_dist_mm/acceleration_mm);
		vitesse_max_mm = temps_rampe_s*acceleration_mm ;
	}
	else
	{	
		temps_rampe_s = vitesse_max_mm / acceleration_mm ;
		accel_dist_mm = (float)0.5*acceleration_mm*temps_rampe_s*temps_rampe_s;
		decel_dist_mm = (float)0.5*acceleration_mm*temps_rampe_s*temps_rampe_s;
	}
	first = 1;
	origine_des_temps = t;
	while(fabs((total_right+total_left)/2-distance_mm)>1)
	{
		/*****TEMPS_DE_DEPART_DE_PERIODE_D'ECHANTILLONNAGE****/
		t0=t;
		/******************CHOIX_DU_SENS**********************/
		if ((distance_mm-((total_right+total_left)/2))>0)
			sens = 1;
		else
			sens = -1;
		/************CALCUL_DE_VITESSE_ACCELERATION***********/
		if (fabs((total_right+total_left)/2) < accel_dist_mm)
			speed_ref = sens*constrain((t-origine_des_temps)*acceleration_mm/1000,0,vitesse_max_mm);
	  /************CALCUL_DE_VITESSE_DECELERATION***********/
		else if (fabs(distance_mm-((total_right+total_left)/2)) < decel_dist_mm){
			if(first == 1){origine_des_temps = t;first =0;}
			speed_ref = sens*constrain(vitesse_max_mm-((t-origine_des_temps)*(acceleration_mm/1000)),0,vitesse_max_mm);}
		/****************VITESSE_PHASE_UNIFORME***************/
		else
			speed_ref = sens*vitesse_max_mm;
		//Right wheel regulation
		right_error = speed_ref - right_speed;
		i_right_error += right_error;
		PWM_RB = kp * right_error + ki * i_right_error;
		if (PWM_RB>PWM_Max) PWM_RB = PWM_Max;
		if (PWM_RB<-PWM_Max) PWM_RB = -PWM_Max;
		//Left wheel regulation
		left_error = speed_ref - left_speed;
		i_left_error += left_error;
		PWM_LB = kp * left_error + ki * i_left_error;
		if (PWM_LB>PWM_Max) PWM_LB = PWM_Max;
		if (PWM_LB<-PWM_Max) PWM_LB = -PWM_Max;
		//Orientation Correction?
		sens = (asinf((goal_y-current_y)/sqrtf((current_x-goal_x)*(current_x-goal_x)+(current_y-goal_y)*(current_y-goal_y)))>0)? 1 : -1;
		target_angle = sens * rad_to_deg(acosf((goal_x-current_x)/sqrtf((current_x-goal_x)*(current_x-goal_x)+(current_y-goal_y)*(current_y-goal_y))));
		left_correction  = coef_correct_erreur_angle * (target_angle-current_phi_deg);
		//left_correction -= coef_correct_erreur_vitesse_angulaire *(right_speed - left_speed);
		right_correction = left_correction;
		PWM_R = PWM_RB + right_correction;
		PWM_L = PWM_LB - left_correction;
		//Execution
		run_motors();
		do delta=t-t0;
		while (delta<T);
	}
	
	stop_motors();
}
	
void rotate(float angle, float speed)
{
	init();
	//Set accel/decel distance
	goal_distance_mm = angle * PI * spacing_encoder/ 180;
	if (fabs(goal_distance_mm) < (2*speed*speed/rampR))
	{
		accel_dist_mm = fabs(goal_distance_mm)/2;
		decel_dist_mm = fabs(goal_distance_mm)/2;
		speed = sqrt(rampR*accel_dist_mm);
	}
	else
	{
		accel_dist_mm = (float)speed*speed/rampR;
		decel_dist_mm = (float)speed*speed/rampR;
	}
	while((fabs(total_right-total_left)<fabs(goal_distance_mm)))
	{
		t0=t;
		//Accel/Decel Speed Set
		if (((total_right-total_left)-goal_distance_mm)<0)
			sens = 1;
		else
			sens = -1;
		if (fabs((total_right-total_left)) < accel_dist_mm)
			speed_ref = sens*constrain(sqrt(rampR*fabs(total_right-total_left)),50,1000);
		else if (fabs((total_right-total_left)-goal_distance_mm) < decel_dist_mm)
			speed_ref = sens*constrain(sqrt(rampR*fabs((total_right-total_left)-goal_distance_mm)),10,1000);
		else
			speed_ref = sens*speed;
		//Right wheel regulation
		right_error = speed_ref - right_speed;
		i_right_error += right_error;
		PWM_RB = kp_rot * right_error + ki_rot * i_right_error;
		//Left wheel regulation
		left_error = - speed_ref - left_speed;
		i_left_error += left_error;
		PWM_LB = kp_rot * left_error + ki_rot * i_left_error;
		//Position Correction;
		left_correction = coef_correct_angle * (total_right + total_left);
		right_correction = - left_correction;
		PWM_R = PWM_RB + right_correction;
		PWM_L = PWM_LB - left_correction;
		//Execution
		run_motors();
		do delta=t-t0;
		while (delta<T);
	}
	stop_motors();
}

void orientate (float orientation, float speed)
{
	target_angle = orientation - current_phi_deg;
	if (target_angle>180) target_angle -= 360;
	if (target_angle<-180) target_angle += 360;
	rotate(target_angle,speed);
}

void Robot_Locate(float goal_x, float goal_y, float speed)
{
	sens = (asinf((goal_y-current_y)/sqrtf((current_x-goal_x)*(current_x-goal_x)+(current_y-goal_y)*(current_y-goal_y)))>0)? 1 : -1;
	target_angle = sens * rad_to_deg(acosf((goal_x-current_x)/sqrtf((current_x-goal_x)*(current_x-goal_x)+(current_y-goal_y)*(current_y-goal_y))));
	orientate(target_angle,speed);
	goal_distance_mm = sqrtf((current_x-goal_x)*(current_x-goal_x)+(current_y-goal_y)*(current_y-goal_y));
	move_distance (goal_x,goal_y,goal_distance_mm,speed);
}
