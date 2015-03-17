/*
 * File:          epuck_collision_avoidance.c
 * Date:          
 * Description:   
 * Author:        
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
  WbDeviceTag ps[8], sharp[8], grs[8], acc1,cmpXY1,cmpZ1, gyro1;
  double left_encoder, right_encoder;
  char ps_names[8][4] = {
  "ps0", "ps1", "ps2", "ps3",
  "ps4", "ps5", "ps6", "ps7"
  };

  char sharp_names[8][9] = {
  "sharp_00", "sharp_01", "sharp_02", "sharp_03",
  "sharp_04", "sharp_05", "sharp_06", "sharp_07"
  };

  /*char groundSensor[8][4] = {
  "gs0", "gs1", "gs2", "gs3", "gs4", "gs5", "gs6", "gs7"
  };*/

  for (i=0; i<8; i++) {
  ps[i] = wb_robot_get_device(ps_names[i]);
  wb_distance_sensor_enable(ps[i], TIME_STEP);
  
  sharp[i] = wb_robot_get_device(sharp_names[i]);
  wb_distance_sensor_enable(sharp[i], TIME_STEP);
  

  }
  
  /*for (i=0; i< 8; i++) {
    grs[i] = wb_robot_get_device(groundSensor[i]);
    wb_light_sensor_enable(grs[i], TIME_STEP);
  
  }*/
  
  // Enable Acceleometer
  acc1 = wb_robot_get_device("accelerometer_01");
  wb_accelerometer_enable(acc1, TIME_STEP);
  
  //Enable compass
  cmpXY1 = wb_robot_get_device("compassXY_01");
  wb_compass_enable(cmpXY1, TIME_STEP);
  cmpZ1 = wb_robot_get_device("compassZ_01");
  wb_compass_enable(cmpZ1, TIME_STEP);
  
  // Enable Gyro
  gyro1 = wb_robot_get_device("gyro_01");
  wb_gyro_enable(gyro1, TIME_STEP);
  
  // Enable encoders
  wb_differential_wheels_enable_encoders(1);
  
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
    
    double sharp_values[8];
    for (i=0; i<8 ; i++)
    {
      sharp_values[i] = wb_distance_sensor_get_value(sharp[i]);
      //printf("s = %f ", sharp_values[i]);
    }
    //printf("\n");
    
    /*double gs_values[8];
    for (i=0; i<8 ; i++)
    {
      gs_values[i] = wb_light_sensor_get_value(grs[i]);
      //printf("gs = %f ", gs_values[i]);
    }*/
    //printf("\n");
    

    // read encoders
    left_encoder = wb_differential_wheels_get_left_encoder();
    right_encoder = wb_differential_wheels_get_right_encoder();
    //printf("left enc = %f ", left_encoder);
    //printf("right enc = %f ", right_encoder);
    
    // Read accelerometer
    const double *accXYZ;
    accXYZ = wb_accelerometer_get_values(acc1);
    /*printf("AX %f ", accXYZ[0]);
    printf("AY %f ", accXYZ[1]);
    printf("AZ %f ", accXYZ[2]);
    printf("\n");*/
    
    
    // Read compass
    const double *cmpXY, *cmpZ;
    cmpXY = wb_compass_get_values(cmpXY1);
    cmpZ = wb_compass_get_values(cmpZ1);
    /*printf("MX %f ", cmpXY[0]);
    printf("MY %f ", cmpXY[1]);
    printf("MZ %f ", cmpZ[2]);
    printf("\n");*/
    
    // calculate bearing
    /*double rad = atan2(cmpXY[0], cmpZ[2]);
    double bearing = (rad ) / 3.1428 * 180.0;
    if (bearing < 0.0)
    bearing = bearing + 360.0;
    printf("bearing = %f \n", bearing);*/
  
    
    // Read gyro
    const double *gyroXYZ;
    gyroXYZ = wb_gyro_get_values(gyro1);
    /*printf("GX %f ", gyroXYZ[0]);
    printf("GY %f ", gyroXYZ[1]);
    printf("GZ %f ", gyroXYZ[2]);
    printf("\n");*/
    


    int threshold = 250;
    /* Process sensor data here */
    //bool left_obstacle = ps_values[0] > 100.0 || ps_values[1] > 100.0 || ps_values[2] > 100.0;
    //bool right_obstacle = ps_values[5] > 100.0 || ps_values[6] > 100.0 || ps_values[7] > 100.0;
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
    printf("%f ", left_speed);
    printf("%f ", right_speed);
    // write actuators inputs
    //left_speed = right_speed = -100;
    wb_differential_wheels_set_speed(left_speed, right_speed);
    //wb_differential_wheels_set_speed(5,-5);
    

  };
  
  /* Enter your cleanup code here */
  
  /* This is necessary to cleanup webots resources */
  wb_robot_cleanup();
  
  return 0;
}
