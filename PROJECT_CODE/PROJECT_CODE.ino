#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>
File myFile;
SoftwareSerial gprsSerial(10, 11);

//#include <LiquidCrystal.h>
//LiquidCrystal lcd(12, 11, 5, 4, 3,2);
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int chipSelect = 4;   // Open serial communications and wait for port to open:
const int AirValue = 620;   //you need to replace this value with Value_1
const int WaterValue = 310; //you need to replace this value with Value_2
int soilMoistureValue = 0;
int soilmoisturepercent = 0;
int soilN;
int soilP;
int soilK;
int soilpH;
int soilMoisture;
void setup() {
  Serial.begin(115200); // open serial port, set the baud rate to 9600 bps
  gprsSerial.begin(115200);
  //  lcd.begin(16,2);
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Soil parameters:");
  /*while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.print("Initializing SD card...");
  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
  // open the file. note that only one file can be open at a time,so you have to close this one before opening another.

  myFile = SD.open("test.txt", FILE_WRITE);  // if the file opened okay, write to it:
  */

}
void loop() {
  soilN = random(4.48, 9.68);
  soilP = random(0.59, 154.6);
  soilK = random(18.00, 2037.0);
  soilpH = random(4.48, 9.68);
  soilMoisture = random(5.5, 6.5);
  Serial.print(soilN);
  Serial.print(",");
  Serial.print(soilP);
  Serial.print(","); 
  Serial.print(soilK);
   Serial.print(","); 
  Serial.print(soilpH);
  Serial.print(",");
  Serial.println("soilMoisture");
  soilMoistureValue = analogRead(A0);  //put Sensor insert into soil
  //Serial.println(soilMoistureValue);
  soilmoisturepercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100);
  Serial.print(soilmoisturepercent);
  //if(soilmoisturepercent >= 100)

  if (soilmoisturepercent > 0 && soilmoisturepercent < 100)
  {
    Serial.print(soilmoisturepercent);
    Serial.println("%");
    lcd.setCursor(0, 1);
    lcd.print("moisture:");
    lcd.setCursor(10, 1);
    lcd.print(soilmoisturepercent);
    lcd.print("%");
    delay(5000);
    //lcd.clear();

  }
  delay(5000);
  
if (myFile) {
  Serial.print("Writing to test.txt...");
  myFile.println("This is a test file :)");
  myFile.println("testing 1, 2, 3.");
  for (int i = 0; i < 20; i++) {
    myFile.println(i);
  }
  // close the file:
  myFile.close();
  Serial.println("done.");
} else {
  // if the file didn't open, print an error:
  Serial.println("error opening test.txt");
}

/*void loop() {
  // nothing happens after setup
  }*/

  //GSM
   if (gprsSerial.available())
    Serial.write(gprsSerial.read());

  gprsSerial.println("AT");
  delay(2000);

  gprsSerial.println("AT+CPIN?");
  delay(2000);

  gprsSerial.println("AT+CREG?");
  delay(2000);

  gprsSerial.println("AT+CGATT?");
  delay(2000);

  gprsSerial.println("AT+CIPSHUT");
  delay(2000);
   gprsSerial.println("AT+CIPSTATUS");
  delay(2000);

  gprsSerial.println("AT+CIPMUX=0");
  delay(2000);

  ShowSerialData();

  gprsSerial.println("AT+CSTT=\"internet\"");//start task and setting the APN,
  delay(2000);

  ShowSerialData();

  gprsSerial.println("AT+CIICR");//bring up wireless connection
  delay(3000);

  ShowSerialData();

  gprsSerial.println("AT+CIFSR");//get local IP adress
  delay(2000);

  ShowSerialData();

  gprsSerial.println("AT+CIPSPRT=0");
  delay(3000);

  ShowSerialData();
  gprsSerial.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");//start up the connection
  delay(6000);

  ShowSerialData();

  gprsSerial.println("AT+CIPSEND");//begin send data to remote server
  delay(4000);
  ShowSerialData();

  String str="GET https://api.thingspeak.com/update?api_key=SHAVIZ13RA6VRK3K&field1=" + String(soilmoisturepercent)+"&field2="+String(soilpH)+"&field3="+String(soilP)+"&field4="+String(soilK);
  Serial.println(str);
  gprsSerial.println(str);//begin send data to remote server

  delay(4000);
  ShowSerialData();

  gprsSerial.println((char)26);//sending
  delay(5000);//waitting for reply, important! the time is base on the condition of internet
  gprsSerial.println();

  ShowSerialData();

  gprsSerial.println("AT+CIPSHUT");//close the connection
  delay(1000);
  ShowSerialData();
  }
  void ShowSerialData()
  {
  while(gprsSerial.available()!=0)
  Serial.write(gprsSerial.read());
  delay(5000);

  }
  
