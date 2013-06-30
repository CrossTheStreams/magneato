/*
  Magneeto Rep Counter and web client
 
 This sketch connects to a a web server and makes a request
 using an Arduino Wifi shield. It also connects to an MPU-9150 
 and uses the acceleration values (from only the z axis) to 
 determine when an exercise rep is being done. A post notification
 is sent to a server when a rep is recognized.
 
 Circuit:
 * Wifi shield attached to pins 10, 11, 12, 13
 * MPU-9150 attached to pins A4, A5, D2 (with 3V3 power and gnd)
 
 Based on WiFi demo
 created 23 April 2012
 modifide 31 May 2012
 by Tom Igoe
 
 Updated 2013 June 29 by Boris Joffe and Morgan Redfield
 
 */

#include <SPI.h>
#include <WiFi.h>

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#include "Wire.h"

// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
#include "MPU6050.h"

char ssid[] = "LikeBright";      //  your network SSID (name) 
char pass[] = "LikeBright123";   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

// Initialize the Wifi client library
WiFiClient client;

// server address:
//char server[] = "www.arduino.cc";
//IPAddress server(64,131,82,241);
char server[] = "192.168.1.124";
//char serverIP[] = "www.arduino.cc"
char serverIP[] = "192.168.1.124";

// http://192.168.1.124:3000/counter/ping

//int serverPort = 80;
int serverPort = 3000;
int wifiTTL = 3000;


// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 accelgyro;

int16_t ax, ay, az;
int rep_state;
int totalReps = 0;

int ready_led = 7;
boolean rep;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(38400); 
  /*while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }*/
  
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present"); 
    // don't continue:
    while(true);
  } 
  
  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) { 
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:    
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    // future TODO - make this faster
    delay(wifiTTL);
  } 
  // you're connected now, so print out the status:
  printWifiStatus();
  
  // Rep Counter and Accel setup
  // join I2C bus (I2Cdev library doesn't do this automatically)
  Wire.begin();
  // initialize device
  Serial.println("Initializing I2C devices...");
  
  pinMode(ready_led, OUTPUT);
  // FIX - program stops here and requires reset to continue successfully (INTERMITTENT)
  accelgyro.initialize();

  // verify connection
  Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection() ? "MPU9150 connection successful" : "MPU9150 connection failed");
  Serial.println();
  
  //magneeto custom stuff
  // todo: add in LPF for accel (can I do that on the MPU9150
  // todo: add in calibration for random placement on weight
  rep_state = 0;
  rep = false;
  digitalWrite(ready_led, HIGH);
}

void loop() {
  // read raw accel/gyro measurements from device
  accelgyro.getAcceleration(&ax, &ay, &az);
    
 while (client.available()) {
   client.flush(); 
   char c = client.read();
  }
 
 /*
  // time between request sent and response received
  unsigned long duration = millis() - requestSentTime;
  char strDuration[5];
  
  char str[100] = "POST Request took ";
  strcat(str, ltoa(duration, strDuration, 10));
  
  Serial.println(str);*/
  
  // determine if a rep has been performed
  if (rep_state == 0)
  {
      if (az < 0)
      {
          rep_state = 1; 
      }
  } else if (rep_state == 1)
  {
      if (az < -6500) {
          rep_state = 2; 
       }
    } else // rep_state = 2
    {
        if (az > 0) {
            rep_state = 1;
            rep = true;
            httpRequest();
        }
    }
    
    if (rep && !client.connected()) {
      Serial.println("disconnecting");
      client.stop();
      rep = false;
    }  
}

// this method makes a HTTP connection to the server:
void httpRequest() {
  // if there's a successful connection:
  
  if (client.connect(server, serverPort)) {
    Serial.println("Connecting...");
    // send the HTTP PUT request:
    client.println("GET /counter/ping HTTP/1.1");
    //char hostStr[] = "Host: ";
    //strcat(hostStr, serverIP);
    client.println("Host: 192.168.1.124");
    //client.println(serverIP);
    client.println("Connection: close");
    client.println();
  } 
  else {
    Serial.println("connection failed ... disconnecting");
    client.stop();
  }
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}






