#if defined(ARDUINO)
 //don't interfere with the arduino IDE!
#else
#include "../libraries/arduino-easy-serial/Channel.h"
#include <iostream>
#include <fstream>

using namespace std;

struct tunings{
  float kp,ki,kd;
} t,t2;

HardwareSerial Serial("/dev/cu.usbmodem14111");

Channel<tunings> tu;
Channel<tunings> fu;
Channel<float> ack;



void setup() {
  tu.init(Serial, 115200, 'T');
  fu.init(Serial, 115200, '2');
  ack.init(Serial, 115200, 'A');
  
}

void loop() {
  float* k;
  switch (cin.get()){
    case 'p': k=&t.kp; cout << "kp = "; break;
    case 'i': k=&t.ki; cout << "ki = "; break;
    case 'd': k=&t.kd; cout << "kd = "; break;
    case 'P': k=&t2.kp; cout << "kp2 = "; break;
    case 'I': k=&t2.ki; cout << "ki2 = "; break;
    case 'D': k=&t2.kd; cout << "kd2 = "; break;
    default: return;
  }
  char op=cin.get();
  float mult;
  cin >> mult;
  if(op=='*')
    (*k)*=mult;
  else if(op=='/')
    (*k)/=mult;
  else if(op=='=')
    (*k)=mult;
  cout << (*k) << endl;
  fu.send(t2);
  tu.send(t);
 
  cout << "I got " << ack.next() << endl;
}

int main(){
  setup();
  while(1) loop();
}
#endif
