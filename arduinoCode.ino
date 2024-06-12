#include <Servo.h>

const int TRIG_PIN = 6;
const int ECHO_PIN = 7;
const int SERVO_PIN = 9;
const int DISTANCE_THRESHOLD = 50;
float distance_cm;
bool Full = false;

Servo servo; 

void Servo_Controller()
{
    if (distance_cm < DISTANCE_THRESHOLD) {
    servo.write(90); // Xoay servo đến 90 độ
    delay(5000);
  } else {
    servo.write(0); // Xoay servo về 0 độ
  }
}
void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  servo.attach(SERVO_PIN);
  servo.write(0); // Đặt góc ban đầu của servo
}

void loop() {
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  float duration_us = pulseIn(ECHO_PIN, HIGH);
  distance_cm = 0.017 * duration_us;

  if(!Full) Servo_Controller();

  if(Serial.readString() == "True")
  {
    Full = true;
  }
  else
  {
    Full = false;
  }
  
//Serial.print("Khoảng cách: ");
//Serial.print(distance_cm);
//Serial.println(" cm");
}
