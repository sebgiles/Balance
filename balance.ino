#include "Wire.h"
#include "Channel.h"
#include "PID.h"
#include "Motor.h"

#include "MPU9250.h"

MPU9250 accelgyro;
Motor servo(2,3);

struct PIDData{
    double input, output;
} pd;
double target;
Channel<PIDData> ch;

PID contr(&pd.input, &pd.output, &target,0,0,0,DIRECT);


double input() {
  static long lastTime=0;
  long time=millis();
  double rot=0;
  if(0!=lastTime) 
    rot= (time-lastTime)*(double)accelgyro.getRotationY()/(1024.0);
  lastTime=time;
  return rot;
}


void setup(){  
  
  struct tunings { float kp, ki, kd; };
  Channel<tunings> tu;
  tu.init(Serial, 115200, 'T');
  tunings t = tu.next();
  contr.SetTunings(t.kp, t.ki, t.kd);

  contr.SetSampleTime(50);
  contr.SetOutputLimits(-1,1);
  
  ch.init(Serial, 115200, 'P');

  Wire.begin();
  accelgyro.initialize();
  
  delay(2000);

  target = input();
  
  contr.SetMode(AUTOMATIC);

}

void loop()
{
  pd.input+=input();
  if(contr.Compute()){
    servo.turn(pd.output);
    ch.send(pd);
  }
}
