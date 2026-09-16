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

// ================= QUEUE =================
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

int currentFloor = 1;   // Start at Ground Floor


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

  // Start with door closed
  door.write(0);

  // Show starting floor
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

  // Check floor buttons
  for (int i = 0; i < 5; i++) {

    if (digitalRead(btnPins[i]) == LOW) {

      addToQueue(i);

      // Button debounce
      delay(300);
    }
  }


  // Check emergency button
  if (digitalRead(emergencyBtn) == LOW) {
    emergencyMode();
  }


  // Process queue
  if (qStart < qEnd && !emergencyActive) {

    int nextFloor = queue[qStart];

    qStart++;

    goToFloor(nextFloor);
  }


  // Reset queue indexes when all requests are completed
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
// MOVE ELEVATOR
// ===================================================

void goToFloor(int target) {

  // ================= MOVING UP =================

  if (target > currentFloor) {

    for (int f = currentFloor + 1; f <= target; f++) {

      showMovingTo(f);

      simulateStep();

      currentFloor = f;
    }
  }


  // ================= MOVING DOWN =================

  else if (target < currentFloor) {

    for (int f = currentFloor - 1; f >= target; f--) {

      showMovingTo(f);

      simulateStep();

      currentFloor = f;
    }
  }


  // ================= SAME FLOOR =================

  else {

    lcd.clear();

    lcd.print("Already At:");

    lcd.setCursor(0, 1);

    lcd.print(floorNames[currentFloor]);

    playFloorRingtone(currentFloor);

    delay(1000);

    return;
  }


  // Destination reached
  arrive(currentFloor);
}


// ===================================================
// DISPLAY MOVEMENT
// ===================================================

void showMovingTo(int floor) {

  lcd.clear();

  lcd.print("Moving to:");

  lcd.setCursor(0, 1);

  lcd.print(floorNames[floor]);
}


// ===================================================
// SIMULATE ONE FLOOR MOVEMENT
// ===================================================

void simulateStep() {

  digitalWrite(motor, HIGH);

  // Simulate elevator travel
  delay(1500);

  digitalWrite(motor, LOW);

  delay(300);
}


// ===================================================
// ARRIVAL
// ===================================================

void arrive(int floor) {

  lcd.clear();

  lcd.print("Arrived:");

  lcd.setCursor(0, 1);

  lcd.print(floorNames[floor]);


  // Play unique ringtone for destination
  playFloorRingtone(floor);


  // Open elevator door
  openDoor();
}


// ===================================================
// DIFFERENT RINGTONE FOR EACH FLOOR
// ===================================================

void playFloorRingtone(int floor) {

  switch (floor) {


    // ================= BASEMENT =================

    case 0:

      // Low tone
      tone(buzzer, 300, 500);
      delay(600);

      tone(buzzer, 250, 500);
      delay(600);

      break;


    // ================= GROUND FLOOR =================

    case 1:

      // Two short beeps
      tone(buzzer, 500, 200);
      delay(300);

      tone(buzzer, 500, 200);
      delay(300);

      break;


    // ================= FLOOR 1 =================

    case 2:

      // Rising melody
      tone(buzzer, 500, 200);
      delay(250);

      tone(buzzer, 700, 200);
      delay(250);

      tone(buzzer, 900, 350);
      delay(400);

      break;


    // ================= FLOOR 2 =================

    case 3:

      // Different three-note melody
      tone(buzzer, 784, 200);
      delay(250);

      tone(buzzer, 988, 200);
      delay(250);

      tone(buzzer, 1175, 400);
      delay(450);

      break;


    // ================= FLOOR 3 =================

    case 4:

      // Highest floor celebration melody
      tone(buzzer, 700, 150);
      delay(200);

      tone(buzzer, 900, 150);
      delay(200);

      tone(buzzer, 1100, 150);
      delay(200);

      tone(buzzer, 1300, 400);
      delay(450);

      break;
  }

  noTone(buzzer);
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


  // Check if someone/object is near the door
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
// ULTRASONIC SENSOR
// ===================================================

long getDistance() {

  digitalWrite(trig, LOW);

  delayMicroseconds(2);

  digitalWrite(trig, HIGH);

  delayMicroseconds(10);

  digitalWrite(trig, LOW);


  long duration = pulseIn(echo, HIGH, 30000);


  // If ultrasonic receives no echo
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


  // Stop motor immediately
  digitalWrite(motor, LOW);


  // Close door
  door.write(0);


  // Clear queue
  qStart = 0;
  qEnd = 0;


  lcd.clear();

  lcd.print("!!! EMERGENCY !!!");


  while (true) {

    // First emergency tone
    tone(buzzer, 1200);

    digitalWrite(led, HIGH);

    delay(200);


    // Second emergency tone
    tone(buzzer, 800);

    digitalWrite(led, LOW);

    delay(200);
  }
}