#include <SPI.h>
#include <Wire.h>


/*
  SD card read/   Wire.begin(9);                // Start I2C Bus as a Slave (Device Number 9)write
 
 This example shows how to read and write data to and from an SD card file  
 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4
 
 created   Nov 2010
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
 
 This example code is in the public domain.
     
 */
 
#include <SD.h>

File myFile;
int sensorPin = A0;  
//float sensorValue;

 union u_temp {
     byte wire[4];
     float temp;
   } temperature;
   
   char buffer[10];
   int tempPin = A0;
char tempString[10];

void setup()
{
      //pinMode(13, OUTPUT);
 //digitalWrite(13, HIGH);
 // Open serial communications and wait for port to open:
 // Serial.begin(9600);
  // while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
 // }
   pinMode(10, OUTPUT);
 digitalWrite(10, LOW);
 
if (!SD.begin(10)) {
   // Serial.println("initialization failed!");
     pinMode(13, OUTPUT);
     digitalWrite(13, HIGH);
     delay(1000);  // do not print too fast!
     digitalWrite(13, LOW);
    return;
  }
temperature.temp = 0;
    
  Wire.begin(9);                // Start I2C Bus as a Slave (Device Number 9)
  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(requestEvent);
}



void loop()
{
  int tempRead = analogRead(tempPin);
    float tempVolt = float(tempRead) / 310.3;
    float tempF = ((100 * tempVolt) - 50)*1.8+32;
    dtostrf(tempF, 8, 2, tempString);


   delay(1000);  // do not print too fast!
    // nothing happens after setup
    

  //Serial.print("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin 
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output 
  // or the SD library functions will not work. 
   pinMode(10, OUTPUT);
   
  
  //Serial.println("initialization done.");
  
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("test.txt", FILE_WRITE);
  
  // if the file opened okay, write to it:
  if (myFile) {
    //Serial.print("Writing to test.txt...");
    //myFile.println("testing 1, 2, 3.");
    
     dtostrf(temperature.temp, 5, 1, buffer);

    myFile.print(buffer);
    myFile.print(",");
     myFile.println(tempString);
    // close the file:
    myFile.close();
    //Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    //Serial.println("error opening test.txt");
  }
  
  // re-open the file for reading:
  myFile = SD.open("test.txt");
  if (myFile) {
    //Serial.println("test.txt:");
    
    // read from the file until there's nothing else in it:
   // while (myFile.available()) {
        //Serial.write(myFile.read());
    //}
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    //Serial.println("error opening test.txt");
  }
 
  
  
}

void receiveEvent(int howMany) {
  for(int i = 0; i < howMany; i++){
    temperature.wire[i]=Wire.receive();    // receive byte as an integer
  }
 
}

void requestEvent(){
 for(int i = 0; i < sizeof(tempString); i++){
     Wire.write(tempString[i]);    // receive byte as an integer
  }
}
