#include <vexMotor.h>
#include <Servo.h>

#define beta 3950

//INPUTS
int forceSensorPin = A1; // 12 kOhm to GND an
int thermistorPin = 11; // no res
int doorLightSensorPin = A0;  // directly
//int floorStateSensorPin = A0;
int floorButtonPins[] = {5, 6, 7};
int emergencyPin = 2;

//OUTPUTS
int alarmLEDPin = 12;
//int sevenSegmentPins[] = {};
vexMotor myVexMotor;
vexMotor myServoMotor;

//CONSTANTS
const int thirdFloorValue = 800;
const int secondFloorValue = 600;
const float roomTemperature = 31.0;
const int oneFloorDistance = 1500;
const int doorOpeningTime = 1000;
const int openDoorDistance = 400;
const int maxWeight = 600;



//STATES
int currentFloor = 0;
int motorSpeed = -110;
int motorSpeed2 = 66;
float tempC = 0.0;
boolean currentDirection = false;
boolean isMoving = false;
boolean isClosed = true;
boolean isAlarm = false;
boolean isRedLEDOn = false;
boolean selectedFloors[] = {false, false, false};
int selectedFloorsNum = 0;
int nextFloor = 1;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(forceSensorPin, INPUT);
  pinMode(thermistorPin, INPUT);
  pinMode(doorLightSensorPin, INPUT);
//  pinMode(floorStateSensorPin, INPUT);
  pinMode(floorButtonPins[0], INPUT);
  pinMode(floorButtonPins[1], INPUT);
  pinMode(floorButtonPins[2], INPUT);
  pinMode(alarmLEDPin, OUTPUT);
//  for (int i = 0; i < 7; i++) {
//    pinMode(sevenSegmentPins[i], OUTPUT);
//  }
  myVexMotor.attach(9);
  myServoMotor.attach(3);
}


//int checkFloor() {
//  if (isMoving == false) {
//    if (analogRead(floorStateSensorPin) < thirdFloorValue) {
//      currentFloor = 2;
//    } else if (analogRead(floorStateSensorPin) < secondFloorValue) {
//      currentFloor = 1;
//    } else
//      currentFloor = 0;
//    return currentFloor;
//  } else {
//    delay(1000);
//    return checkFloor();
//  }
//}

void addFloor(int floorNum) {
  if (selectedFloors[floorNum] == false) {
    selectedFloors[floorNum] = true;
    Serial.print("Floor is added: ");
    Serial.println(floorNum);
    } else {
    Serial.println("Floor is already selected");
  }
}

void readExteriorButtons() {
 // currentFloor = checkFloor();
  for (int i = 0; i < 3; i++) {
    if(digitalRead(floorButtonPins[i]) == HIGH) {
      if(i != currentFloor) {
        addFloor(i);
      } else {
        if(!isMoving) {
          openDoors();
        }
      }
    }
  }
}

void openDoors() {
  if (isClosed) {
    myServoMotor.write(-200);
    Serial.println("Door is open");  
    isClosed = false;
    Serial.print("Total weight: ");
    Serial.println(digitalRead(forceSensorPin));
    delay(4000);
    checkOverweight();
    myServoMotor.write(100);
    checkDoors();
    myServoMotor.write(200);
    Serial.println("Door is closed");
    isClosed = true;
  } else {
    delay(2000);
  }
}

void checkDoors() {
  if (analogRead(doorLightSensorPin) < openDoorDistance) {
    Serial.print("AHTUNG! Object on the way of doors! ");
    Serial.println(analogRead(doorLightSensorPin));
    isClosed = true;
    openDoors();
  }
}

void checkOverweight() {
  if (analogRead(forceSensorPin) > maxWeight) {
    Serial.println("AHTUNG! Overweight!");
    weightLED();
    delay(8000);
    weightLED();
    checkOverweight();
  } 
}

void weightLED() {
  if (!isRedLEDOn) {
    digitalWrite(alarmLEDPin, HIGH);
    isRedLEDOn = true;
  } else {
    digitalWrite(alarmLEDPin, LOW);
    isRedLEDOn = false;
  }
}

void alarm() {
  digitalWrite(alarmLEDPin, HIGH);
  selectedFloors[0] = true;
  selectedFloors[1] = false;
  selectedFloors[2] = false;
  startElevator();
  isAlarm = true;
  delay(10000);
  digitalWrite(alarmLEDPin, LOW);
  isAlarm = false;
  selectedFloors[0] = false;
}

float getTemp() {
  long a =1023 - analogRead(thermistorPin); //read thermistor value 
  float tempC = beta /(log((1025.0 * 10 / a - 10) / 10) + beta / 298.0) - 273.0;
  Serial.print("Temperature is ");
  Serial.println(tempC);
  return tempC;
}

//void setSevenSegment(int floorNum) {
//  if(floorNum = 0) {
//    digitalWrite(sevenSegmentPins[0], LOW);
//    digitalWrite(sevenSegmentPins[1], HIGH);
//    digitalWrite(sevenSegmentPins[2], HIGH);
//    digitalWrite(sevenSegmentPins[3], LOW);
//    digitalWrite(sevenSegmentPins[4], LOW);
//    digitalWrite(sevenSegmentPins[5], LOW);
//    digitalWrite(sevenSegmentPins[6], LOW);
//  } else if(floorNum = 1) {
//    digitalWrite(sevenSegmentPins[0], HIGH);
//    digitalWrite(sevenSegmentPins[1], HIGH);
//    digitalWrite(sevenSegmentPins[2], LOW);
//    digitalWrite(sevenSegmentPins[3], HIGH);
//    digitalWrite(sevenSegmentPins[4], HIGH);
//    digitalWrite(sevenSegmentPins[5], LOW);
//    digitalWrite(sevenSegmentPins[6], HIGH);
//  } else {
//    digitalWrite(sevenSegmentPins[0], HIGH);
//    digitalWrite(sevenSegmentPins[1], HIGH);
//    digitalWrite(sevenSegmentPins[2], HIGH);
//    digitalWrite(sevenSegmentPins[3], HIGH);
//    digitalWrite(sevenSegmentPins[4], LOW);
//    digitalWrite(sevenSegmentPins[5], LOW);
//    digitalWrite(sevenSegmentPins[6], HIGH);
//  }
//}

void startElevator() {
  if(currentFloor == 2) {
    currentDirection = false;
    Serial.println("Current floor is 2");
    if(selectedFloors[1] == true) {
      Serial.println("Moving to floor 1");
      Serial.print("Setting Motor Speed to: ");
      Serial.println(motorSpeed2);
      myVexMotor.write(motorSpeed2);
      Serial.println("Motor is working");
      delay(oneFloorDistance);
      myVexMotor.write(0);
      Serial.println("Motor stopped");  
      Serial.println();
      selectedFloors[1] = false;
      currentFloor = 1;
//      setSevenSegment(currentFloor);
      openDoors();
    } else if (selectedFloors[1] == false && selectedFloors[0] == true) {
      Serial.println("Moving to floor 0");
      Serial.print("Setting Motor Speed to: ");
      Serial.println(motorSpeed2);
      myVexMotor.write(motorSpeed2);
      Serial.println("Motor is working");
      delay(2*oneFloorDistance);
      myVexMotor.write(0);
      Serial.println("Motor stopped");  
      Serial.println();
      selectedFloors[0] = false;
      currentFloor = 0;
//      setSevenSegment(currentFloor);
      openDoors();
    }
  } else if(currentFloor == 1) {
      Serial.println("Current floor is 1");
      if(currentDirection == false && selectedFloors[0] == true) {
        Serial.println("Moving to floor 0");
        Serial.print("Setting Motor Speed to: ");
        Serial.println(motorSpeed2);
        myVexMotor.write(motorSpeed2);
        Serial.println("Motor is working");
        delay(oneFloorDistance);
        myVexMotor.write(0);
        Serial.println("Motor stopped");  
        Serial.println();
        selectedFloors[0] = false;
        currentFloor = 0;
//        setSevenSegment(currentFloor);
        openDoors();
    } else if(currentDirection == false && selectedFloors[2] == true) {
      currentDirection = true;
    } else if(currentDirection == true && selectedFloors[2] == true) {
      Serial.println("Moving to floor 2");
      Serial.print("Setting Motor Speed to: ");
      Serial.println(motorSpeed);
      myVexMotor.write(motorSpeed);
      Serial.println("Motor is working");
      delay(oneFloorDistance);
      myVexMotor.write(0);
      Serial.println("Motor stopped");  
      Serial.println();
      selectedFloors[2] = false;
      currentFloor = 2;
//      setSevenSegment(currentFloor);
      openDoors();
    } else if(currentDirection == true && selectedFloors[0] == true) {
      currentDirection = false;
    }    
  } else if(currentFloor == 0) {
    currentDirection = true;
    Serial.println("Current floor is 0");
    if(selectedFloors[1] == true) {
      Serial.println("Moving to floor 1");
      Serial.print("Setting Motor Speed to: ");
      Serial.println(motorSpeed);
      myVexMotor.write(motorSpeed);
      Serial.println("Motor is working");
      delay(oneFloorDistance);
      myVexMotor.write(0);
      Serial.println("Motor stopped");  
      Serial.println();
      selectedFloors[1] = false;
      currentFloor = 1;
//      setSevenSegment(currentFloor);
      openDoors();
    } else if (selectedFloors[1] == false && selectedFloors[2] == true) {
      Serial.println("Moving to floor 2");
      Serial.print("Setting Motor Speed to: ");
      Serial.println(motorSpeed);
      myVexMotor.write(motorSpeed);
      Serial.println("Motor is working");
      delay(2*oneFloorDistance);
      myVexMotor.write(0);
      Serial.println("Motor stopped");  
      Serial.println();
      selectedFloors[2] = false;
      currentFloor = 2;
//      setSevenSegment(currentFloor);
      openDoors();
  }
}
}

void loop() {
  Serial.print(digitalRead(floorButtonPins[0]));
  Serial.print(digitalRead(floorButtonPins[1]));
  Serial.println(digitalRead(floorButtonPins[2]));
  Serial.print(selectedFloors[0]);
  Serial.print(selectedFloors[1]);
  Serial.println(selectedFloors[2]);
  readExteriorButtons();
  Serial.println("Buttons are read");
  if(getTemp() > roomTemperature || digitalRead(emergencyPin) == HIGH ) {
    if (digitalRead(emergencyPin) == HIGH) {
      Serial.println("Emergency button is pushed!");
    }
    alarm();
  }
  if (!isAlarm) {
    if (!isMoving) {
      startElevator();
      Serial.println("Elevator started");
      Serial.println(analogRead(doorLightSensorPin));
      
    }
  }
}
