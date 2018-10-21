#include <Wire.h>
#include <EEPROM.h>
#include <dht11.h>
////load 
// Enter you own analog values here 

int addr=0;
int counter=1;
float loadA = 10; // kg
int analogvalA = 200; // analog reading taken with load A on the load cell

float loadB = 30; // kg
int analogvalB = 600; // analog reading taken with load B on the load cell

// Upload the sketch again, and confirm, that the kilo-reading from the serial output now is correct, using your known loads
#define load_pin 2
float analogValueAverage = 0;

// How often do we do readings?
long time = 0; //
int timeBetweenReadings = 2000; // We want a reading every 200 ms;
int on=0;
////load
#include <LiquidCrystal.h>
dht11 DHT11;

#define DHT11PIN 2
float a=1.0;
float flag=0.0;
int sum2=0,counter1=0;
long accelX, accelY, accelZ;
float gForceX, gForceY, gForceZ;
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);        // select the pins used on the LCD panel

// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

int read_LCD_buttons(){               // read the buttons
    adc_key_in = analogRead(0);       // read the value from the sensor 

    // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
    // we add approx 50 to those values and check to see if we are close
    // We make this the 1st option for speed reasons since it will be the most likely result

    if (adc_key_in > 1000) return btnNONE; 

    // For V1.1 us this threshold
    if (adc_key_in < 50)   return btnRIGHT;  
    if (adc_key_in < 250)  return btnUP; 
    if (adc_key_in < 450)  return btnDOWN; 
    if (adc_key_in < 650)  return btnLEFT; 
    if (adc_key_in < 850)  return btnSELECT;  


    return btnNONE;                // when all others fail, return this.
}

unsigned long tepTimer ;




long gyroX, gyroY, gyroZ;
float rotX, rotY, rotZ;
int i;
int sum=0;
float sum1=0.0;
//Celsius to Fahrenheit conversion
double Fahrenheit(double celsius)
{
  return 1.8 * celsius + 32;
}


//Celsius to Kelvin conversion
double Kelvin(double celsius)
{
  return celsius + 273.15;
}




void setup() {
   lcd.begin(16, 2);                       // start the library
   pinMode(1, INPUT); //rcvs signal
  //pinMode(A3, OUTPUT); //GND PIN
  //pinMode(A2, OUTPUT); //VCC PIN
  //digitalWrite(A2, HIGH); //
  //digitalWrite(A3, LOW); //connect sensor gnd to arduino gnd
  Serial.begin(115200);
  
  Serial.println("DHT11 TEST PROGRAM ");
  Serial.print("LIBRARY VERSION: ");
  Serial.println(DHT11LIB_VERSION);
  Serial.println();
  Wire.begin();
   
  lcd.setCursor(0,1);
  setupMPU();
}


void loop() {
  recordAccelRegisters();
  recordGyroRegisters();
                // set the LCD cursor   position 

  
 // analogRead(A0));
  //delay(100);
  display_btn();
  if(on==1){
 

  if(analogRead(1)<500)
  {
    
    i++;
    Serial.println(i);
    sum1=sum1+(2*3.1416*.1); //distance
    sum=sum+2;
     Serial.println("Stop something is ahead!!");
     Serial.print("The primary fare is ");
     Serial.println(sum);

     Serial.println("Distance is ");
     Serial.println(sum1);
     lcd.setCursor(0, 0);   
      printData();
    
   
  }
  else 
  {
     Serial.println("Path is clear");
   
  }
}
 
  
 // delay(500);
}

void display_btn(){
  
  
  // lcd.print(millis()/1000);       // display seconds elapsed since power-up

   lcd.setCursor(0,0);             // move to the begining of the second line
   lcd_key = read_LCD_buttons();   // read the buttons

   switch (lcd_key){               // depending on which button was pushed, we perform an action

       case btnRIGHT:{             //  push button "RIGHT" and show the word on the screen
            lcd.print("Agree ");
            on=0;
            EEPROM.write(addr,sum1);
            EEPROM.write(addr+1,sum2);
            EEPROM.write(addr+2,1);
            addr=addr+3;
           // Serial.println("pppppppppppppppppppppp");
            Serial.println(addr);
            delay(1500);
            break;
       }
       case btnLEFT:{
             lcd.print("Disagree   "); //  push button "LEFT" and show the word on the screen
           on=0;
      EEPROM.write(addr,sum1);
            EEPROM.write(addr+1,sum2);
            EEPROM.write(addr+2,0);
            addr=addr+3;
            // Serial.println("pppppppppppppppppppppp");
            Serial.println(addr);
              delay(1500);
             break;
       }    
       case btnUP:{
      //  lcd.setCursor(6,1); 
        //    lcd.print(sum);
         lcd.setCursor(0,0);  
         lcd.clear();
             lcd.print("Fare:0   ");  //  push button "UP" and show the word on the screen
            
             on=1;
             sum2=0;
             counter=1;
               counter1=0;
             break;
       }
       case btnDOWN:{
             lcd.setCursor(0,0);  
         lcd.clear();
            // lcd.print("Fare:0   ");  //  push button "UP" and show the word on the screen
            counter=1;
             counter1=0;
             on=0;
             sum2=1;
             break;
       }
       case btnSELECT:{
             //lcd.print("SELECT");  //  push button "SELECT" and show the word on the screen
             if(on==1){
             if(counter<3){
             counter++;
             sum2=sum2+5;
               if(millis() - tepTimer > 1000){         // output a temperature value per 500ms 
    
             tepTimer = millis();
                 lcd.print("Fare: ");
         lcd.print(sum2);


         
         delay(500);
   }
 
             
             

             }

             } 

              
             break;
       }
       case btnNONE:{
        lcd.setCursor(0,1);
            // lcd.print("NONE  ");  //  No action  will show "None" on the screen
            
             if(on==1){
              if(counter==1){
                lcd.print("Passenger No.: ");    //showing passengers number
              lcd.print(counter);
                }
         else if(counter==2){
            lcd.print("Passenger No.: ");    //showing passengers number
              lcd.print(counter);
              }
             else if(counter==3){
               lcd.print("Passenger No.: ");    //showing passengers number
  lcd.print(counter);
              }

             } 
            

             break;
       }
   }
  
  }

void setupMPU(){
  Wire.beginTransmission(0b1101000); //This is the I2C address of the MPU (b1101000/b1101001 for AC0 low/high datasheet sec. 9.2)
  Wire.write(0x6B); //Accessing the register 6B - Power Management (Sec. 4.28)
  Wire.write(0b00000000); //Setting SLEEP register to 0. (Required; see Note on p. 9)
  Wire.endTransmission();  
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x1B); //Accessing the register 1B - Gyroscope Configuration (Sec. 4.4) 
  Wire.write(0x00000000); //Setting the gyro to full scale +/- 250deg./s 
  Wire.endTransmission(); 
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x1C); //Accessing the register 1C - Acccelerometer Configuration (Sec. 4.5) 
  Wire.write(0b00000000); //Setting the accel to +/- 2g
  Wire.endTransmission(); 
}

void recordAccelRegisters() {
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x3B); //Starting register for Accel Readings
  Wire.endTransmission();
  Wire.requestFrom(0b1101000,6); //Request Accel Registers (3B - 40)
  while(Wire.available() < 6);
  accelX = Wire.read()<<8|Wire.read(); //Store first two bytes into accelX
  accelY = Wire.read()<<8|Wire.read(); //Store middle two bytes into accelY
  accelZ = Wire.read()<<8|Wire.read(); //Store last two bytes into accelZ
  processAccelData();
}

void processAccelData(){
  gForceX = accelX / 16384.0;
  gForceY = accelY / 16384.0; 
  gForceZ = accelZ / 16384.0;
}

void recordGyroRegisters() {
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x43); //Starting register for Gyro Readings
  Wire.endTransmission();
  Wire.requestFrom(0b1101000,6); //Request Gyro Registers (43 - 48)
  while(Wire.available() < 6);
  gyroX = Wire.read()<<8|Wire.read(); //Store first two bytes into accelX
  gyroY = Wire.read()<<8|Wire.read(); //Store middle two bytes into accelY
  gyroZ = Wire.read()<<8|Wire.read(); //Store last two bytes into accelZ
  processGyroData();
}

void processGyroData() {
  rotX = gyroX / 131.0;
  rotY = gyroY / 131.0; 
  rotZ = gyroZ / 131.0;
}

void printData() {


Serial.println("\n");

  int chk = DHT11.read(DHT11PIN);

 // Serial.print("Read sensor: ");
  switch (chk)
  {
    case DHTLIB_OK: 
    Serial.println("OK"); 
    break;
    case DHTLIB_ERROR_CHECKSUM: 
    Serial.println("Checksum error"); 
    break;
    case DHTLIB_ERROR_TIMEOUT: 
    Serial.println("Time out error"); 
    break;
    default: 
    Serial.println("Unknown error"); 
    break;
  }
   if((float)DHT11.humidity>80 && counter1==0){ //increment for humidity
    sum2=sum2+2;
    lcd.print("Fare: ");
   
         lcd.print(sum2);

    
    }
    counter1++;

  Serial.print("Humidity (%): ");
  Serial.println((float)DHT11.humidity, 2);

  Serial.print("Temperature (°C): ");
  Serial.println((float)DHT11.temperature, 2);

//  Serial.print("Temperature (°F): ");
//  Serial.println(Fahrenheit(DHT11.temperature), 2);
//
//  Serial.print("Temperature (°K): ");
//  Serial.println(Kelvin(DHT11.temperature), 2);
////load
 int analogValue = analogRead(load_pin);

  // running average - We smooth the readings a little bit
  analogValueAverage = 0.99*analogValueAverage + 0.01*analogValue;

  // Is it time to print?
  if(millis() > time + timeBetweenReadings){
    float load = analogToLoad(analogValueAverage);

    Serial.print("analogValue: ");
    Serial.println(analogValueAverage);
    Serial.print("             load: ");
    Serial.println(load,2);
    time = millis();
  }
////load
Serial.print(" Y=");
Serial.print(gForceY);
Serial.print("\n");
if(gForceY>=-0.15 && gForceY<=0.15){
flag=0;
//Serial.println(flag);
}
if(gForceY>=-.30 && gForceY<=-0.16){

flag=1.1;
//min1++;
//Serial.println(flag);
}
if(gForceY>=-0.60 && gForceY<=-0.31){
flag=1.2;
//min2++;
//Serial.print(flag);
}
if(gForceY>=0.16 && gForceY<=.30){
//Serial.println(flag);
flag=1.3;
//max1++;
}
if(gForceY>=0.31 && gForceY<=0.45){
//Serial.println(flag);
flag=1.4;
//max2++;
}
if(gForceY>=.46 && gForceY<=0.60){
//Serial.println(flag);
flag=1.5;
//max3++;
}

sum2=sum2+2+(flag*a);  //total vara
 
  Serial.println(flag);
  Serial.println("\n");
  Serial.print("Total fare: ");
Serial.println(sum2);
   if(millis() - tepTimer > 1000){         // output a temperature value per 500ms 
         lcd.setCursor(0,0);
             tepTimer = millis();
                 lcd.print("Fare: ");
         lcd.print(sum2);


         
         delay(1000);
   }
 
//  
//  Serial.print("Gyro (deg)");
//  Serial.print(" X=");
//  Serial.print(rotX);
//  Serial.print(" Y=");
//  Serial.print(rotY);
//  Serial.print(" Z=");
//  Serial.print(rotZ);
//  Serial.print(" Accel (g)");
//  Serial.print(" X=");
//  Serial.print(gForceX);
//  Serial.print(" Y=");
//  Serial.print(gForceY);
//  Serial.print(" Z=");
//  Serial.println(gForceZ);
}


////load
float analogToLoad(float analogval){

  // using a custom map-function, because the standard arduino map function only uses int
  float load = mapfloat(analogval, analogvalA, analogvalB, loadA, loadB);
  return load;
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
////load
