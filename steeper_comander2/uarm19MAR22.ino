#include <cmdArduino.h>

#include <AccelStepper.h>
//#include <MultiStepper.h>
//#include "U8glib.h"
#include <Stepper.h>
#include <Servo.h>

/*****************************************************
This is the demo sketch for the command line interface
by FreakLabs. It's a simple command line interface
where you can define your own commands and pass arguments
to them. 
*****************************************************/
//#include <Cmd.h>

#define BigGear 72.315  // radius
#define SmallGear 17.677  //radius
#define microstepsG 13091
#define microsteps 1309
//#define microstepsG 7500 //este numero es iterado


#define servopin 11
int led_pin = 13;
bool led_blink_enb = false;
int led_blink_delay_time = 1000;
int pwm_pin = 10;
int FEEDR =0;
int FEEDL =0;
float value,valueR,valueL;
float xvalue,yvalue;

// Base 1
#define X_STEP_PIN         54
#define X_DIR_PIN          55
#define X_ENABLE_PIN       38
#define X_MIN_PIN           3
#define X_MAX_PIN           2


//Derecha 2
#define Y_STEP_PIN         60
#define Y_DIR_PIN          61
#define Y_ENABLE_PIN       56
#define Y_MIN_PIN          14
#define Y_MAX_PIN          15

//Izquierda 3
#define Z_STEP_PIN         46
#define Z_DIR_PIN          48
#define Z_ENABLE_PIN       62
#define Z_MIN_PIN          18
#define Z_MAX_PIN          19


//Codo
#define E_STEP_PIN         26
#define E_DIR_PIN          28
#define E_ENABLE_PIN       24


//Wrist
#define E1_STEP_PIN         36
#define E1_DIR_PIN          34
#define E1_ENABLE_PIN       30


#define Q_STEP_PIN         36
#define Q_DIR_PIN          34
#define Q_ENABLE_PIN       30

#define SDPOWER            -1
#define SDSS               53
#define LED_PIN            13

#define FAN_PIN             9

#define PS_ON_PIN          12
#define KILL_PIN           -1

#define HEATER_0_PIN       10
#define HEATER_1_PIN        8
#define FEEDBACKR         13   // ANALOG NUMBERING
#define FEEDBACKL         14   // ANALOG NUMBERING


#define L1 12 
#define L2 12
#define L3 6
#define L4 12
#define L5 6
#define maxspeed 200
#define maxaccel 200

AccelStepper stepper1(1, X_STEP_PIN,X_DIR_PIN);// 
AccelStepper stepper2(1, Y_STEP_PIN,Y_DIR_PIN);// 
AccelStepper stepper3(1, Z_STEP_PIN,Z_DIR_PIN);// Base 1
AccelStepper stepper4(1, E1_STEP_PIN,E1_DIR_PIN);// Wrist
AccelStepper stepper5(1, E_STEP_PIN,E_DIR_PIN);// Codo

Servo myservo;

uint32_t cmdStr2Num(char *str, uint8_t base)
{
    return strtol(str, NULL, base);
}

void setup()
{
  // set the led pin as an output. its part of the demo.
  pinMode(led_pin, OUTPUT); 
  pinMode(pwm_pin, OUTPUT); 
  
  pinMode(Z_ENABLE_PIN, OUTPUT);
  digitalWrite(Z_ENABLE_PIN, 0);
  pinMode(servopin,OUTPUT);
 // myservo.setMaximumPulse(2400);
 // myservo.setMinimumPulse(500);
  myservo.attach(servopin,500,2400);

  pinMode(E_ENABLE_PIN, OUTPUT);
  digitalWrite(E_ENABLE_PIN, 0);

  pinMode(Y_ENABLE_PIN, OUTPUT);
  digitalWrite(Y_ENABLE_PIN, 0);
  pinMode(E1_ENABLE_PIN, OUTPUT);
  digitalWrite(E1_ENABLE_PIN, 0);
  
  // init the  line and set it for a speed of 57600
  cmd.begin(57600);
  cmd.add("reset", reset);
  cmd.add("args", arg_display);
  cmd.add("move", mover);
  cmd.add("M17", M17);
  cmd.add("M18", M18);
  cmd.add("blink", led_blink);
  cmd.add("pwm", led_pwm);
  cmd.add("pos",positioner);
  cmd.add("backr",feedbackr);
  cmd.add("backl",feedbackl);
  cmd.add("stat",stat);
  cmd.add("current",current);
  cmd.add("G1",G1);
  
  stepper3.setMaxSpeed(maxspeed);//Base 1
  stepper3.setAcceleration(170);
  stepper3.setSpeed(maxspeed);
  
  stepper2.setMaxSpeed(maxspeed);
  stepper2.setAcceleration(170);
  stepper2.setSpeed(maxspeed);

  
  stepper1.setMaxSpeed(maxspeed);
  stepper1.setAcceleration(170);
  stepper1.setSpeed(maxspeed);

  stepper5.setMaxSpeed(maxspeed);
  stepper5.setAcceleration(170);
  stepper5.setSpeed(maxspeed);

  stepper4.setMaxSpeed(maxspeed);
  stepper4.setAcceleration(170);
  stepper4.setSpeed(maxspeed);
  //stepper3.moveTo(200);
  //stepper3.move(200);
  myservo.write(180);
  Serial.println("Connected");
  Serial.println("dArm firmware version 1.0");
  //Serial.println("07 Julio 2018");
  Serial.println("19 MAR 2022");
  
}

void loop()
{
  cmd.poll();
  
  if (led_blink_enb)
  {
    digitalWrite(led_pin, HIGH);    // set the LED on
    delay(led_blink_delay_time);    // wait for a second
    digitalWrite(led_pin, LOW);     // set the LED off
    delay(led_blink_delay_time);     
  }

  stepper3.run();  //Base 1
  stepper1.run();
  stepper2.run();
  stepper4.run();
  stepper5.run();
  
  FEEDR = analogRead(FEEDBACKR);
  FEEDL = analogRead(FEEDBACKL);
}

void reset(int arg_cnt, char **args)
{
       if(stepper3.distanceToGo()==0)
              stepper3.setCurrentPosition(0);
       if(stepper2.distanceToGo()==0)
              stepper2.setCurrentPosition(0);
       if(stepper1.distanceToGo()==0)
              stepper1.setCurrentPosition(0);
       if(stepper4.distanceToGo()==0)
              stepper4.setCurrentPosition(0);
       if(stepper5.distanceToGo()==0)
              stepper5.setCurrentPosition(0);
       Serial.println("ok");

}



void M17(int arg_cnt, char **args)
{
       digitalWrite(Z_ENABLE_PIN, 0);
       digitalWrite(Y_ENABLE_PIN, 0);
       digitalWrite(X_ENABLE_PIN, 0);
       digitalWrite(E1_ENABLE_PIN, 0);
       digitalWrite(E_ENABLE_PIN, 0);
       Serial.println("Motors enabled");
}



void feedbackr(int arg_cnt, char **args)
{
       
       Serial.print("R Motor: ");
       Serial.print(FEEDR);

}

void feedbackl(int arg_cnt, char **args)
{
       String str1 = "L Motor ";
       String str2 = " R Motor ";
       String str3 = str1 + FEEDL + str2 + FEEDR; 
       Serial.print(str3);
       //Serial.print("L Motor: ");
       //Serial.print(FEEDL);
       
}

void M18(int arg_cnt, char **args)
{
       digitalWrite(Z_ENABLE_PIN, 1);
       digitalWrite(Y_ENABLE_PIN, 1);
       digitalWrite(X_ENABLE_PIN, 1);
       digitalWrite(E1_ENABLE_PIN, 1);
       digitalWrite(E_ENABLE_PIN, 1);
       Serial.println("Motors disabled");
  
}

void stat(int arg_cnt,char **args)
{
if(stepper1.distanceToGo()==0 && stepper2.distanceToGo()==0 && stepper3.distanceToGo()==0)
    {
    Serial.println("fin");
    return;
    }
 Serial.println("nofin");
 
}

void current(int arg_cnt,char **args)
{
Serial.println("Position:");
Serial.print("Angle R: ");
Serial.println(xvalue);
Serial.print("Angle L: ");
Serial.println(yvalue);

  
}

void positioner(int arg_cnt,char **args)//pos
{
  xvalue = atof(args[1]);
  yvalue = atof(args[2]);

 if(strcmp(args[1],"X")==0)  //Base 1
    {
      while(stepper3.distanceToGo()!=0){
        Serial.print("Distance to Go x: ");
        Serial.println(stepper3.distanceToGo());
        }
      stepper3.moveTo(yvalue*microstepsG/360);
      Serial.println("ok");
     
    }
 if(strcmp(args[1],"Y")==0)
    {
      while(stepper2.distanceToGo()!=0){
        Serial.print("y");
        }
      //stepper2.moveTo(yvalue*microstepsG/360);
      stepper2.moveTo(yvalue*8181/360);
      Serial.println("ok");
      
    }
 if(strcmp(args[1],"Z")==0)
    {
      while(stepper1.distanceToGo()!=0){
        Serial.print("z");
        }
      stepper1.moveTo(yvalue*microstepsG/360);
      Serial.println("ok");
      
    }

 if(strcmp(args[1],"W")==0)//Wrist
    {
      while(stepper4.distanceToGo()!=0){
        Serial.print("w");
        }
      stepper4.moveTo(yvalue*microsteps/360);
      Serial.println("ok"); 
    }
 if(strcmp(args[1],"C")==0)//Codo
    {
      while(stepper5.distanceToGo()!=0){
        Serial.print("c");
        }
      stepper5.moveTo(yvalue*microstepsG/360);
      Serial.println("ok"); 
    }


  }

void mover(int arg_cnt,char **args)
{
 if((strcmp(args[1],"R")==0))
    {
       value = atof(args[2]);
       value = value*microstepsG/360;
       if(stepper3.distanceToGo()==0)
              stepper3.moveTo(value);
       Serial.print("move R=");
       Serial.print(args[2]);
    }

 if((strcmp(args[1],"L")==0))
    {
       value = atof(args[2]);
       value = value*microstepsG/360;
       if(stepper2.distanceToGo()==0)
       stepper2.moveTo(value);
       Serial.print("move L=");
       Serial.print(args[2]);
    }

     if((strcmp(args[1],"B")==0))
    {
       value = atof(args[2]);
       value = value*microstepsG/360;
       if(stepper1.distanceToGo()==0)
       stepper1.moveTo(value);
       Serial.print("move B=");
       Serial.print(args[2]);
    }


   
}

void arg_display(int arg_cnt, char **args)
{
  for (int i=0; i<arg_cnt; i++)
  {
    Serial.print("Arg ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(args[i]);
  }
}


void led_blink(int arg_cnt, char **args)
{
  if (arg_cnt > 1)
  {
    led_blink_delay_time = cmdStr2Num(args[1], 10);
    led_blink_enb = true;
  }
  else
  {
    led_blink_enb = false;
  } 
}


void led_pwm(int arg_cnt, char **args)
{
  int pwm_val;
  
  if (arg_cnt > 1)
  {
    // if args are present, then use the first arg as the brightness level
    pwm_val = cmdStr2Num(args[1], 10);
    analogWrite(pwm_pin, pwm_val);
  }
  else
  {
    // if no args, turn off the LED
    analogWrite(pwm_pin, 0);
  }
}

void G1(int arg_cnt, char **args)
{
  float X,Y,Z;
  float C,C1,D1,D2,A1;
  float phi=3.1416;
  X = atof((args[1]+1));
  Y = atof((args[2]+1));
  Serial.println(args[1]);
  Serial.println(args[2]);
  if((*args[1])=='X')
  {
  Serial.print("X: ");
  Serial.print(X);
  }
  if((*args[2])=='Y')
  {
  Serial.print(" Y: ");
  Serial.println(Y);
  }
  C=sqrt(X*X+Y*Y);
  C1=acos((12*12+12*12-C*C)/(2*12*12))*180/phi;
  D1=atan(Y/X)*180/phi;
  D2=acos((12*12-12*12+C*C)/(2*12*C))*180/phi;
  A1=D1+D2;
  Serial.print(" C1: ");
  Serial.println(C1);
  Serial.print(" A1: ");
  Serial.println(A1);

  if(stepper2.distanceToGo()==0) //left motor
          {
              stepper2.moveTo((A1-90)*microstepsG/360);
              Serial.println("lok");
             // return;
          }
   if(stepper3.distanceToGo()==0) //rigth motor
          {
              stepper3.moveTo((90-C1)*microstepsG/360);
              Serial.println("rok");
             // return;
          }
  return;
}


void uArm(float xarm,float yarm)
{
    float alpha,beta,theta,gamma,phi,m,c,k,f,factor,y,x,x1,y1;
    x=(float)xarm+11;
    y=(float)yarm-4.5;
    factor=180/3.1416;
    m=sqrt(x*x+y*y);
    Serial.print("xarm= ");
    Serial.println(x,2);
    Serial.print("yarm= ");
    Serial.println(y,2);
    gamma=atan(y/x);
    beta=acos((m*m-L1*L1-L2*L2)/(-2*L1*L2));
    alpha=acos((L2*L2-L1*L1-m*m)/(-2*L1*m));
    c=3.141589-beta;
    k=sqrt(L3*L3+L1*L1-2*L3*L1*cos(c));
    phi=acos((L4*L4-k*k-L5*L5)/(-2*k*L5));
    f=acos((L3*L3-k*k-L1*L1)/(-2*k*L1));
    //printf("m = %f gamma=%f alpha= %f beta= %f c= %f k=%f phi=%f f=%f\n",m,gamma*factor,alpha*factor,beta*factor,c*factor,k,phi*factor,f*factor);
    Serial.print("Motor derecho= ");
    Serial.println(-(gamma+alpha)*factor);
    Serial.print("Motor Izquierdo= ");
    Serial.println((gamma+alpha+f+phi)*factor);
    Serial.print("R= ");
    valueR=(gamma+alpha)*factor-38.07;
    Serial.println(valueR);
    valueL=-(gamma+alpha+f+phi)*factor+97.43;
    Serial.print("L= ");
    Serial.println(valueL);

   if(stepper3.distanceToGo()==0)
              stepper3.moveTo(valueR*microstepsG/360);
   if(stepper2.distanceToGo()==0)
              stepper2.moveTo(valueL*microstepsG/360);
    
  
}
