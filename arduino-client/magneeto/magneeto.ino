/*
  Magneeto Rep Counter and web client
 
 This sketch connects to a a web server and makes a request
 using an Arduino Wifi shield. It also connects to an MPU-9150 
 and uses the acceleration values (from only the z axis) to 
 determine when an exercise rep is being done. A post notification
 is sent to a server when a rep is recognized.
 
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

unsigned long requestSentTime = 0;

unsigned long lastConnectionTime = 0;           // last time you connected to the server, in milliseconds
boolean lastConnected = false;                  // state of the connection last time through the main loop
const unsigned long postingInterval = 10*1000;  // delay between updates, in milliseconds

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600); 
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
    delay(10000);
  } 
  // you're connected now, so print out the status:
  printWifiStatus();
}

void loop() {
  // if there's incoming data from the net connection.
  // send it out the serial port.  This is for debugging
  // purposes only:
  //Serial.println("Attempting to read from client...");
  
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }
  
  /*
  // time between request sent and response received
  unsigned long duration = millis() - requestSentTime;
  char strDuration[5];
  
  char str[100] = "POST Request took ";
  strcat(str, ltoa(duration, strDuration, 5));
  
  Serial.println(str);*/

  // if there's no net connection, but there was one last time
  // through the loop, then stop the client:
  if (!client.connected() && lastConnected) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
  }

  // if you're not connected, and ten seconds have passed since
  // your last connection, then connect again and send data:
  if(!client.connected() && (millis() - lastConnectionTime > postingInterval)) {
    httpRequest();
  }
  // store the state of the connection for next time through
  // the loop:
  lastConnected = client.connected();
}

// this method makes a HTTP connection to the server:
void httpRequest() {
  // if there's a successful connection:
  
  if (client.connect(server, serverPort)) {
    Serial.println("\nconnecting...");
    // send the HTTP PUT request:
    client.println("GET /counter/ping HTTP/1.1");
    char hostStr[] = "Host: ";
    strcat(hostStr, serverIP);
    client.println(hostStr);
    client.println("User-Agent: arduino-ethernet");
    //client.println("Content-Type: application/x-www-form-urlencoded");
    //client.println("Name=Jonathan+Doe&Age=23&Formula=a+%2B+b+%3D%3D+13%25%21");
    client.println("Connection: close");
    client.println();

    // note the time that the connection was made:
    lastConnectionTime = millis();
    requestSentTime = millis();
    Serial.println("Sent request to server");
  } 
  else {
    // if you couldn't make a connection:
    char str[200] = "connection to ", strPort[9];
    itoa(serverPort, strPort, 10);
    strcat(str, server);
    strcat(str, " on port ");
    strcat(str, strPort);
    strcat(str, " failed");
    strcat(str, "...disconnecting");
    
    Serial.println(str);
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






