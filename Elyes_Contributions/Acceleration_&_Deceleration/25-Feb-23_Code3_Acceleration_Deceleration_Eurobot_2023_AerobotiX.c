#include "Robot_Navi_Euro20.h"
#include <stdlib.h>
extern volatile long t;
unsigned long long int delta,t0;
extern int PWM_R,PWM_L;
extern int PWM_Max;
extern int PWM_R_Min,PWM_L_Min;
extern int PWM_R_Min_Rot,PWM_L_Min_Rot;
extern int PWM_R_sign_counter,PWM_L_sign_counter;
extern float right_encoder_speed,left_encoder_speed;
extern volatile float total_right,total_left, total_centre;
extern volatile float current_x,current_y,current_phi_deg,current_phi_rad;
extern float ref_x, ref_y;
extern  float spacing_encoder,spacing_wheel,dec;
extern volatile double right_speed, left_speed ;
extern volatile int robot_speed;
float goal_distance_mm, target_angle,target_angler;
int target = 0;
float accel_dist_mm, decel_dist_mm;
int PWM_LB,PWM_RB;
int coef_correct_angle_or=100; //50 Correction rotate
int right_correction=0,left_correction=0;
float temps_rampe_s = 0; 
float temps_rampe_s_accel = 0;
float temps_rampe_s_decel = 0;
float acceleration_mm = 800;  //1500/* en mm/s-2   */
float deceleration_mm = 1000;  //2725
float acceleration_or_mm = 400;
float deceleration_or_mm = 400;
int distance = 0 ;
extern int T ;
float anticipation_accel = 0 ;
float anticipation_decel = 0 ;
float amortissement = 0;
float origine_des_temps = 0;	 /*  en ms    */
int first = 1;
float vitesse_de_retour_en_place_mm = 500 ;
float error ;
float i_error ;
float previous_error ;
float speed_ref = 0, ramp = 350,rampR=90,rampC=500;//max 1500
int sens,sens_or;
long double right_error=0,i_right_error=0 , previous_right_error = 0;
long double left_error=0,i_left_error=0, previous_left_error = 0;
float pwm_de_base_right = 1500 ;
float pwm_de_base_left = 1500 ;
float kp_right = 20, ki_right = 0, kd_right = 4 ; //20 , 0, 0;            17.5 0 5       8,0,1
float kp_left = 20, ki_left = 0, kd_left = 4; //20 , 0, 0; //3.5/0.5     17.5 0 5
float kp_right_curve = 8, ki_right_curve = 0, kd_right_curve = 1 ; //20 , 0, 0;            17.5 0 5
float kp_left_curve = 8, ki_left_curve = 0, kd_left_curve = 1;
float kp_rot = 13, ki_rot = 0.4, kd_rot = 11;
float kp_right_rot = 13, ki_right_rot = 0.4, kd_right_rot = 11;
float kp_left_rot = 13, ki_left_rot = 0.4, kd_left_rot = 11;
float kp_sur_place = 0,kd_sur_place = 0;
float angle = 0;
float prev_speed ;
int coef_correct_dist = 75;
float goal_distance, target_angle;
float accel_dist, decel_dist;
int PWM_LB,PWM_RB;
int coef_correct_erreur_angle = 150;
int coef_correct_erreur_angle_asservi = 200;//150
int coef_correct_erreur_vitesse_angulaire = 2;
int coef_correct_deplacement = 50;
float distance_mm = 0,distance_fixe = 0 ;
float distance_mm_carre = 0 ;
float origin_x,origin_y;
float angly;

void init (void)
{
	//ki_right = ki_right*(T/10) ;
	//ki_left = ki_left*(T/10) ;
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
	previous_right_error = 0;
	previous_left_error = 0;
	error = 0;
	previous_error = 0;
	i_error = 0;
	target_angler = 0;
}
void first_go_non_stop(float goal_x, float goal_y,float vitesse_max_mm)
{
	init();
	distance_mm = sqrtf((current_x-goal_x)*(current_x-goal_x)+(current_y-goal_y)*(current_y-goal_y)); //calcul distance à parcourir
	temps_rampe_s_accel = vitesse_max_mm / acceleration_mm ; 										  // temps rampe accel = vitessemax / acceleration_mm
	if (fabs(distance_mm) < (0.5*acceleration_mm*temps_rampe_s_accel*temps_rampe_s_accel))      	  //si distance < (accel*rampe_accel²)/2  (courbe trapèze)
	{
		accel_dist_mm = distance_mm*(deceleration_mm/(acceleration_mm+deceleration_mm));       	   	  // accel_dist (abscisse 0 -> dist_accel) = dist*(decel/accel+decel)-- consts 
		temps_rampe_s_accel = sqrt(2*accel_dist_mm/acceleration_mm);								  // temp rampe accel = racine(2*acceldist/acceleration)
		vitesse_max_mm = temps_rampe_s_accel*acceleration_mm ;									      // vitesse max ) temps_rampe_accel * acceleration_mm 
	}
	else	   																					   	  //sinon
	{	
		temps_rampe_s_accel = vitesse_max_mm / acceleration_mm ;								      //temps rampeaccel ne change pas
		accel_dist_mm = (float)0.5*acceleration_mm*temps_rampe_s_accel*temps_rampe_s_accel;		      //accel_dist = (accel * temps_rampe²)/2
	}
	first = 1;
	origine_des_temps = t;  
	while((fabs(goal_x - current_x)>2)||(fabs(goal_y - current_y)>2)) //while goal not reached do
	{
		/*****TEMPS_DE_DEPART_DE_PERIODE_D'ECHANTILLONNAGE****/
		t0=t;
		/******************CHOIX_DU_SENS**********************/
		if ((distance_mm-((total_right+total_left)/2))>0)
			sens = 1;
		else
			sens = -1;
		/************CALCUL_DE_VITESSE_ACCELERATION***********/
		if (fabs((total_right+total_left)/2) < accel_dist_mm){ 												// si en phase d'acceleration en fonction de la distance parcourue
			prev_speed = speed_ref ;																		// instancier vitesse ref (1ère ittération prev_speed = 0
			speed_ref = sens*constrain((t-origine_des_temps)*acceleration_mm/1000,0,vitesse_max_mm);} 		//new speed_ref = sens * (temps passé*acceleration_mm/1000) ...
		/****************VITESSE_PHASE_UNIFORME***************/
		else																								//sinon 
		{
			if(first == 0){vitesse_max_mm = speed_ref;first =2;}											//speed_ref prend vitesse maximale
			speed_ref = sens*vitesse_max_mm;
		}
		/**************REGULATION_VITESSE_LINEAIRE************/
		
		previous_right_error = right_error;																	//etat précedant de l'erreur
		right_error = speed_ref - right_speed;																//calcul nouveau erreur
		i_right_error += right_error;																		//sommation du cumul de l'erreur for integration
		PWM_RB = pwm_de_base_right + kp_right * right_error - kd_right * (right_error - previous_right_error) + ki_right * i_right_error ;
// formule PID(PWM)= PWMBase + Kp*Err - Kd*(Err-ErrPrecedant) + Ki*SommmeErr
		previous_left_error = left_error;																	//etat précedant de l'erreur
		left_error = speed_ref - left_speed;																//calcul nouveau erreur
		i_left_error += left_error;																			//sommation du cumul de l'erreur for integration
		PWM_LB = pwm_de_base_left + kp_left * left_error - kd_left * (left_error - previous_left_error) + ki_left * i_left_error ; //formule PWM PID
		
		
		/****************CORRECTION_D'ORIENTATION*************/
		if (fabs(distance_mm-((total_right+total_left)/2))>40){sens = (asinf((goal_y-current_y)/sqrtf((current_x-goal_x)*(current_x-goal_x)+(current_y-goal_y)*(current_y-goal_y)))>0)? 1 : -1;
		angle = sens * rad_to_deg(acosf((goal_x-current_x)/sqrtf((current_x-goal_x)*(current_x-goal_x)+(current_y-goal_y)*(current_y-goal_y))));
		target_angle = angle-current_phi_deg;
		if (target_angle>180) target_angle -= 360;
		if (target_angle<-180) target_angle += 360;}
		left_correction  = coef_correct_erreur_angle * target_angle;
		right_correction = left_correction;
		PWM_R = PWM_RB + right_correction;
		PWM_L = PWM_LB - left_correction;
		
		/***********************EXECUTION*********************/
		run_motors();
		/***ATTENDRE_LA_FIN_DE_LA_PERIODE_D'ECHANTILLONNAGE***/
		do delta=t-t0;
		while (delta<T);
	}
		/********ARRETER_LES_MOTEURS_SI_ON_EST_ARRIVEE*********/
}

void go_non_stop(float goal_x, float goal_y,float vitesse_max_mm)
{
	angle = sens * rad_to_deg(acosf((goal_x-current_x)/sqrtf((current_x-goal_x)*(current_x-goal_x)+(current_y-goal_y)*(current_y-goal_y))));
	target_angle = angle-current_phi_deg;
	if (target_angle>180) target_angle -= 360;
	if (target_angle<-180) target_angle += 360;
	
	if (target_angle > 0){
		PWM_L = 0 ;
		while(fabs(target_angle)>1)
		{
			
			/*****TEMPS_DE_DEPART_DE_PERIODE_D'ECHANTILLONNAGE****/
			t0=t;
			/******************CHOIX_DU_SENS**********************/
				sens = 1;
			/****************VITESSE_PHASE_UNIFORME***************/
				speed_ref = sens*vitesse_max_mm;
			/**************REGULATION_VITESSE_LINEAIRE************/
			previous_right_error = right_error;
			right_error = speed_ref - right_speed;
			i_right_error += right_error;
			PWM_R = pwm_de_base_right + kp_right_curve * right_error - kd_right_curve * (right_error - previous_right_error) + ki_right_curve * i_right_error ;
			/***********************EXECUTION*********************/
			run_motors();
			angle = sens * rad_to_deg(acosf((goal_x-current_x)/sqrtf((current_x-goal_x)*(current_x-goal_x)+(current_y-goal_y)*(current_y-goal_y))));
			target_angle = angle-current_phi_deg;
			if (target_angle>180) target_angle -= 360;
			if (target_angle<-180) target_angle += 360;
			/***ATTENDRE_LA_FIN_DE_LA_PERIODE_D'ECHANTILLONNAGE***/
			do delta=t-t0;
			while (delta<T);
		}
	}
	else if(target_angle < 0)
		PWM_R = 0 ;
		while(fabs(target_angle)>1)
		{
			/*****TEMPS_DE_DEPART_DE_PERIODE_D'ECHANTILLONNAGE****/
			t0=t; 
			/******************CHOIX_DU_SENS**********************/
				sens = 1;
			/****************VITESSE_PHASE_UNIFORME***************/
				speed_ref = sens*vitesse_max_mm;
			/**************REGULATION_VITESSE_LINEAIRE************/
			previous_left_error = left_error;
			left_error = speed_ref - left_speed;
			i_left_error += left_error;
			PWM_L = pwm_de_base_left + kp_left_curve * left_error - kd_left_curve * (left_error - previous_left_error) + ki_left_curve * i_left_error ;
			/***********************EXECUTION*********************/
			run_motors();
			angle = sens * rad_to_deg(acosf((goal_x-current_x)/sqrtf((current_x-goal_x)*(current_x-goal_x)+(current_y-goal_y)*(current_y-goal_y))));
			target_angle = angle-current_phi_deg;
			if (target_angle>180) target_angle -= 360;
			if (target_angle<-180) target_angle += 360;
			/***ATTENDRE_LA_FIN_DE_LA_PERIODE_D'ECHANTILLONNAGE***/
			do delta=t-t0;
			while (delta<T);
		}
	while((fabs(goal_x - current_x)>2)||(fabs(goal_y - current_y)>2))
	{
		/*****TEMPS_DE_DEPART_DE_PERIODE_D'ECHANTILLONNAGE****/
		t0=t;
		/******************CHOIX_DU_SENS**********************/
			sens = 1;
		/****************VITESSE_PHASE_UNIFORME***************/
			speed_ref = sens*vitesse_max_mm;
		/**************REGULATION_VITESSE_LINEAIRE************/
		previous_right_error = right_error;
		right_error = speed_ref - right_speed;
		i_right_error += right_error;
		PWM_RB = pwm_de_base_right + kp_right * right_error - kd_right * (right_error - previous_right_error) + ki_right * i_right_error ;
		
		previous_left_error = left_error;
		left_error = speed_ref - left_speed;
		i_left_error += left_error;
		PWM_LB = pwm_de_base_left + kp_left * left_error - kd_left * (left_error - previous_left_error) + ki_left * i_left_error ;
		/****************CORRECTION_D'ORIENTATION*************/
		if (fabs(distance_mm-((total_right+total_left)/2))>40){sens = (asinf((goal_y-current_y)/sqrtf((current_x-goal_x)*(current_x-goal_x)+(current_y-goal_y)*(current_y-goal_y)))>0)? 1 : -1;
		angle = sens * rad_to_deg(acosf((goal_x-current_x)/sqrtf((current_x-goal_x)*(current_x-goal_x)+(current_y-goal_y)*(current_y-goal_y))));
		target_angle = angle-current_phi_deg;
		if (target_angle>180) target_angle -= 360;
		if (target_angle<-180) target_angle += 360;}
		left_correction  = coef_correct_erreur_angle * target_angle;
		right_correction = left_correction;
		PWM_R = PWM_RB + right_correction;
		PWM_L = PWM_LB - left_correction;
		/***********************EXECUTION*********************/
		run_motors();
		/***ATTENDRE_LA_FIN_DE_LA_PERIODE_D'ECHANTILLONNAGE***/
		do delta=t-t0;
		while (delta<T);
	}
		/********ARRETER_LES_MOTEURS_SI_ON_EST_ARRIVEE*********/
}

void last_go_non_stop(float goal_x, float goal_y,float vitesse_max_mm)
{
	distance_mm = sqrtf((current_x-goal_x)*(current_x-goal_x)+(current_y-goal_y)*(current_y-goal_y));
	temps_rampe_s_decel = vitesse_max_mm / deceleration_mm ;
	angle = sens * rad_to_deg(acosf((goal_x-current_x)/sqrtf((current_x-goal_x)*(current_x-goal_x)+(current_y-goal_y)*(current_y-goal_y))));
	target_angle = angle-current_phi_deg;
	if (target_angle>180) target_angle -= 360;
	if (target_angle<-180) target_angle += 360;
	if (target_angle > 0){
		PWM_L = 0 ;
		while(fabs(target_angle)>1)
		{
			
			/*****TEMPS_DE_DEPART_DE_PERIODE_D'ECHANTILLONNAGE****/
			t0=t;
			/******************CHOIX_DU_SENS**********************/
				sens = 1;
			/****************VITESSE_PHASE_UNIFORME***************/
				speed_ref = sens*vitesse_max_mm;
			/**************REGULATION_VITESSE_LINEAIRE************/
			previous_right_error = right_error;
			right_error = speed_ref - right_speed;
			i_right_error += right_error;
			PWM_R = pwm_de_base_right + kp_right_curve * right_error - kd_right_curve * (right_error - previous_right_error) + ki_right_curve * i_right_error ;
			/***********************EXECUTION*********************/
			run_motors();
			angle = sens * rad_to_deg(acosf((goal_x-current_x)/sqrtf((current_x-goal_x)*(current_x-goal_x)+(current_y-goal_y)*(current_y-goal_y))));
			target_angle = angle-current_phi_deg;
			if (target_angle>180) target_angle -= 360;
			if (target_angle<-180) target_angle += 360;
			/***ATTENDRE_LA_FIN_DE_LA_PERIODE_D'ECHANTILLONNAGE***/
			do delta=t-t0;
			while (delta<T);
		}
	}
	else if(target_angle < 0)
		PWM_R = 0 ;
		while(fabs(target_angle)>1)
		{
			/*****TEMPS_DE_DEPART_DE_PERIODE_D'ECHANTILLONNAGE****/
			t0=t;
			/******************CHOIX_DU_SENS**********************/
				sens = 1;
			/****************VITESSE_PHASE_UNIFORME***************/
				speed_ref = sens*vitesse_max_mm;
			/**************REGULATION_VITESSE_LINEAIRE************/
			previous_left_error = left_error;
			left_error = speed_ref - left_speed;
			i_left_error += left_error;
			PWM_L = pwm_de_base_left + kp_left_curve * left_error - kd_left_curve * (left_error - previous_left_error) + ki_left_curve * i_left_error ;
			/***********************EXECUTION*********************/
			run_motors();
			angle = sens * rad_to_deg(acosf((goal_x-current_x)/sqrtf((current_x-goal_x)*(current_x-goal_x)+(current_y-goal_y)*(current_y-goal_y))));
			target_angle = angle-current_phi_deg;
			if (target_angle>180) target_angle -= 360;
			if (target_angle<-180) target_angle += 360;
			/***ATTENDRE_LA_FIN_DE_LA_PERIODE_D'ECHANTILLONNAGE***/
			do delta=t-t0;
			while (delta<T);
		}	
	if (fabs(distance_mm) < (0.5*deceleration_mm*temps_rampe_s_decel*temps_rampe_s_decel))
	{
		decel_dist_mm = distance_mm*(acceleration_mm/(acceleration_mm+deceleration_mm));
		temps_rampe_s_decel = sqrt(2*decel_dist_mm/deceleration_mm);
	}
	else
	{	
		temps_rampe_s_decel = vitesse_max_mm / deceleration_mm ;
		decel_dist_mm = (float)0.5*deceleration_mm*temps_rampe_s_decel*temps_rampe_s_decel;

	}
	first = 1;
	origine_des_temps = t;
	while((fabs(goal_x - current_x)>2)||(fabs(goal_y - current_y)>2))
	{
		distance_mm_carre = sqrtf((current_x-goal_x)*(current_x-goal_x)+(current_y-goal_y)*(current_y-goal_y));
		distance = (int)fabs(distance_mm-((total_right+total_left)/2));
		/*****TEMPS_DE_DEPART_DE_PERIODE_D'ECHANTILLONNAGE****/
		t0=t;
		/******************CHOIX_DU_SENS**********************/
			sens = 1;
	  /************CALCUL_DE_VITESSE_DECELERATION***********/
		if (fabs(distance_mm_carre) < decel_dist_mm){
			if(first == 1){origine_des_temps = t;first =0;}
			speed_ref = sens*constrain(vitesse_max_mm-(((t-origine_des_temps)/1000)*(deceleration_mm)),0,vitesse_max_mm);
		}
		/****************VITESSE_PHASE_UNIFORME***************/
		else
		{
			if(first == 0){vitesse_max_mm = speed_ref;first =2;}
			speed_ref = sens*vitesse_max_mm;
		}
		/**************REGULATION_VITESSE_LINEAIRE************/
		previous_right_error = right_error;
		right_error = speed_ref - right_speed;
		i_right_error += right_error;
		PWM_RB = pwm_de_base_right + kp_right * right_error - kd_right * (right_error - previous_right_error) + ki_right * i_right_error ;
		
		previous_left_error = left_error;
		left_error = speed_ref - left_speed;
		i_left_error += left_error;
		PWM_LB = pwm_de_base_left + kp_left * left_error - kd_left * (left_error - previous_left_error) + ki_left * i_left_error ;
		/****************CORRECTION_D'ORIENTATION*************/
		if (fabs(distance_mm-((total_right+total_left)/2))>40){sens = (asinf((goal_y-current_y)/sqrtf((current_x-goal_x)*(current_x-goal_x)+(current_y-goal_y)*(current_y-goal_y)))>0)? 1 : -1;
		angle = sens * rad_to_deg(acosf((goal_x-current_x)/sqrtf((current_x-goal_x)*(current_x-goal_x)+(current_y-goal_y)*(current_y-goal_y))));
		target_angle = angle-current_phi_deg;
		if (target_angle>180) target_angle -= 360;
		if (target_angle<-180) target_angle += 360;}
		left_correction  = coef_correct_erreur_angle * target_angle;
		right_correction = left_correction;
		PWM_R = PWM_RB + right_correction;
		PWM_L = PWM_LB - left_correction;
		
		/***********************EXECUTION*********************/
		run_motors();
		/***ATTENDRE_LA_FIN_DE_LA_PERIODE_D'ECHANTILLONNAGE***/
		do delta=t-t0;
		while (delta<T);
	}
		/********ARRETER_LES_MOTEURS_SI_ON_EST_ARRIVEE*********/
		stop_motors();
}
void move_distance_asservi(float goal_x, float goal_y,float vitesse_max_mm)
{
	distance_mm = sqrtf((current_x-goal_x)*(current_x-goal_x)+(current_y-goal_y)*(current_y-goal_y));
	distance_fixe = distance_mm;
	temps_rampe_s_accel = vitesse_max_mm / acceleration_mm ;
	temps_rampe_s_decel = vitesse_max_mm / deceleration_mm ;
	init();  
	if (fabs(distance_mm) < (0.5*deceleration_mm*temps_rampe_s_decel*temps_rampe_s_decel+0.5*acceleration_mm*temps_rampe_s_accel*temps_rampe_s_accel))
	{
		accel_dist_mm = distance_mm*(deceleration_mm/(acceleration_mm+deceleration_mm));
		decel_dist_mm = distance_mm*(acceleration_mm/(acceleration_mm+deceleration_mm));
		temps_rampe_s_accel = sqrt(2*accel_dist_mm/acceleration_mm);
		temps_rampe_s_decel = sqrt(2*decel_dist_mm/deceleration_mm);
		vitesse_max_mm = temps_rampe_s_accel*acceleration_mm ;
		accel_dist_mm *= (1-anticipation_accel) ;
		decel_dist_mm *= (1+anticipation_decel) ;
	}
	else
	{	
		temps_rampe_s_accel = vitesse_max_mm / acceleration_mm ;
		temps_rampe_s_decel = vitesse_max_mm / deceleration_mm ;
		accel_dist_mm = (float)0.5*acceleration_mm*temps_rampe_s_accel*temps_rampe_s_accel;
		decel_dist_mm = (float)0.5*deceleration_mm*temps_rampe_s_decel*temps_rampe_s_decel;
		accel_dist_mm *= (1-anticipation_accel) ;
		decel_dist_mm *= (1+anticipation_decel) ;
	}
	first = 1;
	origin_x = current_x;
	origin_y = current_y;
	sens_or = (asinf((goal_y-current_y)/sqrtf((current_x-goal_x)*(current_x-goal_x)+(current_y-goal_y)*(current_y-goal_y)))>0)? 1 : -1;
	angle = sens_or * rad_to_deg(acosf((goal_x-current_x)/sqrtf((current_x-goal_x)*(current_x-goal_x)+(current_y-goal_y)*(current_y-goal_y))));
	target_angle = angle-current_phi_deg;
	if (target_angle>180) target_angle -= 360;
	if (target_angle<-180) target_angle += 360;
	origine_des_temps = t;
	if (target_angle<=90||target_angle>=-90)
	{
	while(fabs(distance_fixe-distance_mm_carre)>0.5)//(distance_mm > 2)//(fabs(distance_fixe-((total_right+total_left)/2))>0.5)//((fabs(distance_mm-total_left)>0.5)||(fabs(distance_mm-total_right)>0.5))//(fabs(distance_mm-((total_right+total_left)/2))>0.5)
	{  
		distance_mm = sqrtf((current_x-goal_x)*(current_x-goal_x)+(current_y-goal_y)*(current_y-goal_y));
		distance_mm_carre = sqrtf((current_x-origin_x)*(current_x-origin_x)+(current_y-origin_y)*(current_y-origin_y));
		distance = (int)fabs(distance_mm);
		/*****TEMPS_DE_DEPART_DE_PERIODE_D'ECHANTILLONNAGE****/
		t0=t;
		/******************CHOIX_DU_SENS**********************/
		if (fabs(distance_fixe)>fabs(distance_mm_carre))
			sens = 1;
		else
			sens = -1;
		/************CALCUL_DE_VITESSE_ACCELERATION***********/
		if (distance_mm_carre < accel_dist_mm){
			prev_speed = speed_ref ;
			speed_ref = sens*constrain((t-origine_des_temps)*acceleration_mm/1000,0,vitesse_max_mm);}
	  /************CALCUL_DE_VITESSE_DECELERATION***********/
		else 	
		if (distance_mm < decel_dist_mm){
			if(first == 1){origine_des_temps = t;first =0;}//vitesse_max_mm = (float)robot_speed;}
			speed_ref = sens*constrain(vitesse_max_mm-(((t-origine_des_temps)/1000)*(deceleration_mm)),0,vitesse_max_mm);
		}
		/****************VITESSE_PHASE_UNIFORME***************/
		else
		{
			if(first == 0){
				first =2;
				temps_rampe_s_decel = (float)robot_speed / deceleration_mm ;
				decel_dist_mm = (float)0.5*deceleration_mm*temps_rampe_s_decel*temps_rampe_s_decel;
			}
			speed_ref = sens*vitesse_max_mm;
		}
		/**************REGULATION_VITESSE_LINEAIRE************/
		if(speed_ref < 50&&speed_ref>0) speed_ref = 20;
		if(speed_ref < 0) speed_ref = -20;
		previous_right_error = right_error;
		right_error = speed_ref - right_speed;
		i_right_error += right_error; 
		PWM_RB = sens*pwm_de_base_right + kp_right * right_error - kd_right * (right_error - previous_right_error) + ki_right * i_right_error ;
		previous_left_error = left_error;
		left_error = speed_ref - left_speed;
		i_left_error += left_error;
		PWM_LB = sens*pwm_de_base_left + kp_left * left_error - kd_left * (left_error - previous_left_error) + ki_left * i_left_error ;
		//if (speed_ref < robot_speed) stop_motors();
		/****************CORRECTION_D'ORIENTATION*************/
		if (distance_mm>100) 
		{
		sens_or = (asinf((goal_y-current_y)/sqrtf((current_x-goal_x)*(current_x-goal_x)+(current_y-goal_y)*(current_y-goal_y)))>0)? 1 : -1;
		angle = sens_or * rad_to_deg(acosf((goal_x-current_x)/sqrtf((current_x-goal_x)*(current_x-goal_x)+(current_y-goal_y)*(current_y-goal_y))));
		target_angle = angle-current_phi_deg;
		if (target_angle>180) target_angle -= 360;
		if (target_angle<-180) target_angle += 360;
		target_angler = target_angle;
		target = (int)angle;
		}
		else
		{
		sens_or = (asinf((goal_y-current_y)/sqrtf((current_x-goal_x)*(current_x-goal_x)+(current_y-goal_y)*(current_y-goal_y)))>0)? 1 : -1;
		angle = sens_or * rad_to_deg(acosf((goal_x-current_x)/sqrtf((current_x-goal_x)*(current_x-goal_x)+(current_y-goal_y)*(current_y-goal_y))));
		target_angler = angle-current_phi_deg;
		if (target_angler>180) target_angler -= 360;
		if (target_angler<-180) target_angler += 360;
		}
		if (speed_ref<=0)
			sens_or = -sens_or;
		target = (int)angle;
		left_correction  = coef_correct_erreur_angle_asservi * target_angle;
		right_correction = left_correction;
		PWM_R = PWM_RB + right_correction;
		PWM_L = PWM_LB - left_correction;
		/***********************EXECUTION*********************/
		run_motors();
		/***ATTENDRE_LA_FIN_DE_LA_PERIODE_D'ECHANTILLONNAGE***/
		do delta=t-t0;
		while (delta<T);
	}
	}
	else
	{
		while(fabs(distance_fixe-distance_mm_carre)>0.5)//(distance_mm > 2)//(fabs(distance_fixe-((total_right+total_left)/2))>0.5)//((fabs(distance_mm-total_left)>0.5)||(fabs(distance_mm-total_right)>0.5))//(fabs(distance_mm-((total_right+total_left)/2))>0.5)
	{  
		distance_mm = sqrtf((current_x-goal_x)*(current_x-goal_x)+(current_y-goal_y)*(current_y-goal_y));
		distance_mm_carre = sqrtf((current_x-origin_x)*(current_x-origin_x)+(current_y-origin_y)*(current_y-origin_y));
		distance = (int)fabs(distance_mm);
		/*****TEMPS_DE_DEPART_DE_PERIODE_D'ECHANTILLONNAGE****/
		t0=t;
		/******************CHOIX_DU_SENS**********************/
		if (fabs(distance_fixe)>fabs(distance_mm_carre))
			sens = -1;
		else
			sens = 1;
		/************CALCUL_DE_VITESSE_ACCELERATION***********/
		if (distance_mm_carre < accel_dist_mm){
			prev_speed = speed_ref ;
			speed_ref = sens*constrain((t-origine_des_temps)*acceleration_mm/1000,0,vitesse_max_mm);}
	  /************CALCUL_DE_VITESSE_DECELERATION***********/
		else 
			
		if (distance_mm < decel_dist_mm){
			if(first == 1){origine_des_temps = t;first =0;}//vitesse_max_mm = (float)robot_speed;}
			speed_ref = sens*constrain(vitesse_max_mm-(((t-origine_des_temps)/1000)*(deceleration_mm)),0,vitesse_max_mm);
		}
		/****************VITESSE_PHASE_UNIFORME***************/
		else
		{
			if(first == 0){
				first =2;
				temps_rampe_s_decel = (float)robot_speed / deceleration_mm ;
				decel_dist_mm = (float)0.5*deceleration_mm*temps_rampe_s_decel*temps_rampe_s_decel;
			}
			speed_ref = sens*vitesse_max_mm;
		}
		/**************REGULATION_VITESSE_LINEAIRE************/
		if(speed_ref >0) speed_ref = 20;
		if(speed_ref > -50&&speed_ref<0 ) speed_ref = -20;
		previous_right_error = right_error;
		right_error = speed_ref - right_speed;
		i_right_error += right_error; 
		PWM_RB = sens*pwm_de_base_right + kp_right * right_error - kd_right * (right_error - previous_right_error) + ki_right * i_right_error ;
		previous_left_error = left_error;
		left_error = speed_ref - left_speed;
		i_left_error += left_error;
		PWM_LB = sens*pwm_de_base_left + kp_left * left_error - kd_left * (left_error - previous_left_error) + ki_left * i_left_error ;
		//if (speed_ref < robot_speed) stop_motors();
		/****************CORRECTION_D'ORIENTATION*************/
		if (distance_mm>100) 
		{
		sens_or = (asinf((goal_y-current_y)/sqrtf((current_x-goal_x)*(current_x-goal_x)+(current_y-goal_y)*(current_y-goal_y)))>0)? 1 : -1;
		angle = sens_or * rad_to_deg(acosf((goal_x-current_x)/sqrtf((current_x-goal_x)*(current_x-goal_x)+(current_y-goal_y)*(current_y-goal_y))));
		target_angle = angle-current_phi_deg;
		if (target_angle>180) target_angle -= 360;
		if (target_angle<-180) target_angle += 360;
		target_angler = target_angle;
		target = (int)angle;
		}
		else
		{
		sens_or = (asinf((goal_y-current_y)/sqrtf((current_x-goal_x)*(current_x-goal_x)+(current_y-goal_y)*(current_y-goal_y)))>0)? 1 : -1;
		angle = sens_or * rad_to_deg(acosf((goal_x-current_x)/sqrtf((current_x-goal_x)*(current_x-goal_x)+(current_y-goal_y)*(current_y-goal_y))));
		target_angler = angle-current_phi_deg;
		if (target_angler>180) target_angler -= 360;
		if (target_angler<-180) target_angler += 360;
		}
		if (speed_ref<0)
			sens_or = -sens_or;
		target = (int)angle;
		left_correction  = coef_correct_erreur_angle_asservi * target_angle;
		right_correction = left_correction;
		PWM_R = PWM_RB + right_correction;
		PWM_L = PWM_LB - left_correction;
		/***********************EXECUTION*********************/
		run_motors();
		/***ATTENDRE_LA_FIN_DE_LA_PERIODE_D'ECHANTILLONNAGE***/
		do delta=t-t0;
		while (delta<T);
	}
	}
		/********ARRETER_LES_MOTEURS_SI_ON_EST_ARRIVEE*********/
		stop_motors();
}


void doura_asservi()
{
}

void asservissement_orientation (float angle)
{
	init();  
	first = 1;
	origine_des_temps = t;
	while(fabs(distance_mm-((total_right+total_left)/2))>0.5)//((fabs(distance_mm-total_left)>0.5)||(fabs(distance_mm-total_right)>0.5))//(fabs(distance_mm-((total_right+total_left)/2))>0.5)
	{
		//distance_mm_carre = sqrtf((current_x-goal_x)*(current_x-goal_x)+(current_y-goal_y)*(current_y-goal_y));
		distance = (int)fabs(distance_mm-((total_right+total_left)/2));
		/*****TEMPS_DE_DEPART_DE_PERIODE_D'ECHANTILLONNAGE****/
		t0=t;
		/******************CHOIX_DU_SENS**********************/
		if ((distance_mm-((total_right+total_left)/2))>0)
			sens = 1;
		else
			sens = -1;
		/****************CORRECTION_D'ORIENTATION*************/
		target_angle = angle-current_phi_deg;
		if (target_angle>180) target_angle -= 360;
		if (target_angle<-180) target_angle += 360;
		left_correction  = coef_correct_erreur_angle_asservi * target_angle ;//+ kd_orientate ;
		right_correction = left_correction;
		PWM_R = PWM_RB + right_correction;
		PWM_L = PWM_LB - left_correction;
		
		/***********************EXECUTION*********************/
		run_motors();
		/***ATTENDRE_LA_FIN_DE_LA_PERIODE_D'ECHANTILLONNAGE***/
		do delta=t-t0;
		while (delta<T);
	}
		/********ARRETER_LES_MOTEURS_SI_ON_EST_ARRIVEE*********/
		stop_motors();
}	

void rotate(float angle, float speed)
{
	init();
	distance_mm = angle * PI * spacing_encoder/ 180;
	temps_rampe_s_accel = speed / acceleration_or_mm ;
	temps_rampe_s_decel = speed / deceleration_or_mm ;
	init();  
	if (fabs(distance_mm) < (0.5*deceleration_or_mm*temps_rampe_s_decel*temps_rampe_s_decel+0.5*acceleration_or_mm*temps_rampe_s_accel*temps_rampe_s_accel))
	{
		accel_dist_mm = distance_mm*(deceleration_mm/(acceleration_or_mm+deceleration_or_mm));
		decel_dist_mm = distance_mm*(acceleration_mm/(acceleration_or_mm+deceleration_or_mm));
		temps_rampe_s_accel = sqrt(2*accel_dist_mm/acceleration_or_mm);
		temps_rampe_s_decel = sqrt(2*decel_dist_mm/deceleration_or_mm);
		speed = temps_rampe_s_accel*acceleration_or_mm ;
		accel_dist_mm *= (1-anticipation_accel) ;
		decel_dist_mm *= (1+anticipation_decel) ;
	}
	else
	{	
		temps_rampe_s_accel = speed / acceleration_or_mm ;
		temps_rampe_s_decel = speed / deceleration_or_mm ;
		accel_dist_mm = (float)0.5*acceleration_or_mm*temps_rampe_s_accel*temps_rampe_s_accel;
		decel_dist_mm = (float)0.5*deceleration_or_mm*temps_rampe_s_decel*temps_rampe_s_decel;
		accel_dist_mm *= (1-anticipation_accel) ;
		decel_dist_mm *= (1+anticipation_decel) ;
	}
	origine_des_temps = t;
	first = 1;
	origin_x = current_x;
	origin_y = current_y;
	
	while(fabs(current_phi_deg - angle)<1)//((fabs(total_right-total_left)<fabs(goal_distance_mm)))
	{
		t0=t;
		distance_mm_carre = sqrtf((current_x-origin_x)*(current_x-origin_x)+(current_y-origin_y)*(current_y-origin_y));
		if (current_phi_deg-angle>=0)
			sens = 1;
		else
			sens = -1;
		/************CALCUL_DE_VITESSE_ACCELERATION***********/
		if (fabs((total_right-total_left)) < accel_dist_mm){
			speed_ref = sens*constrain((t-origine_des_temps)*acceleration_mm/1000,0,speed);}
	  /************CALCUL_DE_VITESSE_DECELERATION***********/
		else 	
		if (fabs((total_right-total_left)-goal_distance_mm) < decel_dist_mm){
			if(first == 1){origine_des_temps = t;first =0;}//vitesse_max_mm = (float)robot_speed;}
			speed_ref = sens*constrain(speed-(((t-origine_des_temps)/1000)*(deceleration_mm)),0,speed);
		}
		/****************VITESSE_PHASE_UNIFORME***************/
		else
		{
			if(first == 0){
				first =2;
				temps_rampe_s_decel = (float)robot_speed / deceleration_mm ;
				decel_dist_mm = (float)0.5*deceleration_mm*temps_rampe_s_decel*temps_rampe_s_decel;
			}
			speed_ref = sens*speed;
		}

		previous_error = right_error;
		right_error = speed_ref - right_speed;
		i_right_error += right_error;
		PWM_RB = sens*pwm_de_base_right + kp_right_rot * right_error + ki_right_rot * i_right_error - kd_right_rot * (right_error-previous_error);

		left_error = - speed_ref - left_speed;
		i_left_error += left_error;
		PWM_LB = -sens*pwm_de_base_right + kp_left_rot *  left_error + ki_left_rot * i_left_error - kd_left_rot * (right_error-previous_error);

		distance_mm_carre = sqrtf((current_x-origin_x)*(current_x-origin_x)+(current_y-origin_y)*(current_y-origin_y));
		
		/*if (distance_mm>100) 
		{
		sens_or = (asinf((origin_y-current_y)/sqrtf((current_x-origin_x)*(current_x-origin_x)+(current_y-origin_y)*(current_y-origin_y)))>0)? 1 : -1;
		angly = sens_or * rad_to_deg(acosf((origin_x-current_x)/sqrtf((current_x-origin_x)*(current_x-origin_x)+(current_y-origin_y)*(current_y-origin_y))));
		target_angle = angly-current_phi_deg;
		if (target_angle>180) target_angle -= 360;
		if (target_angle<-180) target_angle += 360;
		target_angler = target_angle;
		}
		else
		{
		sens_or = (asinf((origin_y-current_y)/sqrtf((current_x-origin_x)*(current_x-origin_x)+(current_y-origin_y)*(current_y-origin_y)))>0)? 1 : -1;
		angly = sens_or * rad_to_deg(acosf((origin_x-current_x)/sqrtf((current_x-origin_x)*(current_x-origin_y)+(current_y-origin_y)*(current_y-origin_y))));
		target_angler = angly-current_phi_deg;
		if (target_angler>180) target_angler -= 360;
		if (target_angler<-180) target_angler += 360;
		}
		
		if (speed_ref<=0)
			sens_or = -sens_or;*/
			
		sens_or = (asinf((origin_y-current_y)/sqrtf((current_x-origin_x)*(current_x-origin_x)+(current_y-origin_y)*(current_y-origin_y)))>0)? 1 : -1;
		angly = sens_or * rad_to_deg(acosf((origin_x-current_x)/sqrtf((current_x-origin_x)*(current_x-origin_x)+(current_y-origin_y)*(current_y-origin_y))));
		target_angle = angle-current_phi_deg;
		if (target_angle>180) target_angle -= 360;
		if (target_angle<-180) target_angle += 360;
		if ((target_angle<=90||target_angle>=-90)&&(sens == 1))
		{
			left_correction = coef_correct_deplacement * distance_mm_carre;
		}
		else if ((target_angle<=90||target_angle>=-90)&&(sens == -1))
		{
			left_correction = -coef_correct_deplacement * distance_mm_carre;
		}
		else if ((target_angle>90||target_angle<-90)&&(sens == 1))
		{
			left_correction = coef_correct_deplacement * distance_mm_carre;
		}
		else if ((target_angle>90||target_angle<-90)&&(sens == -1))
		{
			left_correction = -coef_correct_deplacement * distance_mm_carre;
		}
		left_correction += coef_correct_angle_or * (total_right + total_left);
		right_correction = - left_correction;
		PWM_R = PWM_RB + right_correction;
		PWM_L = PWM_LB - left_correction;

		run_motors();
		do delta=t-t0;
		while (delta<T);
	}
	stop_motors();
}
