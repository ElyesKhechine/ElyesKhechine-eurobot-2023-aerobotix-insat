using namespace std;
#include <vector>
#include <bits/stdc++.h>
#include <vector>
#include <utility>
#include <cmath>

#define _USE_MATH_DEFINES
#include <cmath>

struct Point{
	float x,y;
};

typedef struct Point Point;
typedef vector<Point> vPoint;

struct Vector {
  Point start, end;
  Vector() {}
  Vector(Point start, Point end) : start(start), end(end) {}

  Point operator+(const Point &p) const {
    return Point(start.x + p.x, start.y + p.y);
  }

  Point operator-(const Point &p) const {
    return Point(start.x - p.x, start.y - p.y);
  }

  double length() const {
    double dx = end.x - start.x;
    double dy = end.y - start.y;
    return sqrt(dx * dx + dy * dy);
  }
};

typedef struct <Vector> Vector;

struct Disk {
  Point center;
  float radius;
};
typedef struct Disk Disk;
typedef vector<Disk> vDisk;

typedef vector<float> vfloat;

Disk compute_disk(Point pi_minus_1, Point pi, Point pi_plus_1, float robot_diameter) {
  Disk disk_pi;
  disk_pi.center = { (pi_minus_1.x + pi_plus_1.x) / 2, (pi_minus_1.y + pi_plus_1.y) / 2 };
  disk_pi.radius= sqrt((pi_minus_1.x - pi_plus_1.x) * (pi_minus_1.x - pi_plus_1.x) + (pi_minus_1.y - pi_plus_1.y) * (pi_minus_1.y - pi_plus_1.y)) / 2 + robot_diameter / 2;
	return disk;
}

float compute_ci(Point pi_minus_1, Point pi, Point pi_plus_1, Disk disk_pi) {
  float ci = sqrt(disk_pi.radius * disk_pi.radius - (pi.x - disk_pi.center.x) * (pi.x - disk_pi.center.x) - (pi.y - disk_pi.center.y) * (pi.y - disk_pi.center.y));
  float length_pi_minus_1 = sqrt((pi.x - pi_minus_1.x) * (pi.x - pi_minus_1.x) + (pi.y - pi_minus_1.y) * (pi.y - pi_minus_1.y));
  float length_pi_plus_1 = sqrt((pi.x - pi_plus_1.x) * (pi.x - pi_plus_1.x) + (pi.y - pi_plus_1.y) * (pi.y - pi_plus_1.y));
  return min(ci, min(length_pi_minus_1, length_pi_plus_1));
}

vDisk minkowski_sum(vDisk obstacles, float robot_diameter) {
  for (Disk& obstacle : obstacles) {
    obstacle.radius += robot_diameter / 2;
  }
  return obstacles;
}

float calculate_ci(vDisk obstacles, int i, float d) {
  vDisk minkowski_sum_obstacles= minkowski_sum(obstacles,d);
  Point p1 = minkowski_sum_obstacles[i-1].center;
  Point p2 = minkowski_sum_obstacles[i].center;
  Point p3 = minkowski_sum_obstacles[i+1].center;
  Vector v1 = p2 - p1;
  Vector v2 = p3 - p2;
  float ci = std::min(v1.length(), v2.length());
  Vector bisector = v1.normalize() + v2.normalize();
  float dist = bisector.length();
  if (dist > 0) {
    float radius = d/2 + bisector.dot(p2 - p1)/dist;
    ci = min(ci, radius);
  }
  return ci;
}

const float PI = 3.14159265358979323846;
const float MAX_BETA = PI / 2;

float compute_beta(Point pi_minus_1, Point pi, Point pi_plus_1) {
   /*
   //METHOD 2
   //construct 2 vectors: p[i-1]p[i] and p[i]p[i+1]
	float dot;
	Vector v1, v2;
	for(int i=1;i<=n-2;i++){
		v1 = Vector(p[i-1],p[i]);
		v2 = Vector(p[i],p[i+1]);
		//dot product of the 2 vectors
		dot = v1[x]*v2[x]+v1[y]*v2[y];
		B[i]=(float) acosf(dot/(v1.length*v2.length));
		if (fabs(B[i])>M_PI_2) B[i]=M_PI_2;
	}*/                	
                   	      	
  float beta = atan2(pi_plus_1.y - pi.y, pi_plus_1.x - pi.x) - atan2(pi_y - pi_minus_1.y, pi.x - pi_minus_1.x);
  if (beta > MAX_BETA) {
    beta = MAX_BETA;
  } else if (beta < -MAX_BETA) {
    beta = -MAX_BETA;
  }
  return beta;
}

vfloat compute_l_bounded_curv(int n, vfloat B(n-1), vPoint p(n)){
	Vector v1, v2;
	vfloat l(n-1), tau(n-1), r(n-1);
	//determine tau[i] and l[i] for each i
	for (int i=1; i<=n-2;i++)
		tau[i]=fabs(tan(B[i]/2));
	for (int i=1; i<=n-2;i++){
		v1 = Vector(p[i-1],p[i]);
		v2 = Vector(p[i],p[i+1]);
		l[i]=min(c[i], (float) (tau[i]*v2.length)/(tau[i]+tau[i+1]), (float) (tau[i]*v1.length)/(tau[i-1]+tau[i]));
		r[i]=(float) l[i] / tau[i]; 
	}
	return l;
}

/*void newton_raphson_search( float c1, float c2, vfloat B, int i , float kc){
	float a = (c1-c2)/(c1+c2);
	float cm = (c1+c2)/2;
	float sf;
	while (c1 > 0 && c2 > 0 && sf >= (B[i]/kc) && sf<= ((2/kc)*tan(B[i]/2))) {
		a = (c1-c2)/(c1+c2);
		sf= B[i]/kc;
		
		
	}
	
}*/

void interpolation (int n, vPoint p(n), float robot_diameter, vDisk obs){
	/*
	Di: a disk defined by its center and radius that is tangent to both segments
(which implies that its center pci belongs to the inner bisector of the angle formed by these
segments), and that fully covers the obstacles cleared by the pair of segments.
	d: diameter (parameter) of robot + information on obstacles: can be used to form the disc Di tangent to both segments of study
	p: table of points pi(xi,yi) in a trajectory, i in [0,n-1]
	ci: clearance parameter of the intermediate point pi, i in [1,n-2]
	pt: a point of tangency between the disc Di and the segments
	Bi: beta i the angle between the 2 segments of trajectory
	*/
	
	//definition of the disk Di
	vDisk D(n-1);
	for(int i=1; i<=n-2; i++){
		D[i]= compute_disk(p[i-1],p[i],p[i+1],robot_diameter);
	}
	
	// compute_ci: Calculates the clearance parameter ci for all positions along a path and store them in an array
	// calculate_ci : calculate the ci value given a robot's position
	vfloat c(n-1);
	for(int i=1; i<=n-2; i++){
		c[i]=min(compute_ci(p[i-1],p[i],p[i+1],D[i]), calculate_ci(obs,i,robot_diameter));
	}
	
	//definitiom of Bi
	vfloat B(n-1);
	for(int i=1; i<=n-2; i++){
		B[i]=compute_beta(p[i-1],p[i],p[i+1]);
	}
	
	/////////////////SOLUTION//////////////////////////////
	
	///////////STEP 1: Producing a path in which the absolute curvature is bounded at all points	
	vfloat l(n-1)= compute_l_bounded_curv(n,B,p);
	
	
	///////////STEP 2: Modifying this path in order to now bound the rate of variation of curvature
	/*
	kc: curvature of the circle arc avoid the obstacles
	ti1 and ti2: pts of tangency between this circle arc and the line segments p[i-1]p[i] and p[i]p[i+1]
	theta1 and theta2 the tangential angles at these two points.
	k1 and k2: pair of curvatures such that 0<=k1<kc and 0<=k2<kc
	sm: distance travelled along the first arc
	sf: total distance travelled over both arcs
	c1: linear variation of the first arc
	-c2: linear variation of the second arc
	TASK: COMPUTE SM AND SF GIVEN K1; K2; KC, AND B[i]
	*/
	float kc, theta1, theta2, k1,k2, km, sm, sf,c1,c2;
	vpoint t1, t2;
	
	for (int i=1; i<n-2 ; i++){
		if (Kc>0){
			if (B[i]> 0 && B[i] <= M_PI_2){
				theta2[i]=theta1[i]+B[i];
				km=k1+c1*sm;
				newton_raphson_search();
			}
			
		}
	}
	
	
	
}
