
void initMultiCurv (void){
	// intiailisation de la fonction multi_curv (pour accumuler l'Integrale)
	total_right=0;
	total_left=0;
	total_centre=0;
	PWM_L_sign_counter=0;
	PWM_R_sign_counter=0;
	right_error=0;
	left_error=0;
}

void Multi_Curv(float R,float theta ,float speed,int i,int n){
 	//curv function used in the robot_locate_multi_curv function
	initMultiCurv(); // integrale accumulée
	theta=(theta*PI)/180;
	goalC=fabs(R*theta);
	remain_distC=goalC-fabs(total_centre);
	speedC=speed;
	if (goalC < (speedC*speedC/rampC)) {
		accel_dist = fabs(remain_distC)/2;
		decel_dist = fabs(remain_distC)/2;
		speedC = speedC* accel_dist / ((float)0.5*speedC*speedC/rampC);
	}
	else{
		accel_dist = (float)0.5*speedC*speedC/rampC;
		decel_dist = (float)0.5*speedC*speedC/((rampC));
	}

		if (i!=0) accel_dist=0; //Accélération du robot juste au début de la trajectoire
		if (i!=(n-1)) decel_dist=0;// Décèlération du robot seulement a la fin de la trajectoire

	while(fabs(total_centre)<fabs(goalC)) {  //(((remain_distC)>=2))||((remain_distC)<=-2)
		t0=t;
	  remain_distC=goalC-fabs(total_centre);
	  if ((fabs(total_centre)) < accel_dist){
			speed_refC = sens_de_mouvement*50+sens_de_mouvement*(speedC-50)*fabs((total_centre)/(accel_dist));
		}
		else if (fabs((remain_distC)) < decel_dist)
			speed_refC = sens_de_mouvement*10+sens_de_mouvement*(speedC-10)*fabs((remain_distC)/(decel_dist));
		else
			speed_refC = sens_de_mouvement*speedC;
		Calcul_speed_Refs(theta,R,sens_de_mouvement,speed_refC);
		//Empietement {just ask us} Pour plus d'informations, contactez Nesrine ou Wassim
		if(i!=(n-1)){
			if ((remain_distC<=Distance_empietement)) {
				if (flag1==0) {
					prev_speed_refL=speed_refL;
					prev_speed_refR=speed_refR;

					Calcul_Curv(matrix[i+1][0],matrix[i+1][1],matrix[i+1][2]);
					speedC=speed;
					tetaC=(tetaC*PI)/180;
					if ((fabs(Rayon*tetaC)-Distance_empietement)< (speedC*speedC/rampC)) {
						speedC = speedC* (fabs((Rayon*tetaC)/2)-(0.5f*Distance_empietement)) / ((float)0.5f*speedC*speedC/rampC);
					}
					Calcul_speed_Refs(tetaC,Rayon,sens_de_mouvement,sens_de_mouvement*speedC);
					new_speed_refL=speed_refL;
					new_speed_refR=speed_refR;
					flag1=1;
				}
			speed_refR=(((1/Distance_empietement)*(((-0.5f)*(prev_speed_refR+new_speed_refR))+prev_speed_refR))*(remain_distC))+(0.5f*(prev_speed_refR+new_speed_refR));
			speed_refL=(((1/Distance_empietement)*(((-0.5f)*(prev_speed_refL+new_speed_refL))+prev_speed_refL))*(remain_distC))+(0.5f*(prev_speed_refL+new_speed_refL));
			}
		}
		if ((total_centre<=Distance_empietement)&&(i!=0)) {
			if (flag2==0) {
					Calcul_Curv(matrix[i][0],matrix[i][1],matrix[i][2]);
					Calcul_speed_Refs(tetaC,Rayon,sens_de_mouvement,speed_refC);
					new_speed_refL=speed_refL;
					new_speed_refR=speed_refR;
					flag2=1;
				}
			speed_refR=(((1/Distance_empietement)*(((-0.5f)*(prev_speed_refR+new_speed_refR))+new_speed_refR))*(total_centre))+(0.5f*(prev_speed_refR+new_speed_refR));
			speed_refL=(((1/Distance_empietement)*(((-0.5f)*(prev_speed_refL+new_speed_refL))+new_speed_refL))*(total_centre))+(0.5f*(prev_speed_refL+new_speed_refL));
		}
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
	flag1=0;
	flag2=0;
	prev_speed_refL=speed_refL;
	prev_speed_refR=speed_refR;
}

void Robot_Locate_Multi_Curv( float** matrix , int n , int speed) {
	// matrix colonne : x/y/phi_target
	// in the matrix put the x,y and phi_target you want the robot to go to in order
	//feha aghlat can be better
	 for(int i=0;i<n;i++) {
			Calcul_Curv(matrix[i][0],matrix[i][1],matrix[i][2]);
			if (i==0)
			{
				orientate(phi_prim,speed);
				init();
			}
				Multi_Curv(Rayon,tetaC,speed,i,n);
	 }
		 stop_motors();
}

void allocation (int taille ) {
	//allocation d une matrice [taille][3] : x/y/phi_target pour (taille) points
	matrix=(float**) malloc (taille*(sizeof(float*)));
	for(int i=0;i<taille;i++)
		matrix[i]=(float*) malloc(3*sizeof(float));
}

void movement_sequence(float speed){
	sens = (asinf((yN-current_y)/sqrtf((current_x-xN)*(current_x-xN)+(current_y-yN)*(current_y-yN)))>0)? 1 : -1;
	float node_target_angle = sens * rad_to_deg(acosf((xN-current_x)/sqrtf((current_x-xN)*(current_x-xN)+(current_y-yN)*(current_y-yN))));
	orientate2(node_target_angle,speed);
	float node_distance = sqrtf((current_x-xN)*(current_x-xN)+(current_y-yN)*(current_y-yN));
}

void move(float speed, int delay){
	// 3ADDEL BEHA
	i_right_error = 0;
	i_left_error = 0;
	t = 0;
	while (t<delay)
	{
		t0=t;
		speed_ref = ramp*t/1000;
		if (speed_ref>speed)
			speed_ref = speed;
		//Right wheel regulation
		right_error = speed_ref - right_speed;
		i_right_error += right_error;
		PWM_R = kp * right_error + ki * i_right_error;
		//Left wheel regulation
		left_error = speed_ref - left_speed;
		i_left_error += left_error;
		PWM_L = kp * left_error + ki * i_left_error;
		run_motors();
		do delta=t-t0;
		while (delta<T);
	}
	stop_motors();
}

float** create_points(float init_x,float init_y,float dist,float phi_init){
	int i;
	float** mat=(float**)malloc(5*sizeof(float*));
	for (i=0;i<5;i++)
		*(mat+i)=(float*)malloc(3*sizeof(float));
	mat[0][0]=init_x;
	mat[0][1]=init_y;
	mat[0][2]=phi_init;

	for(i=1;i<5;i++){
		if(i<=5/2){
			mat[i][0]=mat[i-1][0]+300;
			mat[i][1]=mat[i-1][1]+dist;
			mat[i][2]=phi_init/2;

		}else{
			mat[i][0]=mat[i-1][0]-300;
			mat[i][1]=mat[i-1][1]+dist;
			mat[i][2]=3*phi_init/4;

		}
	}
	return mat;

}

							///////////////////////////////////////////////////////////////////////////
								///////////////////////// FOR THE NEXT GENERATION /////////////////////////
								///////////////////////// 			RABI Y3INKOM 			/////////////////////////
								///////////////////////////////////////////////////////////////////////////

void trajectory_generator(void){
	target_y = 0;
	target_y_prime = 0;
	if (t<2000)
	{
		target_x = dec+(float)0.5*ramp*t*t/1000000;
		target_x_prime = ramp*t/1000;
	}
	else if (t<2500)
	{
		target_x = dec + 1000 + (t-2000);
		target_x_prime = 1000;

	}
	else if (t<4500)
	{
		target_x = dec + 1500 + (t-2500) - (float)0.5*ramp*(t-2500)*(t-2500)/1000000;
		target_x_prime = 1000 - ramp * (t-2500)/1000;
	}
	else
	{
		target_x = dec + 2500;
		target_x_prime = 0;
	}
}

void trajectory (void){
	init();
	float kVx = 0.5, kVy = 0.5, kPx = 0, kPy = 0;
	double pass_matrix[2][2], correction_matrix[2];
	while(1)
	{
		t0=t;
		trajectory_generator();
		//Calculate the correction matrix (x,y)
		correction_matrix[0] = kVx*target_x_prime + kPx*(target_x-ref_x);
		correction_matrix[1] = kVy*target_y_prime + kPy*(target_y-ref_y);
		//Calculate the passing matrix from coordinate system to speed system
		pass_matrix[0][0] = dec*cos(current_phi_rad) + spacing_wheel*sin(current_phi_rad)/2;
		pass_matrix[0][1] = dec*sin(current_phi_rad) - spacing_wheel*cos(current_phi_rad)/2;
		pass_matrix[1][0] = dec*cos(current_phi_rad) - spacing_wheel*sin(current_phi_rad)/2;
		pass_matrix[1][1] = dec*sin(current_phi_rad) + spacing_wheel*cos(current_phi_rad)/2;
		//Calculate the right and left speed
		left_target_speed = (pass_matrix[0][0]*correction_matrix[0] + pass_matrix[0][1]*correction_matrix[1])/dec;
		right_target_speed = (pass_matrix[1][0]*correction_matrix[0] + pass_matrix[1][1]*correction_matrix[1])/dec;
		//Speed regulation
		//Right wheel regulation
		right_error = right_target_speed - right_speed;
		i_right_error += right_error;
		PWM_R = kp * right_error + ki * i_right_error;
		//Left wheel regulation
		left_error = left_target_speed - left_speed;
		i_left_error += left_error;
		PWM_L = kp * left_error + ki * i_left_error;
		//Execute
		run_motors();
		do delta=t-t0;
		while (delta<T);
	}
}
