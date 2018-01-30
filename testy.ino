//

#include <ESP8266WiFi.h>
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
WiFiServer server(80);//Service Port


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
  time_elapce++;
  
  if(time_elapce == 1000000 ) {
    time_elapce = 0;

    time_t now = time(nullptr);
    char *czas = ctime(&now);
    temper = getTemperature();
    //Serial.print("Temperatura -");
    Serial.print(czas);
    //Serial.print(" - wynosi: ");
    Serial.println(temper);
    Serial.println("");
    
    
    temp[n] = temper;
    h[n] = (czas[11] - '0')*10 + czas[12] - '0';
    m[n] = (czas[14] - '0')*10 + czas[15] - '0';
    s[n] = (czas[17] - '0')*10 + czas[18] - '0';
    b[n] = digitalRead(button);
    Serial.println(b[n]);
    n++;

    if(n == BUF) n = 0;
  
    digitalWrite(ledPin, HIGH);
    digitalWrite(ledR, LOW);
    digitalWrite(ledG, LOW);
    digitalWrite(ledB, LOW);
    digitalWrite(ledR, LOW);
        if( temper < tempLBase) {
           if( temper < tempL) { sendSMS(temper); tempL = temper - delta; }
           digitalWrite(ledB, HIGH);
        } else if( temper < tempHBase ) {
          digitalWrite(ledG, HIGH);
          tempL = tempLBase;
          tempH = tempHBase;
        } else {
          if( temper > tempH) { sendSMS(temper); tempH = temper + delta; }
            digitalWrite(ledR, HIGH);
        }

       //if(digitalRead(button) == 0) { sendSMS(temper);}
   
   
}


 
  // Check if a client has connecteddigitalWrite(ledB, HIGH);
  WiFiClient client = server.available();
  if (!client) {
    return;
  }


// Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
   
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  for(int i = 33; i < (request.length()-9); ++i) {
    Serial.println(request[i]);
  }
  Serial.println(request);
  client.flush();

   
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");

   client.println("<head>");
    client.println("<script type='text/javascript' src='https://www.google.com/jsapi'></script>");
    client.println("<script type='text/javascript'>");
      client.println("google.load('visualization', '1', {packages:['corechart', 'controls']});");
      client.println("google.setOnLoadCallback(drawChart);");
      client.println("function drawChart() {");
        client.println("var data = new google.visualization.DataTable();");
          client.println("data.addColumn('timeofday', 'Czas');");
          client.println("data.addColumn('number', 'Temperatura');");
          client.println("data.addColumn({type:'string', role:'annotation'}); ");

bool in=0;
          client.println("data.addRows([");
           
          for(int i = n; (temp[i] != 0) && (i<BUF) ; ++i) {
           //client.print("jestem w jedynce i n wynosi");
           //client.println(n);
             if(i != n) client.print(",");
            client.print("[");

client.print("[");
            client.print(h[i]);
            client.print(",");
            client.print(m[i]);
            client.print(",");
            client.print(s[i]);
client.print("]");
            client.print(",");
            client.print(temp[i]);
            client.print(",");
           if(b[i] == 0) { client.print("'A'");} else {client.print("null");};
           client.print("]");
           client.println("");
           in = 1;
          }
          
          for(int i = 0; (temp[i] != 0) && (i<n) ; ++i) {
            if((in == 0) && (i != 0)) { client.print(", "); } else if(in == 1) { client.print(", "); }
            client.print("[");

client.print("[");
            client.print(h[i]);
            client.print(",");
            client.print(m[i]);
            client.print(",");
            client.print(s[i]);
client.print("]");
            client.print(",");
            client.print(temp[i]);
            client.print(",");
           if(b[i] == 0) { client.print("'A'");} else {client.print("null");};
           client.print("]");
           client.println("");
          }
          //Serial.println("[[8,30,30],  22.34, null],");
          //client.println("[[8,30,31],  22.45,'a'],");
          //client.println("[[8,30,32], 25.13, null]");
          //client.println("['2007', 25.26],");
          //client.println("['2008', 25.89],");
          //client.println("['2009', 25.78],");
          //client.println("['2010', 25.3],");
          //client.println("['2011',  23.4]");
        client.println("]);");
 
        client.println("var options = {");
          client.println("annotations: { textStyle: {fontName: 'Times-Roman',     fontSize: 28, bold: true, italic: true,      color: 'black',      opacity: 0.8    }  },");
        client.println("hAxis: { title: 'Godzina [hh:mm]' },");
        client.println("vAxis: { title: 'Temperatura [*C]' }");
        client.println("};");
 
        client.println("var chart = new google.visualization.LineChart(document.getElementById('chart_div'));");
        client.println("chart.draw(data, options);");
      client.println("}");
    client.println("</script>");
  client.println("</head>");
  client.println("<body>");
    client.println("<div id='chart_div' style='width: 900px; height: 500px;'></div>");
///////////////////////////////////////
    client.println("<form action='/action_page.php'>");
    client.println("<p>Co zrobiono:</p>");
    client.println("<input style='height:100px;width:600px' type='text' name='co_zrobiono'><br>");
    client.println("<input type='submit' value='Submit'>");
    client.println("</form>");



//    client.println("<form action='/\' method='get'>");
//    client.println("<button style='height:300px;width:600px'>Odśwież</button>");

  client.println("</body>");
  
  /*client.print("Led pin is now: ");
  float d=67.45;
  client.println("<br>");
  client.print("Button is: ");
  client.print(digitalRead(button));

  client.println("<br>");
  client.print("LDR =  ");
  client.print(analogRead(LDR));

 
  client.println("<br><br>");
 
 client.println("Click <a href=\"/\">here</a> to reload page <br><br>");
 // getTemperature();
            //client.println("<head></head><body><h1>ESP8266 - Temperature</h1><h3>Temperature in Celsius: ");
             client.print("Temperatura in *C = ");
            client.print(getTemperature() );
            //client.println("*C</h3><h3>Temperature in Fahrenheit: ");
            client.println("<br>");
            client.print("Temperatura in *F = ");
            client.println(temperatureFString);
*/
 
  client.println("</html>");
 
  delay(1);
  Serial.println("Client disconnected");
  Serial.println("");
}
