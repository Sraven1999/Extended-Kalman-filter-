#include "kalman_filter.h"

using Eigen::MatrixXd;
using Eigen::VectorXd;

/* 
 * Please note that the Eigen library does not initialize 
 *   VectorXd or MatrixXd objects with zeros upon creation.
 */

KalmanFilter::KalmanFilter() {}

KalmanFilter::~KalmanFilter() {}

void KalmanFilter::Init(VectorXd &x_in, MatrixXd &P_in, MatrixXd &F_in,
                        MatrixXd &H_in, MatrixXd &R_in, MatrixXd &Q_in) {
  x_ = x_in;
  P_ = P_in;
  F_ = F_in;
  H_ = H_in;
  R_ = R_in;
  Q_ = Q_in;
}

void KalmanFilter::Predict() {
  /**
   * TODO: predict the state
   */
  x_ = F_*x_ ;
  P_ = F_*P_*F_.transpose() + Q_;
}

void KalmanFilter::Update(const VectorXd &z) {
  /**
   * TODO: update the state by using Kalman Filter equations
   */
  VectorXd Y_ = z - H_*x_;
  MatrixXd S_ = H_*P_*(H_.transpose()) + R_;
  MatrixXd K_ = P_*(H_.transpose())*(S_.inverse());
  
  //new estimate
  x_ = x_ + (K_ * Y_);
  long x_size = x_.size();
  MatrixXd I = MatrixXd::Identity(x_size, x_size);
  P_ = (I - K_ * H_) * P_;
}

void KalmanFilter::UpdateEKF(const VectorXd &z) {
  /**
   * TODO: update the state by using Extended Kalman Filter equations
   */
  VectorXd h_x(3);
  float px = x_(0);
  float py = x_(1);
  float vx = x_(2);
  float vy = x_(3);
  float rho_pos_sq = (px*px) + (py*py);
  float rho_pos = sqrt(rho_pos_sq);
  float theta = atan2(py, px);
  float top = px*vx + py*vy;
  float rho_dotpos = top/rho_pos;
  h_x << rho_pos, theta, rho_dotpos;
  VectorXd Y_ = z - h_x;
  if(Y_[1] < -M_PI){
      Y_[1] += ((double)2 * M_PI);
    }
  if(Y_[1] > M_PI){
      Y_[1] -= ((double)2 * M_PI);
    }
  MatrixXd S_ = H_*P_*(H_.transpose()) + R_;
  MatrixXd K_ = P_*(H_.transpose())*(S_.inverse());
  
  //new estimate
  x_ = x_ + (K_ * Y_);
  long x_size = x_.size();
  MatrixXd I = MatrixXd::Identity(x_size, x_size);
  P_ = (I - K_ * H_) * P_;
}
