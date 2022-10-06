#define DEBUG true

//sensor
#define sensor_count 4
#define color_threshold 500
const uint8_t sensor_pins[sensor_count] = {8, 9, 10, 11};
uint8_t sensor[sensor_count]; //set to actual values

// motor pins
#define IN1 300
#define IN2 400
#define IN3 500
#define IN4 600

//mapping stuff
#define direction_priority true // true = left, false = right
int rotation = 0; //0 - forward, 1 - left, 2 - back, 3 - right
int path[10000]; //same key as rotation
int fork = -1; //index of fork last ventured
bool dry_run = true;

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
  if (check(1, 2)) { //going straight
    raw_forward();
  }
  if (check(1) && !check(2)) { //drifting right
    raw_left();
  }
  if (check(2) && !check(1)) { //drifting left
    raw_right();
  }

  //fork

  //all options
  if (check(0, 1, 2, 3)) {
    fork++;
    bool forward_option = check_fork();
    if (direction_priority) {
      left_till_line();
      rotation++;
    }
    else {
      right_till_line();
      rotation--;
    }
    path[fork] = rotation % 4;
  }

  //left only
  else if (check(0, 1)) {
    bool forward_option = check_fork();
    if (forward_option) {
      fork++;
    }
    if (direction_priority || !forward_option) {
      left_till_line();
      rotation++;
    }
    path[fork] = rotation % 4;
  }

  //right only
  else if (check(2, 3)) { //middle + out right pins
    bool forward_option = check_fork();
    if (forward_option) {
      fork++;
    }
    if (!direction_priority || !forward_option) {
      right_till_line();
      rotation--;
    }
    path[fork] = rotation % 4;
  }

  //dead end
  if (!check(1, 2, 3, 4)) {
    left_till_line();
    rotation += 2;
    fork--;
  }
}

void left_till_line() {
  while (!check(1, 2)) {
    raw_left();
  }
}

void right_till_line() {
  while (!check(1, 2)) {
    raw_right();
  }
}

//run when turn
bool check_fork() {
  raw_forward();
  delay(100); //tweak this
  if (true) { //middle sensor therefore straight is option
    return true;
  }
  else {
    return false;
  }
}

//:sparkles: magic
bool Check(int numargs, ...) {
  va_list ap;

  va_start(ap, numargs);
  while (numargs--) {
    int i = va_arg(ap, int);
    if (sensor[i] == 0) {
      return false;
    }
  }
  va_end(ap);

  return true;
}

//raw movement functions
void raw_forward() {
  analogWrite(IN1, HIGH);
  analogWrite(IN2, HIGH);
  analogWrite(IN3, HIGH);
  analogWrite(IN4, HIGH);
}
void raw_right() {
  analogWrite(IN1, HIGH);
  analogWrite(IN2, HIGH);
  analogWrite(IN3, HIGH);
  analogWrite(IN4, HIGH);
}
void raw_left() {
  analogWrite(IN1, HIGH);
  analogWrite(IN2, HIGH);
  analogWrite(IN3, HIGH);
  analogWrite(IN4, HIGH);
}

void sense() {
  for (int i = 0; i < sensor_count; i++) {
    int val = analogRead(sensor_pins[i]);
    sensor[i] = val;
  }

  // for (int i = 0; i < sensor_count; i++) {
  //   uint8_t val = sensor[i];
  //   //easier to read
  //   if (val >= color_threshold) {
  //     sensor[i] = 1;
  //   }
  //   else {
  //     sensor[i] = 0;
  //   }
  // }

  #if DEBUG
  for (int i = 0; i < sensor_count; i++) {
    Serial.print(sensor[i]);
  }
  Serial.println();
  #endif
}