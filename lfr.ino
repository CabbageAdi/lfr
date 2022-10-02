#define DEBUG true

//sensor
#include <QTRSensors.h>
#define sensor_count 8
#define color_threshold 500
const uint8_t sensor_pins[sensor_count] = {5, 6, 7, 8, 9, 10, 11, 12};
uint8_t sensor[sensor_count]; //set this to values

// motor
// pins
#define IN1 1
#define IN2 2
#define IN3 3
#define IN4 4
// variables
#define max_speed 200 //add more speed variables if necessary
#define speed_ratio 1 //different speed for motors if imbalanced

//mapping stuff
#define direction_priority true // true = left, false = right


//magic for convenience
#define NUMARGS(...)  (sizeof((int[]){__VA_ARGS__})/sizeof(int))
#define check(...)  (Check(NUMARGS(__VA_ARGS__), __VA_ARGS__))

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  //fancy preprocessor directives
  #if DEBUG
  Serial.begin(9600);
  #endif
}

void loop() {
  sense();

  //straight line
  if (true) { //middle pins
    raw_forward(max_speed);
  }
  if (true) { //middle no left pin
    raw_left(max_speed);
  }
  if (true) { //middle no right pin
    raw_right(max_speed);
  }

  //fork
  //no mapping right now

  //both options
  if (true) { //all pins + some tolerance idk
    bool forward_option = check_fork();
    if (direction_priority) {
      left_till_line();
    }
    else {
      right_till_line();
    }
  }

  //left only
  if (true) { //middle + out left pins
    bool forward_option = check_fork();
    if (direction_priority && !forward_option) {
      left_till_line();
    }
  }
  //right only
  if (true) { //middle + out right pins
    bool forward_option = check_fork();
    if (!direction_priority && !forward_option) {
      right_till_line();
    }
  }
}

void left_till_line() {

}

void right_till_line() {

}

//run when turn
bool check_fork() {
  raw_forward(max_speed);
  delay(100) //tweak this
  if (true) { //middle sensor therefore straight is option
    return true;
  }
  else {
    return false;
  }
}

void Check(int numargs, ...) {
  va_list ap;

  va_start(ap, numargs);
  while (numargs--){
    i = va_arg(ap, int);
    if (sensor[i] == 0) {
      return false;
    }
  }
  va_end(ap);

  return true;
}

//raw movement functions
void raw_forward(int speed) {
  analogWrite(IN1, speed);
  analogWrite(IN2, speed);
  analogWrite(IN3, speed);
  analogWrite(IN4, speed);
}
void raw_right(int speed) {
  analogWrite(IN1, speed);
  analogWrite(IN2, speed);
  analogWrite(IN3, speed);
  analogWrite(IN4, speed);
}
void raw_left(int speed) {
  analogWrite(IN1, speed);
  analogWrite(IN2, speed);
  analogWrite(IN3, speed);
  analogWrite(IN4, speed);
}

void sense() {
  // qtr.read(sensor)
  for (int i = 0; i < sensor_count; i++) {
    uint8_t val = sensor[i];
    //easier to read
    if (val >= color_threshold) {
      sensor[i] = 1;
    }
    else {
      sensor[i] = 0;
    }
  }

  #if DEBUG
  for (int i = 0; i < sensor_count; i++) {
    Serial.print(sensor[i]);
    //Serial.print('\t');
  }
  Serial.println();
  #endif
}