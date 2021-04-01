#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <webots/position_sensor.h>
#include <webots/distance_sensor.h>
#include <stdio.h> 
#include <webots/robot.h>
// Added a new include file
#include <webots/motor.h>

#define TIME_STEP 64
#define SPEED 6.28*0.4
#define PATH_PLAN_FILE_NAME ../../PathPlan2.txt
#define TIME_STEP 64
#define PI 3.14159
#define POSITION_STEP_LINEAR 165.0/20.0 
#define POSITION_STEP_ROTATION PI*14.150/20.0
#include <time.h> 
  
  void motor_set_position_sync(WbDeviceTag tag_motor, WbDeviceTag tag_sensor, double target, int delay) {
  const double DELTA = 0.001;  // max tolerated difference
  wb_motor_set_position(tag_motor, target);
  wb_position_sensor_enable(tag_sensor, TIME_STEP);
  double effective;  // effective position
  do {
    if (wb_robot_step(TIME_STEP) == -1)
      break;
    delay -= TIME_STEP;
    effective = wb_position_sensor_get_value(tag_sensor);
  } while (fabs(target - effective) > DELTA && delay > 0);
  wb_position_sensor_disable(tag_sensor);
}
  
  
  

int main(int argc, char **argv) {
   wb_robot_init();
   printf("Read path plan from the folder which has the codep\n"); 
   FILE *fp;
   char buff[255];
   fp = fopen(PATH_PLAN_FILE_NAME, "r");


   fgets(buff, 255, (FILE*)fp);
   printf("%s\n", buff );
   

   fclose(fp);
   printf("Done - Path plan read !\n");
    int los;
  los=strlen(buff);



  
  double left_position = 0.0;
  double right_position = 0.0;
  
  double left_encoder_value = 0.0;
  double right_encoder_value = 0.0;
  
  
  int threshold=85; //change this for detecting wall

 
  WbDeviceTag left_motor = wb_robot_get_device("left wheel motor");
  WbDeviceTag right_motor = wb_robot_get_device("right wheel motor");
  
  wb_motor_set_velocity(left_motor, SPEED);
  wb_motor_set_velocity(right_motor, SPEED);
  
  wb_motor_set_position(left_motor, left_position);
  wb_motor_set_position(right_motor, right_position);
  
  //WbDeviceTag ds = wb_robot_get_device("dsF");
  WbDeviceTag ps = wb_robot_get_device("ps1");
  
  WbDeviceTag left_encoder = wb_robot_get_device("left wheel sensor");
  WbDeviceTag right_encoder = wb_robot_get_device("right wheel sensor");
  
  
  //wb_distance_sensor_enable(ds, TIME_STEP);
  wb_distance_sensor_enable(ps, TIME_STEP);
  
  wb_position_sensor_enable(left_encoder, TIME_STEP);
  wb_position_sensor_enable(right_encoder, TIME_STEP);
  
   WbDeviceTag ds_right = wb_robot_get_device("ds_right");
   wb_distance_sensor_enable(ds_right, TIME_STEP);
    WbDeviceTag ds_left = wb_robot_get_device("ds_left");
   wb_distance_sensor_enable(ds_left, TIME_STEP);
   
     WbDeviceTag ds_front = wb_robot_get_device("ds_front");
   wb_distance_sensor_enable(ds_front, TIME_STEP);
   
   
  /*
   * You should declare here WbDeviceTag variables for storing
   * robot devices like this:
   *  WbDeviceTag my_sensor = wb_robot_get_device("my_sensor");
   *  WbDeviceTag my_actuator = wb_robot_get_device("my_actuator");
   */
  

 // get the motor devices

 int step_count=0;
 
 
   printf("Start - Execute path plan!\n");
 
   while (wb_robot_step(TIME_STEP) != -1){
 
  // ds_values = wb_distance_sensor_get_value(ds);
   //ps_values = wb_distance_sensor_get_value(ps);
   
      left_position = left_encoder_value;
      right_position = right_encoder_value;
   
   left_encoder_value = wb_position_sensor_get_value(left_encoder); 
   right_encoder_value = wb_position_sensor_get_value(right_encoder);
 
    //  printf("left encoder at the start: %f \n", left_encoder_value);
    // printf("right encoder at the start: %f \n", right_encoder_value); 

     
    //wb_motor_set_position(right_motor, right_position);
    //printf("Step: %d \n", step_count);
    char a = buff[0];
    int row = a - '0';
    
    char b = buff[1];
    int col = b - '0';
   char head=buff[2];
   
      
     char wall_left='Y';
     char wall_right='Y';
     char wall_front='N';
     
   double sensor_right=0;
    double sensor_left=0;
     double sensor_front=0;
     
  
         wb_robot_step(TIME_STEP);
        sensor_right = wb_distance_sensor_get_value(ds_right);
        sensor_left = wb_distance_sensor_get_value(ds_left);
        sensor_front = wb_distance_sensor_get_value(ds_front);
        wb_robot_step(TIME_STEP);
        sensor_right = wb_distance_sensor_get_value(ds_right);
        sensor_left = wb_distance_sensor_get_value(ds_left);
        sensor_front = wb_distance_sensor_get_value(ds_front);
        
        /*  printf(" front%f", sensor_front);
   printf(" right %f", sensor_right);
 printf(" left %f", sensor_left); */
    
    if(sensor_left>threshold)
    {
    wall_left='Y';
    }
    else
    {
    wall_left='N';
    }
    
      if(sensor_right>threshold)
    {
    wall_right='Y';
    }
    else
    {
    wall_right='N';
    }
    
       if(sensor_front>threshold)
    {
    wall_front='Y';
    }
    else
    {
    wall_front='N';
    }

 printf(" Step: %02d, Row: %d, Column: %d, Heading %c, Left Wall: %c, Front Wall: %c, Right Wall: %c  \n ",step_count,row,col,head,wall_left,wall_front,wall_right);

    
    for(int i=3;i<los;i++)
    {
 //   printf("start %c \n ", buff[i]);
   // left_encoder_value = wb_position_sensor_get_value(left_encoder); 
   //right_encoder_value = wb_position_sensor_get_value(right_encoder);
      //  left_position = left_encoder_value;
    // right_position = right_encoder_value;
      //printf("left encoder in the outer loop: %f \n", left_encoder_value);
    //  printf("right encoder  in the outer loop: %f \n", right_encoder_value); 
        
          
   
    if(buff[i]=='F'){
          //go forward
       left_position += POSITION_STEP_LINEAR;
       right_position += POSITION_STEP_LINEAR; 

      
      if(head=='S')
      {
      row =row+1;
       }
      else  if(head=='N')
      {
      row =row-1;
       }
       else  if(head=='E')
      {
      col =col+1;
       }
        else  if(head=='W')
      {
      col =col-1;
       }
       }
      
      
    
    else if(buff[i]=='L'){
    //turn right    
    left_position -= POSITION_STEP_ROTATION;
    right_position += POSITION_STEP_ROTATION;
    
    if(head=='S')
      {
     head='E';
       }
      else  if(head=='N')
      {
      head='W';
       }
       else  if(head=='E')
      {
      head='N';
       }
        else  if(head=='W')
      {
      head='S';
       }
 
 }
 
   else if(buff[i]=='R'){
    //turn right    
    left_position += POSITION_STEP_ROTATION;
    right_position -= POSITION_STEP_ROTATION;
  
     if(head=='S')
      {
     head='W';
       }
      else  if(head=='N')
      {
      head='E';
       }
       else  if(head=='E')
      {
      head='S';
       }
        else  if(head=='W')
      {
      head='N';
       }
 
 
 }
    
   wb_motor_set_position(left_motor, left_position);
   wb_motor_set_position(right_motor, right_position);
    WbDeviceTag right_motor_position_sensor = wb_motor_get_position_sensor(right_motor);
    motor_set_position_sync(right_motor, right_motor_position_sensor, right_position, 3800); 
    step_count=step_count+1;
        sensor_right = wb_distance_sensor_get_value(ds_right);
         sensor_left = wb_distance_sensor_get_value(ds_left);
        sensor_front = wb_distance_sensor_get_value(ds_front);
      /*  printf(" front%f ", sensor_front);
   printf(" right %f ", sensor_right);
 printf(" left %f  ", sensor_left);*/
    
    if(sensor_left>threshold)
    {
    wall_left='Y';
    }
    else
    {
    wall_left='N';
    }
    
      if(sensor_right>threshold)
    {
    wall_right='Y';
    }
    else
    {
    wall_right='N';
    }
    
       if(sensor_front>threshold)
    {
    wall_front='Y';
    }
    else
    {
    wall_front='N';
    }
   //printf("Step: %02d, Row: %d, Column: %d, Heading: %c,  Left Wall: %c, Front Wall: %c, Right Wall: %c \n ",step_count,row,col,head, wall_left, wall_front, wall_right);
   printf("Step: %02d, Row: %d, Column: %d, Heading %c, Left Wall: %c, Front Wall: %c, Right Wall: %c  \n ",step_count,row,col,head,wall_left,wall_front,wall_right);
  
}
printf("Done - Path plan executed !\n");
 wb_robot_cleanup();
 return 0;
}
}