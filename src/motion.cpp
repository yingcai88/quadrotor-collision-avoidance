#include "motion.h"

void Motion::setAccelerationMax(double const& acceleration_max) {
  this->a_max_horizontal = acceleration_max;
};

void Motion::setAcceleration(Vector3 const& acceleration) {
  this->acceleration = acceleration;
  this->unscaled_acceleration = acceleration;
};

void Motion::ScaleAcceleration(double scale_factor) {
  this->acceleration = unscaled_acceleration * scale_factor;
};

void Motion::setInitialAcceleration(Vector3 const& initial_acceleration_to_set) {
  initial_acceleration = initial_acceleration_to_set;
  jerk = (acceleration - initial_acceleration) / jerk_time;
  position_end_of_jerk_time = 0.1666*jerk*jerk_time*jerk_time*jerk_time + 0.5*initial_acceleration*jerk_time*jerk_time + initial_velocity*jerk_time;
  velocity_end_of_jerk_time = 0.5*jerk*jerk_time*jerk_time + initial_acceleration*jerk_time + initial_velocity;
};

void Motion::setInitialVelocity(Vector3 const& initial_velocity_to_set) {
  initial_velocity = initial_velocity_to_set;
};


Vector3 Motion::getAcceleration() const{
  return this->acceleration;
}

Vector3 Motion::getInitialVelocity() const {
  return initial_velocity;
};

Vector3 Motion::getVelocity(Scalar const& t) const {
  if (t < jerk_time) {
    return 0.5*jerk*t*t + initial_acceleration*t + initial_velocity; 
  }
  else {
    double t_left = t - jerk_time;
    return velocity_end_of_jerk_time + acceleration*t_left;
  }
};

Vector3 Motion::getPosition(Scalar const& t) const {
  if (t < jerk_time) {
    return 0.1666*jerk*t*t*t + 0.5*initial_acceleration*t*t + initial_velocity*t;
  }
  else {
    double t_left = t - jerk_time;
    return position_end_of_jerk_time + 0.5*acceleration*t_left*t_left + velocity_end_of_jerk_time*t_left;
  }
};

Vector3 Motion::getTerminalStopPosition(Scalar const& t) const {
  Vector3 position_end_of_motion = getPosition(t);
  Vector3 velocity_end_of_motion = getVelocity(t);

  double speed = velocity_end_of_motion.norm();
  
  Vector3 stopping_vector = -velocity_end_of_motion/speed;
  Vector3 max_stop_acceleration = a_max_horizontal*stopping_vector;
  Vector3 stopping_jerk = (max_stop_acceleration - acceleration) / jerk_time;
  Vector3 position_end_of_jerk_stop = 0.1666*stopping_jerk*jerk_time*jerk_time*jerk_time + 0.5*acceleration*jerk_time*jerk_time + velocity_end_of_motion*jerk_time + position_end_of_motion;
  Vector3 velocity_end_of_jerk_stop = 0.5*stopping_jerk*jerk_time*jerk_time + acceleration*jerk_time + velocity_end_of_motion;

  // check if stopped during jerk time
  if (velocity_end_of_motion.dot(velocity_end_of_jerk_stop) < 0) {
    return position_end_of_jerk_stop;
  }

  double realistic_stop_accel = a_max_horizontal*stopping_factor;
  double speed_after_jerk = velocity_end_of_jerk_stop.norm();
  double stop_t_after_jerk = (speed_after_jerk / realistic_stop_accel);
  //double extra_drift = speed_after_jerk*0.200;
  double stopping_distance_after_jerk =  0.5 * -realistic_stop_accel * stop_t_after_jerk*stop_t_after_jerk + speed_after_jerk*stop_t_after_jerk;

  return position_end_of_jerk_stop + stopping_distance_after_jerk*-stopping_vector;

}


void Motion::setAccelerationLASER(Vector3 const& acceleration_laser) {
  this->acceleration_laser = acceleration_laser;
};

void Motion::setInitialAccelerationLASER(Vector3 const& initial_acceleration_laser) {
  this->initial_acceleration_laser = initial_acceleration_laser;
  jerk_laser = (acceleration_laser - initial_acceleration_laser) / jerk_time;
  position_end_of_jerk_time_laser = 0.1666*jerk_laser*jerk_time*jerk_time*jerk_time + 0.5*initial_acceleration_laser*jerk_time*jerk_time + initial_velocity_laser*jerk_time;
  velocity_end_of_jerk_time_laser = 0.5*jerk_laser*jerk_time*jerk_time + initial_acceleration_laser*jerk_time + initial_velocity_laser;
};

void Motion::setInitialVelocityLASER(Vector3 const& initial_velocity_laser) {
  this->initial_velocity_laser = initial_velocity_laser;
};

Vector3 Motion::getAccelerationLASER() const{
  return acceleration_laser;
};

Vector3 Motion::getInitialVelocityLASER() const {
  return initial_velocity_laser;
};

Vector3 Motion::getVelocityLASER(Scalar const& t) const {
  if (t < jerk_time) {
    return 0.5*jerk_laser*t*t + initial_acceleration_laser*t + initial_velocity_laser; 
  }
  else {
    double t_left = t - jerk_time;
    return velocity_end_of_jerk_time_laser + acceleration_laser*t_left;
  }
};

Vector3 Motion::getPositionLASER(Scalar const& t) const {
  if (t < jerk_time) {
    return 0.1666*jerk_laser*t*t*t + 0.5*initial_acceleration_laser*t*t + initial_velocity_laser*t;
  }
  else {
    double t_left = t - jerk_time;
    return position_end_of_jerk_time_laser + 0.5*acceleration_laser*t_left*t_left + velocity_end_of_jerk_time_laser*t_left;
  }
};

Vector3 Motion::getTerminalStopPositionLASER(Scalar const& t) const {
  Vector3 position_end_of_motion = getPositionLASER(t);
  Vector3 velocity_end_of_motion = getVelocityLASER(t);

  double speed = velocity_end_of_motion.norm();
  
  Vector3 stopping_vector = -velocity_end_of_motion/speed;
  Vector3 max_stop_acceleration = a_max_horizontal*stopping_vector;
  Vector3 stopping_jerk = (max_stop_acceleration - acceleration) / jerk_time;
  Vector3 position_end_of_jerk_stop = 0.1666*stopping_jerk*jerk_time*jerk_time*jerk_time + 0.5*acceleration*jerk_time*jerk_time + velocity_end_of_motion*jerk_time + position_end_of_motion;
  Vector3 velocity_end_of_jerk_stop = 0.5*stopping_jerk*jerk_time*jerk_time + acceleration*jerk_time + velocity_end_of_motion;

  // check if stopped during jerk time
  if (velocity_end_of_motion.dot(velocity_end_of_jerk_stop) < 0) {
    return position_end_of_jerk_stop;
  }

  double realistic_stop_accel = a_max_horizontal*stopping_factor;
  double speed_after_jerk = velocity_end_of_jerk_stop.norm();
  double stop_t_after_jerk = (speed_after_jerk / realistic_stop_accel);
  //double extra_drift = speed_after_jerk*0.200;
  double stopping_distance_after_jerk =  0.5 * -realistic_stop_accel * stop_t_after_jerk*stop_t_after_jerk + speed_after_jerk*stop_t_after_jerk;

  return position_end_of_jerk_stop + stopping_distance_after_jerk*-stopping_vector;

}

Vector3 Motion::getInitialAccelerationRDF() const {
  return this->initial_acceleration_rdf;
}

void Motion::setAccelerationRDF(Vector3 const& acceleration_rdf) {
  this->acceleration_rdf = acceleration_rdf;
};

void Motion::setInitialAccelerationRDF(Vector3 const& initial_acceleration_rdf) {
  this->initial_acceleration_rdf = initial_acceleration_rdf;
  jerk_rdf = (acceleration_rdf - initial_acceleration_rdf) / jerk_time;
  position_end_of_jerk_time_rdf = 0.1666*jerk_rdf*jerk_time*jerk_time*jerk_time + 0.5*initial_acceleration_rdf*jerk_time*jerk_time + initial_velocity_rdf*jerk_time;
  velocity_end_of_jerk_time_rdf = 0.5*jerk_rdf*jerk_time*jerk_time + initial_acceleration_rdf*jerk_time + initial_velocity_rdf;
};

void Motion::setInitialVelocityRDF(Vector3 const& initial_velocity_rdf) {
  this->initial_velocity_rdf = initial_velocity_rdf;
};

Vector3 Motion::getAccelerationRDF() const{
  return acceleration_rdf;
};

Vector3 Motion::getInitialVelocityRDF() const {
  return initial_velocity_rdf;
};

Vector3 Motion::getVelocityRDF(Scalar const& t) const {
  if (t < jerk_time) {
    return 0.5*jerk_rdf*t*t + initial_acceleration_rdf*t + initial_velocity_rdf; 
  }
  else {
    double t_left = t - jerk_time;
    return velocity_end_of_jerk_time_rdf + acceleration_rdf*t_left;
  }
};

Vector3 Motion::getPositionRDF(Scalar const& t) const {
  if (t < jerk_time) {
    return 0.1666*jerk_rdf*t*t*t + 0.5*initial_acceleration_rdf*t*t + initial_velocity_rdf*t;
  }
  else {
    double t_left = t - jerk_time;
    return position_end_of_jerk_time_rdf + 0.5*acceleration_rdf*t_left*t_left + velocity_end_of_jerk_time_rdf*t_left;
  }
};

Vector3 Motion::getPositionRDF_MonteCarlo(Scalar const& t, Vector3 const& sampled_initial_velocity_rdf) const {
  if (t < jerk_time) {
    return 0.1666*jerk_rdf*t*t*t + 0.5*initial_acceleration_rdf*t*t + sampled_initial_velocity_rdf*t;
  }
  else {
    Vector3 sampled_position_end_of_jerk_time_rdf = 0.1666*jerk_rdf*jerk_time*jerk_time*jerk_time + 0.5*initial_acceleration_rdf*jerk_time*jerk_time + sampled_initial_velocity_rdf*jerk_time;
    //Vector3 sampled_velocity_end_of_jerk_time_rdf = 0.5*jerk_rdf*jerk_time*jerk_time + initial_acceleration_rdf*jerk_time + sampled_initial_velocity_rdf;
    double t_left = t - jerk_time;
    return sampled_position_end_of_jerk_time_rdf + 0.5*acceleration_rdf*t_left*t_left + sampled_initial_velocity_rdf*t_left;
  }

};

Vector3 Motion::getTerminalStopPositionRDF(Scalar const& t) const {
  Vector3 position_end_of_motion = getPositionRDF(t);
  Vector3 velocity_end_of_motion = getVelocityRDF(t);

  double speed = velocity_end_of_motion.norm();
  
  Vector3 stopping_vector = -velocity_end_of_motion/speed;
  Vector3 max_stop_acceleration = a_max_horizontal*stopping_vector;
  Vector3 stopping_jerk = (max_stop_acceleration - acceleration_rdf) / jerk_time;
  Vector3 position_end_of_jerk_stop = 0.1666*stopping_jerk*jerk_time*jerk_time*jerk_time + 0.5*acceleration_rdf*jerk_time*jerk_time + velocity_end_of_motion*jerk_time + position_end_of_motion;
  Vector3 velocity_end_of_jerk_stop = 0.5*stopping_jerk*jerk_time*jerk_time + acceleration_rdf*jerk_time + velocity_end_of_motion;

  // check if stopped during jerk time
  if (velocity_end_of_motion.dot(velocity_end_of_jerk_stop) < 0) {
    return position_end_of_jerk_stop;
  }

  double realistic_stop_accel = a_max_horizontal*stopping_factor;
  double speed_after_jerk = velocity_end_of_jerk_stop.norm();
  double stop_t_after_jerk = (speed_after_jerk / realistic_stop_accel);
  //double extra_drift = speed_after_jerk*0.200;
  double stopping_distance_after_jerk =  0.5 * -realistic_stop_accel * stop_t_after_jerk*stop_t_after_jerk + speed_after_jerk*stop_t_after_jerk;

  return position_end_of_jerk_stop + stopping_distance_after_jerk*-stopping_vector;

}




