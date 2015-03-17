/*
 * File:          firebird6_obstacle_avoidance.c
 * Date:          17 March 2015
 * Description:   Demo code for obstacle avoidance on Firebird 6 robot
 * Author:        Anant Malewar; Nex Robotics
 * Modifications: 
 */

/*
 * You may need to add include files like <webots/distance_sensor.h> or
 * <webots/differential_wheels.h>, etc.
 */
#include <stdio.h>
#include <webots/robot.h>
#include <webots/differential_wheels.h>
#include <webots/distance_sensor.h>
#include <webots/light_sensor.h> 
#include <webots/accelerometer.h>
#include <webots/compass.h> 
#include <webots/gyro.h>
/*
 * You may want to add macros here.
 */
#define TIME_STEP 64

/*
 * This is the main program.
 * The arguments of the main function can be specified by the
 * "controllerArgs" field of the Robot node
 */
int main(int argc, char **argv)
{
  /* necessary to initialize webots stuff */
  wb_robot_init();
  printf("FB6 controller for obstacle avoidance\n");
  
  /*
   * You should declare here WbDeviceTag variables for storing
   * robot devices like this:
   *  WbDeviceTag my_sensor = wb_robot_get_device("my_sensor");
   *  WbDeviceTag my_actuator = wb_robot_get_device("my_actuator");
   */
  int i;
  WbDeviceTag ps[8];

  char ps_names[8][4] = {
  "ps0", "ps1", "ps2", "ps3",
  "ps4", "ps5", "ps6", "ps7"
  };

  for (i=0; i<8; i++) {
  ps[i] = wb_robot_get_device(ps_names[i]);
  wb_distance_sensor_enable(ps[i], TIME_STEP);
  
  }
  
  
  /* main loop
   * Perform simulation steps of TIME_STEP milliseconds
   * and leave the loop when the simulation is over
   */
  while (wb_robot_step(TIME_STEP) != -1) {
    
    /* 
     * Read the sensors :
     * Enter here functions to read sensor data, like:
     *  double val = wb_distance_sensor_get_value(my_sensor);
     */
    double ps_values[8];
    for (i=0; i<8 ; i++)
    {
      ps_values[i] = wb_distance_sensor_get_value(ps[i]);
      printf("%f ", ps_values[i]);
    }
    printf("\n");
    

    int threshold = 250;
    /* Process sensor data here */
    bool left_obstacle = ps_values[0] < threshold || ps_values[1] < threshold || ps_values[7] < threshold || ps_values[2] < threshold;
    bool right_obstacle = ps_values[5] < threshold || ps_values[6] < threshold || ps_values[4] < threshold || ps_values[3] < threshold;
    
    /*
     * Enter here functions to send actuator commands, like:
     * wb_differential_wheels_set_speed(100.0,100.0);
     */
    // init speeds
    double left_speed = 1;
    double right_speed = 1;
    // modify speeds according to obstacles
    if (left_obstacle) {
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
  
  /* Enter your cleanup code here */
  
  /* This is necessary to cleanup webots resources */
  wb_robot_cleanup();
  
  return 0;
}
