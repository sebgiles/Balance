#if defined(ARDUINO)
 //don't interfere with the arduino IDE!
#else
#include "../libraries/arduino-easy-serial/Channel.h"
#include <iostream>

using namespace std;

struct data{
  double input, o;
} d;

struct tunings{
  float kp,ki,kd;
} t;

HardwareSerial Serial("/dev/cu.usbmodem14111");

Channel<data> ch;
Channel<tunings> tu;

void setup() {
  ch.init(Serial, 115200, 'P');
  tu.init(Serial, 115200, 'T');
  cout << "KP?\t"; cin >> t.kp;
  cout << "KI?\t"; cin >> t.ki;
  cout << "KD?\t"; cin >> t.kd;
  tu.send(t);
}

void loop() {
  d = ch.next();
  cout  << d.input << '\t' << d.o << "\n";

}

int main(){
  setup();
  while(1) loop();
}
#endif
