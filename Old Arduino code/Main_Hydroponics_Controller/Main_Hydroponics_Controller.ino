
//*** Main Hydropoincs Controller ***

#include <Wire.h>
#include <TimeLib.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

int overrideSwitch = 22;
int dirtLight = 33;
int hydroLight = 31;
int testLight1 = 4;
int testLight2 = 5;
int timeH, timeM, timeS, time12, currentTime;
int counter = 0;
int consequitive = 0;
int joyPin1 = 0;
int joyPin2 = 1;
int xJoystick = 0;
int yJoystick = 0;
int clockMinute = 60;
int joyHigh = 800;
int joyLow = 200;
int triggerDistance = 15;
int trigPin = 40;    //Trig - green Jumper
int echoPin = 41;    //Echo - yellow Jumper
float duration, cm, inches;

void setup() {

  Serial.begin(9600);

  //Display setup
  lcd.begin(16, 2);  // initialize the lcd for 16 chars 2 lines, turn on backlight
  lcd.backlight(); // finish with backlight on

  //***Set pinmodes
  pinMode(dirtLight, OUTPUT);
  pinMode(hydroLight, OUTPUT);
  pinMode(testLight1, OUTPUT);
  pinMode(testLight2, OUTPUT);
  pinMode(overrideSwitch, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  //setTime(2);

  //Initial clock set
  //     hr, min, sec, day, month, year
  setTime(0, 0,  0,  0,   0,     2016);  //Initializes the time to current time


}

void loop() {

  //***Clock setup
  //Sets hour, min, sec from internal clock
  timeH = hour();
  timeS = second();
  timeM = minute();

  //Changes to 12hr clock
  time12 = timeH;
  if (timeH > 12) {
    time12 = timeH - 12;
  }
  else {
    time12 = timeH;
  }

  //Fixes time for midnight
  if (timeH == 0) {
    time12 = 12;
  }

  //***Printing clock

  lcd.setCursor(0, 0); //Start at character 0 on line 0
  lcd.print("Time");
  lcd.setCursor(0, 1);
  lcd.print(time12);
  lcd.setCursor(2, 1);
  lcd.print(":");
  lcd.setCursor(3, 1); //Start at character 4 on line 2
  lcd.print(timeM);
  lcd.setCursor(5, 1);
  lcd.print(":");
  lcd.setCursor(6, 1); //Start at character 4 on line 2
  lcd.print(timeS);
  delay(50); //Assures I can see the number for a fraction of a second minimum

  // Reset screen every so 5min
  counter = counter + 1;

  if (counter > 1000) {
    counter = 0;
    lcd.begin(16, 2);
    setTime(timeH, timeM, timeS, 1, 1, 2016);
  }

  // Print time to serial
  Serial.print("Time: ");
  Serial.print(timeH);
  Serial.print(":");
  Serial.print(timeM);
  Serial.print(":");
  Serial.print(timeS);
  Serial.println("");

  //***Changing lights
  //Turns lights on for 18hrs a day
  if (timeH > 6) {

    digitalWrite(dirtLight, HIGH);
    digitalWrite(hydroLight, HIGH);
    digitalWrite(testLight1, HIGH);
    digitalWrite(testLight2, HIGH);
  }

  //Turns lights off for 6hrs a day
  else {
    digitalWrite(dirtLight, LOW);
    digitalWrite(hydroLight, LOW);
    digitalWrite(testLight1, LOW);
    digitalWrite(testLight2, LOW); }


    //***Find distance from ultrasonic
    digitalWrite(trigPin, LOW);
    delayMicroseconds(5);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    pinMode(echoPin, INPUT);
    duration = pulseIn(echoPin, HIGH);

    // convert the time into a distance
    cm = (duration / 2) / 29.1;
    inches =  (duration / 2) / 74;

    //Prints all to serial
    Serial.print(cm);
    Serial.print("cm");
    Serial.println();


    //** quick override to turn on all lights
    if (digitalRead(overrideSwitch) == 1) {

      //Turns on all lights during override mode
      digitalWrite(dirtLight, HIGH);
      digitalWrite(hydroLight, HIGH);
    }

    //***Editing clock
    //Only checks for joystick if trigger has been triggered
    if (cm < triggerDistance) {
      Serial.println("Hand detected");
      consequitive = consequitive + 1;
      
      // reads the value of the xjoystick
      yJoystick = abs(1000 - analogRead(joyPin1));
      xJoystick = abs(990 - analogRead(joyPin2));
      Serial.print("Y:");
      Serial.print(xJoystick);
      Serial.print("  X:");
      Serial.println(yJoystick);

      //Adjust minutes of clock up
      if (yJoystick > joyHigh) {
        //Get ready to change min by clearing
        lcd.setCursor(3, 1);
        lcd.print("  ");
        if (consequitive > 4) {
          adjustTime(clockMinute);
        }
        else {
        adjustTime(clockMinute); }
        
        Serial.println("minute raised");
      }

      //Adjust minutes of clock down
      if (yJoystick < joyLow) {
        //Get ready to change min by clearing
        lcd.setCursor(3, 1);
        lcd.print("  ");
        if (consequitive > 4) {
          adjustTime(-clockMinute); }
        else { 
        adjustTime(-clockMinute);}
        
        Serial.println("minute lowered");
      }

      //Adjust hours of clock up
      if (xJoystick < joyLow) {
        //Get ready to change hour by clearing
        lcd.setCursor(0, 1);
        lcd.print("  ");
        adjustTime(+clockMinute * 60);
        Serial.println("hour raised");
      }

      //Adjust hurs of clock down
      if (xJoystick > joyHigh) {
        //Get ready to change hour by clearing
        lcd.setCursor(0, 1);
        lcd.print("  ");
        adjustTime(-clockMinute * 60);
        Serial.println("hour lowered");
      }
    }
    else {
      consequitive = 0;
    }
  
  delay(100); //Before next update
}
