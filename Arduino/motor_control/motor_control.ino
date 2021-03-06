#define X_STEP_PIN         54
#define X_DIR_PIN          55
#define X_ENABLE_PIN       38
#define X_MIN_PIN           3
#define X_MAX_PIN           2

#define Y_STEP_PIN         60
#define Y_DIR_PIN          61
#define Y_ENABLE_PIN       56
#define Y_MIN_PIN          14
#define Y_MAX_PIN          15

#define Z_STEP_PIN         46
#define Z_DIR_PIN          48
#define Z_ENABLE_PIN       62
#define Z_MIN_PIN          18
#define Z_MAX_PIN          19

#define E_STEP_PIN         26
#define E_DIR_PIN          28
#define E_ENABLE_PIN       24

#define Q_STEP_PIN         36
#define Q_DIR_PIN          34
#define Q_ENABLE_PIN       30

#define SDPOWER            -1
#define SDSS               53
#define LED_PIN            13

#define FAN_PIN            9

#define PS_ON_PIN          12
#define KILL_PIN           -1

#define HEATER_0_PIN       10
#define HEATER_1_PIN       8
#define TEMP_0_PIN          13   // ANALOG NUMBERING
#define TEMP_1_PIN          14   // ANALOG NUMBERING

int reverse_delay = 200; // dictates how fast your motor moves
int reverse_ticks = 1000; // dictates the angular distance you move
int forward_delay = 100;
int forward_ticks = 2000;

//ground the other wire of the limit switch, and one to the signal input
void setup() {
  //setup only runs once when the Arduino starts up
  Serial.begin(9600);
  pinMode(X_MIN_PIN, INPUT_PULLUP);
  pinMode(X_MAX_PIN, INPUT_PULLUP);
  pinMode(Y_MIN_PIN, INPUT_PULLUP);
  pinMode(Y_MAX_PIN, INPUT_PULLUP);
  pinMode(Z_MIN_PIN, INPUT_PULLUP);
  pinMode(Z_MAX_PIN, INPUT_PULLUP);
  pinMode(FAN_PIN , OUTPUT);
  pinMode(HEATER_0_PIN , OUTPUT);
  pinMode(HEATER_1_PIN , OUTPUT);
  pinMode(LED_PIN  , OUTPUT);
  
  pinMode(X_STEP_PIN  , OUTPUT);
  pinMode(X_DIR_PIN    , OUTPUT);
  pinMode(X_ENABLE_PIN    , OUTPUT);
  
  pinMode(Y_STEP_PIN  , OUTPUT);
  pinMode(Y_DIR_PIN    , OUTPUT);
  pinMode(Y_ENABLE_PIN    , OUTPUT);
  
  pinMode(Z_STEP_PIN  , OUTPUT);
  pinMode(Z_DIR_PIN    , OUTPUT);
  pinMode(Z_ENABLE_PIN    , OUTPUT);
  
  pinMode(E_STEP_PIN  , OUTPUT);
  pinMode(E_DIR_PIN    , OUTPUT);
  pinMode(E_ENABLE_PIN    , OUTPUT);
  
  pinMode(Q_STEP_PIN  , OUTPUT);
  pinMode(Q_DIR_PIN    , OUTPUT);
  pinMode(Q_ENABLE_PIN    , OUTPUT);
  
  digitalWrite(X_ENABLE_PIN    , LOW); //when set to HIGH, motor is disabled
  digitalWrite(Y_ENABLE_PIN    , LOW);
  digitalWrite(Z_ENABLE_PIN    , LOW);
  digitalWrite(E_ENABLE_PIN    , LOW);
  digitalWrite(Q_ENABLE_PIN    , LOW);

  int pin_curr_dir = X_DIR_PIN;
  int pin_curr_limit = X_MAX_PIN;   // start forward
  int pin_next_limit = X_MIN_PIN;
  int pin_curr_step = X_STEP_PIN;
}

// Currently just moves from one end to the other continuously
void loop() {
  int curr_dir = digitalRead(pin_curr_dir);
  if (limit_hit(pin_curr_limit)) {
    // switch limit pins
    int hold = pin_curr_limit;
    int pin_curr_limit = pin_next_limit;
    int pin_next_limit = hold;
    if (!curr_dir) {
      move_forward(pin_curr_dir, pin_curr_step);
    } else {
      move_backward(pin_curr_dir, pin_curr_step);
    }
  } else {
    if (curr_dir) {
      move_forward(pin_curr_dir, pin_curr_step);
    } else {
      move_backward(pin_curr_dir, pin_curr_step);
    }
  }
}

void print_signal(int pin) {
  int signal = digitalRead(pin);
  Serial.println(signal);
}

// forward direction, set dir as HIGH
void move_forward(int dir_pin, int step_pin) {
  digitalWrite(dir_pin, HIGH); // Enables the motor to move in forward direction
  for (int x = 0; x < forward_ticks; x++) {
    digitalWrite(step_pin, HIGH); 
    delayMicroseconds(forward_delay);//this delay dictates how fast it goes
  }
}

// backward direction, set dir as LOW
void move_backward(int dir_pin, int step_pin) {
  digitalWrite(dir_pin, LOW); // Disables forward direction, moves backward
  for (int x = 0; x < reverse_ticks; x++) {
    digitalWrite(step_pin, HIGH); 
    delayMicroseconds(reverse_delay);
  }
}

// make sure pin matches the limit pin when this function is used
// forward direction is toward maximum
// backward direction is toward minimum
int limit_hit(int limit_pin) {
  return digitalRead(limit_pin);
}

void enable(int enable_pin) {
  digitalWrite(enable_pin, LOW);
}

void disable(int enable_pin) {
  digitalWrite(enable_pin, HIGH);
}

int measure_distance(dir_pin, pin_curr_limit, pin_next_limit, step_pin) {
  digitalWrite(dir_pin, HIGH); // Enables the motor to move in forward direction
  while (!limit_hit(limit_pin)) {
    digitalWrite(step_pin, HIGH);
  }

  // switch limit pins
  int hold = pin_curr_limit;
  int pin_curr_limit = pin_next_limit;
  int pin_next_limit = hold;

  int count = 0;
  while (!limit_hit(limit_pin)) {
    digitalWrite(step_pin, HIGH);
    count++;
  }

  return count;
}
