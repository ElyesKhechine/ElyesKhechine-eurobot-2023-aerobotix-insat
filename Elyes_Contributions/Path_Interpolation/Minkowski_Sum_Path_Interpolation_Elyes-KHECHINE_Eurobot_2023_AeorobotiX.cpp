#include <vector>
#include <cmath>

struct Point {
  float x, y;
};

struct Disk {
  Point center;
  float radius;
};

std::vector<Disk> minkowski_sum(std::vector<Disk> obstacles, float robot_diameter) {
  for (Disk& obstacle : obstacles) {
    obstacle.radius += robot_diameter / 2;
  }
  return obstacles;
}

float compute_ci(Point pi_minus_1, Point pi, Point pi_plus_1, float robot_diameter) {
  Disk disk_pi;
  disk_pi.center = { (pi_minus_1.x + pi_plus_1.x) / 2, (pi_minus_1.y + pi_plus_1.y) / 2 };
  disk_pi.radius = std::sqrt((pi_minus_1.x - pi_plus_1.x) * (pi_minus_1.x - pi_plus_1.x) + (pi_minus_1.y - pi_plus_1.y) * (pi_minus_1.y - pi_plus_1.y)) / 2 + robot_diameter / 2;

  float ci = std::sqrt(disk_pi.radius * disk_pi.radius - (pi.x - disk_pi.center.x) * (pi.x - disk_pi.center.x) - (pi.y - disk_pi.center.y) * (pi.y - disk_pi.center.y));
  float length_pi_minus_1 = std::sqrt((pi.x - pi_minus_1.x) * (pi.x - pi_minus_1.x) + (pi.y - pi_minus_1.y) * (pi.y - pi_minus_1.y));
  float length_pi_plus_1 = std::sqrt((pi.x - pi_plus_1.x) * (pi.x - pi_plus_1.x) + (pi.y - pi_plus_1.y) * (pi.y - pi_plus_1.y));
  return std::min(ci, std::min(length_pi_minus_1, length_pi_plus_1));
}
////////////TODO: MERGE THE FOLLOWING FUNCTIONS
std::vector<float> calculate_ci(std::vector<Point> path, std::vector<Disk> minkowski_sum_obstacles, float robot_diameter) {
  std::vector<float> ci_values(path.size());
  for (int i = 0; i < path.size(); ++i) {
    Point pi_minus_1, pi, pi_plus_1;
    if (i == 0) {
      pi_minus_1 = path[path.size() - 1];
      pi = path[0];
      pi_plus_1 = path[1];
    } else if (i == path.size() - 1) {
      pi_minus_1 = path[path.size() - 2];
      pi = path[path.size() - 1];
      pi_plus_1 = path[0];
    } else {
      pi_minus_1 = path[i - 1];
      pi = path[i];
      
      
      
float calculate_ci(std::vector<Disk> minkowski_sum_obstacles, int i, float d) {
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
    ci = std::min(ci, radius);
  }
  return ci;
}
     

