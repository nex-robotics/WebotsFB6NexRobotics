#include "entry_points.hpp"
#include "Wrapper.hpp"

bool wbr_init(WbrInterface *ri) {
  Wrapper::init();

  ri->mandatory.wbr_start = Wrapper::start;
  ri->mandatory.wbr_stop = Wrapper::stop;
  ri->mandatory.wbr_has_failed = Wrapper::hasFailed;
  ri->mandatory.wbr_robot_step = Wrapper::robotStep;
  ri->mandatory.wbr_stop_actuators = Wrapper::stopActuators;

  ri->wbr_custom_function = Wrapper::callCustomFunction;

  ri->wbr_differential_wheels_encoders_set_refresh_rate = Wrapper::setEncoderRefreshRate;
  ri->wbr_set_refresh_rate = Wrapper::setRefreshRate;
  ri->wbr_differential_wheels_set_speed = Wrapper::differentialWheelsSetSpeed;
  ri->wbr_differential_wheels_set_encoders = Wrapper::differentialWheelsSetEncoders;

  return true;
}

void wbr_cleanup() {
  Wrapper::cleanup();
}
