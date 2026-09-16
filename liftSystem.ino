// ================= MOVE =================
void goToFloor(int target){

  if(target > currentFloor){

    // Moving UP
    for(int f = currentFloor + 1; f <= target; f++){

      showMovingTo(f);
      simulateStep();

      currentFloor = f;

      // Short unique tone for every reached floor
      playFloorTone(f);
    }
  }

  else if(target < currentFloor){

    // Moving DOWN
    for(int f = currentFloor - 1; f >= target; f--){

      showMovingTo(f);
      simulateStep();

      currentFloor = f;

      // Short unique tone for every reached floor
      playFloorTone(f);
    }
  }

  else{

    lcd.clear();
    lcd.print("Already at:");
    lcd.setCursor(0,1);
    lcd.print(floorNames[currentFloor]);

    // Long destination ringtone
    playDestinationRingtone(currentFloor);

    delay(1000);
    return;
  }

  arrive(currentFloor);
}