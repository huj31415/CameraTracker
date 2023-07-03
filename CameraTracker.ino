#include <AccelStepper.h>

#define StepPin 10
#define DirPin 16
#define LEDPin 9
#define BtnPin 2
#define RewindPin 3

// steps per revolution accounting for microsteps
#define StepsPerRevolution 200. * 32.

bool tracking = false;

// Length of a sidereal day in milliseconds
#define SiderealDaySec 86164.0905

const float trackSpeed = (48. * StepsPerRevolution / SiderealDaySec);
const int rewindSpeed = 3200;

AccelStepper stepper(AccelStepper::DRIVER, StepPin, DirPin);

void toggleTracking() {
  tracking = !tracking;
  if (tracking) {
    digitalWrite(LEDPin, HIGH);
  } else {
    digitalWrite(LEDPin, LOW);
  }
}

void setup() {
  pinMode(LEDPin, OUTPUT);
  pinMode(RewindPin, INPUT);
  pinMode(BtnPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BtnPin), toggleTracking, RISING);
  stepper.setMaxSpeed(rewindSpeed);
}

void loop() {
  if (tracking) {
    stepper.setSpeed(trackSpeed);
    stepper.runSpeed();
  } else if (digitalRead(RewindPin)) {
    stepper.setSpeed(-rewindSpeed);
    stepper.runSpeed();
  } else {
    stepper.stop();
  }
}
