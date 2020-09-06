#include "esphome.h"

class SonoffDual : public Component, public switch_::Switch {

  int relaybit;

 public:

  static SonoffDual* relay1;
  static SonoffDual* relay2;
  static SonoffDual* button;

  SonoffDual( int bit) {
    relaybit = bit;
  }

  void setup() override {
    // This will be called by App.setup()
  }
  
  void write_state(bool state) override {
    Serial.write(0xA0);
    Serial.write(0x04);
    int relaybitmask = 1 << relaybit;
    int statebyte = 0;
    if (SonoffDual::relay1->state) statebyte |= 0x01;
    if (SonoffDual::relay2->state) statebyte |= 0x02;
    if (SonoffDual::button->state) statebyte |= 0x04;
    statebyte &= ~relaybitmask;
    if ( state) statebyte |= relaybitmask;
    Serial.write( statebyte);
    Serial.write(0xA1);
    Serial.flush();
    publish_state(state);
  }

  void loop() override {
    if ( Serial.available()) {
      char c = Serial.read();
      if ( c == 160) {
        c = Serial.read();
        if ( c == 4) {
          c = Serial.read();
          relay1->publish_state( c & 0x01);
          relay2->publish_state( c & 0x02);
          button->publish_state( c & 0x04);
        }
        c = Serial.read();
      }
    }
  }
};

SonoffDual* SonoffDual::relay1;
SonoffDual* SonoffDual::relay2;
SonoffDual* SonoffDual::button;
