#include <Robot_Navi_Euro20.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ros.h>
#include "stmRos.h"
#include <std_msgs/Bool.h>
#include <ros.h>

std_msgs::Bool start;
ros::Publisher tirettePub("/tirette", &start);

extern volatile long millis,t;
extern unsigned long long int delta,t0;
bool started=false;

extern ros::NodeHandle nh;

//Motors related variables
extern "C" volatile long d_right;
extern "C" volatile long d_left;
extern "C" int PWM_R,PWM_L;
extern "C" int PWM_Max;
extern "C" int PWM_R_Min,PWM_L_Min;
extern "C" int PWM_R_Min_Rot,PWM_L_Min_Rot;
extern "C" int PWM_R_sign_counter,PWM_L_sign_counter;

//Odometry related variables
extern "C" volatile float total_right,total_left, total_centre;
extern "C" volatile float  current_x,current_y,current_phi_deg,current_phi_rad;
extern "C" float ref_x, ref_y;
extern "C" float spacing_encoder,spacing_wheel,dec;
extern "C" volatile double right_speed, left_speed;

bool tirette=true;

int sum=0;

//Robot Navi Related Variables
float goal_distance, target_angle;
float accel_dist, decel_dist;
int PWM_LB,PWM_RB;
int coef_correct_angle=60; //50 Correction rotate
int right_correction=0,left_correction=0;

//Speed Regulation
float speed_ref, ramp = 400, rampR=100,rampC=500;//max 1500 //deja 800 vitesse kbira
int sens;
double right_error=0,i_right_error=0;
double left_error=0,i_left_error=0;
float kp = 10, ki = 1.15;

//Move
int coef_correct_dist = 25;//25

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
int evitementFlag=1;

float x_obst,y_obst;
float x_obst_abs,y_obst_abs;

float l1,l2,l3,r1,r2,r3 ;
double xN,yN;
float wl1=70,wl2=50,wl3=10,wr1=70,wr2=-50,wr3=-10 ;

float constrain (float x,float min,float Max){
	if (x<min) return min;
	if (x>Max) return Max;
	else return(x);
}

void init (void){
	total_right=0;
	total_left=0;
	total_centre=0;
	PWM_L_sign_counter=0;
	PWM_R_sign_counter=0;
	i_right_error = 0;
	i_left_error = 0;
	right_error=0;
	left_error=0;
}

void set_acc_dec(float& distance,float& speed){
	//Set accel/decel distance
	if (fabs(distance) < (speed*speed/ramp))
	{
		accel_dist = fabs(distance)/2;
		decel_dist = fabs(distance)/2;
		speed = sqrt (2*ramp*accel_dist);
	}
	else
	{
		accel_dist = (float)0.5*speed*speed/ramp;
		decel_dist = (float)0.5*speed*speed/ramp;
	}
}

void regulation (float& distance,float& speed)){
	t0=t;
	//Accel/Decel Speed Set
	if (((total_right+total_left)/2 -distance)<0)
		sens = 1;
	else
		sens = -1;
	if (fabs((total_right+total_left)/2) < accel_dist)
		speed_ref = sens*50+sens*(constrain(sqrt (ramp*fabs(total_right+total_left))-50,0,1000));

	else if (fabs((total_right+total_left)/2 -distance) < decel_dist)
		speed_ref = sens*10+sens*constrain((sqrt(2*ramp*fabs((total_right+total_left)/2 -distance))-10),0,1000);//fabs((total_right+total_left)/2 -distance)
	else
		speed_ref = sens*speed;
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
	//Orientation Correction
	left_correction = coef_correct_dist * (total_right-total_left);
	right_correction = - left_correction;

	PWM_R = PWM_RB + right_correction ;
	PWM_L = PWM_LB + left_correction;

	//Execution
	run_motors();
	do delta=t-t0;
	while (delta<T);
}

void move_distance(float distance,float speed){
	init();
	set_acc_dec(distance,speed);
	while(fabs((total_right+total_left)/2-distance)>1)
	{
		x_obst_abs=current_x+x_obst*cosf(current_phi_rad);
		y_obst_abs=current_y+y_obst*sinf(current_phi_rad);
//			loop();
//			HAL_Delay(5);
		nh.spinOnce();
        if(!evitementFlag && !outsideofmap(x_obst_abs,y_obst_abs))
        	break;
	regulation(distance,speed);
	}
	stop_motors();
}

void asta3(float distance,float speed,  int s, int count){
	//asta3 Right + Left: s==abs(d_right)+abs(d_left); AND count == 200;
	//asta3 Left: s== abs(d_left); AND count ==130
	int compteur=0;
	init();
	set_acc_dec(distance,speed);
	while(fabs((total_right+total_left)/2-distance)>1 && evitementFlag)
	{
		//ASTAAAAA33
		sum=s;
		compteur++;
		if(compteur==count){
			if(sum<1)
				break;
			compteur=0;
			sum=0;
		}
		//END ASTAAAAA33
		nh.spinOnce();
		regulation(distance,speed);
	}
	compteur=0;
	sum=0;
	stop_motors();
}

void Calcul_Curv(float x, float y, float phi_target) {
	//Neww calcul curv 2023 :p
	//fazet sens felekher ki tetnaha robot ywalli ynajem ywakher fel curv mais c'est pas pratique
    corde = 0;
    corde_angle = 0;
    Rayon = 0;
    Xc = 0;
    Yc = 0;
    tetaC = 0;
    phi_target_rad = (double)PI * ((phi_target) / 180);
    while (phi_target_rad>PI)
    	phi_target_rad -= 2*PI;
    while (phi_target_rad<-PI)
   		phi_target_rad += 2*PI;

    //Calcul de la corde
    corde = sqrtf((pow((x - current_x), 2)) + (pow((y - current_y), 2)));

    //Calcul des coordonnées du centre de la courbure

    if ((phi_target == 0) || (phi_target == 180) || (phi_target == -180)) {
        if ((y == 0) && (current_y == 0))
            current_y = 0.000000001; //Résolution du probleme de division par zero
        Xc = x;
        Yc = (float)(((current_x - x) / (y - current_y)) * Xc) + (0.5f * (y + current_y)) - ((pow(current_x, 2) - pow(x, 2)) / (2 * (y - current_y)));
    }
    else {
        Xc = (float)((cosf(phi_target_rad) / sinf(phi_target_rad)) * x + (((current_x * current_x) - (x * x)) / (2 * (y - current_y))) - (0.5f * (current_y - y))) / ((cosf(phi_target_rad) / sinf(phi_target_rad)) + ((current_x - x) / (y - current_y)));
		Yc=(float)((cosf(phi_target_rad)/sinf(phi_target_rad))*(x-Xc))+y;
		}
	//Calcul du rayon de la courbure
	Rayon=sqrtf((pow((x-Xc),2)) + (pow((y-Yc),2)));
  	//Calcul de l'angle de la corde
	sens = (asinf((y-current_y)/sqrtf((current_x-x)*(current_x-x)+(current_y-y)*(current_y-y)))>0)? 1 : -1;
	corde_angle = sens * rad_to_deg(acosf((x-current_x)/sqrtf((current_x-x)*(current_x-x)+(current_y-y)*(current_y-y))));
	//Calcul de la l'angle parcouru tout au long de la courbure [0,180] en valeur absolue
	tetaC=rad_to_deg (2*asin((double)(corde/(2*Rayon))));
	//Calcul du sens_de_mouvement {recupler ou avancer}
	if((phi_target-corde_angle>=90) ||(phi_target- corde_angle<=-90))
    	sens_de_mouvement=-1;
	else
    	sens_de_mouvement=1;

	//Calcul du sens de rotation {trigonometique ou horraire}
	if((sens_de_mouvement==1 && (phi_target-corde_angle)<0) || (sens_de_mouvement==-1 && (-phi_target+corde_angle)<0))
    	tetaC=-1*tetaC;
    	
	//Calcul de l'orientation initiale avant de commencer la courbure
	phi_prim = phi_target - tetaC;
	sens_de_mouvement=1;
}

void Calcul_speed_Refs(float theta,float R,int sens_de_mouvement, float speed){
	// Calcul des vitesses de chaque roue selon la courbure et le sens de mouvement
	if(sens_de_mouvement==1){
		if (theta>=0) {
			speed_refL=speed*((R-(spacing_wheel*0.5f))/R);
			speed_refR=speed*((R+(spacing_wheel*0.5f))/R);
		}
		else if (theta<0) {
			speed_refR=speed*((R-(spacing_wheel*0.5f))/R);
			speed_refL=speed*((R+(spacing_wheel*0.5f))/R);
			}
	}
	else{
		if (theta<0) {
			speed_refL=speed*((R-(spacing_wheel*0.5f))/R);
			speed_refR=speed*((R+(spacing_wheel*0.5f))/R);
		}
		else if (theta>=0) {
			speed_refR=speed*((R-(spacing_wheel*0.5f))/R);
			speed_refL=speed*((R+(spacing_wheel*0.5f))/R);
		}
	}
}

void curv(float R,float theta ,float speed){
	//curv
	init();
	theta=(theta*PI)/180;
	goalC=fabs(R*theta);
	remain_distC=goalC-fabs(total_centre);
	speedC=speed;
	if (goalC < (speedC*speedC/rampC)) {
		accel_dist = fabs(remain_distC)/2;
		decel_dist = fabs(remain_distC)/2;
		speedC = speedC* accel_dist / ((float)0.5*speedC*speedC/rampC);}
	else{
		accel_dist = (float)0.5*speedC*speedC/rampC;
		decel_dist = (float)0.5*speedC*speedC/((rampC));
	}
	nh.spinOnce();
	while(((((remain_distC)>=2)) ||  ((remain_distC)<=-2)) && evitementFlag) {
		t0=t;
		nh.spinOnce();
		goalC=fabs(R*theta);
	  remain_distC=goalC-fabs(total_centre);
	  if ((fabs(total_centre)) < accel_dist){
			speed_refC = sens_de_mouvement*50+sens_de_mouvement*(speedC-50)*fabs((total_centre)/(accel_dist));
		}
		else if (fabs((remain_distC)) < decel_dist)
			speed_refC = sens_de_mouvement*10+sens_de_mouvement*(speedC-10)*fabs((remain_distC)/(decel_dist));
		else
			speed_refC = sens_de_mouvement*speedC;
		Calcul_speed_Refs( theta, R, sens_de_mouvement,speed_refC);
		//Right wheel regulation
		right_error = speed_refR - right_speed;
		i_right_error += right_error;
		PWM_R = kpR * right_error + kiR * i_right_error;
		//Left wheel regulation
		left_error = speed_refL - left_speed;
		i_left_error += left_error;
		PWM_L = kpL * left_error + kiL * i_left_error;
		//Execution
		run_motors();
		do delta=t-t0;
		while (delta<T);
	  }
  	stop_motors();
}

void Robot_locateCurv(float x, float y, float phi_target, float speed) {
	// robot locate curv
	Calcul_Curv(x,y,phi_target);
	orientate2(phi_prim,speed);
	curv(Rayon,tetaC,speed);
}

void rotate(float angle, float speed){
	init();
	//Set accel/decel distance
	goal_distance = angle * PI * spacing_encoder/ 180;
	if (fabs(goal_distance) < (2*speed*speed/rampR))
	{
		accel_dist = fabs(goal_distance)/2;
		decel_dist = fabs(goal_distance)/2;
		speed = sqrt(rampR*accel_dist);
	}
	else
	{
		accel_dist = (float)speed*speed/rampR;
		decel_dist = (float)speed*speed/rampR;
	}
	while( fabs(total_right-total_left)<fabs(goal_distance)&& evitementFlag )
	{
//		nh.spinOnce();
		t0=t;
		//Accel/Decel Speed Set
		if (((total_right-total_left)-goal_distance)<0)
			sens = 1;
		else
			sens = -1;
		if (fabs((total_right-total_left)) < accel_dist)
			speed_ref = sens*constrain(sqrt(rampR*fabs(total_right-total_left)),50,1000);
		else if (fabs((total_right-total_left)-goal_distance) < decel_dist)
			speed_ref = sens*constrain(sqrt(rampR*fabs((total_right-total_left)-goal_distance)),10,1000);
		else
			speed_ref = sens*speed;
		//Right wheel regulation
		right_error = speed_ref - right_speed;
		i_right_error += right_error;
		PWM_RB = kp * right_error + ki * i_right_error;
		//Left wheel regulation
		left_error = - speed_ref - left_speed;
		i_left_error += left_error;
		PWM_LB = kp * left_error + ki * i_left_error;
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

void orientate (float orientation, float speed){
	target_angle = orientation - current_phi_deg;
	if (target_angle>180) target_angle -= 360;
	if (target_angle<-180) target_angle += 360;
	rotate(target_angle,speed);
}

void rotate2(float angle, float speed){
	init();
	//Set accel/decel distance
	goal_distance = angle * PI * spacing_encoder/ 180;
	if (fabs(goal_distance) < (2*speed*speed/rampR)){
		accel_dist = fabs(goal_distance)/2;
		decel_dist = fabs(goal_distance)/2;
		speed = sqrt(rampR*accel_dist);
	}
	else{
		accel_dist = (float)speed*speed/rampR;
		decel_dist = (float)speed*speed/rampR;
	}
	while( fabs(total_right-total_left)<fabs(goal_distance)){
//		nh.spinOnce();
		t0=t;
		//Accel/Decel Speed Set
		if (((total_right-total_left)-goal_distance)<0)
			sens = 1;
		else
			sens = -1;
		if (fabs((total_right-total_left)) < accel_dist)
			speed_ref = sens*constrain(sqrt(rampR*fabs(total_right-total_left)),50,1000);
		else if (fabs((total_right-total_left)-goal_distance) < decel_dist)
			speed_ref = sens*constrain(sqrt(rampR*fabs((total_right-total_left)-goal_distance)),10,1000);
		else
			speed_ref = sens*speed;
		//Right wheel regulation
		right_error = speed_ref - right_speed;
		i_right_error += right_error;
		PWM_RB = kp * right_error + ki * i_right_error;
		//Left wheel regulation
		left_error = - speed_ref - left_speed;
		i_left_error += left_error;
		PWM_LB = kp * left_error + ki * i_left_error;
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

void orientate2(float orientation, float speed){
	target_angle = orientation - current_phi_deg;
	if (target_angle>180) target_angle -= 360;
	if (target_angle<-180) target_angle += 360;
	rotate2(target_angle,speed);
}

void Robot_Locate(float goal_x, float goal_y, float speed){
  while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_11)==GPIO_PIN_SET){
	  tirette=true;
	  nh.spinOnce();
  }
  if(started==false) {
	started=true;
	start.data=true;
	tirettePub.publish(&start);
	nh.spinOnce();
	}
	tirette=false;
	sens = (asinf((goal_y-current_y)/sqrtf((current_x-goal_x)*(current_x-goal_x)+(current_y-goal_y)*(current_y-goal_y)))>0)? 1 : -1;
	target_angle = sens * rad_to_deg(acosf((goal_x-current_x)/sqrtf((current_x-goal_x)*(current_x-goal_x)+(current_y-goal_y)*(current_y-goal_y))));
	orientate(target_angle,speed);
	rotateAck();
	goal_distance = sqrtf((current_x-goal_x)*(current_x-goal_x)+(current_y-goal_y)*(current_y-goal_y));
	move_distance(goal_distance,speed);
}

float deg_to_rad(float x){
	return (float)x/180*M_PI;
}

bool outsideofmap(float x, float y ){
	return(fabs(x)>800||y<0||y>2800);
}

void Robot_Locateobst(float goal_x, float goal_y, float speed){
	while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_11)==GPIO_PIN_SET){
		  tirette=true;
		  nh.spinOnce();
	}
	if(started==false) {
		started=true;
		start.data=true;
	  	tirettePub.publish(&start);
	  	nh.spinOnce();
	}
	tirette=false;

	sens = (asinf((goal_y-current_y)/sqrtf((current_x-goal_x)*(current_x-goal_x)+(current_y-goal_y)*(current_y-goal_y)))>0)? 1 : -1;
	target_angle = sens * rad_to_deg(acosf((goal_x-current_x)/sqrtf((current_x-goal_x)*(current_x-goal_x)+(current_y-goal_y)*(current_y-goal_y))));
	orientate2(target_angle,speed);
	rotateAck();
	goal_distance = sqrtf((current_x-goal_x)*(current_x-goal_x)+(current_y-goal_y)*(current_y-goal_y));
	move_distance(goal_distance,speed);
	x_obst_abs=current_x+x_obst*cosf(current_phi_rad);
	y_obst_abs=current_y+y_obst*sinf(current_phi_rad);
	HAL_Delay(500);
	if(!evitementFlag && !outsideofmap(x_obst_abs,y_obst_abs)){
		float phi_init=current_phi_deg;
		float obst_dist=sqrtf(x_obst*x_obst+y_obst*y_obst);
		float node_cosine = x_obst/obst_dist;
		int sens_rot=(x_obst>0)?1:-1;;
		float x_node=current_x+2*obst_dist* cosf(deg_to_rad(phi_init));
		float y_node=current_y+2*obst_dist* sinf(deg_to_rad(phi_init));
		Calcul_Curv(x_node,y_node,phi_init-sens_rot*(90*(1-fabs(node_cosine))));
		int sign = (tetaC > 0) ? 1 : -1;
		double x_check = Xc + Rayon * cos(current_phi_rad -sign*M_PI/2);
		double y_check = Yc + Rayon * sin(current_phi_rad - sign * M_PI / 2);
		if(!outsideofmap(x_obst,y_obst)&& !outsideofmap(x_check,y_check)){
			orientate2(phi_prim,speed);
			curv(Rayon,tetaC,speed);
			HAL_Delay(100);
			Robot_Locateobst(goal_x, goal_y, speed);
		}
		else if(!outsideofmap(x_obst,y_obst)&& outsideofmap(x_check,y_check)){
			while(!evitementFlag){nh.spinOnce();}
			Robot_Locateobst(goal_x, goal_y, speed);
		}///comportement mahouch aajebni lezem tetbadel aka strategie dynamique
	}
}
