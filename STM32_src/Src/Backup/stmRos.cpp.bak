#include <Robot_Navi_Euro20.h>
#include <stmRos.h>
#include <ros.h>
#include <geometry_msgs/Twist.h>
#include <std_srvs/SetBool.h>
#include <std_msgs/Bool.h>
#include <std_msgs/String.h>
#include <string>
#include <std_msgs/Float32.h>
#include <reel_euro2021/ta7wil.h>
#include <iostream>
#include <gazebo_msgs/ApplyJointEffort.h>
#include <gazebo_msgs/GetLightProperties.h>
#include <reel_euro2021/c.h>
#include <reel_euro2021/jdid.h>
#include <reel_euro2021/jd.h>
#include <reel_euro2021/pose.h>
using namespace std;





ros::NodeHandle nh;
std_msgs::Float32 l1_p;
reel_euro2021::c c;
std_msgs::Bool sending;
reel_euro2021::jdid jdid;
extern ros::Publisher tirettePub;
extern bool evitementFlag;
extern float l1,l2,l3,r1,r2,r3 ;
extern float x_obst,y_obst;
extern volatile float current_x,current_y,current_phi_deg,current_phi_rad;
extern volatile double right_speed, left_speed;
extern volatile float spacing_encoder;
extern volatile double right_encoder_speed,left_encoder_speed;
extern volatile float left_radius,right_radius,spacing_encoder,spacing_wheel;
float test;
double v_rx ;
double omega_r ;
extern volatile float phi_init_deg;

float Vx;
float Vz;
extern bool tirette;


void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
  nh.getHardware()->flush();
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
  nh.getHardware()->reset_rbuf();
}
void messageCb(const geometry_msgs::Twist& vel){
	Vx=vel.linear.x;
	Vz=vel.angular.z;
}

void poseCallback(const reel_euro2021::poseRequest & req,reel_euro2021::poseResponse& res){
	res.x=current_x;
	res.y=current_y;
	res.phi=current_phi_rad;
	res.result="Done rendering Pose";
	res.success=true;

}


void commandCallback(const reel_euro2021::jdRequest & req,reel_euro2021::jdResponse& res){

	if(!strcmp(req.function,"WheelSpacing")){
		spacing_wheel=req.vitesse;
		res.result ="Done Setting Wheel Spacing";

	}
	else if(!strcmp("EncoderSpacing",req.function)){
		spacing_encoder=req.x;
		res.success=true;
		res.result ="Done Setting Encoder Spacing";

	}
	else if(!strcmp("RightRadius",req.function)){
		right_radius=req.x;
		res.success=true;
		res.result ="Done Setting Right Radius";
	}
	else if(!strcmp("LeftRadius",req.function)){
		left_radius=req.x;
		res.success=true;
		res.result ="Done Setting Left Radius";
	}else if(!strcmp("xy",req.function)){
		if(req.x!=0) {current_x=req.x;}
		if(req.y!=0) {current_y=req.y;}
		current_phi_rad=req.phi;
		current_phi_deg= rad_to_deg(current_phi_rad);
		res.success=true;
		res.result ="Done Setting Coordinates";
	}else if(!strcmp("Go",req.function)){
		if(req.vitesse==0){
			Robot_Locate(0, 0, 0);//   Robot_Locate(req.duration.sec,req.duration.nsec,400);
		}else{
			float x=req.x;
			float y=req.y;
			Robot_Locate(x,y,req.vitesse);
		}
		res.success=true;
		res.result ="Done Reaching Goallll";
	}else if(!strcmp("Gobst",req.function)){
		if(req.vitesse!=0){
			float x=req.x;
			float y=req.y;
			Robot_Locateobst(x,y,req.vitesse);
		}
		res.success=true;
		res.result ="Done Reaching Goallll";
	}
	else if(!strcmp("Rotate",req.function)){

			rotate(req.phi, req.vitesse);
			res.success=true;
			res.result ="Done Rotatinggggg";
		}
	else if(!strcmp("Orientate",req.function)){
					float x=req.phi;
					orientate(x, req.vitesse);
					res.success=true;
					res.result ="Done Rotating";
				}
	else if(!strcmp("Move",req.function)){
						float x=req.x;
						move_distance(x, req.vitesse);
						res.success=true;
						res.result ="Done Moving";
					}

	else if(!strcmp("Asta3",req.function)){
					float x=req.x;
				asta3(x, req.vitesse);
				res.success=true;
				res.result ="Done Asta3";
			}
	else if(!strcmp("Asta3L",req.function)){
						float x=req.x;
					asta3L(x, req.vitesse);
					res.success=true;
					res.result ="Done Asta3 Left";
				}

	else if(!strcmp("Motors",req.function)){

						move_distance(200, 200);
						move_distance(-200, 200);
						orientate(90, 300);
						orientate(0, 300);
						res.success=true;
						res.result ="Done Testing Motors";
					}
	else if (!strcmp("GoCurv",req.function)){
		if(req.vitesse==0){
			Robot_locateCurv(0, 0, 0,0);//   Robot_Locate(req.duration.sec,req.duration.nsec,400);
				}else{
					float x=req.x;
					float y=req.y;
					float phi=req.phi;

					Robot_locateCurv(x,y,phi,req.vitesse);
				}
				res.success=true;
				res.result ="Done Reaching Goallll ye jabri";

	}
	else if (!strcmp("GoMultiCurv",req.function)){


						int n =req.nombre;
						reel_euro2021::c* m;
						m=req.curv_path;
						float** mat=(float**)malloc(n*sizeof(float*));
						for (int index=0;index<n;++index)
						{
						    mat[index] = (float*)malloc(3 * sizeof(float));
						}

						for (int i=0;i<n;i++){
							mat[i][0]=m[i].x;
							mat[i][1]=m[i].y;
							mat[i][2]=m[i].phi;
						}
						Robot_Locate_Multi_Curv(mat,n,req.vitesse);

					res.success=true;
					res.result ="Done Reaching Goallll ye jabri";

		}
	else{
		res.success=false;
				res.result ="FAILED !!!!!!!!!";
	}


}

void zonesCallback(const std_msgs::String &req){
	const char* rd = req.data;
    char* end;
    l3 = std::strtod(rd, &end);
    l2 = std::strtod(end, &end);
    l1 = std::strtod(end, &end);
    r1 = std::strtod(end, &end);
    r2 = std::strtod(end, &end);
    r3 = std::strtod(end, &end);
}

void obstacleCallback(const std_msgs::String &req){
	const char* rd = req.data;
    char* end;
    y_obst = std::strtod(rd, &end);
    x_obst = std::strtod(end, &end);
}



void evitementCallback(const std_msgs::Bool &req){
	evitementFlag=req.data;
//	res.message="Done";
//	res.success=true;
	}
ros::Subscriber<geometry_msgs::Twist> sub("cmd_vel", &messageCb );
//ros::Publisher ta7wil_pub("ta7wil", &ta7wil);
ros::Publisher sending_pub("sending", &sending);
ros::Publisher c_pub("c",&c);
ros::ServiceServer<reel_euro2021::poseRequest,reel_euro2021::poseResponse> PoseServ("/StmPose", &poseCallback);
ros::ServiceServer<reel_euro2021::jdRequest,reel_euro2021::jdResponse> subCommandStm("/StmCommand", &commandCallback );
//ros::ServiceServer<std_srvs::SetBoolRequest,std_srvs::SetBoolResponse> evitementService("/evitementSrv", &evitementCallback);
ros::Subscriber<std_msgs::Bool> evitementSubscriber("/evitement", &evitementCallback);
ros::Subscriber<std_msgs::String> obstacleSubscriber("/obstacle", &obstacleCallback);

ros::Subscriber<std_msgs::String> zonesSubscriber("/zones", &zonesCallback);

ros::Publisher l1_pub("l1",&l1_p);

void rotateAck(){
	sending.data=true;
	sending_pub.publish(&sending);
}

void setup(void)
{
  nh.initNode();
  nh.subscribe(sub);
  nh.advertise(c_pub);
  //nh.advertise(ta7wil_pub);
  nh.advertise(sending_pub);
  nh.advertiseService(subCommandStm);
  nh.advertiseService(PoseServ);
  nh.advertise(tirettePub);
  nh.subscribe(evitementSubscriber);
  nh.subscribe(zonesSubscriber);
  nh.subscribe(obstacleSubscriber);
  nh.advertise(l1_pub);
}

void loop(void)
{

  // tf odom->base_link
	c.x=current_x;
	c.y=current_y;
	c.phi=current_phi_deg;
	//l1_p.data=l1;
	// en mm/s
//	 v_rx = ( 	right_encoder_speed + left_encoder_speed) /2;
//	 v_rx = ( 	right_encoder_speed + left_encoder_speed) /2000;
//
//
//	 omega_r = ( right_encoder_speed - left_encoder_speed ) / spacing_encoder;
//	ta7wil.vx=v_rx;
//	ta7wil.vy=omega_r;
// 	  transformStamped.header.frame_id =odom ;
// 	  transformStamped.child_frame_id = base_link ;
//
// 	  transformStamped.transform.translation.x = (double)current_x/1000;
// 	  transformStamped.transform.translation.y = (double)current_y/1000;
//
// 	  transformStamped.transform.rotation = tf::createQuaternionFromYaw(current_phi_rad);
// 	  transformStamped.header.stamp = nh.now();
//
// 	  broadcaster.sendTransform(transformStamped);



 	  //Send ODOM

//
// 	  msg.header.stamp = nh.now();
// 	  msg.header.frame_id = "odom";
//
//
//// 	 v_rx = ( 	right_encoder_speed + left_encoder_speed) /2;
//// 	 omega_r = ( right_encoder_speed - left_encoder_speed ) / spacing_encoder;
//
//
//
// 	  //set the position
// 	  msg.pose.pose.position.x = (double)current_x/1000;
// 	  msg.pose.pose.position.y = (double)current_y/1000;
// 	  msg.pose.pose.position.z = 0.0;
// 	  msg.pose.pose.orientation = tf::createQuaternionFromYaw(current_phi_rad);
//
//
//
//
// 	  //set the velocity
// 	  msg.child_frame_id = "base_link";
// 	  msg.twist.twist.linear.x = v_rx;
// 	  msg.twist.twist.linear.y = 0;
// 	  msg.twist.twist.angular.z = omega_r;
//
// 	   //publish the message
// 	  odom_pub.publish(&msg);
 	c_pub.publish(&c);
 	std_msgs::Bool tirette_msg;
 	tirette_msg.data = tirette;
 	tirettePub.publish(&tirette_msg);
 //	l1_pub.publish(&l1_p);
	//sending_pub.publish(&sending);


 	  nh.spinOnce();
 	  HAL_Delay(100);//badel delay

}

					/////////////////////////////////////////////////////////////////////////////////////////
				   ////////////////////////// 		FOR THE NEXT GENERATION 	  //////////////////////////
				  /////////////////////////              GOOD LUCK	 :D         ///////////////////////////
			     ////////////////////////// 		   MEHDIXLABETIX 2023		   ///////////////////////
			    /////////////////////////////////////////////////////////////////////////////////////////


