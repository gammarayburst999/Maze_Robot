#include <time.h>
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
#define PATH_PLAN_FILE_NAME "../../PathPlan.txt"
#define TIME_STEP 64
#define PI 3.14159
#define POSITION_STEP_LINEAR 165.0/20.0 
#define POSITION_STEP_ROTATION PI*14.150/20.0
#include <time.h> 
#define Start_location "00S" 
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
   /*printf("Read path plan from the folder\n"); 
   FILE *fp;
   char buff[255];
   fp = fopen(PATH_PLAN_FILE_NAME, "r");


   fgets(buff, 255, (FILE*)fp);
   printf("%s\n", buff );
   

   fclose(fp);
   printf("Done - Path plan read !\n");
    int los;
  los=strlen(buff);*/
  int CellValues[5][9];
  for(int q=0;q<5;q++)
    {
    for(int q2=0;q2<9;q2++)
    {
    CellValues[q][q2]=99;
    }
    }

int flagR, flagF, flagL;

  
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
 int FLAG=0;
 
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
    char a = Start_location[0];
    int row = a - '0';
    int Sn=0;
    int valSn;
    char b = Start_location[1];
    int col = b - '0';
   char head=Start_location[2];
  CellValues[row][col]=Sn; 
      
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
    flagL=0;
    }
    else
    {
    wall_left='N';
    flagL=1;
    
    }
    
      if(sensor_right>threshold)
    {
    wall_right='Y';
    flagR=0;
    }
    else
    {
    wall_right='N';
    flagR=1;
    }
    
       if(sensor_front>threshold)
    {
    wall_front='Y';
    flagF=0;
    }
    else
    {
    wall_front='N';
    flagF=1;
    }
//CellValues[row][col]=1;
 printf(" Step: %02d, Row: %d, Column: %d, Heading %c, Left Wall: %c, Front Wall: %c, Right Wall: %c  \n ",step_count,row,col,head,wall_left,wall_front,wall_right);
if(head=='S')
      {
      if(flagF==1)
      {
      row =row+1;
      Sn=Sn+1;
      CellValues[row][col]=Sn;
      row=row-1;
      Sn=Sn-1;
       //printf("valuuee of Sn flag F: %d \n", Sn);
      }
        if(flagR==1)
      {
      col =col-1;
      Sn=Sn+1;
      CellValues[row][col]=Sn;
      Sn=Sn-1;
      col =col+1;
      }
       if(flagL==1)
      {
      col =col+1;
      Sn=Sn+1;
      CellValues[row][col]=Sn; 
      Sn=Sn-1;
      col =col-1;
      }
      }
       
       
        if(head=='N')
      {
      if(flagF==1)
      {
      row =row-1;
      Sn=Sn+1;
      CellValues[row][col]=Sn;
      row=row+1;
      Sn=Sn-1;
       //printf("valuuee of Sn flag F: %d \n", Sn);
      }
        if(flagR==1)
      {
      col =col+1;
      Sn=Sn+1;
      CellValues[row][col]=Sn;
      Sn=Sn-1;
      col =col-1;
      }
       if(flagL==1)
      {
      col =col-1;
      Sn=Sn+1;
      CellValues[row][col]=Sn; 
      Sn=Sn-1;
      col =col+1;
      }
      }
        if(head=='E')
       {
      if(flagF==1)
      {
      col =col+1;
      Sn=Sn+1;
      CellValues[row][col]=Sn;
      col=col-1;
      Sn=Sn-1;
       //printf("valuuee of Sn flag F: %d \n", Sn);
      }
        if(flagR==1)
      {
      row =row+1;
      Sn=Sn+1;
      CellValues[row][col]=Sn;
      Sn=Sn-1;
      row =row-1;
      }
       if(flagL==1)
      {
      row =row-1;
      Sn=Sn+1;
      CellValues[row][col]=Sn; 
      Sn=Sn-1;
      row =row+1;
      }
      }
       if(head=='W')
     {
      if(flagF==1)
      {
      col =col-1;
      Sn=Sn+1;
      CellValues[row][col]=Sn;
      col=col+1;
      Sn=Sn-1;
       //printf("valuuee of Sn flag F: %d \n", Sn);
      }
        if(flagR==1)
      {
      row =row-1;
      Sn=Sn+1;
      CellValues[row][col]=Sn;
      Sn=Sn-1;
      row =row+1;
      }
       if(flagL==1)
      {
      row =row+1;
      Sn=Sn+1;
      CellValues[row][col]=Sn; 
      Sn=Sn-1;
      row =row-1;
      }
      }
       ////////////////////////////////
       /////////////////////////////////
       Sn=Sn+1;
   printf("cell value  time \n"); 
   
              
   for(int m2=0; m2<5; m2++) {
      for(int j2=0;j2<9;j2++) {
         printf("%d ", CellValues[m2][j2]);
         if(j2==8){
            printf("\n");  
         }
         }
         }
       //////////////////////////////////  
     ///////////////////////////////    
     ////////////////////////////////    
     //printf("\n \n \n"); 
    while(!((row==2) && (col==4)))
  {
    //printf("\n \n ROW %d COL %d \n \n", row,col);
    valSn=Sn;
    //printf("valuuee of Sn AATTTTT START F: %d \n \n", valSn);
    if(flagF==1){
    if(head=='S')
      {
    if((CellValues[row+1][col]==valSn)||((CellValues[row+1][col]-CellValues[row][col])>=1)){
       left_position += POSITION_STEP_LINEAR;
       right_position += POSITION_STEP_LINEAR; 
       row =row+1;
    }
    }
    if(head=='N')
      {
    if((CellValues[row-1][col]==valSn)||((CellValues[row-1][col]-CellValues[row][col])>=1)){
       left_position += POSITION_STEP_LINEAR;
       right_position += POSITION_STEP_LINEAR; 
       row =row-1;
    }
    }
    if(head=='W')
      {
    if((CellValues[row][col-1]==valSn)||((CellValues[row][col-1]-CellValues[row][col])>=1)){
       left_position += POSITION_STEP_LINEAR;
       right_position += POSITION_STEP_LINEAR; 
      col =col-1;
    }
    }
    if(head=='E')
      {
      //printf("valuuee of Sn flag F: %d \n", valSn);
     // printf("cellvalue of  flag F: %d \n", CellValues[row][col+1]);
    if((CellValues[row][col+1]==valSn)||((CellValues[row][col+1]-CellValues[row][col])>=1)){
       left_position += POSITION_STEP_LINEAR;
       right_position += POSITION_STEP_LINEAR; 
      col =col+1;
    }
    }
    }
    if(flagF==0)
    {
    Sn=Sn-1;
    }
    if(flagR==1)
    {
    if(head=='S')
      {
    if(CellValues[row][col-1]==valSn){
    left_position += POSITION_STEP_ROTATION;
    right_position -= POSITION_STEP_ROTATION; 
       head='W';
    }
    }
    if(head=='N')
      {
    if(CellValues[row][col+1]==valSn){
    left_position += POSITION_STEP_ROTATION;
    right_position -= POSITION_STEP_ROTATION;
       head='E';
    }
    }
    if(head=='W')
      {
    if(CellValues[row-1][col]==valSn){
    left_position += POSITION_STEP_ROTATION;
    right_position -= POSITION_STEP_ROTATION; 
      head='N';
    }
    }
    if(head=='E')
      {
    if(CellValues[row+1][col]==valSn){
    left_position += POSITION_STEP_ROTATION;
    right_position -= POSITION_STEP_ROTATION;
     head='S';
    }
    }
    }
    if(flagL==1)
    {
    if(head=='S')
      {
    if(CellValues[row][col+1]==valSn){
    left_position -= POSITION_STEP_ROTATION;
    right_position += POSITION_STEP_ROTATION; 
       head='E';
    }
    }
    if(head=='N')
      {
    if(CellValues[row][col-1]==valSn){
      left_position -= POSITION_STEP_ROTATION;
    right_position += POSITION_STEP_ROTATION; 
       head='W';
    }
    }
    if(head=='W')
      {
    if(CellValues[row+1][col]==valSn){
    left_position -= POSITION_STEP_ROTATION;
    right_position += POSITION_STEP_ROTATION; 
      head='S';
    }
    }
    if(head=='E')
      {
    if(CellValues[row-1][col]==valSn){
    left_position -= POSITION_STEP_ROTATION;
    right_position += POSITION_STEP_ROTATION; 
     head='N';
    }
    }
    }
    
    
     if(flagF==0 && flagR==0 && flagL==0)
     {
    
     FLAG=1;
      {
    if(head=='S')
      {
    
    left_position -= POSITION_STEP_ROTATION;
    right_position += POSITION_STEP_ROTATION; 
       head='E';
    
    }
    if(head=='N')
      {
   
      left_position -= POSITION_STEP_ROTATION;
    right_position += POSITION_STEP_ROTATION; 
       head='W';
    
    }
    if(head=='W')
      {
   
    left_position -= POSITION_STEP_ROTATION;
    right_position += POSITION_STEP_ROTATION; 
      head='S';
    
    }
    if(head=='E')
      {
   
    left_position -= POSITION_STEP_ROTATION;
    right_position += POSITION_STEP_ROTATION; 
     head='N';
    
    }
    }
     }
       if(flagF==1 && flagR==0 && flagL==0)
       {
       
       }
   wb_motor_set_position(left_motor, left_position);
   wb_motor_set_position(right_motor, right_position);
    WbDeviceTag right_motor_position_sensor = wb_motor_get_position_sensor(right_motor);
    motor_set_position_sync(right_motor, right_motor_position_sensor, right_position, 3800); 
    step_count=step_count+1;
        sensor_right = wb_distance_sensor_get_value(ds_right);
         sensor_left = wb_distance_sensor_get_value(ds_left);
        sensor_front = wb_distance_sensor_get_value(ds_front);  
    
       
    if(sensor_left>threshold)
    {
    wall_left='Y';
    flagL=0;
    }
    else
    {
    wall_left='N';
    flagL=1;
    }
    
    
    if(sensor_right>threshold)
    {
    wall_right='Y';
    flagR=0;
    }
    else
    {
    wall_right='N';
    flagR=1;
    }
    
    if(sensor_front>threshold)
    {
    wall_front='Y';
    flagF=0;
    }
    else
    {
    wall_front='N';
    flagF=1;
    } 
    
    // SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS
    
    if(head=='S')
      {
      if(flagF==1)
      {
      row =row+1;
      
      Sn=Sn+1;
      if(CellValues[row][col]>=Sn)
      {
      CellValues[row][col]= CellValues[row-1][col]+1;
      }
      
      
      row=row-1;
      Sn=Sn-1;
       //printf("valuuee of Sn flag F: %d \n", Sn);
      }
        if(flagR==1)
      {
      col =col-1;
      Sn=Sn+1;
      if(CellValues[row][col]>=Sn)
      {
      CellValues[row][col]=CellValues[row][col+1]+1;
      }
      Sn=Sn-1;
      col =col+1;
      }
       if(flagL==1)
      {
      col =col+1;
      Sn=Sn+1;
      if(CellValues[row][col]>=Sn)
      {
      CellValues[row][col]=CellValues[row][col-1]+1;
      }
      Sn=Sn-1;
      col =col-1;
      }
      }
       
      //NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN 
      if(head=='N')
      {
      if(flagF==1)
      {
      row =row-1;
      Sn=Sn+1;
      if(CellValues[row][col]>=Sn)
      {
      CellValues[row][col]=CellValues[row+1][col]+1;
      }
      row=row+1;
      Sn=Sn-1;
       //printf("valuuee of Sn flag F: %d \n", Sn);
      }
        if(flagR==1)
      {
      col =col+1;
      Sn=Sn+1;
      if(CellValues[row][col]>=Sn)
      {
      CellValues[row][col]=CellValues[row][col-1]+1;
      }
      Sn=Sn-1;
      col =col-1;
      }
       if(flagL==1)
      {
      col =col-1;
      Sn=Sn+1;
      if(CellValues[row][col]>=Sn)
      {
      CellValues[row][col]=CellValues[row][col+1]+1;
      }
      Sn=Sn-1;
      col =col+1;
      }
      }
      
      //EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
       if(head=='E')
       {
      if(flagF==1)
      {
      col =col+1;
      Sn=Sn+1;
      if(CellValues[row][col]>=Sn)
      {
      CellValues[row][col]=CellValues[row][col-1]+1;
      }
      col=col-1;
      Sn=Sn-1;
       //printf("valuuee of Sn flag F: %d \n", Sn);
      }
        if(flagR==1)
      {
      row =row+1;
      Sn=Sn+1;
      if(CellValues[row][col]>=Sn)
      {
      CellValues[row][col]=CellValues[row-1][col]+1;
      }
      Sn=Sn-1;
      row =row-1;
      }
       if(flagL==1)
      {
      row =row-1;
      Sn=Sn+1;
      if(CellValues[row][col]>=Sn)
      {
      CellValues[row][col]=CellValues[row+1][col]+1;
      } 
      Sn=Sn-1;
      row =row+1;
      }
      }
      
      //WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW
      if(head=='W')
     {
      if(flagF==1)
      {
      col =col-1;
      Sn=Sn+1;
      if(CellValues[row][col]>=Sn)
      {
      CellValues[row][col]=Sn;
      }
      col=col+1;
      Sn=Sn-1;
       //printf("valuuee of Sn flag F: %d \n", Sn);
      }
        if(flagR==1)
      {
      row =row-1;
      Sn=Sn+1;
      if(CellValues[row][col]>=Sn)
      {
      CellValues[row][col]=Sn;
      }
      Sn=Sn-1;
      row =row+1;
      }
       if(flagL==1)
      {
      row =row+1;
      Sn=Sn+1;
      if(CellValues[row][col]>=Sn)
      {
      CellValues[row][col]=Sn;
      }
      Sn=Sn-1;
      row =row-1;
      }
      }
      
      if(FLAG!=1)
      {
      Sn=Sn+1;
      
      }
       if(FLAG==1)
      {
      Sn=Sn-1;
      FLAG=0;
      }
      
    //  printf("\n cell value - Sn=========== %d \n", CellValues[row][col]-Sn); 
      if(CellValues[row][col]-Sn==-2)
      {
      Sn=CellValues[row][col]+1;
      }
     
      
    
    
    
    
    
    
    
    
    
    
    
    
    
    
printf("Step: %02d, Row: %d, Column: %d, Heading %c, Left Wall: %c, Front Wall: %c, Right Wall: %c  \n ",step_count,row,col,head,wall_left,wall_front,wall_right); 
printf("cell value Step time %02d \n", step_count); 
  for(int m2=0; m2<5; m2++) {
      for(int j2=0;j2<9;j2++) {
         printf("%02d ", CellValues[m2][j2]);
         if(j2==8){
            printf("\n");  
         }
         }
         }
 
}


printf("Done - Path plan executed !\n");
 wb_robot_cleanup();
 return 0;
}
}