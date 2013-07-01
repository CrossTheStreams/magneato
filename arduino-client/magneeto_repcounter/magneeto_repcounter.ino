/*
  Repeating Wifi Web client
 
 This sketch connects to a a web server and makes a request
 using an Arduino Wifi shield.
 
 Circuit:
 * Wifi shield attached to pins 10, 11, 12, 13
 
 created 23 April 2012
 modifide 31 May 2012
 by Tom Igoe
 
 http://arduino.cc/en/Tutorial/WifiWebClientRepeating
 This code is in the public domain.
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
//char server[] = "hautaulogy.local";
char server[] = "192.168.1.137";
//IPAddress server(64,131,82,241);

unsigned long lastConnectionTime = 0;           // last time you connected to the server, in milliseconds
boolean lastConnected = false;                  // state of the connection last time through the main loop
const unsigned long postingInterval = 5*1000;  // delay between updates, in milliseconds

// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 accelgyro;

int16_t ax, ay, az;
int rep_state;

boolean led_state;
int led_pin = 9;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600); 
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  
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
    delay(10000);
  } 
  // you're connected now, so print out the status:
  printWifiStatus();
  
  // set up for accel
  // join I2C bus (I2Cdev library doesn't do this automatically)
    Wire.begin();
     // initialize device
    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();

    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(accelgyro.testConnection() ? "MPU9150 connection successful" : "MPU9150 connection failed");
    
    //magneeto custom stuff
    // todo: add in LPF for accel (can I do that on the MPU9150
    rep_state = 0;
    
    pinMode(led_pin, OUTPUT);
    led_state = HIGH;
    digitalWrite(led_pin, led_state);   
}

void loop() {
  // read raw accel/gyro measurements from device
  accelgyro.getAcceleration(&ax, &ay, &az);
    
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
        Serial.println("rep");
        if(!client.connected()) {
          httpRequest();
        }
    }
  }
  
  // if there's incoming data from the net connection.
  // send it out the serial port.  This is for debugging
  // purposes only:
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }

  // if there's no net connection, but there was one last time
  // through the loop, then stop the client:
  if (!client.connected() && lastConnected) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
    led_state = !led_state;
    digitalWrite(led_pin, led_state);
  }

  // if you're not connected, and ten seconds have passed since
  // your last connection, then connect again and send data:
  //if(!client.connected() && (millis() - lastConnectionTime > postingInterval)) {
  //  httpRequest();
  //}
  // store the state of the connection for next time through
  // the loop:
  lastConnected = client.connected();
}

// this method makes a HTTP connection to the server:
void httpRequest() {
  // if there's a successful connection:
  if (client.connect(server, 3000)) {
    Serial.println("connecting...");
    // send the HTTP PUT request:
    client.println("GET /counter/ping HTTP/1.1");
    client.println("Host: 192.168.1.124");
    //client.println(serverIP);
    client.println("Connection: close");
    client.println();

    // note the time that the connection was made:
    lastConnectionTime = millis();
  } 
  else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
    Serial.println("disconnecting.");
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






