#include <PinChangeInterrupt.h>
#include <PinChangeInterruptBoards.h>
#include <PinChangeInterruptPins.h>
#include <PinChangeInterruptSettings.h>
#include <Encoder.h>
#include <Joystick.h>

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_GAMEPAD,
  2, 0,                  // Button Count, Hat Switch Count
  true, true, false,     // X and Y, but no Z Axis
  false, false, false,   // No Rx, Ry, or Rz
  false, false,          // No rudder or throttle
  false, false, false);  // No accelerator, brake, or steering

#define LEFT_KNOB_A 2
#define LEFT_KNOB_B 3
#define LEFT_KNOB_BUTTON 8

#define RIGHT_KNOB_A 1
#define RIGHT_KNOB_B 0
#define RIGHT_KNOB_BUTTON 9

void leftButtonInterrupt() {
  uint8_t trigger = getPCINTTrigger(digitalPinToPCINT(LEFT_KNOB_BUTTON));

  if (trigger == FALLING) {
    Joystick.setButton(0, 1);
  }
  else  if (trigger == RISING) {
    Joystick.setButton(0, 0);
  }    
}

void rightButtonInterrupt() {
  uint8_t trigger = getPCINTTrigger(digitalPinToPCINT(RIGHT_KNOB_BUTTON));

  if (trigger == FALLING) {
    Joystick.setButton(1, 1);
  }
  else  if (trigger == RISING) {
    Joystick.setButton(1, 0);
  }    
}

Encoder knobLeft(LEFT_KNOB_A, LEFT_KNOB_B);
Encoder knobRight(RIGHT_KNOB_A, RIGHT_KNOB_B);

void setup() {
  pinMode(LEFT_KNOB_BUTTON, INPUT_PULLUP);
  pinMode(RIGHT_KNOB_BUTTON, INPUT_PULLUP);

  //interrupts for buttons
  attachPCINT(digitalPinToPCINT(LEFT_KNOB_BUTTON), leftButtonInterrupt, CHANGE);
  attachPCINT(digitalPinToPCINT(RIGHT_KNOB_BUTTON), rightButtonInterrupt, CHANGE);

  Joystick.begin();
  Joystick.setXAxisRange(-1, 1);
  Joystick.setYAxisRange(-1, 1);

  //debugging
  #ifdef DEBUG
    Serial.begin(9600);
  #endif
}

void loop() {
  int left_knob, right_knob;
  
  left_knob = knobLeft.read();
  if (left_knob > 0 && left_knob % 4 == 0) {
    for (int i = 0; i < left_knob / 4; i++) {
      Joystick.setYAxis(1);
      #ifdef DEBUG
        Serial.print("Left Knob UP");
        Serial.println();
      #endif
    }
    knobLeft.write(0);
  }
  else if (left_knob < 0 && left_knob % 4 == 0) {
    for (int i = 0; i > left_knob / 4; i--) {
      Joystick.setYAxis(-1);
      #ifdef DEBUG
        Serial.print("Left Knob DOWN");
        Serial.println();
      #endif
    }
    knobLeft.write(0);
  }
  else {
    Joystick.setYAxis(0);
  }
    
  
  right_knob = knobRight.read();
  if (right_knob > 0 && right_knob % 4 == 0) {
    for (int i = 0; i < right_knob / 4; i++) {
      Joystick.setXAxis(1);
      #ifdef DEBUG
        Serial.print("Right Knob UP");
        Serial.println();
      #endif
    }
    knobRight.write(0);
  }
  else if (right_knob < 0 && right_knob % 4 == 0) {
    for (int i = 0; i > right_knob / 4; i--) {
      Joystick.setXAxis(-1);
      #ifdef DEBUG
        Serial.print("Right Knob DOWN");
        Serial.println();
      #endif
    }
    knobRight.write(0);
  }
  else {
    Joystick.setXAxis(0);
  }
}
