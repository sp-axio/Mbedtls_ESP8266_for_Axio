/************************************************************
ESP8266_Shield_Demo.h
SparkFun ESP8266 AT library - Demo
Jim Lindblom @ SparkFun Electronics
Original Creation Date: July 16, 2015
https://github.com/sparkfun/SparkFun_ESP8266_AT_Arduino_Library

This example demonstrates the basics of the SparkFun ESP8266
AT library. It'll show you how to connect to a WiFi network,
get an IP address, connect over TCP to a server (as a client),
and set up a TCP server of our own.

Development environment specifics:
  IDE: Arduino 1.6.5
  Hardware Platform: Arduino Uno
  ESP8266 WiFi Shield Version: 1.0

This code is released under the MIT license.

Distributed as-is; no warranty is given.
************************************************************/

//////////////////////
// Library Includes //
//////////////////////
// SoftwareSerial is required (even you don't intend on
// using it).
//#include <SoftwareSerial.h> 
#include "ESP8266WiFi.h"
#include "ESP8266Mbedtls.h"

//////////////////////////////
// WiFi Network Definitions //
//////////////////////////////
// Replace these two character strings with the name and
// password of your WiFi network.
const char mySSID[] = "myssid";
const char myPSK[] = "mypsk";

//////////////////
// HTTP Strings //
//////////////////
const char destServer[] = "example.com";
int mbedtlsPort = 4433;
const String htmlHeader = "HTTP/1.1 200 OK\r\n"
                          "Content-Type: text/html\r\n"
                          "Connection: close\r\n\r\n"
                          "<!DOCTYPE HTML>\r\n"
                          "<html>\r\n";

const String httpRequest = "GET / HTTP/1.1\n"
                           "Host: example.com\n"
                           "Connection: close\n\n";


// errorLoop prints an error code, then loops forever.
void errorLoop(int error)
{
  Serial.print(F("Error: ")); Serial.println(error);
  Serial.println(F("Looping forever."));
  for (;;)
    ;
}

// serialTrigger prints a message, then waits for something
// to come in from the serial port.
void serialTrigger(String message)
{
  Serial.println();
  Serial.println(message);
  Serial.println();
  while (!Serial.available())
    ;
  while (Serial.available())
    Serial.read();
}

void initializeESP8266()
{
  // esp8266.begin() verifies that the ESP8266 is operational
  // and sets it up for the rest of the sketch.
  // It returns either true or false -- indicating whether
  // communication was successul or not.
  // true
  int retVal = esp8266.begin();
  if (retVal != true)
  {
    Serial.println(F("Error talking to ESP8266."));
    errorLoop(retVal);
  }
  // set esp8266 single mode
  retVal = esp8266.setMux(0);
  if(!retVal){
    Serial.println(F("Error talking to ESP8266."));
    errorLoop(retVal);
  }
  Serial.println(F("ESP8266 Shield Present"));
}

void connectESP8266()
{
  // The ESP8266 can be set to one of three modes:
  //  1 - ESP8266_MODE_STA - Station only
  //  2 - ESP8266_MODE_AP - Access point only
  //  3 - ESP8266_MODE_STAAP - Station/AP combo
  // Use esp8266.getMode() to check which mode it's in:
  int retVal = esp8266.getMode();
  if (retVal != ESP8266_MODE_STA)
  { // If it's not in station mode.
    // Use esp8266.setMode([mode]) to set it to a specified
    // mode.
    retVal = esp8266.setMode(ESP8266_MODE_STA);
    if (retVal < 0)
    {
      Serial.println(F("Error setting mode."));
      errorLoop(retVal);
    }
  }
  Serial.println(F("Mode set to station"));

  // esp8266.status() indicates the ESP8266's WiFi connect
  // status.
  // A return value of 1 indicates the device is already
  // connected. 0 indicates disconnected. (Negative values
  // equate to communication errors.)
  retVal = esp8266.status();
  if (retVal <= 0)
  {
    Serial.print(F("Connecting to "));
    Serial.println(mySSID);
    // esp8266.connect([ssid], [psk]) connects the ESP8266
    // to a network.
    // On success the connect function returns a value >0
    // On fail, the function will either return:
    //  -1: TIMEOUT - The library has a set 30s timeout
    //  -3: FAIL - Couldn't connect to network.
    retVal = esp8266.connect(mySSID, myPSK);
    if (retVal < 0)
    {
      Serial.println(F("Error connecting"));
      errorLoop(retVal);
    }
  }
}

void displayConnectInfo()
{
  char connectedSSID[24];
  memset(connectedSSID, 0, 24);
  // esp8266.getAP() can be used to check which AP the
  // ESP8266 is connected to. It returns an error code.
  // The connected AP is returned by reference as a parameter.

  int retVal=0;
  while( retVal <= 0){
     retVal = esp8266.getAP(connectedSSID);
  }
  Serial.print(F("Connected to: "));
  Serial.println(connectedSSID);
  
  // esp8266.localIP returns an IPAddress variable with the
  // ESP8266's current local IP address.
  IPAddress myIP = esp8266.localIP();
  Serial.print(F("My IP: ")); Serial.println(myIP);
}

void mbedtlsClientDemo()
{
  int retVal;
  unsigned char buf[1024] = {0,};

  Serial.println("Start Mbedtls client");
  ESP8266Mbedtls mbedtls;
  
  Serial.println("try to connect to server...");
  retVal = mbedtls.connect(destServer, mbedtlsPort, 0); 
  if( retVal == 1){
     Serial.println("connect success!");
  }
  else{
     Serial.println("connect failed!");
     return;
  }
    
  mbedtls.setupSSL();
   
  retVal = mbedtls.connectSSL();
  if( retVal == 0){
     Serial.println("SSL connect success!");
  }
  else{
     Serial.println("SSL connect failed!");
     return;
  }

  Serial.println("Write to SSL server..");
  retVal = mbedtls.writeSSL();
  if( retVal != 0){
     Serial.println("SSL write failed!");
     return;
  }
  
  mbedtls.readSSL(buf, 1023);
  Serial.println("Read from SSL Server...");
  Serial.println("================== received begin =========");
  Serial.println((char *)buf);
  Serial.println("================== received done =========");
  
  Serial.println("disconnect SSL connection");
  if(mbedtls.connectedSSL()){
     mbedtls.closeSSL();
  }
  mbedtls.stopSSL();
  mbedtls.cleanupSSL();
}

// All functions called from setup() are defined below the
// loop() function. They modularized to make it easier to
// copy/paste into sketches of your own.
void setup() 
{
  // Serial Monitor is used to control the demo and view
  // debug information.
  Serial.begin(115200);
  //serialTrigger(F("Press any key to begin."));

  // initializeESP8266() verifies communication with the WiFi
  // shield, and sets it up.
  initializeESP8266();

  // connectESP8266() connects to the defined WiFi network.
  connectESP8266();

  // displayConnectInfo prints the Shield's local IP
  // and the network it's connected to.
  displayConnectInfo();

  serialTrigger(F("Press any key to connect client."));
  mbedtlsClientDemo();
}

void loop() 
{

}

