#include <Wire.h>
//#include <Time.h>
#include <TimeLib.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

int overrideSwitch = 22;
int dirtLight = 33;
int hydroLight =31;
int testLight1 = 4;
int testLight2 = 5;
int timeH;
int timeM;
int timeS;
int time12;
int counter = 0;
int currentTime;
int joyPin1 = 0;            
int joyPin2 = 1;        
int xJoystick = 0;                 
int yJoystick = 0;                  
int clockMinute = 60;
int joyHigh = 800;
int joyLow 200;

void setup() {
  
  Serial.begin(9600);
  
  //Display setup  
  lcd.begin(16,2);   // initialize the lcd for 16 chars 2 lines, turn on backlight
  lcd.backlight(); // finish with backlight on 

//***Set pinmodes
  pinMode(dirtLight, OUTPUT);
  pinMode(hydroLight, OUTPUT);
  pinMode(testLight1, OUTPUT);
  pinMode(testLight2, OUTPUT);
  pinMode(overrideSwitch, INPUT);
  setTime(2);

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
    time12 = timeH - 12;}
  else {
    time12 = timeH;}
 
  //Fixes time for midnight
  if (timeH == 0) {
    time12 = 12; }

//***Printing clock

  lcd.setCursor(0,0); //Start at character 0 on line 0
  lcd.print("Time");
  lcd.setCursor(0,1); 
  lcd.print(time12);
  lcd.setCursor(2,1);
  lcd.print(":");
  lcd.setCursor(3,1); //Start at character 4 on line 2
  lcd.print(timeM);
  lcd.setCursor(5,1);
  lcd.print(":");
  lcd.setCursor(6,1); //Start at character 4 on line 2
  lcd.print(timeS);

  // Reset screen every so 5min
  counter = counter + 1; 

  if (counter > 1000) {
    counter = 0;
    lcd.begin(16,2);
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

//***Chaning lights
  //Turns lights on for 18hrs a day
  if (timeH > 6) {
    
    digitalWrite(dirtLight, HIGH);
    digitalWrite(hydroLight,HIGH);
    digitalWrite(testLight1, HIGH);
    digitalWrite(testLight2, HIGH); }

  //Turns lights off for 6hrs a day
  else {
    digitalWrite(dirtLight, LOW);
    digitalWrite(hydroLight, LOW);
    digitalWrite(testLight1, LOW);
    digitalWrite(testLight2, LOW);
    
//***Editing clock
  //Only checks for joystick if trigger has been triggered
  if (digitalRead(overrideSwitch) == 1) {
    digitalWrite(dirtLight, HIGH);
    digitalWrite(hydroLight, HIGH);
  
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
    lcd.setCursor(3,1); 
    lcd.print("  ");
    adjustTime(clockMinute);
    Serial.println("minute raised");}

  //Adjust minutes of clock down
  if (yJoystick < joyLow) {
    //Get ready to change min by clearing
    lcd.setCursor(3,1); 
    lcd.print("  ");
    adjustTime(-clockMinute);
    Serial.println("minute lowered"); }

  //Adjust hours of clock up
  if (xJoystick < joyLow) {
    //Get ready to change hour by clearing
    lcd.setCursor(0,1); 
    lcd.print("  ");
    adjustTime(+clockMinute*60);
    Serial.println("hour raised"); }

  //Adjust hurs of clock down
  if (xJoystick > joyHigh) {
    //Get ready to change hour by clearing
    lcd.setCursor(0,1); 
    lcd.print("  ");
    adjustTime(-clockMinute*60);
    Serial.println("hour lowered"); }
   
  }

  delay(300);
}
