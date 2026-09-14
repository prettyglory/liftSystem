#include <Servo.h>
#include <LiquidCrystal.h>

// LCD
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

// Servo
Servo door;

// Buttons
int btnPins[] = {2,3,4,5,6};
String floorNames[] = {"Basement","Ground","Floor1","Floor2","Floor3"};
int emergencyBtn = 7;

// Ultrasonic
int trig = 8;
int echo = 9;

// Other components
int servoPin = 10;
int buzzer = 11;
int led = 12;
int motor = 13;

// Queue system
int queue[10];
int qStart = 0;
int qEnd = 0;

bool emergencyActive = false;

// Current floor (0=Basement, 1=Ground, 2=Floor1, 3=Floor2, 4=Floor3)
int currentFloor = 1; // Default: Ground

// Tone frequencies for each floor (you can adjust)
int floorTones[] = {600, 700, 800, 900, 1000}; 
// Basement, Ground, Floor1, Floor2, Floor3

// ================= SETUP =================
void setup(){
  lcd.begin(16,2);
  door.attach(servoPin);

  for(int i=0;i<5;i++){
    pinMode(btnPins[i], INPUT_PULLUP);
  }

  pinMode(emergencyBtn, INPUT_PULLUP);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(motor, OUTPUT);

  // Show default floor at start
  lcd.print("At:");
  lcd.setCursor(0,1);
  lcd.print(floorNames[currentFloor]);
  delay(2000);
  lcd.clear();
}

// ================= LOOP =================
void loop(){

  // read buttons → add to queue
  for(int i=0;i<5;i++){
    if(digitalRead(btnPins[i]) == LOW){
      addToQueue(i);
      delay(300); // debounce
    }
  }

  // emergency check
  if(digitalRead(emergencyBtn) == LOW){
    emergencyMode();
  }

  // process queue
  if(qStart < qEnd && !emergencyActive){
    int nextFloor = queue[qStart];
    qStart++;
    goToFloor(nextFloor);
  }
}

// ================= QUEUE =================
void addToQueue(int floor){

  lcd.clear();
  lcd.print("Added:");
  lcd.setCursor(0,1);
  lcd.print(floorNames[floor]);

  queue[qEnd] = floor;
  qEnd++;
}

// ================= MOVE =================
void goToFloor(int target){

  // Decide direction and move floor-by-floor
  if(target > currentFloor){
    // Moving up
    for(int f = currentFloor + 1; f <= target; f++){
      showMovingTo(f);
      simulateStep(f); // pass floor index to play tone
      currentFloor = f;
    }
  }
  else if(target < currentFloor){
    // Moving down
    for(int f = currentFloor - 1; f >= target; f--){
      showMovingTo(f);
      simulateStep(f); // pass floor index to play tone
      currentFloor = f;
    }
  }
  else{
    // Already on the requested floor
    lcd.clear();
    lcd.print("Already at:");
    lcd.setCursor(0,1);
    lcd.print(floorNames[currentFloor]);
    delay(1500);
    lcd.clear();
    return;
  }

  arrive(currentFloor);
}

// Show "Moving to: <floor>" for each step
void showMovingTo(int floor){
  lcd.clear();
  lcd.print("Moving to:");
  lcd.setCursor(0,1);
  lcd.print(floorNames[floor]);
}

// Simulate moving one floor and play tone for that floor
void simulateStep(int floor){
  digitalWrite(motor, HIGH);
  delay(1500); // time to move one floor
  digitalWrite(motor, LOW);

  // Play floor-specific tone when passing/arriving at this floor
  tone(buzzer, floorTones[floor]);
  delay(300);
  noTone(buzzer);

  delay(500); // small pause between floors
}

// ================= ARRIVE =================
void arrive(int floor){

  lcd.clear();
  lcd.print("Arrived:");
  lcd.setCursor(0,1);
  lcd.print(floorNames[floor]);

  beep(); // final arrival beep (1kHz)
  openDoor();
}

// ================= DOOR SMART =================
void openDoor(){

  lcd.clear();
  lcd.print("Door Opening");

  door.write(90);
  delay(2000);

  // safety check
  while(getDistance() < 20){
    lcd.setCursor(0,1);
    lcd.print("Person detected");
    delay(500);
  }

  door.write(0);
  lcd.clear();
  lcd.print("Door Closed");
}

// ================= ULTRASONIC =================
long getDistance(){

  digitalWrite(trig, LOW);
  delayMicroseconds(2);

  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  long duration = pulseIn(echo, HIGH);
  return duration * 0.034 / 2;
}

// ================= BEEP =================
void beep(){
  tone(buzzer,1000);
  delay(200);
  noTone(buzzer);
}

// ================= EMERGENCY =================
void emergencyMode(){

  emergencyActive = true;

  lcd.clear();
  lcd.print("!!! EMERGENCY !!!");

  digitalWrite(motor, LOW);
  door.write(0);

  // clear queue
  qStart = 0;
  qEnd = 0;

  while(true){

    tone(buzzer,1200);
    digitalWrite(led,HIGH);
    delay(200);

    tone(buzzer,800);
    digitalWrite(led,LOW);
    delay(200);
  }
}