//

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <time.h>
#include "Gsender.h"

#define BUF 100


#define LDR   A0
#define ONE_WIRE_BUS 5


OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);


 

const char* ssid = "Tilgin-xxQdR6SKXhzC";//type your ssid
const char* password = "hYDaRdS84zrh8";//type your password
const char* ssid_A = "AndroidAP";//type your ssid
const char* password_A = "bafk5969";//type your password
 
int ledPin = 2; // inside LED
int ledB = 12; // blue Led
int ledG = 13; // green Led
int ledR = 15; // red Led
int button = 4; // button
int time_elapce = 0;  //time between measuring temperature
ESP8266WebServer server(80);

void handleRoot() {
  int sec = millis() / 1000;
   int min = sec / 60;
   int hr = min / 60;
        char temp[420];
   snprintf ( temp, 400,

"<html>\
  <head>\
    <title>ESP8266 Demo</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>Hello from ESP8266!</h1>\
    <p>Uptime: %02d:%02d:%02d</p>\
    <form action='http://192.168.4.1' method='get'>\
    F_name: <input type='text' name='fname'><br>\
    <input type='submit' value='Submit'>\
</form>\
  </body>\
</html>",

      hr, min % 60, sec % 60
   );
  server.send(200, "text/html", temp);
}



void sendSMS(float temper) {
  Gsender *gsender = Gsender::Instance();    // Getting pointer to class instance
    String subject = "UAWAGA!!!!! ";
    if(gsender->Subject(subject)->Send("+48605450390@text.plusgsm.pl", (String)temper)) {
        Serial.println("Message send.");
    } else {
        Serial.print("Error sending message: ");
        Serial.println(gsender->getError());
    }
}

 
void setup() {
  
  
  DS18B20.begin();
 
  Serial.begin(115200);
  delay(10);
 
  pinMode(LDR, INPUT);
  pinMode(button, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  pinMode(ledB, OUTPUT);
  digitalWrite(ledB, LOW);
 
    pinMode(ledG, OUTPUT);
  digitalWrite(ledG, LOW);

    pinMode(ledR, OUTPUT);
  digitalWrite(ledR, LOW);
  
  // Connect to WiFi network
// Connect to WiFi network from Android
  int z = 1;

  
 
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid_A);
  
    WiFi.begin(ssid_A, password_A);
  
    while (WiFi.status() != WL_CONNECTED && z < 20) {
      delay(500);
      Serial.print(".");
      z++;
   }
    
 if (WiFi.status() != WL_CONNECTED) {
    Serial.println();
    Serial.print("Fail connected to  ");
    Serial.println(ssid_A);
    Serial.println();
  
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
  
    WiFi.begin(ssid, password);
  
    while (WiFi.status() != WL_CONNECTED && z < 30) {
      delay(500);
      Serial.print(".");
      z++;
    }
    z = 1;
  }

 
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Start the server
  server.begin();
server.on("/", handleRoot);
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
  // Connect to time serwer
  configTime(1 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  Serial.println("\nWaiting for time");
  while (!time(nullptr)) {
    Serial.print(".");
    delay(1000);
}
}

float getTemperature() {
    float tempC;
    DS18B20.requestTemperatures();
    tempC = DS18B20.getTempCByIndex(0);
 
    digitalWrite(ledPin, LOW);
    delay(100);
    digitalWrite(ledPin, HIGH);
    return tempC;
}

 
  
   float temper;
   float temp[BUF];
   int n = 0;

   int s[BUF];
   int m[BUF];
   int h[BUF];
   bool b[BUF];

  int tempLBase = 23;
  int tempHBase = 40;
  int tempL = tempLBase;
  int tempH = tempHBase;
  int delta = 2;

 
void loop() {

server.handleClient();

}
