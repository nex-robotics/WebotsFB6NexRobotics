/*
 * File:          fire_bird_6_obstacle_avoidance.c
 * Date:          17 March 2015
 * Description:   Demo code for obstacle avoidance on Firebird 6 robot
 * Author:        Anant Malewar; Nex Robotics
 * Modifications:
 */

#include <webots/robot.h>
#include <webots/differential_wheels.h>
#include <webots/distance_sensor.h>

#include <stdio.h>

#define TIME_STEP 64

int main(int argc, char **argv)
{
  wb_robot_init();

  printf("Fire Bird 6 controller for obstacle avoidance\n");

  int i;
  WbDeviceTag ps[8];

  char ps_names[8][4] = {
    "ps0", "ps1", "ps2", "ps3",
    "ps4", "ps5", "ps6", "ps7"
  };

  for (i = 0; i < 8; ++i) {
    ps[i] = wb_robot_get_device(ps_names[i]);
    wb_distance_sensor_enable(ps[i], TIME_STEP);
  }

  while (wb_robot_step(TIME_STEP) != -1) {

    double ps_values[8];
    for (i = 0; i < 8 ; ++i) {
      ps_values[i] = wb_distance_sensor_get_value(ps[i]);
      printf("%f ", ps_values[i]);
    }
    printf("\n");

    int threshold = 250;

    bool left_obstacle = ps_values[1] < threshold || ps_values[0] < threshold;
    bool right_obstacle = ps_values[3] < threshold || ps_values[4] < threshold;
    bool front_obstacle = ps_values[2] < threshold;

    // init speeds
    double left_speed = 1;
    double right_speed = 1;

    // modify speeds according to obstacles
    if (front_obstacle) {
      // turn back, but slightly right to not block the robot
      left_speed -= 1.0;
      right_speed -= 2.0;
    }
    else if (left_obstacle) {
      // turn right
      left_speed += 2.0;
      right_speed -= 2.0;
    }
    else if (right_obstacle) {
      // turn left
      left_speed -= 2.0;
      right_speed += 2.0;
    }

    // write actuators inputs
    wb_differential_wheels_set_speed(left_speed, right_speed);
  };

  wb_robot_cleanup();

  return 0;
}
