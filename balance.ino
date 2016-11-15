#include "Wire.h"
#include "Channel.h"
#include "PID.h"
#include "Motor.h"
#include "MPU9250.h"
#include <Servo.h>


MPU9250 accelgyro;
Motor servo(2,3);
Servo arm;

double input, output, target;
double i2, o2, t2;

struct tunings { 
  float kp, ki, kd; 
};

Channel<tunings> tu;
Channel<tunings> fu;
Channel<float> ack;


PID contr(&input, &output, &target,0.000005,0,0,DIRECT);
PID s2s(&i2, &o2, &t2,0.0005,0,0,DIRECT);

double rotation() {
  static long lastTime=0;
  long time=micros();
  double rot=0;
  if(0!=lastTime) 
    rot= (time-lastTime)*(double)accelgyro.getRotationY()/(1024.0);
  lastTime=time;
  return rot;
}
double tilt() {
  static long lastTime=0;
  long time=micros();
  double rot=0;
  if(0!=lastTime) 
    rot= (time-lastTime)*(double)accelgyro.getRotationX()/(1024.0);
  lastTime=time;
  return rot;
}

void tune(tunings& t){
  contr.SetTunings(t.kp, t.ki, t.kd);
  target = input;
  ack.send(t.kp);
}
void tune2(tunings& t){
  s2s.SetTunings(t.kp, t.ki, t.kd);
  t2 = i2;
  ack.send(t.kp);
}

void setup(){  
  
  tu.init(Serial, 115200, 'T', tune);
  fu.init(Serial, 115200, '2', tune2);
  ack.init(Serial, 115200, 'A');
  contr.SetOutputLimits(-1,1);
  contr.SetSampleTime(20);
  s2s.SetSampleTime(20);
  s2s.SetOutputLimits(60,180);
  arm.attach(5);
  Wire.begin();
  accelgyro.initialize();
  delay(1000);

  target = rotation();
  t2=tilt();
  
  contr.SetMode(AUTOMATIC);
  s2s.SetMode(AUTOMATIC);

}

void loop()
{
  input+=rotation();
  i2-=tilt();
  if(contr.Compute()){
    servo.turn(output);
  }
  if(s2s.Compute()){
    arm.write(o2);
  }
  tu.check();
  fu.check();
}
