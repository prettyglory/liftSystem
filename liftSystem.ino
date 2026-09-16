#include <Servo.h>
#include <LiquidCrystal.h>

// ================= LCD =================
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

// ================= SERVO =================
Servo door;

// ================= BUTTONS =================
int btnPins[] = {2, 3, 4, 5, 6};

String floorNames[] = {
  "Basement",
  "Ground",
  "Floor1",
  "Floor2",
  "Floor3"
};

int emergencyBtn = 7;

// ================= ULTRASONIC =================
int trig = 8;
int echo = 9;

// ================= OTHER COMPONENTS =================
int servoPin = 10;
int buzzer = 11;
int led = 12;
int motor = 13;

// ================= QUEUE SYSTEM =================
int queue[10];
int qStart = 0;
int qEnd = 0;

bool emergencyActive = false;

// Current floor
// 0 = Basement
// 1 = Ground
// 2 = Floor1
// 3 = Floor2
// 4 = Floor3
int currentFloor = 1;


// ===================================================
// SETUP
// ===================================================

void setup() {

  lcd.begin(16, 2);

  door.attach(servoPin);

  for (int i = 0; i < 5; i++) {
    pinMode(btnPins[i], INPUT_PULLUP);
  }

  pinMode(emergencyBtn, INPUT_PULLUP);

  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  pinMode(buzzer, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(motor, OUTPUT);

  // Door initially closed
  door.write(0);

  // Motor initially OFF
  digitalWrite(motor, LOW);

  // LED initially OFF
  digitalWrite(led, LOW);

  // Display starting floor
  lcd.clear();
  lcd.print("Elevator At:");

  lcd.setCursor(0, 1);
  lcd.print(floorNames[currentFloor]);

  delay(2000);
}


// ===================================================
// MAIN LOOP
// ===================================================

void loop() {

  // Read floor buttons
  for (int i = 0; i < 5; i++) {

    if (digitalRead(btnPins[i]) == LOW) {

      addToQueue(i);

      // debounce
      delay(300);

      // Wait until button is released
      while (digitalRead(btnPins[i]) == LOW) {
        delay(10);
      }
    }
  }


  // Emergency button
  if (digitalRead(emergencyBtn) == LOW) {
    emergencyMode();
  }


  // Process queue
  if (qStart < qEnd && !emergencyActive) {

    int nextFloor = queue[qStart];

    qStart++;

    goToFloor(nextFloor);
  }


  // Reset queue after all requests are processed
  if (qStart == qEnd) {
    qStart = 0;
    qEnd = 0;
  }
}


// ===================================================
// ADD FLOOR TO QUEUE
// ===================================================

void addToQueue(int floor) {

  // Prevent queue overflow
  if (qEnd >= 10) {

    lcd.clear();
    lcd.print("Queue Full!");

    delay(1000);

    return;
  }

  queue[qEnd] = floor;

  qEnd++;

  lcd.clear();
  lcd.print("Added:");

  lcd.setCursor(0, 1);
  lcd.print(floorNames[floor]);

  delay(500);
}


// ===================================================
// MOVE TO FLOOR
// ===================================================

void goToFloor(int target) {

  // ================= MOVING UP =================

  if (target > currentFloor) {

    for (int f = currentFloor + 1; f <= target; f++) {

      showMovingTo(f);

      simulateStep();

      currentFloor = f;

      // Short unique tone whenever a floor is reached
      playFloorTone(f);
    }
  }


  // ================= MOVING DOWN =================

  else if (target < currentFloor) {

    for (int f = currentFloor - 1; f >= target; f--) {

      showMovingTo(f);

      simulateStep();

      currentFloor = f;

      // Short unique tone whenever a floor is reached
      playFloorTone(f);
    }
  }


  // ================= ALREADY AT FLOOR =================

  else {

    lcd.clear();
    lcd.print("Already At:");

    lcd.setCursor(0, 1);
    lcd.print(floorNames[currentFloor]);

    // Long destination ringtone
    playDestinationRingtone(currentFloor);

    delay(1000);

    return;
  }


  // Destination reached
  arrive(currentFloor);
}


// ===================================================
// SHOW MOVEMENT
// ===================================================

void showMovingTo(int floor) {

  lcd.clear();

  lcd.print("Moving to:");

  lcd.setCursor(0, 1);

  lcd.print(floorNames[floor]);
}


// ===================================================
// SIMULATE MOVING ONE FLOOR
// ===================================================

void simulateStep() {

  digitalWrite(motor, HIGH);

  // Simulate elevator movement
  delay(1500);

  digitalWrite(motor, LOW);

  delay(300);
}


// ===================================================
// SHORT UNIQUE FLOOR TONES
// ===================================================

void playFloorTone(int floor) {

  switch (floor) {

    // Basement
    case 0:
      tone(buzzer, 300, 150);
      delay(200);
      break;


    // Ground
    case 1:
      tone(buzzer, 450, 150);
      delay(200);
      break;


    // Floor 1
    case 2:
      tone(buzzer, 600, 150);
      delay(200);
      break;


    // Floor 2
    case 3:
      tone(buzzer, 800, 150);
      delay(200);
      break;


    // Floor 3
    case 4:
      tone(buzzer, 1000, 150);
      delay(200);
      break;
  }

  noTone(buzzer);
}


// ===================================================
// LONG DESTINATION RINGTONES
// ===================================================

void playDestinationRingtone(int floor) {

  switch (floor) {

    // =================================================
    // BASEMENT
    // Low descending ringtone
    // =================================================

    case 0:

      tone(buzzer, 350, 400);
      delay(500);

      tone(buzzer, 300, 400);
      delay(500);

      tone(buzzer, 250, 800);
      delay(900);

      break;


    // =================================================
    // GROUND FLOOR
    // Bell-style ringtone
    // =================================================

    case 1:

      tone(buzzer, 500, 350);
      delay(450);

      tone(buzzer, 700, 350);
      delay(450);

      tone(buzzer, 500, 350);
      delay(450);

      tone(buzzer, 800, 800);
      delay(900);

      break;


    // =================================================
    // FLOOR 1
    // Rising ringtone
    // =================================================

    case 2:

      tone(buzzer, 500, 250);
      delay(300);

      tone(buzzer, 650, 250);
      delay(300);

      tone(buzzer, 800, 250);
      delay(300);

      tone(buzzer, 1000, 900);
      delay(1000);

      break;


    // =================================================
    // FLOOR 2
    // High-low ringtone
    // =================================================

    case 3:

      tone(buzzer, 900, 300);
      delay(350);

      tone(buzzer, 700, 300);
      delay(350);

      tone(buzzer, 1000, 300);
      delay(350);

      tone(buzzer, 800, 300);
      delay(350);

      tone(buzzer, 1100, 900);
      delay(1000);

      break;


    // =================================================
    // FLOOR 3
    // Long celebration ringtone
    // =================================================

    case 4:

      tone(buzzer, 700, 200);
      delay(250);

      tone(buzzer, 850, 200);
      delay(250);

      tone(buzzer, 1000, 200);
      delay(250);

      tone(buzzer, 1150, 300);
      delay(350);

      tone(buzzer, 1300, 1200);
      delay(1300);

      break;
  }

  noTone(buzzer);
}


// ===================================================
// ARRIVAL
// ===================================================

void arrive(int floor) {

  lcd.clear();

  lcd.print("Arrived:");

  lcd.setCursor(0, 1);

  lcd.print(floorNames[floor]);

  // Destination ringtone is longer than normal floor tone
  playDestinationRingtone(floor);

  openDoor();
}


// ===================================================
// SMART DOOR
// ===================================================

void openDoor() {

  lcd.clear();

  lcd.print("Door Opening");

  // Open door
  door.write(90);

  delay(2000);


  // Check if person/object is blocking door
  while (getDistance() < 20) {

    lcd.clear();

    lcd.print("Door Blocked!");

    lcd.setCursor(0, 1);

    lcd.print("Please Wait");

    delay(500);
  }


  // Close door
  door.write(0);

  lcd.clear();

  lcd.print("Door Closed");

  lcd.setCursor(0, 1);

  lcd.print(floorNames[currentFloor]);

  delay(1000);
}


// ===================================================
// ULTRASONIC DISTANCE
// ===================================================

long getDistance() {

  digitalWrite(trig, LOW);

  delayMicroseconds(2);

  digitalWrite(trig, HIGH);

  delayMicroseconds(10);

  digitalWrite(trig, LOW);


  long duration = pulseIn(echo, HIGH, 30000);


  // No echo received
  if (duration == 0) {
    return 999;
  }


  long distance = duration * 0.034 / 2;

  return distance;
}


// ===================================================
// EMERGENCY MODE
// ===================================================

void emergencyMode() {

  emergencyActive = true;

  // Stop elevator motor
  digitalWrite(motor, LOW);

  // Close door
  door.write(0);

  // Clear queue
  qStart = 0;
  qEnd = 0;

  lcd.clear();

  lcd.print("!!! EMERGENCY !!!");


  // Continuous emergency alarm
  while (true) {

    tone(buzzer, 1200);

    digitalWrite(led, HIGH);

    delay(200);


    tone(buzzer, 800);

    digitalWrite(led, LOW);

    delay(200);
  }
}