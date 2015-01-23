#include <SPI.h>
#include <Wire.h>
#include <SD.h>

File myFile;
int sensorPin = A0;  


 union u_temp {
   byte wire[4];
   float temp;
 } temperature;
   
char buffer[10];
int tempPin = A0;
char tempString[10];

void setup()
{
  pinMode(10, OUTPUT);
  digitalWrite(10, LOW);
 
if (!SD.begin(10)) {
     pinMode(13, OUTPUT);
     digitalWrite(13, HIGH);
     delay(1000);  
     digitalWrite(13, LOW);
    return;
  }
  temperature.temp = 0;
  Wire.begin(9);//start i2c as a slave
  Wire.onReceive(receiveEvent); 
  Wire.onRequest(requestEvent);
}



void loop()
{
  int tempRead = analogRead(tempPin);
    float tempVolt = float(tempRead) / 310; //3.1 Volts per division
    float tempF = ((100 * tempVolt) - 50)*1.8+32; // From TMP36 Datasheet, then C -> F conversion
    dtostrf(tempF, 8, 2, tempString);


  delay(1000); 
  pinMode(10, OUTPUT);
  myFile = SD.open("test.txt", FILE_WRITE);
  if (myFile) {
    dtostrf(temperature.temp, 5, 1, buffer);
    myFile.print(buffer);
    myFile.print(",");
    myFile.println(tempString);
    myFile.close();
  } 
  myFile = SD.open("data.txt");
  if (myFile) {
    myFile.close();
  } 
  
}

void receiveEvent(int howMany) {
  for(int i = 0; i < howMany; i++){
    temperature.wire[i]=Wire.receive();   
  }
 
}

void requestEvent(){
 for(int i = 0; i < sizeof(tempString); i++){
     Wire.write(tempString[i]);    
  }
}
