#include <cmath>

const float PI = 3.14159265358979323846;
const float MAX_BETA = PI / 2;

float compute_beta(const float x1, const float y1, const float x2, const float y2,
                   const float x3, const float y3) {
  float beta = atan2(y3 - y2, x3 - x2) - atan2(y2 - y1, x2 - x1);
  if (beta > MAX_BETA) {
    beta = MAX_BETA;
  } else if (beta < -MAX_BETA) {
    beta = -MAX_BETA;
  }
  return beta;
}
